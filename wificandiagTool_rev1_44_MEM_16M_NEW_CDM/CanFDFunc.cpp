#include "CanFDFunc.h" 
#include "WifiFunc.h" 
#include "Reprogramming.h"

uint8_t SendMessageFDEcuInfo[5][WDLC] = {{0x03,0x22,0xF1,0xA0,0x55,0x55,0x55,0x55},   // System SW ECUSoftwareVersion
                           //            {0x03,0x22,0xF1,0x95,0x55,0x55,0x55,0x55},   // System SW ECUSHardwareVersion
									  	 {0x03,0x22,0xF1,0x91,0x55,0x55,0x55,0x55},	  // System HW ECUSHardwareVersion
									     {0x03,0x22,0xF1,0x94,0x55,0x55,0x55,0x55},   // System Supplier ECUSoftwareVersion
                                         {0x03,0x22,0xF1,0x92,0x55,0x55,0x55,0x55},
										 {0x03,0x22,0xF1,0x95,0x55,0x55,0x55,0x55}    // System SW ECUSoftwareVersionF195
										 };  // System Supplier ECUHardwareVersion

uint8_t SendMessageFDdtcInfo[6][WDLC] = {{0x03,0x19,0x02,0x01,0x55,0x55,0x55,0x55},   // Pending DTC
									  	 {0x03,0x19,0x02,0x08,0x55,0x55,0x55,0x55},	  // Confirmed DTC
									     {0x04,0x14,0xFF,0xFF,0xFF,0x55,0x55,0x55},   // Pending & Confirmed DTC
										 {0x03,0x19,0x01,0x01,0x55,0x55,0x55,0x55},   // Pending DTC Count
									     {0x03,0x19,0x01,0x08,0x55,0x55,0x55,0x55},   // Confirmed DTC Count
										 {0x04,0x14,0xFF,0xFF,0xFF,0x55,0x55,0x55}};  // Pending & Confirmed DTC Count


static const uint8_t MCP2517_SCK  = 18 ; // SCK input of MCP2517FD
static const uint8_t MCP2517_MOSI = 23 ; // SDI input of MCP2517FD
static const uint8_t MCP2517_MISO = 19 ; // SDO output of MCP2517FD

static const uint8_t MCP2517_CS   = 16 ; // CS input of MCP2517FD
static const uint8_t MCP2517_INT  = 34 ; // INT output of MCP2517FD

SPIClass vspi (VSPI) ; // See ESP32 demo sketch: SPI_Multiple_Busses
ACAN2517FD canfd(MCP2517_CS, vspi, MCP2517_INT);

//--------------------------------------//	 
//RECEIVE FUNCTION
//--------------------------------------// 
void receiveFromFilter0 (const CANFDMessage & inMessage) 
{  
	Serial.println ("Match filter 0") ;
}
//-----------------------------------------
void receiveFromFilter1 (const CANFDMessage & inMessage) 
{ 
	Serial.println ("Match filter 1") ;
}
//-------------------------------------------
void receiveFromFilter2 (const CANFDMessage & inMessage) 
{ 
	Serial.println ("Match filter 2") ;
}


void CanFDFuncClass::InitCanFDSetup(void)
{
	uint32_t inIdentifierSBW;
	
    tx_frame.id  = SharedMemory.P2_ID;
    tx_frame.len = 16;//8;

    dx_frame.id  = SharedMemory.F2_ID; 
    dx_frame.len = 16;//8;
    
    rx_frame.id  = SharedMemory.ECU2_ID;  //SharedMemory.ECU1_ID
    rx_frame.len = 16;//8;

	fdi_frame.id  = 0x710;
	fdi_frame.len = CANFDDLC;

	//----------------------------------- Begin SPI
	vspi.begin (MCP2517_SCK, MCP2517_MISO, MCP2517_MOSI) ;

	//--- Configure ACAN2517FD
	Serial.print ("sizeof (ACAN2517FDSettings): ") ;
	Serial.print (sizeof (ACAN2517FDSettings)) ;
	Serial.println (" bytes") ;
	Serial.println ("Configure ACAN2517FD") ;
	ACAN2517FDSettings settings (ACAN2517FDSettings::OSC_40MHz, 500 * 1000, ACAN2517FDSettings::DATA_BITRATE_x4) ;
	settings.mRequestedMode = ACAN2517FDSettings::NormalFD ; // Select loopback mode

#if 1
//------------------------- Filters ------- 
	ACAN2517FDFilters filters ;  
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		inIdentifierSBW	  = SharedMemory.ECU1_ID;
		break;
	case 2:  // CAN FD
		inIdentifierSBW	  = SharedMemory.ECU2_ID;
		break;
	}
	Serial.printf("inIdentifierSBW --%X\n",inIdentifierSBW) ;
	filters.appendFrameFilter (kStandard, inIdentifierSBW, receiveFromFilter0) ; // Filter #0: receive standard frame with identifier 0x123
