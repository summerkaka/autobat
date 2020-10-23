#include "mainwindow.h"

#define BAT_ITEM_NUM    0x19
#define HEATER_ITEM_NUM 0x08
#define FC_ITEM_NUM     0x0a

stBattery Battery_1;
stBattery Battery_2;
stHeater Heater;
stAdaptor Adaptor;
stSystem FieldCase;
uint32_t runtime_min = 0;
uint32_t runtime_sec = 0;
bool Gc_on = false;
bool FanOn = false;
bool Valve_1_On = false;
bool Valve_2_On = false;
float gas1_pressure;
float gas2_pressure;
int tt = 0;

void InfoInit(void)
{
    Battery_1.index = 0;
    Battery_1.voltage = 0;
    Battery_1.temperature = 0;
    Battery_1.mux_on = false;

    Battery_2.index = 1;
    Battery_2.voltage = 0;
    Battery_2.temperature = 0;
    Battery_2.mux_on = false;
}

static void
UpdateAioHandler(const stCanPacket &packet)
{
    int32_t value;

    value = GetLongL(&packet.data[1]);
    value = (value & 0x00800000) ? (value | 0xff000000) :  (value & 0x00ffffff);

    switch (packet.data[0]) {
    case 0x01:  // bat_1 volt
        Battery_1.voltage = (float)value / 1000;
        break;
    case 0x02:  // bat_2 volt
        Battery_2.voltage = (float)value / 1000;
        break;
    case 0x03:  // bat_1 current
        Battery_1.current = (float)value / 1000;
        break;
    case 0x04:  // bat_2 current
        Battery_2.current = (float)value / 1000;
        break;
    case 0x05:  // bat_1 temperature
        Battery_1.temperature = (float)value / 1000;
        break;
    case 0x06:  // bat_2 temperature
        Battery_2.temperature = (float)value / 1000;
        break;
    case 0x07:  // bat_1 level
        Battery_1.level = (float)value;
        break;
    case 0x08:  // bat_2 level
        Battery_2.level = (float)value;
        break;
    case 0x09:  // bat_1 capacity
        Battery_1.capacity = (float)value;
        break;
    case 0x0a:  // bat_1 capacity
        Battery_2.capacity = (float)value;
        break;
    case 0x0b:  // bat_1 remain_time
        Battery_1.remain_time = (float)value;
        break;
    case 0x0c:  // bat_21 remain_time
        Battery_2.remain_time = (float)value;
        break;
    case 0x0d:  // bat_1 charge_times
        Battery_1.charge_times = (float)value;
        break;
    case 0x0e:  // bat_2 charge_times
        Battery_2.charge_times = (float)value;
        break;
    case 0x20:  // heater temperature
        Heater.temperature = (float)value / 1000;
        break;
    case 0x21:  // heater duty cycle
        Heater.duty = (float)value;
        break;
    case 0x30:  // adaptor voltage
        Adaptor.voltage = (float)value;
        break;
    case 0x40:  // fieldcase output voltage
        FieldCase.v_syspwr = (float)value;
        break;
    case 0x41:  // fieldcase output current
        FieldCase.consumption = (float)value;
        break;
    case 0x50:  // gas 1 pressure
        gas1_pressure = (float)value / 1000;
        break;
    case 0x51:  // gas 2 pressure
        gas2_pressure = (float)value / 1000;
        break;
    default : break;
    }
}

static void
ReadDectADCHander(const stCanPacket &packet)
{
    uint16_t value = GetWordL(&packet.data[0]);
    // adaptor supply
    Adaptor.status = (value & DIO_ADAPTOR_SUPPLY) ? 1 : 0;
    // bat1 mux_on
    Battery_1.mux_on = (value & DIO_BAT1_SUPPLY) ? 1 : 0;
    // bat2 mux_on
    Battery_2.mux_on = (value & DIO_BAT2_SUPPLY) ? 1 : 0;
    // fan switch
    FanOn = (value & DIO_FAN) ? 1 : 0;
    // heater switch
    Heater.status = (value & DIO_HEATER) ? 1 : 0;
    // bat1 charge
    Battery_1.charge_en = (value & DIO_BAT1_CHARGE) ? 1 : 0;
    // bat2 charge
    Battery_2.charge_en = (value & DIO_BAT2_CHARGE) ? 1 : 0;
    // bat1 aged
    Battery_1.is_aged = (value & DIO_BAT1_AGED) ? 1 : 0;
    // bat2 aged
    Battery_2.is_aged = (value & DIO_BAT2_AGED) ? 1 : 0;
    // valve1 switch
    Valve_1_On = (value & DIO_VALVE1_SW) ? 1 : 0;
    // valve2 switch
    Valve_2_On = (value & DIO_VALVE2_SW) ? 1 : 0;
}

/**
 * @ingroup intern
 * @brief update UI according to data indicated by CAN bus packet
 * @param none
 * @return none
 */
