#ifndef TftGuiLCDFun_h
#define TftGuiLCDFun_h

#include "FS.h"
#include "RepFileFunc.h"
#include "Reprogramming.h"
#include "ReprogrammingFD.h"

#include <SPI.h>
//#include <TFT_eSPI.h>      // Hardware-specific library

// Additional functions
//#include "GfxUi.h"         // Attached to this sketch

#include "GUIslice.h"
#include "GUIslice_ex.h"
#include "GUIslice_drv.h"

// Include any extended elements
#include "elem/XCheckbox.h"
#include "elem/XSlider.h"
#include "elem/XTextbox.h"
#include "elem/XListbox.h"


//#include <Adafruit_GFX.h>

// Note that font files are located within the Adafruit-GFX library folder:
//<Fonts !Start!>
//#include "Fonts/FreeSerif9pt7b.h"
//<Fonts !End!>

#define ADDRADIO_2  1

#if 0
// ------------------------------------------------
// Enumerations for pages, elements, fonts, images
// ------------------------------------------------
//<Enum !Start!>
enum {E_PG_MAIN,E_PG2,E_PG_INTRO,E_PG_DIAG
      ,E_PG_FILE,E_PG_CSV};
enum {E_BOX10,E_BOX11,E_BOX12,E_BOX2
      ,E_BOX3,E_BOX4,E_BOX5,E_BOX6
      ,E_BOX8,E_BOX9,E_BTN1,E_BTN10
      ,E_BTN11,E_BTN12,E_BTN13,E_BTN14
      ,E_BTN15,E_BTN16,E_BTN17,E_BTN18
      ,E_BTN19,E_BTN2,E_BTN20,E_BTN21
      ,E_BTN22,E_BTN23,E_BTN24,E_BTN25
      ,E_BTN3,E_BTN4,E_BTN5,E_BTN6
      ,E_BTN7,E_BTN8,E_BTN9,E_CHECK1
      ,E_CHECK2,E_CHECK3,E_PROGRESS1,E_RADIO1
      ,E_RADIO2,E_SCAN1,E_ELEM_LISTBOX};
enum {E_FONT_SERIFB1,E_FONT_SERIFI1,E_FONT_SERIFI3,E_FONT_SERIFI4
      ,E_FONT_TXT1};
//<Enum !End!>

#else
//<Enum !Start!>
enum {E_PG_MAIN,E_PG2,E_PG_INTRO,E_PG_DIAG
      ,E_PG_FILE,E_PG_CSV,E_PG_OPTION,E_PG_MAIN2
	  ,E_PG9};
enum {E_BOX10,E_BOX11,E_BOX12,E_BOX13
      ,E_BOX15,E_BOX16,E_BOX17,E_BOX18,E_BOX19
      ,E_BOX2,E_BOX3,E_BOX4,E_BOX5,E_BOX6
      ,E_BOX8,E_BOX9,E_BTN1,E_BTN10
      ,E_BTN11,E_BTN12,E_BTN13,E_BTN14
      ,E_BTN15,E_BTN16,E_BTN17,E_BTN18
      ,E_BTN19,E_BTN2,E_BTN20,E_BTN21
      ,E_BTN22,E_BTN23,E_BTN24,E_BTN25
      ,E_BTN26,E_BTN27,E_BTN28,E_BTN29
      ,E_BTN3,E_BTN30,E_BTN31,E_BTN32
      ,E_BTN33,E_BTN34,E_BTN35,E_BTN36
      ,E_BTN37,E_BTN38,E_BTN39,E_BTN4
      ,E_BTN40,E_BTN41,E_BTN42,E_BTN43
      ,E_BTN44,E_BTN45,E_BTN46,E_BTN47
      ,E_BTN5,E_BTN6,E_BTN7,E_BTN8,E_BTN9
#ifdef ADDRADIO_2
	  ,E_CHECK3,E_CHECK4,E_PROGRESS1,E_PROGRESS2,E_PROGRESS3
	  ,E_RADIO1,E_RADIO2,E_RADIO3,E_RADIO4,E_SCAN1,E_ELEM_LISTBOX};
