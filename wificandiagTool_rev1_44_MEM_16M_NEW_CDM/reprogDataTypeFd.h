#ifndef reprogDataTypeFd_h
#define reprogDataTypeFd_h

//#include "ReprogrammingClass.h" 
#define BUFS     	  4093
#define BLOCKBUFS     4093
#define BLOCKBUFSDATA 4093

//#define PhysicalID    0x7BE

#define FRAMELENGTH   64
#define RFRAMELENGTH  63
#define HALFRAMEDLC   32
#define FLOWLENGTH    10
#define NORMALDLC     8

#define HSFRAMELENGTH   8 //64
#define HSRFRAMELENGTH  7 //63
#define HSHALFRAMEDLC   8 //32
#define HSFLOWLENGTH    10
#define HSNORMALDLC     8

#define FirstFrameLength	   60
#define ConsecutiveFrameLength 63

#define HSFirstFrameLength	   	 4
#define HSConsecutiveFrameLength 7


class ReprogDataTypeClassFd 
{
public:
    byte MultiFrame[11];
    byte SendFrame[10];
	
//  byte FirstFrame[10];
//  byte FlowFrame[10] ; 
   
    byte FlowFrame[FLOWLENGTH];
    byte FirstFrame[FRAMELENGTH];
    byte ConsecutiveFrame[FRAMELENGTH];
   
        
    int reprogFile =0;
    int reprogAPPtoFBL=0;
    int timerTick =0;
    int LastTick  =0;
    int PendingTick  =0;
    int reprogExpiredCheck =0;
    byte reprogbuf[8];// = new byte[8];
    byte reprogbufFDSize[16];// = new byte[8];
    byte DiagDatabuf[8];// = new byte[8];
    unsigned long  reprogFirstFlameSize=0;
    int  reprogHexSize=0;
	int  reprogFileHexSize=0;
    int  reprogHexLineCnt=0;
    byte Address[4]; // = new byte[4];
    byte HexSize[4]; // = new byte[4];
    int reprogStartIdx  =0;
    int reprogStartStop =0;
    int reprogEndIdx=0;
    int reprogSeq =0;
    int ProgressBarTotal =0;
    int ProgressBar =0;
    int ProcessStep =0;
    int stepFirst=0;
    int stepLock =0;
    int stepLock1=0;
    int stepLock2 =0;
    int stepIdx=0;
    int stepLength =0;
    int stepSendLock =0;
    int stepFrmIdx=0;
    boolean mFlagDownload = false;
    boolean mTimerStart   = false;
		
};

extern ReprogDataTypeClassFd ReprogDataFd;

#endif // reprogDataType_h
