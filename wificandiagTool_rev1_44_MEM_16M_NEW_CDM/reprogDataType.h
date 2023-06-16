#ifndef reprogDataType_h
#define reprogDataType_h

//#include "ReprogrammingClass.h" 

class ReprogDataTypeClass 
{
public:
    byte MultiFrame[11];
    byte SendFrame[10];
    byte FirstFrame[10];
    byte FlowFrame[10] ; 
   
    byte ConsecutiveFrame[10];
        
    int reprogFile =0;
    int reprogAPPtoFBL=0;
    int timerTick =0;
    int LastTick  =0;
    int PendingTick  =0;
    int reprogExpiredCheck =0;
    byte reprogbuf[8];// = new byte[8];
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

extern ReprogDataTypeClass ReprogData;

#endif // reprogDataType_h
