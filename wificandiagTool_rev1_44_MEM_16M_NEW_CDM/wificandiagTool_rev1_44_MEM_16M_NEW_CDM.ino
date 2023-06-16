/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include "WifiFunc.h"
#include "CanFunc.h"
#include "CanFDFunc.h"
#include "RepFileFunc.h"
#include "Reprogramming.h"
#include "ReprogrammingFD.h"
#include "TftLCDFun.h"
#include "TftGuiLCDFun.h"
#include "WebScript.h"


//#include <SPI.h>
//#include <TFT_eSPI.h>      // Hardware-specific library

#define CANTIMESETTING  1 // 1//5  //4//4
#define OTATIMESETTING  1 // 1//5  //4//4
#define HSCANTIMESETT   1 // 1//5  //4//4
//#define TFTSPI			
#define OTAWEBCONNECTING  1000 // 1//5  //4//4

WebServer  	   otaserver(80);
AsyncWebServer webserver(80);


//---Define CAN Function Class...
 CanFuncClass  CanFnc;  

//--------------------------------
 CanFDFuncClass CanFDFun;

//---Define WIFIFunction Class...
 WiFiFuncClass WiFiFnc;

//---Define RepFile Function Class...
 RepFileFuncClass RFileFnc;

//---Define ReprogDataFd Function Class...
 ReprogDataTypeClassFd ReprogDataFd;

//---Define Reprogramming Class...
 ReprogrammingClass ReprogmmingData;

//---Define Reprogramming Class...
 ReprogrammingClassFD ReprogmmingDataFD;

//---Define Reprogramming Class...
 TftLCDFunClass TftLCDFun; 

//---Define Reprogramming Class...
 TftGuiLCDFunClass TftGuiLCDFun; 

