#ifndef CAN_IF_H
#define CAN_IF_H


int CAN_SendFrame(int fd, uint32_t id, const uint8_t *pdata, uint8_t length, uint16_t timeout_ms);
int CAN_RecvFrame(int fd, struct can_frame *pframe, uint16_t timeout_ms);
int CAN_SendBulk(int fd, uint32_t id, const uint8_t *pdata, uint16_t length, uint16_t interval_us);
int CAN_DeInit(int fd_can);
int CAN_Init(const char *name, uint32_t bitrate);


#endif // CAN_IF_H
