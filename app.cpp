#include "app_include.h"
#include "mainwindow.h"

#define POOL_NUM 32
#define POLL_NUM 2

stBattery Battery_1 = {
    Battery_1.status = 1,
    Battery_1.voltage = 12,
    Battery_1.current = 5,
    Battery_1.temperature = 25,
    Battery_1.level = 0,
    Battery_1.capacity = 22000,
    Battery_1.err_code = 0,
    Battery_1.mux_on = 1,
};

stBattery Battery_2 = {
    Battery_2.status = 1,
    Battery_2.voltage = 8,
    Battery_2.current = 5,
    Battery_2.temperature = 28,
    Battery_2.level = 0,
    Battery_2.capacity = 22000,
    Battery_2.err_code = 0,
    Battery_2.mux_on = 1,
};

stHeater Heater = {
    Heater.status = 0,
    Heater.temperature = 0,
    Heater.pt100_adccode = 0,
    Heater.setpoint = 0,
    Heater.duty = 0,
    Heater.kp = 0,
    Heater.ki = 0,
    Heater.kd = 0,
};

/**
 * @ingroup intern
 * @brief update UI according to data indicated by CAN bus packet
 * @param none
 * @return none
 */
static void
UpdateInfoHandler(const stCanPacket &packet)
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
            break;
        case 0x01:  // voltage
            value = GetWordL(&packet.data[2]);
            bat->voltage = (float)value / 100;
            break;
        case 0x02: // current
            value = GetWordL(&packet.data[2]);
            bat->current = (float)value / 100;
            break;
        case 0x03: // temperature
            value = GetWordL(&packet.data[2]);
            bat->temperature = (float)value / 100;
            break;
        case 0x04: // level
            value = GetWordL(&packet.data[2]);
            bat->level = value;
            break;
        case 0x05: // capacity
            value = GetWordL(&packet.data[2]);
            bat->capacity = value;
            break;
        case 0x06: // error code
            value = GetWordL(&packet.data[2]);
            bat->err_code = value;
            break;
        case 0x07:
            bat->mux_on = packet.data[2] == 0 ? 0 : 1;
            break;
        default : break;
        }

    // heater content
    case 0x03:
        switch (packet.data[1]) {
        case 0x00:  // status
            Heater.status = packet.data[2];
            break;
        case 0x01:  // temperature
            value = GetWordL(&packet.data[2]);
            Heater.temperature = value / 100;
            break;
        case 0x02:  // setpoint
            value = GetWordL(&packet.data[2]);
            Heater.setpoint = value / 100;
            break;
        case 0x03:  // duty cycle
            value = GetWordL(&packet.data[2]);
            Heater.duty = value / 10000;
            break;
        case 0x04:  // PID parameter
            Heater.kp = GetWordL(&packet.data[2]);
            Heater.ki = GetWordL(&packet.data[4]);
            Heater.kd = GetWordL(&packet.data[6]);
            break;
        case 0x05:  // pt100 adc code
            Heater.pt100_adccode = GetLongL(&packet.data[2]);
        default : break;
        }
    default : break;
    }
}

/**
 * @ingroup extern
 * @brief send CAN bus command
 * @param target_id: CAN address for target
 * @param src_id:    CAN address from source
 * @param cmd_cmd:   CAN command number
 * @param *data:     pointer to payload buffer
 * @param dlc:       CAN packet dlc
 * @return 0: succeed, negative: fail
 */
int
SendCommand(uint8_t target_id, uint8_t src_id, uint8_t cmd_num, uint8_t *data, uint8_t dlc)
{
    tuCanId can_id;
    can_id.field.Target = target_id;
    can_id.field.Src = src_id;
    can_id.field.CmdNum = cmd_num;

//    return CAN_SendFrame(fd_cansocket, can_id.all, (const uint8_t *)data, dlc, 20);
    return 0;
}

Thread_Poll::Thread_Poll(QObject *parent)
{}

Thread_Poll::~Thread_Poll()
{}

Thread_Listen::Thread_Listen(QObject *parent)
{}

Thread_Listen::~Thread_Listen()
{}

Thread_Dummy::Thread_Dummy(QObject *parent)
{}

Thread_Dummy::~Thread_Dummy()
{}

/**
 * @ingroup extern
 * @brief listen can socket, receive packet and call appropriate handler()
 * @param none
 * @return none
 */
void Thread_Listen::run()
{
    int ret = 0;
    stCanPacket frame_rx;
    frame_rx.can_id.all = 0x00;
    memset(frame_rx.data, 0, 8);

    while (1) {
//        ret = CAN_RecvFrame(fd_cansocket, &frame_rx, 20);
        if (ret > 0 && (((stCanId *)&frame_rx.can_id)->Target == CANID_CHARGE)) {
            switch (((stCanId *)&frame_rx.can_id)->CmdNum) {
            case CMD_RD_ZONE:   // get info from target then update onto ui
                UpdateInfoHandler(frame_rx);
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
}

/**
 * @ingroup extern
 * @brief polling for all battery information via canbus periodically
 * @param interval: polling interval in millisecond
 * @return none
 */
void Thread_Poll::run()
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
//            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }

        // poll for battery_2 information
        data[0] = 0x02;
        for (i = 1; i < 8; i++) {
            data[1] = i;
//            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }

        // poll for heater information
        data[0] = 0x03;
        for (i = 0; i < 6; i++) {
            data[1] = i;
//            CAN_SendFrame(fd_cansocket, can_id, data, dlc, 20);
        }

        sleep(1);
    }
}

void Thread_Dummy::run()
{
    while (1)
    {
        QThread::sleep(1);

        Battery_1.voltage++;
        Battery_2.voltage++;
        Battery_1.temperature++;
        Battery_2.temperature++;
    }
}




