void setup() {
 // put your setup code here, to run once:
    WifiCanInitSetup();

    // Configure Wifi Init Setup ...
    WiFiFnc.InitWifiSetup();

    // Configure Rep File Init Setup ...
    RFileFnc.InitRepFileSetup(0);
    
    // Configure CAN  Init  Setup ...
    CanFnc.InitCanSetup();
    
    // Configure SPI CANFD Init Setup ...
	CanFDFun.InitCanFDSetup();
	
    // Configure Rep ReprogmmingData Setup ...
    ReprogmmingData.InitReprogrammingClass();
	
    // Configure Init TFT LCD Display ...
#ifdef TFTSPI    
	TftLCDFun.InitTftLCDClass();
#else
	TftGuiLCDFun.InitTftGuiLCDClass();
#endif
// WIFI B/D SW Version Writing
//    memcpy(SharedMemory.WifiBdSWVer,"1.00.00 : 20191112",STR_SIZE);

    //start Each TASK Module
    xTaskCreate(    CanFdHsReprogram,   /* Task function. */
                    "CanFdReprogram", /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

    xTaskCreate(    WifiConnection,   /* Task function. */
                    "WifiConnection", /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

    xTaskCreate(    WifiCANDiag,      /* Task function. */
                    "WifiCANDiag",    /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

    xTaskCreate(    TimerTick_1ms,    /* Task function. */
                    "TimerTick_1ms",  /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

	xTaskCreate(	LcdDiaplyTask,	  /* Task function. */
				    "LcdDiaplyTask",  /* String with name of task. */
					10000,			  /* Stack size in bytes. */
					NULL, 			  /* Parameter passed as input of the task */
					1,				  /* Priority of the task. */
					NULL);			  /* Task handle. */
	
	xTaskCreate(	CanFDTxTask,	  /* Task function. */
				    "CanFDTxTask",    /* String with name of task. */
					10000,			  /* Stack size in bytes. */
					NULL, 			  /* Parameter passed as input of the task */
					1,				  /* Priority of the task. */
					NULL);			  /* Task handle. */

	xTaskCreate(	CanFDRxTask,	  /* Task function. */
				    "CanFDRxTask",    /* String with name of task. */
					10000,			  /* Stack size in bytes. */
					NULL, 			  /* Parameter passed as input of the task */
					1,				  /* Priority of the task. */
					NULL);			  /* Task handle. */
	
	xTaskCreate(	DiagCANWebsocketTask,	  /* Task function. */
				    "DiagCANWebsocketTask",    /* String with name of task. */
					10000,			  /* Stack size in bytes. */
					NULL, 			  /* Parameter passed as input of the task */
					1,				  /* Priority of the task. */
					NULL);			  /* Task handle. */

    delay(500);                       // After Set up Env, Should wait for a momnet
                                
}


void CanFdHsReprogram( void * parameter )
{
  while(1)
  {
    if(WiFiFnc.WifiWorking == 0)
    {
#if 1
		if(ReprogData.reprogStartStop || ReprogDataFd.reprogStartStop)
		{
			switch(SharedMemory.File_CAN_TYPE)
			{
			case 1:  // HS CAN
//				Serial.printf("[ReprogData.reprogStartStop]%d --------------------\n",ReprogData.reprogStartStop);
				CanFnc.Recu_Fragment = 0; 		
				ReprogmmingData.FlashDownload();
				break;
			case 2:  // CAN FD
				CanFDFun.Recu_Fragment = 0; 		
				ReprogmmingDataFD.FlashDownload();
				break;
			}
		}
#else
	   ReprogmmingData.TxBuffer = RFileFnc.StringToStringArray(1,253);
	   for(int i=0;i <253 ;i++)	Serial.printf("%02X",ReprogmmingData.TxBuffer[i]);
 	   CanFnc.CAN_ComCheck = 1;
#endif
    }
	else 
	{
		ReprogData.timerTick = 0;
		ReprogDataFd.timerTick = 0;
		
		CanFnc.CAN_DIAG_MODE_Sel   = CANDIAG_NONE;
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
		CanFnc.Recu_Fragment       = CANDIAG_NONE;
		CanFDFun.Recu_Fragment     = CANDIAG_NONE;
	}

#if 0
	vTaskDelay(CANTIMESETTING / portTICK_PERIOD_MS);
#else  //SharedMemory.STmin
	switch(SharedMemory.File_CAN_TYPE)
	{
		case 1:  // HS CAN
			vTaskDelay(SharedMemory.STmin / portTICK_PERIOD_MS);
			break;
		case 2:  // CAN FD
			vTaskDelay(CANTIMESETTING / portTICK_PERIOD_MS);
			break;
	}	
#endif
  }
  vTaskDelete( NULL );
  
} 


void WifiConnection( void * parameter )
{
while(1)
{
    if(WiFiFnc.WifiWorking)
  	{
#if 0
		if(TftGuiLCDFun.WifiSetting==0){
			WiFiFnc.WifiApModefuncUDP();
		}
		else{
			WiFiFnc.WififuncUDP();
		}
#else
   if(SharedMemory.AsyncWebMStart == 0)
   {
	// On HTTP request for root, provide index.html file  
		webserver.on("/", HTTP_GET, WiFiFnc.onIndexRequest);   
	// On HTTP request for style sheet, provide style.css  
	    webserver.on("/wifidiag.css", HTTP_GET, WiFiFnc.onCSSRequest);  
	// Handle requests for pages that do not exist  
    	webserver.onNotFound(WiFiFnc.onPageNotFound);  
	// Start web server  
    	webserver.begin();  
	// Start WebSocket server and assign callback  
    	WiFiFnc.webSocket.begin();  
    	WiFiFnc.webSocket.onEvent(WiFiFnc.onWebSocketEvent);

		SharedMemory.AsyncWebMStart = 1;
   	}
#endif		
	}
	else
	{
//		WiFiFnc.WifiDisConnection();
		SharedMemory.AsyncWebMStart = 0;
	}         


#if 1
    if(SharedMemory.OTAMStart)
  	{
		if(SharedMemory.OTAWebConnection == 0)
		{
			/*use mdns for host name resolution*/
			if (!MDNS.begin(WiFiFnc.networkName))
			{ //http://esp32.local
				Serial.println("Error setting up MDNS responder!");
			}
			else
			{
				SharedMemory.OTAWebConnection = 1;
				Serial.println("mDNS responder started");

				/*return index page which is stored in serverIndex */
				otaserver.on("/", HTTP_GET, []() 
				{
					otaserver.sendHeader("Connection", "close");
					otaserver.send(200, "text/html", loginIndex);
				});

				otaserver.on("/serverIndex", HTTP_GET, []() 
				{
					otaserver.sendHeader("Connection", "close");
					otaserver.send(200, "text/html", serverIndex);
				});
/*----------------------------INI FILE Update ---------------------------------*/				
#if 1
				/*handling uploading firmware file */
				otaserver.on("/updateini", HTTP_POST, []() 
				{
					otaserver.sendHeader("Connection", "close");
					otaserver.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
//					ESP.restart();
				}, []() {
					HTTPUpload& uploadini = otaserver.upload();
					if (uploadini.status == UPLOAD_FILE_START) 
					{
						Serial.printf("Update ini File: %s\n", uploadini.filename.c_str());
						if (!Update.begin(UPDATE_SIZE_UNKNOWN,U_SPIFFS)) { //start with max available size
						  Update.printError(Serial);
						}
					} 
					else if (uploadini.status == UPLOAD_FILE_WRITE) 
					{
						/* flashing firmware to ESP*/
						if (Update.write(uploadini.buf, uploadini.currentSize) != uploadini.currentSize) 
						{
						  Update.printError(Serial);
						}
					} 
					else if (uploadini.status == UPLOAD_FILE_END) 
					{
						if (Update.end(true)) { //true to set the size to the current progress
						  Serial.printf("Update ini File Success: %u\nRebooting...\n", uploadini.totalSize);
						} else {
						  Update.printError(Serial);
						}
					}
				});					
#endif					
/*----------------------------Data FILE Update ---------------------------------*/				
#if 1
				/*handling uploading firmware file */
				otaserver.on("/update", HTTP_POST, []() 
				{
					otaserver.sendHeader("Connection", "close");
					otaserver.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
//					ESP.restart();
				}, []() {
					HTTPUpload& upload = otaserver.upload();
					if (upload.status == UPLOAD_FILE_START) 
					{
						Serial.printf("Update: %s\n", upload.filename.c_str());
						if (!Update.begin(UPDATE_SIZE_UNKNOWN,U_SPIFFS)) { //start with max available size
						  Update.printError(Serial);
						}
					} 
					else if (upload.status == UPLOAD_FILE_WRITE) 
					{
						/* flashing firmware to ESP*/
						if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) 
						{
						  Update.printError(Serial);
						}
#if 0						
//						TftLCDFun.OTAprogresspercent = (100 - (upload.currentSize * 100 / upload.totalSize));
						TftLCDFun.otapercent = ((float)upload.currentSize * 100 / (float)upload.totalSize);
						TftLCDFun.OTAprogresspercent = (100 - TftLCDFun.otapercent);
						TftGuiLCDFun.ProgressBarGageOTA(TftLCDFun.OTAprogresspercent,TftLCDFun.OtaStartStop);
						if(TftLCDFun.OTAprogresspercent >= 100){
							if(TftLCDFun.progresspercentSuccess) TftLCDFun.OtaStartStop = false;
						}
#endif
					} 
					else if (upload.status == UPLOAD_FILE_END) 
					{
						if (Update.end(true)) { //true to set the size to the current progress
						  Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
						} else {
						  Update.printError(Serial);
						}
					}
#endif					
				});
				otaserver.begin();
				SharedMemory.OTAWebConnection = 2;
			}
			vTaskDelay(OTAWEBCONNECTING / portTICK_PERIOD_MS);  //SharedMemory.STmin
		}	   
		else if(SharedMemory.OTAWebConnection == 1)
	    {
	    
		}
		else if(SharedMemory.OTAWebConnection == 2)
	    {
			otaserver.handleClient();
//			Serial.printf("Updating ..............................................\n");
			 
			vTaskDelay(OTATIMESETTING / portTICK_PERIOD_MS);  //SharedMemory.STmin
		}
  	}
	else
	{
  // Look for and handle WebSocket data  
  	  WiFiFnc.webSocket.loop();	

	  SharedMemory.OTAWebConnection = 0;
      vTaskDelay(SharedMemory.STmin / portTICK_PERIOD_MS);  //SharedMemory.STmin
	}
#else
//	vTaskDelay(500 / portTICK_PERIOD_MS / 1000);
	ets_delay_us(500);
#endif
}
vTaskDelete( NULL );
  
} 


void WifiCANDiag( void * parameter )
{
  while(1)
  {
    CanFnc.CANtXrXStatus = CanFnc.ReceivedCanDataFromECU();

//    WiFiFnc.SendtoWifiRespData();
	
			   
//  Serial.println("WifiCANDiag Task");
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );
  
}

void TimerTick_1ms( void * parameter )
{
  uint8_t  CAN_M_Sel = 0 ;

  while(1)
  {
  	if(ReprogData.reprogStartStop || ReprogDataFd.reprogStartStop)
	{
// 		if(ReprogData.mTimerStart)  ReprogData.timerTick++;
		ReprogData.timerTick++;
//		Serial.printf("TimerTick_1ms  [ReprogData.timerTick+]--%d\n",ReprogData.timerTick );

		ReprogDataFd.timerTick++;

	    if(ReprogData.reprogExpiredCheck == 1 && SharedMemory.File_CAN_TYPE == 1)
		{
	      ReprogmmingData.vfClearAllEnv(); 
		  TftLCDFun.SuccessFail = true;   // Display Fail
		  ReprogData.reprogStartStop = 0; // Reprogramming Fail
		  Serial.printf("TimerTick_1ms ----[TftLCDFun.SuccessFail]--%d\n",TftLCDFun.SuccessFail);
	    }
#if 0		
		else if(ReprogData.timerTick >= TIMEHSMONITOR)
		{
			ReprogmmingData.vfClearAllEnv(); 
			TftLCDFun.SuccessFail = true;	// Display Fail
			ReprogData.reprogStartStop = 0; // Reprogramming Fail
			ReprogData.timerTick       = 0;
			CanFnc.InitCanSetup();
			Serial.printf("TimerTick_1ms ----[ CanFnc.InitCanSetup]--%d\n",ReprogData.reprogStartStop );
		}
#endif
	    if(ReprogDataFd.reprogExpiredCheck == 1 && SharedMemory.File_CAN_TYPE == 2)
		{
	      ReprogmmingDataFD.vfClearAllEnv(); 
		  TftLCDFun.SuccessFail = true;   // Display Fail
		  ReprogDataFd.reprogStartStop = 0; // Reprogramming Fail
		  Serial.printf("TimerTick_1ms ----[TftLCDFun.SuccessFail]--%d\n",TftLCDFun.SuccessFail);
		  Serial.printf("TimerTick_1ms ----[ReprogDataFd.reprogExpiredCheck=]--%d\n",ReprogDataFd.reprogExpiredCheck);
	      ReprogmmingDataFD.vfClearAllEnv(); 
	    } 
		
	}
	else 
	{
	    if(!WiFiFnc.WifiWorking)
	 	{
	 		if(SharedMemory.File_CAN_TYPE == 1) 	 CAN_M_Sel = CanFnc.CAN_DIAG_MODE_Sel;
	 		else if(SharedMemory.File_CAN_TYPE == 2) CAN_M_Sel = CanFDFun.CAN_DIAG_MODE_Sel;
			
			switch(CAN_M_Sel)
			{
				case CANDIAG_NONE:
//					CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
					break;
				case CANDIAG_SWVER:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_SWVER;
						CanFnc.bFlowControl = true;  // in order to see SW Version 1.00.04
						CanFnc.sendMessageByteEcuInfoSwVersion();
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_SWVER;
						CanFDFun.sendMessageFDByteEcuInfoSwVersion();
						break;
							
					}					
					break;
				case CANDIAG_SWVER_DIAG:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_SWVER_DIAG;
						CanFnc.sendMessageByteEcuInfoSwVersionF195();
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_SWVER_DIAG;
						CanFDFun.sendMessageFDByteEcuInfoSwVersionF195();
						break;
					}		
					break;
				case CANDIAG_HWVER:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_HWVER;
						CanFnc.sendMessageByteEcuInfoHwVersion();
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_HWVER;
						CanFDFun.sendMessageFDByteEcuInfoHwVersion();
						break;
					}		
					break;
				case CANDIAG_SWVER_SUP:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_SWVER_SUP;
						CanFnc.sendMessageByteEcuInfoSwVersionSup();
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_SWVER_SUP;
						CanFDFun.sendMessageFDByteEcuInfoSwVersionSup();
						break;
					}		
					break;
				case CANDIAG_HWVER_SUP:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_HWVER_SUP;
						CanFnc.sendMessageByteEcuInfoHwVersionSup();
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_HWVER_SUP;
						CanFDFun.sendMessageFDByteEcuInfoHwVersionSup();
						break;
					}		
					break;
				case CANDIAG_DTCSTATUS_P:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_DTCSTATUS_P;
						CanFnc.sendMessageByteDtcInfoPendingStatus();
						Serial.printf("CANDIAG_DTCSTATUS_P ----[ CANDIAG_DTCSTATUS_P]--%d\n",CanFnc.CAN_DIAG_MODE_Sel );
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_DTCSTATUS_P;
						CanFDFun.sendMessageFDByteDtcInfoPendingStatus();
						Serial.printf("CANDIAG_DTCSTATUS_P ----[ CANDIAG_DTCSTATUS_P]--%d\n",CanFDFun.CAN_DIAG_MODE_Sel );
						break;
					}		
					break;		
				case CANDIAG_DTCSTATUS_A:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_DTCSTATUS_A;
						CanFnc.sendMessageByteDtcInfoAllStatus();
						Serial.printf("CANDIAG_DTCSTATUS_P ----[ CANDIAG_DTCSTATUS_P]--%d\n",CanFnc.CAN_DIAG_MODE_Sel );
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_DTCSTATUS_A;
						CanFDFun.sendMessageFDByteDtcInfoAllStatus();
						Serial.printf("CANDIAG_DTCSTATUS_P ----[ CANDIAG_DTCSTATUS_P]--%d\n",CanFDFun.CAN_DIAG_MODE_Sel );
						break;
					}		
					break;					
				case CANDIAG_DTCSTATUSCNT_P:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_DTCSTATUSCNT_P;
						CanFnc.sendMessageByteDtcInfoPendingStatusCnt();
						Serial.printf("CANDIAG_DTCSTATUSCNT_P ----[ CANDIAG_DTCSTATUSCNT_P]--%d\n",CanFnc.CAN_DIAG_MODE_Sel );
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_DTCSTATUSCNT_P;
						CanFDFun.sendMessageFDByteDtcInfoPendingStatusCnt();
						Serial.printf("CANDIAG_DTCSTATUSCNT_P ----[ CANDIAG_DTCSTATUSCNT_P]--%d\n",CanFDFun.CAN_DIAG_MODE_Sel );
						break;
					}		
					break;				
				case CANDIAG_DTCSTATUSCNT_C:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_DTCSTATUSCNT_C;
						CanFnc.sendMessageByteDtcInfoConfirmingStatusCnt();
						Serial.printf("CANDIAG_DTCSTATUSCNT_C ----[ CANDIAG_DTCSTATUSCNT_C]--%d\n",CanFnc.CAN_DIAG_MODE_Sel );
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_DTCSTATUSCNT_C;
						CanFDFun.sendMessageFDByteDtcInfoConfirmingStatusCnt();
						Serial.printf("CANDIAG_DTCSTATUSCNT_C ----[ CANDIAG_DTCSTATUSCNT_C]--%d\n",CanFDFun.CAN_DIAG_MODE_Sel );
						break;
					}		
					break;												
				case CANDIAG_DTCSTATUSCNT_A:
					switch(SharedMemory.File_CAN_TYPE)
					{
					case 1:  // HS CAN
						CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFnc.DiagModeSel = CANDIAG_DTCSTATUSCNT_A;
						CanFnc.sendMessageByteDtcInfoAllStatusCnt();
						Serial.printf("CANDIAG_DTCSTATUSCNT_C ----[ CANDIAG_DTCSTATUSCNT_C]--%d\n",CanFnc.CAN_DIAG_MODE_Sel );
						break;
					case 2:  // CAN FD
						CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_NONE;
						CanFDFun.DiagModeSel = CANDIAG_DTCSTATUSCNT_A;
						CanFDFun.sendMessageFDByteDtcInfoAllStatusCnt();
						Serial.printf("CANDIAG_DTCSTATUSCNT_C ----[ CANDIAG_DTCSTATUSCNT_C]--%d\n",CanFDFun.CAN_DIAG_MODE_Sel );
						break;
					}		
					break;												
				default: 
					break;
			}
    	}
	}
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );
  
}

