#ifndef Reprogramming_h
#define Reprogramming_h

#include "CanFunc.h" 
#include "SPIFFS.h"
#include "reprogDataType.h"
#include "RepFileFunc.h"

//#define PhysicalID    0x71E
//#define FunctionalID  0x7DF
//#define ECUID         0x73E

#define NOK			  0x00
#define OK			  0x01
#define TIMEOUT		  0x02
#define OKNOFLOW	  0x03
#define OVERTIME      0x01

#define BUFSFD     	    4093
#define BLOCKBUFSFD     4093
#define BLOCKBUFSDATAFD 4093
#define FDBUFFER		4095

#define TIMEHSMONITOR	5000  //10000
#define TIMEMAXCNTHS	10000 //10000
#define T3CNTN			100
#define TIMECNT			25000
#define TIMECNTEXT1		5
#define NORMALTIMECNT	500
#define DOUBLETIMECNT	1000
#define D2TIMECNT		1500
#define DDTIMECNT    	2000


#define HSFRAMELENGTH   8 //64
#define HSRFRAMELENGTH  7 //63
#define HSHALFRAMEDLC   8 //32
#define HSFLOWLENGTH    10
#define HSNORMALDLC     8
#define HSCANDATA		2


#define ESP_TesterPresent                                 20
#define ESP_WaitFunction                                  21
#define ESP_TesterPresent_2                               22
#define ESP_FlowControlFrame                              23


#define ESP_ReadDataByIdentifier_ECUSoftwareVersion       0
#define ESP_ReadDataByIdentifier                          1

#define ESP_DiagnosticSessionControl_Extended_Session     1

#define ESP_ControlDTCSetting_OFF                         2
#define ESP_CommunicationControl_disableRxAndTx           3
#define ESP_DiagnosticSessionControl_programmingSession   4

#define ESP_SecurityAccess_Request_Seed                   5
#define ESP_SecurityAccess_Send_KEY                       6
#define ESP_RoutineControl_eraseMemory_APP                7
#define ESP_Transfer_App_File                             8
#define ESP_Transfer_App_Reprogramming                    9
#define ESP_RequestTransferExit							  10	
#define ESP_RoutineControl_checkprogrammingdependency     11
#define ESP_ECUReset_HardReset                            12
#define ESP_DiagnosticSessionControl_Extended_Session2    13
#define ESP_CommunicationControl_Enable_RxAndTx           14
#define ESP_ControlDTCSetting_ON						  15
#define ESP_DiagnosticSessionControl_Standard			  16
#define ESP_ReadDataByIdentifier_ECUSoftwareVersion_Last  17


#define ESP_ReadDataByIdentifier_ECUSoftwareVersion_L     15
#define ESP_ReadDataByIdentifier_L                        16
#define ESP_Exit                                          17

class ReprogrammingClass 
{
public:
    void FlashDownload(void);
    int  WaitFunction(int counter);
    void ReprogRxBufClear(void);
    void InitReprogrammingClass(void);
	int  FlowControlFrameResp(int id, byte* data, int counter);
    int  singleFrameResp(int id, byte* data, int counter);  
    int  singleFirstFrameResp(int id, byte* data, int counter);  
    int  singleFrameNoResp(int id, byte* data, int counter);
    int  MultiFrameSend(int id, byte* data, int dlc);
//  int  ReprogMultiFrameSend(int id, byte* data, int dlc, int Seq);
    int  singleFramePendingResp(int id, byte* data, int counter);

	int  ProcessStepZero(void);
	int  ProcessStepTwo(void);
	int  ProcessStepOne(byte* Frame, byte* data, int dlc, int Seq);
	int  ReprogMultiFrameHsSend(int id, byte* data, int dlc,int Seq);
	long getFileData(File Srcfile, byte* fdata, int Startidx, int mode);
	int  singleFrameSecurityResp(int id, byte* data, int counter, int sequence);
	int  singleFramePendingRespLast(int id, byte* data, int counter);

	void ReprogramInitVariable(int mode);
    void vfClearAllEnv(void);
	void vfClearEnv(void);
	void DTC_Clear_Functional(void);
        
public:
    int   FDDLC =0;	
    int   DLC =0;
    int   respSta =1;
    int   respTimeCount =0;
    byte  tmpCpy[512];
    byte* TxBuffer;
	byte  ReadData[BUFSFD];
	byte  SendBuf[FDBUFFER];
  
	byte FlowFrame[HSFLOWLENGTH];
	byte FirstFrame[HSFRAMELENGTH+HSCANDATA];
	byte ConsecutiveFrame[HSFRAMELENGTH+HSCANDATA];

	int PhysicalID    = 0x0;
    int FunctionalID  = 0x0;
    int ECUID         = 0x0;

	int PhysicalID2   = 0x0;
    int FunctionalID2 = 0x0;
    int ECUID2        = 0x0;

	long filelength = 0; 
    File RepHSFile;
	uint8_t RepFileTestChk = 0x0;
		
	int FrmCnt = 0;
	int Resp   = 0;
	int LastloopCnt = 0;
	int CanFileTest = 0;
	int LastMsgCnt  = 0;
};

extern ReprogrammingClass ReprogmmingData;

#endif // Reprogramming_h
