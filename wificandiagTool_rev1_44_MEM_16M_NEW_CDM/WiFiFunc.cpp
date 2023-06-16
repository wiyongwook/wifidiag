#include "WifiFunc.h"

#if 0
	DynamicJsonBuffer jsonBuffer(128);
	JsonObject& root = jsonBuffer.createObject();
	root["sensor"] = "gps";
	root["time"] = 1351824120;

	JsonArray& data = root.createNestedArray("data");
	data.add(48.756080, 6);  // 6 is the number of decimals to print
	data.add(2.302038, 6);   // if not specified, 2 digits are printed

	StreamString databuf;
	root.printTo(databuf);

	webSocket.sendTXT(databuf);
#else
/*	StaticJsonBuffer<600> jsonBufferTx;
	JsonObject& rootTx = jsonBufferTx.createObject();
	
	// Add a nested array.
	JsonArray& JsonDataT = rootTx.createNestedArray("data");

	uint16_t DatabufT[100];

	StaticJsonBuffer<100> jsonBufferRx;
	uint8_t Databuf[100];
	String  TxJsonData;
	uint8_t Diag0100_Status = 0;

	uint8_t j = 1;
*/	
#endif
void WiFiFuncClass::InitWebSocketSetup(void)
{
//	StaticJsonBuffer<600> jsonBufferTx;
//	StaticJsonBuffer<100> jsonBufferRx;
//	rootTx  = jsonBufferTx.createObject();
	rootTx["type"] = "Diag0100";
	rootTx["text"] = "001";
	rootTx["id"]   = "0";

				  
//  Add a nested array.
//	JsonDataT = rootTx.createNestedArray("data");
	for(int i=0; i<21; i++) {
		JsonDataT.add(0);
	}	

}

void WiFiFuncClass::InitWifiSetup(void)
{
	// Init Port Number.....
	if(TftGuiLCDFun.WifiSetting==0)
	{
		SetupWiFiAp(networkName, networkPswd, udpPort);
	}
	else
	{
	    connectToWiFi(networkName, networkPswd);
		
	//	Local_IpAddress = WiFi.localIP().toString();
	//	Serial.println("IpAddress : "+ Local_IpAddress);

	}

	InitWebSocketSetup();
}

uint8_t WiFiFuncClass:: InitConnectiongUDP(void)
{
	String TmpChar;
//	uint8_t InitStatus = 0;
//	if(InitStatus == CONNECTED) return InitStatus;

	switch(connected)
	{
		case DISCONNECT:
		  //Connect to the WiFi network
		   if(WifiConnectingCnt >= 1000){
			   connectToWiFi(networkName, networkPswd);
			   WifiConnectingCnt = 0;
//			   WiFiFnc.connected = IDLE;
		   	}		
			break;
		case CONNECTING:
		    memset(Udpbuf, 0x00, PLEN);
    		udp.parsePacket();			
		    if(udp.read(Udpbuf, PLEN) > 0)
		    {
				remoteIp = udp.remoteIP();
				Serial.print(remoteIp); Serial.print("\n");
//			 	Serial.print("Server to client: ");	Serial.println((char *)Udpbuf);
				InitStatus = CONNECTING;
				connected  = CONNECTED;
		    } 
			break;
		case CONNECTED:
			//Send a packet
			if(InitStatus == CONNECTING){
				udp.beginPacket(remoteIp,udpPort);
				udp.printf("WifiDiagOK");
				udp.endPacket();
				InitStatus = CONNECTED;
				connected  = CONNECTED;
				Serial.print("WifiDiagOK"); Serial.print("\n");
//			    TftLCDFun.WifiTextDisplay();
			}
			else
			{
				memset(Udpbuf, 0x00, PLEN);
				udp.parsePacket();			
				if(udp.read(Udpbuf, PLEN) > 0)
				{
#if 1
//					TmpChar = (char *)Udpbuf;
//					if( strncmp( TmpChar, "WifiDgStop", PLEN ) == 0 )
					if( Udpbuf[0]=='W' && Udpbuf[1]=='i' && Udpbuf[2]=='f' && Udpbuf[3]=='i' && Udpbuf[4]=='D' &&
						Udpbuf[5]=='g' && Udpbuf[6]=='S' && Udpbuf[7]=='T' && Udpbuf[8]=='o' && Udpbuf[9]=='p')
					{
						Serial.println("Server Ap Stop ");
						InitStatus = CONNECTED_STOP;
						connected  = CONNECTED_STOP;//CONNECTED_CHK;
					}
					else if( Udpbuf[0]=='W' && Udpbuf[1]=='i' && Udpbuf[2]=='f' && Udpbuf[3]=='i' && Udpbuf[4]=='D' &&
							 Udpbuf[5]=='i' && Udpbuf[6]=='a' && Udpbuf[7]=='g' && Udpbuf[8]=='R' && Udpbuf[9]=='Q')
					{
						udp.beginPacket(remoteIp,udpPort);
						udp.printf("WifiDiagOK");
						udp.endPacket();
						InitStatus = CONNECTED;
						connected  = CONNECTED;
						Serial.print("WifiDiagOK[Connected]"); Serial.print("\n");
					}
					else
					{
						memcpy(RData,Udpbuf,PLEN ); 	
						WifiDataSendingChk = true;
					}
#else
					memcpy(RData,Udpbuf,PLEN ); 	
					WifiDataSendingChk = true;
#endif
				} 
			}
			break;
		case CONNECTED_STOP:
			memset(Udpbuf, 0x00, PLEN);
			udp.parsePacket();			
			if(udp.read(Udpbuf, PLEN) > 0)
			{
				remoteIp = udp.remoteIP();
				Serial.print(remoteIp); Serial.print("\n");
			 	Serial.print("[CONNECTED_STOP] Server to client: ");	Serial.println((char *)Udpbuf);
				InitStatus = CONNECTING;
				connected  = CONNECTED;
			} 
			break;
		default: 
			break;
			
	}

	WifiConnectingCnt++;

return InitStatus;

}


