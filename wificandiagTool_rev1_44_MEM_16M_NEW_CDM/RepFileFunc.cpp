#include "RepFileFunc.h"
#include "Reprogramming.h"


#define BufLength     1024//512*2
#define iBufLength    1022//511*2

//---Define Reprogramming Class...
// ReprogrammingClass ReprogmmingData;
ReprogDataTypeClass ReprogData;
SharedMemoryClass   SharedMemory;


void RepFileFuncClass::InitRepFileSetup(uint8_t mode)
{
    long x, y, z, k;
    String loc,loc2;

	if(mode == 4)
	{
		Serial.println("# --mode 4 ....SharedMemory.HexSize [SharedMemory.Address] ---: ");

		SharedMemory.DownloadStepFile  = (byte)0x00;
#if 0		
		SharedMemory.Address[0] = (byte)0xA0;
		SharedMemory.Address[1] = (byte)0x01;
		SharedMemory.Address[2] = (byte)0x80;
		SharedMemory.Address[3] = (byte)0x00;   
#else
		SharedMemory.Address[0] = (byte)((SharedMemory.StartAddress & 0xFF000000)>>24);   	//StartAddress
		SharedMemory.Address[1] = (byte)((SharedMemory.StartAddress & 0xFF0000)>>16);   	//StartAddress
		SharedMemory.Address[2] = (byte)((SharedMemory.StartAddress & 0xFF00)>>8);   		//StartAddress
		SharedMemory.Address[3] = (byte)((SharedMemory.StartAddress & 0xFF)>>0);   			//StartAddress
#endif
		SharedMemory.HexSize[0] = (byte)((SharedMemory.FileSize & 0xFF000000)>>24);
		SharedMemory.HexSize[1] = (byte)((SharedMemory.FileSize & 0xFF0000)>>16);
		SharedMemory.HexSize[2] = (byte)((SharedMemory.FileSize & 0xFF00)>>8);
		SharedMemory.HexSize[3] = (byte)((SharedMemory.FileSize & 0xFF)>>0);

		listFileCheck(SPIFFS, "/", 0);

		return;
	}
	
	if(mode == 3)
	{
		Serial.println("# --mode 3 ....SharedMemory.HexSize [SharedMemory.Address] ---: ");

		SharedMemory.DownloadStepFile  = (byte)0x00;
#if 0		
		SharedMemory.Address[0] = (byte)0xA0;
		SharedMemory.Address[1] = (byte)0x01;
		SharedMemory.Address[2] = (byte)0x80;
		SharedMemory.Address[3] = (byte)0x00;   
#else
		SharedMemory.Address[0] = (byte)((SharedMemory.StartAddress & 0xFF000000)>>24);   	//StartAddress
		SharedMemory.Address[1] = (byte)((SharedMemory.StartAddress & 0xFF0000)>>16);   	//StartAddress
		SharedMemory.Address[2] = (byte)((SharedMemory.StartAddress & 0xFF00)>>8);   		//StartAddress
		SharedMemory.Address[3] = (byte)((SharedMemory.StartAddress & 0xFF)>>0);   			//StartAddress
#endif

		SharedMemory.HexSize[0] = (byte)((SharedMemory.FileSize & 0xFF000000)>>24);
		SharedMemory.HexSize[1] = (byte)((SharedMemory.FileSize & 0xFF0000)>>16);
		SharedMemory.HexSize[2] = (byte)((SharedMemory.FileSize & 0xFF00)>>8);
		SharedMemory.HexSize[3] = (byte)((SharedMemory.FileSize & 0xFF)>>0);

		return;
	}

	if(mode == 0) 
	{
		// initialize EEPROM with predefined size
		EEPROM.begin(EEPROM_SIZE);

		// read the last LED state from flash memory
		WifiSettingdState   = EEPROM.read(0);  // Radio

		HsCanSettingdState  = EEPROM.read(1);  // CheckBox
		CanFDSettingdState  = EEPROM.read(2);
		WifiOnOffdState     = EEPROM.read(3); 
		STminTime  			= EEPROM.read(4);  // Touch X, Y ....
		DtcAllClearState    = EEPROM.read(5);  // Touch X, Y ....
		HSCANBTNState	    = EEPROM.read(5);  // Touch X, Y ....

		if(WifiSettingdState  > 1) WifiSettingdState  = 0; 
		if(HsCanSettingdState > 1) HsCanSettingdState = 0;
		if(CanFDSettingdState > 1) CanFDSettingdState = 1;
		if(WifiOnOffdState > 1)    WifiOnOffdState = 1;
		if(STminTime >= 10 || STminTime <= 0)    STminTime = 1;
		if(DtcAllClearState >= 1 )    DtcAllClearState = 1;
		if(HSCANBTNState >= 1 )       HSCANBTNState = 1;

		WiFiFnc.WifiWorking = WifiOnOffdState;
		SharedMemory.WifiOnOffdState  = WifiOnOffdState;
		SharedMemory.DtcAllClearState = DtcAllClearState;

		Serial.printf("[Wifi]%d [HsCan]%d [CanFD]%d [OnOff]%d [STminTime]%d [DtcAllClearState]%d\n",
		WifiSettingdState,HsCanSettingdState,CanFDSettingdState,WifiOnOffdState,STminTime,DtcAllClearState);

	}

    //SPIFFS Start..................
	if (!SPIFFS.begin(true)) {
	  Serial.println("# ERROR: can not mount SPIFFS");
	}

	listDir(SPIFFS, "/", 0);

#if 0
	CanFile = SPIFFS.open(DownLoadFileName[0]);
//	CanFile = SPIFFS.open(DownLoadMainFileName);
#else
	CanFile = SPIFFS.open(DownLoadFileName[TftGuiLCDFun.SelectedID]);
#endif
	if (!CanFile) 		// can not open file
	{                                          
	  Serial.println("# [Init]----ERROR: can not open file");
	  mFileDownload = false;
	  delay(1000);
	}

	if(mFileDownload)
	{
	   if(mode)
	   {
		switch(SharedMemory.File_CAN_TYPE)
		{
		case 1:  // HS CAN
			Serial.printf("# --Reading Data from Open file [HSCAN] ---: %s \n",DownLoadFileName[TftGuiLCDFun.SelectedID]);
			ReadFile(CanFile,constOPCodeHS,DownLoadFileName[TftGuiLCDFun.SelectedID]);	//DownLoadMainFileName
			break;
		case 2:  // CAN FD
			Serial.printf("# --Reading Data from Open file [CAN FD] ---: %s \n",DownLoadFileName[TftGuiLCDFun.SelectedID]);
			ReadFileFD(CanFile,constOPCode,DownLoadFileName[TftGuiLCDFun.SelectedID]);	//DownLoadMainFileName
			break;
				
		}

	   }
	}
	CloseFile();

#if 0
	CSVFile = SPIFFS.open(CSVFileName);
	if (!CSVFile) 		// can not open file
	{                                          
	  Serial.println("# ----ERROR: can not open CSVfile");
	  delay(1000);
	}
	else
	{
 	  Serial.println("# CSVfile Open Successfully # :" + CSVFileName);
	}
	CSVFileContent = CSVFile.readStringUntil('\n');  
	Serial.println("[CSVFileContent]" + CSVFileContent);
    readValsP(12,&x, &y, &z, &k, &loc, &loc2 );
	//First 4 Long datatype variables 
	Serial.println(x);
	Serial.println(y);
	Serial.println(z);
	Serial.println(k);
	//Last 2 String type variables
	Serial.println(loc);
	Serial.println(loc2);
	CSVFile.close();
#endif
}


