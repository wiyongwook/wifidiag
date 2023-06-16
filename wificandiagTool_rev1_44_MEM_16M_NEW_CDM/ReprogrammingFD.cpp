#include "ReprogrammingFD.h"
#include "Reprogramming.h"
#include "CanFDFunc.h" 
#include "TftLCDFun.h"

#define BUFS     	  4093
#define BLOCKBUFS     4093
#define BLOCKBUFSDATA 4093

#define NOK			  0x00
#define OK			  0x01
#define TIMEOUT		  0x02
#define OKNOFLOW	  0x03

#define OVERTIME	  0x01

byte TesterPresentFD[8] = {(byte) 0x02, (byte) 0x3E, (byte) 0x80, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00};
byte DownloadStepFD[20][8] = {
        {(byte) 0x03, (byte) 0x22, (byte) 0xF1, (byte) 0x89, (byte) 0x55, (byte) 0x55, (byte) 0x55, (byte) 0x55},  //0  ReadDataByIdentifier_ECUSoftwareVersion
        {(byte) 0x02, (byte) 0x10, (byte) 0x03, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //1  DiagnosticSessionControl_Extended_Session
        {(byte) 0x02, (byte) 0x85, (byte) 0x82, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //2  ControlDTCSetting_OFF
        {(byte) 0x03, (byte) 0x28, (byte) 0x83, (byte) 0x03, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //3  CommunicationControl_disableRxAndTx
        {(byte) 0x02, (byte) 0x10, (byte) 0x02, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC},  //4  DiagnosticSessionControl_programmingSession
        {(byte) 0x02, (byte) 0x27, (byte) 0x01, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC},  //5  SecurityAccess_Reques_Seed
        {(byte) 0x06, (byte) 0x27, (byte) 0x02, (byte) 0xD2, (byte) 0x93, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC},  //6  SecurityAccess_Send_KEY
        {(byte) 0x05, (byte) 0x31, (byte) 0x01, (byte) 0xFF, (byte) 0x00, (byte) 0x00, (byte) 0xCC, (byte) 0xCC},  //7  RoutineControl_eraseMemory_APP

        {(byte) 0x10, (byte) 0x0B, (byte) 0x34, (byte) 0x00, (byte) 0x44, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //8
		{(byte) 0x21, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x55, (byte) 0x55},  //9

        {(byte) 0x01, (byte) 0x37, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC},  //10 Transfer_App_File
        {(byte) 0x04, (byte) 0x31, (byte) 0x01, (byte) 0xFF, (byte) 0x01, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC},  //11 RoutineControl_checkprogrammingdependency
        {(byte) 0x02, (byte) 0x11, (byte) 0x01, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC},  //12 ECUReset_hardReset
        {(byte) 0x02, (byte) 0x10, (byte) 0x03, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //13 DiagnosticSessionControl_Extended_Session
        {(byte) 0x03, (byte) 0x28, (byte) 0x80, (byte) 0x03, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //14 CommunicationControl_Enable_RxAndTx
        {(byte) 0x02, (byte) 0x85, (byte) 0x81, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //15 ControlDTCSetting_On
        {(byte) 0x02, (byte) 0x10, (byte) 0x81, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //16 DiagnosticSessionControl
        {(byte) 0x03, (byte) 0x22, (byte) 0xF1, (byte) 0x89, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC, (byte) 0xCC},  //17 ReadDataByIdentifier_ECUSoftwareVersion
        {(byte) 0x30, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00},  //18 ReadDataByIdentifier_Flow_Control

		{(byte) 0x04, (byte) 0x14, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0x00, (byte) 0x00, (byte) 0x00}}; //19 Clear DTC ..Functional 

#if 0
byte TransferAppFrame[11] = { (byte)0x34,(byte)0x00,(byte)0x44,SharedMemory.Address[0],SharedMemory.Address[1],SharedMemory.Address[2],SharedMemory.Address[3], 
	                           SharedMemory.HexSize[0],SharedMemory.HexSize[1],SharedMemory.HexSize[2],SharedMemory.HexSize[3]};
#else
byte TransferAppFrame[11] = { (byte)0x34,(byte)0x00,(byte)0x44, (byte)0xA0, (byte)0x01, (byte)0x80, (byte)0x00, (byte)0x00, (byte)0x1C, (byte)0x3C, (byte)0x44} ;
#endif

void ReprogrammingClassFD::FlashDownload(void) 
{
	int tmpbuf =0;
//	Serial.printf("[ReprogrammingClassFD]-------[FlashDownload - ReprogDataFd.stepLock] %d [ReprogDataFd.stepFirst]%d\n",ReprogDataFd.stepLock,ReprogDataFd.stepFirst);

#ifdef CANFDSENDTEST__
    if(!CanFileTest)
	{
		ReprogDataFd.stepLock = ESP_Transfer_App_Reprogramming;
		ReprogDataFd.stepFirst = 0;
		ReprogDataFd.reprogAPPtoFBL = NOK;
		Serial.printf("[FlashDownload - ReprogDataFd.stepLock] %d [ReprogDataFd.stepFirst]%d\n",ReprogDataFd.stepLock,ReprogDataFd.stepFirst);
		CanFileTest = 1;
   	}
#endif

	//22 F1 89 , Response is need.
	 if(ReprogDataFd.stepLock==0)
	 {
		switch(ReprogDataFd.stepFirst)
		{
			case ESP_ReadDataByIdentifier_ECUSoftwareVersion:							 // F1 89
				ReprogDataFd.timerTick = 0;     //2019.12.17 modify by yongwookwi
				tmpbuf = singleFirstFrameResp(PhysicalID2, DownloadStepFD[0],NORMALTIMECNT*4);
//				tmpbuf = singleFirstFrameResp(PhysicalID2, DownloadStepFD[0],65535);
				if(tmpbuf == OK)
				{
					ReprogDataFd.stepFirst = ESP_FlowControlFrame;
					Serial.println("[ReprogrammingClassFD]-------[ESP_ReadDataByIdentifier_ECUSoftwareVersion --- O.K]");
				}
				else if(tmpbuf == TIMEOUT)
				{
					ReprogDataFd.reprogExpiredCheck = OVERTIME;
				}
				else if(tmpbuf == OKNOFLOW)
				{
					ReprogDataFd.reprogAPPtoFBL = OK;
					ReprogDataFd.stepLock	= ESP_DiagnosticSessionControl_Extended_Session;
					ReprogDataFd.stepLock1	= 0;
					ReprogDataFd.stepFirst	= ESP_FlowControlFrame+1;
					Serial.println("[ReprogrammingClassFD]-------[NO FLOW CONTROL --- O.K]");
				}
				break;
			case ESP_FlowControlFrame:													// 0x30, 0x00,	0x14, 
//				tmpbuf = FlowControlFrameResp(PhysicalID2, DownloadStepFD[18],NORMALTIMECNT);
				tmpbuf = FlowControlFrameResp(PhysicalID2, DownloadStepFD[18],2000);
				if(tmpbuf==OK)
				{
					if((ReprogDataFd.reprogbuf[0] == 0x21) /*&& (ReprogData.reprogbuf[5] == 0x30) && (ReprogData.reprogbuf[6]==0x31)*/ ){
//						ReprogDataFd.reprogAPPtoFBL = OK;
					}
					else if((ReprogDataFd.reprogbuf[0] == 0x22) /*&& (ReprogData.reprogbuf[5] == 0x00)(*/ )
					{
						ReprogDataFd.reprogAPPtoFBL = OK;
						ReprogDataFd.stepLock   = ESP_DiagnosticSessionControl_Extended_Session;
						ReprogDataFd.stepLock1  = 0;
						ReprogDataFd.stepFirst  = ESP_FlowControlFrame+1;
						Serial.println("[ReprogrammingClassFD]-------[ESP_FlowControlFrame --- O.K]");
					}
					else
					{
						ReprogDataFd.reprogAPPtoFBL = NOK;
						Serial.println("[ReprogrammingClassFD]-------[ESP_FlowControlFrame --- Not O.K]");
					}
					//ReprogData.timerTick =0;
				}
				else if(tmpbuf == TIMEOUT)
				{
					ReprogDataFd.reprogExpiredCheck = OVERTIME;
					Serial.println("[ReprogrammingClassFD]-------[ESP_FlowControlFrame --- OVERTIME]");
					return;
				}
				else{
			
				}
				
			default:
				break;
		}
 	}
	 
    if(ReprogDataFd.reprogAPPtoFBL == OK)
    {
        switch(ReprogDataFd.stepLock)
        {
            case ESP_DiagnosticSessionControl_Extended_Session:                     // 10 90
			    tmpbuf = singleFrameResp(FunctionalID2,DownloadStepFD[1],NORMALTIMECNT);	    // 10 03
                if(tmpbuf == OK)
				{
                    ReprogDataFd.stepLock = ESP_ControlDTCSetting_OFF;
					Serial.printf("ESP_DiagnosticSessionControl_Extended_Session ---%d \n",ESP_DiagnosticSessionControl_Extended_Session );
                }
				else if(tmpbuf == TIMEOUT)
				{
					ReprogDataFd.reprogExpiredCheck = OVERTIME;
				}
                break;
            case ESP_ControlDTCSetting_OFF:
                tmpbuf = singleFrameNoResp(FunctionalID2,DownloadStepFD[2],NORMALTIMECNT);         // 85 82
                if(tmpbuf == OK)
				{
                    ReprogDataFd.stepLock = ESP_CommunicationControl_disableRxAndTx;
					Serial.printf("ESP_ControlDTCSetting_OFF ---%d \n",ESP_ControlDTCSetting_OFF );
                }
                break;
             case ESP_CommunicationControl_disableRxAndTx:
                tmpbuf = singleFrameNoResp(FunctionalID2,DownloadStepFD[3],NORMALTIMECNT);        // 28 83 02
                if(tmpbuf == OK)
				{
                    ReprogDataFd.stepLock = ESP_DiagnosticSessionControl_programmingSession;
					Serial.printf("ESP_CommunicationControl_disableRxAndTx ---%d \n",ESP_CommunicationControl_disableRxAndTx );
                }
                break;
             case ESP_DiagnosticSessionControl_programmingSession:
                tmpbuf = singleFramePendingResp(PhysicalID2,DownloadStepFD[4],DDTIMECNT);
                if(tmpbuf == OK)
				{
                    ReprogDataFd.stepLock = ESP_SecurityAccess_Request_Seed;//ESP_SecurityAccess_Request_Seed; //ESP_WaitFunction;
					Serial.printf("ESP_DiagnosticSessionControl_programmingSession ---%d \n",ESP_DiagnosticSessionControl_programmingSession );
               	}
				else if(tmpbuf == TIMEOUT)
				{                                                    //10 85 , Response is not need.
                    ReprogDataFd.reprogExpiredCheck = OVERTIME;
//					Serial.printf("ESP_DiagnosticSessionControl_programmingSession ---%d \n",ESP_DiagnosticSessionControl_programmingSession );
                    return;
                }else{

                }
                break;        
 	        case ESP_SecurityAccess_Request_Seed:
//				Serial.printf("[Init]ESP_SecurityAccess_Reques_Seed ---%d [ReprogDataFd.stepLock1] %d \n",ESP_SecurityAccess_Request_Seed,ReprogDataFd.stepLock1 );
				
	            tmpbuf = singleFrameSecurityResp(PhysicalID2,DownloadStepFD[5],DOUBLETIMECNT,1);		// 27 01
				if(tmpbuf == OK)
				{
	                ReprogDataFd.stepLock = ESP_SecurityAccess_Send_KEY;
					Serial.printf("ESP_SecurityAccess_Request_Seed ---%d \n",ESP_SecurityAccess_Request_Seed );
	            }
				else if(tmpbuf == TIMEOUT)
	            {                                           //27 01 , Response is not need.
	                ReprogDataFd.reprogExpiredCheck = OVERTIME;
	                return;
	            }else{

	            }
	            break;				
		    case ESP_SecurityAccess_Send_KEY:
                tmpbuf = singleFrameSecurityResp(PhysicalID2,DownloadStepFD[6],DOUBLETIMECNT,2);		// 27 02
				if(tmpbuf ==  OK)
				{
                    ReprogDataFd.stepLock = ESP_RoutineControl_eraseMemory_APP;
					Serial.printf("ESP_SecurityAccess_Send_KEY ---%d \n",ESP_SecurityAccess_Send_KEY );
                }
				else if(tmpbuf == TIMEOUT)
				{
                    ReprogDataFd.reprogExpiredCheck = OVERTIME;
                    return;
				}else{

                }
                break;
	        case ESP_RoutineControl_eraseMemory_APP:
				tmpbuf = singleFramePendingRespLast(PhysicalID2,DownloadStepFD[7],DDTIMECNT);//31 01 FF 00 XX, Response is not need.
//				Serial.printf("ESP_RoutineControl_eraseMemory_APP----%d \n",tmpbuf);
				if(tmpbuf ==  OK)
				{
                    ReprogDataFd.stepLock = ESP_Transfer_App_File;
					Serial.printf("ESP_RoutineControl_eraseMemory_APP ---%d \n",ESP_RoutineControl_eraseMemory_APP );
                }
				else if(tmpbuf == TIMEOUT)
				{
                    ReprogDataFd.reprogExpiredCheck = OVERTIME;
                    return;
                }else{

                }
                break;
		     case ESP_Transfer_App_File:
                tmpbuf = DoubleFrameResp(PhysicalID2,TransferAppFrame, sizeof(TransferAppFrame),NORMALTIMECNT);
				Serial.printf("ESP_Transfer_App_File----%d [Address[4]] %x %x %x %x\n",tmpbuf,SharedMemory.Address[0],SharedMemory.Address[1],SharedMemory.Address[2],SharedMemory.Address[3]);
				Serial.printf("ESP_Transfer_App_File----%d [HexSize[4]] %x %x %x %x\n",tmpbuf,SharedMemory.HexSize[0],SharedMemory.HexSize[1],SharedMemory.HexSize[2],SharedMemory.HexSize[3]);

				if(tmpbuf ==  OK)
				{
					ReprogDataFd.stepLock = ESP_Transfer_App_Reprogramming;
					ReprogDataFd.ProcessStep = 0;
					String dnFileName ;
					Serial.printf("ESP_Transfer_App_File----%d \n");
					int lenS = SharedMemory.downloadfileName.length();

					if(TftGuiLCDFun.SelectedID) // 2020.05.08
					{
						lenS = lenS + 1;
					}
					
					dnFileName = SharedMemory.downloadfileName.substring(0, lenS -1); 	
					Serial.println("[R: downloadfileName] : "+ dnFileName); //ReprogDataFd.reprogHexSize
					Serial.printf("[ReprogDataFd.reprogHexSize] %d [respSta]%d \n",ReprogDataFd.reprogHexSize,respSta); 
					RepFile = RFileFnc.OpenFileFD(dnFileName);

					WaitFunction(1000);
				}
				else if(tmpbuf == TIMEOUT)
			    {
                    ReprogDataFd.reprogExpiredCheck = OVERTIME;
                    return;
                }
				else{
                }
                break;
					
            default: 
                break;
             
        }
    }
	
	if(ReprogDataFd.stepLock == ESP_Transfer_App_Reprogramming)
	{
#ifdef CANFDSENDTEST__
    if(!RepFileTest){
		Serial.printf("[ESP_Transfer_App_Reprogramming] %d [SharedMemory.FileSize] %d \n",ReprogDataFd.stepLock,SharedMemory.FileSize);
		Serial.println("[downloadfileName] : "+ SharedMemory.downloadfileName);
		String TestName = "/RG3_C_01.bin";
		int len = SharedMemory.downloadfileName.length();
        SharedMemory.downloadfileName = SharedMemory.downloadfileName.substring(0, len -1);		
#if 1
		RepFileTest = RFileFnc.OpenFileFD(SharedMemory.downloadfileName);
//		RepFileTest = RFileFnc.OpenFileFD("/RG3_C_01.bin");
#else
		RepFileTest = SPIFFS.open(SharedMemory.downloadfileName);
#endif		
		if (!RepFileTest){  // can not open file                                        
		  Serial.println("# [Init]----ERROR: can not open file");
		  delay(100);
		}
   	}
#endif
//		if( SharedMemory.FileSize != 0)
		if( ReprogDataFd.reprogHexSize != 0 )
		{
			if(respSta == 1)
			{
				ReprogDataFd.reprogStartIdx = ReprogDataFd.reprogEndIdx;
				ReprogDataFd.reprogSeq 		= ReprogDataFd.reprogSeq + 1;

				if (ReprogDataFd.reprogHexSize >= BLOCKBUFSDATAFD)
				{
					ReprogDataFd.reprogEndIdx = ReprogDataFd.reprogEndIdx + BLOCKBUFSDATAFD;
					FDDLC = BLOCKBUFSFD + 2  ;

					//TxBuffer = RFileFnc.StringToStringArray(ReprogData.reprogSeq - 1, BLOCKBUFSDATA, 0);
					//					for(int i=0;i <BLOCKBUFSDATA ;i++) Serial.printf("%02X",ReprogmmingData.TxBuffer[i]);
#ifdef CANFDSENDTEST__
					filelength = getFileData(RepFileTest, SendBuf, FDDLC , 0);
#else
					filelength = getFileData(RepFile, SendBuf, FDDLC , 0);
#endif

#ifndef DEBUG_PRINTF__	
					Serial.printf("# [getFileData]--%d\n",filelength);
#endif
				}
				else
				{
					FDDLC = ReprogDataFd.reprogHexSize + 2 ;
#ifdef CANFDSENDTEST__
					filelength = getFileData(RepFileTest, SendBuf, FDDLC , 0);
#else
					filelength = getFileData(RepFile, SendBuf, FDDLC , 0);
#endif

#ifndef DEBUG_PRINTF__	
					Serial.printf("#[LAST] [getFileData]--%d\n",filelength);
#endif
					LastloopCnt = 0x01;
				}				
				respTimeCount=0;
				respSta =10;
			}
			respSta = ReprogMultiFrameFdSend(PhysicalID2, SendBuf, FDDLC, ReprogDataFd.reprogSeq);

			if (ReprogDataFd.timerTick >= TIMEMAXCNT)
			{
				ReprogDataFd.reprogExpiredCheck = 1;
				ReprogDataFd.timerTick = 0;
				
			 // Configure SPI CANFD Init Setup ...
			 // CanFDFun.InitCanFDSetup();  
			}		
		}
		else
		{
			filelength = getFileData(RepFile, SendBuf, FDDLC , 1);

			RepFile.close();
			vfClearAllEnv();
			Serial.printf("ESP_Transfer_App_Reprogramming--[ ReprogDataFd.reprogHexSize] %d \n", ReprogDataFd.reprogHexSize);
			ReprogDataFd.stepLock = ESP_RequestTransferExit;
		}		
	}

	switch(ReprogDataFd.stepLock)
	{
	case ESP_RequestTransferExit:
//		Serial.printf("[ESP_RequestTransferExit]----reprogbuf[1] %x : reprogbuf[3] %x \n",ReprogDataFd.reprogbuf[1],ReprogDataFd.reprogbuf[3]);
	    tmpbuf = singleFramePendingRespLast(PhysicalID2,DownloadStepFD[10],DDTIMECNT);		//37
//	    tmpbuf = singleFramePendingResp(PhysicalID2,DownloadStepFD[10],D2TIMECNT);		//37
		if(tmpbuf == OK)
		{
//			vfClearAllEnv();
			Serial.printf("ESP_RequestTransferExit----%d \n",tmpbuf);
			ReprogDataFd.stepLock = ESP_RoutineControl_checkprogrammingdependency;
		}
		else if(tmpbuf == TIMEOUT)
		{    																	//10 85 , Response is not need.
	        ReprogDataFd.reprogExpiredCheck = OVERTIME;
	        return;
	    }else{
	    }
	    break;
	case ESP_RoutineControl_checkprogrammingdependency:
	    tmpbuf = singleFrameResp(PhysicalID2,DownloadStepFD[11],T3CNTN);	        	// 31 01 FF 01
		if(tmpbuf == OK)
		{
			Serial.printf("ESP_RoutineControl_checkprogrammingdependency----\n");
			ReprogDataFd.stepLock = ESP_ECUReset_HardReset;
		}
		else if(tmpbuf == TIMEOUT)
		{    																	//10 85 , Response is not need.
	        ReprogDataFd.reprogExpiredCheck = OVERTIME;
	        return;
	    }else{
	    }
	    break;
	case ESP_ECUReset_HardReset:
	    tmpbuf = singleFrameNoResp(PhysicalID2,DownloadStepFD[12],T3CNTN);    		//11 81   //singleFrameResp
	    if(tmpbuf == OK)
		{
			Serial.printf("ESP_ECUReset_HardReset---- \n");
	        ReprogDataFd.stepLock =ESP_DiagnosticSessionControl_Extended_Session2;
	    }
	    break;
	case ESP_DiagnosticSessionControl_Extended_Session2:
	    tmpbuf = singleFrameNoResp( FunctionalID2,DownloadStepFD[13],T3CNTN);  		//10 03
	    if(tmpbuf == OK)
		{
			Serial.printf("ESP_DiagnosticSessionControl_Extended_Session2----\n");
	        ReprogDataFd.stepLock = ESP_CommunicationControl_Enable_RxAndTx;
	    }
	    break;	
	case ESP_CommunicationControl_Enable_RxAndTx:
	    tmpbuf = singleFrameNoResp(FunctionalID2,DownloadStepFD[14],T3CNTN); 			//29 02
	    if(tmpbuf == OK)
		{
			Serial.printf("ESP_CommunicationControl_Enable_RxAndTx----\n");
	        ReprogDataFd.stepLock = ESP_ControlDTCSetting_ON;
	    }
	    break;
	case ESP_ControlDTCSetting_ON:
	    tmpbuf =singleFrameNoResp(FunctionalID2,DownloadStepFD[15],T3CNTN);  			//85 81
	    if(tmpbuf == OK)
		{
			Serial.printf("ESP_ControlDTCSetting_ON----\n");
	        ReprogDataFd.stepLock =ESP_DiagnosticSessionControl_Standard;
	    }
	    break;
	case ESP_DiagnosticSessionControl_Standard:
	    tmpbuf = singleFrameNoResp(FunctionalID2,DownloadStepFD[16],T3CNTN);  		//10 81
	    if(tmpbuf == OK)
		{
			Serial.printf("ESP_DiagnosticSessionControl_Standard----\n");
	        ReprogDataFd.stepLock =ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last;
	    }
	    break;
	case ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last:
	    tmpbuf = singleFrameNoResp(PhysicalID2,DownloadStepFD[17],T3CNTN);  			//22 F1

		TftLCDFun.progresspercentSuccess = 1; 

	    if(tmpbuf == OK)
		{
//			RFileFnc.CloseFile();
			vfClearAllEnv();
	        ReprogDataFd.stepLock 		 = 0;
	        ReprogDataFd.reprogStartStop = 0;
	        ReprogData.reprogStartStop 	 = 0;
			Serial.printf("[THE END] ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last--[ReprogDataFd.reprogStartStop ]-%d\n",ReprogDataFd.reprogStartStop);
			tmpbuf = singleFrameNoResp(FunctionalID2,DownloadStepFD[19],NORMALTIMECNT);		   // 04 14 Clear DTC...
			Serial.println("[THE END] CLEAR_DTC_ALL--[0x04, 0x14, 0xFF, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC ]-----The End");
	    }
	    break;			
	default:
		break;
			
	}	
}

void ReprogrammingClassFD::vfClearAllEnv(void){
    ReprogDataFd.stepLock  = 0;
    ReprogDataFd.timerTick = 0;
    ReprogDataFd.stepLock1 = 0;
    ReprogDataFd.stepLock2 = 0;
    ReprogDataFd.ProgressBar = 0;
    ReprogDataFd.reprogExpiredCheck = 0;
    ReprogDataFd.reprogEndIdx   = 0;
    ReprogDataFd.reprogStartIdx = 0;
    ReprogDataFd.ProcessStep    = 0;
    ReprogDataFd.reprogSeq      = 0;

	Resp = 0;
	FrmCnt = 0;
    LastloopCnt = 0;
	respSta = 1;


	ReprogDataFd.stepFrmIdx = 0x0;
 	ReprogDataFd.stepIdx    = 0x0;
	ReprogDataFd.stepLength = 0x0;
	ReprogDataFd.stepSendLock = 0;
	ReprogDataFd.timerTick	  = 0;
	ReprogDataFd.stepLock1	  = 0;
	ReprogDataFd.ProcessStep  = 0;
	ReprogDataFd.LastTick	  = 0;

    ReprogDataFd.mFlagDownload = false;
    ReprogDataFd.mTimerStart   = false;

	filelength = getFileData(RepFile, SendBuf, FDDLC , 1);
	if(RepFile)	RepFile.close();
//	RepFile.close();
	
} 

void ReprogrammingClassFD::ReprogramInitVariable(int mode)
{

	if(SharedMemory.File_CAN_TYPE == 0x01)
	{
	   PhysicalID	 = SharedMemory.P1_ID;
	   FunctionalID  = SharedMemory.F1_ID;
	   ECUID		 = SharedMemory.ECU1_ID;
	}
	else if(SharedMemory.File_CAN_TYPE == 0x02)
	{
	   PhysicalID2	  = SharedMemory.P2_ID;
	   FunctionalID2  = SharedMemory.F2_ID;
	   ECUID2		  = SharedMemory.ECU2_ID;
	}

	ReprogDataFd.reprogHexSize = SharedMemory.FileSize;
	ReprogDataFd.reprogFileHexSize = SharedMemory.FileSize;

    RFileFnc.InitRepFileSetup(3);

	TftLCDFun.StartStop = true;
	TftLCDFun.SuccessFail = false;
	TftLCDFun.progresspercent = 0;

	ReprogmmingData.respSta = 1;
	ReprogmmingData.respTimeCount = 0;
	ReprogDataFd.stepLock  = 0;
	ReprogDataFd.stepLock1 = 0;	
	ReprogDataFd.stepFirst = 0;


	ReprogDataFd.timerTick = 0;
	if(mode) 	ReprogDataFd.reprogStartStop = 1;  // Start Reprogramming
	else	    ReprogDataFd.reprogStartStop = 0;
	ReprogDataFd.reprogExpiredCheck = 0;
	ReprogmmingDataFD.vfClearAllEnv(); 

}

// Get the data of a file
long ReprogrammingClassFD::getFileData(File Srcfile, byte* fdata, int Startidx, int mode)
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


int ReprogrammingClassFD::WaitFunction(int counter){
    int Resp = NOK;

	for(int i=0;i < counter * 100; i++){}
		
    if(ReprogDataFd.timerTick >=counter){
        ReprogDataFd.timerTick =0;
        Resp = OK;
    }
    return Resp;
}


void ReprogrammingClassFD::ReprogRxBufClear(void){
#if 1
  for(int i=0;i<sizeof(ReprogDataFd.reprogbuf);i++){
      ReprogDataFd.reprogbuf[i] = (byte)0x00;
	  
  }
#else
   memset(ReprogData.reprogbuf ,0x00, 8);
   memset(CanFnc.rx_frame.data.u8,0x00, 8);
#endif
}

int ReprogrammingClassFD::singleFramePendingRespLast(int id, byte* data, int counter){

byte sendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],
																	(byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
int Resp = NOK;

  if(ReprogDataFd.stepLock1 == 0)
  {
      CanFDFun.sendMessageByteFD(sendFrame,NormalDLC);
	  ReprogDataFd.timerTick   = 0;
	  ReprogDataFd.ProcessStep = 0;	  
      ReprogDataFd.stepLock1   = 1;
//	  Serial.printf("[singleFramePendingResp]-------ReprogDataFd.ProcessStep[%d] ==0\n",ReprogDataFd.ProcessStep);
  }

  if(ReprogDataFd.ProcessStep == 0)
  {
    if((ReprogDataFd.reprogbuf[1] == 0x7F)&&(ReprogDataFd.reprogbuf[3] == 0x78))
	{
//		ReprogRxBufClear();
//      ReprogDataFd.timerTick = 0;

		if(ReprogDataFd.PendingTick == 1)
		{
			ReprogDataFd.stepLock1 = 0;
	        singleFrameNoResp(FunctionalID2,TesterPresentFD,1000);
			ReprogDataFd.stepLock1   = 1;
			ReprogDataFd.PendingTick = 0;
		}
        Resp = NOK;
    }

    if((ReprogDataFd.reprogbuf[1] == (data[1] + 0x40))) 
	{
//      ReprogDataFd.ProcessStep = 0;
        ReprogDataFd.timerTick   = 0;
//      ReprogDataFd.stepLock2   = 0;
        ReprogDataFd.stepLock1   = 0;
        Resp = OK;
    }

    if(ReprogDataFd.timerTick >= counter)  //Timing is expired
	{ 
	    ReprogDataFd.timerTick   = 0;
		ReprogDataFd.PendingTick = 1;
//      Resp = TIMEOUT;
		Resp = NOK;
    }
  }
  return Resp;
}


int ReprogrammingClassFD::singleFramePendingResp(int id, byte* data, int counter){

byte sendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],
																	(byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
int Resp = NOK;

  if(ReprogDataFd.stepLock1 == 0)
  {
	  ReprogRxBufClear();
      CanFDFun.sendMessageByteFD(sendFrame,NormalDLC);
	  ReprogDataFd.timerTick   = 0;
	  ReprogDataFd.ProcessStep = 0;	  
      ReprogDataFd.stepLock1   = 1;
//	  Serial.printf("[singleFramePendingResp]-------ReprogDataFd.ProcessStep[%d] ==0\n",ReprogDataFd.ProcessStep);
  }

  if(ReprogDataFd.ProcessStep == 0)
  {
    if((ReprogDataFd.reprogbuf[1] == 0x7F)&&(ReprogDataFd.reprogbuf[3] == 0x78))
	{
#if 0	
//		ReprogDataFd.stepLock1 = 0;
//      singleFrameNoResp(FunctionalID2,TesterPresentFD,500);
//		ReprogDataFd.stepLock1   = 1;
//		ReprogDataFd.PendingTick = 0;
#endif
        Resp = NOK;
		ReprogRxBufClear();
//		Serial.println("[ESP_RequestTransferExit]-------ReprogData.reprogbuf[1]== 0x7F)&&(ReprogData.reprogbuf[3]== 0x78");
    }

    if((ReprogDataFd.reprogbuf[1] == (data[1] + 0x40))) 
	{
//      ReprogDataFd.ProcessStep = 0;
        ReprogDataFd.timerTick   = 0;
//      ReprogDataFd.stepLock2   = 0;
        ReprogDataFd.stepLock1   = 0;
        Resp = OK;
    }

    if(ReprogDataFd.timerTick >= counter)  //Timing is expired
	{ 
	    ReprogDataFd.timerTick   = 0;
		ReprogDataFd.PendingTick = 1;
        Resp = TIMEOUT;
//		Resp = NOK;
    }
  }
  return Resp;
}

    
int ReprogrammingClassFD::singleFrameNoResp(int id, byte* data, int counter) {
  
    byte SendFrame[10] = {(byte) ((id & 0xFF00) >> 8), (byte) (id & 0xFF), (byte) data[0],(byte) data[1],(byte) data[2],(byte) data[3],
                                                                           (byte) data[4],(byte) data[5],(byte) data[6],(byte) data[7]};
    int RespP = NOK;

    if (ReprogDataFd.stepLock1 == 0) 
	{
//      ReprogRxBufClear();
        CanFDFun.sendMessageByteFD(SendFrame,NormalDLC);
        ReprogDataFd.stepLock1 = 1;
        ReprogDataFd.timerTick = 0;
//      RespP = OK;
    }

    if (ReprogDataFd.timerTick >= counter) 
	{
        RespP = OK;
        ReprogDataFd.timerTick = 0;
        ReprogDataFd.stepLock1 = 0;
    }

    return RespP;
}

int ReprogrammingClassFD::singleFrameResp(int id, byte* data, int counter)
{
    byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],
																		(byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
    int RespT = NOK;
    
    if(ReprogDataFd.stepLock1 ==0){
        ReprogRxBufClear();
        CanFDFun.sendMessageByteFD(SendFrame,NormalDLC);
        ReprogDataFd.timerTick =0;
        ReprogDataFd.stepLock1 =1;
    }

    if(ReprogDataFd.reprogbuf[1] == data[1] + 0x40) {
        ReprogDataFd.timerTick =0;
        ReprogDataFd.stepLock1 =0;
        RespT = OK;
    }

    if(ReprogDataFd.timerTick >= counter){
        ReprogDataFd.timerTick =0;
        RespT = TIMEOUT;
    }

    return RespT;
}

int ReprogrammingClassFD::singleFrameSecurityResp(int id, byte* data, int counter, int sequence)
{
    byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],
																		(byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
    int RespT = NOK;
	static byte SecurityFrame[2];
	 	
    if(ReprogDataFd.stepLock1 == 0)
	{
        ReprogRxBufClear();

		if(sequence ==  1)
		{
	        CanFDFun.sendMessageByteFD(SendFrame,NormalDLC);
		}
		else if(sequence ==  2)
		{
			if(	SharedMemory.SecurityTpye == 0x01 )
			{
				SendFrame[5] = SecurityFrame[0];
				SendFrame[6] = SecurityFrame[1];
		        CanFDFun.sendMessageByteFD(SendFrame,NormalDLC);
			}
			else if( SharedMemory.SecurityTpye == 0x02 )
			{
				SendFrame[5] = SecurityFrame[0];
				SendFrame[6] = SecurityFrame[1];
				SendFrame[7] = SecurityFrame[2];
				SendFrame[8] = SecurityFrame[3];
				CanFDFun.sendMessageByteFD(SendFrame,NormalDLC);
			}		
		}
		else
		{
		}
		
        ReprogDataFd.timerTick =0;
        ReprogDataFd.stepLock1 =1;
    }

    if(ReprogDataFd.reprogbuf[1] == data[1] + 0x40)
	{
        ReprogDataFd.timerTick =0;
        ReprogDataFd.stepLock1 =0;
        RespT = OK;

		if(sequence ==  1)
		{
			if(	SharedMemory.SecurityTpye == 0x01 )
			{
				SecurityFrame[0] = 0xFF - ReprogDataFd.reprogbuf[3];
				SecurityFrame[1] = 0xFF - ReprogDataFd.reprogbuf[4];
				Serial.printf("[ ReprogDataFd.reprogbuf[3]-%x--ReprogDataFd.reprogbuf[4] ]-%x \n",ReprogDataFd.reprogbuf[3] ,ReprogDataFd.reprogbuf[4]);
				Serial.printf("[ SecurityFrame[0]-%x--SecurityFrame[1] ]-%x \n",SecurityFrame[0] ,SecurityFrame[1] );
			}
			else if( SharedMemory.SecurityTpye == 0x02 )
			{
				SecurityFrame[0] = 0xFF - ReprogDataFd.reprogbuf[3];
				SecurityFrame[1] = 0xFF - ReprogDataFd.reprogbuf[4];
				SecurityFrame[2] = 0xFF - ReprogDataFd.reprogbuf[5];
				SecurityFrame[3] = 0xFF - ReprogDataFd.reprogbuf[6];
				Serial.printf("[ ReprogDataFd.reprogbuf[3]-%x--ReprogDataFd.reprogbuf[4] ]-%x ReprogDataFd.reprogbuf[5]-%x--ReprogDataFd.reprogbuf[6] ]-%x\n",
					ReprogDataFd.reprogbuf[3] ,ReprogDataFd.reprogbuf[4],ReprogDataFd.reprogbuf[4] ,ReprogDataFd.reprogbuf[6]);
				Serial.printf("[ SecurityFrame[0]-%x--SecurityFrame[1] ]-%x SecurityFrame[2]-%x--SecurityFrame[3] ]-%x \n",
					SecurityFrame[0] ,SecurityFrame[1] ,SecurityFrame[2] ,SecurityFrame[3]  );
			}
		}
		else if(sequence ==  2)
		{
		
	    }
		else{
    	}
   	}

    if(ReprogDataFd.timerTick >= counter)
	{
        ReprogDataFd.timerTick =0;
        RespT = TIMEOUT;
    }

    return RespT;
}

int ReprogrammingClassFD::FlowControlFrameResp(int id, byte* data, int counter)
{
    byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],(byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
                                                                        
    int RespS = NOK;
    
    if(ReprogDataFd.stepLock1 == 0)
	{
        ReprogRxBufClear();
        CanFDFun.sendMessageByteFD(SendFrame,NormalDLC);
        ReprogDataFd.timerTick =0;
        ReprogDataFd.stepLock1 = 1;
    }

    if(ReprogDataFd.reprogbuf[0] == 0x21 || ReprogDataFd.reprogbuf[0] == 0x22) 
	{
        ReprogDataFd.timerTick =0;
        ReprogDataFd.stepLock1 =0;
        RespS = OK;
//      Serial.println("FlowControlFrameResp-------FlowFrame OK\n");
    }

    if(ReprogDataFd.timerTick >= counter)
	{
        ReprogDataFd.timerTick =0;
        RespS = TIMEOUT;
        Serial.printf("FlowControlFrameResp-------TIMEOUT  %d %d\n",ReprogDataFd.timerTick,counter);
    }

    return RespS;
}

int ReprogrammingClassFD::singleFirstFrameResp(int id, byte* data, int counter)
{
    byte SendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3], (byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
                                                                        
    int RespS = NOK;
    
//  Serial.printf("singleFirstFrameResp------ReprogData.timerTick %d ---counter %d : \n",ReprogDataFd.timerTick ,counter);
//	ReprogDataFd.stepLock1 = 0;

    if(ReprogDataFd.stepLock1 == 0)
	{
      ReprogRxBufClear();
      CanFDFun.sendMessageByteFD(SendFrame,NormalDLC);
	  ReprogDataFd.timerTick =0;
      ReprogDataFd.stepLock1 = 1;
    }

    if(ReprogDataFd.reprogbuf[2] == (data[1] + 0x40)) 
	{
        ReprogDataFd.timerTick =0;
        ReprogDataFd.stepLock1 =0;
        RespS = OK;
        Serial.printf("singleFirstFrameResp-------FlowFrameFd [ReprogDataFd.reprogbuf[2]] -%x\n",ReprogDataFd.reprogbuf[2]);
    }
    else if(ReprogDataFd.reprogbuf[1] == (data[1] + 0x40)) 
	{
        ReprogDataFd.timerTick =0;
        ReprogDataFd.stepLock1 =0;
        RespS = OKNOFLOW;
        Serial.printf("singleFirstFrameResp-------FlowFrameFd [ReprogDataFd.reprogbuf[2]] -%x\n",ReprogDataFd.reprogbuf[2]);
    }

    if(ReprogDataFd.timerTick >= counter)
	{
        ReprogDataFd.timerTick =0;
        RespS = TIMEOUT;
    }

    return RespS;
}

int ReprogrammingClassFD::DoubleFrameResp(int id, byte* data, int FrameSize ,int counter)
{
    byte SendFrameFdD[TransferAppDLC+2]; // = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3], (byte)data[4], (byte)data[5], (byte)data[6],(byte)data[7]};
    int RespS = 0;

	SendFrameFdD[0] = (byte)((id & 0xFF00)>>8); //, (byte)(id & 0xFF)
	SendFrameFdD[1] = (byte)(id & 0xFF);
	SendFrameFdD[2] = (byte)(0x00); 
	SendFrameFdD[3] = (byte)FrameSize;

	memcpy(&SendFrameFdD[4],data,FrameSize);

#ifdef TESTFILEREAD__	
	ReprogDataFd.stepLock1 = 0;
#endif
    if(ReprogDataFd.stepLock1 == 0)
	{
//    ReprogRxBufClear();

      memcpy(&SendFrameFdD[7] ,SharedMemory.Address,4);
      memcpy(&SendFrameFdD[11],SharedMemory.HexSize,4);

	  SendFrameFdD[15] = 0xCC;
	  SendFrameFdD[16] = 0xCC;
	  SendFrameFdD[17] = 0xCC;
	  	
      CanFDFun.sendMessageByteFD(SendFrameFdD,TransferAppDLC);
	  ReprogDataFd.timerTick = 0;
      ReprogDataFd.stepLock1 = 1;
    }

//  Serial.printf("DoubleFrameResp---ReprogData.timerTick %d --counter %d ---data[0] %x : ReprogDataFd.reprogbuf[1] %x: \n",ReprogDataFd.timerTick ,counter,data[0],ReprogDataFd.reprogbuf[1]);

    if(ReprogDataFd.reprogbuf[1] == (data[0] + 0x40)) 
	{
        ReprogDataFd.timerTick = 0;
        ReprogDataFd.stepLock1 = 0;
        RespS = 1;
        Serial.printf("DoubleFrameResp O.K - [ReprogDataFd.reprogbuf[2]] -%x\n",ReprogDataFd.reprogbuf[2]);
    }

    if(ReprogDataFd.timerTick >= counter)
	{
        ReprogDataFd.timerTick = 0;
		ReprogDataFd.stepLock1 = 0;
        RespS = 2;
    }
	
    return RespS;
}


int ReprogrammingClassFD::ReprogMultiFrameFdSend(int id, byte* data, int dlc,int Seq)
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
	if (ReprogDataFd.stepLock1 == 0)
	{
#ifdef DEBUG_PRINTF__	
		Serial.printf("[FirstFrame - LastloopCnt] %02X [ReprogDataFd.timerTick] %d\n", LastloopCnt,ReprogDataFd.timerTick );
#endif

		FrmCnt++;
		if (LastloopCnt)
		{
			FirstFrame[0+CANDATA] = dlch;
			FirstFrame[1+CANDATA] = dlcl;
			FirstFrame[2+CANDATA] = 0x36;
			FirstFrame[3+CANDATA] = FrmCnt;
		}
		else
		{
			FirstFrame[0+CANDATA] = 0x1F;
			FirstFrame[1+CANDATA] = 0xFF;
			FirstFrame[2+CANDATA] = 0x36;
			FirstFrame[3+CANDATA] = FrmCnt;
		}
		memcpy(&FirstFrame[4+CANDATA], data ,FirstFrameLength);

//		ReprogRxBufClear();
#ifdef CANFD_ENABLE__
		CanFDFun.sendMessageByteFD(FirstFrame,sizeof(FirstFrame)-CANDATA);
#endif
		ReprogDataFd.stepFrmIdx = 0x20;
		ReprogDataFd.stepLock1  = 0x01;
		ReprogDataFd.stepIdx    = FirstFrameLength;
		ReprogDataFd.stepLength = dlc - FirstFrameLength;
#ifdef DEBUG_PRINTF__				
		for (int i = 0; i < FRAMELENGTH; i++)	Serial.printf("%02X ", FirstFrame[CANDATA+i]); Serial.printf("\n");
#endif		
	}

	switch (ReprogDataFd.ProcessStep)
	{
	case 0 :
		Resp = ProcessStepZero();
#ifdef TESTFILEREAD__	
		ReprogDataFd.ProcessStep = 1;
		ReprogDataFd.timerTick   = 0;
#endif		
		break;
	case 1:
		Resp = ProcessStepOne(ConsecutiveFrame, data, dlc, Seq);
#ifdef TESTFILEREAD__	
//		ReprogDataFd.ProcessStep  = 1;
//		ReprogDataFd.timerTick	  = 0;
//		ReprogDataFd.stepSendLock = 0;
#endif		
		break;
	case 2:
		Resp = ProcessStepTwo();
#ifdef TESTFILEREAD__	
		Resp = 1;
#endif
		if(Resp==1)
		{
			ReprogDataFd.stepSendLock = 0;
			ReprogDataFd.stepLength   = 0;
			ReprogDataFd.stepIdx      = 0;
			ReprogDataFd.timerTick    = 0;
			ReprogDataFd.stepLock1    = 0;
			ReprogDataFd.ProcessStep  = 0;
			ReprogDataFd.LastTick	  = 0;

			if (ReprogDataFd.reprogHexSize < BLOCKBUFSDATA && LastloopCnt == 0x01) 
			{
				LastloopCnt == 0x00;
				ReprogDataFd.reprogHexSize = 0;
				Serial.printf("[ProcessStepTwo-last] ReprogDataFd.reprogHexSize --> %d  -\n",ReprogDataFd.reprogHexSize);
			}			
		}
		break;
	default:
		break;
	}
#endif
	return Resp ;
}

int ReprogrammingClassFD::ProcessStepZero(void)
{
	int	respzero = 0;
#ifdef DEBUG_PRINTF__				
	Serial.printf("[ProcessStepZero] [ReprogDataFd.timerTick] %d\n", ReprogDataFd.timerTick );
#endif

	if (ReprogDataFd.ProcessStep == 0)
	{
		if (ReprogDataFd.reprogbuf[0] == 0x30)
		{
//			ReprogRxBufClear();
			ReprogDataFd.ProcessStep = 1;
			ReprogDataFd.timerTick   = 0;
		}
		else if (ReprogDataFd.timerTick > TIMECNT)
		{
			ReprogDataFd.timerTick  = 0;
			ReprogDataFd.stepLength = 0;
			ReprogDataFd.stepIdx    = 0;
			ReprogDataFd.stepLock1  = 0;
			respzero = 2;
		}
	}
	return respzero;
}

int ReprogrammingClassFD::ProcessStepTwo(void)
{
	int	resptwo = 0;
 
	if (ReprogDataFd.ProcessStep == 2)
	{
#ifdef TESTFILEREAD__	
		ReprogDataFd.reprogbuf[1] = 0x76; // Simulation Test .. in order to implement comment this part
#endif	
		if (ReprogDataFd.reprogbuf[1] == 0x7F && ReprogDataFd.LastTick == 0)
		{
			ReprogRxBufClear();
			ReprogDataFd.timerTick = 0;
			ReprogDataFd.LastTick = 1;
			ReprogDataFd.ProcessStep = 2;		
			resptwo = 0;
		}
		else if (ReprogDataFd.reprogbuf[1] == 0x76 && ReprogDataFd.LastTick == 1)
		{
			ReprogDataFd.ProcessStep = 0;
			ReprogDataFd.LastTick    = 0;
			ReprogDataFd.timerTick   = 0;

			if (ReprogDataFd.reprogHexSize >= BLOCKBUFSDATA) 
			{
				ReprogDataFd.reprogHexSize -= BLOCKBUFSDATA;
#if 0
				Serial.printf("[ProcessStepTwo] ReprogDataFd.reprogHexSize --> %d  -\n",ReprogDataFd.reprogHexSize);
#endif
			}
			else 
			{
				Serial.printf("[LAST] [ProcessStepTwo] ReprogDataFd.reprogHexSize --> %d  -\n",ReprogDataFd.reprogHexSize);
			}

			resptwo = 1;
		}
		else if (ReprogDataFd.timerTick > TIMECNT)
		{
			ReprogDataFd.timerTick = 0;
			ReprogDataFd.stepLength = 0;
			ReprogDataFd.stepIdx = 0;
			ReprogDataFd.ProcessStep = 0;
			ReprogDataFd.timerTick = 0;
			ReprogDataFd.stepLock1 = 0;
			resptwo = 2;
		}

	}
	return resptwo ;
}

int ReprogrammingClassFD::ProcessStepOne(byte* Frame, byte* data, int dlc, int Seq)
{
	int respone = 0;

	if (ReprogDataFd.ProcessStep == 1)
	{
#ifdef DEBUG_PRINTF__				
		Serial.printf("[ProcessStepOne] - [ReprogDataFd.stepLength] %d [ReprogDataFd.timerTick] %d\n",ReprogDataFd.stepLength, ReprogDataFd.timerTick );
#endif
		if (ReprogDataFd.stepSendLock == 0)
		{
			ReprogDataFd.stepFrmIdx++;
			if (ReprogDataFd.stepFrmIdx > 0x2F) ReprogDataFd.stepFrmIdx = 0x20;

			if (ReprogDataFd.stepLength >= FRAMELENGTH)
			{
				ConsecutiveFrame[CANDATA] = ReprogDataFd.stepFrmIdx;
				for (int i = 0; i < ConsecutiveFrameLength; i++){
					ConsecutiveFrame[CANDATA + i + 1] = data[ReprogDataFd.stepIdx + i];
				}
				ReprogDataFd.stepIdx    += ConsecutiveFrameLength;
				ReprogDataFd.stepLength -= ConsecutiveFrameLength;

#ifdef CANFD_ENABLE__
				CanFDFun.sendMessageByteFD(ConsecutiveFrame,sizeof(ConsecutiveFrame)-CANDATA);
#endif

#ifdef DEBUG_PRINTF__				
				for (int i = 0; i < FRAMELENGTH; i++)	Serial.printf("%02X ", ConsecutiveFrame[CANDATA+i]); Serial.printf("\n");
#endif				
			}
			else
			{
				ConsecutiveFrame[CANDATA] = ReprogDataFd.stepFrmIdx;
				
				if (LastloopCnt)
				{
					for (int i = 0; i < ReprogDataFd.stepLength - 2; i++){
						ConsecutiveFrame[CANDATA + i + 1] = data[ReprogDataFd.stepIdx + i];
					}
					
					if(ReprogDataFd.stepLength <= HALFRAMEDLC)
					{
						for (int i = ReprogDataFd.stepLength; i < HALFRAMEDLC; i++) ConsecutiveFrame[CANDATA+i] = 0xCC;
#ifdef CANFD_ENABLE__
						CanFDFun.sendMessageByteFD(ConsecutiveFrame,HALFRAMEDLC);
#endif
					}
					else
					{
						for (int i = ReprogDataFd.stepLength; i < FRAMELENGTH; i++) ConsecutiveFrame[CANDATA+i] = 0xCC;
#ifdef CANFD_ENABLE__
						CanFDFun.sendMessageByteFD(ConsecutiveFrame,FRAMELENGTH);
#endif
					}
//					vfClearAllEnv();
					ReprogDataFd.stepLength   = 0;
					ReprogDataFd.stepSendLock = 1;
					ReprogDataFd.ProcessStep  = 2;
#ifdef CANFDSENDTEST__
					RepFileTest.close();
#else
					RepFile.close();
#endif
					Serial.printf("[ProcessStepOne] ESP_Transfer_App_Reprogramming-\n");
					
#ifdef DEBUG_PRINTF__				
					for (int i = 0; i < HALFRAMEDLC; i++) Serial.printf("%02X ", ConsecutiveFrame[CANDATA+i]); Serial.printf("\n");
#endif				
				
				}
				else
				{
				    if(LastMsgCnt)
			    	{
			    	//  Serial.printf("[1 ReprogDataFd.stepFrmIdx]%02X %02X \n",ReprogDataFd.stepFrmIdx,ConsecutiveFrame[CANDATA]);
						ConsecutiveFrame[CANDATA] = ReprogDataFd.stepFrmIdx - 1;
			    	//  Serial.printf("[1 ReprogDataFd.stepFrmIdx]%02X %02X \n",ReprogDataFd.stepFrmIdx,ConsecutiveFrame[CANDATA]);

						for (int i = 0; i < ReprogDataFd.stepLength - 2; i++){
							ConsecutiveFrame[CANDATA+i + 1] = data[ReprogDataFd.stepIdx + i];
						}
						for (int i = 2; i < NORMALDLC; i++) ConsecutiveFrame[CANDATA+i] = 0xCC;
#ifdef CANFD_ENABLE__
	//					CanFDFun.sendMessageByteFD(ConsecutiveFrame,sizeof(ConsecutiveFrame)-CANDATA);
						CanFDFun.sendMessageByteFD(ConsecutiveFrame,NORMALDLC);
#endif
						ReprogDataFd.stepLength   = 0;
						ReprogDataFd.stepSendLock = 1;
						ReprogDataFd.ProcessStep  = 2;
#ifdef DEBUG_PRINTF__				
						for (int i = 0; i < NORMALDLC; i++) Serial.printf("%02X ", ConsecutiveFrame[CANDATA+i]); Serial.printf("\n");
#endif
						LastMsgCnt = 0x00;
			    	}
					else
					{
						LastMsgCnt = 0x01;
						ReprogDataFd.ProcessStep  = 1;
					}
				}
			}
		}
		
#if 0
		if (ReprogDataFd.stepSendLock == 1)
		{
			if (ReprogDataFd.timerTick >= TIMECNTEXT1)
			{
				ReprogDataFd.timerTick    = 0;
			}
		}
#endif		
	}
	return respone;
}

void ReprogrammingClassFD::DTC_Clear_Functional(void)
{
	int Retbuf =0;
//	ReprogDataFd.stepLock1 = 0;
//	Serial.printf("[THE END] ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last--[ReprogDataFd.reprogStartStop ]-%d\n",ReprogDataFd.reprogStartStop);
	if(RFileFnc.DtcAllClearState){
//		Retbuf = singleFrameNoResp(FunctionalID2,DownloadStepFD[19],NORMALTIMECNT); 	   // 04 14 Clear DTC...
		CanFDFun.SendCanFDDataToECU_Functional(DownloadStepFD[19],WDLC);
		Serial.println("[THE END-CAN FD] CLEAR_DTC_ALL--[ID 7DF 0x04, 0x14, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00 ]-----The End");
	}else{	
//		Retbuf = singleFrameNoResp(PhysicalID2,DownloadStepFD[19],NORMALTIMECNT);
		CanFDFun.SendCanFDDataToECU_Pysical(DownloadStepFD[19],WDLC);
//		CanFDFun.SendCanFDDataToECU_Functional(DownloadStepFD[19],WDLC); //2020.05.06 Modify by yongwook wi

		Serial.println("[THE END-CAN FD] CLEAR_DTC_ALL--[ID 7BE 0x04, 0x14, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00 ]-----The End");
	}
}