uint8_t WiFiFuncClass:: InitApModeConnectiongUDP(void)
{
	String TmpChar;

	switch(WiFiFnc.ApModeConStatus)
	{
		case STARTING:
		case READY:
		case STOP:
			 WifiDataSendingChk = false;
			 break;
		case AP_STADISCONNECTED:
			 WifiDataSendingChk = false;
			 WifiDisConnection();
 			 break;
		case AP_STACONNECTED:
			 WiFiFnc.ApModeConStatus = AP_UDPCONNECTING;
			 Serial.print("[AP_STACONNECTED]"); Serial.print("\n");
			 break;
//		case AP_UDPCONNECTING:
		case AP_STAIPASSIGNED:
		    memset(Udpbuf, 0x00, PLEN);
    		udp.parsePacket();			
		    if(udp.read(Udpbuf, PLEN) > 0)
		    {
				Serial.print("[AP_STAIPASSIGNED-DATA RECEIVED]"); Serial.print("\n");

				if( Udpbuf[0]=='W' && Udpbuf[1]=='i' && Udpbuf[2]=='f' && Udpbuf[3]=='i' && Udpbuf[4]=='D' &&
				    Udpbuf[5]=='i' && Udpbuf[6]=='a' && Udpbuf[7]=='g' && Udpbuf[8]=='R' && Udpbuf[9]=='Q')
				{
					remoteIp = udp.remoteIP();
					Serial.print(remoteIp); Serial.print("\n");
					InitStatus = AP_UDPCONNECTEDOK;
					connected  = AP_UDPCONNECTEDOK;
					WiFiFnc.ApModeConStatus = AP_UDPCONNECTEDOK;
					TftGuiLCDFun.InitParamSet = false;

					udp.beginPacket(remoteIp,udpPort);
					udp.printf("WifiDiagOK");
					udp.endPacket();
					Serial.print("WifiDiagOK[Connected]"); Serial.print("\n");
				}
		    } 
			else{
//				Serial.print("[AP_UDPCONNECTING]"); Serial.print("\n");
			}
			break;
		case AP_UDPCONNECTEDOK:
			memset(Udpbuf, 0x00, PLEN);
			udp.parsePacket();			
			if(udp.read(Udpbuf, PLEN) > 0)
			{
#if 1
				if( Udpbuf[0]=='W' && Udpbuf[1]=='i' && Udpbuf[2]=='f' && Udpbuf[3]=='i' && Udpbuf[4]=='D' &&
					Udpbuf[5]=='g' && Udpbuf[6]=='S' && Udpbuf[7]=='t' && Udpbuf[8]=='o' && Udpbuf[9]=='p')
				{
					Serial.println("Server Ap Stop ");
					InitStatus = AP_UDPCONNECTSTOP;
					connected  = AP_UDPCONNECTSTOP;//CONNECTED_CHK;
					WiFiFnc.ApModeConStatus = AP_UDPCONNECTSTOP;
					TftGuiLCDFun.InitParamSet = false;
					WifiDataSendingChk =  false;
				}
				else
				{
					memcpy(RData,Udpbuf,PLEN ); 	
					WifiDataSendingChk = true;
				}
#else
				memcpy(RData,Udpbuf,PLEN ); 	
				WifiDataSendingChk = true;
#endif
			} 
			else
			{
//				Serial.print("[AP_UDPCONNECTEDOK]"); Serial.print("\n");  //WifiDataSendingChk = true;
//				Serial.printf("[WifiDataSendingChk] - %d",WifiDataSendingChk); Serial.print("\n");  
			}
			
			break;
		case AP_UDPCONNECTSTOP:
			memset(Udpbuf, 0x00, PLEN);
			udp.parsePacket();			
			if(udp.read(Udpbuf, PLEN) > 0)
			{
				if( Udpbuf[0]=='W' && Udpbuf[1]=='i' && Udpbuf[2]=='f' && Udpbuf[3]=='i' && Udpbuf[4]=='D' &&
				    Udpbuf[5]=='i' && Udpbuf[6]=='a' && Udpbuf[7]=='g' && Udpbuf[8]=='R' && Udpbuf[9]=='Q')
				{
					remoteIp = udp.remoteIP();
					Serial.print(remoteIp); Serial.print("\n");
					InitStatus = AP_UDPCONNECTEDOK;
					connected  = AP_UDPCONNECTEDOK;
					WiFiFnc.ApModeConStatus = AP_UDPCONNECTEDOK;
					TftGuiLCDFun.InitParamSet = false;

					udp.beginPacket(remoteIp,udpPort);
					udp.printf("WifiDiagOK");
					udp.endPacket();
					Serial.print("WifiDiagOK[AP_UDPCONNECTSTOP - Connected]"); Serial.print("\n");
				}
			} 
			else{
//				WifiDataSendingChk =  false;
			}
			break;
		default: 
			break;
			
	}
	WifiConnectingCnt++;

return InitStatus;

}