static void
DebugRdHandler(const stCanPacket &packet)
{
    stBattery *bat;
    int16_t value = 0;
    tuType32Bit tu_value;

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
            tu_value.tInt32  = GetLongL(&packet.data[2]);
            bat->voltage = tu_value.tFloat;
            break;
        case 0x02:  // current
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            bat->current = tu_value.tFloat;
            break;
        case 0x03:  // temperature
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            bat->temperature = tu_value.tFloat;
            break;
        case 0x04:  // level
            bat->level = GetWordL(&packet.data[2]);
            break;
        case 0x05:  // capacity
            bat->capacity = GetWordL(&packet.data[2]);
            break;
        case 0x06:  // mux_on
            bat->mux_on = packet.data[2];
            break;
        case 0x07:  // remain_time
            bat->remain_time = GetWordL(&packet.data[2]);
            break;
        case 0x08:  // charge Iset
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            bat->charge_current = tu_value.tFloat;
            break;
        case 0x09:  // charge_timer
            bat->fastcharge_timer = GetLongL(&packet.data[2]);
            break;
        case 0x0a:  // pre_start_time
            bat->pre_start_time = GetLongL(&packet.data[2]);
            break;
        case 0x0b:  // fast_start_time
            bat->fast_start_time = GetLongL(&packet.data[2]);
            break;
        case 0x0c:  // trickle_start_time
            bat->trickle_start_time = GetLongL(&packet.data[2]);
            break;
        case 0x0d:  // charge_finish_time
            bat->charge_finish_time = GetLongL(&packet.data[2]);
            break;
        case 0x0e:  // charge_times
            bat->charge_times = GetWordL(&packet.data[2]);
            break;
        case 0x0f:  // scale_flag
            bat->scale_flag = packet.data[2];
            break;
        case 0x10:  // is_aged
            bat->is_aged = packet.data[2];
            break;
        case 0x11:  // ng
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            bat->ng = tu_value.tFloat;
            break;
        case 0x12:  // impedance
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            bat->impedance = tu_value.tFloat;
            break;
        case 0x13:  // charge_en
            bat->charge_en = packet.data[2];
            break;
        case 0x14:  // error code
            bat->err_code = GetWordL(&packet.data[2]);
            break;
        case 0x015: // level_top
            bat->level_top = GetWordL(&packet.data[2]);
            break;
        case 0x16:  // level_bottom
            bat->level_bottom = GetWordL(&packet.data[2]);
            break;
        case 0x17:  // light_transition
            bat->light_transition = GetLongL(&packet.data[2]);
            break;
        case 0x18:  // heavy_transition
            bat->heavy_transition = GetLongL(&packet.data[2]);
            break;
        default : break;
        }
        break;
    // heater content
    case 0x03:
        switch (packet.data[1]) {
        case 0x00:  // status
            Heater.status = packet.data[2];
            break;
        case 0x01:  // temperature
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            Heater.temperature = tu_value.tFloat;
            break;
        case 0x02:  // setpoint
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            Heater.setpoint = tu_value.tFloat;
            break;
        case 0x03:  // duty cycle
            Heater.duty = GetWordL(&packet.data[2]);
            break;
        case 0x04:  // PID Kp
            Heater.kp = GetWordL(&packet.data[2]);
            break;
        case 0x05:  // PID Ki
            Heater.ki = GetWordL(&packet.data[2]);
            break;
        case 0x06:  // PID Kd
            Heater.kd = GetWordL(&packet.data[2]);
            break;
        case 0x07:  // pt100 adc code
            Heater.pt100_adccode = GetLongL(&packet.data[2]);
        default : break;
        }
        break;
    // fieldcase content
    case 0x04:
        switch (packet.data[1]) {
        case 0x00:  // cover
            FieldCase.is_covered = packet.data[2];
            break;
        case 0x01:  // power button
            FieldCase.is_switchon = packet.data[2];
            break;
        case 0x02:  // v_adaptor
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            Adaptor.voltage = tu_value.tFloat;
            break;
        case 0x03:  // v_vcc12
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            FieldCase.v_syspwr = tu_value.tFloat;
            break;
        case 0x04:  // gc current
            tu_value.tInt32 = GetLongL(&packet.data[2]);
            FieldCase.consumption = tu_value.tFloat;
            break;
        case 0x05:  // runtime minute
            runtime_min = GetLongL(&packet.data[2]);
            break;
        case 0x06:  // runtime second
            runtime_sec = GetLongL(&packet.data[2]);
            break;
        case 0x07:  // switchon time
            FieldCase.switchon_time = GetLongL(&packet.data[2]);
            break;
        case 0x08:  // switchoff time
            FieldCase.switchoff_time = GetLongL(&packet.data[2]);
            break;
        case 0x09:  // gc_pwr
            Gc_on = packet.data[2];
            break;
        default : break;
        }
    default : break;
    }
}