//	filters.appendFrameFilter (kStandard, 0x7BE, receiveFromFilter0) ; // Filter #0: receive standard frame with identifier 0x123
//----------------------------------- Filters ok ------- 
	if (filters.filterStatus () != ACAN2517FDFilters::kFiltersOk) {    
		Serial.print ("Error filter ") ;    
		Serial.print (filters.filterErrorIndex ()) ;    
		Serial.print (": ") ;    
		Serial.println (filters.filterStatus ()) ;  
	}	
	//----------------------------------- Filters ok ------- 
#endif

	settings.mDriverTransmitFIFOSize = 2;
	settings.mDriverReceiveFIFOSize = 2;


//	settings.mBitRatePrescaler = 1 ; 
#if 1
	settings.mArbitrationPhaseSegment1 -= 0 ; // 63 -> 55: safe , 1 <= PS1 <= 256
	settings.mArbitrationPhaseSegment2 += 0 ; // 16 -> 24: safe , 1 <= PS2 <= 128
	settings.mArbitrationSJW = settings.mArbitrationPhaseSegment2 ; 	 	   // 16 -> 24: safe , 1 <= SJW <= PS2

	settings.mDataPhaseSegment1 -= 0 ; // 63 -> 55: safe , 1 <= PS1 <= 256
	settings.mDataPhaseSegment2 += 0 ; // 16 -> 24: safe , 1 <= PS2 <= 128
	settings.mDataSJW += 0 ; 	 	   // 16 -> 24: safe , 1 <= SJW <= PS2
//	settings.mBitRatePrescaler += 0 ; 
#else
	settings.mArbitrationPhaseSegment1 = 63 ; // 63 -> 55: safe , 1 <= PS1 <= 256
	settings.mArbitrationPhaseSegment2 = 16 ; // 16 -> 24: safe , 1 <= PS2 <= 128
	settings.mArbitrationSJW = 6 ; 		   // 16 -> 24: safe , 1 <= SJW <= PS2

	settings.mDataPhaseSegment1 = 15 ; // 63 -> 55: safe , 1 <= PS1 <= 256
	settings.mDataPhaseSegment2 = 4 ; // 16 -> 24: safe , 1 <= PS2 <= 128
	settings.mDataSJW = 5 ;		   // 16 -> 24: safe , 1 <= SJW <= PS2
#endif

	//--- RAM Usage
	Serial.print ("MCP2517FD RAM Usage: ") ;
	Serial.print (settings.ramUsage ()) ;
	Serial.println (" bytes") ;

	//--- Begin
	const uint32_t errorCode = canfd.begin (settings, [] { canfd.isr () ; },filters) ;
   
	if (errorCode == 0) 
	{
	  Serial.print ("Bit Rate prescaler: ") ;
	  Serial.println (settings.mBitRatePrescaler) ;
	  
	  Serial.print ("Arbitration Phase segment 1: ") ;
	  Serial.println (settings.mArbitrationPhaseSegment1) ;
	  Serial.print ("Arbitration Phase segment 2: ") ;
	  Serial.println (settings.mArbitrationPhaseSegment2) ;
	  Serial.print ("Arbitration SJW:") ;
	  Serial.println (settings.mArbitrationSJW) ;

	  Serial.print ("Actual Arbitration Bit Rate: ") ;
	  Serial.print (settings.actualArbitrationBitRate ()) ;
	  Serial.println (" bit/s") ;

	  Serial.print ("Data Phase segment 1: ") ;
	  Serial.println (settings.mDataPhaseSegment1) ;
	  Serial.print ("Data Phase segment 2: ") ;
	  Serial.println (settings.mDataPhaseSegment2) ;
	  Serial.print ("Data SJW:") ;
	  Serial.println (settings.mDataSJW) ;
	  
	  Serial.print ("Actual Data Bit Rate: ") ;
	  Serial.print (settings.actualDataBitRate ()) ;
	  Serial.println (" bit/s") ;

	  Serial.print ("Exact Arbitration Bit Rate ? ") ;
	  Serial.println (settings.exactArbitrationBitRate () ? "yes" : "no") ;
	  Serial.print ("Exact Data Bit Rate ? ") ;
	  Serial.println (settings.exactDataBitRate () ? "yes" : "no") ;
	  Serial.print ("Arbitration Sample point: ") ;
	  Serial.print (settings.arbitrationSamplePointFromBitStart ()) ;
	  Serial.println ("%") ;
	  Serial.print ("Data Sample point: ") ;
	  Serial.print (settings.dataSamplePointFromBitStart ()) ;
	  Serial.println ("%") ;
	  Serial.print ("Consistency: 0x") ;
	  Serial.println (settings.CANBitSettingConsistency (), HEX) ; // != 0, meaning error
	}
	else
	{
	  Serial.print ("Configuration error 0x") ;
	  Serial.println (errorCode, HEX) ;
	}

}


