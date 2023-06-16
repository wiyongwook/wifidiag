#ifndef SharedMemory_h
#define SharedMemory_h

#include <ESP32CAN.h>
#include <CAN_config.h>

#include "SPIFFS.h"
#include <string.h> 

#define CANFD_ENABLE__
//#define DEBUG_PRINTF__
//#define TESTFILEREAD__
//#define CANFDSENDTEST__

//#define PhysicalID    0x71E
//#define FunctionalID  0x7DF
//#define ECUID         0x73E


class SharedMemoryClass 
{
public:
	File Fp; // file Point
	
public:
    byte DownloadStepFile;
    byte Address[4]; // = new byte[4];
    byte HexSize[4]; // = new byte[4];	
	
public:
    uint16_t ECU1_ID = 0;
	uint16_t P1_ID = 0;
	uint16_t F1_ID = 0;
	uint16_t CAN_TYPE1 = 0;

    uint16_t ECU2_ID = 0;
	uint16_t P2_ID = 0;
	uint16_t F2_ID = 0;
	uint16_t CAN_TYPE2 = 0;
	uint16_t REP_BLOCK_SIZE  = 0;
	uint16_t SecurityTpye    = 0;
	uint16_t FlowControlType = 0;

	uint16_t File_CAN_TYPE = 0;
	uint16_t STmin = 1;
	uint16_t WifiOnOffdState  = 0;
	uint16_t DtcAllClearState  = 0;

	uint16_t DtcPNum = 0xFF;
	uint16_t DtcHNum = 0xFF;

	uint16_t OTAMenuOption = 0;
	uint16_t OTAMStart = 0;
	uint16_t OTAWebConnection = 0;
	uint16_t OTAConnect = 0;

	uint16_t AsyncWebMStart = 0;

	String downloadfileName = "";
	String WifiBdSWVer = "1.20.00:20230324"; 
//	String csvfileName = "";  

	char ECU_ID[2][20] ; 
	char P_ID[2][20]   ; 
	char F_ID[2][20]   ;
	char CAN_TYPE[2][20]; 
	char ADDRESS[2][20]; 
	char B_SIZE[2][20]; 
	char S_TYPE[2][20]; 
	char FlowControl[2][20]; 
	

	char FileName[20]; 
	char FileCanType[20]; 
	long FileSize;
	unsigned long StartAddress;
	long AllFileSize[20];

	int DiagCANWebsocketTask = 100;	
};

extern SharedMemoryClass SharedMemory;

#endif // SharedMemory_h
