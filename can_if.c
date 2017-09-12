#include "app_include.h"

static int WaitRxAvailable(int fd, uint32_t wait_useconds)
{
    int ret = 0;

    fd_set read_fdset;
    struct timeval timeout;

    FD_ZERO(&read_fdset);
    FD_SET(fd, &read_fdset);

    timeout.tv_sec = 0;
    timeout.tv_usec = wait_useconds;

    do {
        ret = select(fd + 1, &read_fdset, NULL, NULL, &timeout);
    } while (ret < 0 && errno == EINTR);

    if (ret == 0)
        errno = ETIMEDOUT;

    return ret;
}

static int WaitTxAvailable(int fd, uint32_t wait_useconds)
{
    int ret = 0;

    fd_set write_fdset;
    struct timeval timeout;

    FD_ZERO(&write_fdset);
    FD_SET(fd, &write_fdset);

    timeout.tv_sec = 0;
    timeout.tv_usec = wait_useconds;

    do {
        ret = select(fd + 1, NULL, &write_fdset, NULL, &timeout);
    } while (ret < 0 && errno == EINTR);

    if (ret == 0)
        errno = ETIMEDOUT;

    return ret;
}

static int Phy_Init(void)
{
#if defined ZC702
    if (Gpio_Pin_Config(9, 906, "out", 0) < 0) {
        perror("fail to config CAN phy\n");
        return -1;
    }
    usleep(5000);
    if (Gpio_Pin_DeConfig(9, 906)) {
        perror("fail to deconfig CAN phy\n");
        return -1;
    }
    return 0;
#elif defined HMBIRD_MB

#endif
}

/**
 * @ingroup extern
 * @brief send a single can_frame via socketcan
 * @param fd : file descriptor of can socket
 * @param id : can_iendentifier
 * @param *pdata : pointer to can frame payload data buffer
 * @param length : length of payload
 * @param timeout : timeout setting for send
 * @return sizeof(struct can_frame) if success
 * @return negativ if failed
 */
int CAN_SendFrame(int fd, uint32_t id, const uint8_t *pdata, uint8_t length, uint16_t timeout_ms)
{
    if (length > 8) {
        perror("CAN_Send() error: request to send more than 8 bytes\n");
        return -1;
    }

    struct can_frame frame;
    int ret = 0;

    frame.can_id = id;
    memcpy(&frame.data[0], pdata, length);
    frame.can_dlc = length;
    ret = WaitTxAvailable(fd, timeout_ms * 1000);
//    usleep(5000);       // for slow response usb-can adaptor to capture packet
    if(ret <= 0) {
        printf("CAN_Send() wait-available error\n");
        return -1;
    }else {
        if ((ret = write(fd, &frame, sizeof(frame))) < 0)
            perror("cansocket: negative to send out\n");
        return ret;
    }
}

/**
 * @ingroup extern
 * @brief receive a single can_frame via socketcan
 * @param fd : file descriptor of can socket
 * @param *pframe : contrainer of received frame
 * @param timeout : timeout setting for send
 * @return sizeof(struct can_frame) if success
 * @return negativ if failed
 */
int CAN_RecvFrame(int fd, struct can_frame *pframe, uint16_t timeout_ms)
{
    int ret = 0;

    ret = WaitRxAvailable(fd, timeout_ms * 1000);
    if(ret <= 0) {
        printf("CAN_Receive() wait-available error\n");
        return -1;
    }else {
        if ((ret = read(fd, pframe, sizeof(struct can_frame))) < 0)
            perror("cansocket: negative to receive\n");
        return ret;
    }
}

int CAN_SendBulk(int fd, uint32_t id, const uint8_t *pdata, uint16_t length, uint16_t interval_us)
{
    uint16_t num_of_frame = length >> 3;
    const uint8_t *ptr = pdata;
    int ret = 0, i = 0;

    if (length % 8 != 0)
        num_of_frame++;

    for (i = 0; i < num_of_frame - 1; i++) {
        ret = CAN_SendFrame(fd, id, ptr, 8, 5);
        ptr += 8;
        if (ret == -1)
            return ret;
        usleep(interval_us);
    }
    return CAN_SendFrame(fd, id, ptr, length - (ptr - pdata), 5);
}

int CAN_DeInit(int fd_can)
{
    if(fd_can != 0)
        close(fd_can);

	return 0;
}

/**
 * @ingroup extern
 * @brief initial CAN socket, phy device
 * @param name : CAN device name, should be the same as ifconfig -a
 * @param bitrate : CAN bus bitrate
 * @return file descriptor of CAN socket if success
 * @return negativ error code if failed
 */
int CAN_Init(const char *name, uint32_t bitrate)
{
    int state = 0, ret = 0, fd_can = 0;
    struct sockaddr_can addr;
    struct ifreq ifr;

    ret = can_get_state(name,&state);
    if(ret != 0){
        printf("can_get_state() Error\n");
        return -1;
    }
    if(state != CAN_STATE_STOPPED){
        ret = can_do_stop(name);
        if(ret != 0){
            printf("can_do_stop() error\n");
            return -1;
        }
        sleep(1);
        printf("can_do_stop() OK\n");
    }else {
        printf("%s already stopped\n", name);
    }

    ret = can_set_bitrate(name, bitrate);
    if(ret != 0){
        perror("can_set_bitrate() error\n");
        return -1;
    }else
        printf("can_set_bitrate() OK\n");

    struct can_bittiming bt;
    can_get_bittiming("can0", &bt);
    printf("can get bitrate %d\n", bt.bitrate);
    if (!can_do_start(name)) {
        printf("can_do_start() ok\n");
    }else {
        perror("can_do_start() error\n");
        return -1;
    }

    // initialize CAN transceiver
    if ((ret = Phy_Init()) < 0) {
        perror("fail to initialize can phy\n");
        return ret;
    }else {
        printf("phy init ok\n");
    }

    // open can socket
    if((fd_can = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error opening socket");
        return ret;
    }

    strcpy(ifr.ifr_name, name);
    if (ioctl(fd_can, SIOCGIFINDEX, &ifr)) {
        perror("ioctl");
        return -1;
    }

    addr.can_family  = PF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    printf("%s at index %d\n", name, ifr.ifr_ifindex);

    if(bind(fd_can, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error in socket bind");
        return -1;
    }
    return fd_can;
}

