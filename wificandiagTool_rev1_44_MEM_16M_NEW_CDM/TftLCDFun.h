#ifndef TftLCDFun_h
#define TftLCDFun_h

#include "FS.h"
#include "RepFileFunc.h"
#include "Reprogramming.h"

#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

// Additional functions
#include "GfxUi.h"         // Attached to this sketch


class TftLCDFunClass 
{
public:
	void InitTftLCDClass(void);
	void touch_calibrate(void);
	void drawFrame(void);
	void VersionConfirmBtn(uint8_t press);
    void DownloadBtn(uint8_t press);
	void WIFISelBtn(uint8_t press);
	void TchCalBtn(uint8_t press);
    void SettingBtn(uint8_t press);
	void drawProgress(uint8_t percentage, String text,uint8_t mode);
    void TouchBtnCheck(void) ;
	int  isPressBtn(uint16_t xp,uint16_t yp);
	void ReprogramInitVariable(void);
	void WifibtnPressed(void);
	void WifiTextDisplay(void);
		
public:
	TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

	TFT_eSprite graph1 = TFT_eSprite(&tft); // Sprite object graph1
	TFT_eSprite stext1 = TFT_eSprite(&tft); // Sprite object stext1
	TFT_eSprite stext2 = TFT_eSprite(&tft); // Sprite object stext2

	GfxUi ui = GfxUi(&tft); // Jpeg and bmpDraw functions TODO: pull outside of a class
	
public:
	int graphVal = 1;
	int delta = 1;
	int grid = 0;
	int tcount = 0;
	
	int timercount 		= 0;
	int TouchPressd     = 0;
//	int progresspercent = 0;
	float progresspercent = 0;
	float OTAprogresspercent = 0;
	float percent = 0;
	float otapercent = 0;
	
	int progresspercentSuccess = 0;
	int otaprogresspercentSuccess = 0;
	String sPercent;
	bool StartStop     = false;	
	bool SuccessFail   = false;	

	bool OtaStartStop	   = false; 
	bool OtaSuccessFail   = false; 
	
};

extern TftLCDFunClass TftLCDFun;

#endif // RepFileFunc_h
