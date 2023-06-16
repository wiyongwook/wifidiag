#include "TftLCDFun.h"
#include "WifiFunc.h"


// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The SPIFFS file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData1"
// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

// Keypad start position, key sizes and spacing
#define KEY_X 40 // Centre of key
#define KEY_Y 96
#define KEY_W 62 // Width and height
#define KEY_H 30
#define KEY_SPACING_X 18 // X and Y gap
#define KEY_SPACING_Y 20
#define KEY_TEXTSIZE 1   // Font size multiplier

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

// Numeric display box size and location
#define DISP_X 1
#define DISP_Y 10
#define DISP_W 238
#define DISP_H 50
#define DISP_TSIZE 3
#define DISP_TCOLOR TFT_CYAN

// Number length, buffer for storing it and character index
#define NUM_LEN 12
char numberBuffer[NUM_LEN + 1] = "";
uint8_t numberIndex = 0;

// We have a status line for messages
#define STATUS_X 120 // Centred on this
#define STATUS_Y 65

boolean SwitchOn = false;

// Comment out to stop drawing black spots
#define BLACK_SPOT

#define T_FRAME_Y 240

// Switch position and size
#define FRAME_X 260
#define FRAME_Y 25
#define FRAME_W 60
#define FRAME_H 40

// scroll position and size
#define SCROLL_X 10
#define SCROLL_Y 110
#define SCROLL_W 310
#define SCROLL_H 40

// Green zone size
#define GREENBUTTON_X FRAME_X
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W FRAME_W
#define GREENBUTTON_H FRAME_H

// Red zone size
#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y + FRAME_H + 1
#define REDBUTTON_W FRAME_W
#define REDBUTTON_H FRAME_H

// Yellow zone size
#define YELLOWBUTTON_X FRAME_X
#define YELLOWBUTTON_Y FRAME_Y + REDBUTTON_Y + SCROLLBAR_H + 23
#define YELLOWBUTTON_W FRAME_W
#define YELLOWBUTTON_H FRAME_H

// Blue zone size
#define BLUEBUTTON_X 10//FRAME_X
#define BLUEBUTTON_Y FRAME_Y + REDBUTTON_Y + SCROLLBAR_H + 23
#define BLUEBUTTON_W FRAME_W
#define BLUEBUTTON_H FRAME_H

// Blue zone size
#define SKYBLUEBUTTON_X 20+BLUEBUTTON_W//FRAME_X
#define SKYBLUEBUTTON_Y FRAME_Y + REDBUTTON_Y + SCROLLBAR_H + 23
#define SKYBLUEBUTTON_W FRAME_W
#define SKYBLUEBUTTON_H FRAME_H


// Touch Green zone size
#define T_GREENBUTTON_X FRAME_X
#define T_GREENBUTTON_Y T_FRAME_Y - FRAME_Y
#define T_GREENBUTTON_W FRAME_W
#define T_GREENBUTTON_H FRAME_H

// Touch Red zone size
#define T_REDBUTTON_X FRAME_X
#define T_REDBUTTON_Y T_FRAME_Y - (FRAME_Y + FRAME_H + 1)
#define T_REDBUTTON_W FRAME_W
#define T_REDBUTTON_H FRAME_H

// Touch Yellow zone size
#define T_YELLOWBUTTON_X FRAME_X
#define T_YELLOWBUTTON_Y T_FRAME_Y - 165 //(FRAME_Y + FRAME_H + 1)
#define T_YELLOWBUTTON_W FRAME_W
#define T_YELLOWBUTTON_H FRAME_H

// Touch BLUE zone size
#define T_BLUEBUTTON_X 10//FRAME_X 
#define T_BLUEBUTTON_Y T_FRAME_Y - 165 //(FRAME_Y + FRAME_H + 1)
#define T_BLUEBUTTON_W FRAME_W
#define T_BLUEBUTTON_H FRAME_H

// Touch SKYBLUE zone size
#define T_SKYBLUEBUTTON_X 20+T_BLUEBUTTON_W//FRAME_X
#define T_SKYBLUEBUTTON_Y T_FRAME_Y - 165 //(FRAME_Y + FRAME_H + 1)
#define T_SKYBLUEBUTTON_W FRAME_W
#define T_SKYBLUEBUTTON_H FRAME_H

