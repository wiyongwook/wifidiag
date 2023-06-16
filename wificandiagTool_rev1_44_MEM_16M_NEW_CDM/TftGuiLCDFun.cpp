#include "TftGuiLCDFun.h"
#include "WifiFunc.h"


void TftGuiLCDFunClass::InitTftGuiLCDClass(void)
{
  // Initialize
   
   gslc_InitDebug(&DebugOut);
  
	if (!gslc_Init(&m_gui,&m_drv,m_asPage,MAX_PAGE,m_asFont,MAX_FONT)) { return; }
  
	// ------------------------------------------------
	// Load Fonts
	// ------------------------------------------------
	//<Load_Fonts !Start!>
		if (!gslc_FontAdd(&m_gui,E_FONT_SERIFB1,GSLC_FONTREF_PTR,&FreeSerifBold9pt7b,1)) { return; }
		if (!gslc_FontAdd(&m_gui,E_FONT_SERIFI1,GSLC_FONTREF_PTR,&FreeSerifItalic9pt7b,1)) { return; }
		if (!gslc_FontAdd(&m_gui,E_FONT_SERIFI3,GSLC_FONTREF_PTR,&FreeSerifItalic18pt7b,1)) { return; }
		if (!gslc_FontAdd(&m_gui,E_FONT_SERIFI4,GSLC_FONTREF_PTR,&FreeSerifItalic24pt7b,1)) { return; }
		if (!gslc_FontAdd(&m_gui,E_FONT_TXT1,GSLC_FONTREF_PTR,NULL,1)) { return; }
	//<Load_Fonts !End!>
  
	// ------------------------------------------------
	// Create graphic elements
	// ------------------------------------------------
	InitGUI();
  
	// ------------------------------------------------
	// Save some element references for quick access
	// ------------------------------------------------
  //<Quick_Access !Start!>
	m_pElemProgress1  = gslc_PageFindElemById(&m_gui,E_PG_MAIN,E_PROGRESS1);
	m_pElemProgress2  = gslc_PageFindElemById(&m_gui,E_PG_MAIN2,E_PROGRESS2);
	m_pElemProgress3  = gslc_PageFindElemById(&m_gui,E_PG9,E_PROGRESS3);
  //<Quick_Access !End!>
  
  //<Startup !Start!>
	// ------------------------------------------------
	// Start up display on first page
	// ------------------------------------------------
	gslc_SetPageCur(&m_gui,E_PG_INTRO);
  //<Startup !End!>



}

void TftGuiLCDFunClass::Gui_Update()
{
	char ATxt[TXTLENGTH];
	TSizeAP  = strlen(APMODE);  // 5
	TSizeSTA = strlen(STAMODE); // 6 


	InitValueSet();

	// ------------------------------------------------
	// Periodically call GUIslice update function
	// ------------------------------------------------
	gslc_Update(&m_gui);


	if(WifiSetting==1)
	{
		if((WiFiFnc.connected == CONNECTED) && (WifiConnected == false))
		{
			WifiConnected = true;
			memcpy(ATxt,STAMODE,TSizeSTA);
			WiFiFnc.Local_IpAddress.toCharArray(&ATxt[TSizeSTA],TXTLENGTH-TSizeSTA); 
			snprintf(ATxt,TXTLENGTH,"%s",ATxt);
			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr4,GSLC_COL_RED);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr4,ATxt);
		}

		if( WiFiFnc.connected == DISCONNECT && (WifiConnected == true))
		{
			WifiConnected = false;
			memcpy(ATxt,STAMODE,TSizeSTA);
			WiFiFnc.Local_IpAddress.toCharArray(&ATxt[TSizeSTA],TXTLENGTH-TSizeSTA); 
			snprintf(ATxt,TXTLENGTH,"%s",ATxt);
			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr4,GSLC_COL_WHITE);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr4,ATxt);
		}
	}

	
}


// ------------------------------------------------
// Create page elements
// ------------------------------------------------
bool TftGuiLCDFunClass::InitGUI()
{
	 gslc_tsElemRef* pElemRef = NULL;
	
	//<InitGUI !Start!>
	  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
	  gslc_PageAdd(&m_gui,E_PG2,m_asPage2Elem,MAX_ELEM_PG2_RAM,m_asPage2ElemRef,MAX_ELEM_PG2);
	  gslc_PageAdd(&m_gui,E_PG_INTRO,m_asPage3Elem,MAX_ELEM_PG_INTRO_RAM,m_asPage3ElemRef,MAX_ELEM_PG_INTRO);
	  gslc_PageAdd(&m_gui,E_PG_DIAG,m_asPage4Elem,MAX_ELEM_PG_DIAG_RAM,m_asPage4ElemRef,MAX_ELEM_PG_DIAG);
	  gslc_PageAdd(&m_gui,E_PG_FILE,m_asPage5Elem,MAX_ELEM_PG_FILE_RAM,m_asPage5ElemRef,MAX_ELEM_PG_FILE);
	  gslc_PageAdd(&m_gui,E_PG_CSV,m_asPage6Elem,MAX_ELEM_PG_CSV_RAM,m_asPage6ElemRef,MAX_ELEM_PG_CSV);
	  gslc_PageAdd(&m_gui,E_PG_OPTION,m_asPage7Elem,MAX_ELEM_PG_OPTION_RAM,m_asPage7ElemRef,MAX_ELEM_PG_OPTION);
	  gslc_PageAdd(&m_gui,E_PG_MAIN2,m_asPage8Elem,MAX_ELEM_PG_MAIN2_RAM,m_asPage8ElemRef,MAX_ELEM_PG_MAIN2);
	  gslc_PageAdd(&m_gui,E_PG9,m_asPage9Elem,MAX_ELEM_PG9_RAM,m_asPage9ElemRef,MAX_ELEM_PG9);

	  
	  // Background flat color
	  gslc_SetBkgndColor(&m_gui,GSLC_COL_BLACK);
	
	  // -----------------------------------
	  // PAGE: E_PG_MAIN
	   
	 // Create E_SCAN1 box
	  pElemRef = gslc_ElemCreateBox(&m_gui,E_SCAN1,E_PG_MAIN,(gslc_tsRect){5,5,310,230});
	  // Set the callback function to handle all drawing for the element
	  gslc_ElemSetDrawFunc(&m_gui,pElemRef,&CbDrawScanner);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,GSLC_COL_BLACK,GSLC_COL_BLACK);
  
	   
	 // Create E_BOX5 box
	  pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX5,E_PG_MAIN,(gslc_tsRect){10,180,300,50});
  	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GREEN,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){10,44,50,26},
		(char*)"File Name",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){10,10,50,26},
		(char*)"ECU Ver",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){10,80,50,26},
		(char*)"Down Info",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){12,185,25,12},
		(char*)"IP",0,E_FONT_TXT1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){12,212,25,12},
		(char*)"SSID",0,E_FONT_TXT1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){180,185,25,12},
		(char*)"PORT",0,E_FONT_TXT1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){180,212,25,12},
		(char*)"PW",0,E_FONT_TXT1);
	  
	  // create E_BTN1 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN1,E_PG_MAIN,
		(gslc_tsRect){250,10,60,30},(char*)"Ver",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GREEN,GSLC_COL_GREEN,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  pElemRefBk0 = pElemRef;
	  
	  // create E_BTN2 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN2,E_PG_MAIN,
		(gslc_tsRect){250,80,60,30},(char*)"Down",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_CYAN);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  pElemRefBk1 = pElemRef;
	  
	  // create E_BTN3 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN3,E_PG_MAIN,
		(gslc_tsRect){10,140,60,30},(char*)"Set-Up",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_YELLOW,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  
	  // create E_BTN4 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN4,E_PG_MAIN,
		(gslc_tsRect){75,140,60,30},(char*)"Option",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  
	  // create E_BTN5 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN5,E_PG_MAIN,
		(gslc_tsRect){250,140,60,30},(char*)"WIFI",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_RED,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  pElemRefBk2 = pElemRef;
	  
	  // create E_BTN6 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN6,E_PG_MAIN,
		(gslc_tsRect){140,140,60,30},(char*)"Diag",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	
	  // Create progress bar E_PROGRESS1 
	  pElemRef = gslc_ElemXGaugeCreate(&m_gui,E_PROGRESS1,E_PG_MAIN,&m_sXGauge[0],
		(gslc_tsRect){10,115,300,20},0,100,0,GSLC_COL_GREEN,false);
	  
	  // Create GSLC_ID_AUTO text label
	  static char mstr2[MAX_STR] = "";
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){100,10,145,30},
		mstr2,sizeof(mstr2),E_FONT_SERIFI1);
	  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  m_pElemTextStr2 = pElemRef;

	  // Create GSLC_ID_AUTO text label
	  static char mstr1[MAX_STR] = "";
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){100,45,210,30}, //10,210
		mstr1,sizeof(mstr1),E_FONT_SERIFI1);
	  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  m_pElemTextStr1 = pElemRef;
	  
	  // Create GSLC_ID_AUTO text label
	  static char mstr3[MAX_STR] = "";
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){100,80,145,30},
		mstr3,sizeof(mstr3),E_FONT_SERIFI1);
	  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  m_pElemTextStr3 = pElemRef;
	  
	  // Create GSLC_ID_AUTO text label
	  static char mstr4[MAX_STR] = "";
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){40,180,130,22},
		mstr4,sizeof(mstr4),E_FONT_TXT1);
	  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  m_pElemTextStr4 = pElemRef;
	  
	  // Create GSLC_ID_AUTO text label
	  static char mstr5[MAX_STR] = "";
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){40,208,130,22},
		mstr5,sizeof(mstr5),E_FONT_TXT1);
	  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  m_pElemTextStr5 = pElemRef;
	  
	  // Create GSLC_ID_AUTO text label
	  static char mstr6[MAX_STR] = "";
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){210,180,100,22},
		mstr6,sizeof(mstr6),E_FONT_TXT1);
	  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  m_pElemTextStr6 = pElemRef;
	  
	  // Create GSLC_ID_AUTO text label
	  static char mstr7[MAX_STR] = "";
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){210,208,100,22},
		mstr7,sizeof(mstr7),E_FONT_TXT1);
	  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  m_pElemTextStr7 = pElemRef;

	  // create E_BTN21 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN21,E_PG_MAIN,
		(gslc_tsRect){205,140,40,30},(char*)"File",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_YELLOW,GSLC_COL_GRAY_LT1);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	  // create E_BTN46 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN46,E_PG_MAIN,
		(gslc_tsRect){10,80,85,30},(char*)"Down Info",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_ORANGE,GSLC_COL_GRAY_LT2);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	
	  // -----------------------------------
	  // PAGE: E_PG2
	   
	  // Create E_BOX2 box
	  pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX2,E_PG2,(gslc_tsRect){5,5,310,230});
	   
	  // Create E_BOX3 box
	  pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX3,E_PG2,(gslc_tsRect){60,40,250,40});
	   
	  // Create E_BOX4 box
	  pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX4,E_PG2,(gslc_tsRect){60,90,250,40});
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){10,10,82,26},
		(char*)"Set Up",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){10,45,40,26},
		(char*)"WIFI",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){10,95,40,26},
		(char*)"CAN",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){70,45,81,26},
		(char*)"STA Mode",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){200,45,73,26},
		(char*)"AP Mode",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){70,95,62,26},
		(char*)"HSCAN",0,E_FONT_SERIFI1);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){200,95,64,26},
		(char*)"CANFD",0,E_FONT_SERIFI1);
	  
	  // create E_BTN7 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN7,E_PG2,
		(gslc_tsRect){250,200,60,30},(char*)"O.K",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  
	  // Create radio button E_RADIO1
	  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_RADIO1,E_PG2,&m_asXCheck[0],
		(gslc_tsRect){160,50,20,20},true,GSLCX_CHECKBOX_STYLE_ROUND,(gslc_tsColor){255,200,0},false);
	  gslc_ElemSetGroup(&m_gui,pElemRef,E_GROUP1);  
	  gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
	  pElemRefCheckRadiO1 = pElemRef;

	  // Create radio button E_RADIO2
	  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_RADIO2,E_PG2,&m_asXCheck[1],
		(gslc_tsRect){280,50,20,20},true,GSLCX_CHECKBOX_STYLE_ROUND,(gslc_tsColor){255,200,0},true);
	  gslc_ElemSetGroup(&m_gui,pElemRef,E_GROUP1);  
	  gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
	  pElemRefCheckRadiO2 = pElemRef;
#if 0
	  // create checkbox E_CHECK1
	  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_CHECK1,E_PG2,&m_asXCheck[2],
		(gslc_tsRect){160,100,20,20},false,GSLCX_CHECKBOX_STYLE_X,(gslc_tsColor){255,200,0},true);
      gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
	  pElemRefCheckBox1 = pElemRef;
		 
	  // create checkbox E_CHECK2
	  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_CHECK2,E_PG2,&m_asXCheck[3],
		(gslc_tsRect){280,100,20,20},false,GSLCX_CHECKBOX_STYLE_X,(gslc_tsColor){255,200,0},true);
      gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
	  pElemRefCheckBox2 = pElemRef;
