#include "app_include.h"
#include "mainwindow.h"
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
    .err_code = 0,
    .mux_on = 0
};

stBattery Battery_2 = {
    .status = 1,
    .voltage = 8,
    .current = 5,
    .temperature = 28,
    .level = 0,
    .capacity = 0,
    .err_code = 0,
    .mux_on = 0
};

stHeater Heater = {
    .status = 0,
    .temperature = 0,
    .pt100_adccode = 0,
    .setpoint = 0,
    .duty = 0,
    .kp = 0,
    .ki = 0,
    .kd = 0
};

/**
 * @ingroup intern
 * @brief update UI according to data indicated by CAN bus packet
 * @param none
 * @return none
 */
static void
UpdateUiHandler(const struct can_frame &packet, MainWindow *w)
{
    stBattery *bat;
    int16_t value = 0;

    switch (packet.data[0]) {

    // battery content
    case 0x01:
    case 0x02:
        if (packet.data[0] == 0x01) {
            bat = &Battery_1;
        }else if (packet.data[0] == 0x02) {
            bat = &Battery_2;
        }
        switch (packet.data[1]) {
        case 0x00:  // status
            bat->status = packet.data[2];
            w->BatteryTabWrite(packet.data[0] - 1, packet.data[1], &bat->status);
            break;
        case 0x01:  // voltage
            value = GetWordL(&packet.data[2]);
            bat->voltage = (float)value / 100;
            w->BatteryTabWrite(packet.data[0] - 1, packet.data[1], &bat->voltage);
            break;
        case 0x02: // current
            value = GetWordL(&packet.data[2]);
            bat->current = (float)value / 100;
            w->BatteryTabWrite(packet.data[0] - 1, packet.data[1], &bat->current);
            break;
        case 0x03: // temperature
            value = GetWordL(&packet.data[2]);
            bat->temperature = (float)value / 100;
            w->BatteryTabWrite(packet.data[0] - 1, packet.data[1], &bat->temperature);
            break;
        case 0x04: // level
            value = GetWordL(&packet.data[2]);
            bat->level = value;
            w->BatteryTabWrite(packet.data[0] - 1, packet.data[1], &bat->level);
            break;
        case 0x05: // capacity
            value = GetWordL(&packet.data[2]);
            bat->capacity = value;
            w->BatteryTabWrite(packet.data[0] - 1, packet.data[1], &bat->capacity);
            break;
        case 0x06: // error code
            value = GetWordL(&packet.data[2]);
            bat->err_code = value;
            w->BatteryTabWrite(packet.data[0] - 1, packet.data[1], &bat->err_code);
            break;
        case 0x07:
            bat->mux_on = packet.data[2] == 0 ? 0 : 1;
            w->BatteryTabWrite(packet.data[0] - 1, packet.data[1], &bat->mux_on);
            break;
        default : break;
        }

    // heater content
    case 0x03:
        switch (packet.data[1]) {
        case 0x00:  // status
            Heater.status = packet.data[2];
            w->HeaterTabWrite(0x00, &Heater.status);
            w->HeaterTabWrite(0x04, &Heater.status);
            break;
        case 0x01:  // temperature
            value = GetWordL(&packet.data[2]);
            Heater.temperature = value / 100;
            w->HeaterTabWrite(0x01, &Heater.temperature);
            break;
        case 0x02:  // setpoint
            value = GetWordL(&packet.data[2]);
            Heater.setpoint = value / 100;
            w->HeaterTabWrite(0x05, &Heater.setpoint);
            break;
        case 0x03:  // duty cycle
            value = GetWordL(&packet.data[2]);
            Heater.duty = value / 10000;
            w->HeaterTabWrite(0x03, &Heater.duty);
            break;
        case 0x04:  // PID parameter
            Heater.kp = GetWordL(&packet.data[2]);
            w->HeaterTabWrite(0x06, &Heater.kp);
            Heater.ki = GetWordL(&packet.data[4]);
            w->HeaterTabWrite(0x07, &Heater.ki);
            Heater.kd = GetWordL(&packet.data[6]);
            w->HeaterTabWrite(0x08, &Heater.kd);
            break;
        case 0x05:  // pt100 adc code
            Heater.pt100_adccode = GetLongL(&packet.data[2]);
            w->HeaterTabWrite(0x02, &Heater.pt100_adccode);
        default : break;
        }
    default : break;
    }
}

void
SendCommand(uint8_t target_id, uint8_t src_id, uint8_t cmd_num, uint8_t *data, uint8_t dlc)
{
    tuCanId can_id;
    can_id.field.Target = target_id;
    can_id.field.Src = src_id;
    can_id.field.CmdNum = cmd_num;

    CAN_SendFrame(fd_cansocket, can_id.all, (const uint8_t *)data, dlc, 20);
}


/**
 * @ingroup extern
 * @brief listen can socket, receive packet and call appropriate handler()
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
            case CMD_RD_ZONE:   // get info from target then update onto ui
                UpdateUiHandler(frame_rx, (MainWindow *)para);
                break;
            case CMD_WR_ZONE:   // get info from ui then send command to target
                // get answer for previous command, need do nothing
                break;
            case CMD_WRITE_SP:
                // get answer for previous command, need do nothing
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

        // poll for battery_1 information
        data[0] = 0x01;
        for (i = 0; i < 8; i++) {
            data[1] = i;
            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }

        // poll for battery_2 information
        data[0] = 0x02;
        for (i = 1; i < 8; i++) {
            data[1] = i;
            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }

        // poll for heater information
        data[0] = 0x03;
        for (i = 0; i < 6; i++) {
            data[1] = i;
            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }

        sleep(1);
    }

    return (void*)NULL;
}






