// Scroll zone size
#define SCROLLBAR_X SCROLL_X
#define SCROLLBAR_Y SCROLL_Y
#define SCROLLBAR_W SCROLL_W
#define SCROLLBAR_H SCROLL_H


void TftLCDFunClass::InitTftLCDClass(void){
 // Initialise the TFT screen
	tft.init();

	// Set the rotation before we calibrate
	tft.setRotation(1);

	// Calibrate the touch screen and retrieve the scaling factors
//	touch_calibrate();

	// Clear the screen & FILE Text ....
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(10, 10);
	tft.setTextFont(2);
	tft.setTextSize(1);
	tft.setTextColor(TFT_YELLOW, TFT_BLACK);
	tft.println("DOWNLOAD FILE :");

//------------------------------------------------------------
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(120, 10);
	tft.println(RFileFnc.DownLoadFileName[0]);   // File Name ....
//	RFileFnc.DownLoadFileName[0];
	Serial.printf("[CanFile.name] : %s \n",RFileFnc.DownLoadFileName[0]);
	tft.drawLine(120,25,250,25,TFT_WHITE);

	tft.setTextFont(2);
	tft.setTextSize(1);
	tft.setTextColor(TFT_YELLOW, TFT_BLACK);
	tft.setCursor(10, 50);
	tft.println("ECU VERSION    :");	 			// ECU Version
	tft.drawLine(120,65,250,65,TFT_WHITE);
//	tft.setCursor(50, 50);
//	tft.println(""); 			 			// ECU Version Number ....
// Draw button (this example does not use library Button class)
	VersionConfirmBtn(0);
//------------------------------------------------------------

	tft.setTextFont(2);
	tft.setTextSize(1);
	tft.setTextColor(TFT_YELLOW, TFT_BLACK);
	tft.setCursor(10, 90);
	tft.println("DOWNLOAD INFO :");	 			// ECU Version
	tft.drawLine(120,105,250,105,TFT_WHITE);
//	tft.setCursor(50, 150);
//	tft.println(""); 			 			// ECU Version Number ....
// Draw button (this example does not use library Button class)
	DownloadBtn(0);

//------------------------------------------------------------scroll bar display
	drawProgress(0, "START",0);
//------------------------------------------------------------
    WIFISelBtn(0);
    TchCalBtn(0);
    SettingBtn(0);
//------------------------------------------------------------
#if 1
	WifiTextDisplay();
#endif
	Serial.printf("[InitTftLCDClass]--TFT LCD Init ..... \n");
}
   
   
//------------------------------------------------------------------------------------------

void  TftLCDFunClass::touch_calibrate(void)
{
 uint16_t calData[5];
 uint8_t calDataOK = 0;

 // check file system exists
 if (!SPIFFS.begin()) {
   Serial.println("Formating file system");
   SPIFFS.format();
   SPIFFS.begin();
 }

 // check if calibration file exists and size is correct
 if (SPIFFS.exists(CALIBRATION_FILE)) {
   if (REPEAT_CAL)
   {
	 // Delete if we want to re-calibrate
	 SPIFFS.remove(CALIBRATION_FILE);
   }
   else
   {
	 File f = SPIFFS.open(CALIBRATION_FILE, "r");
	 if (f) {
	   if (f.readBytes((char *)calData, 14) == 14)
		 calDataOK = 1;
	   f.close();
	 }
   }
 }

 if (calDataOK && !REPEAT_CAL) {
   // calibration data valid
   tft.setTouch(calData);
 } else {
   // data not valid so recalibrate
   tft.fillScreen(TFT_BLACK);
   tft.setCursor(20, 0);
   tft.setTextFont(2);
   tft.setTextSize(1);
   tft.setTextColor(TFT_WHITE, TFT_BLACK);

   tft.println("Touch corners as indicated");

   tft.setTextFont(1);
   tft.println();

   if (REPEAT_CAL) {
	 tft.setTextColor(TFT_RED, TFT_BLACK);
	 tft.println("Set REPEAT_CAL to false to stop this running again!");
   }

   tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

   tft.setTextColor(TFT_GREEN, TFT_BLACK);
   tft.println("Calibration complete!");

   // store data
   File f = SPIFFS.open(CALIBRATION_FILE, "w");
   if (f) {
	 f.write((const unsigned char *)calData, 14);
	 f.close();
   }
 }
}

