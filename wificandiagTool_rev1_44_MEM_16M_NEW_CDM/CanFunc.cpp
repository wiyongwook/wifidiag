#include "CanFunc.h" 
#include "WifiFunc.h" 
#include "Reprogramming.h"

//SharedMemoryClass   SharedMemory;

CAN_device_t CAN_cfgs;

uint8_t SendMessageEcuInfo[5][WDLC] = {
//  							   {0x03,0x22,0xF1,0xA0,0x55,0x55,0x55,0x55},   // System SW ECUSoftwareVersion
									   {0x03,0x22,0xF1,0x89,0x55,0x55,0x55,0x55},   // System SW ECUSoftwareVersion //2021.08.22 modify CDM internal SW changed
//                                     {0x03,0x22,0xF1,0x95,0x55,0x55,0x55,0x55},  	// System Supplier ECUSoftwareVersion
									   {0x03,0x22,0xF1,0x91,0x55,0x55,0x55,0x55},	// System HW ECUSHardwareVersion
									   {0x03,0x22,0xF1,0x94,0x55,0x55,0x55,0x55},   // System Supplier ECUSoftwareVersion
                                       {0x03,0x22,0xF1,0x92,0x55,0x55,0x55,0x55},
//									   {0x03,0x22,0xF1,0x95,0x55,0x55,0x55,0x55}    // System Supplier ECUSoftwareVersionF195
									   {0x03,0x22,0xF1,0x89,0x55,0x55,0x55,0x55}    // System Supplier ECUSoftwareVersionF189
									   };  // System Supplier ECUHardwareVersion


uint8_t SendMessagedtcInfo[6][WDLC] = {{0x03,0x19,0x02,0x01,0x55,0x55,0x55,0x55},   // Pending DTC
									   {0x03,0x19,0x02,0x08,0x55,0x55,0x55,0x55},	// Confirmed DTC
									   {0x04,0x14,0xFF,0xFF,0xFF,0x55,0x55,0x55},   // Pending & Confirmed DTC
									   {0x03,0x19,0x01,0x01,0x55,0x55,0x55,0x55},   // Pending DTC Count
									   {0x03,0x19,0x01,0x08,0x55,0x55,0x55,0x55},   // Confirmed DTC Count
									   {0x04,0x14,0xFF,0xFF,0xFF,0x55,0x55,0x55}};  // Pending & Confirmed DTC Count


uint8_t SendMessageEcuInfoV1[5][WDLC] = {
									   {0x03,0x22,0xF1,0xA0,0x55,0x55,0x55,0x55},   // System SW ECUSoftwareVersion
//                                     {0x03,0x22,0xF1,0x95,0x55,0x55,0x55,0x55},  	// System Supplier ECUSoftwareVersion
									   {0x03,0x22,0xF1,0x91,0x55,0x55,0x55,0x55},	// System HW ECUSHardwareVersion
									   {0x03,0x22,0xF1,0x94,0x55,0x55,0x55,0x55},   // System Supplier ECUSoftwareVersion
                                       {0x03,0x22,0xF1,0x92,0x55,0x55,0x55,0x55},
									   {0x03,0x22,0xF1,0x95,0x55,0x55,0x55,0x55}     // System Supplier ECUSoftwareVersionF195
									   };  // System Supplier ECUHardwareVersion
  