void LcdDiaplyTask( void * parameter )
{

	while(1)
	{
#ifdef TFTSPI    

		TftLCDFun.TouchBtnCheck();
		
		if(TftLCDFun.StartStop)
		{
#if 0		
			TftLCDFun.timercount++;
			TftLCDFun.progresspercent = TftLCDFun.timercount/10;
			TftLCDFun.sPercent = String(TftLCDFun.progresspercent);
			TftLCDFun.sPercent += " %";
#else
            TftLCDFun.progresspercent = (100 - (ReprogData.reprogHexSize * 100 / ReprogData.reprogFileHexSize));

			TftLCDFun.sPercent = String(TftLCDFun.progresspercent);
			TftLCDFun.sPercent += "%";
#endif
			TftLCDFun.drawProgress(TftLCDFun.progresspercent,TftLCDFun.sPercent,1);
			
	        if(TftLCDFun.progresspercent >= 100){
				TftLCDFun.StartStop = false;
        	}
		//	   Serial.println("LcdDiaplySetup Task 100ms -------");
		}
		else
		{
//		   Serial.printf("LcdDiaplySetup Task 100ms ----[TftLCDFun.progresspercent]--%d\n",TftLCDFun.progresspercent);
		}

#else
	// ------------------------------------------------
	// Periodically call GUIslice update function
	// ------------------------------------------------

	if(TftLCDFun.StartStop)
	{
		switch(SharedMemory.File_CAN_TYPE)
		{
		case 1:  // HS CAN
			TftLCDFun.percent = ((float)ReprogData.reprogHexSize * 100 / (float)ReprogData.reprogFileHexSize);
			TftLCDFun.progresspercent = (100 - TftLCDFun.percent);
			TftGuiLCDFun.ProgressBarGage(TftLCDFun.progresspercent,TftLCDFun.StartStop);
			
			if(TftLCDFun.progresspercent >= 100){
				if(TftLCDFun.progresspercentSuccess) TftLCDFun.StartStop = false;
			}
			break;
		case 2:  // CAN FD
			TftLCDFun.progresspercent = (100 - (ReprogDataFd.reprogHexSize * 100 / ReprogDataFd.reprogFileHexSize));
			TftGuiLCDFun.ProgressBarGage(TftLCDFun.progresspercent,TftLCDFun.StartStop);
			
			if(TftLCDFun.progresspercent >= 100){
				if(TftLCDFun.progresspercentSuccess) TftLCDFun.StartStop = false;
			}
			break;
		}

   	}

	TftGuiLCDFun.Gui_Update();
  

#endif
		
	   vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	vTaskDelete( NULL );
}


void CanFDTxTask( void * parameter )
{
#if 0
  int id = SharedMemory.ECU2_ID;
  byte data[8];
  data[0] = 0x17;
  data[1] = 0x18;
  data[2] = 0x19;
  data[3] = 0x1a;
  data[4] = 0x1b;
  data[5] = 0x1c;
  data[6] = 0x1d;
  data[7] = 0x1e;
  byte CanSendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], data[2],data[3],data[4],data[5],data[6],data[7]};
