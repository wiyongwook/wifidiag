#include "Reprogramming.h"
#include "CanFunc.h" 
#include "TftLCDFun.h"

//#define BUFS     	  512
//#define BLOCKBUFS     512
//#define BLOCKBUFSDATA 512

#define BUFS     	  4093
#define BLOCKBUFS     4093
#define BLOCKBUFSDATA 4093

byte TesterPresent[8] = {(byte) 0x02, (byte) 0x3E, (byte) 0x80, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55};
byte DownloadStep[20][8] = {
        {(byte) 0x03, (byte) 0x22, (byte) 0xF1, (byte) 0x89, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //0  ReadDataByIdentifier_ECUSoftwareVersion
        {(byte) 0x02, (byte) 0x10, (byte) 0x03, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //1  DiagnosticSessionControl_Extended_Session
        {(byte) 0x02, (byte) 0x85, (byte) 0x82, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //2  ControlDTCSetting_OFF
        {(byte) 0x03, (byte) 0x28, (byte) 0x83, (byte) 0x03, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //3  CommunicationControl_disableRxAndTx
        {(byte) 0x02, (byte) 0x10, (byte) 0x02, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //4  DiagnosticSessionControl_programmingSession
        {(byte) 0x02, (byte) 0x27, (byte) 0x01, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //5  SecurityAccess_Reques_Seed
        {(byte) 0x06, (byte) 0x27, (byte) 0x02, (byte) 0xD2, (byte) 0x93, (byte) 0x00, (byte) 0x1F, (byte) 0x55},  //6  SecurityAccess_Send_KEY
        {(byte) 0x05, (byte) 0x31, (byte) 0x01, (byte) 0xFF, (byte) 0x00, (byte) 0x00, (byte) 0x55, (byte) 0x55},  //7  RoutineControl_eraseMemory_APP

		{(byte) 0x10, (byte) 0x0B, (byte) 0x34, (byte) 0x00, (byte) 0x44, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //8
        {(byte) 0x21, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x55, (byte) 0x55},  //9

		{(byte) 0x01, (byte) 0x37, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //10 Transfer_App_File
        {(byte) 0x04, (byte) 0x31, (byte) 0x01, (byte) 0xFF, (byte) 0x01, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //11 RoutineControl_checkprogrammingdependency
        {(byte) 0x02, (byte) 0x11, (byte) 0x81, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //12 ECUReset_hardReset
        {(byte) 0x02, (byte) 0x10, (byte) 0x90, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //13 DiagnosticSessionControl_Extended_Session
        {(byte) 0x02, (byte) 0x29, (byte) 0x02, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //14 CommunicationControl_Enable_RxAndTx
        {(byte) 0x02, (byte) 0x85, (byte) 0x81, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //15 ControlDTCSetting_On
        {(byte) 0x02, (byte) 0x10, (byte) 0x81, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //16 DiagnosticSessionControl
        {(byte) 0x03, (byte) 0x22, (byte) 0xF1, (byte) 0x89, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //17 ReadDataByIdentifier_ECUSoftwareVersion
        {(byte) 0x30, (byte) 0x08, (byte) 0x14, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //18 ReadDataByIdentifier_Flow_Control

		{(byte) 0x04, (byte) 0x14, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0x55, (byte) 0x55, (byte) 0x55}}; //19 Clear DTC ..Functional 
		

 
 void ReprogrammingClass::FlashDownload(void) 
 {
	  byte MultiFrame[11] = { (byte)0x34,(byte)0x00,(byte)0x44,ReprogData.Address[0],ReprogData.Address[1],ReprogData.Address[2],ReprogData.Address[3],
															   ReprogData.HexSize[0],ReprogData.HexSize[1],ReprogData.HexSize[2],ReprogData.HexSize[3] };
	  int tmpbuf =0;
 
	  //22 F1 89 , Response is need.
	  if(ReprogData.stepLock==0)
	  {
//		  Serial.println("[INIT ESP_ReadDataByIdentifier_ECUSoftwareVersion]-------");
#if 1     
		 switch(ReprogData.stepFirst)
		 {
			 case ESP_ReadDataByIdentifier_ECUSoftwareVersion:							  // F1 89
	//		 	 ReprogData.timerTick = 0;
	//			 Serial.printf("[PhysicalID %X]-------[ESP_ReadDataByIdentifier_ECUSoftwareVersion] \n",PhysicalID);
				 tmpbuf = singleFirstFrameResp(PhysicalID, DownloadStep[0],NORMALTIMECNT*4);
				 if(tmpbuf == OK)
				 {
					 ReprogData.stepFirst = ESP_FlowControlFrame;
				 }
				 else if(tmpbuf == TIMEOUT)
				 {
					 ReprogData.reprogExpiredCheck = OVERTIME;
				 }
				 else if(tmpbuf == OKNOFLOW)
				 {
					 ReprogData.reprogAPPtoFBL = OK;
					 ReprogData.stepLock	= ESP_DiagnosticSessionControl_Extended_Session;
					 ReprogData.stepLock1   = 0;
					 ReprogData.stepFirst   = ESP_FlowControlFrame+1;
					 Serial.println("[ReprogrammingClass]-------[NO FLOW CONTROL --- O.K]");
				 }
				 break;				 
			 case ESP_FlowControlFrame: 												 // 0x30, 0x00,  0x14, 
 // 			 Serial.printf("ESP_FlowControlFrame ---%d \n",ReprogData.stepLock );
				 tmpbuf = FlowControlFrameResp(PhysicalID, DownloadStep[18],NORMALTIMECNT);
//				 tmpbuf = FlowControlFrameResp(PhysicalID, DownloadStep[18],2000);
				 if(tmpbuf==OK)
				 {
					 if((ReprogData.reprogbuf[0] == 0x21) /*&& (ReprogData.reprogbuf[5] == 0x30) && (ReprogData.reprogbuf[6]==0x31)*/ ){
						 ReprogData.reprogAPPtoFBL = 1;
					 }
					 else if((ReprogData.reprogbuf[0] == 0x22) /*&& (ReprogData.reprogbuf[5] == 0x00)(*/ )
					 {
						 ReprogData.reprogAPPtoFBL = OK;
						 ReprogData.stepLock   = ESP_DiagnosticSessionControl_Extended_Session;
						 ReprogData.stepLock1  = 0;
						 ReprogData.stepFirst  = ESP_FlowControlFrame+1;
 // 					 Serial.printf(" ReprogData.reprogbuf[0] --0000000-%d [ReprogData.reprogAPPtoFBL]---%d\n",ReprogData.reprogbuf[0],ReprogData.reprogAPPtoFBL );
					 }else{
						 ReprogData.reprogAPPtoFBL = NOK;
					 }
					 //ReprogData.timerTick =0;
				 }
				 else if(tmpbuf == TIMEOUT)
				 {
					 ReprogData.reprogExpiredCheck = OVERTIME;
					 return;
				 }else{
			 
				 }
				 
			 default:
				 break;
		 }
#endif
	  }
	  
	 if(ReprogData.reprogAPPtoFBL == OK)
	 {
		 switch(ReprogData.stepLock)
		 {
			 case ESP_DiagnosticSessionControl_Extended_Session:					             // 10 90
				 tmpbuf = singleFrameResp(FunctionalID,DownloadStep[1],NORMALTIMECNT);		 // 10 90
				 if(tmpbuf == OK)
				 {
					 ReprogData.stepLock = ESP_ControlDTCSetting_OFF;
  				 	 Serial.printf("ESP_DiagnosticSessionControl_Extended_Session ---%d \n",ESP_DiagnosticSessionControl_Extended_Session );
				 }
				 else if(tmpbuf == TIMEOUT)
				 {
					ReprogData.reprogExpiredCheck = OVERTIME;
				 }				 
				 break;
			 case ESP_ControlDTCSetting_OFF:
				 tmpbuf = singleFrameNoResp(FunctionalID,DownloadStep[2],NORMALTIMECNT);		 // 85 82
				 if(tmpbuf == OK)
				 {
					 ReprogData.stepLock = ESP_CommunicationControl_disableRxAndTx;
					 Serial.printf("ESP_ControlDTCSetting_OFF ---%d \n",ESP_ControlDTCSetting_OFF );
				 }
				 break;
			  case ESP_CommunicationControl_disableRxAndTx:
				 tmpbuf = singleFrameNoResp(FunctionalID,DownloadStep[3],NORMALTIMECNT);		 // 28 02
				 if(tmpbuf == OK)
				 {
					 ReprogData.stepLock = ESP_DiagnosticSessionControl_programmingSession;
					 Serial.printf("ESP_CommunicationControl_disableRxAndTx ---%d \n",ESP_CommunicationControl_disableRxAndTx );

					 ReprogData.stepLock1  = 0;
					 ReprogData.ProcessStep= 0;
				 }
				 break;
			  case ESP_DiagnosticSessionControl_programmingSession:
				 tmpbuf = singleFramePendingResp(PhysicalID,DownloadStep[4],DDTIMECNT);
				 if(tmpbuf == OK)
				 {
					 ReprogData.stepLock = ESP_SecurityAccess_Request_Seed;//ESP_SecurityAccess_Request_Seed; //ESP_WaitFunction;
					 Serial.printf("ESP_DiagnosticSessionControl_programmingSession ---%d \n",ESP_DiagnosticSessionControl_programmingSession );
				 }
				 if(tmpbuf == TIMEOUT)
				 {													//10 85 , Response is not need.
					 ReprogData.reprogExpiredCheck =1;
     				 Serial.printf("ESP_DiagnosticSessionControl_programmingSession -TIME OUT--%d \n",ESP_DiagnosticSessionControl_programmingSession );
					 return;
				 }else{
				 
                 }
				 break; 			   
			 case ESP_SecurityAccess_Request_Seed:
				 tmpbuf = singleFrameSecurityResp(PhysicalID,DownloadStep[5],DOUBLETIMECNT,1);		 // 27 01
				 if(tmpbuf ==  OK)
				 {
					 ReprogData.stepLock = ESP_SecurityAccess_Send_KEY;
					 Serial.printf("ESP_SecurityAccess_Request_Seed ---%d \n",ESP_SecurityAccess_Request_Seed );
				 }
				 else if(tmpbuf == TIMEOUT)
				 {
					 ReprogData.reprogExpiredCheck = OVERTIME;
					 return;
			 	 }else{
 
				 }
				 break; 			 
			 case ESP_SecurityAccess_Send_KEY:
				 tmpbuf = singleFrameSecurityResp(PhysicalID,DownloadStep[6],DOUBLETIMECNT,2);		 // 27 02
//				 tmpbuf = singleFrameResp(PhysicalID,DownloadStep[6],DOUBLETIMECNT);		 // 27 02
				 if( tmpbuf == OK)
				 {
					 ReprogData.stepLock = ESP_RoutineControl_eraseMemory_APP;
					 Serial.printf("ESP_SecurityAccess_Send_KEY ---%d \n",ESP_SecurityAccess_Send_KEY );
				 }
				 else if(tmpbuf ==  TIMEOUT)
				 {
					 ReprogData.reprogExpiredCheck = OVERTIME;
					 return;
				 }else{
 
				 }
				 break;
			 case ESP_RoutineControl_eraseMemory_APP:
//				 tmpbuf = singleFramePendingResp(PhysicalID,DownloadStep[7],DDTIMECNT);//31 01 FF 00 XX, Response is not need.
				 tmpbuf = singleFramePendingRespLast(PhysicalID,DownloadStep[7],DDTIMECNT);//31 01 FF 00 XX, Response is not need.
//   			 Serial.printf("ESP_RoutineControl_eraseMemory_APP----%d \n",tmpbuf);
				 if(tmpbuf == OK)
				 {
					 ReprogData.stepLock = ESP_Transfer_App_File;
					 Serial.printf("ESP_RoutineControl_eraseMemory_APP ---%d \n",ESP_RoutineControl_eraseMemory_APP );
				 }
				 else if(tmpbuf == TIMEOUT)
				 {
					 ReprogData.reprogExpiredCheck = OVERTIME;
					 return;
				 }else{
 
				 }
				 break;
			  case ESP_Transfer_App_File:
//			     tmpbuf = DoubleFrameResp(PhysicalID,TransferAppFrame, sizeof(TransferAppFrame),NORMALTIMECNT);
				 memcpy(&MultiFrame[3],SharedMemory.Address,sizeof(SharedMemory.Address));
				 memcpy(&MultiFrame[7],SharedMemory.HexSize,sizeof(SharedMemory.HexSize));

				 tmpbuf = MultiFrameSend(PhysicalID, MultiFrame, sizeof(MultiFrame));

				 Serial.printf("ESP_Transfer_App_File----%d [Address[4]] %x %x %x %x\n",tmpbuf,SharedMemory.Address[0],SharedMemory.Address[1],SharedMemory.Address[2],SharedMemory.Address[3]);
				 Serial.printf("ESP_Transfer_App_File----%d [HexSize[4]] %x %x %x %x\n",tmpbuf,SharedMemory.HexSize[0],SharedMemory.HexSize[1],SharedMemory.HexSize[2],SharedMemory.HexSize[3]);
				 
				 if(tmpbuf == OK)
				 {
					 ReprogData.stepLock    = ESP_Transfer_App_Reprogramming;
					 ReprogData.ProcessStep = 0;

					 String dnHSFileName ;
					 Serial.printf("ESP_Transfer_App_File----%d \n");
					 int lenT = SharedMemory.downloadfileName.length();
					 dnHSFileName = SharedMemory.downloadfileName.substring(0, lenT -1);	 
					 Serial.println("[R: dnHSFileName] : "+ dnHSFileName); //ReprogData.reprogHexSize
					 Serial.printf("[ReprogData.reprogHexSize] %d [respSta]%d \n",ReprogData.reprogHexSize,respSta); 
					 RepHSFile = RFileFnc.OpenFileFD(dnHSFileName);
					 
					 WaitFunction(1000);
				 }
				 else if(tmpbuf == TIMEOUT)
				 {
					 ReprogData.reprogExpiredCheck = OVERTIME;
					 return;
				 }else{
 
				 }
				 break;
					 
			  default: 
				 break;
			  
		 }
	 }
	 
	 if(ReprogData.stepLock == ESP_Transfer_App_Reprogramming)
	 {
#ifdef CANFDSENDTEST__
			 if(!RepFileTest){
				 Serial.printf("[ESP_Transfer_App_Reprogramming] %d [SharedMemory.FileSize] %d \n",ReprogData.stepLock,SharedMemory.FileSize);
				 Serial.println("[downloadfileName] : "+ SharedMemory.downloadfileName);
				 String TestName = "/RG3_C_01.bin";
				 int len = SharedMemory.downloadfileName.length();
				 SharedMemory.downloadfileName = SharedMemory.downloadfileName.substring(0, len -1);	 
#if 1
				 RepFileTest = RFileFnc.OpenFileFD(SharedMemory.downloadfileName);
		 // 	 RepFileTest = RFileFnc.OpenFileFD("/RG3_C_01.bin");
#else
				 RepFileTest = SPIFFS.open(SharedMemory.downloadfileName);
#endif		
				 if (!RepFileTest){  // can not open file										 
				   Serial.println("# [Init]----ERROR: can not open file");
				   delay(100);
				 }
			 }
#endif
	 
		 if( ReprogData.reprogHexSize!= 0)
		 {
			 if(respSta == 1)
			 {
				 ReprogData.reprogStartIdx = ReprogData.reprogEndIdx;
				 ReprogData.reprogSeq 	   = ReprogData.reprogSeq + 1;
		 
				if (ReprogData.reprogHexSize >= BLOCKBUFSDATAFD)
				{
					ReprogData.reprogEndIdx = ReprogData.reprogEndIdx + BLOCKBUFSDATAFD;
					FDDLC = BLOCKBUFSFD + 2  ;

					//TxBuffer = RFileFnc.StringToStringArray(ReprogData.reprogSeq - 1, BLOCKBUFSDATA, 0);
					//					for(int i=0;i <BLOCKBUFSDATA ;i++) Serial.printf("%02X",ReprogmmingData.TxBuffer[i]);
#ifdef CANFDSENDTEST__
					filelength = getFileData(RepFileTest, SendBuf, FDDLC , 0);
#else
					filelength = getFileData(RepHSFile, SendBuf, FDDLC , 0);
#endif

#ifndef DEBUG_PRINTF__	
					Serial.printf("# [getFileData]--%d\n",filelength);
#endif
				}
				else
				{
					FDDLC = ReprogData.reprogHexSize + 2 ;
#ifdef CANFDSENDTEST__
					filelength = getFileData(RepFileTest, SendBuf, FDDLC , 0);
#else
					filelength = getFileData(RepHSFile, SendBuf, FDDLC , 0);
#endif

#ifndef DEBUG_PRINTF__	
					Serial.printf("#[LAST] [getFileData]--%d\n",filelength);
#endif
					LastloopCnt = 0x01;
				}				
				respTimeCount=0;
				respSta =10;
			}

			respSta=ReprogMultiFrameHsSend(PhysicalID,SendBuf,FDDLC,ReprogData.reprogSeq); //ReprogMultiFrameHsSend

	 		if (ReprogData.timerTick >= TIMEMAXCNTHS)
			{
				ReprogData.reprogExpiredCheck = 1;
				ReprogData.timerTick = 0;
				
			 // Configure SPI CANFD Init Setup ...
			 // CanFDFun.InitCanFDSetup();  
			}		
		 }
		 else
		 {
			 filelength = getFileData(RepHSFile, SendBuf, FDDLC , 1);
			 
			 RepHSFile.close();
			 vfClearAllEnv();
			 Serial.printf("ESP_Transfer_App_Reprogramming--[ ReprogData.reprogHexSize] %d \n", ReprogData.reprogHexSize);
			 ReprogData.stepLock = ESP_RequestTransferExit;
		 }
	 }
 
	   switch(ReprogData.stepLock)
	   {
		 case ESP_RequestTransferExit:
			 tmpbuf = singleFramePendingRespLast(PhysicalID,DownloadStep[10],DDTIMECNT);//31 01 FF 00 XX, Response is not need.
//			 tmpbuf = singleFramePendingResp(PhysicalID,DownloadStep[10],DDTIMECNT);		 //37
			 if(tmpbuf == OK)
			 {
				  Serial.printf("ESP_RequestTransferExit----%d \n",tmpbuf);
				  ReprogData.stepLock =ESP_RoutineControl_checkprogrammingdependency;
//				  vfClearEnv();
			 }						 
			 else if(tmpbuf == TIMEOUT)
			 {														 //10 85 , Response is not need.
				 ReprogData.reprogExpiredCheck = OVERTIME;
				 return;
			 }else{
 
			 }
			 break;
		 case ESP_RoutineControl_checkprogrammingdependency:
			 tmpbuf =singleFrameResp(PhysicalID,DownloadStep[11],T3CNTN);				 // 31 01 FF 01
			 if(tmpbuf == OK)
			 {
				  Serial.printf("ESP_RoutineControl_checkprogrammingdependency----\n");
				  ReprogData.stepLock =ESP_ECUReset_HardReset;
			 }else if(tmpbuf == TIMEOUT ){														 //10 85 , Response is not need.
				 ReprogData.reprogExpiredCheck = OVERTIME;
				 return;
			 }else{ 
			 }
			 break;
		 case ESP_ECUReset_HardReset:
			 tmpbuf = singleFrameNoResp(PhysicalID,DownloadStep[12],T3CNTN);			 //11 81
			 if(tmpbuf == OK)
			 {
				 Serial.printf("ESP_ECUReset_HardReset---- \n");
				 ReprogData.stepLock = ESP_DiagnosticSessionControl_Extended_Session2;
			 }
			 break;
		 case ESP_DiagnosticSessionControl_Extended_Session2:
			 tmpbuf = singleFrameNoResp( FunctionalID,DownloadStep[13],T3CNTN);		 //10 90
			 if(tmpbuf == OK)
			 {
				 Serial.printf("ESP_DiagnosticSessionControl_Extended_Session2----\n");
				 ReprogData.stepLock = ESP_CommunicationControl_Enable_RxAndTx;
			 }
			 break;  
		 case ESP_CommunicationControl_Enable_RxAndTx:
			 tmpbuf = singleFrameNoResp(FunctionalID,DownloadStep[14],T3CNTN); 		 //29 02
			 if(tmpbuf == OK)
			 {
				 Serial.printf("ESP_CommunicationControl_Enable_RxAndTx----\n");
				 ReprogData.stepLock = ESP_ControlDTCSetting_ON;
			 }
			 break;
		 case ESP_ControlDTCSetting_ON:
			 tmpbuf =singleFrameNoResp(FunctionalID,DownloadStep[15],100);			 //85 81
			 if(tmpbuf == OK){
				 Serial.printf("ESP_ControlDTCSetting_ON----\n");
				 ReprogData.stepLock =ESP_DiagnosticSessionControl_Standard;
			 }
			 break;
		 case ESP_DiagnosticSessionControl_Standard:
			 tmpbuf = singleFrameNoResp(FunctionalID,DownloadStep[16],T3CNTN); 		 //10 81
			 if(tmpbuf == OK)
			 {
				 Serial.printf("ESP_DiagnosticSessionControl_Standard----\n");
				 ReprogData.stepLock =ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last;
			 }
			 break;
		 case ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last:
			 tmpbuf = singleFrameNoResp(PhysicalID,DownloadStep[17],T3CNTN);			 //22 F1
			 
			 TftLCDFun.progresspercentSuccess = 1; 

			 if(tmpbuf == OK)
			 {
//				 RFileFnc.CloseFile();
				 vfClearAllEnv();
 				 ReprogData.stepLock =0;
				 ReprogData.reprogStartStop=0;
				 Serial.printf("[THE END] ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last--[ReprogData.reprogStartStop ]-%d\n",ReprogData.reprogStartStop);
				 tmpbuf = singleFrameNoResp(FunctionalID,DownloadStep[19],NORMALTIMECNT);		// 04 14 Clear DTC...
				 Serial.println("[THE END] CLEAR_DTC_ALL--[0x04, 0x14, 0xFF, 0xFF, 0xFF, 0x55, 0x55, 0x55 ]-----The End");
			 }
			 break; 		 
		 default:
			 break;
				 
	 }		 
	  
}
 
int ReprogrammingClass::WaitFunction(int counter){
    int Resp = 0;

	for(int i=0;i < counter * 100; i++){}
	if(ReprogData.timerTick >=counter){
        ReprogData.timerTick = 0;
        Resp = OK;
    }
    return Resp;
}
    
void ReprogrammingClass::ReprogRxBufClear(void){
#if 1
  for(int i=0;i<sizeof(ReprogData.reprogbuf);i++){
      ReprogData.reprogbuf[i] = (byte)0x00;
	  
  }
#else
   memset(ReprogData.reprogbuf ,0x00, 8);
   memset(CanFnc.rx_frame.data.u8,0x00, 8);
#endif
}

void ReprogrammingClass::InitReprogrammingClass(void)
{
	byte* TxBuffer=new byte[BUFS/2];

    if(SharedMemory.File_CAN_TYPE == 0x01)
   	{
		PhysicalID    = SharedMemory.P1_ID;
	    FunctionalID  = SharedMemory.F1_ID;
	    ECUID         = SharedMemory.ECU1_ID;
   	}
	else if(SharedMemory.File_CAN_TYPE == 0x02)
	{
		PhysicalID2    = SharedMemory.P2_ID;
	    FunctionalID2  = SharedMemory.F2_ID;
	    ECUID2         = SharedMemory.ECU2_ID;
	}
	
	Serial.printf("[PhysicalID]  ------%x \n",PhysicalID);
	Serial.printf("[FunctionalID]------%x \n",FunctionalID);
	Serial.printf("[ECUID]       ------%x \n",ECUID);

	Serial.printf("[PhysicalID2]  ------%x \n",PhysicalID2);
	Serial.printf("[FunctionalID2]------%x \n",FunctionalID2);
	Serial.printf("[ECUID2]       ------%x \n",ECUID2);

}

int ReprogrammingClass::singleFramePendingResp(int id, byte* data, int counter){

byte sendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], data[2],data[3],data[4],data[5],data[6],data[7]};

int Resp = NOK;

  if(ReprogData.stepLock1 == 0)
  {
      ReprogRxBufClear();
      CanFnc.sendMessageByte(sendFrame);
	  ReprogData.timerTick   = 0;
	  ReprogData.ProcessStep = 0;	  
      ReprogData.stepLock1   = 1;
//	  Serial.println("[singleFramePendingResp]-------ReprogData.stepLock2 ==0");
  }

  if(ReprogData.ProcessStep ==0)
  {
//	Serial.printf("[ReprogData.reprogbuf[1]]-------%x ---\n",ReprogData.reprogbuf[1]);

	if((ReprogData.reprogbuf[1]== 0x7F)&&(ReprogData.reprogbuf[3]== 0x78))
	{
//      singleFrameNoResp(PhysicalID,TesterPresent,50);
        Resp = NOK;
        ReprogData.timerTick =0;
		ReprogRxBufClear();
//		Serial.println("[singleFramePendingResp]-------ReprogData.reprogbuf[1]== 0x7F)&&(ReprogData.reprogbuf[3]== 0x78");
    }
    if((ReprogData.reprogbuf[1] == (data[1] + 0x40))) 
	{
//      ReprogData.ProcessStep =0;
        ReprogData.timerTick=0;
//      ReprogData.stepLock2 =0;
        ReprogData.stepLock1 =0;
        Resp = OK;
    }

    if(ReprogData.timerTick >= counter){ //Timing is expired
        ReprogData.timerTick   = 0;
		ReprogData.PendingTick = 1;
        Resp = TIMEOUT;
    }
  }
  return Resp;
}

int ReprogrammingClass::singleFramePendingRespLast(int id, byte* data, int counter)
{

byte sendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],
																	(byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
int Resp = NOK;

  if(ReprogData.stepLock1 == 0)
  {
      CanFnc.sendMessageByte(sendFrame);
	  ReprogData.timerTick   = 0;
	  ReprogData.ProcessStep = 0;	  
      ReprogData.stepLock1   = 1;
//	  Serial.printf("[singleFramePendingResp]-------ReprogData.ProcessStep[%d] ==0\n",ReprogData.ProcessStep);
  }

  if(ReprogData.ProcessStep == 0)
  {
    if((ReprogData.reprogbuf[1] == 0x7F)&&(ReprogData.reprogbuf[3] == 0x78))
	{
//		ReprogRxBufClear();
//      ReprogData.timerTick = 0;

		if(ReprogData.PendingTick == 1)
		{
			ReprogData.stepLock1   = 0;
	        singleFrameNoResp(FunctionalID,TesterPresent,1000);
			ReprogData.stepLock1   = 1;
			ReprogData.PendingTick = 0;
		}
        Resp = NOK;
    }

    if((ReprogData.reprogbuf[1] == (data[1] + 0x40))) 
	{
//      ReprogData.ProcessStep = 0;
        ReprogData.timerTick   = 0;
//      ReprogData.stepLock2   = 0;
        ReprogData.stepLock1   = 0;
        Resp = OK;
    }

    if(ReprogData.timerTick >= counter)  //Timing is expired
	{ 
	    ReprogData.timerTick   = 0;
		ReprogData.PendingTick = 1;
//      Resp = TIMEOUT;
		Resp = NOK;
    }
  }
  return Resp;
}
	
int ReprogrammingClass::singleFrameNoResp(int id, byte* data, int counter) {
  
    byte SendFrame[10] = {(byte) ((id & 0xFF00) >> 8), (byte) (id & 0xFF), (byte) data[0],(byte) data[1],(byte) data[2],(byte) data[3],
                                                                           (byte) data[4],(byte) data[5],(byte) data[6],(byte) data[7]};
    int RespP = NOK;

    if (ReprogData.stepLock1 == 0) {
//      ReprogRxBufClear();
        CanFnc.sendMessageByte(SendFrame);
        ReprogData.stepLock1 = 1;
        ReprogData.timerTick = 0;
    }

    if (ReprogData.timerTick >= counter) {
        RespP = OK;
        ReprogData.timerTick = 0;
        ReprogData.stepLock1 = 0;
    }
    return RespP;
}

int ReprogrammingClass::singleFrameResp(int id, byte* data, int counter)
{
    byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], data[2],data[3],data[4],data[5],data[6],data[7]};
    int RespT =0;
    
//  System.out.println("Single Frame ");
    if(ReprogData.stepLock1 ==0){
        ReprogRxBufClear();
        CanFnc.sendMessageByte(SendFrame);
        ReprogData.stepLock1 =1;
//      ReprogData.mTimerStart = true;
    }

    if(ReprogData.timerTick >= counter){
        ReprogData.timerTick =0;
        RespT =2;
    }

    if(ReprogData.reprogbuf[1] == data[1] + 0x40) {
        ReprogData.timerTick =0;
        ReprogData.stepLock1 =0;
        RespT = 1;
//      ReprogData.mTimerStart = false;
    }

//  System.out.println("Single Frame End");
    return RespT;
}


int ReprogrammingClass::FlowControlFrameResp(int id, byte* data, int counter)
{
    byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],(byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
                                                                        
    int RespS = 0;
    
    if(ReprogData.stepLock1 == 0){
        ReprogRxBufClear();
        CanFnc.sendMessageByte(SendFrame);
        ReprogData.stepLock1 = 1;
    }

    if(ReprogData.timerTick >= counter){
        ReprogData.timerTick =0;
        RespS = 2;
    }

    if(ReprogData.reprogbuf[0] == 0x21 || ReprogData.reprogbuf[0] == 0x22) {
        ReprogData.timerTick =0;
        ReprogData.stepLock1 =0;
        RespS = 1;
//      Serial.println("FlowControlFrameResp-------FlowFrame");
    }
	

//  System.out.println("Single Frame End");
    return RespS;
}

int ReprogrammingClass::singleFirstFrameResp(int id, byte* data, int counter)
{
    byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3], (byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
                                                                        
    int RespS = 0;
    
//  Serial.printf("singleFirstFrameResp------ReprogData.timerTick %d --------------counter %d : \n",ReprogData.timerTick ,counter);

    if(ReprogData.stepLock1 == 0)
	{
        ReprogRxBufClear();
        CanFnc.sendMessageByte(SendFrame);
        ReprogData.stepLock1 = 1;
    }

    if(ReprogData.reprogbuf[2] == (data[1] + 0x40)) 
	{
        ReprogData.timerTick =0;
        ReprogData.stepLock1 =0;
        RespS = OK;
//      ReprogData.mTimerStart = false;
//      CanFnc.sendMessageByte(FlowFrame);
        Serial.printf("singleFirstFrameResp-------FlowFrame [ReprogData.reprogbuf[2]] -%x\n",ReprogData.reprogbuf[2]);
    }
    else if(ReprogData.reprogbuf[1] == (data[1] + 0x40)) 
	{
        ReprogData.timerTick =0;
        ReprogData.stepLock1 =0;
        RespS = OKNOFLOW;
        Serial.printf("singleFirstFrameResp-------FlowFrameFd [ReprogData.reprogbuf[2]] -%x\n",ReprogData.reprogbuf[2]);
    }

	if(ReprogData.timerTick >= counter)
	{
		ReprogData.timerTick = 0;
		RespS = TIMEOUT;
	}

    return RespS;
}

int ReprogrammingClass::singleFrameSecurityResp(int id, byte* data, int counter, int sequence)
{
    byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],
																		(byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
    int RespT = NOK;
	static byte SecurityFrame[4];
	 	
    if(ReprogData.stepLock1 == 0)
	{
        ReprogRxBufClear();

		if(sequence ==  1)
		{
	        CanFnc.sendMessageByte(SendFrame);
		}
		else if(sequence ==  2)
		{
			if(	SharedMemory.SecurityTpye == 0x01 )
			{
				SendFrame[5] = SecurityFrame[0];
				SendFrame[6] = SecurityFrame[1];
				CanFnc.sendMessageByte(SendFrame);
			}
			else if( SharedMemory.SecurityTpye == 0x02 )
			{
				SendFrame[5] = SecurityFrame[0];
				SendFrame[6] = SecurityFrame[1];
				SendFrame[7] = SecurityFrame[2];
				SendFrame[8] = SecurityFrame[3];
				CanFnc.sendMessageByte(SendFrame);
			}
		}		
		else
		{
		}
		
        ReprogData.timerTick =0;
        ReprogData.stepLock1 =1;
    }

    if(ReprogData.reprogbuf[1] == data[1] + 0x40)
	{
        ReprogData.timerTick =0;
        ReprogData.stepLock1 =0;
        RespT = OK;

		if(sequence ==  1)
		{
			if(	SharedMemory.SecurityTpye == 0x01 )
			{
				SecurityFrame[0] = 0xFF - ReprogData.reprogbuf[3];
				SecurityFrame[1] = 0xFF - ReprogData.reprogbuf[4];
				Serial.printf("[ 01 ---  ReprogData.reprogbuf[3]-%x--ReprogData.reprogbuf[4] ]-%x \n",ReprogData.reprogbuf[3] ,ReprogData.reprogbuf[4]);
				Serial.printf("[ SecurityFrame[0]-%x--SecurityFrame[1] ]-%x \n",SecurityFrame[0] ,SecurityFrame[1] );
			}		
			else if( SharedMemory.SecurityTpye == 0x02 )
			{
				SecurityFrame[0] = 0xFF ^ ReprogData.reprogbuf[3];
				SecurityFrame[1] = 0xFF ^ ReprogData.reprogbuf[4];
				SecurityFrame[2] = 0xFF ^ ReprogData.reprogbuf[5];
				SecurityFrame[3] = 0xFF ^ ReprogData.reprogbuf[6];

				Serial.printf("[ 02  ---  ReprogData.reprogbuf[3]-%x--ReprogData.reprogbuf[4] ]-%x \n",ReprogData.reprogbuf[3] ,ReprogData.reprogbuf[4]);
				Serial.printf("[ SecurityFrame[0]-%x--SecurityFrame[1] ]-%x \n",SecurityFrame[0] ,SecurityFrame[1] );
			}
		}
		else if(sequence ==  2)
		{
		
	    }
		else{
    	}
   	}

    if(ReprogData.timerTick >= counter)
	{
        ReprogData.timerTick =0;
        RespT = TIMEOUT;
    }

    return RespT;
}

int ReprogrammingClass::MultiFrameSend(int id, byte* data, int dlc)
{
    int Resp = NOK;

    byte FirstFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)0x10, (byte)dlc, data[0],data[1],data[2],data[3],data[4],data[5]};
    byte ConsecutiveFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)0x21, (byte)0x55,(byte)0x55,(byte)0x55,(byte)0x55,(byte)0x55,(byte)0x55, (byte)0x55};

    if(ReprogData.stepLock1 == 0)
	{
        ReprogRxBufClear();
        CanFnc.sendMessageByte(FirstFrame);
        ReprogData.stepLock1  = 1;
        ReprogData.stepIdx    = 6;
        ReprogData.stepLength = dlc -6;
        ReprogData.stepFrmIdx = 0x21;
        ReprogData.timerTick  = 0;
        ReprogData.ProcessStep= 0;
    }

    if(ReprogData.ProcessStep == 0)
	{
        if(ReprogData.reprogbuf[0] == 0x30)
		{
            ReprogData.ProcessStep = 1;
            ReprogData.timerTick   = 0;
            ReprogData.stepSendLock= 0;
			Serial.println("[ MultiFrameSend - ReprogData.reprogbuf[0] == 0x30] --O.K");
        }
        else if(ReprogData.timerTick > 500)
		{
            ReprogData.timerTick  = 0;
            ReprogData.stepLength = 0;
            ReprogData.stepIdx    = 0;
            Resp =  TIMEOUT;
			Serial.println("[ MultiFrameSend - ReprogData.timerTick > 500 ]--NG");
        }else{

        }
    }
    else if(ReprogData.ProcessStep == 1)
	{
        ConsecutiveFrame[2] = (byte)ReprogData.stepFrmIdx;
        for(int i=0;i<ReprogData.stepLength;i++){
            ConsecutiveFrame[i+3] = data[ReprogData.stepIdx+i];
        }

        for (int i = ReprogData.stepLength; i < 7; i++) {
            ConsecutiveFrame[i+3] = (byte) 0x55;
        }
		ReprogRxBufClear();
        CanFnc.sendMessageByte(ConsecutiveFrame);
        ReprogData.stepLength   = 0;
        ReprogData.stepSendLock = 0;
        ReprogData.ProcessStep  = 2;
        ReprogData.timerTick    = 0;
		Serial.println("[ MultiFrameSend - ReprogData.ProcessStep = 0x2] --O.K");
    }
	else if(ReprogData.ProcessStep == 2)
	{
        if(ReprogData.reprogbuf[1] == 0x74)
		{
            ReprogData.ProcessStep = 1;
            ReprogData.timerTick   = 0;
            ReprogData.stepLock1   = 0;
            Resp = OK;
			Serial.println("[ MultiFrameSend - ReprogData.reprogbuf[1] == 0x74] --O.K");
        }
        else if(ReprogData.timerTick > 500)
		{
            ReprogData.timerTick  = 0;
            ReprogData.stepLength = 0;
            ReprogData.stepIdx    = 0;
            ReprogData.stepLock1  = 0;
            Resp = TIMEOUT;
        }else{

        }
    }
    return Resp;
}


void ReprogrammingClass::vfClearAllEnv(void)
{
	Resp 	= 0;
	FrmCnt 	= 0;
    LastloopCnt = 0;
	LastMsgCnt  = 0;	
	respSta = 1;
	
    ReprogData.stepLock =0;
    ReprogData.timerTick =0;
    ReprogData.stepLock1 =0;
    ReprogData.stepLock2 =0;
    ReprogData.ProgressBar =0;
    ReprogData.reprogExpiredCheck=0;
    ReprogData.reprogEndIdx =0;
    ReprogData.reprogStartIdx =0;
    ReprogData.ProcessStep =0;
    ReprogData.reprogSeq =0;
    ReprogData.mFlagDownload = false;
    ReprogData.mTimerStart   = false;
} 
//----
void ReprogrammingClass::vfClearEnv(void){
//  mHandler2.removeMessages(1);
//  ReprogData.stepLock =0;
    ReprogData.timerTick =0;
    ReprogData.stepLock1 =0;
    ReprogData.stepLock2 =0;
//  ReprogData.ProgressBar =0;
    ReprogData.reprogExpiredCheck=0;
    ReprogData.reprogEndIdx =0;
    ReprogData.reprogStartIdx =0;
    ReprogData.ProcessStep =0;
    ReprogData.reprogSeq =0;
	ReprogData.reprogFirstFlameSize = 0;
	
//  ReprogData.mFlagDownload = false;
//  ReprogData.mTimerStart   = false;
}


// Get the data of a file
long ReprogrammingClass::getFileData(File Srcfile, byte* fdata, int Startidx, int mode)
{
	byte* dBuf = NULL;
	int DataSize = Startidx - 2;

	long lCurPos, lEndPos;
//	lCurPos = ftell(Srcfile);
	if(mode == 1) 
	{
		Srcfile.seek(0, SeekCur);
		lEndPos = Srcfile.position();
		return lEndPos;
	}

	if(Srcfile.available())
	{
	//	fread(ReadData, DataSize, 1, Srcfile);
	//	read(ReadData, DataSize);
		Srcfile.read(ReadData, DataSize);
#if 0
		Serial.printf("[getFileData]");
		for (int i = 0; i < FRAMELENGTH; i++)Serial.printf("%02X ", ReadData[i]); Serial.printf("\n");
#endif
		memcpy(fdata, ReadData, BUFSFD);

	//	fseek(Srcfile, Startidx, SEEK_CUR);
	//	lEndPos = ftell(Srcfile);
	//	Srcfile.seek(Startidx, SeekCur);
		lEndPos = Srcfile.position();
	}
	
	return lEndPos;
}

int ReprogrammingClass::ReprogMultiFrameHsSend(int id, byte* data, int dlc,int Seq)
{
    int Resp = 0,dlch = 0,dlcl = 0; 

	dlch = dlc / 256;
	dlcl = dlc % 256;
	dlch = 0x10 + dlch;

//  byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), //ConsecutiveFrame
	FirstFrame[0] = (byte)((id & 0xFF00)>>8);
	FirstFrame[1] = (byte)(id & 0xFF);
	ConsecutiveFrame[0] = (byte)((id & 0xFF00)>>8);
	ConsecutiveFrame[1] = (byte)(id & 0xFF);

#if 1
	if (ReprogData.stepLock1 == 0)
	{
#ifdef DEBUG_PRINTF__	
		Serial.printf("[FirstFrame - LastloopCnt] %02X [ReprogData.timerTick] %d\n", LastloopCnt,ReprogData.timerTick );
#endif

		FrmCnt++;
		if (LastloopCnt)
		{
			FirstFrame[0+HSCANDATA] = dlch;
			FirstFrame[1+HSCANDATA] = dlcl;
			FirstFrame[2+HSCANDATA] = 0x36;
			FirstFrame[3+HSCANDATA] = FrmCnt;
		}
		else
		{
			FirstFrame[0+HSCANDATA] = 0x1F;
			FirstFrame[1+HSCANDATA] = 0xFF;
			FirstFrame[2+HSCANDATA] = 0x36;
			FirstFrame[3+HSCANDATA] = FrmCnt;
		}
		memcpy(&FirstFrame[4+HSCANDATA], data ,HSFirstFrameLength);

//		ReprogRxBufClear();
#ifndef CANFD_ENABLE__
		CanFDFun.sendMessageByteFD(FirstFrame,sizeof(FirstFrame)-HSCANDATA);
#else
		CanFnc.sendMessageByte(FirstFrame);
#endif
		ReprogData.stepFrmIdx = 0x20;
		ReprogData.stepLock1  = 0x01;
		ReprogData.stepIdx    = HSFirstFrameLength;
		ReprogData.stepLength = dlc - HSFirstFrameLength;
#ifdef DEBUG_PRINTF__				
		for (int i = 0; i < FRAMELENGTH; i++)	Serial.printf("%02X ", FirstFrame[HSCANDATA+i]); Serial.printf("\n");
#endif		
	}

	switch (ReprogData.ProcessStep)
	{
	case 0 :
		Resp = ProcessStepZero();
#ifdef TESTFILEREAD__	
		ReprogData.ProcessStep = 1;
		ReprogData.timerTick   = 0;
#endif		
		break;
	case 1:
		Resp = ProcessStepOne(ConsecutiveFrame, data, dlc, Seq);
#ifdef TESTFILEREAD__	
//		ReprogData.ProcessStep  = 1;
//		ReprogData.timerTick	  = 0;
//		ReprogData.stepSendLock = 0;
#endif		
		break;
	case 2:
		Resp = ProcessStepTwo();
#ifdef TESTFILEREAD__	
		Resp = 1;
#endif
		if(Resp==1)
		{
			ReprogData.stepSendLock = 0;
			ReprogData.stepLength   = 0;
			ReprogData.stepIdx      = 0;
			ReprogData.timerTick    = 0;
			ReprogData.stepLock1    = 0;
			ReprogData.ProcessStep  = 0;
			ReprogData.LastTick	  = 0;

			if (ReprogData.reprogHexSize < BLOCKBUFSDATA && LastloopCnt == 0x01) 
			{
				LastloopCnt == 0x00;
				ReprogData.reprogHexSize = 0;
				Serial.printf("[ProcessStepTwo-last] ReprogData.reprogHexSize --> %d  -\n",ReprogData.reprogHexSize);
			}			
		}
		break;
	default:
		break;
	}
#endif
	return Resp ;
}

int ReprogrammingClass::ProcessStepZero(void)
{
	int	respzero = 0;
#ifdef DEBUG_PRINTF__				
	Serial.printf("[ProcessStepZero] [ReprogData.timerTick] %d\n", ReprogData.timerTick );
#endif

	if (ReprogData.ProcessStep == 0)
	{
		if (ReprogData.reprogbuf[0] == 0x30)
		{
//			ReprogRxBufClear();
			ReprogData.ProcessStep = 1;
			ReprogData.timerTick   = 0;
		}
		else if (ReprogData.timerTick > TIMECNT)
		{
			ReprogData.timerTick  = 0;
			ReprogData.stepLength = 0;
			ReprogData.stepIdx    = 0;
			ReprogData.stepLock1  = 0;
			respzero = 2;
		}
	}
	return respzero;
}

int ReprogrammingClass::ProcessStepTwo(void)
{
	int	resptwo = 0;
 
	if (ReprogData.ProcessStep == 2)
	{
#ifdef TESTFILEREAD__	
		ReprogData.reprogbuf[1] = 0x76; // Simulation Test .. in order to implement comment this part
#endif	
#if 1
		if (ReprogData.reprogbuf[1] == 0x76)
		{
			ReprogData.ProcessStep = 0;
			ReprogData.LastTick    = 0;
			ReprogData.timerTick   = 0;

			if (ReprogData.reprogHexSize >= BLOCKBUFSDATA) 
			{
				ReprogData.reprogHexSize -= BLOCKBUFSDATA;
			}
			else 
			{
				Serial.printf("[LAST] [ProcessStepTwo] ReprogData.reprogHexSize --> %d	-\n",ReprogData.reprogHexSize);
			}

			resptwo = 1;
		}
		else if (ReprogData.timerTick > TIMECNT)
		{
			ReprogData.timerTick = 0;
			ReprogData.stepLength = 0;
			ReprogData.stepIdx = 0;
			ReprogData.ProcessStep = 0;
			ReprogData.timerTick = 0;
			ReprogData.stepLock1 = 0;
			resptwo = 2;
		}

#else
		if (ReprogData.reprogbuf[1] == 0x7F && ReprogData.LastTick == 0)
		{
			ReprogRxBufClear();
			ReprogData.timerTick = 0;
			ReprogData.LastTick = 1;
			ReprogData.ProcessStep = 2;		
			resptwo = 0;
		}
		else if (ReprogData.reprogbuf[1] == 0x76 && ReprogData.LastTick == 1)
		{
			ReprogData.ProcessStep = 0;
			ReprogData.LastTick    = 0;
			ReprogData.timerTick   = 0;

			if (ReprogData.reprogHexSize >= BLOCKBUFSDATA) 
			{
				ReprogData.reprogHexSize -= BLOCKBUFSDATA;
#if 0
				Serial.printf("[ProcessStepTwo] ReprogData.reprogHexSize --> %d  -\n",ReprogData.reprogHexSize);
#endif
			}
			else 
			{
				Serial.printf("[LAST] [ProcessStepTwo] ReprogData.reprogHexSize --> %d  -\n",ReprogData.reprogHexSize);
			}

			resptwo = 1;
		}
		else if (ReprogData.timerTick > TIMECNT)
		{
			ReprogData.timerTick = 0;
			ReprogData.stepLength = 0;
			ReprogData.stepIdx = 0;
			ReprogData.ProcessStep = 0;
			ReprogData.timerTick = 0;
			ReprogData.stepLock1 = 0;
			resptwo = 2;
		}
#endif		

	}
	return resptwo ;
}

int ReprogrammingClass::ProcessStepOne(byte* Frame, byte* data, int dlc, int Seq)
{
	int respone = 0;

	if (ReprogData.ProcessStep == 1)
	{
#ifdef DEBUG_PRINTF__				
		Serial.printf("[ProcessStepOne] - [ReprogData.stepLength] %d [ReprogData.timerTick] %d\n",ReprogData.stepLength, ReprogData.timerTick );
#endif
		if (ReprogData.stepSendLock == 0)
		{
			ReprogData.stepFrmIdx++;
			if (ReprogData.stepFrmIdx > 0x2F) ReprogData.stepFrmIdx = 0x20;

			if (ReprogData.stepLength >= HSFRAMELENGTH)
			{
				ConsecutiveFrame[HSCANDATA] = ReprogData.stepFrmIdx;
				for (int i = 0; i < HSConsecutiveFrameLength; i++){
					ConsecutiveFrame[HSCANDATA + i + 1] = data[ReprogData.stepIdx + i];
				}
				ReprogData.stepIdx    += HSConsecutiveFrameLength;
				ReprogData.stepLength -= HSConsecutiveFrameLength;

#ifndef CANFD_ENABLE__
				CanFDFun.sendMessageByteFD(ConsecutiveFrame,sizeof(ConsecutiveFrame)-HSCANDATA);
#else
				CanFnc.sendMessageByte(ConsecutiveFrame);
#endif

#ifdef DEBUG_PRINTF__				
				for (int i = 0; i < HSFRAMELENGTH; i++)	Serial.printf("%02X ", ConsecutiveFrame[HSCANDATA+i]); Serial.printf("\n");
#endif				
			}
			else
			{
				ConsecutiveFrame[HSCANDATA] = ReprogData.stepFrmIdx;
				
				if (LastloopCnt)
				{
					for (int i = 0; i < ReprogData.stepLength - 2; i++){
						ConsecutiveFrame[HSCANDATA + i + 1] = data[ReprogData.stepIdx + i];
					}
					
					if(ReprogData.stepLength <= HSHALFRAMEDLC)
					{
						for (int i = ReprogData.stepLength; i < HSHALFRAMEDLC; i++) ConsecutiveFrame[HSCANDATA+i] = 0xCC;
#ifndef CANFD_ENABLE__
						CanFDFun.sendMessageByteFD(ConsecutiveFrame,HSHALFRAMEDLC);
#else
						CanFnc.sendMessageByte(ConsecutiveFrame);
#endif
					}
					else
					{
						for (int i = ReprogData.stepLength; i < HSFRAMELENGTH; i++) ConsecutiveFrame[HSCANDATA+i] = 0xCC;
#ifndef CANFD_ENABLE__
						CanFDFun.sendMessageByteFD(ConsecutiveFrame,HSFRAMELENGTH);
#else
						CanFnc.sendMessageByte(ConsecutiveFrame);
#endif
					}
//					vfClearAllEnv();
					ReprogData.stepLength   = 0;
					ReprogData.stepSendLock = 1;
					ReprogData.ProcessStep  = 2;
#ifdef CANFDSENDTEST__
					RepFileTest.close();
#else
					RepHSFile.close();
#endif
					Serial.printf("[ProcessStepOne] ESP_Transfer_App_Reprogramming-\n");
					
#ifdef DEBUG_PRINTF__				
					for (int i = 0; i < HSHALFRAMEDLC; i++) Serial.printf("%02X ", ConsecutiveFrame[HSCANDATA+i]); Serial.printf("\n");
#endif				
				
				}
				else
				{
				    if(LastMsgCnt)
			    	{
			    	//  Serial.printf("[1 ReprogData.stepFrmIdx]%02X %02X \n",ReprogData.stepFrmIdx,ConsecutiveFrame[HSCANDATA]);
						ConsecutiveFrame[HSCANDATA] = ReprogData.stepFrmIdx - 1;
			    	//  Serial.printf("[1 ReprogData.stepFrmIdx]%02X %02X \n",ReprogData.stepFrmIdx,ConsecutiveFrame[HSCANDATA]);

						for (int i = 0; i < ReprogData.stepLength - 2; i++){
							ConsecutiveFrame[HSCANDATA+i + 1] = data[ReprogData.stepIdx + i];
						}
						for (int i = 2; i < HSNORMALDLC; i++) ConsecutiveFrame[HSCANDATA+i] = 0xCC;
#ifndef CANFD_ENABLE__
	//					CanFDFun.sendMessageByteFD(ConsecutiveFrame,sizeof(ConsecutiveFrame)-HSCANDATA);
						CanFDFun.sendMessageByteFD(ConsecutiveFrame,HSNORMALDLC);
#else
						CanFnc.sendMessageByte(ConsecutiveFrame);
#endif
						ReprogData.stepLength   = 0;
						ReprogData.stepSendLock = 1;
						ReprogData.ProcessStep  = 2;
#ifdef DEBUG_PRINTF__				
						for (int i = 0; i < HSNORMALDLC; i++) Serial.printf("%02X ", ConsecutiveFrame[HSCANDATA+i]); Serial.printf("\n");
#endif
						LastMsgCnt = 0x00;
			    	}
					else
					{
						LastMsgCnt = 0x01;
						ReprogData.ProcessStep  = 1;
					}
				}
			}
		}
		
#if 0
		if (ReprogData.stepSendLock == 1)
		{
			if (ReprogData.timerTick >= TIMECNTEXT1)
			{
				ReprogData.timerTick    = 0;
			}
		}
#endif		
	}
	return respone;
}


void ReprogrammingClass::ReprogramInitVariable(int mode)
{

	ReprogData.reprogHexSize     = SharedMemory.FileSize;
	ReprogData.reprogFileHexSize = SharedMemory.FileSize;

    RFileFnc.InitRepFileSetup(3);

	TftLCDFun.StartStop   = true;
	TftLCDFun.SuccessFail = false;
	TftLCDFun.progresspercent = 0;
	
	ReprogmmingData.respSta = 1;
	ReprogmmingData.respTimeCount = 0;

	ReprogData.stepLock  = 0;
	ReprogData.stepLock1 = 0;	
	ReprogData.stepFirst = 0;
//	ReprogData.reprogHexSize = ReprogData.reprogFileHexSize;
	ReprogData.timerTick = 0;

	if(mode) 	ReprogData.reprogStartStop = 1;  // Start Reprogramming
	else	    ReprogData.reprogStartStop = 0;

	ReprogData.reprogExpiredCheck = 0;
	vfClearAllEnv(); 
}

void ReprogrammingClass::DTC_Clear_Functional(void)
{
	if(RFileFnc.DtcAllClearState){
	//	Serial.printf("[THE END] ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last--[ReprogData.reprogStartStop ]-%d\n",ReprogData.reprogStartStop);
	//	singleFrameNoResp(FunctionalID,DownloadStep[19],NORMALTIMECNT);	   // 04 14 Clear DTC...
		CanFnc.SendCanDataToECU_Functional(DownloadStep[19]);
		Serial.println("[THE END-HS CAN] CLEAR_DTC_ALL--[0x04, 0x14, 0xFF, 0xFF, 0xFF, 0x55, 0x55, 0x55 ]-----The End");
	}else{
	//	singleFrameNoResp(FunctionalID,DownloadStep[19],NORMALTIMECNT);    // 04 14 Clear DTC...
		CanFnc.SendCanDataToECU_Pysical(DownloadStep[19]);
	//	CanFnc.SendCanDataToECU_Functional(DownloadStep[19]);  //2020.05.06 Modify by yongwook wi
		Serial.println("[THE END-HS CAN] CLEAR_DTC_ALL--[0x04, 0x14, 0xFF, 0xFF, 0xFF, 0x55, 0x55, 0x55 ]-----The End");
	}
}
