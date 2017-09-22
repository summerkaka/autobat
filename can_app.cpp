#include "app_include.h"

#include <can_app.h>
#include <can_if.h>


#define POOL_NUM 32
#define POLL_NUM 2

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
    .voltage = 12,
    .current = 5,
    .temperature = 25,
    .level = 0,
    .capacity = 0,
    .err_code = 0
};

stBattery Battery_2 = {
    .status = 1,
    .voltage = 8,
    .current = 5,
    .temperature = 28,
    .level = 0,
    .capacity = 0,
    .err_code = 0
};

stHeater Heater = {
    .status = 0,
    .temperature = 0,
    .setpoint = 0,
    .duty = 0,
    .kp = 0,
    .ki = 0,
    .kd = 0
};

static void
ZoneHandler(const struct can_frame &packet)
{
    stBattery *bat;
    int16_t value = 0;

    if (packet.data[0] == 0x01) {
        bat = &Battery_1;
    }else if (packet.data[0] == 0x02) {
        bat = &Battery_2;
    }
    switch (packet.data[1]) {
    case 0x01:  // status
        bat->status = packet.data[2];
        break;
    case 0x02:  // voltage
        value = GetWordL(&packet.data[2]);
        bat->voltage = (float)value / 100;
        break;
    case 0x03: // current
        value = GetWordL(&packet.data[2]);
        bat->current = (float)value / 100;
        break;
    case 0x04: // temperature
        value = GetWordL(&packet.data[2]);
        bat->temperature = (float)value / 100;
        break;
    case 0x05: // level
        value = GetWordL(&packet.data[2]);
        bat->level = value;
        break;
    case 0x06: // capacity
        value = GetWordL(&packet.data[2]);
        bat->capacity = value;
        break;
    case 0x07: // error code
        value = GetWordL(&packet.data[2]);
        bat->err_code = value;
        break;
    default : break;
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
    struct can_frame frame_rx;

    while (1) {
        ret = CAN_RecvFrame(fd_cansocket, &frame_rx, 20);
        if (ret > 0 && (((stCanId *)&frame_rx.can_id)->Target == CANID_CHARGE)) {
            switch (((stCanId *)&frame_rx.can_id)->CmdNum) {
            case 0x63:
                ZoneHandler(frame_rx);
                break;
            case 0x98:
                break;
            default : break;
            }
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