uint8_t SendMessage[15][WDLC] ={{0x03,0x22,0xF1,0x89,0x55,0x55,0x55,0x55},   // ReadDataByIdentifier_ECUSoftwareVersion
                                {0x30,0x00,0x14,0x55,0x55,0x55,0x55,0x55},   // ReadDataByIdentifier_Flow_Control
                                {0x02,0x10,0x90,0x55,0x55,0x55,0x55,0x55},   // DiagnosticSessionControl_Extended_Session
                                {0x02,0x85,0x82,0x55,0x55,0x55,0x55,0x55},   // ControlDTCSetting_OFF
                                {0x02,0x28,0x02,0x55,0x55,0x55,0x55,0x55},   // CommunicationControl_disableRxAndTx
                                {0x02,0x10,0x85,0x55,0x55,0x55,0x55,0x55},   // DiagnosticSessionControl_programmingSession
                                {0x02,0x27,0x01,0x55,0x55,0x55,0x55,0x55},   // SecurityAccess_Reques_Seed
                                {0x04,0x27,0x02,0x00,0x00,0x55,0x55,0x55},   // SecurityAccess_Send_KEY
                                {0x05,0x31,0x01,0xFF,0x00,0x00,0x55,0x55},   // RoutineControl_eraseMemory_APP
                                {0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00},   // Transfer_App_File
                                {0x04,0x31,0x01,0xFF,0x01,0x55,0x55,0x55},   // RoutineControl_checkprogrammingdependency
                                {0x02,0x11,0x81,0x55,0x55,0x55,0x55,0x55},   // ECUReset_ hardReset
                                {0x02,0x29,0x02,0x55,0x55,0x55,0x55,0x55},   // CommunicationControl_Enable_RxAndTx
                                {0x03,0x22,0xF1,0x89,0x55,0x55,0x55,0x55},   // ReadDataByIdentifier_ECUSoftwareVersion_L
                                {0x30,0x00,0x14,0x55,0x55,0x55,0x55,0x55}};  // ReadDataByIdentifier_L 


void CanFuncClass::InitCanSetup(void)
{
 // Configure CAN Setup Envirmomnet ...
    SelectP = 0;
    SelectN = 0;
    FC      = 0;
    Delay   = 0;
    CANtXrXStatus = 0;

    tx_frame.FIR.B.FF = CAN_frame_std;
    tx_frame.MsgID = SharedMemory.P1_ID;//0x71E;
    tx_frame.FIR.B.DLC = 8;

    dx_frame.FIR.B.FF = CAN_frame_std;
    dx_frame.MsgID = SharedMemory.F1_ID;//0x7DF;
    dx_frame.FIR.B.DLC = 8;
    
    rx_frame.FIR.B.FF = CAN_frame_std;
    rx_frame.MsgID = SharedMemory.ECU1_ID;//0x73E;
    rx_frame.FIR.B.DLC = 8;

    CAN_cfgs.speed=CAN_SPEED_500KBPS;
    CAN_cfgs.tx_pin_id = GPIO_NUM_5; // - ESP32 GPIO5 will act as CAN_Tx.
    CAN_cfgs.rx_pin_id = GPIO_NUM_4; // - ESP32 GPIO4 will act as CAN_Rx.
    CAN_cfgs.rx_queue = xQueueCreate(1000,sizeof(rx_frame));

#if 1
	// Set CAN Filter
	// See in the SJA1000 Datasheet chapter "6.4.15 Acceptance filter"
	// and the APPLICATION NOTE AN97076 chapter "4.1.2 Acceptance Filter" 
	// for PeliCAN Mode
#if 0	
	CAN_filter_t p_filter;
	p_filter.FM = Single_Mode;

	p_filter.ACR0 = 0x07;
	p_filter.ACR1 = 0x3E;
	p_filter.ACR2 = 0x00;
	p_filter.ACR3 = 0x00;

	p_filter.AMR0 = 0xFF;
	p_filter.AMR1 = 0xFF;
	p_filter.AMR2 = 0xFF;
	p_filter.AMR3 = 0xFF;
	ESP32Can.CANConfigFilter(&p_filter);
#else
	CAN_filter_t p_filter;
	p_filter.FM = Single_Mode;

	p_filter.ACR0 = SharedMemory.ECU1_ID/0xFF;
	p_filter.ACR1 = (uint8_t)(SharedMemory.ECU1_ID & 0xFF);
	p_filter.ACR2 = 0x00;
	p_filter.ACR3 = 0x00;

	p_filter.AMR0 = 0xFF;
	p_filter.AMR1 = 0xFF;
	p_filter.AMR2 = 0xFF;
	p_filter.AMR3 = 0xFF;
	ESP32Can.CANConfigFilter(&p_filter);
#endif
#endif
  //start CAN Module
    ESP32Can.CANInit();
  
	Serial.println("CAN Setup started");    

}