#else	  
  	  ,E_CHECK1,E_CHECK2,E_CHECK3,E_PROGRESS1
	  ,E_RADIO1,E_RADIO2,E_SCAN1,E_ELEM_LISTBOX};
#endif

enum {E_FONT_SERIFB1,E_FONT_SERIFI1,E_FONT_SERIFI3,E_FONT_SERIFI4
      ,E_FONT_TXT1};
//<Enum !End!>



#endif
 
enum {E_GROUP1,E_GROUP2};


// ------------------------------------------------
// Instantiate the GUI
// ------------------------------------------------

#if 0
// Define the maximum number of elements per page
//<ElementDefines !Start!>
#define MAX_PAGE                6
#define MAX_FONT                5
#define MAX_ELEM_PG_MAIN 24
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN
#define MAX_ELEM_PG2 21
#define MAX_ELEM_PG2_RAM MAX_ELEM_PG2
#define MAX_ELEM_PG_INTRO 5
#define MAX_ELEM_PG_INTRO_RAM MAX_ELEM_PG_INTRO
#define MAX_ELEM_PG_DIAG 17
#define MAX_ELEM_PG_DIAG_RAM MAX_ELEM_PG_DIAG
#define MAX_ELEM_PG_FILE 5+1
#define MAX_ELEM_PG_FILE_RAM MAX_ELEM_PG_FILE
#define MAX_ELEM_PG_CSV 20
#define MAX_ELEM_PG_CSV_RAM MAX_ELEM_PG_CSV
//<ElementDefines !End!>


#else
// Define the maximum number of elements per page
//<ElementDefines !Start!>
#define MAX_PAGE                9
#define MAX_FONT                5
#define MAX_ELEM_PG_MAIN 25
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN
#define MAX_ELEM_PG2 23
#define MAX_ELEM_PG2_RAM MAX_ELEM_PG2
#define MAX_ELEM_PG_INTRO 5
#define MAX_ELEM_PG_INTRO_RAM MAX_ELEM_PG_INTRO
#define MAX_ELEM_PG_DIAG 17
#define MAX_ELEM_PG_DIAG_RAM MAX_ELEM_PG_DIAG
#define MAX_ELEM_PG_FILE 5+1
#define MAX_ELEM_PG_FILE_RAM MAX_ELEM_PG_FILE
#define MAX_ELEM_PG_CSV 26
#define MAX_ELEM_PG_CSV_RAM MAX_ELEM_PG_CSV
#define MAX_ELEM_PG_OPTION 11
#define MAX_ELEM_PG_OPTION_RAM MAX_ELEM_PG_OPTION
#define MAX_ELEM_PG_MAIN2 22
#define MAX_ELEM_PG_MAIN2_RAM MAX_ELEM_PG_MAIN2
#define MAX_ELEM_PG9 15
#define MAX_ELEM_PG9_RAM MAX_ELEM_PG9

//<ElementDefines !End!>


#endif



#define MAX_STR                 20
#define TXTLENGTH    			20
#define LISTTXTLENGTH    		30

class TftGuiLCDFunClass 
{
public:
	void InitTftGuiLCDClass(void);
	// Define debug message function
	static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }
	
	// ------------------------------------------------
	// Callback Methods
	// ------------------------------------------------
	// Common Button callback
	static bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY);
	static bool CbDrawScanner(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw);
	static bool CbSlidePos(void* pvGui,void* pvElemRef,int16_t nPos);
	static bool CbCheckbox(void* pvGui, void* pvElemRef, int16_t nSelId, bool bChecked);
	static bool CbCheckboxCAN(void* pvGui, void* pvElemRef, int16_t nSelId, bool bChecked);

	static bool CbListbox(void* pvGui, void* pvElemRef, int16_t nSelId);
	
	bool InitGUI();
	void Gui_Update();
	void InitValueSet();
	void ChangeButtonSet();
	void ChangeInitButtonSet(void);
	void SetUpInit();
	void ListBoxUpdate();