void CanFDFuncClass::SendCanFDDataToECU_Pysical(uint8_t *SendToEcuCanData, uint8_t length)
{
  uint8_t i,DataLow[FDMAXLEN];
  bool ok;

  memcpy(DataLow ,SendToEcuCanData, length);

  tx_frame.len = length;
  for(i= 0; i< length; i++)
  {
    tx_frame.data[i] = DataLow[i];
  }

  ok = canfd.tryToSend (tx_frame) ;
  if (ok){ 
//  	Serial.println ("Send Success") ;
  }
  else
  {
  	Serial.println ("Send failure") ;
	
	if(ReprogDataFd.reprogStartStop)
	{	
		ReprogDataFd.reprogExpiredCheck = 1;	
	}
  }

}

void CanFDFuncClass::SendCanFDDataToECU_Functional(uint8_t *SendToEcuCanData, uint8_t length)
{
  uint8_t i,DataLow[FDMAXLEN];
  bool ok;

  memcpy(DataLow ,SendToEcuCanData, length);
  
  dx_frame.len = length;
  for(i= 0; i< 8; i++)
  {
    dx_frame.data[i] = DataLow[i];
  }

  ok = canfd.tryToSend (dx_frame) ;
  if (ok)  {
//  	Serial.println ("Send Success") ;
  }
  else
  {
  	Serial.println ("Send failure") ;

	if(ReprogDataFd.reprogStartStop)
	{	
		ReprogDataFd.reprogExpiredCheck = 1;	
	}
  }
}