#else
 	  bool RadioCheck34 ;
      RadioCheck34 = RFileFnc.CanFDSettingdState;

	  // Create radio button E_RADIO3
	  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_RADIO3,E_PG2,&m_asXCheckR[0],
		  (gslc_tsRect){160,100,20,20},true,GSLCX_CHECKBOX_STYLE_ROUND,(gslc_tsColor){255,200,0},true);
	  gslc_ElemSetGroup(&m_gui,pElemRef,E_GROUP2);  
	  gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckboxCAN);
	  pElemRefCheckRadiO3 = pElemRef;

	  // Create radio button E_RADIO4
      pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_RADIO4,E_PG2,&m_asXCheckR[1],
	      (gslc_tsRect){280,100,20,20},true,GSLCX_CHECKBOX_STYLE_ROUND,(gslc_tsColor){255,200,0},false);
	  gslc_ElemSetGroup(&m_gui,pElemRef,E_GROUP2);  
	  gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckboxCAN);
	  pElemRefCheckRadiO4 = pElemRef;
#endif
  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){230,10,60,12},
		(char*)"SL Corporation",0,E_FONT_TXT1);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_GRAY);
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){10,150,40,26},
		(char*)"OPT",0,E_FONT_SERIFI1);
	   
	  // Create E_BOX10 box
	  pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX10,E_PG2,(gslc_tsRect){60,140,250,40});
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){70,145,72,26},
		(char*)"WIFI ON",0,E_FONT_SERIFI1);
	   
	  // create checkbox E_CHECK3
	  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_CHECK3,E_PG2,&m_asXCheck[2],
		(gslc_tsRect){160,150,20,20},false,GSLCX_CHECKBOX_STYLE_X,(gslc_tsColor){255,200,0},true);
      gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
	  pElemRefCheckBox3 = pElemRef;
	  	
	  // create E_BTN24 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN24,E_PG2,
		(gslc_tsRect){10,200,60,30},(char*)"ENV",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){200,145,68,26},
	  (char*)"HS ON",0,E_FONT_SERIFI1);

	  // create checkbox E_CHECK4
	  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_CHECK4,E_PG2,&m_asXCheck[3],
	  (gslc_tsRect){280,150,20,20},false,GSLCX_CHECKBOX_STYLE_X,(gslc_tsColor){255,200,0},true);
      gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
	  pElemRefCheckBox4 = pElemRef;

	  // -----------------------------------
	  // PAGE: E_PG_INTRO
		
	  // Create E_BOX6 box
	  pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX6,E_PG_INTRO,(gslc_tsRect){5,5,310,230});
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,GSLC_COL_BLACK,GSLC_COL_BLACK);

	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_INTRO,(gslc_tsRect){50,50,56,67},
	    (char*)"SL",0,E_FONT_SERIFI4);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BROWN,GSLC_COL_BLACK,GSLC_COL_BLACK);

	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_INTRO,(gslc_tsRect){110,60,185,51},
	    (char*)"Corporation",0,E_FONT_SERIFI3);

	  // create E_BTN8 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN8,E_PG_INTRO,
	    (gslc_tsRect){130,160,60,30},(char*)"O.K",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY_DK3,GSLC_COL_BLUE_DK1,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_INTRO,(gslc_tsRect){30,202,250,28},
	    (char*)"SL Corporation All Rights Reserved",0,E_FONT_SERIFI1);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_RED);


	 
	  // -----------------------------------
	   // PAGE: E_PG_DIAG
		
	   // Create E_BOX8 box
	   pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX8,E_PG_DIAG,(gslc_tsRect){5,5,310,230});
	   
	   // create E_BTN9 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN9,E_PG_DIAG,
		 (gslc_tsRect){10,10,100,30},(char*)"S/W Ver",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_YELLOW,GSLC_COL_GRAY_LT3);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   pElemRefBk10 = pElemRef;
	   
	   // Create GSLC_ID_AUTO text label  --- DIAG SW Version
	   static char mstr9[MAX_STR] = "";
	   pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_DIAG,(gslc_tsRect){115,10,200,30},
		 mstr9,sizeof(mstr9),E_FONT_SERIFI1);
	   gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   m_pElemTextStr9 = pElemRef;

	   
	   // create E_BTN10 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN10,E_PG_DIAG,
		 (gslc_tsRect){250,195,60,30},(char*)"O.K",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLACK,GSLC_COL_BLUE,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   
	   // create E_BTN11 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN11,E_PG_DIAG,
		 (gslc_tsRect){10,45,100,30},(char*)"H/W Ver",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   pElemRefBk11 = pElemRef;
	   
	   // Create GSLC_ID_AUTO text label  --- DIAG HW Version
	   static char mstr10[MAX_STR] = "";
	   pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_DIAG,(gslc_tsRect){115,45,200,30},
		 mstr10,sizeof(mstr10),E_FONT_SERIFI1);
	   gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   m_pElemTextStr10 = pElemRef;
	   
	   // create E_BTN12 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN12,E_PG_DIAG,
		 (gslc_tsRect){10,80,100,30},(char*)"PartN/B RD",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_YELLOW,GSLC_COL_GRAY_LT2);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   pElemRefBk12 = pElemRef;

	   
	   // Create GSLC_ID_AUTO text label  --- DIAG SW_SUP Version
	   static char mstr11[MAX_STR] = "";
	   pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_DIAG,(gslc_tsRect){115,80,200,30},
		 mstr11,sizeof(mstr11),E_FONT_SERIFI1);
	   gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   m_pElemTextStr11 = pElemRef;
	   
	   // create E_BTN13 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN13,E_PG_DIAG,
		 (gslc_tsRect){10,115,100,30},(char*)"PartN/B WR",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT2);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   pElemRefBk13 = pElemRef;

	   
	   // Create GSLC_ID_AUTO text label  --- DIAG HW_SUP Version
	   static char mstr12[MAX_STR] = "";
	   pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_DIAG,(gslc_tsRect){115,115,200,30},
		 mstr12,sizeof(mstr12),E_FONT_SERIFI1);
	   gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLACK,GSLC_COL_BLACK);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   m_pElemTextStr12 = pElemRef;
	   
	   // create E_BTN14 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN14,E_PG_DIAG,
		 (gslc_tsRect){10,155,70,30},(char*)"DID100",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GREEN,GSLC_COL_GREEN,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   
	   // create E_BTN15 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN15,E_PG_DIAG,
		 (gslc_tsRect){85,155,70,30},(char*)"DID101",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   
	   // create E_BTN16 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN16,E_PG_DIAG,
		 (gslc_tsRect){245,155,70,30},(char*)"DID103",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   
	   // create E_BTN17 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN17,E_PG_DIAG,
		 (gslc_tsRect){170,155,70,30},(char*)"DID102",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   
	   // create E_BTN18 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN18,E_PG_DIAG,
		 (gslc_tsRect){10,195,70,30},(char*)"DID104",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   
	   // create E_BTN19 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN19,E_PG_DIAG,
		 (gslc_tsRect){85,195,70,30},(char*)"DID105",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	   
	   // create E_BTN20 button with text label
	   pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN20,E_PG_DIAG,
		 (gslc_tsRect){170,195,70,30},(char*)"DID106",0,E_FONT_SERIFI1,&CbBtnCommon);
	   gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	   gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN,GSLC_COL_GRAY);
	   gslc_ElemSetFrameEn(&m_gui,pElemRef,true);


	  // -----------------------------------
	  // PAGE: E_PG_FILE
	   
	  // Create E_BOX9 box
	  pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX9,E_PG_FILE,(gslc_tsRect){5,5,310,230});
	  
	  // Create GSLC_ID_AUTO text label
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_FILE,(gslc_tsRect){10,10,68,26},
		(char*)"File List",0,E_FONT_SERIFI1);

	  // Create GSLC_ID_AUTO text label  ///-------ListBox Text
	  static char ListBoxstr[LISTTXTLENGTH] = "";
	  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_FILE,(gslc_tsRect){80,10,230,30},
		ListBoxstr,sizeof(ListBoxstr),E_FONT_SERIFI1);
	  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	  m_pElemListBoxTextStr = pElemRef;

	  // create E_BTN22 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN22,E_PG_FILE,
		(gslc_tsRect){250,200,60,30},(char*)"O.K",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
 	  
	  // create E_BTN23 button with text label
	  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN23,E_PG_FILE,
		(gslc_tsRect){10,200,60,30},(char*)"Del",0,E_FONT_SERIFI1,&CbBtnCommon);
	  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GREEN,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	  // Create the XListbox  
	  // - Other APIs that can be called:  
	  //   gslc_ElemXListboxItemsSetGap(&m_gui, pElemRef, 2);	
	  //   gslc_ElemXListboxItemsSetTxtMargin(&m_gui, pElemRef, 5, 8);  

	  RFileFnc.DownLoadFileName[0].toCharArray(ListBoxTxt1,LISTTXTLENGTH);
	  RFileFnc.DownLoadFileName[1].toCharArray(ListBoxTxt2,LISTTXTLENGTH);
	  RFileFnc.DownLoadFileName[2].toCharArray(ListBoxTxt3,LISTTXTLENGTH);
	  RFileFnc.DownLoadFileName[3].toCharArray(ListBoxTxt4,LISTTXTLENGTH);
	  RFileFnc.DownLoadFileName[4].toCharArray(ListBoxTxt5,LISTTXTLENGTH);

	  snprintf(ListBoxTxt1,LISTTXTLENGTH,"%s",ListBoxTxt1);
	  snprintf(ListBoxTxt2,LISTTXTLENGTH,"%s",ListBoxTxt2);
	  snprintf(ListBoxTxt3,LISTTXTLENGTH,"%s",ListBoxTxt3);
	  snprintf(ListBoxTxt4,LISTTXTLENGTH,"%s",ListBoxTxt4);
	  snprintf(ListBoxTxt5,LISTTXTLENGTH,"%s",ListBoxTxt5);

	  static uint8_t m_pXListboxBuf[50];  
	  pElemRef = gslc_ElemXListboxCreate(&m_gui, E_ELEM_LISTBOX, E_PG_FILE, &m_sXListbox,
	  	(gslc_tsRect) { 10, 45, 300, 150 }, E_FONT_SERIFI1, m_pXListboxBuf, 50*2, XLISTBOX_SEL_NONE);	
	  gslc_ElemXListboxSetSize(&m_gui, pElemRef, 5, 1);  
	  gslc_ElemXListboxItemsSetSize(&m_gui, pElemRef, XLISTBOX_SIZE_AUTO, XLISTBOX_SIZE_AUTO);  
	  gslc_ElemXListboxItemsSetTxtMargin(&m_gui, pElemRef, 5, 8);  
	  gslc_ElemXListboxAddItem(&m_gui, pElemRef, ListBoxTxt1);  
	  gslc_ElemXListboxAddItem(&m_gui, pElemRef, ListBoxTxt2);  
	  gslc_ElemXListboxAddItem(&m_gui, pElemRef, ListBoxTxt3);  
	  gslc_ElemXListboxAddItem(&m_gui, pElemRef, ListBoxTxt4);  
	  gslc_ElemXListboxAddItem(&m_gui, pElemRef, ListBoxTxt5);  
	  gslc_ElemXListboxSetSelFunc(&m_gui, pElemRef, &CbListbox);  
	  gslc_ElemSetCol(&m_gui, pElemRef, GSLC_COL_BLUE_DK3, GSLC_COL_GRAY_DK3, GSLC_COL_GREEN_DK1);  
	  gslc_ElemSetFrameEn(&m_gui, pElemRef, true);  
	  gslc_ElemSetTxtCol(&m_gui, pElemRef, GSLC_COL_WHITE);	
	  m_pElemListbox = pElemRef; // Save for quick access
	  
	// -----------------------------------
	// PAGE: E_PG_CSV

	// Create E_BOX11 box
    pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX11,E_PG_CSV,(gslc_tsRect){5,5,310,230});

    // Create E_BOX12 box
    pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX12,E_PG_CSV,(gslc_tsRect){5,5,310,65});
    gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BROWN,GSLC_COL_BLACK,GSLC_COL_BLACK);

	// Create E_BOX13 box
	pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX13,E_PG_CSV,(gslc_tsRect){5,75,310,65});
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BROWN,GSLC_COL_BLACK,GSLC_COL_BLACK);
  
//---------------------------------------------ECU 1 ---------------------------
    // Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){10,10,80,26},
      (char*)"Ecu1 ID",0,E_FONT_SERIFI1);
  
 	// Create GSLC_ID_AUTO text label
 	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){165,10,80,26},
      (char*)"PhyReqID",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){10,40,60,26},
	  (char*)"FunReqID",0,E_FONT_SERIFI1);
	
	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){165,40,40,26},
	  (char*)"Type",0,E_FONT_SERIFI1);