bool RepFileFuncClass::readValsP(int lineNum,long* v1, long* v2, long* v3, long* v4, String* loc,String* loc2) 
{
	  char line[200], *ptr, *str ;
	  
	  for(int i=0; i < lineNum; i++){	  
	  	CSVFileContent = CSVFile.readStringUntil('\n');  
		Serial.println(CSVFileContent);
  	  }
	  
	  if (!readLine(CSVFile, line, sizeof(line))) {
	    return false;  // EOF or too long
	  }
	  
	  *v1 = strtol(line, &ptr, 10);
	  if (ptr == line) return false;  // bad number if equal
	  while (*ptr) {
	    if (*ptr++ == ',') break;
	  }
	  *v2 = strtol(ptr, &str, 10);
	  while (*ptr) {
	    if (*ptr++ == ',') break;
	  }
	  *v3 = strtol(ptr, &str, 10);
	  while (*ptr) {
	    if (*ptr++ == ',') break;
	  }
	  *v4 = strtol(ptr, &str, 10);
	  while (*ptr) {
	    if (*ptr++ == ',') break;
	  }
	  String a = strtok_r(ptr, ",", &str);
	  String first(str);
	  *loc = first;
	  String let(a);
	  *loc2 = let;
	  
	  return str != ptr;  // true if number found
}

