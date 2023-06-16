#ifndef WiFiFunc_h
#define WiFiFunc_h

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>

#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

#include "CanFunc.h" 
#include "Reprogramming.h"
#include "TftLCDFun.h"
#include "TftGuiLCDFun.h"

#define ReadDataByIdentifier_ECUSoftwareVersion       0
#define ReadDataByIdentifier                          1
#define DiagnosticSessionControl_Extended_Session     2
#define ControlDTCSetting_OFF                         3
#define CommunicationControl_disableRxAndTx           4
#define DiagnosticSessionControl_programmingSession   5
#define SecurityAccess_Reques_Seed                    6
#define SecurityAccess_Send_KEY                       7
#define RoutineControl_eraseMemory_APP                8
#define Transfer_App_File                             9
#define RoutineControl_checkprogrammingdependency     10
#define ECUReset_hardReset                            11
#define CommunicationControl_Enable_RxAndTx           12
#define ReadDataByIdentifier_ECUSoftwareVersion_L     13
#define ReadDataByIdentifier_L                        14
#define Exit                                          15

#define ECUSoftwareVersion       					  0
#define ECUHardwareVersion	                          1
#define SUPECUSoftwareVersion       				  2
#define SUPECUHardwareVersion	                      3
#define ECUSoftwareVersionF195	                      4

#define DTCStatusPending	       					  0
#define DTCStatusConfirming	                          1
#define DTCStatusALL								  2
#define DTCStatusPendingCnt	       					  3
#define DTCStatusConfirmingCnt	                      4
#define DTCStatusALLCnt								  5


#define WDLC     8   
#define PDLC     10-2   
#define PLEN     10   
#define TRUE     1   
#define FALSE    0   
#define BPLEN    512   
#define FDMAXLEN 64   


#define STARTING	    		0   
#define READY					1   
#define STOP					2   
#define AP_STADISCONNECTED		3
#define AP_STACONNECTED 		4
#define AP_STAIPASSIGNED		5
#define AP_PROBEREQRECVED		6
#define AP_UDPCONNECTING		7
#define AP_UDPCONNECTEDOK		8
#define AP_UDPCONNECTSTOP		9

#define DISCONNECT	    0   
#define CONNECTING      1 
#define CONNECTED       2 
#define CONNECTED_STOP  3 
#define IDLE          100

//#define DEBUG__         1

class WiFiFuncClass 
{
public:
  void InitWifiSetup(void);
  void InitWebSocketSetup(void);
//  void WifiFucRoutine(void);
  void WifiDisConnection(void);
  void WifiMsgtoCANSend(uint8_t *WifimsgDataFrame);
  void CANMsgSendtoWifi(uint8_t *CANMsgtoWifiFrame);
  void CANMsgSendtoWifiInfo(uint8_t Info);
  void SendtoWifiRespData(void);
  void WififuncUDP(void);
  void WifiApModefuncUDP(void);
  void connectToWiFi(const char * ServerSsid, const char * ServerPwd);
  void SetupWiFiAp(const char * ServerSsid, const char * ServerPwd, uint16_t udpPort);

  uint8_t InitConnectiongUDP(void);
  uint8_t InitApModeConnectiongUDP(void);
  static void WiFiEvent(WiFiEvent_t event);

  static void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length);
  static void onIndexRequest(AsyncWebServerRequest *request); 
  static void onCSSRequest(AsyncWebServerRequest *request) ;
  static void onPageNotFound(AsyncWebServerRequest *request);

   
public:
  uint8_t  CustomInfo  ;
  uint8_t  VehicleInfo ;
  uint8_t  ProductInfo ;
  uint8_t  Baudrate    ;
  uint8_t  Tp_Time     ;
  uint8_t  Reserved[3] ;
  uint16_t CANiD       ;
  uint16_t TimerCnt    = 0;
  uint8_t  DLCInfo     ;
  uint8_t  Data[8]     ;
  uint8_t  CheckSum    ;
  
  uint8_t  RData[PLEN] ;
  uint8_t  SData[PLEN] ;
  uint8_t  RBData[BPLEN] ;
  
  uint8_t  WifiConnected  = 0;
  uint8_t  WifiConnecting = 0;
  uint8_t  WifiWorking = 1;
  uint8_t  WifiDataSelect = 0;
  uint8_t  WifiDataSndChk = 0;
	
  uint16_t  WifiConnectingCnt = 0;
  uint16_t  WifiDataSendingChk= 0;
  uint8_t   ConStatus = 0;

  char msg_buf[100];
  int led_state = 0;
  int swversion = 10;
  int hwversion = 20;

public:
   // Set these to your desired credentials.

	// WiFi network name and password:
 	const char * networkName = "SLWIFIDIAG01"; //iptime_1071506;;
	const char * networkPswd = "0123456789";	 //dong0303;//
    const char * host = "192.168.1.100"; // ip or dns
    String Local_IpAddress ; // ip or dns
    
	const uint16_t LASTIP = 100;
	const uint16_t udpPort = 9000;
	IPAddress remoteIp;
	IPAddress SeverIp;
	
	//IP address to send UDP data to:
	// either use the ip address of the server or 
	// a network broadcast address

	//Are we currently connected?
	uint8_t connected = DISCONNECT;
	uint8_t ApModeConStatus = STARTING;
	
	uint8_t InitStatus = 0;
	
	uint8_t buffer[1024];
	uint8_t Udpbuf[10];
	int  len,cnt =0;
	
	//The udp library class
	WiFiUDP udp;
	WiFiServer server;
	WiFiClient client;

//	AsyncWebServer Webserver(80);
	WebSocketsServer webSocket = WebSocketsServer(9000);

	StaticJsonBuffer<600> jsonBufferTx;
	StaticJsonBuffer<100> jsonBufferRx;

	JsonObject& rootTx   = jsonBufferTx.createObject();
	JsonArray& JsonDataT = rootTx.createNestedArray("data");

	uint16_t DatabufT[100];
	uint8_t  Databuf[100];
	String	 TxJsonData;
	uint8_t  Diag0100_Status = 0;
	uint8_t j = 1;

};

extern WiFiFuncClass WiFiFnc;




#endif // WiFiFunc_h
