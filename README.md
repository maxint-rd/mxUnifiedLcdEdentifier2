# mxUnifiedLcdEdentifier2
mxUnifiedIO device library for Arduino. Device specific driver for the LCD in the e.dentifier2 bank card device, derived from the Adafruit GFX library. Use SPI shift registers or I2C I/O expanders via the mxUnifiedIO API.

DESCRIPTION
===========
This is a library for the LCD display found in the e.dentifier2 bank card reader. The e.dentifier2 is a banking security device, developed by Todos Sweden (now part of Gemalto) for the dutch ABN-AMRO bank. About 2.5 million of these devices have been distributed amongst customers. Since 2015 the device is partially replaced by the mobile app. Next to generating response codes for login and for transactions, the device can also be used to view the balance and history of the chipknip wallet, a payment system that was decomissioned on december 31, 2014. The USB interface of the device is intended to simplify using the device, but its implementation introduced a security vulnarability when connected. Nevertheless, the device is still in use.

The LCD panel of the device is marked "C41000169 A14 11 0E 019264". Unfortunately no public documentation of the LCD panel in the device was found. By reverse engineering sufficient information was gained to use the display in an 3.3V Arduino environment. The control commandset resembles that of Philips LCD control chips (such as the popular PCD8544) and other similar chips (such as the Sitronix ST7549T), but the LCD differs in functionality. For more information see the various sections below.