bool RepFileFuncClass::readLine(File &f, char* line, size_t maxLen) 
{
  for (size_t n = 0; n < maxLen; n++) 
  {
    int c = f.read();
    if ( c < 0 && n == 0) return false;  // EOF
    if (c < 0 || c == '\n') {
      line[n] = 0;
      return true;
    }
    line[n] = c;
  }
  return false; // line too long
}

void RepFileFuncClass::OpenFile(String Str)
{

  //SPIFFS Start..................
	if (!SPIFFS.begin(true)) {
	  Serial.println("# ERROR: can not mount SPIFFS");
	}

	CanFile = SPIFFS.open(Str);
	if (!CanFile) {                                          // can not open file
	  Serial.println("# ----ERROR: can not open file");
	  delay(1000);
	}
 
}

File RepFileFuncClass::OpenFileFD(String Str)
{
File DownFile;
	
  //SPIFFS Start..................
	if (!SPIFFS.begin(true)) {
	  Serial.println("# ERROR: can not mount SPIFFS");
	}

	DownFile = SPIFFS.open(Str);
	if (!DownFile) {                                          // can not open file
	  Serial.println("# ----ERROR: can not open file");
	  delay(1000);
	}
 return DownFile;
}

void RepFileFuncClass::CloseFile(void)
{
  CanFile.close();
}

void RepFileFuncClass::ReadFileFD (File CFile, int requestCode, String FilePath ) 
{
	SharedMemory.Fp =  CFile;

    SharedMemory.HexSize[0] = (byte)((SharedMemory.FileSize & 0xFF000000)>>24);
	SharedMemory.HexSize[1] = (byte)((SharedMemory.FileSize & 0xFF0000)>>16);
	SharedMemory.HexSize[2] = (byte)((SharedMemory.FileSize & 0xFF00)>>8);
	SharedMemory.HexSize[3] = (byte)((SharedMemory.FileSize & 0xFF)>>0);

	switch(requestCode)
	{
		case 7://constOPCode
			  	SharedMemory.DownloadStepFile 		= (byte)0x00;
#if 0				
            	SharedMemory.Address[0] = (byte)0x0A;
            	SharedMemory.Address[1] = (byte)0x01;
            	SharedMemory.Address[2] = (byte)0x80;
            	SharedMemory.Address[3] = (byte)0x00;
#else
				SharedMemory.Address[0] = (byte)((SharedMemory.StartAddress & 0xFF000000)>>24); 	//StartAddress
				SharedMemory.Address[1] = (byte)((SharedMemory.StartAddress & 0xFF0000)>>16);		//StartAddress
				SharedMemory.Address[2] = (byte)((SharedMemory.StartAddress & 0xFF00)>>8);			//StartAddress
				SharedMemory.Address[3] = (byte)((SharedMemory.StartAddress & 0xFF)>>0);			//StartAddress
#endif

				break;
		case 8://constCAL1Code
				SharedMemory.DownloadStepFile		= (byte)0x01;
				SharedMemory.Address[0] = (byte)0x00;
				SharedMemory.Address[1] = (byte)0x03;
				SharedMemory.Address[2] = (byte)0x00;
				SharedMemory.Address[3] = (byte)0x00;			
				break;
		case 9://constCAL2Code
				SharedMemory.DownloadStepFile		= (byte)0x02;
				SharedMemory.Address[0] = (byte)0x00;
				SharedMemory.Address[1] = (byte)0x03;
				SharedMemory.Address[2] = (byte)0x00;
				SharedMemory.Address[3] = (byte)0x00;			
				break;
		case 10://constEepCode
				SharedMemory.DownloadStepFile		= (byte)0x03;
				SharedMemory.Address[0] = (byte)0x00;
				SharedMemory.Address[1] = (byte)0x03;
				SharedMemory.Address[2] = (byte)0x00;
				SharedMemory.Address[3] = (byte)0x00;			
				break;
		
	}
	
//	CFile.fseek( CFile,	5, SEEK_SET);	   
//	CFile.seek();
	
}