void CanFuncClass::Can_Stop(void)
{
 // Configure CAN Setup Envirmomnet ...
 //start CAN Module
    ESP32Can.CANStop();
 
	Serial.println("CAN Can_Stop Function started");    
}

void CanFuncClass::SendCanDataToECU_Pysical(uint8_t *SendToEcuCanData)
{
  uint8_t i,DataLow[WDLC];

  memcpy(DataLow ,SendToEcuCanData, WDLC);
  for(i= 0; i< 8; i++)
  {
    tx_frame.data.u8[i] = DataLow[i];
  }
  ESP32Can.CANWriteFrame(&tx_frame);
  SelectN = SelectP;
  SelectP = 0xFF;
}

void CanFuncClass::SendCanDataToECU_Functional(uint8_t *SendToEcuCanData)
{
  uint8_t i,DataLow[WDLC];

  memcpy(DataLow ,SendToEcuCanData, WDLC);
  for(i= 0; i< 8; i++)
  {
    dx_frame.data.u8[i] = DataLow[i];
  }
  ESP32Can.CANWriteFrame(&dx_frame);
}

void CanFuncClass::sendMessageByte(uint8_t *SendEcuCanDataFrame)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

//  CAN_iD = ( (SendEcuCanDataFrame[0] & 0xff) << 8 || ( SendEcuCanDataFrame[1] & 0xff) );
    CAN_iD = ( (SendEcuCanDataFrame[0] & 0xff) << 8 ); //+  ( SendEcuCanDataFrame[1] & 0xff) );
    CAN_iD = ( CAN_iD + SendEcuCanDataFrame[1] );
    
// Serial.printf("you have %d hours to come to me",time);
// Serial.printf("sendMessageByte------SendEcuCanDataFrame[0] : %x  SendEcuCanDataFrame[1] : %x \n",SendEcuCanDataFrame[0], SendEcuCanDataFrame[1]);
// Serial.printf("sendMessageByte------%x \n",CAN_iD);

  if(CAN_iD == SharedMemory.P1_ID) //SharedMemory.ECU1_ID
  {
      memcpy(SendCanData ,&SendEcuCanDataFrame[2], WDLC);
      SendCanDataToECU_Pysical(SendCanData);
  }
  else if(CAN_iD == SharedMemory.F1_ID) //SharedMemory.ECU1_ID
  {
      memcpy(SendCanData ,&SendEcuCanDataFrame[2], WDLC);
      SendCanDataToECU_Functional(SendCanData);
  }
  else
  {
	  memcpy(SendCanData ,&SendEcuCanDataFrame[2], WDLC);
	  SendCanDataToECU_Pysical(SendCanData);
  }
}

void CanFuncClass::sendMessageByteEcuInfoSwVersion(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;


  CalDLC = 0;
  DLC 	 = 0;

//SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier_ECUSoftwareVersion]);
  SendCanDataToECU_Pysical(SendMessageEcuInfo[ECUSoftwareVersion]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageEcuInfo[ECUSoftwareVersion][i]); Serial.print("\n");

}

void CanFuncClass::sendMessageByteEcuInfoHwVersion(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

   CalDLC = 0;
   DLC    = 0;

//SendCanDataToECU_Pysical(SendMessageEcuInfo[ECUHardwareVersion]);
  SendCanDataToECU_Pysical(SendMessageEcuInfo[ECUHardwareVersion]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageEcuInfo[ECUHardwareVersion][i]); Serial.print("\n");
  
}

void CanFuncClass::sendMessageByteEcuInfoSwVersionSup(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;


  CalDLC = 0;
  DLC 	 = 0;

//SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier_ECUSoftwareVersion]);
  SendCanDataToECU_Pysical(SendMessageEcuInfo[SUPECUSoftwareVersion]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageEcuInfo[SUPECUSoftwareVersion][i]); Serial.print("\n");

}

void CanFuncClass::sendMessageByteEcuInfoSwVersionF195(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;


  CalDLC = 0;
  DLC 	 = 0;

//SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier_ECUSoftwareVersion]);
  SendCanDataToECU_Pysical(SendMessageEcuInfo[ECUSoftwareVersionF195]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageEcuInfo[ECUSoftwareVersionF195][i]); Serial.print("\n");

}