static void
CAN_Listen(void)
{
    int ret = 0;
    CAN_msg msgrx;
    stCanPacket packet;

    packet.can_id.all = 0x00;
    packet.dlc = 0;
    memset(packet.data, 0, 8);
    msgrx.format = EXTENDED_FORMAT;
    msgrx.id = 0;
    msgrx.len = 0;
    msgrx.type = DATA_FRAME;

    ret = can_port.RecvCANMessage(&msgrx, 20);

    if (ret == CAN_OK) {
        packet.can_id.all = msgrx.id;
        memcpy(packet.data, msgrx.data, msgrx.len);
        packet.dlc = msgrx.len;
    }else {
        return;
    }

    tt++;

    if (packet.can_id.field.Target == CANID_PC) {
        switch (packet.can_id.field.CmdNum) {
        case CMD_READ_AIO:
            UpdateAioHandler(packet);
            break;
        case CMD_READ_ZONE:
            break;
        case CMD_READ_DIO:
            ReadDectADCHander(packet);
            break;
        case CMD_WRITE_SP:
            // get answer for previous command, need do nothing
            break;
        case CMD_DEBUG_RD:
            DebugRdHandler(packet);
            break;
        default : break;
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
    tuCanId can_id;
    CAN_msg msgtx;

    while (1)
    {
        can_id.all = 0;
        can_id.field.Target = CANID_CHARGE;
        can_id.field.Src = CANID_PC;
        msgtx.ch = 0;
        msgtx.id = can_id.all;
        msgtx.type = DATA_FRAME;
        msgtx.format = EXTENDED_FORMAT;

        /* polling for AIO */
        can_id.field.CmdNum = CMD_READ_AIO;
        msgtx.id = can_id.all;
        msgtx.len = 1;
        // battery
        for (i = 1; i <= 0x0e; i++) {   // aio
            msgtx.data[0] = i;
            can_port.SendCANMessage(&msgtx, 1000);
            CAN_Listen();
        }
        // heater
        msgtx.data[0] = 0x20;
        can_port.SendCANMessage(&msgtx, 1000);
        CAN_Listen();
        msgtx.data[0] = 0x21;
        can_port.SendCANMessage(&msgtx,1000);
        CAN_Listen();
        // adaptor
        msgtx.data[0] = 0x30;
        can_port.SendCANMessage(&msgtx, 1000);
        CAN_Listen();
        // field case
        msgtx.data[0] = 0x40;
        can_port.SendCANMessage(&msgtx, 1000);
        CAN_Listen();
        msgtx.data[0] = 0x41;
        can_port.SendCANMessage(&msgtx,1000);
        CAN_Listen();
        // gas
        msgtx.data[0] = 0x50;
        can_port.SendCANMessage(&msgtx, 1000);
        CAN_Listen();
        msgtx.data[0] = 0x51;
        can_port.SendCANMessage(&msgtx, 1000);
        CAN_Listen();

        /* polling for DIO */
        can_id.field.CmdNum = CMD_DETECT_ADC;
        msgtx.id = can_id.all;
        msgtx.len = 0;
        can_port.SendCANMessage(&msgtx, 5);
        CAN_Listen();

        /* polling for ZONE */
        can_id.field.CmdNum = CMD_READ_ZONE;
        msgtx.id = can_id.all;
        msgtx.len = 1;
        for (i = 1; i <= 3; i++) {
            msgtx.data[0] = i;
            can_port.SendCANMessage(&msgtx, 5);
            CAN_Listen();
        }

        /* polling for debug info */
        can_id.field.CmdNum = CMD_DEBUG_RD;
        msgtx.id = can_id.all;
        msgtx.len = 2;
        msgtx.data[0] = 0x01;
        for (i = 0; i < BAT_ITEM_NUM; i++) {
            msgtx.data[1] = i;
            can_port.SendCANMessage(&msgtx, 5);
            CAN_Listen();
        }
        msgtx.data[0] = 0x02;
        for (i = 0; i < BAT_ITEM_NUM; i++) {
            msgtx.data[1] = i;
            can_port.SendCANMessage(&msgtx, 5);
            CAN_Listen();
        }
        msgtx.data[0] = 0x03;
        for (i = 0; i < HEATER_ITEM_NUM; i++) {
            msgtx.data[1] = i;
            can_port.SendCANMessage(&msgtx, 5);
            CAN_Listen();
        }

        msgtx.data[0] = 0x04;
        for (i = 0; i < FC_ITEM_NUM; i++) {
            msgtx.data[1] = i;
            can_port.SendCANMessage(&msgtx, 5);
            CAN_Listen();
        }
    }
}

Thread_Poll::Thread_Poll(QObject *parent)
{}

Thread_Poll::~Thread_Poll()
{}

//uint32_t HashTable_Bat[17][4] = {

//}
