void RepFileFuncClass::ReadFile (File CFile, int requestCode, String FilePath ) 
{
		SharedMemory.Fp =  CFile;
	
		SharedMemory.HexSize[0] = (byte)((SharedMemory.FileSize & 0xFF000000)>>24);
		SharedMemory.HexSize[1] = (byte)((SharedMemory.FileSize & 0xFF0000)>>16);
		SharedMemory.HexSize[2] = (byte)((SharedMemory.FileSize & 0xFF00)>>8);
		SharedMemory.HexSize[3] = (byte)((SharedMemory.FileSize & 0xFF)>>0);
    
        if(constOPCodeHS == requestCode)
        {
            DownloadStepFile 	  = (byte)0x00;
#if 0			
            ReprogData.Address[0] = (byte)0x0A;
            ReprogData.Address[1] = (byte)0x01;
            ReprogData.Address[2] = (byte)0x80;
            ReprogData.Address[3] = (byte)0x00;

		  	SharedMemory.DownloadStepFile = (byte)0x00;
        	SharedMemory.Address[0] = (byte)0x0A;
        	SharedMemory.Address[1] = (byte)0x01;
        	SharedMemory.Address[2] = (byte)0x80;
        	SharedMemory.Address[3] = (byte)0x00;					
#else
			ReprogData.Address[0] = (byte)((SharedMemory.StartAddress & 0xFF000000)>>24); 	//StartAddress
			ReprogData.Address[1] = (byte)((SharedMemory.StartAddress & 0xFF0000)>>16);		//StartAddress
			ReprogData.Address[2] = (byte)((SharedMemory.StartAddress & 0xFF00)>>8);			//StartAddress
			ReprogData.Address[3] = (byte)((SharedMemory.StartAddress & 0xFF)>>0);			//StartAddress

			SharedMemory.Address[0] = (byte)((SharedMemory.StartAddress & 0xFF000000)>>24);   	//StartAddress
			SharedMemory.Address[1] = (byte)((SharedMemory.StartAddress & 0xFF0000)>>16);   	//StartAddress
			SharedMemory.Address[2] = (byte)((SharedMemory.StartAddress & 0xFF00)>>8);   		//StartAddress
			SharedMemory.Address[3] = (byte)((SharedMemory.StartAddress & 0xFF)>>0);   			//StartAddress
#endif

        }
        else if(constCAL1Code == requestCode)
        {
            DownloadStepFile 	  = (byte)0x01;
            ReprogData.Address[0] = (byte)0x00;
            ReprogData.Address[1] = (byte)0x03;
            ReprogData.Address[2] = (byte)0x00;
            ReprogData.Address[3] = (byte)0x00;

        }
        else if(constCAL2Code == requestCode)
        {
            DownloadStepFile      = (byte)0x02;
            ReprogData.Address[0] = (byte)0x00;
            ReprogData.Address[1] = (byte)0x03;
            ReprogData.Address[2] = (byte)0xC0;
            ReprogData.Address[3] = (byte)0x00;
        }
        else if(constEepCode == requestCode){
            DownloadStepFile      = (byte)0x03;
            ReprogData.Address[0] = (byte)0x00;
            ReprogData.Address[1] = (byte)0x10;
            ReprogData.Address[2] = (byte)0x00;
            ReprogData.Address[3] = (byte)0x00;

        }

//	    FileNameList = txtFile.name(FilePath);
//      txtFile = new File(FilePath);
}