void WiFiFuncClass:: WififuncUDP(void)
{
	
	ConStatus = InitConnectiongUDP();

	if((ConStatus == CONNECTED) && WifiDataSendingChk)
//	if((ConStatus == CONNECTED))
	{
		WifiDataSendingChk = false;
		WifiConnectingCnt = 0;
		WifiMsgtoCANSend(RData);
		Serial.print("WifiMsgtoCANSend :		  ");  for(int i=0;i<PLEN;i++) Serial.printf("%02X ",RData[i]); Serial.print("\n");
	}

}

void WiFiFuncClass::WifiApModefuncUDP(void)
{
	
	ConStatus = InitApModeConnectiongUDP();

	if((ConStatus == AP_UDPCONNECTEDOK) && WifiDataSendingChk)
	{
		WifiDataSendingChk = false;
		WifiConnectingCnt = 0;
		WifiMsgtoCANSend(RData);
		Serial.print("WifiMsgtoCANSend :		  ");  for(int i=0;i<PLEN;i++) Serial.printf("%02X ",RData[i]); Serial.print("\n");
	}

}


void WiFiFuncClass:: WifiDisConnection(void)
{
	if(WiFiFnc.ApModeConStatus == AP_UDPCONNECTEDOK)
	{
		WiFiFnc.ApModeConStatus = STOP;
	    InitStatus	 = 0;
		connected 	 = 0;
		// close the connection:
		WifiConnected = false;	// close the connection:
//		client.stop();
		Serial.println("[WifiDisConnection] -- Client Disconnected.");
	}
}