void CanFDFuncClass::sendMessageByteFD(uint8_t *SendEcuCanDataFrame,uint8_t DataLength)
{
	uint8_t  SendCanData[WDLC];
	uint8_t  SendCanDataL[TransferAppDLC];
	uint8_t  SendCanDataH[CANFDDLC];
	uint8_t  SendCanDataHalf[HALFRAMEDLC];
	
	uint16_t CAN_iD ;

	// CAN_iD = ( (SendEcuCanDataFrame[0] & 0xff) << 8 || ( SendEcuCanDataFrame[1] & 0xff) );
	CAN_iD = ( (SendEcuCanDataFrame[0] & 0xff) << 8 ); //+	( SendEcuCanDataFrame[1] & 0xff) );
	CAN_iD = ( CAN_iD + SendEcuCanDataFrame[1] );

#if 0
    Serial.printf("CANFD : Send Data\n");
    for(int i = 0; i < DataLength ; i++)	Serial.printf("%02X ,", SendEcuCanDataFrame[i+2]);   Serial.printf("\n"); 
#endif

	if(CAN_iD == SharedMemory.P2_ID || CAN_iD == SharedMemory.F2_ID)  //SharedMemory.ECU1_ID
	{
		switch(DataLength)
		{
		case WDLC:
			if(CAN_iD == SharedMemory.P2_ID)
			{
				memcpy(SendCanData ,&SendEcuCanDataFrame[2], DataLength);
				SendCanFDDataToECU_Pysical(SendCanData,DataLength);
			}
			else
			{
				memcpy(SendCanData ,&SendEcuCanDataFrame[2], DataLength);
				SendCanFDDataToECU_Functional(SendCanData,DataLength);
			}
			break;
		case TransferAppDLC:
			if(CAN_iD == SharedMemory.P2_ID)
			{
#ifdef DEBUG__			
				Serial.printf("CANFD : Send Data\n");
				for(int i = 0; i < DataLength ; i++)	Serial.printf("%02X ,", SendEcuCanDataFrame[i+2]);	 Serial.printf("\n"); 
#endif			
				memcpy(SendCanDataL ,&SendEcuCanDataFrame[2], DataLength);
				SendCanFDDataToECU_Pysical(SendCanDataL,DataLength);
			}
			else
			{
				memcpy(SendCanDataL ,&SendEcuCanDataFrame[2], DataLength);
				SendCanFDDataToECU_Functional(SendCanDataL,DataLength);
			}
			break;  //HALFRAMEDLC
		case HALFRAMEDLC:
			if(CAN_iD == SharedMemory.P2_ID)
			{
#ifdef DEBUG__			
				Serial.printf("HALFRAMEDLC : Send Data\n");
				for(int i = 0; i < DataLength ; i++)	Serial.printf("%02X ,", SendEcuCanDataFrame[i+2]);	 Serial.printf("\n"); 
#endif			
				memcpy(SendCanDataHalf ,&SendEcuCanDataFrame[2], DataLength);
				SendCanFDDataToECU_Pysical(SendCanDataHalf,DataLength);
			}
			else
			{
				memcpy(SendCanDataHalf ,&SendEcuCanDataFrame[2], DataLength);
				SendCanFDDataToECU_Functional(SendCanDataHalf,DataLength);
			}
			break;	//HALFRAMEDLC
		case CANFDDLC:
			if(CAN_iD == SharedMemory.P2_ID)
			{
#ifdef DEBUG__			
				Serial.printf("CANFDDLC - 64 : Send Data\n");
				for(int i = 0; i < DataLength ; i++)	Serial.printf("%02X ,", SendEcuCanDataFrame[i+2]);	 Serial.printf("\n"); 
#endif
				memcpy(SendCanDataH,&SendEcuCanDataFrame[2], DataLength);
				SendCanFDDataToECU_Pysical(SendCanDataH,DataLength);
			}
			else
			{
				memcpy(SendCanDataH,&SendEcuCanDataFrame[2], DataLength);
				SendCanFDDataToECU_Functional(SendCanDataH,DataLength);
			}
			break;
		}
	}
	else
	{
#if 0
		if(DataLength == WDLC){
			memcpy(SendCanData ,&SendEcuCanDataFrame[2], DataLength);
			SendCanFDDataToECU_Pysical(SendCanData,DataLength);
		}
		else{
			memcpy(SendCanDataL ,&SendEcuCanDataFrame[2], DataLength);
			SendCanFDDataToECU_Pysical(SendCanDataL,DataLength);
		}
#endif
	}
}

uint8_t CanFDFuncClass::ReceivedCanFDDataFromECU(void)
{
   uint8_t TX_RX_Check;
   //receive next CAN frame from queue
  if(canfd.available ())
  {
	canfd.receive (rx_frame) ;
	 
	if(rx_frame.id == SharedMemory.ECU2_ID || rx_frame.id == SharedMemory.F2_ID)
	{
#if 0
		memcpy(ReprogDataFd.reprogbuf ,rx_frame.data, WDLC);
#else
		if(CanFDFun.Recu_Fragment > 0)
		{
			switch(CanFDFun.Recu_Fragment)
			{
			case CANDIAG_NONE:
				break;
			case CANDIAG_ECUINFO:
				memcpy(ReprogDataFd.DiagDatabuf ,rx_frame.data, WDLC);
				Serial.print("Received Data-[CANDIAG_ECUINFO] :				  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",ReprogDataFd.DiagDatabuf[i]); Serial.print("\n");
				ECUInfomsgFDDecoder(ReprogDataFd.DiagDatabuf);
				break;
			}

		}
		else
		{
	       memcpy(ReprogDataFd.reprogbuf ,rx_frame.data, WDLC);
		}
#endif
#if 0 
if(ReprogDataFd.stepLock == ESP_RequestTransferExit)
{
        Serial.printf("CABFD From 0x%08x, DLC %d\n",rx_frame.id,  rx_frame.len);
        for(int i = 0; i < rx_frame.len ; i++)  Serial.printf("%02X ,", (char)rx_frame.data[i]); 
		Serial.printf("\n"); 
}
#endif	
		TX_RX_Check = true;
//		ReprogDataFd.timerTick =0;
	}
#if 0      
	else if(rx_frame.id != 0x0 )
	{
		memcpy(ReprogDataFd.reprogbufFDSize ,rx_frame.data, rx_frame.len);
		Serial.printf("CABFD From 0x%08x, DLC %d\n",rx_frame.id,  rx_frame.len);
		for(int i = 0; i < rx_frame.len ; i++)	Serial.printf("%02X ,", (char)rx_frame.data[i]); 
		Serial.printf("\n"); 
		TX_RX_Check = false;
	}
#endif	
  }
  return TX_RX_Check;
}