//---------------------------------------------ECU 2 ---------------------------
	// Create GSLC_ID_AUTO text label
	 pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){10,80,60,26},
	   (char*)"Ecu2 ID",0,E_FONT_SERIFI1);
	 
	 // Create GSLC_ID_AUTO text label
	 pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){165,80,80,26},
	   (char*)"PhyReqID",0,E_FONT_SERIFI1);
	 
	 // Create GSLC_ID_AUTO text label
	 pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){10,110,81,26},
	   (char*)"FunReqID",0,E_FONT_SERIFI1);
	 
	 // Create GSLC_ID_AUTO text label
	 pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){165,110,80,26},
	   (char*)"Type",0,E_FONT_SERIFI1);


	// Create GSLC_ID_AUTO text label
	static char mstr20[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){90,10,65,25},
	  mstr20,sizeof(mstr20),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV1 = pElemRef;


	// Create GSLC_ID_AUTO text label
	static char mstr21[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){245,10,65,25},
	  mstr21,sizeof(mstr21),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV2 = pElemRef;


	// Create GSLC_ID_AUTO text label
	static char mstr22[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){90,40,65,25},
	  mstr22,sizeof(mstr22),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV3 = pElemRef;

	// Create GSLC_ID_AUTO text label
	static char mstr23[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){210,40,100,25},
	  mstr23,sizeof(mstr23),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV4 = pElemRef;

#if 1
	// Create GSLC_ID_AUTO text label
	static char mstr24[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){90,80,65,26},
		mstr24,sizeof(mstr24),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV5 = pElemRef;

	// Create GSLC_ID_AUTO text label
	static char mstr25[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){245,80,65,26},
		mstr25,sizeof(mstr25),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV6 = pElemRef;

	// Create GSLC_ID_AUTO text label
	static char mstr26[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){90,110,65,25},
		mstr26,sizeof(mstr26),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV7 = pElemRef;

	// Create GSLC_ID_AUTO text label
	static char mstr27[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){210,110,100,26},
		mstr27,sizeof(mstr27),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV8 = pElemRef;

	// create E_BTN25 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN25,E_PG_CSV,
	(gslc_tsRect){250,200,60,30},(char*)"O.K",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
#endif


	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){10,145,30,26},
	(char*)"File",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){165,145,30,26},
	(char*)"Type",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	static char mstr28[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){45,145,110,25},
		mstr28,sizeof(mstr28),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV9 = pElemRef;

	// Create GSLC_ID_AUTO text label
	static char mstr29[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){210,145,100,26},
		mstr29,sizeof(mstr29),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrCSV10 = pElemRef;

 
	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){10,203,90,26},
	(char*)"SW Version",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	static char mstr30[MAX_STR] = "";
//	mstr30 = SharedMemory.WifiBdSWVer.c_str();
	memcpy(mstr30,SharedMemory.WifiBdSWVer.c_str(),MAX_STR);
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_CSV,(gslc_tsRect){100,203,140,26},
		mstr30,sizeof(mstr30),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);	
	m_pElemTextStrCSV11 = pElemRef;

	
	// -----------------------------------
	// PAGE: E_PG_OPTION

	// Create E_BOX15 box
	pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX15,E_PG_OPTION,(gslc_tsRect){5,5,310,230});

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_OPTION,(gslc_tsRect){10,10,62,20},
	(char*)"Options",0,E_FONT_SERIFI1);

	// Create E_BOX16 box
	pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX16,E_PG_OPTION,(gslc_tsRect){10,40,300,35});
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GREEN,GSLC_COL_BLACK,GSLC_COL_BLACK);

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_OPTION,(gslc_tsRect){20,45,62,26},
	(char*)"STmin",0,E_FONT_SERIFI1);

	// create E_BTN26 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN26,E_PG_OPTION,
	  (gslc_tsRect){95,45,60,25},(char*)"Down",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_YELLOW,GSLC_COL_RED);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	
	// create E_BTN27 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN27,E_PG_OPTION,
	  (gslc_tsRect){245,45,60,25},(char*)"Up",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_YELLOW,GSLC_COL_RED);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);


	// Create GSLC_ID_AUTO text label
	static char mstrOption[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_OPTION,(gslc_tsRect){160,44,80,26},
	mstrOption,sizeof(mstrOption),E_FONT_SERIFB1);

	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrOptionSTmin = pElemRef;

	// create E_BTN28 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN28,E_PG_OPTION,
	(gslc_tsRect){250,200,60,30},(char*)"O.K",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN29 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN29,E_PG_OPTION,
	(gslc_tsRect){10,200,60,30},(char*)"TCAL",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN30 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN30,E_PG_OPTION,
	(gslc_tsRect){80,200,60,30},(char*)"ETC",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN42 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN42,E_PG_OPTION,
	  (gslc_tsRect){150,200,90,30},(char*)"OTA SW",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// -----------------------------------
	// PAGE: E_PG_MAIN2

	// Create E_BOX17 box
	pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX17,E_PG_MAIN2,(gslc_tsRect){5,5,310,230});

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN2,(gslc_tsRect){10,10,82,26},
	(char*)"ECU Ver",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN2,(gslc_tsRect){10,44,50,26},
	(char*)"File Name",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN2,(gslc_tsRect){10,80,82,26},
	(char*)"Down Info",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	static char m2str1[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN2,(gslc_tsRect){100,10,145,30},
	m2str1,sizeof(m2str1),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStr13 = pElemRef;

	// Create GSLC_ID_AUTO text label
	static char m2str2[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN2,(gslc_tsRect){100,45,210,30},
	m2str2,sizeof(m2str2),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStr14 = pElemRef;

	// Create GSLC_ID_AUTO text label
	static char m2str3[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN2,(gslc_tsRect){100,80,145,30},
	m2str3,sizeof(m2str3),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStr15 = pElemRef;

	// Create progress bar E_PROGRESS2 
	pElemRef = gslc_ElemXGaugeCreate(&m_gui,E_PROGRESS2,E_PG_MAIN2,&m_sXGauge[1],
	(gslc_tsRect){10,115,300,20},0,100,0,GSLC_COL_GREEN,false);

	// create E_BTN31 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN31,E_PG_MAIN2,
	(gslc_tsRect){250,10,60,30},(char*)"Ver",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GREEN,GSLC_COL_GREEN,GSLC_COL_GRAY_DK2);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN32 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN32,E_PG_MAIN2,
	(gslc_tsRect){250,80,60,30},(char*)"Down",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_CYAN);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	pElemRefM2Bk1 = pElemRef;

	// create E_BTN33 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN33,E_PG_MAIN2,
	(gslc_tsRect){10,140,60,30},(char*)"Set-Up",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_YELLOW,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN34 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN34,E_PG_MAIN2,
	(gslc_tsRect){75,140,60,30},(char*)"Option",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN35 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN35,E_PG_MAIN2,
	(gslc_tsRect){140,140,60,30},(char*)"Diag",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT2);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN36 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN36,E_PG_MAIN2,
	(gslc_tsRect){205,140,40,30},(char*)"File",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN37 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN37,E_PG_MAIN2,
	(gslc_tsRect){250,140,60,30},(char*)"Reset",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GREEN,GSLC_COL_RED,GSLC_COL_GRAY_LT2);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN38 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN38,E_PG_MAIN2,
    (gslc_tsRect){250,185,60,45},(char*)"DTC-H",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_ORANGE,GSLC_COL_GRAY_LT2);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN39 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN39,E_PG_MAIN2,
	(gslc_tsRect){10,185,60,45},(char*)"DTC-C",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_ORANGE,GSLC_COL_GRAY_LT2);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	
	// create E_BTN40 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN40,E_PG_MAIN2,
	(gslc_tsRect){75,185,60,45},(char*)"DTC-P",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT2);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN41 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN41,E_PG_MAIN2,
    (gslc_tsRect){140,185,105,45},(char*)"-",0,E_FONT_TXT1,&CbBtnCommon);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	
	// Create GSLC_ID_AUTO text label
	static char mstrP[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN2,(gslc_tsRect){140,185,50,45},
	mstrP,sizeof(mstrP),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStr16 = pElemRef;

	// Create GSLC_ID_AUTO text label
	static char mstrH[MAX_STR] = "";
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN2,(gslc_tsRect){195,185,50,45},
	mstrH,sizeof(mstrH),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStr17 = pElemRef;

	// create E_BTN47 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN47,E_PG_MAIN2,
	  (gslc_tsRect){10,80,85,30},(char*)"Down Info",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_ORANGE,GSLC_COL_GRAY_LT2);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	
	// -----------------------------------
	// PAGE: E_PG9

	// Create E_BOX18 box
	pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX18,E_PG9,(gslc_tsRect){5,5,310,230});

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){10,10,136,26},
	(char*)"OTA SW Upgrade",0,E_FONT_SERIFI1);

	// create E_BTN43 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN43,E_PG9,
	(gslc_tsRect){250,200,60,30},(char*)"O.K",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

	// create E_BTN44 button with text label
	static char mstrOTASTARTSTOP[MAX_STR] = "";	
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN44,E_PG9,
	(gslc_tsRect){10,200,60,30},mstrOTASTARTSTOP,sizeof(mstrOTASTARTSTOP),E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
	m_pElemTextStrOTAStartStop = pElemRef;

#if 0
	// create E_BTN45 button with text label
	pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN45,E_PG9,
	(gslc_tsRect){80,200,60,30},(char*)"Stop",0,E_FONT_SERIFI1,&CbBtnCommon);
	gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
#endif

	// Create E_BOX19 box
	pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX19,E_PG9,(gslc_tsRect){5,45,310,150});
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_YELLOW,GSLC_COL_BLACK,GSLC_COL_BLACK);

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){10,50,63,26},
	(char*)"IP Addr",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	static char mstrOTAIP[MAX_STR] = "";	
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){85,50,225,30},
	mstrOTAIP,sizeof(mstrOTAIP),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
    m_pElemTextStrOTAIP = pElemRef;	

	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){10,90,72,26},
	(char*)"SSID/PW",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	static char mstrOTASSID[MAX_STR] = "";	
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){85,90,120,30},
	mstrOTASSID,sizeof(mstrOTASSID),E_FONT_TXT1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
    m_pElemTextStrOTASSIDPW = pElemRef;	

	// Create GSLC_ID_AUTO text label
	static char mstrOTAPW[MAX_STR] = "";	
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){210,90,100,30},
	mstrOTAPW,sizeof(mstrOTAPW),E_FONT_TXT1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_GRAY_LT1);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
    m_pElemTextStrOTAPW = pElemRef;	
	
	// Create GSLC_ID_AUTO text label
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){10,130,60,26},
	(char*)"F-Name",0,E_FONT_SERIFI1);

	// Create GSLC_ID_AUTO text label
	static char mstrOTANAME[MAX_STR] = "";	
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){85,130,225,30},
	mstrOTANAME,sizeof(mstrOTANAME),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_ORANGE,GSLC_COL_BLACK,GSLC_COL_BLACK);
	gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
    m_pElemTextStrOTAFNAME = pElemRef;	

	// Create progress bar E_PROGRESS3 
	pElemRef = gslc_ElemXGaugeCreate(&m_gui,E_PROGRESS3,E_PG9,&m_sXGauge[2],
	(gslc_tsRect){85,170,225,20},0,100,0,GSLC_COL_GREEN,false);
	gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLACK,GSLC_COL_BLACK);

	// Create GSLC_ID_AUTO text label
	static char mstrOTAPER[MAX_STR] = "";	
	pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG9,(gslc_tsRect){10,165,60,25},
	mstrOTAPER,sizeof(mstrOTAPER),E_FONT_SERIFI1);
	gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
    m_pElemTextStrOTAPER = pElemRef;	
	//<InitGUI !End!>


return true;

}

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Common Button callback

bool TftGuiLCDFunClass::CbListbox(void* pvGui, void* pvElemRef, int16_t nSelId)
{ 
	gslc_tsGui*     pGui = (gslc_tsGui*)(pvGui);  
	gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef); 
	gslc_tsElem*    pElem = gslc_GetElemFromRef(pGui, pElemRef);  

	if (pElemRef == NULL) 
	{    
		return false;  
	}  // Update the status message with the selection  
	if (nSelId == XLISTBOX_SEL_NONE) 
	{
		snprintf(TftGuiLCDFun.ListBoxTxt, LISTTXTLENGTH, "NONE");  
	}
	else 
	{ 
		gslc_ElemXListboxGetItem(&TftGuiLCDFun.m_gui, pElemRef, nSelId, TftGuiLCDFun.ListBoxTxt, LISTTXTLENGTH);  
		GSLC_DEBUG_PRINT("CbListbox Selected nSelId[%d] -- %s\n", nSelId,TftGuiLCDFun.ListBoxTxt);	   
		TftGuiLCDFun.SelectedID = nSelId;

		memcpy(SharedMemory.FileName,TftGuiLCDFun.ListBoxTxt,20);
//		DownLoadFileName[0] = SharedMemory.downloadfileName;
//	    SharedMemory.downloadfileName = TftGuiLCDFun.ListBoxTxt;
		GSLC_DEBUG_PRINT("[SharedMemory.FileName] -- %s\n", SharedMemory.FileName);	   
	} 

	gslc_ElemSetTxtStr(&TftGuiLCDFun.m_gui, TftGuiLCDFun.m_pElemListBoxTextStr, TftGuiLCDFun.ListBoxTxt);  
	return true;
}

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Common Button callback
bool TftGuiLCDFunClass::CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
   gslc_tsElem* pElem = pElemRef->pElem;

   if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_BTN1:
	  case E_BTN31:
        //TODO- Replace with button handling code
        TftGuiLCDFun.VersionCheckSW();
       	TftGuiLCDFun.InitParamSet = false;
        break;
      case E_BTN2:
	  case E_BTN32:
        //TODO- Replace with button handling code
		if(WiFiFnc.WifiWorking == 0)
		{
			switch(SharedMemory.File_CAN_TYPE)
			{
			case 1:  // HS CAN
			    if(RFileFnc.HSCANBTNState)
		    	{
					ReprogmmingData.ReprogramInitVariable(1);
					Serial.printf("[E_BTN2-SharedMemory.File_CAN_TYPE] %d [ReprogData.reprogStartStop] %d \n",SharedMemory.File_CAN_TYPE,ReprogData.reprogStartStop);
					ReprogData.timerTick = 0;
		    	}
				else
				{
					Serial.printf("[RFileFnc.HSCANBTNState] %d [E_BTN32-SharedMemory.File_CAN_TYPE] %d [ReprogData.reprogStartStop] %d \n",RFileFnc.HSCANBTNState,SharedMemory.File_CAN_TYPE,ReprogData.reprogStartStop);
					ReprogData.timerTick = 0;
				}
				break;
			case 2:  // CAN FD
				ReprogmmingDataFD.ReprogramInitVariable(1);
				Serial.printf("[E_BTN2-SharedMemory.File_CAN_TYPE] %d [ReprogDataFd.reprogStartStop] %d \n",SharedMemory.File_CAN_TYPE,ReprogDataFd.reprogStartStop);
				ReprogDataFd.timerTick = 0;
				break;
			}


		}
		else{
			
		}
		Serial.printf("[E_BTN2] %d  \n",WiFiFnc.WifiWorking);
        break;
      case E_BTN3:
	  case E_BTN33:
        //TODO- Replace with button handling code
		gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG2);
        break;
      case E_BTN4:
	  case E_BTN34:
        //TODO- Replace with button handling code
	    gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_OPTION);
        break;
      case E_BTN5:
//	  case E_BTN37:
        //TODO- Replace with button handling code
		if(WiFiFnc.WifiWorking == 0){
			TftLCDFun.progresspercent = 0; //(100 - (ReprogData.reprogHexSize * 100 / ReprogData.reprogFileHexSize));
			TftGuiLCDFun.ProgressBarGage(TftLCDFun.progresspercent,0);
		}
		else{
			
		}
		WiFiFnc.WifiWorking = !WiFiFnc.WifiWorking;
		TftGuiLCDFun.ChangeButtonSet();
		Serial.printf("[E_BTN5] %d  \n",WiFiFnc.WifiWorking);
        break;
      case E_BTN6:
	  case E_BTN35:
        //TODO- Replace with button handling code  //E_PG_DIAG
		gslc_ElemSetTxtStr(&TftGuiLCDFun.m_gui,TftGuiLCDFun.m_pElemTextStr9,"");
		gslc_ElemSetTxtStr(&TftGuiLCDFun.m_gui,TftGuiLCDFun.m_pElemTextStr10,"");
		gslc_ElemSetTxtStr(&TftGuiLCDFun.m_gui,TftGuiLCDFun.m_pElemTextStr11,"");
		gslc_ElemSetTxtStr(&TftGuiLCDFun.m_gui,TftGuiLCDFun.m_pElemTextStr12,"");

		gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_DIAG);
        break;
      case E_BTN7:
        //TODO- Replace with button handling code
       	TftGuiLCDFun.InitParamSet = false;
		TftGuiLCDFun.EEPSaveParam();
        if(WiFiFnc.WifiWorking == 0){
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN2);
        }else{
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN);
       	}
        break;
      case E_BTN8:
        //TODO- Replace with button handling code
        if(WiFiFnc.WifiWorking == 0){
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN2);
        }else{
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN);
       	}
		
        break;		
      case E_BTN9:
        //TODO- Replace with button handling code
		Serial.printf("Press Button Number 9 - SW Version check \n");     
        TftGuiLCDFun.VersionCheckSWDiag();
       	TftGuiLCDFun.InitParamSet = false;
        break;
      case E_BTN10:
        //TODO- Replace with button handling code
        if(WiFiFnc.WifiWorking == 0){
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN2);
        }else{
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN);
    	}
        break;
	  case E_BTN11:
		//TODO- Replace with button handling code
		Serial.printf("Press Button Number 11 - HW Version check \n");	   
		TftGuiLCDFun.VersionCheckHW();
		TftGuiLCDFun.InitParamSet = false;
		break;
	  case E_BTN12:
		//TODO- Replace with button handling code
		Serial.printf("Press Button Number 12 - SW SUP Version check \n");	   
		TftGuiLCDFun.VersionCheckSW_SUP();
		TftGuiLCDFun.InitParamSet = false;
		break;
	  case E_BTN13:
		//TODO- Replace with button handling code
		Serial.printf("Press Button Number 13 - HW SUP Version check \n");	   
		TftGuiLCDFun.VersionCheckHW_SUP();
		TftGuiLCDFun.InitParamSet = false;
		break;
	  case E_BTN14:
	    //TODO- Replace with button handling code
		break;
	  case E_BTN15:
		//TODO- Replace with button handling code
		break;
	  case E_BTN16:
		//TODO- Replace with button handling code
		break;
	  case E_BTN17:
		//TODO- Replace with button handling code
		break;
	  case E_BTN18:
		//TODO- Replace with button handling code
		break;
	  case E_BTN19:
		//TODO- Replace with button handling code
		break;
	  case E_BTN20:
		//TODO- Replace with button handling code
		break;
	  case E_BTN21:
	  case E_BTN36:
		//TODO- Replace with button handling code
		gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_FILE);
		break;
	  case E_BTN22: //2020.05.07
	    //TODO- Replace with button handling code
	    
//		DownLoadFileName[0] = SharedMemory.FileName.downloadfileName;
//		RFileFnc.DownLoadFileName[0].toCharArray(SharedMemory.FileName,TXTLENGTH);
		SharedMemory.downloadfileName = RFileFnc.DownLoadFileName[TftGuiLCDFun.SelectedID] ; //2020.05.08 MODIFY BY YONGWOOKWI
		RFileFnc.InitRepFileSetup(4);
		TftGuiLCDFun.InitParamSet = false;
		
//		gslc_ElemSetTxtStr(&TftGuiLCDFun.m_gui,TftGuiLCDFun.m_pElemTextStr1,SharedMemory.FileName);
//		gslc_ElemSetTxtStr(&TftGuiLCDFun.m_gui,TftGuiLCDFun.m_pElemTextStr14,SharedMemory.FileName);

        if(WiFiFnc.WifiWorking == 0){
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN2);
        }else{
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN);
       	}

	    break;
	  case E_BTN23:
	    //TODO- Replace with button handling code
	    RFileFnc.FileRemove(TftGuiLCDFun.ListBoxTxt);
		// Configure Rep File Init Setup ...
		RFileFnc.InitRepFileSetup(1);
		TftGuiLCDFun.ListBoxUpdate();
	    break;
	  case E_BTN24:
		//TODO- Replace with button handling code
		TftGuiLCDFun.CsvParamView();
		gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_CSV);
		break;
	  case E_BTN25:
	    //TODO- Replace with button handling code
        if(WiFiFnc.WifiWorking == 0){
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN2);
        }else{
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN);
       	}
	    break;
		
	  case E_BTN26:  // Push the button "-"
		//TODO- Replace with button handling code
		if(SharedMemory.STmin <= 1) SharedMemory.STmin = 1;
		else 						SharedMemory.STmin--;
		TftGuiLCDFun.InitParamSet = false;
		break;
	  case E_BTN27: // Push the button "+"
		//TODO- Replace with button handling code
		if(SharedMemory.STmin >= 10) SharedMemory.STmin = 10;
		else 						 SharedMemory.STmin++;
		TftGuiLCDFun.InitParamSet = false;
		break;

	  case E_BTN28:
  		//TODO- Replace with button handling code
		TftGuiLCDFun.EEPSaveParam();
        if(WiFiFnc.WifiWorking == 0){
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN2);
        }else{
	  		gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN);
       	}
  		break;
	  case E_BTN29:
		//TODO- Replace with button handling code
		break;
	  case E_BTN30:
		//TODO- Replace with button handling code
		break;
	  case E_BTN37:
		  //TODO- Replace with button handling code
		  if(WiFiFnc.WifiWorking == 0)
		  {
			  TftLCDFun.progresspercent = 0; //(100 - (ReprogData.reprogHexSize * 100 / ReprogData.reprogFileHexSize));
			  TftGuiLCDFun.ProgressBarGage(TftLCDFun.progresspercent,0);
			   ReprogmmingDataFD.vfClearAllEnv();			  
			   ReprogmmingData.vfClearAllEnv();		

			   ReprogDataFd.stepLock 		= 0;
	           ReprogDataFd.reprogStartStop = 0;
	           ReprogData.reprogStartStop 	= 0;
			   
			   SharedMemory.DtcPNum = 0xFF;
			   SharedMemory.DtcHNum = 0xFF;

			   CanFnc.DiagModeSel   = CANDIAG_NONE;
			   CanFDFun.DiagModeSel = CANDIAG_NONE;

			   TftGuiLCDFun.InitParamSet = false;
		  }
		  else{
			  
		  }
//		  WiFiFnc.WifiWorking = !WiFiFnc.WifiWorking;
		  TftGuiLCDFun.ChangeInitButtonSet();
		  Serial.printf("[E_BTN37] %d  \n",WiFiFnc.WifiWorking);
		  break;
	  case E_BTN39:
  		//TODO- Replace with button handling code
#if 0  		
		if(WiFiFnc.WifiWorking == 0)
		{
			switch(SharedMemory.File_CAN_TYPE)
			{
			case 1:  // HS CAN
				ReprogmmingData.DTC_Clear_Functional();
//				Serial.printf("[E_BTN2-SharedMemory.File_CAN_TYPE] %d [ReprogData.reprogStartStop] %d \n",SharedMemory.File_CAN_TYPE,ReprogData.reprogStartStop);
				break;
			case 2:  // CAN FD
				ReprogmmingDataFD.DTC_Clear_Functional();
				break;
			}


		}
		else{
			
		}
#else
//		SharedMemory.DtcHNum = 0;
		TftGuiLCDFun.CheckDTC_ALLCnt_SUP();//CheckDTC_ALL_SUP
		TftGuiLCDFun.InitParamSet = false;
		Serial.printf("Press Button Number 39 - DTC Reading Button Press \n");	  
#endif
		Serial.printf("[E_BTN39-DTC-C] [File_CAN_TYPE] %d  \n",SharedMemory.File_CAN_TYPE);
  		break;
		
	  case E_BTN38:
		//TODO- Replace with button handling code
		Serial.printf("Press Button Number 38 - DTC Reading Button Press \n");	  
		SharedMemory.DtcHNum = 0xFF;
		TftGuiLCDFun.CheckDTC_ConfirmingCnt_SUP();//CheckDTC_ALL_SUP
		TftGuiLCDFun.InitParamSet = false;
		break;
	  case E_BTN40:
		//TODO- Replace with button handling code
		Serial.printf("Press Button Number 40 - DTC Reading Button Press \n");	   
		SharedMemory.DtcPNum = 0xFF;
		TftGuiLCDFun.CheckDTC_PendingCnt_SUP();//CheckDTC_ALL_SUP
		TftGuiLCDFun.InitParamSet = false;
		break;
	  case E_BTN41:
		//TODO- Replace with button handling code
	    gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_OPTION);
		break;
	 case E_BTN42:
	    //TODO- Replace with button handling code
//		TftGuiLCDFun.WifiSetting = 1;
//		RFileFnc.WifiSettingdState = TftGuiLCDFun.WifiSetting;

		SharedMemory.OTAMenuOption = 1;		
	    gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG9);
		Serial.printf("Press Button Number 42 - OTA SW UPDATE Button Press \n");	   
		TftGuiLCDFun.InitParamSet = false;
		break;
	 case E_BTN43:
	    //TODO- Replace with button handling code
        if(WiFiFnc.WifiWorking == 0){
			gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN2);
        }else{
	  		gslc_SetPageCur(&TftGuiLCDFun.m_gui,E_PG_MAIN);
       	}
