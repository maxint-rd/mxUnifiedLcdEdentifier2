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

#if defined(ESP8266)
#include <pgmspace.h>
#endif
#ifdef __AVR__
#include <avr/pgmspace.h>
#endif
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifdef __AVR__
  #include <util/delay.h>
#endif

#ifndef _BV
  #define _BV(x) (1 << (x))
#endif

#include <stdlib.h>

#include "mxUnifiedLcdEdentifier2.h"

// the memory buffer for the LCD
uint8_t lcdedent2_buffer[LCDWIDTH * LCDROWS] = {
/*
// row 0x46 - bottom row (row5), right to left, only top part (high nibble) shown, top pixel in highest bit
0xF0, 0x12, 0x14, 0x18, 0x10, 0x20,
0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x10, 0x10, 0x10, 0xF0,	// ending left of bottom row

// row 0x45 - second row from bottom (row 4)
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

// row 0x44 - third row
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

// row 0x43 - second row
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

// row 0x42 - top row (right to left)
0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xFF,
*/

/* Adafruit logo (orinal for Nokia 5110 84x48 modified to 102*36), added edges */
// row 0x46 bottom (row5), right to left, only top part (high nibble) shown, top pixel in highest bit
// row 1 of upside down image							|----- ** -------|
0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xFC, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 

// ending left of bottom row

// row 0x45 second from bottom (row 4)
// row 2 of upside down image							| xM ******      |
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00, 0x00, 0xFF, 0x40, 0x20, 0x10, 0x20, 0x40, 0xFF,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0xC0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 

// row 0x44 - third 
// row 3 of upside down image							|    ********    |
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x87, 0x8F, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF,
0xC1, 0xC0, 0xE0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE,
0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 

// row 0x43 - second 
// row 4 of upside down image							|      ******    |
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0xC0, 0xE0, 0xF1, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x0F, 0x87,
0xE7, 0xFF, 0xFF, 0xFF, 0x1F, 0x1F, 0x3F, 0xF9, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 

// row 0x42 - top (right to left, top pixel in highest bit) - topside is side with connector
// row 5 of upside down image							/----- ** * ----\    <-- NOTE: space is required after literal backslash in comment!
0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
0x00, 0x00, 0x00, 0xF0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
0x7E, 0x3F, 0x3F, 0x0F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xE0, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFC, 0xF0, 0x01, 0x01, 0x01, 0x01, 0x81, 0x81, 0x81, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80,
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40,
0x20, 0x10, 0x08, 0x04, 0x02, 0x01,


/* Adafruit logo (orinal for Nokia 5110 84x48)
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xFC, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0xC0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x87, 0x8F, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF,
0xC1, 0xC0, 0xE0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE,
0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0xC0, 0xE0, 0xF1, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x0F, 0x87,
0xE7, 0xFF, 0xFF, 0xFF, 0x1F, 0x1F, 0x3F, 0xF9, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xF0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
0x7E, 0x3F, 0x3F, 0x0F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xE0, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFC, 0xF0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01,
0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
*/
};


// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and optimized
//#define enablePartialUpdate

#ifdef enablePartialUpdate
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
#endif



static void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) {
#ifdef enablePartialUpdate
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
#endif
}

//
// Constructors
//

mxUnifiedLcdEdentifier2::mxUnifiedLcdEdentifier2(mxUnifiedIO *pUniOut) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT)
{	// all pins via mxUnifiedIO: data pins on expanded pins P0-P7, write DC on pin P8 and CLK on P9.
	_pUniOut=pUniOut;

  _dc = 8;
  _clk = 9;
  _fUseMCUpins=false;
}


mxUnifiedLcdEdentifier2::mxUnifiedLcdEdentifier2(mxUnifiedIO *pUniOut, int8_t DC, int8_t CLK) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT)
{	// data pins via mxUnifiedIO expander (such as I2C PCF8574) with data pins on expanded pins P0-P7 and control pins DC and CLK as regular pins.
	_pUniOut=pUniOut;

  _dc = DC;
  _clk = CLK;
  _fUseMCUpins=true;
}



//
// Private helper methods
//
void mxUnifiedLcdEdentifier2::lcdClkPulse(bool fCmd, bool fStartPulse)
{	// send a clock pulse with or without setting DC pin
	// TODO: when using two cascaded shiftregisters, sending CMD before CLK is
	// significantly slower (250us instead of 150 without CMD)
	// This can perhaps be improved by combining both together using setBit and sendBits.
	// Note that using two MCU pins is abouth 90% faster than two expanded pins.
	if(fStartPulse)
	{
	  if(fCmd) digitWrite(_dc, LOW, _fUseMCUpins);
	  digitWrite(_clk, LOW, _fUseMCUpins);
	}
	else
	{
	  digitWrite(_clk, HIGH, _fUseMCUpins);
	  if(fCmd) digitWrite(_dc, HIGH, _fUseMCUpins);
	}
}