//	void ProgressBarGage(uint16_t percentage,uint8_t OnOff);
	void ProgressBarGage(float percentage,uint8_t OnOff);
	void ProgressBarGageOTA(float percentage,uint8_t OnOff);

	void VersionCheckSW(void);
	void VersionCheckSWDiag(void);
	void VersionCheckHW(void);
	void VersionCheckSW_SUP(void);
	void VersionCheckHW_SUP(void);
	void EEPSaveParam(void);
	void CsvParamView(void);

	void CheckDTC_Pending_SUP(void);
	void CheckDTC_Confirming_SUP(void);
	void CheckDTC_ALL_SUP(void);

	void CheckDTC_PendingCnt_SUP(void);
	void CheckDTC_ConfirmingCnt_SUP(void);
	void CheckDTC_ALLCnt_SUP(void);
	void OTASWUpgradeMenu(void);
	
public:
	gslc_tsXListbox             	m_sXListbox;	
	gslc_tsXTextbox          	    m_sTextbox;
	char                        	m_acTextboxBuf[MAX_STR];

	gslc_tsElemRef*  m_pElemListbox = NULL;
	gslc_tsElemRef*  m_pElemListBoxTextStr = NULL;

// MAIN1_BG
	gslc_tsElemRef* pElemRefBk0 = NULL;
    gslc_tsElemRef* pElemRefBk1 = NULL;
	gslc_tsElemRef* pElemRefBk2 = NULL;
    gslc_tsElemRef* pElemRefBk3 = NULL;
	gslc_tsElemRef* pElemRefBk4 = NULL;
    gslc_tsElemRef* pElemRefBk5 = NULL;
// MAIN2_BG
	gslc_tsElemRef* pElemRefM2Bk0 = NULL;
    gslc_tsElemRef* pElemRefM2Bk1 = NULL;
	gslc_tsElemRef* pElemRefM2Bk2 = NULL;
    gslc_tsElemRef* pElemRefM2Bk3 = NULL;
	gslc_tsElemRef* pElemRefM2Bk4 = NULL;
    gslc_tsElemRef* pElemRefM2Bk5 = NULL;

	gslc_tsElemRef* pElemRefBk10 = NULL;
    gslc_tsElemRef* pElemRefBk11 = NULL;
	gslc_tsElemRef* pElemRefBk12 = NULL;
    gslc_tsElemRef* pElemRefBk13 = NULL;

	gslc_tsElemRef* pElemRefCheckBox1 = NULL;
    gslc_tsElemRef* pElemRefCheckBox2 = NULL;
	gslc_tsElemRef* pElemRefCheckBox3 = NULL;
	gslc_tsElemRef* pElemRefCheckBox4 = NULL;
	
    gslc_tsElemRef* pElemRefCheckRadiO1 = NULL;
    gslc_tsElemRef* pElemRefCheckRadiO2 = NULL;
    gslc_tsElemRef* pElemRefCheckRadiO3 = NULL;
    gslc_tsElemRef* pElemRefCheckRadiO4 = NULL;

	gslc_tsElemRef* m_pElemTextStr1 = NULL;  
	gslc_tsElemRef* m_pElemTextStr2 = NULL;
	gslc_tsElemRef* m_pElemTextStr3 = NULL;  
	gslc_tsElemRef* m_pElemTextStr4 = NULL;
	gslc_tsElemRef* m_pElemTextStr5 = NULL;  
	gslc_tsElemRef* m_pElemTextStr6 = NULL;
	gslc_tsElemRef* m_pElemTextStr7 = NULL;  
	gslc_tsElemRef* m_pElemTextStr8 = NULL;

	gslc_tsElemRef* m_pElemTextStr9 = NULL;  
	gslc_tsElemRef* m_pElemTextStr10 = NULL;  
	gslc_tsElemRef* m_pElemTextStr11 = NULL;  
	gslc_tsElemRef* m_pElemTextStr12 = NULL;  

	gslc_tsElemRef* m_pElemTextStr13 = NULL;  // MEMU2 TEXT Edit
	gslc_tsElemRef* m_pElemTextStr14 = NULL;  
	gslc_tsElemRef* m_pElemTextStr15 = NULL;  

	gslc_tsElemRef* m_pElemTextStr16 = NULL;  // MEMU2 TEXT Edit P :
	gslc_tsElemRef* m_pElemTextStr17 = NULL;  // MEMU2 TEXT Edit H : 

	gslc_tsElemRef* m_pElemTextStrCSV1 = NULL;  
	gslc_tsElemRef* m_pElemTextStrCSV2 = NULL;  
	gslc_tsElemRef* m_pElemTextStrCSV3 = NULL;  
	gslc_tsElemRef* m_pElemTextStrCSV4 = NULL;  

	gslc_tsElemRef* m_pElemTextStrCSV5 = NULL;	
	gslc_tsElemRef* m_pElemTextStrCSV6 = NULL;	
	gslc_tsElemRef* m_pElemTextStrCSV7 = NULL;	
	gslc_tsElemRef* m_pElemTextStrCSV8 = NULL;	

	gslc_tsElemRef* m_pElemTextStrCSV9 = NULL;	
	gslc_tsElemRef* m_pElemTextStrCSV10 = NULL;	
	gslc_tsElemRef* m_pElemTextStrCSV11 = NULL;	

	gslc_tsElemRef* m_pElemTextStrOptionSTmin = NULL;	