void  TftLCDFunClass::VersionConfirmBtn(uint8_t press)
{
	if(!press)  	tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_GREEN);
	else			tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_DARKGREY);

	drawFrame();
	tft.setTextColor(TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextDatum(MC_DATUM);
	tft.drawString("VER", GREENBUTTON_X + 30, GREENBUTTON_Y + (GREENBUTTON_H / 2));
	SwitchOn = true;
}

void  TftLCDFunClass::DownloadBtn(uint8_t press)
{
	if(!press)  	tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_RED);
	else     		tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_DARKGREY);

	drawFrame();
	tft.setTextColor(TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextDatum(MC_DATUM);
	tft.drawString("DOWN", REDBUTTON_X + 30, REDBUTTON_Y + (REDBUTTON_H / 2));
	SwitchOn = true;
}

void  TftLCDFunClass::WIFISelBtn(uint8_t press)
{
    if(!press)  tft.fillRect(YELLOWBUTTON_X, YELLOWBUTTON_Y, YELLOWBUTTON_W, YELLOWBUTTON_H, TFT_YELLOW);
	else     	tft.fillRect(YELLOWBUTTON_X, YELLOWBUTTON_Y, YELLOWBUTTON_W, YELLOWBUTTON_H, TFT_DARKGREY);
	
	drawFrame();
	tft.setTextColor(TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextDatum(MC_DATUM);

	tft.drawString("WIFI",  YELLOWBUTTON_X + 30, YELLOWBUTTON_Y + (YELLOWBUTTON_H / 2));

	SwitchOn = true;
}


void  TftLCDFunClass::TchCalBtn(uint8_t press)
{
    if(!press)  		tft.fillRect(BLUEBUTTON_X, BLUEBUTTON_Y, BLUEBUTTON_W, BLUEBUTTON_H, TFT_BLUE);
	else     			tft.fillRect(BLUEBUTTON_X, BLUEBUTTON_Y, BLUEBUTTON_W, BLUEBUTTON_H, TFT_DARKGREY);
	
	drawFrame();
	tft.setTextColor(TFT_YELLOW);
	tft.setTextSize(1);
	tft.setTextDatum(MC_DATUM);

	tft.drawString("T_Cal",  BLUEBUTTON_X + 30, BLUEBUTTON_Y + (BLUEBUTTON_H / 2));

	SwitchOn = true;
}

void  TftLCDFunClass::SettingBtn(uint8_t press)
{
    if(!press)  tft.fillRect(SKYBLUEBUTTON_X, SKYBLUEBUTTON_Y, SKYBLUEBUTTON_W, SKYBLUEBUTTON_H, TFT_ORANGE);
	else     	tft.fillRect(SKYBLUEBUTTON_X, SKYBLUEBUTTON_Y, SKYBLUEBUTTON_W, SKYBLUEBUTTON_H, TFT_DARKGREY);
	
	drawFrame();
	tft.setTextColor(TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextDatum(MC_DATUM);

	tft.drawString("Setting",  SKYBLUEBUTTON_X + 30, SKYBLUEBUTTON_Y + (SKYBLUEBUTTON_H / 2));

	SwitchOn = true;
}


void  TftLCDFunClass::drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, TFT_BLACK);
}