void CanFDFuncClass::SendCANFDMessage_10ms(void)
{
	uint8_t  SendFDInitCanDataL[CANFDDLC];
	bool ok;

//	 memcpy(DataLow ,SendToEcuCanData, WDLC);
     memset(SendFDInitCanDataL ,0x00, CANFDDLC);

	 fdi_frame.len = CANFDDLC;
	 fdi_frame.brs = 1;
	 fdi_frame.fdf = 1;
	 fdi_frame.rtr = 0;
	 fdi_frame.ext = 0;
	 fdi_frame.id  = 0x710;

	 fdi_frame.pad();

	 for(int i= 0; i< CANFDDLC; i++)
	 {
	    fdi_frame.data[i] = SendFDInitCanDataL[i];
	 }

	 ok = canfd.tryToSend (fdi_frame) ;
	 if (ok)  {
	  	Serial.println ("Send Success") ;
	 }
	 else  Serial.println ("Send failure") ;
  

}


void CanFDFuncClass::sendMessageFDByteEcuInfoSwVersion(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;


  CalDLC = 0;
  DLC 	 = 0;

//SendCanFDDataToECU_Pysical(SendMessage[ReadDataByIdentifier_ECUSoftwareVersion]);
  SendCanFDDataToECU_Pysical(SendMessageFDEcuInfo[ECUSoftwareVersion],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDEcuInfo[ECUSoftwareVersion][i]); Serial.print("\n");

}


void CanFDFuncClass::sendMessageFDByteEcuInfoHwVersion(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

//SendCanDataToECU_Pysical(SendMessageFDEcuInfo[ECUHardwareVersion]);
  SendCanFDDataToECU_Pysical(SendMessageFDEcuInfo[ECUHardwareVersion],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDEcuInfo[ECUHardwareVersion][i]); Serial.print("\n");
}

void CanFDFuncClass::sendMessageFDByteEcuInfoSwVersionSup(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC 	 = 0;

//SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier_ECUSoftwareVersion]);
  SendCanFDDataToECU_Pysical(SendMessageFDEcuInfo[SUPECUSoftwareVersion],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDEcuInfo[SUPECUSoftwareVersion][i]); Serial.print("\n");
}

void CanFDFuncClass::sendMessageFDByteEcuInfoSwVersionF195(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC 	 = 0;

//SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier_ECUSoftwareVersion]);
  SendCanFDDataToECU_Pysical(SendMessageFDEcuInfo[ECUSoftwareVersionF195],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDEcuInfo[ECUSoftwareVersionF195][i]); Serial.print("\n");
}


void CanFDFuncClass::sendMessageFDByteEcuInfoHwVersionSup(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

//SendCanDataToECU_Pysical(SendMessageFDEcuInfo[ECUHardwareVersion]);
  SendCanFDDataToECU_Pysical(SendMessageFDEcuInfo[SUPECUHardwareVersion],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDEcuInfo[SUPECUHardwareVersion][i]); Serial.print("\n");
}

void CanFDFuncClass::sendMessageFDByteDtcInfoPendingStatus(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanFDDataToECU_Pysical(SendMessageFDdtcInfo[DTCStatusPending],WDLC);
//SendCanFDDataToECU_Pysical(SendMessageFDdtcInfo[DTCStatusALL],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDdtcInfo[DTCStatusPending][i]); Serial.print("\n");
}

void CanFDFuncClass::sendMessageFDByteDtcInfoConfirmingStatus(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanFDDataToECU_Pysical(SendMessageFDdtcInfo[DTCStatusConfirming],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDdtcInfo[DTCStatusConfirming][i]); Serial.print("\n");
}

void CanFDFuncClass::sendMessageFDByteDtcInfoAllStatus(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanFDDataToECU_Pysical(SendMessageFDdtcInfo[DTCStatusALL],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDdtcInfo[DTCStatusALL][i]); Serial.print("\n");
}



void CanFDFuncClass::sendMessageFDByteDtcInfoPendingStatusCnt(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanFDDataToECU_Pysical(SendMessageFDdtcInfo[DTCStatusPendingCnt],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDdtcInfo[DTCStatusPendingCnt][i]); Serial.print("\n");
}

