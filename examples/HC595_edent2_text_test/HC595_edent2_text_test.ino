/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution

Example sketch adapted by Maxint R&D to demonstrate I2C driven display
https://github.com/maxint-rd/I2C-PCF8574-PCD8544-Nokia-5110-LCD
*********************************************************************/

#include <mxUnified74HC595.h>
#include <mxUnifiedLcdEdentifier2.h>

// lets test using another font
#include <Fonts/FreeSansOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>


mxUnified74HC595 unio = mxUnified74HC595();                  // use hardware SPI pins, no cascading (requires additional pins for DC and CLK
//mxUnified74HC595 unio = mxUnified74HC595(2);               // use hardware SPI pins, two cascaded shift-registers (slightly slower, but more pins)

//mxUnified74HC595 unio = mxUnified74HC595(3, 2, 0);      // alternative software SPI pins for ESP-01: SS, MOSI, SCLK, no cascading (slow, but pin-freedom)
//mxUnified74HC595 unio = mxUnified74HC595(3, 2, 0, 2);      // alternative software SPI pins for ESP-01: SS, MOSI, SCLK,  two cascaded shift-registers (slow, but pin-freedom)

// The easiest way to connect the e.dentifier2 LCD to the 74HC595 shift register on a
// breadboard is to use a shiftregister with GND next to pins P0-P6 add wires to P7, D8/D9 to DC/CLK and 3.3V to VCC.
//mxUnifiedLcdEdentifier2 display = mxUnifiedLcdEdentifier2(&unio);         // e.dentifier2 LCD: datapins P0-P7, DC=P8, CLK=P9 (all expander pins)
mxUnifiedLcdEdentifier2 display = mxUnifiedLcdEdentifier2(&unio, 8, 9);         // e.dentifier2 LCD: datapins P0-P7, DC=D8, CLK=D9 (DC and CLK are MCU pins)

#define LED_BLINK 2       // LED_BUILTIN=GPIO1 (TX) on ESP-01, GPIO2 on ESP-12E, 13 on Uno (should be redefined to 2 to avoid conflict)

#define XPOS 0
#define YPOS 1
#define DELTAY 2

void delayBlink(int nDelay, int nPin=LED_BLINK)
{ // delay some msecs and blink on/off at half delay interval
  unio.digitalWrite(12, HIGH);
  unio.digitalWrite(0, HIGH);
  digitalWrite(nPin, HIGH);
  delay(nDelay/2);
  unio.digitalWrite(12, LOW);
  unio.digitalWrite(0, LOW);
  digitalWrite(nPin, LOW);
  delay(nDelay/2);
}

void setup()
{
  pinMode(LED_BLINK, OUTPUT);
  Serial.begin(115200);             // For ESP-01 board: Disable Serial.begin and call display.begin specifying GPIO 1 and 3.
  Serial.println(F("\n\n"));
  Serial.println(F("HC595_edent2_test_text"));

  uint32_t tStart=millis();
  unio.begin();     // start using the mxUnified74HC595 shift register
  unio.setBitOrder(LSBFIRST);        // change bitOrder to match pin layout of 595 shiftregister with display (LSB next to GND is Qh of 595)

  display.clearDisplay();   // clear buffer to skip splash-screen
  display.begin();  // regular begin() using default settings
  //display.clearDisplay();   // clears the screen and buffer
  //display.display();
  delayBlink(100);

  // Changing the contrast is not supported on e.dentifier2 LCD
  // display.setContrast(50);

  // text display tests using regular font
  display.setRotation(2);  // rotate 180 degrees counter clockwise, can also use values of 2 and 3 to go further.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hallo");
  display.setTextSize(2);     // NOTE: wrapped size 2 big text crashes on 168 and blocks all further display! Not on 328, so probably memory related
  display.setTextColor(BLACK);
  display.println("YouTube");
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.setCursor(0,28);    // the e.dentifier2 LCD is 102*36
  display.println("   Maxint 2017   ");
  display.display();
  delayBlink(2000);

  // show text using another font
  display.clearDisplay();
  display.setFont(&FreeSerif9pt7b);
  display.setTextColor(BLACK);
  display.setCursor(0,14);    // different origin-system on fonts in rotated display
  display.print("Hallo");
  display.setFont(&FreeSansOblique12pt7b);
  display.setCursor(0,34);    // different origin-system on fonts in rotated display
  display.print("YouTube");
  display.display();
  delayBlink(2000);

  // invert display
  for(int i=0; i<5; i++)
  {
    display.invertDisplay(true);
    delayBlink(200);
    display.invertDisplay(false);
    delayBlink(200);
  }

  // mirror display
  // unfortunately mirror X is not supported on e.dentifier2 display!
  for(int i=0; i<5; i++)
  {
    display.mirrorDisplay(false, true);
    delayBlink(400);
    display.mirrorDisplay(false, false);
    delayBlink(200);
  }



  // change contrast
  for(uint8_t i=10; i<120; i++)
  {
    display.setContrast(i);
    delayBlink(50);
  }
  display.setContrast(40);
  delayBlink(1000);


  // show characters  using regular font
  display.clearDisplay();
  display.setFont();  // revert back to the regular font
  display.clearDisplay();
  testdrawchar();
  delayBlink(2000);

  // rotation example
  display.clearDisplay();
  display.setRotation(1);  // rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello");
  display.setTextSize(2);
  display.println("YouTube");
  display.display();
  delayBlink(500);

  display.setRotation(0);  
  display.clearDisplay();   // clears the screen and buffer
  display.println("Hello");
  display.setTextSize(2);
  display.println("YouTube");
  display.display();
  delayBlink(500);

  display.setRotation(3);  
  display.clearDisplay();   // clears the screen and buffer
  display.println("Hello");
  display.setTextSize(2);
  display.println("YouTube");
  display.display();
  delayBlink(500);

  display.setRotation(0);  
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(1);
  display.println("Hallo");
  display.setTextSize(2);     // NOTE: wrapped size 2 big text crashes on 168 and blocks all further display! Not on 328, so probably memory related
  display.setTextColor(BLACK);
  display.println("YouTube");
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.setCursor(0,28);    // the e.dentifier2 LCD is 102*36
  display.println("   Maxint 2017   ");
  display.display();
  
  delayBlink(2000);

  // revert back to no rotation
  display.setRotation(0);

  // revert back to the regular font
  display.setFont();
  display.clearDisplay();   // clears the screen and buffer
  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(BLACK);
}

int nCnt=0;
void loop()
{
  nCnt++;
  bool fInverse=nCnt%2==1;
  display.setTextColor(fInverse?BLACK:WHITE, fInverse?WHITE:BLACK);
  display.print("Hello YouTube");
  display.display();
  delayBlink(200); 
  if(nCnt>8)
  {
    nCnt-=random(8);
    display.setRotation(nCnt%4);
    display.setCursor(nCnt%6,nCnt%8);
  }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    //display.writeChar(i);
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
      //display.println();
  }    
  display.display();
}