void mxUnifiedLcdEdentifier2::lcdWrite(bool fCmd, uint8_t nData)
{ //  send a command to the lcd

  // flip the clock pin and the cmd pin as needed
  lcdClkPulse(fCmd, true);

  // send the data using the io expander
  _pUniOut->set8Bits(nData);
  _pUniOut->sendBits();
  // flip the clock pin and the cmd pin back as needed
  lcdClkPulse(fCmd, false);

  delayMicroseconds(1);
  _pUniOut->set8Bits(0xFF);
  _pUniOut->sendBits();
  //_pUniOut->send8Bits();

  // set cmd pin
  delayMicroseconds(9);
}

void mxUnifiedLcdEdentifier2::lcdCmd(uint8_t nCmd)
{
  lcdWrite(true, nCmd);
}

void mxUnifiedLcdEdentifier2::lcdData(uint8_t nData)
{
  lcdWrite(false, nData);
}

void mxUnifiedLcdEdentifier2::lcdRowCol(uint8_t nRow, uint8_t nCol)		//nRow=0, nCol=0
{
  lcdWrite(true, LCDEDENT2_SETYADDR | nRow);
  lcdWrite(true, LCDEDENT2_SETXADDR | nCol);
  lcdWrite(true, LCDEDENT2_DISPMODE);			// driver also seems to work when this command isn't sent
}


void mxUnifiedLcdEdentifier2::lcdClear()
{
  // clear the lcd
  for(uint8_t nRow=2; nRow<=6; nRow++)
  {
		lcdRowCol(nRow);

    for(uint8_t n=0; n<102; n++)
      lcdData(0x00);
    delayMicroseconds(50);
  }
}

void mxUnifiedLcdEdentifier2::lcdInit()
{
  pnMode(_dc, OUTPUT, _fUseMCUpins);
  digitWrite(_dc, HIGH, _fUseMCUpins);
  pnMode(_clk, OUTPUT, _fUseMCUpins);
  digitWrite(_clk, HIGH, _fUseMCUpins);
  _pUniOut->set8Bits(0xFF);
  _pUniOut->sendBits();
//  _pUniOut->send8Bits();

//  delay(100);

  // send init/reset commands
  lcdCmd(0x23);
  lcdCmd(0x03);

  // wait
  delay(2);

  // send init block commands
  lcdCmd(0x23);
  lcdCmd(0x08);
  lcdCmd(0x92);
  lcdCmd(0x21);
  lcdCmd(0x12);
  lcdCmd(0x0C);
  lcdCmd(0xA8);
  lcdCmd(0x20);
  lcdCmd(0x05);
  lcdCmd(0x0C);

  // send init block data
  delayMicroseconds(15);
  for(uint8_t nRow=0; nRow<10; nRow++)
  {
		lcdRowCol(nRow);

    for(uint8_t n=0; n<102; n++)
      lcdData(0x00);
  }

/*
  // wait
  delay(10);
  lcdClear();
*/
}


//
// Public methods
//

// the most basic function, set a single pixel
void mxUnifiedLcdEdentifier2::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;

  int16_t t;
  switch(rotation){
    case 1:
      t = x;
      x = y;
      y =  LCDHEIGHT - 1 - t;
      break;
    case 2:
      x = LCDWIDTH - 1 - x;
      y = LCDHEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = LCDWIDTH - 1 - y;
      y = t;
      break;
  }

  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return;

  // x is which column
  y+=LCDROWFIXY;		//fix rounding half row
  if (color) 
    lcdedent2_buffer[x+ (y/8)*LCDWIDTH] |= _BV(y%8);  
  else
    lcdedent2_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8); 

  updateBoundingBox(x,y,x,y);
}


// the most basic function, get a single pixel
uint8_t mxUnifiedLcdEdentifier2::getPixel(int8_t x, int8_t y) {
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return 0;

  y+=LCDROWFIXY;		//fix rounding half row
  return (lcdedent2_buffer[x+ (y/8)*LCDWIDTH] >> (y%8)) & 0x1;  
}

void mxUnifiedLcdEdentifier2::begin(uint8_t contrast, uint8_t bias)
{
	//Serial.println(F("mxUnifiedLcdEdentifier2 begin"));
	lcdInit();

  // set up a bounding box for screen updates
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  // Push out lcdedent2_buffer to the Display (will show the AFI logo)
  display();
}


void mxUnifiedLcdEdentifier2::setContrast(uint8_t val)
{	// TODO

}


