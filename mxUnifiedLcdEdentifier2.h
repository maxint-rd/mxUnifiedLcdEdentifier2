/*********************************************************************
This is a library for e.dentifier2 LCD display

Based on the Nokia 5110 library for Adafruit GFX
(Written by Limor Fried/Ladyada  for Adafruit Industries).

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution

Library adapted by Maxint R&D to drive Nokia 5110 display via an
I2C I/O expander or the 74HC595 shift register using the mxUnifiedIO API.
https://github.com/maxint-rd/mxUnifiedPIO
*********************************************************************/
#ifndef _MXUNIFIEDLCDEDENT2_H
#define _MXUNIFIEDLCDEDENT2_H

#if !defined(_MXUNIFIEDIO_DEBUG)
#define _MXUNIFIEDIO_DEBUG 0		// Note: Serial debugging not supported on ATtiny85 
#endif


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
#endif

//#include <Wire.h>
#include <SPI.h>
#include <mxUnifiedIO.h>
#include <Adafruit_GFX.h>


#define BLACK 1
#define WHITE 0

// actual height is 36 pixels, but need byte aligned room in buffer
#define LCDWIDTH 102
#define LCDHEIGHT 36
#define LCDROWS ((uint8_t)((LCDHEIGHT / 8.0) +0.5))		// ad 0.5 to round up to full bytes
//#define LCDROWFIXY 4
/*
//(((uint8_t)(LCDROWS)*8) - LCDHEIGHT)				// fix y position to compensate for half row
*/
#define LCDROWFIXY  (((LCDROWS)*8) - LCDHEIGHT)				// fix y position to compensate for half row 

// LCD commands (need further analysis to find precise meaning)
// Commands sent after powering on [2 bytes]
// Every command and data byte and gets two LOW clock pulses. WR-pin: 0.25us started just before, CS-pin: 0.125us right at start of each byte
// Timing of clock pulse is 8.5us to 13us.
// Command bytes have DC-pin set LOW: 0.25us started just before each command byte.
#define LCDEDENT2_INIT1_A 0x23
#define LCDEDENT2_INIT1_B 0x03

// Commands sent 2ms after INIT1  [10 bytes]
#define LCDEDENT2_INIT2_A 0x23
#define LCDEDENT2_INIT2_B 0x08
#define LCDEDENT2_INIT2_C 0x92
#define LCDEDENT2_INIT2_D 0x21
#define LCDEDENT2_INIT2_E 0x12
#define LCDEDENT2_INIT2_F 0x0C
#define LCDEDENT2_INIT2_G 0xa8
#define LCDEDENT2_INIT2_H 0x20
#define LCDEDENT2_INIT2_I 0x05


// These can be used to with LCDEDENT2_FUNCTIONSET to mirror the display in Y direction
// these comands are same as PCF8548
#define LCDEDENT2_MIRRORY 0x08
#define LCDEDENT2_MIRRORX 0x10		// unfortunately X is not supported on e.dentifier2 display!
#define LCDEDENT2_EXT_DISPCONF 0x08
#define LCDEDENT2_EXT_DISPCONF_DO 0x4			// D0=1: LSB is on top, D0=0: MSB is on top
#define LCDEDENT2_EXT_DISPCONF_TRS 0x2		// seem not supported on e.dentifier2 display!
#define LCDEDENT2_EXT_DISPCONF_BRS 0x1		// seem not supported on e.dentifier2 display!


// these comands are same as Nokia 5110, PCD8544
#define LCDEDENT2_BASICINSTRUCTION 0x0
#define LCDEDENT2_EXTENDEDINSTRUCTION 0x1

#define LCDEDENT2_DISPLAYBLANK 0x0
#define LCDEDENT2_DISPLAYNORMAL 0x4
#define LCDEDENT2_DISPLAYALLON 0x1
#define LCDEDENT2_DISPLAYINVERTED 0x5

// H = 0
#define LCDEDENT2_FUNCTIONSET 0x20
#define LCDEDENT2_DISPLAYCONTROL 0x08
#define LCDEDENT2_SETYADDR 0x40
#define LCDEDENT2_SETXADDR 0x80

// H = 1
#define LCDEDENT2_SETTEMP 0x04
#define LCDEDENT2_SETBIAS 0x10
#define LCDEDENT2_SETVOP 0x80



// Clear-all is send 2ms after INIT2 and consists of 10 numbered blocks
// Each block is preceeded by a numbered BLOCK1 command
#define LCDEDENT2_BLOCK1_ROW 0x40		// combined with blocknumbers 0-9: 0x40 - 0x49
#define LCDEDENT2_BLOCK1_COL 0x80
#define LCDEDENT2_BLOCK1_GO 0x07
// Directly (13us) after the block command, the block data is sent
// The clear data of each block is 102 bytes of 0x00