//		TftGuiLCDFun.WifiSetting = 0;
//		RFileFnc.WifiSettingdState = TftGuiLCDFun.WifiSetting;

		SharedMemory.OTAMenuOption = 0;
		Serial.printf("Press Button Number 43 - OTA SW UPDATE O.K Button Press \n");	   
		TftGuiLCDFun.InitParamSet = false;
		SharedMemory.OTAMStart  = 0;
		SharedMemory.OTAConnect = 0;
	    break;
	case E_BTN44:
	    //TODO- Replace with button handling code
	    if(SharedMemory.OTAConnect) SharedMemory.OTAMStart = !SharedMemory.OTAMStart;
		Serial.printf("Press Button Number 44 - OTA START/STO Button Press [SharedMemory.OTAMStart] %d\n",SharedMemory.OTAMStart);
		TftGuiLCDFun.InitParamSet = false;
		break;

	case E_BTN46:
		//TODO- Replace with button handling code
		break;
	
	case E_BTN47:
	    //TODO- Replace with button handling code
		if(RFileFnc.CanFDSettingdState == 1)
		{
			RFileFnc.CanFDSettingdState = 0;
			RFileFnc.HsCanSettingdState = 1;
			
//			RFileFnc.CanFDSettingdState = TftGuiLCDFun.CANtypeSetting;
//			GSLC_DEBUG_PRINT("CanFDSettingdState Value[E_RADIO3] [%d] selected\n", TftGuiLCDFun.CANtypeSetting);	   
		}
		else if(RFileFnc.CanFDSettingdState == 0)
		{
			RFileFnc.CanFDSettingdState = 1;
			RFileFnc.HsCanSettingdState = 0;
			
//			RFileFnc.CanFDSettingdState = TftGuiLCDFun.CANtypeSetting;
//			GSLC_DEBUG_PRINT("CanFDSettingdState Value[E_RADIO4] [%d] selected\n", TftGuiLCDFun.CANtypeSetting);	   
		}
		
       	TftGuiLCDFun.InitParamSet = false;
		TftGuiLCDFun.EEPSaveParam();

		if(WiFiFnc.WifiWorking == 0)
		{
			 TftLCDFun.progresspercent = 0; //(100 - (ReprogData.reprogHexSize * 100 / ReprogData.reprogFileHexSize));
			 TftGuiLCDFun.ProgressBarGage(TftLCDFun.progresspercent,0);
			 ReprogmmingDataFD.vfClearAllEnv(); 			
			 ReprogmmingData.vfClearAllEnv();	  

			 ReprogDataFd.stepLock		  = 0;
			 ReprogDataFd.reprogStartStop = 0;
			 ReprogData.reprogStartStop   = 0;
			 
			 SharedMemory.DtcPNum = 0xFF;
			 SharedMemory.DtcHNum = 0xFF;

			 CanFnc.DiagModeSel   = CANDIAG_NONE;
			 CanFDFun.DiagModeSel = CANDIAG_NONE;

			 TftGuiLCDFun.InitParamSet = false;
		}

		
		break;
		
//<Button Enums !End!>
      default:
        break;
    }
  }
  return true;
}   

// Scanner drawing callback function
// - This is called when E_ELEM_SCAN is being rendered
bool  TftGuiLCDFunClass::CbDrawScanner(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw)
{
 	 int nInd;
	
	  // Typecast the parameters to match the GUI and element types
	  gslc_tsGui*	  pGui		= (gslc_tsGui*)(pvGui);
	  gslc_tsElemRef* pElemRef	= (gslc_tsElemRef*)(pvElemRef);
	  gslc_tsElem*	  pElem 	= pElemRef->pElem;
	
	  // Create shorthand variables for the origin
#if 1	  
	  int16_t  nX = pElem->rElem.x;
	  int16_t  nY = pElem->rElem.y;
#else
	  int16_t  nX = pElem->rElem.x + 5;
	  int16_t  nY = pElem->rElem.y;
#endif
	
	  // Draw the background
	  gslc_tsRect rInside = pElem->rElem;
	  rInside = gslc_ExpandRect(rInside,-1,-1);
	  gslc_DrawFillRect(pGui,rInside,pElem->colElemFill);
	
	  // Enable localized clipping
	  gslc_SetClipRect(pGui,&rInside);
	
	  //TODO - Add your drawing graphic primitives
	
	  // Disable clipping region
	  gslc_SetClipRect(pGui,NULL);
	
	  // Draw the frame
	  gslc_DrawFrameRect(pGui,pElem->rElem,pElem->colElemFrame);
	
	  // Clear the redraw flag
	  gslc_ElemSetRedraw(&TftGuiLCDFun.m_gui,pElemRef,GSLC_REDRAW_NONE);
	
	  return true;

}

// Callback function for when a slider's position has been updated
bool  TftGuiLCDFunClass::CbSlidePos(void* pvGui,void* pvElemRef,int16_t nPos)
{
  
 	return true;
  
}
//<Tick Callback !Start!>
//<Tick Callback !End!>

// Checkbox / radio callbacks
// - Creating a callback function is optional, but doing so enables you to
//   detect changes in the state of the elements.

bool TftGuiLCDFunClass::CbCheckbox(void* pvGui, void* pvElemRef, int16_t nSelId, bool bChecked)
{
	gslc_tsGui*     pGui      = (gslc_tsGui*)(pvGui);  
	gslc_tsElemRef* pElemRef  = (gslc_tsElemRef*)(pvElemRef);  
	gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);  

	if (pElemRef == NULL){   
		return false;  
	}  // Determine which element issued the callback  

	switch (pElem->nId) 
	{   
#if 0
		case E_CHECK1:      
			 TftGuiLCDFun.checkbox1 = bChecked;
			 RFileFnc.HsCanSettingdState = TftGuiLCDFun.checkbox1;
			 RFileFnc.CanFDSettingdState = 0;
			 GSLC_DEBUG_PRINT("Callback: Check1[ID=%d] state=%u\n", pElem->nId,bChecked);      
//			 TftGuiLCDFun.InitParamSet = false;
			 break;
		case E_CHECK2:		
			 TftGuiLCDFun.checkbox2 = bChecked;
			 RFileFnc.CanFDSettingdState = TftGuiLCDFun.checkbox2;
			 RFileFnc.HsCanSettingdState = 0;
			 GSLC_DEBUG_PRINT("Callback: Check2[ID=%d] state=%u\n", pElem->nId,bChecked);      
//			 TftGuiLCDFun.InitParamSet = false;
			 break;
#endif			 
		case E_CHECK3:		
			 TftGuiLCDFun.checkbox3 = bChecked;
			 RFileFnc.WifiOnOffdState = TftGuiLCDFun.checkbox3;
			 GSLC_DEBUG_PRINT("Callback: Check3[ID=%d] state=%u\n", pElem->nId,bChecked);      
			 break;    
		case E_CHECK4: 	 
			 TftGuiLCDFun.checkbox4 = bChecked;
			 RFileFnc.DtcAllClearState = TftGuiLCDFun.checkbox4;
			 RFileFnc.HSCANBTNState    = RFileFnc.DtcAllClearState;
			 GSLC_DEBUG_PRINT("Callback: Check4[ID=%d] state=%u\n", pElem->nId,bChecked);		
			 break;	
		case E_RADIO1:
		case E_RADIO2:
			
//		case E_RADIO3:
//		case E_RADIO4:		
			
			// For the radio buttons, determine which ID is currently selected (nSelId)      
			// - Note that this may not always be the same as the element that      
			//   issued the callback (pElem->nId)      
			// - A return value of GSLC_ID_NONE indicates that no radio buttons      
			//   in the group are currently selected      
			if (nSelId == GSLC_ID_NONE) {        
				GSLC_DEBUG_PRINT("Callback: Radio[ID=NONE] selected\n", "");      
			} else {  
				GSLC_DEBUG_PRINT("Callback: Radio[ID=%d] selected\n", nSelId);     
				if(nSelId == E_RADIO1){
					TftGuiLCDFun.WifiSetting = 1;
					RFileFnc.WifiSettingdState = TftGuiLCDFun.WifiSetting;
					GSLC_DEBUG_PRINT("WifiSetting Value[E_RADIO1] [%d] selected\n", TftGuiLCDFun.WifiSetting);	   
				}else if(nSelId == E_RADIO2){
					TftGuiLCDFun.WifiSetting = 0;
					RFileFnc.WifiSettingdState = TftGuiLCDFun.WifiSetting;
					GSLC_DEBUG_PRINT("WifiSetting Value[E_RADIO2] [%d] selected\n", TftGuiLCDFun.WifiSetting);	   
				}
#if 0				
				else if(nSelId == E_RADIO3){
					TftGuiLCDFun.CANtypeSetting = 0;
					RFileFnc.CanFDSettingdState = TftGuiLCDFun.CANtypeSetting;
					GSLC_DEBUG_PRINT("CanFDSettingdState Value[E_RADIO3] [%d] selected\n", TftGuiLCDFun.CANtypeSetting);	   
				}else if(nSelId == E_RADIO4){
					TftGuiLCDFun.CANtypeSetting = 1;
					RFileFnc.CanFDSettingdState = TftGuiLCDFun.CANtypeSetting;
					GSLC_DEBUG_PRINT("CanFDSettingdState Value[E_RADIO4] [%d] selected\n", TftGuiLCDFun.CANtypeSetting);	   
				}
#endif				
			} 
			break;    
		default:     
			break;  
	}	// switch 
	return true;
}


bool TftGuiLCDFunClass::CbCheckboxCAN(void* pvGui, void* pvElemRef, int16_t nSelId, bool bChecked)
{
	gslc_tsGui*     pGui      = (gslc_tsGui*)(pvGui);  
	gslc_tsElemRef* pElemRef  = (gslc_tsElemRef*)(pvElemRef);  
	gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);  

	if (pElemRef == NULL){   
		return false;  
	}  // Determine which element issued the callback  

	switch (pElem->nId) 
	{   
		case E_RADIO3:
		case E_RADIO4:		
			// For the radio buttons, determine which ID is currently selected (nSelId)      
			// - Note that this may not always be the same as the element that      
			//   issued the callback (pElem->nId)      
			// - A return value of GSLC_ID_NONE indicates that no radio buttons      
			//   in the group are currently selected      
			if (nSelId == GSLC_ID_NONE) {        
				GSLC_DEBUG_PRINT("Callback: Radio[ID=NONE] selected\n", "");      
			} else {  
				GSLC_DEBUG_PRINT("Callback: Radio[ID=%d] selected\n", nSelId);     
				if(nSelId == E_RADIO3){
					TftGuiLCDFun.CANtypeSetting = 0;
					RFileFnc.CanFDSettingdState = TftGuiLCDFun.CANtypeSetting;
					GSLC_DEBUG_PRINT("CanFDSettingdState Value[E_RADIO3] [%d] selected\n", TftGuiLCDFun.CANtypeSetting);	   
				}else if(nSelId == E_RADIO4){
					TftGuiLCDFun.CANtypeSetting = 1;
					RFileFnc.CanFDSettingdState = TftGuiLCDFun.CANtypeSetting;
					GSLC_DEBUG_PRINT("CanFDSettingdState Value[E_RADIO4] [%d] selected\n", TftGuiLCDFun.CANtypeSetting);	   
				}
			} 
			break;
		default:     
			break;  
	}	// switch 
	return true;
}


void TftGuiLCDFunClass::InitValueSet()
{
	char	acTxt1[TXTLENGTH];
	char	acTxt2[TXTLENGTH];
	char	acTxt3[TXTLENGTH];
	char	acTxt4[TXTLENGTH];
	char	acTxt5[TXTLENGTH];
	char	acTxt6[TXTLENGTH];
	char	acTxt7[TXTLENGTH];
	char	acTxt8[TXTLENGTH];

	char	acTxt10[TXTLENGTH];
	char	acTxt11[TXTLENGTH];
	char	acTxt12[TXTLENGTH];
	
	char	acTxtOut[TXTLENGTH];

	char	OptTxtOut[TXTLENGTH];

	char	DTCTxtP[TXTLENGTH];
	char	DTCTxtH[TXTLENGTH];

	char	OTAIP[TXTLENGTH];
	char	OTASSID[TXTLENGTH];
	char	OTAPW[TXTLENGTH];
	char	OTAFILE[TXTLENGTH];


	TSizeAP  = strlen(APMODE);  // 5
	TSizeSTA = strlen(STAMODE); // 6 

	TDTCP = strlen(DTCP);  // 5
	TDTCH = strlen(DTCH); // 6 
	
#if 0
	if(TftGuiLCDFun.SelParamSet && (TftGuiLCDFun.NextSelParamSet == 0))
	{
		TftGuiLCDFun.NextSelParamSet = TftGuiLCDFun.SelParamSet;
		TftGuiLCDFun.SelParamSet = 0;
		InitParamSet = 0;
	}	
	else
	{
		InitParamSet = 1;
	}
#endif

	if(InitParamSet) return ;
	InitParamSet = true;

#if 0	
//  TftGuiLCDFun.SelectedID
	RFileFnc.DownLoadFileName[0].toCharArray(acTxt1, TXTLENGTH); 
	GSLC_DEBUG_PRINT("[DownLoadFileName-acTxt1] [%s]\n", acTxt1);	   
#else
	RFileFnc.DownLoadFileName[TftGuiLCDFun.SelectedID].toCharArray(acTxt1, TXTLENGTH); 
	GSLC_DEBUG_PRINT("[DownLoadFileName-acTxt1] [%s]\n", acTxt1);	   
#endif

	if(acTxt1==NULL){
		snprintf(acTxt1,TXTLENGTH,"%s","There is no File");
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr1,acTxt1);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr14,acTxt1);
	}else{
		snprintf(acTxt1,TXTLENGTH,"%s",acTxt1);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr1,acTxt1);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr14,acTxt1);
	}

	if(CanFnc.bCompleteCANMessage)
	{
		memset(acTxt2,0x00,TXTLENGTH);
		for(int i=0;i<CanFnc.DLC;i++) acTxt2[i] = CanFnc.InBuffer[i];
//		memcpy(acTxt2,CanFnc.InBuffer, strlen(CanFnc.InBuffer)); 
		snprintf(acTxt2,TXTLENGTH, "%s",acTxt2);   // char number : 19...

		GSLC_DEBUG_PRINT("[CanFnc.DiagModeSel] [%d]\n", CanFnc.DiagModeSel);	   

		if(CanFnc.DiagModeSel == CANDIAG_SWVER){
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr2,acTxt2);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr13,acTxt2);
		}else if(CanFnc.DiagModeSel == CANDIAG_SWVER_DIAG){ 
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr9,acTxt2);
		}
	}
	else if(CanFDFun.bCompleteCANMessage)
	{
		memset(acTxt2,0x00,TXTLENGTH);
		for(int i=0;i<CanFDFun.DLC;i++) acTxt2[i] = CanFDFun.InBuffer[i];
//		memcpy(acTxt2,CanFDFun.InBuffer, strlen(CanFDFun.InBuffer)); 
		snprintf(acTxt2,TXTLENGTH, "%s",acTxt2);   // char number : 19...

		GSLC_DEBUG_PRINT("[CanFDFun.DiagModeSel] [%d]\n", CanFDFun.DiagModeSel);	   

		if(CanFDFun.DiagModeSel == CANDIAG_SWVER){
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr2,acTxt2);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr13,acTxt2);
		}else if(CanFDFun.DiagModeSel == CANDIAG_SWVER_DIAG){ 
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr9,acTxt2);
		}
	}
	else
	{
		memset(acTxt2,0x00,TXTLENGTH);
		snprintf(acTxt2,TXTLENGTH,"Version...");   // char number : 19...
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr2,acTxt2);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr13,acTxt2);
	}

	memset(acTxt3,0x00,TXTLENGTH);

	if(RFileFnc.CanFDSettingdState == 0)
	{	
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr15,GSLC_COL_RED); //GSLC_COL_ORANGE
		snprintf(acTxt3,TXTLENGTH,"[HS]START.....");   // char number : 19...
	}
	else{
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr15,GSLC_COL_ORANGE); //GSLC_COL_ORANGE
		snprintf(acTxt3,TXTLENGTH,"[FD]START.....");   // char number : 19...
	}

	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr3,acTxt3);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr15,acTxt3);