/***************************************************************************************
**Update progress bar
***************************************************************************************/
void TftLCDFunClass::drawProgress(uint8_t percentage, String text, uint8_t mode) 
{  
	tft.setTextDatum(BC_DATUM);  
	tft.setTextColor(TFT_YELLOW, TFT_BLACK);  
	tft.setTextPadding(80);  

	String tFileSize,dFileSize,pFileSize;

	tFileSize = String(ReprogData.reprogFileHexSize);
	dFileSize = String(ReprogData.reprogFileHexSize - ReprogData.reprogHexSize);
	
	pFileSize = dFileSize;
	pFileSize += "/";
	pFileSize += tFileSize;

	if(mode == 0)
	{
		tft.drawString(text, 150, 100);
	}
	else if(mode == 1)
	{
		tft.drawString(text, 145, 100);
		if(TftLCDFun.SuccessFail){
			tft.setTextColor(TFT_RED, TFT_BLACK);  
			tft.drawString(" FAIL  ", 210, 100);
//			StartStop = false;
		}else if(percentage == 100){
			tft.drawString("SUCCESS", 210, 100);
		}else{
			tft.drawString(pFileSize, 210, 100);
		}
	}
		
	ui.drawProgressBar(SCROLLBAR_X, SCROLLBAR_Y, SCROLLBAR_W, SCROLLBAR_H, percentage, TFT_WHITE, TFT_ORANGE);  tft.setTextPadding(0);
}

void TftLCDFunClass::TouchBtnCheck(void) 
{
	uint16_t x, y,BtnID = 0;

	// See if there's any touch data for us
	if (tft.getTouch(&x, &y))
	{
		Serial.printf("[TouchBtnCheck] X : %d ... Y : %d...\n",x,y);	
//		tft.fillCircle(x, y, 2, TFT_BLACK);
		BtnID = isPressBtn(x, y);	
		TouchPressd = BtnID;
	}
	else
	{
		switch(TouchPressd)
		{
		case 100:
			break;
		case 1:
		    VersionConfirmBtn(0);
			TouchPressd = 0;
			break;
		case 2:
			DownloadBtn(0);
			TouchPressd = 0;
			ReprogramInitVariable();
 			break;
		case 3:
			WIFISelBtn(0);
			TouchPressd = 0;
			WifibtnPressed();

			if(WiFiFnc.WifiWorking){
			    VersionConfirmBtn(1);
				DownloadBtn(1);
				drawProgress(0, "START",0);
			}
			else{
			    VersionConfirmBtn(0);
				DownloadBtn(0);
			}
				
			break;
		case 4:
			TchCalBtn(0);
			TouchPressd = 0;
			break;

		case 5:
			SettingBtn(0);
			TouchPressd = 0;
			break;

		default:
			if(WiFiFnc.WifiWorking){
			    VersionConfirmBtn(1);
				DownloadBtn(1);
			}
			else{
			    VersionConfirmBtn(0);
				DownloadBtn(0);
			}
			TouchPressd = 100;
			break;
		}
	}
	

}


int  TftLCDFunClass::isPressBtn(uint16_t xp,uint16_t yp)
{
	// Draw a block spot to show where touch was calculated to be

	if ((xp >= T_GREENBUTTON_X) && (xp <= (T_GREENBUTTON_X + T_GREENBUTTON_W))) {
		if ((yp >= (T_GREENBUTTON_Y - T_GREENBUTTON_H)) && (yp <= T_GREENBUTTON_Y)) {
			 Serial.println("VERSION GREEN btn hit");
		     if(WiFiFnc.WifiWorking){
				 return 0;
	     	 }
			 else{
				 VersionConfirmBtn(1);
				 return 1;
		 	 }
		}
	}
	
	if ((xp >= T_REDBUTTON_X) && (xp <= (T_REDBUTTON_X + T_REDBUTTON_W))) {
		if ((yp >= (T_REDBUTTON_Y - T_REDBUTTON_H)) && (yp <= (T_REDBUTTON_Y ))) {
			 Serial.println("DOWN RED btn hit");
	  	     if(WiFiFnc.WifiWorking){
				 return 0;
	     	 }
			 else{
			  	DownloadBtn(1);
			  	return 2;
		 	}
		}
	}

	if ((xp >= T_YELLOWBUTTON_X) && (xp <= (T_YELLOWBUTTON_X + T_YELLOWBUTTON_W))) {
		if ((yp >= (T_YELLOWBUTTON_Y - T_YELLOWBUTTON_H)) && (yp <= (T_YELLOWBUTTON_Y ))) {
		  Serial.println("WIFI YELLOW btn hit");
		  WIFISelBtn(1);
		  return 3;
		}
	}

	if ((xp >= T_BLUEBUTTON_X) && (xp <= (T_BLUEBUTTON_X + T_BLUEBUTTON_W))) {
		if ((yp >= (T_BLUEBUTTON_Y - T_BLUEBUTTON_H)) && (yp <= (T_BLUEBUTTON_Y ))) {
		  Serial.println("TCH CAL BLUE btn hit");
		  TchCalBtn(1);
		  return 4;
		}
	}

	if ((xp >= T_SKYBLUEBUTTON_X) && (xp <= (T_SKYBLUEBUTTON_X + T_SKYBLUEBUTTON_W))) {
		if ((yp >= (T_SKYBLUEBUTTON_Y - T_SKYBLUEBUTTON_H)) && (yp <= (T_SKYBLUEBUTTON_Y ))) {
		  Serial.println("Setting ORANGE btn hit");
		  SettingBtn(1);
		  return 5;
		}
	}
	
	return 0;

}

