#include "app_include.h"

#define POOL_NUM 32
#define POLL_NUM 2

struct can_frame canrx_pool[POOL_NUM];

const struct can_frame cantx_pool[POLL_NUM] = {
    // bat-1 status
    {
        .can_id = 0x042022c3,
        .can_dlc = 0,
#ifdef  DESKTOP
        .__pad = 0,
        .__res0 = 0,  /* reserved / padding */
        .__res1 = 0,  /* reserved / padding */
#endif
        .data = {0x01,0x01,0,0,0,0,0,0},
    },
    {}
};

stBattery Battery_1 = {
    .status = 1,
    .voltage = 10,
    .current = 5,
    .temperature = 25
};

stBattery Battery_2 = {
    .status = 1,
    .voltage = 10,
    .current = 5,
    .temperature = 28
};

void
Msg_Init(void)
{
    for (int i = 0; i < POOL_NUM; i++) {
        canrx_pool[i].can_id = 0;
        canrx_pool[i].can_dlc = 0;
        memset(canrx_pool[i].data, 0, 8);
    }
}

/**
 * @ingroup extern
 * @brief listen can socket, receive packet and put to packet pool, main program polls the packet pool and update UI
 * @param none
 * @return none
 */
void *
CAN_Listen(void *para)
{
    int ret = 0;
    int i = 0;
    struct can_frame frame_rx;

    while (1) {
        ret = CAN_RecvFrame(fd_cansocket, &frame_rx, 20);
        if (ret > 0 && (((stCanId *)&frame_rx.can_id)->Target == CANID_CHARGE)) {
            pthread_rwlock_wrlock(&lock_canpool);
            for (i = 0; i < POOL_NUM; i++) {
                if (canrx_pool[i].can_id == 0) {
                    memcpy(&canrx_pool[i], &frame_rx, sizeof(struct can_frame));
                    break;
                }
            }
            pthread_rwlock_unlock(&lock_canpool);
        }
    }

    return (void*)NULL;
}

/**
 * @ingroup extern
 * @brief polling for all battery information via canbus periodically
 * @param interval: polling interval in millisecond
 * @return none
 */
void *
CAN_Poll(void *para)
{
    uint8_t i = 0;
    uint32_t can_id = 0x042022c3;
    uint32_t dlc = 1;
    uint8_t data[8] = {0};

    while (1)
    {
        can_id = 0x042022c3;
        dlc = 2;
        data[0] = 0x01;
        for (i = 1; i < 9; i++) {
            data[1] = i;
            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }
        data[0] = 0x02;
        for (i = 1; i < 9; i++) {
            data[1] = i;
            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }
        data[0] = 0x03;
        for (i = 1; i < 4; i++) {
            data[1] = i;
            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }

        sleep(15);
    }

    return (void*)NULL;
}


static void
ZoneHandler(const stCanPacket *packet)
{
    stBattery *bat;
    int16_t value = 0;

    if (packet->data[0] == 0x01) {
        bat = &Battery_1;
    }else if (packet->data[0] == 0x02) {
        bat = &Battery_2;
    }
    switch (packet->data[1]) {
    case 0x01:  // status
        bat->status = packet->data[2];
        break;
    case 0x02:  // voltage
        value = GetWordL(&packet->data[2]);
        bat->voltage = (float)value / 100;
        break;
    case 0x03: // current
        value = GetWordL(&packet->data[2]);
        bat->current = (float)value / 100;
        break;
    case 0x04: // temperature
        value = GetWordL(&packet->data[2]);
        bat->temperature = (float)value / 100;
        break;
    case 0x05: // level
        value = GetWordL(&packet->data[2]);
        bat->level = value;
        break;
    case 0x06: // capacity
        value = GetWordL(&packet->data[2]);
        bat->capacity = value;
        break;
    case 0x07: // error code
        value = GetWordL(&packet->data[2]);
        bat->err_code = value;
        break;
    default : break;
    }
}

/**
 * @ingroup extern
 * @brief polling for all battery information via canbus periodically
 * @param interval: polling interval in millisecond
 * @return none
 */

void *
DataUpdate(void *para)
{
    int i = 0;
    tuCanId can_id;

    while (1)
    {
        pthread_rwlock_rdlock(&lock_canpool);

        for (i = 0; i < POOL_NUM; i++) {
            if (canrx_pool[i].can_id != 0) {
                can_id.all = canrx_pool[i].can_id;
                switch(can_id.field.CmdNum) {
                case 0x63:
                    ZoneHandler((const stCanPacket *)&canrx_pool[i]);
                    break;
                case 0x98:
                    break;
                default : break;
                }
                canrx_pool[i].can_id = 0;
                memset(&canrx_pool[i], 0, sizeof(struct can_frame));
            }
        }

        pthread_rwlock_unlock(&lock_canpool);

        sleep(1);
    }

    return (void *)NULL;
}





