void CanFDFuncClass::sendMessageFDByteDtcInfoConfirmingStatusCnt(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanFDDataToECU_Pysical(SendMessageFDdtcInfo[DTCStatusConfirmingCnt],WDLC);
  Serial.print("Send Data-[SendCanFDDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDdtcInfo[DTCStatusConfirmingCnt][i]); Serial.print("\n");
}

void CanFDFuncClass::sendMessageFDByteDtcInfoAllStatusCnt(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;
  if(RFileFnc.DtcAllClearState){
	SendCanFDDataToECU_Functional(SendMessageFDdtcInfo[DTCStatusALLCnt],WDLC);
  	Serial.print("Send Data-[SendCanFDDataToECU--FUNCTIONAL[7DF]] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDdtcInfo[DTCStatusALLCnt][i]); Serial.print("\n");
  }else{
    SendCanFDDataToECU_Functional(SendMessageFDdtcInfo[DTCStatusALLCnt],WDLC);    //2020.05.06 modify by yongwookwi
//	SendCanFDDataToECU_Pysical(SendMessageFDdtcInfo[DTCStatusALLCnt],WDLC);
	Serial.print("Send Data-[SendCanFDDataToECU--PYSICAL[7BE]] :			");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageFDdtcInfo[DTCStatusALLCnt][i]); Serial.print("\n");
  }
  
}

void CanFDFuncClass::ECUInfomsgFDDecoder(byte readmsg[])
{
//	String stReadSplit = readmsg.split(" ");
//	System.out.println("This is Input ECU Information :" + readmsg);
//	Serial.println("This is Input ECU Information :" + (char *)readmsg);
	if ((readmsg[0] == 0x03) && (readmsg[1] == 0x7F)) 
	{
//		  Toast.makeText(getActivity(), "NEGATIVE RESPONSE IS RECEIVED!!", Toast.LENGTH_SHORT).show();
		  Serial.println("NEGATIVE RESPONSE IS RECEIVED :");
	}
	else
	{
		/* First Frame Check*/
		if (bFlowControl == true) 
		{
			byte tmpStr[8] ={0x30,0x08,0x14,0x55,0x55,0x55,0x55,0x55};
			SendCanFDDataToECU_Pysical(tmpStr,WDLC);
			if (readmsg[0] == 0x10) 
			{
				DLC = readmsg[1] ;
				DLC = DLC - 3;
//				Log.d(TAG, Integer.toString(DLC));
			}
			for (int i = 5; i < 8; i++) {
				InBuffer[CalDLC] = readmsg[i];
				CalDLC++;
			}
			bFlowControl = false;
		}
		else
		{
			/* Consecutive Frame Response Check */
			if (DLC != 0) 
			{
				for (int i = 1; i < 8; i++) 
				{
					InBuffer[CalDLC] = readmsg[i];
					CalDLC++;
		
					if (CalDLC == DLC) 
					{
						bSortComplete = true;
						break;
					}
				}
			} 
			else 
			{
				/* Single Frame Response Check */
//				DLC = Integer.parseInt(readmsg[2]);
				if(readmsg[1] == 0x40 + ServiceID)
				{
					DLC = readmsg[0];
					DLC = DLC - 3 ;
			
					for (int i = 0; i < DLC; i++) 
					{
						InBuffer[i] = readmsg[i+4];
					}
					bSortComplete = true;
				}
				else if(readmsg[1] == 0x40 + ServiceDTCID)
				{
					DLC = readmsg[0];
					DLC = DLC - 3 ;
			
					for (int i = 0; i < DLC; i++) 
					{
						InBuffer[i] = readmsg[i+5];
					}
					bSortComplete = true;
				}
				else if(readmsg[1] == 0x40 + ServiceDTCRem)
				{
					DLC = readmsg[0];
					DLC = DLC - 1 ;
			
					for (int i = 0; i < DLC; i++) 
					{
						InBuffer[i] = readmsg[i+4];
					}
					bSortComplete = true;
					SharedMemory.DtcPNum = 0;
					SharedMemory.DtcHNum = 0;
				}
				
			}
			if (bSortComplete == true) 
			{
//				sortingInfo(InBuffer, RequestInfo);
//				RequestInfo = 0;
				bSortComplete = false;
				bCompleteCANMessage = true;

			    Recu_Fragment = CANDIAG_NONE;
				Serial.print("bSortComplete : ");  for(int i=0;i<DLC;i++) Serial.printf("%02X ",InBuffer[i]); Serial.print("\n");
			}
		}
	}
}