void WiFiFuncClass::SetupWiFiAp(const char * ServerSsid, const char * ServerPwd, uint16_t Port)
{
    // delete old config
    WiFi.disconnect(true);
    //register event handler
    WiFi.onEvent(WiFiEvent);

	// You can remove the password parameter if you want the AP to be open.
	WiFi.mode(WIFI_AP);
	WiFi.softAP(ServerSsid, ServerPwd);
	//	WiFi.begin(ssid, password);
	//	WiFi.begin("spiderman", "spiderboy");

	Serial.println("Wait 100 ms for Server START...");
	delay(100);

   // You can Set softAPConfig ......
    Serial.println("Set softAPConfig");
    IPAddress Ip(192, 168, 1, 100);	   // if you want to set IP each devicee for WIFICAN, Changing this Part....
    IPAddress NMask(255, 255, 255, 0);
    WiFi.softAPConfig(Ip, Ip, NMask);
	 
	 // You can Set AP IP address...
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

	Local_IpAddress = WiFi.localIP().toString();
	Serial.println("IpAddress : "+ Local_IpAddress);

	//Initiate connection
//	WiFi.begin(ServerSsid, ServerPwd);

	Serial.println("Staring for WIFI Ap Mode .........");

	// Starting Sever AP ...
//	server.begin(Port);
//	Serial.println("Server started");	
  
}


void WiFiFuncClass::connectToWiFi(const char * ServerSsid, const char * ServerPwd)
{
  Serial.println("Connecting to WiFi network: " + String(ServerSsid)); 

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);

  Local_IpAddress = WiFi.localIP().toString();
  Serial.println("IpAddress : "+ Local_IpAddress);
  
  //Initiate connection
  WiFi.begin(ServerSsid, ServerPwd);

  Serial.println("Waiting for WIFI connection...");
}

#if 1
//wifi event handler
void WiFiFuncClass::WiFiEvent(WiFiEvent_t event)
{
    switch(event) 
	{
	case SYSTEM_EVENT_AP_START:   
       	TftGuiLCDFun.InitParamSet = false;
		WiFiFnc.ApModeConStatus =  STARTING;
		Serial.println("WiFi access point started");            
		break;        
	case SYSTEM_EVENT_AP_STOP:
       	TftGuiLCDFun.InitParamSet = false;
		WiFiFnc.ApModeConStatus =  STOP;
		Serial.println("WiFi access point  stopped"); 
		break;        
	case SYSTEM_EVENT_AP_STACONNECTED: 
       	TftGuiLCDFun.InitParamSet = false;
		WiFiFnc.ApModeConStatus =  AP_STACONNECTED;
		Serial.println("Client connected");  
		break;      
	case SYSTEM_EVENT_AP_STADISCONNECTED:    
       	TftGuiLCDFun.InitParamSet = false;
		WiFiFnc.ApModeConStatus =  AP_STADISCONNECTED;
		Serial.println("Client disconnected");
		break;     
	case SYSTEM_EVENT_AP_STAIPASSIGNED:    
       	TftGuiLCDFun.InitParamSet = false;
		WiFiFnc.ApModeConStatus =  AP_STAIPASSIGNED;
		Serial.println("Assigned IP address to client");   
#if 0
		//When connected set 
		Serial.print("WiFi connected! IP address: ");
		Serial.println(WiFi.localIP());  
		//initializes the UDP state
		//This initializes the transfer buffer
		WiFiFnc.udp.begin(WiFi.localIP(),WiFiFnc.udpPort);
//		WiFiFnc.udp.begin(WiFiFnc.SeverIp.fromString(WiFiFnc.host),WiFiFnc.udpPort);
		WiFiFnc.remoteIp = WiFiFnc.udp.remoteIP();
		Serial.print(", Server IP :  ");
		Serial.println(WiFiFnc.remoteIp);
		Serial.print(", port ");
		Serial.println(WiFiFnc.udp.remotePort());
#else

		//When connected set 
		//initializes the UDP state
		//This initializes the transfer buffer
		Serial.printf(", Starting UDP Server :	\n");
//		WiFiFnc.udp.begin(WiFiFnc.SeverIp.fromString(WiFiFnc.host),WiFiFnc.udpPort);
		WiFiFnc.udp.begin(WiFiFnc.udpPort);
//		WiFiFnc.remoteIp = WiFiFnc.udp.remoteIP();
		Serial.print(", Server IP :  ");
		Serial.println(WiFiFnc.host);
		Serial.print(", port ");
		Serial.println(WiFiFnc.udpPort);
#endif		
		break;        
	case SYSTEM_EVENT_AP_PROBEREQRECVED:      
       	TftGuiLCDFun.InitParamSet = false;
		WiFiFnc.ApModeConStatus =  AP_PROBEREQRECVED;
		Serial.println("Received probe request");     
		break;	
#if 0	
	  case SYSTEM_EVENT_WIFI_READY:             
	  	   Serial.println("WiFi interface ready"); 
		   break;
	  case SYSTEM_EVENT_SCAN_DONE:
	  	   Serial.println("Completed scan for access points");
		   break;
	  case SYSTEM_EVENT_STA_START:
		   WiFiFnc.Local_IpAddress = WiFi.localIP().toString();
		   Serial.println("IpAddress : "+ WiFiFnc.Local_IpAddress);
		
	  	   Serial.println("WiFi client started");
		   break;
	  case SYSTEM_EVENT_STA_STOP:
	  	   Serial.println("WiFi clients stopped");
		   break;
#endif		   
      case SYSTEM_EVENT_STA_LOST_IP:
	  	   Serial.println("Lost IP address and IP address is reset to 0");
		   WiFiFnc.connected  = DISCONNECT;
		   WiFiFnc.InitStatus = DISCONNECT;
		   break;
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          WiFiFnc.udp.begin(WiFi.localIP(),WiFiFnc.udpPort);
          WiFiFnc.remoteIp = WiFiFnc.udp.remoteIP();
          Serial.print(", Server IP :  ");
          Serial.println(WiFiFnc.remoteIp);
          Serial.print(", port ");
          Serial.println(WiFiFnc.udp.remotePort());

//		  TftLCDFun.WifiTextDisplay();
		  WiFiFnc.Local_IpAddress = WiFi.localIP().toString();
		  Serial.println("IpAddress : "+ WiFiFnc.Local_IpAddress);

          WiFiFnc.connected = CONNECTING;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          WiFiFnc.connected  = DISCONNECT;
		  WiFiFnc.InitStatus = DISCONNECT;
          break;
		  
    }
}
#endif