void RepFileFuncClass::listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
	int i=0;

	for(int j=0;j<10;j++) DownLoadFileName[j] = "";
	
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

	// Delete files
	// SPIFFS.remove("/TouchCalData3");

    File file = root.openNextFile();
    while(file)
	{
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
			SharedMemory.AllFileSize[i] = file.size();
//          Serial.println(file.size());
            Serial.println(file.size(),HEX);
#if 1			
			if(!strcmp(file.name(),"/SetUp.ini"))
			{
				SetUpEnvLoad(file);
			}

			if(isFnHex((char *)file.name()))
			{
				DownLoadFileName[i] = file.name();
				i++;
				if(i>=10) break;
			}
			else
			{
				for(int j=i;j<10;j++) DownLoadFileName[j] = "";
				DownLoadFileName[0] = SharedMemory.downloadfileName;
				Serial.printf("Not Found File in the List [num %d]\n",i);
			}
#endif			
			
        }
        file = root.openNextFile();
    }

	root.close();
	file.close();
}


void RepFileFuncClass::listFileCheck(fs::FS &fs, const char * dirname, uint8_t levels)
{
	int i=0;

    File root = fs.open(dirname);

    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

	// Delete files
	// SPIFFS.remove("/TouchCalData3");

    File file = root.openNextFile();

	if(!strcmp(file.name(),"/SetUp.ini"))
	{
		SetUpEnvLoad(file);
	}

	root.close();
	file.close();
}

void RepFileFuncClass::SetUpEnvLoad(File sfile)
{
	char ch[100], *ptr, *str ;
	long  v1;
	unsigned long v2;
	

	DownLoadMainFileName = sfile.readStringUntil('\n');	
	if(TftGuiLCDFun.SelectedID == 0)
	{
		Serial.println("[SetUp.ini file] :" + DownLoadMainFileName);
		SharedMemory.downloadfileName = DownLoadMainFileName;
		strcpy(SharedMemory.FileName,DownLoadMainFileName.c_str());
		SharedMemory.FileSize = SharedMemory.AllFileSize[0];
	}else{
		DownLoadMainFileName = SharedMemory.downloadfileName ;
		Serial.println("[SetUp.ini file n1 ] :" + DownLoadMainFileName);
		strcpy(SharedMemory.FileName,DownLoadMainFileName.c_str());
		SharedMemory.FileSize = SharedMemory.AllFileSize[TftGuiLCDFun.SelectedID];
		Serial.printf("[SharedMemory.FileSize %X] :\n",SharedMemory.FileSize);
	}
	

	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		ReprogData.reprogHexSize = SharedMemory.FileSize;
		ReprogData.reprogFileHexSize = SharedMemory.FileSize;
		break;
	case 2:// CAN FD
		ReprogDataFd.reprogHexSize = SharedMemory.FileSize;
		ReprogDataFd.reprogFileHexSize = SharedMemory.FileSize;
		break;
	}

