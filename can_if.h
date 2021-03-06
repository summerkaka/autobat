#ifndef CAN_IF_H
#define CAN_IF_H

#include "stdint.h"
#include "RT_CAN.h"

#define CAN_BITRATE     1000000

#define CANID_CHARGE    0x0d
#define CANID_PUMP      0x12
#define CANID_MB        0x21
#define CANID_PC        0x31

#define CMD_PING        0x00
#define CMD_DLD         0x01
#define CMD_SENDDATA    0x02
#define CMD_REQUEST     0x03
#define CMD_RESET       0x05
#define CMD_ASK_APPAREA 0xaa
#define CMD_JUMPTOAPP   0xab
#define CMD_JUMPTOBL    0xac
#define CMD_WRITECRC    0xad
#define CMD_PROGRAM_START   0xae
#define CMD_PROGRAM_END     0xaf

#define CMD_READ_AIO        0x61
#define CMD_DETECT_ADC      0x62
#define CMD_READ_ZONE       0x63
#define CMD_WR_ZONE         0x64
#define	CMD_READ_ID         0x80
#define	CMD_READ_DIO        0x87
#define	CMD_WRITE_DIO       0x88
#define CMD_WRITE_SP        0x98
#define CMD_DEBUG_RD        0xf0
#define CMD_DEBUG_WR        0xf1

// DIO mask macro
#define BLOCK_A 0x01
#define BLOCK_B 0x02
#define DIO_ADAPTOR_SUPPLY	0x0001
#define DIO_BAT1_SUPPLY		0x0002
#define DIO_BAT2_SUPPLY		0x0004
#define DIO_FAN				0x0008
#define DIO_HEATER			0x0010
#define	DIO_BAT1_CHARGE		0x0020
#define DIO_BAT2_CHARGE		0x0040
#define DIO_BAT1_AGED       0x0080
#define DIO_BAT2_AGED       0x0100
#define DIO_VALVE1_SW		0x0200
#define DIO_VALVE2_SW		0x0400

#define GetLongH(ptr)   (uint32_t)*(uint8_t *)(ptr) << 24 | \
                        (uint32_t)*((uint8_t *)(ptr) + 1) << 16 | \
                        (uint32_t)*((uint8_t *)(ptr) + 2) << 8  | \
                        (uint32_t)*((uint8_t *)(ptr) + 3)

#define GetLongL(ptr)   (uint32_t)*((uint8_t *)(ptr) + 3) << 24 | \
                        (uint32_t)*((uint8_t *)(ptr) + 2) << 16 | \
                        (uint32_t)*((uint8_t *)(ptr) + 1) << 8  | \
                        (uint32_t)*(ptr)

#define GetWordH(ptr)   (uint16_t)*(uint8_t *)(ptr) << 8  | \
                        (uint16_t)*((uint8_t *)(ptr) + 1)

#define GetWordL(ptr)   *((uint8_t *)(ptr) + 1) << 8  | \
                        *(uint8_t *)(ptr)

// ptr is h
#define WriteLongH(ptr, value)      do {*(uint8_t *)(ptr)       = (value) >> 24; \
                                        *(uint8_t *)((ptr) + 1) = (value) >> 16 & 0xff; \
                                        *(uint8_t *)((ptr) + 2) = (value) >> 8 & 0xff;  \
                                        *(uint8_t *)((ptr) + 3) = (value) & 0xff;       \
                                    } while (0)
// ptr is l
#define WriteLongL(ptr, value)      do {*(uint8_t *)((ptr) + 3) = (value) >> 24; \
                                        *(uint8_t *)((ptr) + 2) = (value) >> 16 & 0xff; \
                                        *(uint8_t *)((ptr) + 1) = (value) >> 8 & 0xff;  \
                                        *(uint8_t *)(ptr)       = (value) & 0xff;       \
                                    } while (0)
// ptr is h
#define WriteWordH(ptr, value)      do {*(uint8_t *)(ptr)       = (value) >> 8; \
                                        *(uint8_t *)((ptr) + 1) = (value);      \
                                    } while (0)
// ptr is l
#define WriteWordL(ptr, value)      do {*(uint8_t *)((ptr) + 1) = (value) >> 8; \
                                        *(uint8_t *)(ptr)       = (value);      \
                                    } while (0)


#define CMD_SHIFT   1
#define DEST_SHIFT  9
#define SRC_SHIFT   21

#define GET_ID_CMD(id)              (id & ((uint32_t)0xff << 1))

#define GET_ID_DEST(id)             (id & ((uint32_t)0xff << DEST_SHIFT))

#define GET_ID_SRC(id)              (id & ((uint32_t)0xff << 21))

#define WRITE_ID_CMD(id, value)     do {id &= ~((uint32_t)0xff << CMD_SHIFT);   \
                                        id |= value << CMD_SHIFT;               \
                                    } while (0)

#define WRITE_ID_DEST(id, value)    do {id &= ~((uint32_t)0xff << DEST_SHIFT);  \
                                        id |= value << DEST_SHIFT;              \
                                    } while (0)

#define WRITE_ID_SRC(id, value)     do {id &= ~((uint32_t)0xff << SRC_SHIFT);   \
                                        id |= value << SRC_SHIFT;               \
                                    } while (0)

#define IDE_FLAG    0x00080000u

typedef struct {
//    uint32_t RTR        :1;  //0
    uint32_t CmdNum     :8;  //8:1
    uint32_t Target     :8;  //16:9
    uint32_t rsvd0      :2;  //18:17
//    uint32_t IDE        :1;  //19
//    uint32_t STR        :1;  //20
    uint32_t Src        :8;  //28:21
    uint32_t rsvd1      :3;  //31:29
} stCanId;

typedef union {
    uint32_t 	all;
    stCanId     field;
} tuCanId;

#define CMD_MAXLENGTH 32

typedef struct {
    tuCanId can_id;
    uint8_t dlc;
    uint8_t data[CMD_MAXLENGTH];
} stCanPacket;

typedef union {
    float   tFloat;
    int32_t tInt32;
} tuType32Bit;

extern bool com_isopen;
extern int16_t com_num;
extern RT_CAN can_port;
extern CAN_msg msg_tx;
extern CAN_msg msg_rx;
int8_t CAN_Init(uint32_t bitrate);
int8_t SendCommand(uint8_t target_id, uint8_t src_id, uint8_t cmd_num, uint8_t *data, uint8_t dlc, uint16_t timeout);


#endif // CAN_IF_H