void WiFiFuncClass::WifiMsgtoCANSend(uint8_t *WifimsgDataFrame)
{
 int id;
 
 byte data[8];
 byte wifimsgsendFrame[10] = { (byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], (byte)data[2],(byte)data[3],(byte)data[4],(byte)data[5],(byte)data[6],(byte)data[7]};

#if 0
     if((WifimsgDataFrame[0] == 0x00) && (WifimsgDataFrame[1] == 0x00) )
     {
     	memcpy(wifimsgsendFrame,&WifimsgDataFrame[1],PDLC);
		CanFnc.sendMessageByte(wifimsgsendFrame);
     }
	 else
 	 {
 	 
 	 }
#else
	if(TftGuiLCDFun.WifiSetting)
	{
		if (ConStatus == CONNECTED  )  // loop while the client's connected
		{			
			CanFnc.sendMessageByte(WifimsgDataFrame);
		}
		else
		{
			Serial.println("[WifiMsgtoCANSend fnc-STA MODE]  Client Disconnected.");
		}
	}
	else
	{
		if (ConStatus == AP_UDPCONNECTEDOK  )  // loop while the client's connected
		{			
			CanFnc.sendMessageByte(WifimsgDataFrame);
		}
		else
		{
			Serial.println("[WifiMsgtoCANSend fnc - AP MODE]  Client Disconnected.");
		}
	}
#endif

}
void WiFiFuncClass::CANMsgSendtoWifi(uint8_t *CANMsgtoWifiFrame)
{
	int  id;
	id = CanFnc.rx_frame.MsgID;
	byte data[8];
	byte CANmsgWifiSendFrame[10] = {(byte)((id & 0xFF00)>>8), (byte)(id & 0xFF), (byte)data[0], (byte)data[1], data[2],data[3],data[4],data[5],data[6],data[7]};

//	Serial.printf("CANMsgSendtoWife :\n");

	if(TftGuiLCDFun.WifiSetting)
	{
		if (ConStatus == CONNECTED)  // loop while the client's connected
		{			
	//		Serial.printf("ConStatus == CONNECTED : \n");
			memcpy(&CANmsgWifiSendFrame[2],CANMsgtoWifiFrame,WDLC);
			
			Serial.print("CANmsgWifiSendFrame :                   ");		for(int i=0;i<PLEN;i++) Serial.printf("%02X ",CANmsgWifiSendFrame[i]); Serial.print("\n");
	//		Serial.print("WifiMsgtoCANSend :				  ");  for(int i=0;i<PLEN;i++) Serial.printf("%02X ",RData[i]); Serial.print("\n");

	//		client.write(CANmsgWifiSendFrame, PLEN);
			udp.beginPacket(remoteIp,udpPort);
			udp.write(CANmsgWifiSendFrame, PLEN);;
	//		udp.printf(CANmsgWifiSendFrame);
			udp.endPacket();

		}
	}
	else
	{
			if (ConStatus == AP_UDPCONNECTEDOK)  // loop while the client's connected
			{			
				memcpy(&CANmsgWifiSendFrame[2],CANMsgtoWifiFrame,WDLC);
				Serial.print("CANmsgWifiSendFrame : 				  ");		for(int i=0;i<PLEN;i++) Serial.printf("%02X ",CANmsgWifiSendFrame[i]); Serial.print("\n");
				udp.beginPacket(remoteIp,udpPort);
				udp.write(CANmsgWifiSendFrame, PLEN);;
				udp.endPacket();
			}
	
	}

}

