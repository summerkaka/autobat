#include "app_include.h"

bool com_isopen = false;
int16_t com_num = 0;
RT_CAN can_port;
CAN_msg msg_tx;
CAN_msg msg_rx;


int8_t CAN_Init(uint32_t bitrate)
{
    DWORD Len = sizeof(BYTE);
    BYTE by = 0;
    BYTE rate[4] = {0};

    // config for auto resend
    if (can_port.SetConfigValue(CAN_ART_ID, &by, &Len, 1000) != CAN_OK)
    {
        printf("CAN config auto resend fail\n");
        return -1;
    }
    // config for baud rate
    WriteLongL(rate, bitrate);
    Len = 4;
    if (can_port.SetConfigValue(CAN_BAUD_ID, &rate, &Len, 1000) != CAN_OK)
    {
        printf("CAN config bitrate fail\n");
        return -2;
    }

    CAN_msg msg;
    msg.ch = 0;
    msg.id = 0x12345678;
    msg.data[0] = 0x24;
    msg.format = EXTENDED_FORMAT;
    msg.type = DATA_FRAME;
    msg.len = 1;
    can_port.SendCANMessage(&msg, 5);
    return 0;
}

int8_t SendCommand(uint8_t target_id, uint8_t src_id, uint8_t cmd_num, uint8_t *data, uint8_t dlc, uint16_t timeout)
{
    tuCanId can_id;
    CAN_msg msgtx;
    can_id.all = 0;
    can_id.field.Target = target_id;
    can_id.field.Src = src_id;
    can_id.field.CmdNum = cmd_num;

    msgtx.id = can_id.all;
    msgtx.len = dlc;
    msgtx.type = DATA_FRAME;
    msgtx.format = EXTENDED_FORMAT;
    msgtx.ch = 0x00;
    memcpy(msgtx.data, data, dlc);

    if (can_port.SendCANMessage(&msgtx, timeout) == 1)
        return 0;
    else
        return -1;
}