void CanFuncClass::sendMessageByteEcuInfoHwVersionSup(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

//SendCanDataToECU_Pysical(SendMessageEcuInfo[ECUHardwareVersion]);
  SendCanDataToECU_Pysical(SendMessageEcuInfo[SUPECUHardwareVersion]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessageEcuInfo[SUPECUHardwareVersion][i]); Serial.print("\n");
  
}


void CanFuncClass::sendMessageByteDtcInfoPendingStatus(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanDataToECU_Pysical(SendMessagedtcInfo[DTCStatusPending]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessagedtcInfo[DTCStatusPending][i]); Serial.print("\n");
}

void CanFuncClass::sendMessageByteDtcInfoConfirmingStatus(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanDataToECU_Pysical(SendMessagedtcInfo[DTCStatusConfirming]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessagedtcInfo[DTCStatusConfirming][i]); Serial.print("\n");
}

void CanFuncClass::sendMessageByteDtcInfoAllStatus(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanDataToECU_Pysical(SendMessagedtcInfo[DTCStatusALL]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessagedtcInfo[DTCStatusALL][i]); Serial.print("\n");
}



void CanFuncClass::sendMessageByteDtcInfoPendingStatusCnt(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanDataToECU_Pysical(SendMessagedtcInfo[DTCStatusPendingCnt]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessagedtcInfo[DTCStatusPendingCnt][i]); Serial.print("\n");
}

void CanFuncClass::sendMessageByteDtcInfoConfirmingStatusCnt(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;

  SendCanDataToECU_Pysical(SendMessagedtcInfo[DTCStatusConfirmingCnt]);
  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessagedtcInfo[DTCStatusConfirmingCnt][i]); Serial.print("\n");
}

void CanFuncClass::sendMessageByteDtcInfoAllStatusCnt(void)
{
  uint8_t  SendCanData[WDLC];
  uint16_t CAN_iD ;

  CalDLC = 0;
  DLC    = 0;
  if(RFileFnc.DtcAllClearState){
	  SendCanDataToECU_Functional(SendMessagedtcInfo[DTCStatusALLCnt]);
	  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessagedtcInfo[DTCStatusALLCnt][i]); Serial.print("\n");
  }else{
	  SendCanDataToECU_Functional(SendMessagedtcInfo[DTCStatusALLCnt]); //2020.05.06 modify by yongwookwi
//	  SendCanDataToECU_Pysical(SendMessagedtcInfo[DTCStatusALLCnt]);
	  Serial.print("Send Data-[SendCanDataToECU] : 			  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",SendMessagedtcInfo[DTCStatusALLCnt][i]); Serial.print("\n");
  }
  
}

void CanFuncClass::ECUInfomsgDecoder(byte readmsg[])
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
			SendCanDataToECU_Pysical(tmpStr);
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
					SharedMemory.DtcPNum = 0xFF;
					SharedMemory.DtcHNum = 0xFF;
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

uint8_t CanFuncClass::ReceivedCanDataFromECU(void)
{
    uint8_t TX_RX_Check;
    //receive next CAN frame from queue
    if(xQueueReceive(CAN_cfgs.rx_queue,&rx_frame, 1*portTICK_PERIOD_MS)==pdTRUE)
    {
//    if(rx_frame.MsgID == 0x73E || rx_frame.MsgID == 0x7DF)  //SharedMemory.ECU1_ID;
	  if(rx_frame.MsgID == SharedMemory.ECU1_ID || rx_frame.MsgID == SharedMemory.F1_ID)  //SharedMemory.ECU1_ID;
      {
#if 0        
//      ReceivedCanDataParsing(SelectN);
		ReceivedCanDataCopy();
#else
		if(Recu_Fragment > 0)
		{
			switch(CanFnc.Recu_Fragment)
			{
			case CANDIAG_NONE:
				break;
			case CANDIAG_ECUINFO:
				memcpy(ReprogData.DiagDatabuf ,rx_frame.data.u8, WDLC);
				Serial.print("Received Data-[CANDIAG_ECUINFO] :				  ");  for(int i=0;i<WDLC;i++) Serial.printf("%02X ",ReprogData.DiagDatabuf[i]); Serial.print("\n");
				ECUInfomsgDecoder(ReprogData.DiagDatabuf);
				break;
			}

		}
		else
		{
	        ReceivedCanDataCopy();
		}
#endif
      }
      TX_RX_Check = TRUE;
    }
    else
    {
      TX_RX_Check = FALSE;
    }

    return TX_RX_Check;
}

