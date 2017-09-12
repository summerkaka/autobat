#include "app_include.h"

#define PATH_BUF_SIZE 64

stGpio gpio0_16;

void Gpio_Init(void)
{
#if USE_QTGUI
    QMessageBox msg;
#endif
    int memfd0;
    long *mem0;
    long wr_val0 = 0;

    //open /dev/mem with read and write mode
    if((memfd0 = open ("/dev/mem", O_RDWR)) < 0){
        perror("fail to open /dev/mem\n");
#if USE_QTGUI
        msg.setText("[ERROR] Can't open /dev/mem.");
        msg.exec();
#endif
    }
    //map physical memory
    mem0 = (long*)mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, memfd0, 0xf8000000);

    if (mem0 == MAP_FAILED) {
        perror("mmap faile\n");
#if USE_QTGUI
        msg.setText("[ERROR] mmap mem0 error.");
        msg.exec();
#endif
    }
    //Write memory
    wr_val0 = 0xDF0D;
    memcpy((void*)((long)mem0+0x08), &wr_val0, 4); //memcpy count size_t by byte! so it's 4
    wr_val0 = 0x1200;
    memcpy((void*)((long)mem0+0x720), &wr_val0, 4);

    //unmap mem
    munmap(mem0, 5); //destroy map memory
    close(memfd0);   //close file
}


/**
 * @ingroup extern
 * @brief configure gpio pin direction, and initial value
 * @param num : index of ps_mio gpio pin
 * @param direction : gpio pin direction. 0: in, 1: out
 * @param init_value : used for output pin. 0 or 1
 * @return 0 if success
 * @return negativ if failed
 */
int Gpio_Pin_Config(int num, int base, char *direction, bool init_value)
{
    char buf[PATH_BUF_SIZE] = {0};
    int fd = 0, ret = 0, index = num + base;
#if USE_QTGUI
    QMessageBox msg;
#endif
    //set gpio export
    if ((fd = open("/sys/class/gpio/export", O_WRONLY)) < 0) {
        perror("fail to open gpio export\n");
#if USE_QTGUI
        msg.setText("[ERROR] open export fail.");
        msg.exec();
#endif
        return fd;
    }
    snprintf(buf, PATH_BUF_SIZE, "%d", index);
    ret = write(fd, buf, strlen(buf));
    close(fd);
    if (ret  < 0) {
        perror("fail to write gpio export\n");
        return ret;
    }
    usleep(5000);

    // Set out direction
    snprintf(buf, PATH_BUF_SIZE, "/sys/class/gpio/gpio%d/direction", index);
    if ((fd = open(buf, O_WRONLY)) < 0) {
        perror("fail to open gpio direction\n");
        return fd;
    }
    if (strcmp(direction, "out") == 0) {
        ret = write(fd, "out", 3);

    }else if (strcmp(direction, "in") == 0) {
        ret = write(fd, "in", 2);
    }else {
        perror("invalid direction argument\n");
        return -1;
    }
    if (ret < 0) {
        printf("fail to set direction of mio%d\n", num);
    }
    close(fd);
    usleep(5000);

    // Set GPIO init status
    snprintf(buf, PATH_BUF_SIZE, "/sys/class/gpio/gpio%d/value", index);
    if ((fd = open(buf, O_WRONLY)) < 0) {
        perror("fail to open gpio value\n");
        return fd;
    }

    if (init_value == 1) {
        ret = write(fd, "1", 1);
    }else if (init_value == 0) {
        ret = write(fd, "0", 1);
    }else {
        printf("wrong mio-gpio%d init value\n", num);
    }
    if (ret < 0) {
        printf("fail to write mio-gpio%d value\n", num);
    }
    close(fd);
    return ret;
}

int Gpio_Pin_DeConfig(int num, int base)
{
    char buf[PATH_BUF_SIZE] = {0};
    int fd = 0, ret = 0, index = num + base;
#if USE_QTGUI
    QMessageBox msg;
#endif
    //set gpio export
    if ((fd = open("/sys/class/gpio/unexport", O_WRONLY)) < 0) {
        perror("fail to open gpio unexport\n");
#if USE_QTGUI
        msg.setText("[ERROR] open export fail.");
        msg.exec();
#endif
        return fd;
    }
    snprintf(buf, PATH_BUF_SIZE, "%d", index);
    ret = write(fd, buf, strlen(buf));
    if (ret < 0) {
        perror("fail to unexport gpio\n");
        return ret;
    }
    close(fd);
    return 0;
}

/**
 * @ingroup extern
 * @brief write output pin value
 * @param Num : index of ps_mio gpio pin
 * @param Value : gpio pin value
 * @return 0 if success
 * @return negativ if failed
 */
int Gpio_Write(int num, bool value)
{
    int fd, ret, index = num + 906;
    char buf[64];
#if USE_QTGUI
    QMessageBox msg;
#endif

    sprintf(buf, "/sys/class/gpio/gpio%d/value", index);
    if ((fd = open(buf, O_RDWR)) < 0) {
        printf("fail to open %s", buf);
#if USE_QTGUI
        msg.setText("[ERROR] Can't open gpio device.");
        msg.exec();
#endif
        return fd;
    }

    if ((ret = read(fd, buf, 1)) < 0) {
        printf("Can't read gpio%d\n", index);
#if USE_QTGUI
        msg.setText("[ERROR] Can't read gpio.");
        msg.exec();
#endif
        close(fd);
        return ret;
    }
    printf("gpio%d value is %d\n", index, buf[0]);

    if (!value){
        ret = write(fd, "0", 1);
    }else if (value) {
        ret = write(fd, "1", 1);
    }

    if (ret < 0) {
        printf("fail to write gpio%d\n", index);
#if USE_QTGUI
        msg.setText("[ERROR] Can't write gpio%d.");
        msg.exec();
#endif
    }
    close(fd);
    return ret;
}





