//*****************************************************************
///////////////////////////////////////
/// Set Up page  ....
///////////////////////////////////////
#if 1
	if(RFileFnc.WifiSettingdState)
	{
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO1,true);
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO2,false);
	}
	else
	{
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO1,false);
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO2,true);
	}
#endif

#if 0
	if(RFileFnc.CanFDSettingdState == 0)
	{
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO3,true);
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO4,false);
	}
	else
	{
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO3,false);
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO4,true);
	}
#else
	if(RFileFnc.CanFDSettingdState == 0)
	{
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO3,true);
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO4,false);
	}
	else
	{
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO3,false);
		gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckRadiO4,true);
	}
#endif

	if(RFileFnc.WifiOnOffdState)	gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckBox3,true);
	else							gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckBox3,false);
		
	if(RFileFnc.DtcAllClearState)	gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckBox4,true);
	else							gslc_ElemXCheckboxSetState(&m_gui,pElemRefCheckBox4,false);
	
//*****************************************************************

///////////////////////////////////////
/// Diag page  ....
///////////////////////////////////////

	if(CanFnc.bCompleteCANMessage)
	{
		switch(CanFnc.DiagModeSel)
		{
		case CANDIAG_NONE:
		case CANDIAG_SWVER:
		case CANDIAG_SWVER_DIAG:  // button 9
			 break;
		case CANDIAG_HWVER:		  // button 11 	m_pElemTextStr10 = pElemRef;
			 memset(acTxt10,0x00,TXTLENGTH);
			 for(int i=0;i<CanFnc.DLC;i++) acTxt10[i] = CanFnc.InBuffer[i];
			 snprintf(acTxt10,TXTLENGTH, "%s",acTxt10);   // char number : 19...
			 GSLC_DEBUG_PRINT("[CanFnc.DiagModeSel] [%d]\n", CanFnc.DiagModeSel);	   
			 gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr10,acTxt10);
			 break;
		case CANDIAG_SWVER_SUP:	  // button 12  m_pElemTextStr11
			 memset(acTxt11,0x00,TXTLENGTH);
			 for(int i=0;i<CanFnc.DLC;i++) acTxt11[i] = CanFnc.InBuffer[i];
			 snprintf(acTxt11,TXTLENGTH, "%s",acTxt11);   // char number : 19...
			 GSLC_DEBUG_PRINT("[CanFnc.DiagModeSel] [%d]\n", CanFnc.DiagModeSel);	   
			 gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr11,acTxt11);
			 break;
		case CANDIAG_HWVER_SUP:   // button 13  m_pElemTextStr12
			 memset(acTxt12,0x00,TXTLENGTH);
			 for(int i=0;i<CanFnc.DLC;i++) acTxt12[i] = CanFnc.InBuffer[i];
			 snprintf(acTxt12,TXTLENGTH, "%s",acTxt12);   // char number : 19...
			 GSLC_DEBUG_PRINT("[CanFnc.DiagModeSel] [%d]\n", CanFnc.DiagModeSel);	   
			 gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr12,acTxt12);
		 	 break;
		case CANDIAG_DTCSTATUSCNT_P:   // button DTC-P
			 SharedMemory.DtcPNum = CanFnc.InBuffer[1];
			 GSLC_DEBUG_PRINT("[SharedMemory.DtcPNum] [%d]\n", SharedMemory.DtcPNum);	   
			 CanFnc.DiagModeSel = CANDIAG_NONE;
			 break;
		case CANDIAG_DTCSTATUSCNT_C:   // button DTC-H
			 SharedMemory.DtcHNum = CanFnc.InBuffer[1];
			 GSLC_DEBUG_PRINT("[SharedMemory.DtcHNum] [%d]\n", SharedMemory.DtcHNum);	   
			 CanFnc.DiagModeSel = CANDIAG_NONE;
			 break;			 
			
		}
	}
	else if(CanFDFun.bCompleteCANMessage)
	{
		switch(CanFDFun.DiagModeSel)
		{
		case CANDIAG_NONE:
		case CANDIAG_SWVER:
		case CANDIAG_SWVER_DIAG:  // button 9
			 break;
		case CANDIAG_HWVER:		  // button 11 	m_pElemTextStr10 = pElemRef;
			 memset(acTxt10,0x00,TXTLENGTH);
			 for(int i=0;i<CanFDFun.DLC;i++) acTxt10[i] = CanFDFun.InBuffer[i];
			 snprintf(acTxt10,TXTLENGTH, "%s",acTxt10);   // char number : 19...
			 GSLC_DEBUG_PRINT("[CanFDFun.DiagModeSel] [%d]\n", CanFDFun.DiagModeSel);	   
			 gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr10,acTxt10);
			 break;
		case CANDIAG_SWVER_SUP:	  // button 12  m_pElemTextStr11
			 memset(acTxt11,0x00,TXTLENGTH);
			 for(int i=0;i<CanFDFun.DLC;i++) acTxt11[i] = CanFDFun.InBuffer[i];
			 snprintf(acTxt11,TXTLENGTH, "%s",acTxt11);   // char number : 19...
			 GSLC_DEBUG_PRINT("[CanFDFun.DiagModeSel] [%d]\n", CanFDFun.DiagModeSel);	   
			 gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr11,acTxt11);
			 break;
		case CANDIAG_HWVER_SUP:   // button 13  m_pElemTextStr12
			 memset(acTxt12,0x00,TXTLENGTH);
			 for(int i=0;i<CanFDFun.DLC;i++) acTxt12[i] = CanFDFun.InBuffer[i];
			 snprintf(acTxt12,TXTLENGTH, "%s",acTxt12);   // char number : 19...
			 GSLC_DEBUG_PRINT("[CanFnc.DiagModeSel] [%d]\n", CanFDFun.DiagModeSel);	   
			 gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr12,acTxt12);
		 	 break;
		case CANDIAG_DTCSTATUSCNT_P:   // button DTC-P
			 SharedMemory.DtcPNum = CanFDFun.InBuffer[1];
			 GSLC_DEBUG_PRINT("[SharedMemory.DtcPNum] [%d]\n", SharedMemory.DtcPNum);	   
			 CanFDFun.DiagModeSel = CANDIAG_NONE;
			 break;
		case CANDIAG_DTCSTATUSCNT_C:   // button DTC-H
			 SharedMemory.DtcHNum = CanFDFun.InBuffer[1];
			 GSLC_DEBUG_PRINT("[SharedMemory.DtcHNum] [%d]\n", SharedMemory.DtcHNum);	   
			 CanFDFun.DiagModeSel = CANDIAG_NONE;
			 break;
			
		}
	}
	
//*****************************************************************


#if 1
	
///////////////////////////////////////
/// Small Font ....
///////////////////////////////////////
    if(WifiSetting==0)
	{
///---------------------------------------------------------IP Address
		memset(acTxt4,0x00,TXTLENGTH);
		WiFiFnc.Local_IpAddress.toCharArray(acTxt4,TXTLENGTH); 
		memcpy(acTxt4,(char*)WiFiFnc.host, strlen(WiFiFnc.host)); 
		snprintf(acTxt4,TXTLENGTH,"%s",acTxt4);
		
///---------------------------------------------------------Port
		memset(acTxt6,0x00,TXTLENGTH);
		snprintf(acTxt6,TXTLENGTH,"%4u",WiFiFnc.udpPort);
		
		switch(WiFiFnc.ApModeConStatus)
		{
			case STARTING:
			case READY:
			case STOP:
			case AP_STADISCONNECTED:
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr4,GSLC_COL_WHITE);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr4,acTxt4);
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr6,GSLC_COL_WHITE);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr6,acTxt6);
				break;				
			case AP_UDPCONNECTEDOK:
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr4,GSLC_COL_RED);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr4,acTxt4);
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr6,GSLC_COL_RED);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr6,acTxt6);
				break;
			case AP_UDPCONNECTSTOP:
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr4,GSLC_COL_ORANGE);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr4,acTxt4);
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr6,GSLC_COL_ORANGE);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr6,acTxt6);
				break;
			default : 
				break;
			
		}
    }
	else if(WifiSetting==1)
	{
		memcpy(acTxt4,STAMODE,TSizeSTA);
		WiFiFnc.Local_IpAddress.toCharArray(&acTxt4[TSizeSTA],TXTLENGTH-TSizeSTA); 
		snprintf(acTxt4,TXTLENGTH,"%s",acTxt4);
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr4,GSLC_COL_WHITE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr4,acTxt4);

   	}

	if(WifiSetting==0)
	{
///---------------------------------------------------------SSID
		memset(acTxt5,0x00,TXTLENGTH);
		memcpy(acTxt5,APMODE,TSizeAP);
		memcpy(&acTxt5[TSizeAP],(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
		snprintf(acTxt5,TXTLENGTH,"%s",acTxt5);
		
///---------------------------------------------------------Passward
		memset(acTxt7,0x00,TXTLENGTH);
	//	WiFiFnc.networkPswd.toCharArray(acTxt, TXTLENGTH); 
		memcpy(acTxt7,(char*)WiFiFnc.networkPswd, strlen(WiFiFnc.networkPswd)); 
		snprintf(acTxt7,TXTLENGTH,"%s",acTxt7);
		
		switch(WiFiFnc.ApModeConStatus)
		{
			case STARTING:
			case READY:
			case STOP:
			case AP_STADISCONNECTED:
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr5,GSLC_COL_WHITE);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr5,acTxt5);
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr7,GSLC_COL_WHITE);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr7,acTxt7);
				break;				
			case AP_STACONNECTED:
			case AP_STAIPASSIGNED:
			case AP_PROBEREQRECVED:
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr5,GSLC_COL_RED);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr5,acTxt5);
				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr7,GSLC_COL_RED);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr7,acTxt7);
				break;
			default : 
				break;
		}
///---------------------------------------------------------P : 
		memset(DTCTxtP,0x00,TXTLENGTH);
		memcpy(DTCTxtP,DTCP,TDTCP);
		snprintf(&DTCTxtP[TDTCP],TXTLENGTH,"%02X%",SharedMemory.DtcPNum);
//		memcpy(&DTCTxtP[TDTCP],(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
		snprintf(DTCTxtP,TXTLENGTH,"%s",DTCTxtP);
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr16,GSLC_COL_WHITE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr16,DTCTxtP);
///---------------------------------------------------------H : 
		memset(DTCTxtH,0x00,TXTLENGTH);
		memcpy(DTCTxtH,DTCH,TDTCH);
		snprintf(&DTCTxtH[TDTCH],TXTLENGTH,"%02X%",SharedMemory.DtcHNum);