void WiFiFuncClass::CANMsgSendtoWifiInfo(uint8_t Info)
{

	int  id;
	id = CanFnc.rx_frame.MsgID;
	byte data[8];
	byte CANmsgWifiSendFrameInfo[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		
//	Serial.printf("CANMsgSendtoWife :\n");

	if (ConStatus == CONNECTED)  // loop while the client's connected
	{			
//		Serial.printf("ConStatus == CONNECTED : \n");
//		memcpy(&CANmsgWifiSendFrame[2],CANMsgtoWifiFrame,WDLC);

		switch(Info)
		{
		case 0:
			CANmsgWifiSendFrameInfo[0] = 0x01;
			CANmsgWifiSendFrameInfo[1] = 0x08;
			for(int i=2;i<PLEN;i++) CANmsgWifiSendFrameInfo[i] = 0x00;			
//			client.write(CANmsgWifiSendFrameInfo, PLEN);
			udp.beginPacket(remoteIp,udpPort);
//			udp.printf(CANmsgWifiSendFrame);
			udp.endPacket();

			Serial.print("CANmsgWifiSendFrameInfo :  ");		for(int i=0;i<PLEN;i++) Serial.printf("%02X ",CANmsgWifiSendFrameInfo[i]); Serial.print("\n");
			break;
		case 1:
			
			break;
		default:
			break;
		}
		
	}
}


void WiFiFuncClass::SendtoWifiRespData(void)
{

	if(WifiDataSelect == 1)
	{
		switch(WifiDataSndChk)
		{
			case 0:
				CANMsgSendtoWifiInfo(0);						
				WifiDataSndChk=1;
				break;
			case 1:
				CANMsgSendtoWifiInfo(1);						
				break;

			default : 
				break;
		}
	}
}

// Callback: receiving any WebSocket message
void WiFiFuncClass::onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length)
{

#if 0
	//   "type": "type",  
	//   "text": "text",  
	// 	 "data": Databuf  

	// Most of the time, you can rely on the implicit casts.	
	// In other case, you can do doc["time"].as<long>();  
	const char* typeS = WiFiFnc.doc["type"];
	const char* textS = WiFiFnc.doc["text"];
	const char* ipS   = WiFiFnc.doc["id"];
	uint8_t dataS     = WiFiFnc.doc["data"][0];
#endif	

	// Figure out the type of WebSocket event 
	switch(type) 
	{	 // Client has disconnected    
		case WStype_DISCONNECTED:	  
			Serial.printf("[%u] Disconnected!\n", client_num);	  
			break;	
			// New client has connected    
		case WStype_CONNECTED:	   
			{		
			IPAddress ip = WiFiFnc.webSocket.remoteIP(client_num);
			Serial.printf("[%u] Connection from ", client_num);
			Serial.println(ip.toString()); 	 
			}		
			break;	  
			// Handle text messages from client	 
		case WStype_TEXT:
			{
#if 1
				// Print out raw message	  
#ifdef DEBUG__
				Serial.printf("[%u] Received text: %s \n", client_num, payload);    
#endif
				JsonObject& rootRx = WiFiFnc.jsonBufferRx.parseObject(payload);
				WiFiFnc.jsonBufferRx.clear();
				const char* typeC = rootRx["type"];
				const char* textC = rootRx["text"];
				const char* idC   = rootRx["id"];
//				Databuf = rootRx["data"];

#ifdef DEBUG__
				Serial.printf("typeC : "); Serial.println(typeC);	
				Serial.printf("textC : "); Serial.println(textC);	
				Serial.printf("idC   : "); Serial.println(idC);	
#endif

				// Toggle LED	  
				if( strcmp((char *)typeC, "LEDOnOff") == 0 ) 
				{		
					if(WiFiFnc.led_state)
					{
						Serial.printf("LEDOnOff LED to %u\n", WiFiFnc.led_state);	
						WiFiFnc.Diag0100_Status =  2;
						WiFiFnc.led_state = WiFiFnc.led_state ? 0 : 1;		  
						WiFiFnc.TxJsonData = "";
						
						WiFiFnc.rootTx["type"] = "led";
						WiFiFnc.rootTx["text"] = "1" ;
						WiFiFnc.rootTx.printTo(WiFiFnc.TxJsonData);
					
#ifdef DEBUG__
						Serial.print(F("Sending: "));
						Serial.print(WiFiFnc.TxJsonData);
						Serial.println();
#endif
						WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.TxJsonData);	 
						
					}
					else 
					{
						Serial.printf("LEDOnOff LED to %u\n", WiFiFnc.led_state);	
						WiFiFnc.Diag0100_Status =  0;
						WiFiFnc.led_state = WiFiFnc.led_state ? 0 : 1;		  
						WiFiFnc.TxJsonData = "";
						
						WiFiFnc.rootTx["type"] = "led";
						WiFiFnc.rootTx["text"] = "0";
						WiFiFnc.rootTx.printTo(WiFiFnc.TxJsonData);
					
#ifdef DEBUG__
						Serial.print(F("Sending: "));
						Serial.print(WiFiFnc.TxJsonData);
						Serial.println();
#endif
						WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.TxJsonData);	 

					}
				} 
				else if(strcmp((char *)typeC, "SwVersion") == 0 ) 
				{	
					WiFiFnc.Diag0100_Status =  0;
					WiFiFnc.TxJsonData = "";
					sprintf(WiFiFnc.msg_buf, "%d", WiFiFnc.swversion);	   
					Serial.printf("Sending to SwVersion[%u]: %s\n", client_num, WiFiFnc.msg_buf);
					WiFiFnc.rootTx["type"] = "swver";
					WiFiFnc.rootTx["text"] = WiFiFnc.msg_buf ;
					WiFiFnc.rootTx.printTo(WiFiFnc.TxJsonData);
#ifdef DEBUG__
					Serial.print(F("Sending:SwVersion "));
					Serial.print(WiFiFnc.TxJsonData);
					Serial.println();
#endif					
					WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.TxJsonData);	 
					// Message not recognized	   
				}
				else if(strcmp((char *)typeC, "HwVersion") == 0 ) 
				{	
					WiFiFnc.Diag0100_Status =  0;
					WiFiFnc.TxJsonData = "";
					sprintf(WiFiFnc.msg_buf, "%d", WiFiFnc.hwversion);	   
					Serial.printf("Sending to HwVersion [%u]: %s\n", client_num, WiFiFnc.msg_buf);
					WiFiFnc.rootTx["type"] = "hwver";
					WiFiFnc.rootTx["text"] = WiFiFnc.msg_buf ;
					WiFiFnc.rootTx.printTo(WiFiFnc.TxJsonData);
#ifdef DEBUG__
					Serial.print(F("Sending:HwVersion "));
					Serial.print(WiFiFnc.TxJsonData);
					Serial.println();
#endif					
					WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.TxJsonData);	 
					// Message not recognized	   
				}
				else if(strcmp((char *)typeC, "DiagSignalOn") == 0 ) 
				{
	                WiFiFnc.Diag0100_Status = 2;
				}
				else if(strcmp((char *)typeC, "DiagSignalOff") == 0 ) 
				{
					WiFiFnc.Diag0100_Status = 0;
				}
			    else 
				{ 	   
				    Serial.println("[%u] Message not recognized");	   
				}	

  			// Message Sending	   
				if(WiFiFnc.Diag0100_Status == 2 )
				{
					if(strcmp((char *)textC, "DiagOn") == 0 )
					{
					  WiFiFnc.Diag0100_Status = 0;
					  WiFiFnc.TxJsonData = "";

#if 1
					  WiFiFnc.rootTx["type"] = "Diag0100";
					  WiFiFnc.rootTx["text"] = "001";
					  WiFiFnc.rootTx["id"]   = "0";
//					  rootTx["data"] = JsonDataT[0];
#endif
					  for(int i=0; i<21; i++) {
						  WiFiFnc.DatabufT[i] = random(100);
						  WiFiFnc.JsonDataT.set(i,WiFiFnc.DatabufT[i]);
	//					  Serial.printf("%d ",DatabufT[i]);
					  }
					  
					  WiFiFnc.rootTx.printTo(WiFiFnc.TxJsonData);
#ifdef DEBUG__
					  Serial.print(F("Sending:DiagOn "));
					  Serial.print(WiFiFnc.TxJsonData);
					  Serial.println();
#endif					  
					  WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.TxJsonData);   
					} 
					else if(strcmp((char *)textC, "DiagOff") == 0 )
					{
					  WiFiFnc.Diag0100_Status = 0;
					  WiFiFnc.TxJsonData = "";
					  
					  WiFiFnc.rootTx["type"] = "Diag0100";
					  WiFiFnc.rootTx["text"] = "002";
//					  rootTx["data"] = DatabufT;
					  WiFiFnc.rootTx.printTo(WiFiFnc.TxJsonData);
#ifdef DEBUG__
					  Serial.print(F("Sending:DiagOff "));
					  Serial.print(WiFiFnc.TxJsonData);
					  Serial.println();
#endif					  
					  WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.TxJsonData);   
					}
				}
				else if(WiFiFnc.Diag0100_Status == 0 )
				{
	                WiFiFnc.Diag0100_Status = 1;
				}
			}
			break;	// For everything else: do nothing	  
