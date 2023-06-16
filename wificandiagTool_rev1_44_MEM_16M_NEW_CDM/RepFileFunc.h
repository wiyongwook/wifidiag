#ifndef RepFileFunc_h
#define RepFileFunc_h


#define S19_24BIT_FILE_START 10
#define S19_24BIT_FILE_END   74
#define S19_24BIT_FILE_SIZE  77

#define S19_32BIT_FILE_START 12
#define S19_32BIT_FILE_END   76
#define S19_32BIT_FILE_SIZE  79

// define the number of bytes you want to access
#define EEPROM_SIZE 32
#define STR_SIZE  		6//20
#define ADDRESS_SIZE    12//20

//#define FILENAME "CSV.csv"


#include "SPIFFS.h"
#include <string.h> 
#include <EEPROM.h>
//#include <CSVFile.h>
#include "WifiFunc.h"
#include "SharedMemory.h"


class RepFileFuncClass 
{
public:
    void InitRepFileSetup(uint8_t mode);
    void OpenFile(String Str);
    void CloseFile(void);
    void ReadFile (File CFile,int requestCode, String FilePath);
	void ReadFileFD (File CFile, int requestCode, String FilePath );
    void vfByteSorting(File CFile);  
    byte* hexStringToByteArray(void);
	byte* HexStringToByte(String abc, int length);
	byte* StringToStringArray(void) ;
	byte* StringToStringArray(int fCount) ;
	byte* StringToStringArray(int fCount,int DataCount,int LastCnt);
	char  ConvertCharToHex(char ch);	

	void  listDir(fs::FS &fs, const char * dirname, uint8_t levels);
	void  listFileCheck(fs::FS &fs, const char * dirname, uint8_t levels);
	
	bool  isFnHex(char* filename);
	bool  FileRemove(char* filename);
	bool  readValsP(int lineNum,long* v1, long* v2, long* v3, long* v4, String* loc,String* loc2); 
	bool  readLine(File &f, char* line, size_t maxLen); 
	void  SetUpEnvLoad(File sfile);
	File  OpenFileFD(String Str);

public:
//	CSVFile csv;
//	TxBuffer *TxBuf = new TxBuffer;

	byte* HexArray=new byte[512];
	byte* TempData = new byte[512*2];
	byte* tmp2Data = new byte[512];


	boolean mFileDownload = true;

	byte* data ;
	
    char buf[512];  
	int DownloadStepFile = 0;

	char WifiSettingdState 	 = 0;
	char CanFDSettingdState  = 0;
	char HsCanSettingdState  = 0;
	char WifiOnOffdState     = 0;
	char DtcAllClearState    = 0;
	char HSCANBTNState       = 0;
	char STminTime		     = 0;

	String UpdateBuffer = "";
//  String UpdateBufferQ[500]  ;
    String UpdateBufferQ  ;
	String UpdateBufferR  ;

//	String UpdateBufferN[5000] ;
	
    String OpPath;
    String Cal1Path;
    String Cal2Path;
    String EepPath;

	const int constOPCodeHS	= 0;
	const int constOPCode	= 7;
	const int constCAL1Code = 8;
	const int constCAL2Code = 9;
    const int constEepCode  =10;

    File txtFile;
    File CanFile;
    File CSVFile;

//	String DownLoadFileName = "/SW_1.03.01.s19";
	String DownLoadFileName[10];// = "";
	String DownLoadMainFileName;// = "";
	String CSVFileName;// = "";
	String CSVFileContent;// = "";
	String FileNameList;

	String F_Type;// = "";

	String R_ID[2];// = "";
	String S_ID[2];// = "";
	String F_ID[2];
	String CAN_TYPE[2];  //1 : HSCAN , 2 : CANFD

	String ADDRESS_START[2];  //Address
	String BLOCK_SIZE[2];  //Address
	String Security_TYPE[2];  //Address
	String FlowControlOnOff;  //Address
	
};

extern RepFileFuncClass RFileFnc;

#endif // RepFileFunc_h