//		memcpy(&DTCTxtH[TDTCH],(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
		snprintf(DTCTxtH,TXTLENGTH,"%s",DTCTxtH);
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr17,GSLC_COL_WHITE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr17,DTCTxtH);
///----------------------------------------------------------------- : 
		
	}
	else if(WifiSetting==1)
	{
		memcpy(acTxt5,STAMODE,TSizeSTA);
		memcpy(&acTxt5[TSizeSTA],(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
		snprintf(acTxt5,TXTLENGTH,"%s",acTxt5);

		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr5,GSLC_COL_ORANGE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr5,acTxt5);

	}

	
#endif

	ChangeButtonSet();

//*****************************************************************
///////////////////////////////////////
/// Option Menu Page  ....
///////////////////////////////////////
#if 1
	memset(OptTxtOut,0x00,TXTLENGTH);
//	m_pElemTextStrOptionSTmin
//	snprintf(OptTxtOut,TXTLENGTH, "%s",SharedMemory.STmin);   // char number : 19...
	snprintf(OptTxtOut,TXTLENGTH,"%u",SharedMemory.STmin);
	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOptionSTmin,GSLC_COL_YELLOW);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOptionSTmin,OptTxtOut);

//	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr6,GSLC_COL_ORANGE);
//	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr6,acTxt6);
	
#endif

//*****************************************************************
//////////////////////////////////////////
	if(SharedMemory.OTAMenuOption)
	{
//  	   OTASWUpgradeMenu();
		if(WifiSetting==0)
		{
			switch(WiFiFnc.ApModeConStatus)
			{
				case STARTING:
				case READY:
				case STOP:
				case AP_STADISCONNECTED:
					memcpy(OTAIP,acTxt4,TXTLENGTH);
					gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAIP,GSLC_COL_WHITE);
					gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAIP,OTAIP);

					memset(OTASSID,0x00,TXTLENGTH);
					memcpy(OTASSID,(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
					gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTASSIDPW,GSLC_COL_WHITE);
					gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTASSIDPW,OTASSID);

					memset(OTAPW,0x00,TXTLENGTH);
					memcpy(OTAPW,(char*)WiFiFnc.networkPswd, strlen(WiFiFnc.networkPswd)); 
					gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAPW,GSLC_COL_WHITE);
					gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAPW,OTAPW);

					memcpy(OTAFILE,"No File", TXTLENGTH); 
					gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAFNAME,GSLC_COL_WHITE);
					gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAFNAME,OTAFILE);

					SharedMemory.OTAConnect = 0;
					break;				
				case AP_STACONNECTED:
				case AP_STAIPASSIGNED:
				case AP_PROBEREQRECVED:
					memcpy(OTAIP,acTxt4,TXTLENGTH);
					gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAIP,GSLC_COL_RED);
					gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAIP,OTAIP);
					
					memset(OTASSID,0x00,TXTLENGTH);
					memcpy(OTASSID,(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
					gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTASSIDPW,GSLC_COL_RED);
					gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTASSIDPW,OTASSID);
					
					memset(OTAPW,0x00,TXTLENGTH);
					memcpy(OTAPW,(char*)WiFiFnc.networkPswd, strlen(WiFiFnc.networkPswd)); 
					gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAPW,GSLC_COL_RED);
					gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAPW,OTAPW);
					
					memcpy(OTAFILE,"No File", TXTLENGTH); 
					gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAFNAME,GSLC_COL_ORANGE);
					gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAFNAME,OTAFILE);

					SharedMemory.OTAConnect = 1;
					break;
				default : 
					break;
			}
		
	
		}
		else if(WifiSetting==1)
		{
			memcpy(OTAIP,acTxt4,TXTLENGTH);
			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAIP,GSLC_COL_RED);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAIP,OTAIP);
			
			memset(OTASSID,0x00,TXTLENGTH);
			memcpy(OTASSID,(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTASSIDPW,GSLC_COL_RED);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTASSIDPW,OTASSID);
			
			memset(OTAPW,0x00,TXTLENGTH);
			memcpy(OTAPW,(char*)WiFiFnc.networkPswd, strlen(WiFiFnc.networkPswd)); 
			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAPW,GSLC_COL_RED);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAPW,OTAPW);
			
			memcpy(OTAFILE,"No File", TXTLENGTH); 
			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAFNAME,GSLC_COL_RED);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAFNAME,OTAFILE);

			SharedMemory.OTAConnect = 1;
		}

		if(SharedMemory.OTAMStart)
		{
			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAStartStop,GSLC_COL_RED);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAStartStop,"Stop");
		}			
		else
		{
			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAStartStop,GSLC_COL_WHITE);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAStartStop,"Start");
		}
	
	}

}

void TftGuiLCDFunClass::OTASWUpgradeMenu()
{
#if 0
	if(WifiSetting==0)
	{
		memcpy(OTAIP,acTxt4,TXTLENGTH);
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAIP,GSLC_COL_WHITE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAIP,OTAIP);
		
		memset(OTASSID,0x00,TXTLENGTH);
		memcpy(OTASSID,(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTASSIDPW,GSLC_COL_WHITE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTASSIDPW,OTASSID);
		
		memset(OTAPW,0x00,TXTLENGTH);
		memcpy(OTAPW,(char*)WiFiFnc.networkPswd, strlen(WiFiFnc.networkPswd)); 
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAPW,GSLC_COL_WHITE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAPW,OTAPW);
		
		memcpy(OTAFILE,"No File", TXTLENGTH); 
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAFNAME,GSLC_COL_WHITE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAFNAME,OTAFILE);
	}
	else if(WifiSetting==1)
	{
		memcpy(OTAIP,acTxt4,TXTLENGTH);
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAIP,GSLC_COL_ORANGE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAIP,OTAIP);
		
		memset(OTASSID,0x00,TXTLENGTH);
		memcpy(OTASSID,(char*)WiFiFnc.networkName, strlen(WiFiFnc.networkName)); 
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTASSIDPW,GSLC_COL_ORANGE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTASSIDPW,OTASSID);
		
		memset(OTAPW,0x00,TXTLENGTH);
		memcpy(OTAPW,(char*)WiFiFnc.networkPswd, strlen(WiFiFnc.networkPswd)); 
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAPW,GSLC_COL_ORANGE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAPW,OTAPW);
		
		memcpy(OTAFILE,"No File", TXTLENGTH); 
		gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrOTAFNAME,GSLC_COL_ORANGE);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAFNAME,OTAFILE);
	}
#endif	
}

void TftGuiLCDFunClass::ChangeButtonSet()
{

	if(WiFiFnc.WifiWorking)
	{
		gslc_ElemSetTxtCol(&m_gui,pElemRefBk0,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk0,GSLC_COL_GREEN,GSLC_COL_GRAY,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk0,true);
		
		gslc_ElemSetTxtCol(&m_gui,pElemRefBk1,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk1,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk1,true);

		gslc_ElemSetTxtCol(&m_gui,pElemRefBk2,GSLC_COL_WHITE);
		gslc_ElemSetCol(&m_gui,pElemRefBk2,GSLC_COL_BLUE_DK2,GSLC_COL_RED,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk2,true);

		//pElemRefBk10 ~ 13 

		gslc_ElemSetTxtCol(&m_gui,pElemRefBk10,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk10,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk10,true);

		gslc_ElemSetTxtCol(&m_gui,pElemRefBk11,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk11,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk11,true);

		gslc_ElemSetTxtCol(&m_gui,pElemRefBk12,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk12,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk12,true);

		gslc_ElemSetTxtCol(&m_gui,pElemRefBk13,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk13,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk13,true);

		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr2,"Version...");
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr13,"Version...");
		
	}
	else
	{
		gslc_ElemSetTxtCol(&m_gui,pElemRefBk0,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk0,GSLC_COL_GREEN,GSLC_COL_GREEN,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk0,true);
		
#if 1
		if( RFileFnc.HSCANBTNState == 0)  //2020.05.06 modify by yongwookwi
		{
			if(RFileFnc.CanFDSettingdState == 0)
			{
				//pElemRefBk10 ~ 13 //2020.05.06 //pElemRefM2Bk1
				gslc_ElemSetTxtCol(&m_gui,pElemRefM2Bk1,GSLC_COL_WHITE);
				gslc_ElemSetCol(&m_gui,pElemRefM2Bk1,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY,GSLC_COL_GRAY);
				gslc_ElemSetFrameEn(&m_gui,pElemRefM2Bk1,true);
				GSLC_DEBUG_PRINT("RFileFnc.CanFDSettingdState -- %d\n", RFileFnc.CanFDSettingdState);	  
			}
			else
			{
				gslc_ElemSetTxtCol(&m_gui,pElemRefM2Bk1,GSLC_COL_WHITE);
				gslc_ElemSetCol(&m_gui,pElemRefM2Bk1,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
				gslc_ElemSetFrameEn(&m_gui,pElemRefM2Bk1,true);
			}
		}
		else
		{
			gslc_ElemSetTxtCol(&m_gui,pElemRefM2Bk1,GSLC_COL_WHITE);
			gslc_ElemSetCol(&m_gui,pElemRefM2Bk1,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
			gslc_ElemSetFrameEn(&m_gui,pElemRefM2Bk1,true);
		}
#endif		

		gslc_ElemSetTxtCol(&m_gui,pElemRefBk2,GSLC_COL_WHITE);
		gslc_ElemSetCol(&m_gui,pElemRefBk2,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK2,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk2,true);

		gslc_ElemSetTxtCol(&m_gui,pElemRefBk10,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk10,GSLC_COL_BLUE_DK4,GSLC_COL_YELLOW,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk10,true);
		
		gslc_ElemSetTxtCol(&m_gui,pElemRefBk11,GSLC_COL_WHITE);
		gslc_ElemSetCol(&m_gui,pElemRefBk11,GSLC_COL_BLUE_DK4,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk11,true);
		
		gslc_ElemSetTxtCol(&m_gui,pElemRefBk12,GSLC_COL_BLACK);
		gslc_ElemSetCol(&m_gui,pElemRefBk12,GSLC_COL_BLUE_DK4,GSLC_COL_YELLOW,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk12,true);
		
		gslc_ElemSetTxtCol(&m_gui,pElemRefBk13,GSLC_COL_WHITE);
		gslc_ElemSetCol(&m_gui,pElemRefBk13,GSLC_COL_BLUE_DK4,GSLC_COL_BLUE_DK4,GSLC_COL_GRAY);
		gslc_ElemSetFrameEn(&m_gui,pElemRefBk13,true);
		
	}

}

void TftGuiLCDFunClass::ChangeInitButtonSet()
{
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr2,"Version...");
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr13,"Version...");
}

void TftGuiLCDFunClass::SetUpInit()
{



}

void TftGuiLCDFunClass::ListBoxUpdate()
{
//	static uint8_t m_pXListboxBuf[50];	
	 memset(ListBoxTxt1,0x00,LISTTXTLENGTH);
	 memset(ListBoxTxt2,0x00,LISTTXTLENGTH);
	 memset(ListBoxTxt3,0x00,LISTTXTLENGTH);
	 memset(ListBoxTxt4,0x00,LISTTXTLENGTH);
	 memset(ListBoxTxt5,0x00,LISTTXTLENGTH);

 	 RFileFnc.DownLoadFileName[0].toCharArray(ListBoxTxt1,LISTTXTLENGTH);
	 RFileFnc.DownLoadFileName[1].toCharArray(ListBoxTxt2,LISTTXTLENGTH);
	 RFileFnc.DownLoadFileName[2].toCharArray(ListBoxTxt3,LISTTXTLENGTH);
	 RFileFnc.DownLoadFileName[3].toCharArray(ListBoxTxt4,LISTTXTLENGTH);
	 RFileFnc.DownLoadFileName[4].toCharArray(ListBoxTxt5,LISTTXTLENGTH);

	 Serial.println("DownLoadFileName[0] : "+RFileFnc.DownLoadFileName[0]);
	 Serial.println("DownLoadFileName[1] : "+RFileFnc.DownLoadFileName[1]);
	 Serial.println("DownLoadFileName[2] : "+RFileFnc.DownLoadFileName[2]);
	 Serial.println("DownLoadFileName[3] : "+RFileFnc.DownLoadFileName[3]);
	 Serial.println("DownLoadFileName[4] : "+RFileFnc.DownLoadFileName[4]);
	 
#if 1
	 snprintf(ListBoxTxt1,LISTTXTLENGTH,"%s",ListBoxTxt1);
	 snprintf(ListBoxTxt2,LISTTXTLENGTH,"%s",ListBoxTxt2);
	 snprintf(ListBoxTxt3,LISTTXTLENGTH,"%s",ListBoxTxt3);
	 snprintf(ListBoxTxt4,LISTTXTLENGTH,"%s",ListBoxTxt4);
	 snprintf(ListBoxTxt5,LISTTXTLENGTH,"%s",ListBoxTxt5);

	 GSLC_DEBUG_PRINT("ListBoxTxt1 -- %s\n", ListBoxTxt1);	   
	 GSLC_DEBUG_PRINT("ListBoxTxt2 -- %s\n", ListBoxTxt2);	   
	 GSLC_DEBUG_PRINT("ListBoxTxt3 -- %s\n", ListBoxTxt3);	   
	 GSLC_DEBUG_PRINT("ListBoxTxt4 -- %s\n", ListBoxTxt4);	   
	 GSLC_DEBUG_PRINT("ListBoxTxt5 -- %s\n", ListBoxTxt5);	   
#endif

#if 1
	//void gslc_ElemXListboxReset ( gslc_tsGui  pGui, gslc_tsElemRef  pElemRef )
    gslc_ElemXListboxReset(&m_gui, m_pElemListbox);

	snprintf(ListBoxTxt, LISTTXTLENGTH, "NONE");  
	gslc_ElemSetTxtStr(&m_gui, m_pElemListBoxTextStr, ListBoxTxt);  

	gslc_ElemXListboxAddItem(&m_gui, m_pElemListbox, ListBoxTxt1);  
	gslc_ElemXListboxAddItem(&m_gui, m_pElemListbox, ListBoxTxt2);  
	gslc_ElemXListboxAddItem(&m_gui, m_pElemListbox, ListBoxTxt3);  
	gslc_ElemXListboxAddItem(&m_gui, m_pElemListbox, ListBoxTxt4);  
#endif

}