This library requires the Adafruit GFX library, the [mxUnifiedIO](https://github.com/maxint-rd/mxUnifiedIO) library and a suitable mxUnifiedIO expander driver. This driver was made using the Adafruit PCD8544 Nokia 5110 LCD library as example, but instead of directly using pins it allows the display to be driven via the expanded pins of an I2C I/O expander or a shift-register.

YouTube videos
==============
 - Looking inside the e.dentifier2 device: https://youtu.be/leq9zHxpAJc
 - Analysing and using the LCD: https://youtu.be/NXtHkjaIge0

LCD panel pinout
================
           135791357
     /-----"""""""""-----\
     +-------------------+ 
     |                   |
     | 12345678901234567 |
     | 2                 |
     | 3                 |
     | 4                 |
     | '~`^"             |
     +-------------------+

The e.dentifier2 LCD panel is a 102x36 pixels graphical display with a chip on glass (COG). The connector has 18 pins which are not fully understood yet. By experiments the following minimal connections were found working using a 3.3v Arduino 328 @ 8MHz and a 74HC595 shift register.

| pin(s)   |name    |description |
|---------|---------|------------|
|  1,2,5 	|	3V VCC  |	POWER+. Connect 1 to 2 and 5, then to Arduino 3.3V |
|  3,4	   |	WR_CLK	 |	Write clock.	Connect 3 to 4 and to (expanded) pin 8 |
|  6   	 	|	DC	     |	Data/command. Connect to (expanded) pin 9 |
|  7-14  	|	D7-D0   |	Data pins.	Connect to expanded pins 0-7 |
|  15    	|	GND		 		|	POWER-. Connect to Arduino GND 0V |
|  16    	|	LCDCAP  | LCD Contrast.	Connect to GND via 100nF Capacitor. 10K resistor may work too. |
|  17-18  |	NC	     |	Not connected (-3V - 5V) output of LCD chargepump |

(See files in [documentation](/documentation) for more info)


Using SPI shift-register
========================
The (shared) SPI bus requires three data-lines to connect one or more shift registers (SS, MOSI and SCK). On the Arduino UNO, Nano and Pro Mini, the pins for (fast) hardware SPI are
D10, D11 and D13. On the ESP8266 the default SPI pins are GPIO15, GPIO13 and GPIO14. When using (slower) software SPI others can be selected too.

This driver supports using one shift-register for the datapins in combination with two regular pins for WR and DC. Alternatively two cascaded registers can be using in which case the datapins
are all supplied by the ending (first) register and the WR and DC by the second. For the shift-register the mxUnified74HC595 driver is used over SPI.

See the included example for suggested connections of the display module to the shift register(s).

Using I2C I/O expander
======================
The (shared) I2C bus only requires two data-lines (SDA and SCL). On the Arduino UNO, Nano and Pro Mini, these can be found on pins A4 and A5. On the ESP8266 the default pins are GPIO4 and GPIO5, but others can be selected too.

To drive this LCD module, the I/O expander needs to have at least 8 output pins available, all at the same H/L level. For that reason the cheap I2C backpack board can not be used for this display.

See the examples in mxUnifiedPCD8544_Nokia_5110_LCD for suggested connections of the display module to an I2C LCD driver interface.
This LCD driver library has not been tested with an I2C I/O expander yet, but should function just as well as when using a shift register.

CREDITS
========
1. Credits to Adafruit for creating the original PCD8544 library and the required Adafruit GFX library.
Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

  Pick a Nokia 5110 display up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

Original version written by Limor Fried/Ladyada for Adafruit Industries. BSD license, check license.txt for more information
All text above must be included in any redistribution.

2. Modified library by Maxint R&D 2017. 
BSD license applies. See below for more information.


INSTALLATION
============
Download the required libraries (see links below). Use the Arduino IDE to add the .ZIP library file via the menu.
Alternatively copy all library files into a folder under your <arduinosketchfolder>/libraries/ folder and restart the IDE.

You will also have to install the Adafruit GFX Graphics core library which does all the circles, text, rectangles, etc; as well as the
mxUnifiedIO library. Also install either the mxUnifiedPCF8574 library or the mxUnified74HC595 libraries, which are used to drive the LCD chip.

See the included examples to learn more about using this library.


TROUBLESHOOTING TIPS
====================
- Use the I2C scanner sketch to see if the PCF8574 I2C LCD driver is properly connected and responding.
- Check if all data-pins between display and interface modules are properly connected, according the pins as defined in your sketch.
- Use a multimeter to check pin voltages. Note: the e.dentifier2 is a 3V LCD display. It has been tested to work on 3.3V. It may fail when using higher voltages.

Features & limitations
======================
- The current version of this library supports ESP8266 and Atmel ATmega328 MCUs. On some experiments using an ESP-01 occasional noise was observed, possibly due to power issues. Noise may cause wild pixels on the display or errors initializing properly, resulting in nothing displayed or in distorted images, depending on which bits flipped when.
- Currently the LCD commands are not decisively fully known. The command-set resembles that of the Nokia 5110 (PCD8544) and other Philips LCD controllers as well as similar chips such as the Sitronix ST7549T. Multiple controller manufacturers are using similar protocols, which can be recognized for instance by their addressing scheme (0x4xxx and 0x8xxx for rows and columns). Based on the datasheets of the Philips and Sitronix controllers more commands were discovered (such as inverting and setting contrast). All commands used by the e.dentifier2 device have been identified (see documentation) but more commands could be applicable to this LCD. 

LINKS
=====
Required libraries:
- This library:
    * https://github.com/maxint-rd/mxUnifiedLcdEdentifier2
- Supporting libraries:
    * https://github.com/maxint-rd/mxUnifiedIO
    * https://github.com/maxint-rd/mxUnifiedPCF8574
    * https://github.com/maxint-rd/mxUnified74HC595
- Adafruit GFX Library: 
    * https://github.com/adafruit/Adafruit-GFX-Library

For reference:
- Rob Tillaarts PCF8574 library:
    * https://github.com/RobTillaart/Arduino/tree/master/libraries/PCF8574
    * https://playground.arduino.cc/Main/PCF8574Class
- Adafruit tutorials:
    * https://learn.adafruit.com/nokia-5110-3310-monochrome-lcd
    * https://learn.adafruit.com/adafruit-gfx-graphics-library/overview
- Arduino I2C scanner sketches:
    * https://playground.arduino.cc/Main/I2cScanner
- The original Adafruit PCD8544 Nokia 5110 LCD SPI library:
    * https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library

More about the e.dentifier2 device:
 - http://nl.wikipedia.org/wiki/E.dentifier
 - http://nl.wikipedia.org/wiki/Chipknip
 - http://www.ru.nl/publish/pages/769526/2011_arjanblom.pdf


LICENSE
=======
This library inherits the BSD license from the original library which limits the usage of this library to specific terms.
Read license.txt for more information.

Disclaimer
==========
All code on this GitHub account, including this library is provided to you on an as-is basis without guarantees and with all liability dismissed. It may be used at your own risk. Unfortunately I have no means to provide support.