#else
			// Print out raw message	  
			Serial.printf("[%u] Received text: %s\n", client_num, payload);	   
			// Toggle LED	  
			if( strcmp((char *)payload, "LEDOnOff") == 0 ) 
			{		
				WiFiFnc.led_state = WiFiFnc.led_state ? 0 : 1;		  
				sprintf(WiFiFnc.msg_buf, "%d", WiFiFnc.led_state); 	   
				Serial.printf("LEDOnOff LED to %u\n", WiFiFnc.led_state); 	   
				WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.msg_buf);		
	//			digitalWrite(led_pin, led_state);	  
				// Report the state of the LED	  
			} 
			else if(strcmp((char *)payload, "SwVersion") == 0 ) 
			{	
				sprintf(WiFiFnc.msg_buf, "%d", WiFiFnc.swversion); 	   
				Serial.printf("Sending to SwVersion[%u]: %s\n", client_num, WiFiFnc.msg_buf);
				WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.msg_buf);		
				// Message not recognized	   
			}
			else if(strcmp((char *)payload, "HwVersion") == 0 ) 
			{	
				sprintf(WiFiFnc.msg_buf, "%d", WiFiFnc.hwversion); 	   
				Serial.printf("Sending to HwVersion [%u]: %s\n", client_num, WiFiFnc.msg_buf);
				WiFiFnc.webSocket.sendTXT(client_num, WiFiFnc.msg_buf);		
				// Message not recognized	   
			}
		    else 
			{ 	   
			    Serial.println("[%u] Message not recognized");	   
			}	
			break;	// For everything else: do nothing	  
