#ifndef CanFDFunc_h
#define CanFDFunc_h

#include "CanFDFunc.h" 
#include "WifiFunc.h" 
#include "Reprogramming.h"
#include <SPI.h>
#include <ACAN2517FD.h>
#include <ACAN2517FDFilters.h>
#include <ACAN2517FDSettings.h>
#include <ACANFDBuffer.h>
#include <CANFDMessage.h>
//#include <CANMessage.h>

//#define DEBUG__

class CanFDFuncClass 
{
public:
  void  InitCanFDSetup(void);
  void	SendCanFDDataToECU_Pysical(uint8_t *SendToEcuCanData, uint8_t length);
  void	SendCanFDDataToECU_Functional(uint8_t *SendToEcuCanData, uint8_t length);
  uint8_t ReceivedCanFDDataFromECU(void);
  void	sendMessageByteFD(uint8_t *SendEcuCanDataFrame,uint8_t DataLength);
  void  SendCANFDMessage_10ms(void);

  void  sendMessageFDByteEcuInfoSwVersion(void);
  void  sendMessageFDByteEcuInfoSwVersionF195(void);
  void  sendMessageFDByteEcuInfoHwVersion(void);
  void  sendMessageFDByteEcuInfoSwVersionSup(void);
  void  sendMessageFDByteEcuInfoHwVersionSup(void);
  void  ECUInfomsgFDDecoder(byte readmsg[]);
  
  void sendMessageFDByteDtcInfoPendingStatus(void);
  void sendMessageFDByteDtcInfoConfirmingStatus(void);
  void sendMessageFDByteDtcInfoAllStatus(void);
  void sendMessageFDByteDtcInfoPendingStatusCnt(void);
  void sendMessageFDByteDtcInfoConfirmingStatusCnt(void);
  void sendMessageFDByteDtcInfoAllStatusCnt(void);


public:
	CANFDMessage rx_frame ;
	CANFDMessage tx_frame ;
	CANFDMessage dx_frame ;
	CANFDMessage fdi_frame;

public:
    uint8_t  CANtXrXStatus ;

    uint8_t  CAN_DIAG_MODE_Sel = 0 ;
    uint8_t  DiagModeSel = 0 ;
    uint8_t  SelectP ;
    uint8_t  SelectN ;
    uint8_t  FC ;
    uint16_t Delay ;
    uint16_t DLC =8;
	uint16_t CalDLC = 0;

	uint8_t  Recu_Fragment = 0;
	bool bFlowControl  = false;
    bool bSortComplete = false;
    bool bCompleteCANMessage = false;
	byte InBuffer[100];

};

extern  CanFDFuncClass CanFDFun;

#endif // CanFDFunc_h