//	SharedMemory.FileSize = sfile.size();
#if 0
	SharedMemory.HexSize[0] = (byte)((SharedMemory.FileSize & 0xFF000000)>>24);
	SharedMemory.HexSize[1] = (byte)((SharedMemory.FileSize & 0xFF0000)>>16);
	SharedMemory.HexSize[2] = (byte)((SharedMemory.FileSize & 0xFF00)>>8);
	SharedMemory.HexSize[3] = (byte)((SharedMemory.FileSize & 0xFF)>>0);
	Serial.printf("[SetUpEnvLoad] [HexSize[4]] %x %x %x %x\n",SharedMemory.HexSize[0],SharedMemory.HexSize[1],SharedMemory.HexSize[2],SharedMemory.HexSize[3]);
#endif

	F_Type = sfile.readStringUntil('\n');	
	Serial.println("[file Can Type] :" + F_Type);
	strcpy(ch,F_Type.c_str());
	memcpy(SharedMemory.FileCanType,ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
    if(v1 == 0x1)		memcpy(SharedMemory.FileCanType,"HSCAN",STR_SIZE);
	else if(v1 == 0x2)	memcpy(SharedMemory.FileCanType,"CANFD",STR_SIZE);
#if 0
	Serial.printf("[file Can Type] :%x \n",v1);
	SharedMemory.File_CAN_TYPE = (uint16_t) v1;
#else
	SharedMemory.File_CAN_TYPE = RFileFnc.CanFDSettingdState + 1;
	Serial.printf("[[20191118-SetUpEnvLoad]------------------SharedMemory.File_CAN_TYPE ] : %d \n", SharedMemory.File_CAN_TYPE );
#endif
	
//----Channel 1------------------------------------------------------------/
	S_ID[0] = sfile.readStringUntil('\n');	
	Serial.println("[ECU ID] :" + S_ID[0]);
	strcpy(ch,S_ID[0].c_str());
	memcpy(SharedMemory.ECU_ID[0],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[ECU1_ID] :%x \n",v1);
	SharedMemory.ECU1_ID = (uint16_t) v1;

	R_ID[0] = sfile.readStringUntil('\n');	
	Serial.println("[P1 ID] :" + R_ID[0]);
	strcpy(ch,R_ID[0].c_str());
	memcpy(SharedMemory.P_ID[0],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[P1_ID] :%x \n",v1);
	SharedMemory.P1_ID = (uint16_t) v1;

	F_ID[0] = sfile.readStringUntil('\n');	
	Serial.println("[F1 ID] :" + F_ID[0]);
	strcpy(ch,F_ID[0].c_str());
	memcpy(SharedMemory.F_ID[0],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[F1_ID] :%x \n",v1);
	SharedMemory.F1_ID = (uint16_t) v1;

	CAN_TYPE[0] = sfile.readStringUntil('\n');	
	Serial.println("[CAN_TYPE1 - HSCAN[1] : CANFD[2] :" + CAN_TYPE[0]);
	strcpy(ch,CAN_TYPE[0].c_str());
//	memcpy(SharedMemory.CAN_TYPE[0],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
    if(v1 == 0x1)		memcpy(SharedMemory.CAN_TYPE[0],"HSCAN",STR_SIZE);
	else if(v1 == 0x2)	memcpy(SharedMemory.CAN_TYPE[0],"CANFD",STR_SIZE);
	
	Serial.printf("[CAN_TYPE1] %x \n",v1);
	SharedMemory.CAN_TYPE1 = (uint16_t) v1;
	
//----Channel 2------------------------------------------------------------/
	S_ID[1] = sfile.readStringUntil('\n');	
	Serial.println("[ECU2 ID] :" + S_ID[1]);
	strcpy(ch,S_ID[1].c_str());
	memcpy(SharedMemory.ECU_ID[1],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[ECU2_ID] :%x \n",v1);
	SharedMemory.ECU2_ID = (uint16_t) v1;

	R_ID[1] = sfile.readStringUntil('\n');	
	Serial.println("[P2 ID] :" + R_ID[1]);
	strcpy(ch,R_ID[1].c_str());
	memcpy(SharedMemory.P_ID[1],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[P2_ID] :%x \n",v1);
	SharedMemory.P2_ID = (uint16_t) v1;

	F_ID[1] = sfile.readStringUntil('\n');	
	Serial.println("[F2 ID] :" + F_ID[1]);
	strcpy(ch,F_ID[1].c_str());
	memcpy(SharedMemory.F_ID[1],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[F2_ID] :%x \n",v1);
	SharedMemory.F2_ID = (uint16_t) v1;

	CAN_TYPE[1] = sfile.readStringUntil('\n');	
	Serial.println("[CAN_TYPE2 - HSCAN[1] : CANFD[2] :" + CAN_TYPE[1]);
	strcpy(ch,CAN_TYPE[1].c_str());
//	memcpy(SharedMemory.CAN_TYPE[1],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
    if(v1 == 0x1)		memcpy(SharedMemory.CAN_TYPE[1],"HSCAN",STR_SIZE);
	else if(v1 == 0x2)	memcpy(SharedMemory.CAN_TYPE[1],"CANFD",STR_SIZE);

	Serial.printf("[CAN_TYPE2] %x \n",v1);
	SharedMemory.CAN_TYPE2 = (uint16_t) v1;

	//----Start Address--------------------------------------------------------/
	ADDRESS_START[0] = sfile.readStringUntil('\n');	
	Serial.println("[ADDRESS_START] :" + ADDRESS_START[0]);
	strcpy(ch,ADDRESS_START[0].c_str());
	memcpy(SharedMemory.ADDRESS[0],ch,ADDRESS_SIZE);
	v2 = strtoul(ch, &ptr, 16);
	Serial.printf("[ADDRESS] :%x \n",v2);
	SharedMemory.StartAddress = v2;

	//----Block Size ----------------------------------------------------------/
	BLOCK_SIZE[0] = sfile.readStringUntil('\n'); 
	Serial.println("[BLOCK_SIZE] :" + BLOCK_SIZE[0]);
	strcpy(ch,BLOCK_SIZE[0].c_str());
	memcpy(SharedMemory.B_SIZE[0],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[B_SIZE] :%x \n",v1);
	SharedMemory.REP_BLOCK_SIZE = (uint16_t)v1;
	
	//----Security Type ----------------------------------------------------------/
	Security_TYPE[0] = sfile.readStringUntil('\n'); 
	Serial.println("[Security_TYPE] :" + Security_TYPE[0]);
	strcpy(ch,Security_TYPE[0].c_str());
	memcpy(SharedMemory.S_TYPE[0],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[S_TYPE] :%x \n",v1);
	SharedMemory.SecurityTpye = (uint16_t)v1;

	//----FlowControl On/Off-------------------------------------------------------/
	FlowControlOnOff = sfile.readStringUntil('\n'); 
	Serial.println("[FlowControlOnOff] :" + FlowControlOnOff);
	strcpy(ch,FlowControlOnOff.c_str());
	memcpy(SharedMemory.FlowControl[0],ch,STR_SIZE);
	v1 = strtol(ch, &ptr, 16);
	Serial.printf("[FlowControl] :%x \n",v1);
	SharedMemory.FlowControlType = (uint16_t)v1;

	//----Channel 2------------------------------------------------------------/

	SharedMemory.STmin = STminTime;


	
}

bool RepFileFuncClass::FileRemove(char* filename) {

  int8_t len = strlen(filename);
  bool result;
  SPIFFS.remove(filename);
  return result;
}

bool RepFileFuncClass::isFnHex(char* filename) 
{
  int8_t len = strlen(filename);
  bool result;
  if (  strstr(strlwr(filename + (len - 4)), ".s19")
	 || strstr(strlwr(filename + (len - 4)), ".bin")
    // and anything else you want
    ) {
    result = true;
  } else {
    result = false;
  }
  return result;
}