#endif
		case WStype_BIN:	  
		case WStype_ERROR:	
		case WStype_FRAGMENT_TEXT_START:	
		case WStype_FRAGMENT_BIN_START:    
		case WStype_FRAGMENT:	
		case WStype_FRAGMENT_FIN:	 
		default:	   
			break;
	}
}

// Callback: send homepage
void WiFiFuncClass::onIndexRequest(AsyncWebServerRequest *request)
{
#if 1
	IPAddress remote_ip = request->client()->remoteIP();  
	Serial.println("[" + remote_ip.toString() +				   "] HTTP GET request of " + request->url());
	request->send(SPIFFS, "/index.html", "text/html");
#endif	
}

// Callback: send style sheet
void WiFiFuncClass::onCSSRequest(AsyncWebServerRequest *request)
{
	IPAddress remote_ip = request->client()->remoteIP();
	Serial.println("[" + remote_ip.toString() +				   "] HTTP GET request of " + request->url());
	request->send(SPIFFS, "/wifidiag.css", "text/css");
}

// Callback: send 404 if requested file does not exist
void WiFiFuncClass::onPageNotFound(AsyncWebServerRequest *request)
{
	IPAddress remote_ip = request->client()->remoteIP();
	Serial.println("[" + remote_ip.toString() +                  "] HTTP GET request of " + request->url());
	request->send(404, "text/plain", "Not found");
}