void TftLCDFunClass::WifiTextDisplay(void)
{
	if(WiFiFnc.WifiWorking)
	{
		tft.setTextColor(TFT_YELLOW, TFT_BLACK);
		tft.setCursor(10, 200);
		tft.println("[IP] :                     [PORT] :");
		tft.setCursor(10, 220);
		tft.println("[SSID] :                     [PW] :");

		tft.setTextColor(TFT_RED, TFT_BLACK);

		tft.setCursor(50, 200);
//		tft.println(WiFiFnc.host);
		tft.println(WiFiFnc.Local_IpAddress);
		tft.setCursor(230, 200);
		tft.println(WiFiFnc.udpPort);

		tft.setTextPadding(40);  
		tft.drawString("ON", 290, 210);
		
		tft.setCursor(60, 220);
		tft.println(WiFiFnc.networkName);
		tft.setCursor(230, 220);
		tft.println(WiFiFnc.networkPswd);
	}
	else
	{
		tft.setTextColor(TFT_YELLOW, TFT_BLACK);
		tft.setCursor(10, 200);
		tft.println("[IP] :                     [PORT] :");
		tft.setCursor(10, 220);
		tft.println("[SSID] :                     [PW] :");

	
		tft.setTextColor(TFT_DARKGREY, TFT_BLACK);

		tft.setCursor(50, 200);
//		tft.println(WiFiFnc.host);
		tft.println(WiFiFnc.Local_IpAddress);
		tft.setCursor(230, 200);
		tft.println(WiFiFnc.udpPort);

		tft.setTextPadding(40);  
		tft.drawString("OFF", 290, 210);

		tft.setCursor(60, 220);
		tft.println(WiFiFnc.networkName);
		tft.setCursor(230, 220);
		tft.println(WiFiFnc.networkPswd);
	}
	

}


void TftLCDFunClass::WifibtnPressed(void)
{
	WiFiFnc.WifiWorking = !WiFiFnc.WifiWorking;

	WifiTextDisplay();

	Serial.printf("WifibtnPressed [WiFiFnc.WifiWorking] %d\n",WiFiFnc.WifiWorking);
	
}


void TftLCDFunClass::ReprogramInitVariable(void)
{
//	RFileFnc.CloseFile();
	
    RFileFnc.InitRepFileSetup(1);

	StartStop = true;
	SuccessFail = false;
	progresspercent = 0;
	ReprogmmingData.respSta = 1;
	ReprogmmingData.respTimeCount = 0;
	ReprogData.stepLock  = 0;
	ReprogData.stepLock1 = 0;	
	ReprogData.stepFirst = 0;
//	ReprogData.reprogHexSize = ReprogData.reprogFileHexSize;
	ReprogData.timerTick = 0;
	ReprogData.reprogStartStop = 1;  // Start Reprogramming
	ReprogData.reprogExpiredCheck = 0;
	ReprogmmingData.vfClearAllEnv(); 
}