void mxUnifiedLcdEdentifier2::invertDisplay(boolean i)
{	// TODO
}

// clear everything
void mxUnifiedLcdEdentifier2::clearDisplay(void)
{
  memset(lcdedent2_buffer, 0, LCDWIDTH*LCDROWS);
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  cursor_y = cursor_x = 0;
}


void mxUnifiedLcdEdentifier2::display(void)
{
  uint8_t col, maxcol, p;
  
  for(p = 0; p < LCDROWS; p++) {		// Adafruit magic 6 is page (y-row) number
#ifdef enablePartialUpdate
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
#endif

    //lcdCmd(LCDEDENT2_SETYADDR | p);


#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;
#endif

		//lcdRowCol(p+2, col);			// first visible row is row 2
		lcdRowCol(LCDROWS-p+1, col);			// reversed rows to cope for LSBFIRST bit order, first visible row is row 2
    //lcdCmd(LCDEDENT2_SETXADDR | col);
    //lcdCmd(LCDEDENT2_BLACKONH);

		// for speed optimisation changing the _dc and _cs pins are combined and the display data transmission is bundled
		//digitWriteTwo(_dc, HIGH, _cs, LOW);
		//digitWrite(_dc, HIGH);

    _pUniOut->startTransmission();
    uint16_t uCnt=0;
    for(; col <= maxcol; col++)
    {
  		//Serial.print(F("W"));
	  	lcdData(lcdedent2_buffer[(LCDWIDTH*(p))+col]);
  		uCnt++;
  		if(uCnt%2==0 && col<maxcol-1)
 			{	// can't send too much in one go because I2C has limited buffer (32 bytes) on both ATmega and ESP8266
 				// Combining more than two column-bytes per transmission causes data to be lost, even when running at lower speeds.
 				// Note that for each byte 16 clk changes are required, meaning 16 I2C bytes are send per byte.
 				// No error (e.g. for buffer overflow) is given, but data is definitely not complete.
 				// (ending every 4th column gives half complete columns, every 8th gives quarter screen)
 				// possibility to combine depends on number of _pUniOut->write() calls.
	  		//Serial.print(F("E"));
			  _pUniOut->endTransmission();
	  		//Serial.print(F("B"));
		    _pUniOut->startTransmission();
		  }
    }
	  _pUniOut->endTransmission();

    //if (_cs > 0)
    //  digitWrite(_cs, HIGH);
#ifdef ESP8266
		// Since the display method takes the longest time on the relative slow I2C transmission,
		// the watchdog of the ESP may bite after repetitive calls
		// This is a good moment to feed that dog and keep it happy!
		yield();
#endif
  }

#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif
}

void mxUnifiedLcdEdentifier2::digitWrite(uint8_t nPin, uint8_t nValue, bool fUseMCUpin)		// fUseMCUpin=true
{	// set a pin of the I2C I/O expander/shiftregister
  //Serial.println(F("mxUnifiedLcdEdentifier2::digitWrite"));
#if(_MXUNIFIEDIO_DEBUG)
  Serial.print(F("mxUnifiedLcdEdentifier2::digitWrite("));
#endif
/*
  _pUniOut->startTransmission();
  _pUniOut->setBit(nPin, nValue);
  _pUniOut->sendBits();
  _pUniOut->endTransmission();
*/
	if(fUseMCUpin)
		::digitalWrite(nPin, nValue);		// call the regular digitalWrite using the global scope resolution operator
	else
	{
#if(_MXUNIFIEDIO_DEBUG)
  Serial.print(nPin);
  Serial.print(F(","));
  Serial.print(nValue);
  //Serial.println(F(")"));
#endif
		_pUniOut->digitalWrite(nPin, nValue);
	}
#if(_MXUNIFIEDIO_DEBUG)
  Serial.print(F(", mcupin="));
  Serial.print(fUseMCUpin?"T":"F");
  Serial.println(F(")"));
#endif
}

void mxUnifiedLcdEdentifier2::digitWriteTwo(uint8_t nPin1, uint8_t nValue1, uint8_t nPin2, uint8_t nValue2)
{	// set the status of two pins and write them
  _pUniOut->startTransmission();
  _pUniOut->setBit(nPin1, nValue1);
  _pUniOut->setBit(nPin2, nValue2);
  _pUniOut->sendBits();
  _pUniOut->endTransmission();
}

void mxUnifiedLcdEdentifier2::pnMode(uint8_t nPin, uint8_t nMode, bool fUseMCUpin)		// fUseMCUpin=true
{
	// currently only output pins are supported by mxUnifiedIO
	if(fUseMCUpin)
		::pinMode(nPin, nMode);		// call the regular pinMode using the global scope resolution operator
	else
		_pUniOut->pinMode(nPin, nMode);
}