// 105ms after Clear-all another clear is sent
// This clear has five blocks (blocknumbers 2-6: 0x42 - 0x46)
// Directly after the block command, the block data is sent
// The clear data of each block is 102 bytes of 0x00
// Within 1ms (500us - 800us) after the second clear the e.dentifier2 sends display data

// When sending a text image, data is send in multiple blocks using different timings
// Each black pixel is sent as a HIGH bit, each white pixel as LOW.
// The text image is sent as five numbered blocks (blocknumbers 2-6: 0x42 - 0x46)
// Each block has 17x6 bytes of character data
// Blocks #2 and #3 have fast timing (13us or 25us between chars, 10us per byte, 1us low, 9us high)
// Blocks #4, #5 and #6 have slow timing: 37us bytes 1-4, 9us byte 5, 64us byte 6 + char pause

// When sending a bitmap image, data is send partially, starting at differen positions
// White is LOW, black is HIGH
// The ABN-AMRO logo is sent in three data blocks using different positions
// Each block has fast timing: 8.5us per byte (1us low, 7.5us high)
#define LCDEDENT2_BLOCK2_ROW 0x43
#define LCDEDENT2_BLOCK2_COL 0xC8
#define LCDEDENT2_BLOCK2_GO  0x07
#define LCDEDENT2_BLOCK2_END 0xA9

#define LCDEDENT2_BLOCK3_ROW 0x44
#define LCDEDENT2_BLOCK3_COL 0xC8
#define LCDEDENT2_BLOCK3_GO  0x07
#define LCDEDENT2_BLOCK3_END 0xA9

#define LCDEDENT2_BLOCK4_ROW 0x45
#define LCDEDENT2_BLOCK4_COL 0xCC
#define LCDEDENT2_BLOCK4_GO  0x07
#define LCDEDENT2_BLOCK3_END 0xAD

#define LCDEDENT2_SETYADDR 0x40
#define LCDEDENT2_SETXADDR 0x80
#define LCDEDENT2_DISPMODE 0x07		// 0x07


// Fastest SPI speed is 4MHZ (DIV2 on 328@8Mhz, DIV4 on 328@16Mhz)
#define LCDEDENT2_SPI_CLOCK_DIV SPI_CLOCK_DIV2

class mxUnifiedLcdEdentifier2 : public Adafruit_GFX
{
 public:
 	// To drive the LCD display eight datalines are required plus two control lines (DC and CLK)
 	// the constructors below offer various ways to define these lines.
 	
  // all pins via pins P0-P9 of an mxUnifiedIO expander (such as two cascaded 74HC595): data pins on expanded pins P0-P7, DC on P8 and write CLK on pin P9.
  mxUnifiedLcdEdentifier2(mxUnifiedIO *pUniOut);
  
  // data pins via pins P0-P7 of an mxUnifiedIO expander (such as I2C PCF8574) and control pins DC and CLK as regular MCU pins.
  mxUnifiedLcdEdentifier2(mxUnifiedIO *pUniOut, int8_t DC, int8_t CLK);

/* TODO:
  // data pins via two separate mxUnifiedIO interfaces with data pins on P0-P7 and control pins DC and CLK on second expand pins P0 and P1.
  mxUnifiedLcdEdentifier2(mxUnifiedIO *pUniOut, imxUnifiedIO *pUniOut2);
*/

  void begin(uint8_t contrast = 40, uint8_t bias = 0x04);
  void setContrast(uint8_t val);
	void invertDisplay(boolean f);
	void mirrorDisplay(boolean fMirrorX, boolean fMirrorY);
  void display();
	void clearDisplay(void);
  
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  uint8_t getPixel(int8_t x, int8_t y);

 private:
  int8_t _dc, _clk;
  bool _fUseMCUpins;		// use MCU pins for DC and CLK or use mxUnifiedIO pins DC=P8/CLK=P9
  mxUnifiedIO *_pUniOut;

  void digitWrite(uint8_t nPin, uint8_t nValue, bool fUseMCUpin=true);
	void digitWriteTwo(uint8_t nPin1, uint8_t nValue1, uint8_t nPin2, uint8_t nValue2);
	void pnMode(uint8_t, uint8_t, bool fUseMCUpin=true);

	void lcdClkPulse(bool fCmd, bool fStartPulse);
	void lcdWrite(bool fCmd, uint8_t nData);
	void lcdCmd(uint8_t nCmd);
	void lcdData(uint8_t nData);
	void lcdRowCol(uint8_t nRow=0, uint8_t nCol=0);
	void lcdClear();
	void lcdInit();
};

#endif