//	gslc_tsElemRef* m_pElemTextStrOptionSTminP = NULL;	
//	gslc_tsElemRef* m_pElemTextStrOptionSTminM = NULL;	

	gslc_tsElemRef* m_pElemTextStrOTAIP = NULL;	
	gslc_tsElemRef* m_pElemTextStrOTASSIDPW = NULL; 
	gslc_tsElemRef* m_pElemTextStrOTAFNAME = NULL; 
	gslc_tsElemRef* m_pElemTextStrOTAPER = NULL; 
	gslc_tsElemRef* m_pElemTextStrOTAPW = NULL; 
	gslc_tsElemRef* m_pElemTextStrOTAStartStop = NULL; 

public:
	
	// GUI Elements
	gslc_tsGui						m_gui;
	gslc_tsDriver					m_drv;
	gslc_tsFont 					m_asFont[MAX_FONT];
	gslc_tsPage 					m_asPage[MAX_PAGE];

#if 0	
	//<GUI_Extra_Elements !Start!>
	gslc_tsElem 					m_asPage1Elem[MAX_ELEM_PG_MAIN_RAM];
	gslc_tsElemRef					m_asPage1ElemRef[MAX_ELEM_PG_MAIN];
	gslc_tsElem 					m_asPage2Elem[MAX_ELEM_PG2_RAM];
	gslc_tsElemRef					m_asPage2ElemRef[MAX_ELEM_PG2];
	gslc_tsElem 					m_asPage3Elem[MAX_ELEM_PG_INTRO_RAM];
	gslc_tsElemRef					m_asPage3ElemRef[MAX_ELEM_PG_INTRO];
	gslc_tsElem 					m_asPage4Elem[MAX_ELEM_PG_DIAG_RAM];
	gslc_tsElemRef					m_asPage4ElemRef[MAX_ELEM_PG_DIAG];
	gslc_tsElem 					m_asPage5Elem[MAX_ELEM_PG_FILE_RAM];
	gslc_tsElemRef					m_asPage5ElemRef[MAX_ELEM_PG_FILE];
	gslc_tsElem 					m_asPage6Elem[MAX_ELEM_PG_CSV_RAM];
	gslc_tsElemRef					m_asPage6ElemRef[MAX_ELEM_PG_CSV];
	gslc_tsXCheckbox				m_asXCheck[5];
	gslc_tsXGauge					m_sXGauge[1];