void TftGuiLCDFunClass::ProgressBarGage(float percentage,uint8_t OnOff)
{
	char	acTxt[TXTLENGTH];

	// Perform drawing updates
	// - Note: we can make the updates conditional on the active
	//	 page by checking gslc_GetPageCur() first.
	
//	snprintf(acTxt,MAX_STR,"%u",m_nCount);
//	gslc_ElemSetTxtStr(&m_gui,pElemCnt,acTxt);

	gslc_ElemXGaugeUpdate(&m_gui,m_pElemProgress1,(int)percentage);
	gslc_ElemXGaugeUpdate(&m_gui,m_pElemProgress2,(int)percentage);
//	gslc_ElemXGaugeUpdate(&m_gui,m_pElemProgress3,(int)percentage);
	
	if(OnOff)
	{
		if(TftLCDFun.SuccessFail)
		{
			memset(acTxt,0x00,TXTLENGTH);
			snprintf(acTxt,TXTLENGTH," FAIL.....\n");
//			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr3,GSLC_COL_RED); //GSLC_COL_ORANGE
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr3,acTxt);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr15,acTxt);
			TftLCDFun.StartStop = false;
		}
		else if(percentage >= 100)
		{
			if(TftLCDFun.progresspercentSuccess)
			{
				memset(acTxt,0x00,TXTLENGTH);
				snprintf(acTxt,TXTLENGTH," SUCCESS..\n");
//				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr3,GSLC_COL_ORANGE); //GSLC_COL_ORANGE
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr3,acTxt);
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr15,acTxt);
				TftLCDFun.progresspercentSuccess = 0;
			}
		}
		else if(percentage < 100)
		{
			memset(acTxt,0x00,TXTLENGTH);
			snprintf(acTxt,TXTLENGTH,"%3.1f %%",percentage);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr3,acTxt);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr15,acTxt);
//			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr3,(char *)"%");
		}
	}
	else
	{
		memset(acTxt,0x00,TXTLENGTH);
		if(RFileFnc.CanFDSettingdState == 0){	
			snprintf(acTxt,TXTLENGTH,"[HS]START.....");   // char number : 19...
		}
		else{
			snprintf(acTxt,TXTLENGTH,"[FD]START.....");   // char number : 19...
		}
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr3,acTxt);
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr15,acTxt);
		TftLCDFun.StartStop = false;
	}
}


void TftGuiLCDFunClass::ProgressBarGageOTA(float percentage,uint8_t OnOff)
{
	char	acTxt[TXTLENGTH];

	// Perform drawing updates
	// - Note: we can make the updates conditional on the active
	//	 page by checking gslc_GetPageCur() first.
	
//	snprintf(acTxt,MAX_STR,"%u",m_nCount);
//	gslc_ElemSetTxtStr(&m_gui,pElemCnt,acTxt);

	gslc_ElemXGaugeUpdate(&m_gui,m_pElemProgress3,(int)percentage);
	
	if(OnOff)
	{
		if(TftLCDFun.OtaSuccessFail)
		{
			memset(acTxt,0x00,TXTLENGTH);
			snprintf(acTxt,TXTLENGTH," FAIL.....\n");
//			gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr3,GSLC_COL_RED); //GSLC_COL_ORANGE
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAFNAME,acTxt);
			TftLCDFun.OtaStartStop = false;
		}
		else if(percentage >= 100)
		{
			if(TftLCDFun.otaprogresspercentSuccess)
			{
				memset(acTxt,0x00,TXTLENGTH);
				snprintf(acTxt,TXTLENGTH," SUCCESS..\n");
//				gslc_ElemSetTxtCol(&m_gui,m_pElemTextStr3,GSLC_COL_ORANGE); //GSLC_COL_ORANGE
				gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAPER,acTxt);
				TftLCDFun.progresspercentSuccess = 0;
			}
		}
		else if(percentage < 100)
		{
			memset(acTxt,0x00,TXTLENGTH);
			snprintf(acTxt,TXTLENGTH,"%3.1f %%",percentage);
			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAPER,acTxt);
//			gslc_ElemSetTxtStr(&m_gui,m_pElemTextStr3,(char *)"%");
		}
	}
	else
	{
		memset(acTxt,0x00,TXTLENGTH);
		if(RFileFnc.CanFDSettingdState == 0){	
			snprintf(acTxt,TXTLENGTH,"[OTA FILE] START.....");   // char number : 19...
		}
		else{
			snprintf(acTxt,TXTLENGTH,"[OTA FILE] START.....");   // char number : 19...
		}
		gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrOTAFNAME,acTxt);
		TftLCDFun.OtaStartStop = false;
	}
}

void TftGuiLCDFunClass::VersionCheckSW(void)  //2020.02.27. Version Diaply Error .....
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_SWVER;
		CanFnc.Recu_Fragment = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
//		CanFnc.bFlowControl = true;
//		CanFnc.bFlowControl = false;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_SWVER;
		CanFDFun.Recu_Fragment = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
//		CanFDFun.bFlowControl = true;
		break;
	}

	if(SharedMemory.FlowControlType == 0x01)
	{
		CanFnc.bFlowControl   = true;
		CanFDFun.bFlowControl = true;
	}
	else if(SharedMemory.FlowControlType == 0x02)
	{
		CanFnc.bFlowControl   = false;
		CanFDFun.bFlowControl = false;
	}
	
	
}

void TftGuiLCDFunClass::VersionCheckSWDiag(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_SWVER_DIAG;
		CanFnc.Recu_Fragment = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
		CanFnc.bFlowControl = true;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_SWVER_DIAG;
		CanFDFun.Recu_Fragment = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
		CanFDFun.bFlowControl = true;
		break;
	}
}
	
void TftGuiLCDFunClass::VersionCheckHW(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_HWVER;
		CanFnc.Recu_Fragment = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
//		CanFnc.bFlowControl = true;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_HWVER;
		CanFDFun.Recu_Fragment = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
//		CanFDFun.bFlowControl = true;
		break;
	}


	if(SharedMemory.FlowControlType == 0x01)
	{
		CanFnc.bFlowControl   = true;
		CanFDFun.bFlowControl = true;
	}
	else if(SharedMemory.FlowControlType == 0x02)
	{
		CanFnc.bFlowControl   = false;
		CanFDFun.bFlowControl = false;
	}

	
}

void TftGuiLCDFunClass::VersionCheckSW_SUP(void)
{

	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_SWVER_SUP;
		CanFnc.Recu_Fragment = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
//		CanFnc.bFlowControl = true;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_SWVER_SUP;
		CanFDFun.Recu_Fragment = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
//		CanFDFun.bFlowControl = true;
		break;
	}


	if(SharedMemory.FlowControlType == 0x01)
	{
		CanFnc.bFlowControl   = true;
		CanFDFun.bFlowControl = true;
	}
	else if(SharedMemory.FlowControlType == 0x02)
	{
		CanFnc.bFlowControl   = false;
		CanFDFun.bFlowControl = false;
	}	
	
}

void TftGuiLCDFunClass::VersionCheckHW_SUP(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_HWVER_SUP;
		CanFnc.Recu_Fragment 	 = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
		CanFnc.bFlowControl = true;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_HWVER_SUP;
		CanFDFun.Recu_Fragment 	   = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
		CanFDFun.bFlowControl = true;
		break;
	}	


/*	if(SharedMemory.FlowControlType == 0x01)
	{
		CanFnc.bFlowControl   = true;
		CanFDFun.bFlowControl = true;
	}
	else if(SharedMemory.FlowControlType == 0x02)
	{
		CanFnc.bFlowControl   = false;
		CanFDFun.bFlowControl = false;
	}
*/	
}


void TftGuiLCDFunClass::CheckDTC_Pending_SUP(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUS_P;
		CanFnc.Recu_Fragment 	 = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
		CanFnc.bFlowControl = true;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUS_P;
		CanFDFun.Recu_Fragment 	   = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
		CanFDFun.bFlowControl = true;
		break;
	}	
}

void TftGuiLCDFunClass::CheckDTC_Confirming_SUP(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUS_C;
		CanFnc.Recu_Fragment 	 = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
		CanFnc.bFlowControl = true;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUS_C;
		CanFDFun.Recu_Fragment 	   = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
		CanFDFun.bFlowControl = true;
		break;
	}	
}

void TftGuiLCDFunClass::CheckDTC_ALL_SUP(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUS_A;
		CanFnc.Recu_Fragment 	 = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
		CanFnc.bFlowControl = true;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUS_A;
		CanFDFun.Recu_Fragment 	   = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
		CanFDFun.bFlowControl = true;
		break;
	}	
}

void TftGuiLCDFunClass::CheckDTC_PendingCnt_SUP(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUSCNT_P;
		CanFnc.Recu_Fragment 	 = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
		CanFnc.bFlowControl = false;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUSCNT_P;
		CanFDFun.Recu_Fragment 	   = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
		CanFDFun.bFlowControl = false;
		break;
	}	
}

void TftGuiLCDFunClass::CheckDTC_ConfirmingCnt_SUP(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUSCNT_C;
		CanFnc.Recu_Fragment 	 = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
		CanFnc.bFlowControl = false;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUSCNT_C;
		CanFDFun.Recu_Fragment 	   = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
		CanFDFun.bFlowControl = false;
		break;
	}	
}

void TftGuiLCDFunClass::CheckDTC_ALLCnt_SUP(void)
{
	switch(SharedMemory.File_CAN_TYPE)
	{
	case 1:  // HS CAN
		CanFnc.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUSCNT_A;
		CanFnc.Recu_Fragment 	 = CANDIAG_ECUINFO;
		CanFnc.bCompleteCANMessage = false;
		CanFnc.bFlowControl = false;
		break;
	case 2:  // CAN FD
		CanFDFun.CAN_DIAG_MODE_Sel = CANDIAG_DTCSTATUSCNT_A;
		CanFDFun.Recu_Fragment 	   = CANDIAG_ECUINFO;
		CanFDFun.bCompleteCANMessage = false;
		CanFDFun.bFlowControl = false;
		break;
	}	
}

void TftGuiLCDFunClass::EEPSaveParam(void)
{
//RFileFnc
	EEPROM.write(0, RFileFnc.WifiSettingdState); // Radio

	EEPROM.write(1, RFileFnc.HsCanSettingdState);
	EEPROM.write(2, RFileFnc.CanFDSettingdState);
	EEPROM.write(3, RFileFnc.WifiOnOffdState);

	EEPROM.write(4, SharedMemory.STmin);
	EEPROM.write(5, RFileFnc.DtcAllClearState);

	WiFiFnc.WifiWorking = RFileFnc.WifiOnOffdState;

	ReprogData.reprogStartStop = 0;
    ReprogDataFd.reprogStartStop = 0;
    // Configure CAN  Init  Setup ...
//  CanFnc.InitCanSetup();

	EEPROM.commit();

	Serial.printf("[EEPSave] [Wifi]%d [HsCan]%d [CanFD]%d [OnOff]%d [STmin]%d [DTCAL]%d\n",	RFileFnc.WifiSettingdState,
			RFileFnc.HsCanSettingdState,RFileFnc.CanFDSettingdState,RFileFnc.WifiOnOffdState,RFileFnc.STminTime,RFileFnc.DtcAllClearState);

	SharedMemory.File_CAN_TYPE = RFileFnc.CanFDSettingdState + 1;
	Serial.printf("[[EEPSaveParam[File_CAN_TYPE]]------------------SharedMemory.File_CAN_TYPE ] : %d \n", SharedMemory.File_CAN_TYPE );
	Serial.printf("[[EEPSaveParam[STmin]]------------------SharedMemory.STmin ] : %d \n", SharedMemory.STmin );
	
}

void TftGuiLCDFunClass::CsvParamView(void)
{
	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV1,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV1,SharedMemory.ECU_ID[0]);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV2,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV2,SharedMemory.P_ID[0]);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV3,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV3,SharedMemory.F_ID[0]);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV4,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV4,SharedMemory.CAN_TYPE[0]);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV5,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV5,SharedMemory.ECU_ID[1]);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV6,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV6,SharedMemory.P_ID[1]);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV7,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV7,SharedMemory.F_ID[1]);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV8,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV8,SharedMemory.CAN_TYPE[1]);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV9,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV9,SharedMemory.FileName);

	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV10,GSLC_COL_WHITE);
	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV10,SharedMemory.FileCanType);

//  m_pElemTextStrOptionSTmin
//	gslc_ElemSetTxtCol(&m_gui,m_pElemTextStrCSV11,GSLC_COL_WHITE);
//	gslc_ElemSetTxtStr(&m_gui,m_pElemTextStrCSV11,SharedMemory.WifiBdSWVer);
}
