#include <stdint.h>
#include "app_include.h"

#define MSG_NUM 10
#define POLL_NUM 2

struct can_frame canrx_pool[MSG_NUM];

const struct can_frame cantx_pool[2] = {
    {
        .can_id = 0x0,
        .can_dlc = 0,
        .__pad = 0,
        .__res0 = 0,  /* reserved / padding */
        .__res1 = 0,  /* reserved / padding */
        .data = {0,0,0,0,0,0,0,0},
    },
    {
        .can_id = 0x0,
        .can_dlc = 0,
        .__pad = 0,
        .__res0 = 0,  /* reserved / padding */
        .__res1 = 0,  /* reserved / padding */
        .data = {0,0,0,0,0,0,0,0},
    },
//    {}
};

void
Msg_Init(void)
{
    for (int i = 0; i < MSG_NUM; i++) {
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
            for (i = 0; i < MSG_NUM; i++) {
                if (canrx_pool[i].can_id == 0) {
                    memcpy(&canrx_pool[i], &frame_rx, sizeof(struct can_frame));
                    break;
                }
            }
            pthread_rwlock_unlock(&lock_canpool);
            sem_post(&sem_display);
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
CAN_Poll(void *interval)
{
    uint8_t i = 0;
    uint32_t inverval_us = *(uint16_t *)interval * 1000;

    while (1)
    {
        for (i = 0; i < POLL_NUM; i++) {
            CAN_SendFrame(fd_cansocket, cantx_pool[i].can_id, (const uint8_t *)cantx_pool[i].data, cantx_pool[i].can_dlc, 20);
        }
        usleep(inverval_us);
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
UpdateUi(void *para)
{
    int i = 0;
    tuCanId can_id;

    while (1)
    {
        sem_wait(&sem_display);
        pthread_rwlock_rdlock(&lock_canpool);

//        RealTimeCurve::updateCurve(&i);

        for (i = 0; i < MSG_NUM; i++) {
            if (canrx_pool[i].can_id != 0) {
                can_id.all = canrx_pool[i].can_id;
                switch(can_id.field.CmdNum) {
                case 0x00:
                    break;
                default : break;
                }
            }
        }

        pthread_rwlock_unlock(&lock_canpool);
    }
    return (void *)NULL;
}





