void  CanFuncClass::ReceivedCanDataCopy(void)
{
	printf("0[%x] 1[%x] 2[%x] 3[%x] 4[%x] 5[%x] 6[%x] 7[%x]\n",rx_frame.data.u8[0],rx_frame.data.u8[1],rx_frame.data.u8[2],rx_frame.data.u8[3], rx_frame.data.u8[4],rx_frame.data.u8[5],rx_frame.data.u8[6],rx_frame.data.u8[7]);

    memcpy(ReprogData.reprogbuf ,rx_frame.data.u8, WDLC);
   
//	printf("0[%x] 1[%x] 2[%x] 3[%x] 4[%x] 5[%x] 6[%x] 7[%x]\n",ReprogData.reprogbuf[0],ReprogData.reprogbuf[1],ReprogData.reprogbuf[2],ReprogData.reprogbuf[3],	ReprogData.reprogbuf[4],ReprogData.reprogbuf[5],ReprogData.reprogbuf[6],ReprogData.reprogbuf[7]);

    if(WiFiFnc.WifiWorking)
  	{
		WiFiFnc.CANMsgSendtoWifi(ReprogData.reprogbuf);
   	}
}

void CanFuncClass::ReceivedCanDataParsing(uint8_t RetValueSel)
{
    switch(RetValueSel)
    {
      case ReadDataByIdentifier_ECUSoftwareVersion:
           if((rx_frame.data.u8[2]) == 0x62)
           {
             SelectP = ReadDataByIdentifier; //SelectN++;
             SelectN = 0xFF; 
           }
           printf(" ReadDataByIdentifier_ECUSoftwareVersion  SelectP %d\n",SelectP);           
           break;
      case ReadDataByIdentifier:
           if((rx_frame.data.u8[0]) == 0x21){
             FC++;
             printf(" ReadDataByIdentifier_21 SelectP %d\n",SelectP);           
           }
           else if((rx_frame.data.u8[0]) == 0x22  &&  FC == 0x01 ){
             SelectP = DiagnosticSessionControl_Extended_Session; //SelectN++;
             SelectN = 0xFF; 
             FC = 0x00;
             printf(" ReadDataByIdentifier_22  SelectP %d\n",SelectP);           
	       }
           else{
           }
           break;
      case DiagnosticSessionControl_Extended_Session:
           Delay = 100;
           SelectN = 0xFF; 
           break;
      case ControlDTCSetting_OFF:
           SelectP = SelectN++;
           SelectN = 0xFF; 
           break;
      case CommunicationControl_disableRxAndTx:
           SelectP = SelectN++;
           SelectN = 0xFF; 
           break;    
      case DiagnosticSessionControl_programmingSession:
           if((rx_frame.data.u8[1]) == 0x50)
           {
             SelectP = SecurityAccess_Reques_Seed; //SelectN++;
             SelectN = 0xFF; 
           }
           break;    
      case SecurityAccess_Reques_Seed:
           if((rx_frame.data.u8[1]) == 0x01  &&  (rx_frame.data.u8[2]) == 0x67 )
           {
             SelectP = SecurityAccess_Send_KEY;//SelectN++;
             SelectN = 0xFF; 
           }
           break;    
      case SecurityAccess_Send_KEY:
           SelectP = RoutineControl_eraseMemory_APP; //SelectN++;
           SelectN = 0xFF; 
           break;    
      case RoutineControl_eraseMemory_APP:
           if((rx_frame.data.u8[1]) == 0x71)
           {
             SelectP = SelectN++;
             SelectN = 0xFF; 
           }
           break;    
      case Transfer_App_File:
      
           break;    
      case RoutineControl_checkprogrammingdependency:
           if((rx_frame.data.u8[1]) == 0x71)
           {
             SelectP = SelectN++;
             SelectN = 0xFF; 
           }
           break;    
      case ECUReset_hardReset:
           SelectP = SelectN++;
           SelectN = 0xFF; 
           Delay = 500;
           break;    
      case CommunicationControl_Enable_RxAndTx:
           SelectP = SelectN++;
           SelectN = 0xFF; 
           Delay = 1000;
           break;    
      case ReadDataByIdentifier_ECUSoftwareVersion_L:
           if((rx_frame.data.u8[2]) == 0x62)
           {
             SelectP = SelectN++;
             SelectN = 0xFF; 
           }
           break;
      case ReadDataByIdentifier_L:
           if((rx_frame.data.u8[0]) == 0x21){
             FC++;
           }
           else if((rx_frame.data.u8[0]) == 0x22  &&  FC == 0x01 ){
             SelectP = SelectN++;
             SelectN = 0xFF; 
             FC = 0x00;
           }
           else{
           }
           break;
          
      default:
           break;
    }

}

