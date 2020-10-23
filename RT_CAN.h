#pragma once
#include "rt_com.h"

#define CAN_RESET_ID		0x01FFFEc0		//
#define CAN_ABOM_ID			0x01fffEb0		//
#define CAN_ART_ID			0x01fffEa0		//
#define CAN_FILTER_BASE_ID	0x01fffEe0		//
#define CAN_BAUD_ID			0x01fffEd0		//
#define CAN_INIT_FLASH_ID	0x01fffEff		//

#define CAN_FILTER_ID(x)	(CAN_FILTER_BASE_ID + (x))	//
#define CAN_FILTER_ENABLE   0x80000000		//

#define MAX_CAN_BAUD		1000000			//
#define CAN_CONFIG_CHANNEL	0xff			//
#define FILTER_ID_MASK		0x0000000f		//
#define ENABLE_MASK			0x80000000		//
#define DISABLE_MASK		0				//

typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;		//--
typedef enum {DATA_FRAME = 0, REMOTE_FRAME}         CAN_FRAME;		//--
typedef enum
{
    CAN_ERR_TRANS = 0,		//
    CAN_OK,					//
    CAN_ERR_ACK,			//
    CAN_ERR_CHANNEL,		//
    CAN_ERR_PARAM,			//
    CAN_ERR_TIMEOUT			//
}CAN_ERROR;

//
typedef struct  {
    DWORD id;                 // 29
    BYTE  data[8];            //
    BYTE  len;                //
    BYTE  ch;                 //
    BYTE  format;             //
    BYTE  type;               //
}  CAN_msg;

class RT_CAN :
	public RT_COM
{
public:
	RT_CAN(void);
	~RT_CAN(void);

	CAN_ERROR SetConfigValue(DWORD dwID, void *pValueBuf, DWORD *pdwLen, DWORD dwTimeOut);
	CAN_ERROR GetConfigValue(DWORD dwID, void *pValueBuf, DWORD *pdwLen, DWORD dwTimeOut);
	CAN_ERROR SendCANMessage(CAN_msg *pMsg, DWORD dwTimeout = INFINITE);
	CAN_ERROR RecvCANMessage(CAN_msg *pMsg, DWORD dwTimeout = INFINITE);

	virtual void OnRecvPackage(const BYTE * byBuf, DWORD dwLen);
	virtual DWORD GetConfigDataLen(DWORD dwID);

protected:
	CAN_ERROR SendConfigData(DWORD dwID, void *pBufData, DWORD dwBufLen, DWORD dwTimeout);
	CAN_ERROR RequestConfigData(DWORD dwID, DWORD dwTimeout);

	HANDLE m_hConfigEvnet;
	HANDLE m_hRecvMsgEvnet;

	CAN_msg m_CANConfigMsg;
	CAN_msg m_CANRecvMsg;
};