#endif


  while(1)
  {
#if 0  
	  if(ReprogData.reprogStartStop){
		  ReprogmmingData.FlashDownload();
	  }
#else
//	 CanFDFun.sendMessageByteFD(CanSendFrame,8);
//	 Serial.printf("CanFDTxTask -----%d\n",TftLCDFun.SuccessFail);
#endif	 

  	if(ReprogDataFd.reprogStartStop)
	{
		switch(SharedMemory.File_CAN_TYPE)
		{
		case 1:  // HS CAN
			break;
		case 2:  // CAN FD
			//ReprogmmingDataFD.ReprogramInitVariable(1);
          	//CanFDFun.SendCANFDMessage_10ms();
			break;
				
		}


	}

//  Serial.printf("TimerTick_1ms ----[TftLCDFun.SuccessFail]--%d\n",TftLCDFun.SuccessFail);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );
}


void CanFDRxTask( void * parameter )
{
  while(1)
  {
    CanFDFun.CANtXrXStatus = CanFDFun.ReceivedCanFDDataFromECU();
	  
//  Serial.println("WifiCANDiag Task");
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );
  
}

void DiagCANWebsocketTask( void * parameter )
{
  while(1)
  {
  int TimeDiagCan = SharedMemory.DiagCANWebsocketTask;

	  
//  Serial.println("DiagCANWebsocketTask Task");
    vTaskDelay(TimeDiagCan / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );
  
}




void WifiCanInitSetup(void)
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Wifi CAN Module");
    Serial.println();
    Serial.println("Configuring access point...");
    Serial.println();  

//	SharedMemory.WifiBdSWVer
}

void loop() {
  // put your main code here, to run repeatedly:

}