void  CanFuncClass::SendWifiDataCAN(uint8_t  SendSelect)
{
    switch(SendSelect)
    {
      case ReadDataByIdentifier_ECUSoftwareVersion:
           SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier_ECUSoftwareVersion]);
           break;
      case ReadDataByIdentifier:
           SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier]);
           break;
      case DiagnosticSessionControl_Extended_Session:
           SendCanDataToECU_Functional(SendMessage[DiagnosticSessionControl_Extended_Session]);
           SelectP = ControlDTCSetting_OFF; //SelectP++;
           printf("[FUN] DiagnosticSessionControl_Extended_Session  SelectP %d\n",SelectP);           
           break;
      case ControlDTCSetting_OFF:
           SendCanDataToECU_Functional(SendMessage[ControlDTCSetting_OFF]);
           SelectP = CommunicationControl_disableRxAndTx; //SelectP++;
           printf("[FUN] ControlDTCSetting_OFF  SelectP %d\n",SelectP);           
           break;
      case CommunicationControl_disableRxAndTx:
           SendCanDataToECU_Functional(SendMessage[CommunicationControl_disableRxAndTx]);
           SelectP = DiagnosticSessionControl_programmingSession; //SelectP++;
           printf("[FUN] CommunicationControl_disableRxAndTx  SelectP %d\n",SelectP);           
           break;    
      case DiagnosticSessionControl_programmingSession:
           SendCanDataToECU_Pysical(SendMessage[DiagnosticSessionControl_programmingSession]);
           break;    
      case SecurityAccess_Reques_Seed:
           SendCanDataToECU_Pysical(SendMessage[SecurityAccess_Reques_Seed]);
           break;    
      case SecurityAccess_Send_KEY:
           SendCanDataToECU_Pysical(SendMessage[SecurityAccess_Send_KEY]);
           break;    
      case RoutineControl_eraseMemory_APP:
           SendCanDataToECU_Pysical(SendMessage[RoutineControl_eraseMemory_APP]);
           break;    
      case Transfer_App_File:
           SendCanDataToECU_Pysical(SendMessage[Transfer_App_File]);
           break;    
      case RoutineControl_checkprogrammingdependency:
           SendCanDataToECU_Pysical(SendMessage[RoutineControl_checkprogrammingdependency]);
           break;    
      case ECUReset_hardReset:
           SendCanDataToECU_Pysical(SendMessage[ECUReset_hardReset]);
           break;    
      case CommunicationControl_Enable_RxAndTx:
           SendCanDataToECU_Functional(SendMessage[CommunicationControl_Enable_RxAndTx]);
           break;    
      case ReadDataByIdentifier_ECUSoftwareVersion_L:
           SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier_ECUSoftwareVersion_L]);
           break;
      case ReadDataByIdentifier_L:
           SendCanDataToECU_Pysical(SendMessage[ReadDataByIdentifier_L]);
           break;
          
      default:
        break;
    }
  
}
