#ifndef ReprogrammingFD_h
#define ReprogrammingFD_h

#include "CanFDFunc.h" 
#include "SPIFFS.h"
#include "reprogDataTypeFd.h"
#include "RepFileFunc.h"



#define CANDATA			2
#define NormalDLC       8
#define TransferAppDLC  16
#define CANFDDLC        64


#define BUFSFD     	    4093
#define BLOCKBUFSFD     4093
#define BLOCKBUFSDATAFD 4093
#define FDBUFFER		4095

#define TIMEMAXCNT		3500 //10000
#define T3CNTN			100
#define TIMECNT			5000
#define TIMECNTEXT1		5
#define NORMALTIMECNT	500
#define DOUBLETIMECNT	1000
#define D2TIMECNT		1500
#define DDTIMECNT    	2000

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

#define ESP_SecurityAccess_Request_Seed                    5
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

class ReprogrammingClassFD 
{
public:
    void FlashDownload(void);
    int  WaitFunction(int counter);
    void ReprogRxBufClear(void);
	void ReprogramInitVariable(int mode);
    void InitReprogrammingClass(void);
	long getFileData(File Srcfile, byte* fdata, int Startidx, int mode);
	int  FlowControlFrameResp(int id, byte* data, int counter);
    int  singleFrameResp(int id, byte* data, int counter);  
    int  singleFirstFrameResp(int id, byte* data, int counter);  
    int  singleFrameNoResp(int id, byte* data, int counter);
	int  DoubleFrameResp(int id, byte* data,int FrameSize, int counter);
    int  MultiFrameSend(int id, byte* data, int dlc);
    int  ReprogMultiFrameFdSend(int id, byte* data, int dlc, int Seq);
    int  singleFramePendingResp(int id, byte* data, int counter);
	int  singleFramePendingRespLast(int id, byte* data, int counter);
	int  singleFrameSecurityResp(int id, byte* data, int counter, int sequence);
    void vfClearAllEnv(void);
	void vfClearEnv(void);
	int  ProcessStepZero(void);
	int  ProcessStepTwo(void);
	int  ProcessStepOne(byte* Frame, byte* data, int dlc, int Seq);
	void DTC_Clear_Functional(void);
        
public:
    int   FDDLC =0;
    int   respSta =1;
    int   respTimeCount =0;
    byte  tmpCpy[512];
    byte* TxBuffer;
	byte  ReadData[BUFSFD];
	byte  SendBuf[FDBUFFER];

	byte FlowFrame[FLOWLENGTH];
	byte FirstFrame[FRAMELENGTH+CANDATA];
	byte ConsecutiveFrame[FRAMELENGTH+CANDATA];
	
	int PhysicalID    = 0x0;
    int FunctionalID  = 0x0;
    int ECUID         = 0x0;

	int PhysicalID2	  = 0x0;
	int FunctionalID2 = 0x0;
	int ECUID2		  = 0x0;

	long filelength = 0; 
		
    File RepFile;

    File RepFileTest;
	uint8_t RepFileTestChk = 0x0;

	int FrmCnt = 0;
	int Resp   = 0;
	int LastloopCnt = 0;
	int CanFileTest = 0;
	int LastMsgCnt  = 0;


};

extern ReprogrammingClassFD ReprogmmingDataFD;

#endif // ReprogrammingFD_h
