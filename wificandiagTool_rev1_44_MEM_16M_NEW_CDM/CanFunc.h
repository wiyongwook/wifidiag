#ifndef CanFunc_h
#define CanFunc_h

#include <ESP32CAN.h>
#include <CAN_config.h>

#include "SPIFFS.h"
#include <string.h> 
#include "SharedMemory.h" 


#if 0
#define SEND_P  0x71E //0x73E
#define RECV_P  0x73E //0x71E
#define SEND_F  0x7DF
#else
#define SEND_P  0x71E //0x73E
#define RECV_P  0x73E//0x71E
#define SEND_F  0x7DF
#endif

#define CANDIAG_NONE		0
#define CANDIAG_SWVER		1
#define CANDIAG_HWVER		2
#define CANDIAG_SWVER_SUP	3
#define CANDIAG_HWVER_SUP	4
#define CANDIAG_SWVER_DIAG  10

#define CANDIAG_DTCSTATUS_P  11
#define CANDIAG_DTCSTATUS_C  12
#define CANDIAG_DTCSTATUS_A  13

#define CANDIAG_DTCSTATUSCNT_P  14
#define CANDIAG_DTCSTATUSCNT_C  15
#define CANDIAG_DTCSTATUSCNT_A  16

#define ServiceID       0x22
#define ServiceDTCID    0x19
#define ServiceDTCRem   0x14

#define CANDIAG_ECUINFO 53

class CanFuncClass 
{
public:
    void  InitCanSetup(void);
	void  Can_Stop(void);
    void  SendCanDataToECU_Pysical(uint8_t *SendToEcuCanData);
    void  SendCanDataToECU_Functional(uint8_t *SendToEcuCanData);
    uint8_t ReceivedCanDataFromECU(void);
    void  ReceivedCanDataCopy(void);    
    void  ReceivedCanDataParsing(uint8_t RetValueSel);
    void  SendWifiDataCAN(uint8_t  SendSelect);
    void  sendMessageByte(uint8_t *SendEcuCanDataFrame);
	void  sendMessageByteEcuInfoSwVersion(void);
	void  sendMessageByteEcuInfoSwVersionF195(void);
	void  sendMessageByteEcuInfoHwVersion(void);
	void  sendMessageByteEcuInfoSwVersionSup(void);
	void  sendMessageByteEcuInfoHwVersionSup(void);
	void  ECUInfomsgDecoder(byte readmsg[]);

	void sendMessageByteDtcInfoPendingStatus(void);
	void sendMessageByteDtcInfoConfirmingStatus(void);
	void sendMessageByteDtcInfoAllStatus(void);
	void sendMessageByteDtcInfoPendingStatusCnt(void);
	void sendMessageByteDtcInfoConfirmingStatusCnt(void);
	void sendMessageByteDtcInfoAllStatusCnt(void);
	
public:
    CAN_frame_t rx_frame;
    CAN_frame_t tx_frame;
    CAN_frame_t dx_frame;
    uint8_t  CANtXrXStatus ;
    uint8_t  CAN_ComCheck  ;
    uint8_t  CAN_DIAG_MODE_Sel = 0 ;
    uint8_t  DiagModeSel = 0 ;
    uint8_t  SelectP ;
    uint8_t  SelectN ;
    uint8_t  FC ;
    uint16_t Delay ;
    uint16_t DLC =0;
	uint16_t CalDLC = 0;


	uint8_t  Recu_Fragment = 0;
	bool bFlowControl  = false;
    bool bSortComplete = false;
    bool bCompleteCANMessage = false;
	byte InBuffer[100];
	
};

extern CanFuncClass CanFnc;

#endif // CanFunc_h