#else
	//<GUI_Extra_Elements !Start!>
	gslc_tsElem 					m_asPage1Elem[MAX_ELEM_PG_MAIN_RAM];
	gslc_tsElemRef					m_asPage1ElemRef[MAX_ELEM_PG_MAIN];
	gslc_tsElem 					m_asPage2Elem[MAX_ELEM_PG2_RAM];
	gslc_tsElemRef					m_asPage2ElemRef[MAX_ELEM_PG2];
	gslc_tsElem 					m_asPage3Elem[MAX_ELEM_PG_INTRO_RAM];
	gslc_tsElemRef					m_asPage3ElemRef[MAX_ELEM_PG_INTRO];
	gslc_tsElem 					m_asPage4Elem[MAX_ELEM_PG_DIAG_RAM];
	gslc_tsElemRef					m_asPage4ElemRef[MAX_ELEM_PG_DIAG];
	gslc_tsElem 					m_asPage5Elem[MAX_ELEM_PG_FILE_RAM];
	gslc_tsElemRef					m_asPage5ElemRef[MAX_ELEM_PG_FILE];
	gslc_tsElem 					m_asPage6Elem[MAX_ELEM_PG_CSV_RAM];
	gslc_tsElemRef					m_asPage6ElemRef[MAX_ELEM_PG_CSV];
	gslc_tsElem 					m_asPage7Elem[MAX_ELEM_PG_OPTION_RAM];
	gslc_tsElemRef					m_asPage7ElemRef[MAX_ELEM_PG_OPTION];
	gslc_tsElem 					m_asPage8Elem[MAX_ELEM_PG_MAIN2_RAM];
	gslc_tsElemRef					m_asPage8ElemRef[MAX_ELEM_PG_MAIN2];
	gslc_tsElem 					m_asPage9Elem[MAX_ELEM_PG9_RAM];
	gslc_tsElemRef					m_asPage9ElemRef[MAX_ELEM_PG9];
	
#ifdef ADDRADIO_2
	gslc_tsXCheckbox				m_asXCheck[4];
	gslc_tsXCheckbox				m_asXCheckR[2];
#else
	gslc_tsXCheckbox				m_asXCheck[5];
#endif
	gslc_tsXGauge					m_sXGauge[3];

#endif

public:
	
	// ------------------------------------------------
	// Save some element references for update loop access
	// ------------------------------------------------
	//<Save_References !Start!>
	gslc_tsElemRef* 			   m_pElemProgress1  = NULL;
	gslc_tsElemRef* 			   m_pElemProgress2  = NULL;
	gslc_tsElemRef* 			   m_pElemProgress3  = NULL;
	//<Save_References !End!>

public:
	bool WifiConnected = false;		
	bool InitParamSet  = false;		
	bool SelParamSet  = false;		
	bool NextSelParamSet  = false;		

	bool checkbox1 = false;
	bool checkbox2 = false;
	bool checkbox3 = false;
	bool checkbox4 = false;
	
	uint8_t WifiSetting = 0;
	uint8_t TSizeAP  = 0;
	uint8_t TSizeSTA = 0;
	uint8_t CANtypeSetting = 0;

	uint8_t TDTCP = 0;
	uint8_t TDTCH = 0;

	uint8_t SelectedID = 0;
	char	ListBoxTxt[LISTTXTLENGTH];
	
	const char * APMODE  = "[AP] ";  // AP MODE;
	const char * STAMODE = "[STA] "; // STA MODE

	const char * DTCP = "P:";  // AP MODE;
	const char * DTCH = "H:"; // STA MODE


    char	ListBoxTxt1[LISTTXTLENGTH];
	char	ListBoxTxt2[LISTTXTLENGTH];
	char	ListBoxTxt3[LISTTXTLENGTH];
	char	ListBoxTxt4[LISTTXTLENGTH];
	char	ListBoxTxt5[LISTTXTLENGTH];
		
};



extern TftGuiLCDFunClass TftGuiLCDFun;

#endif // RepFileFunc_h
