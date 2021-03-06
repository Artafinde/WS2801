#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

/*****************************************************************************
Example sketch for driving Adafruit WS2801 pixels!


  Designed specifically to work with the Adafruit RGB Pixels!
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

  These pixels use SPI to transmit the color data, and have built in
  high speed PWM drivers for 24 bit color per pixel
  2 pins are required to interface

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

*****************************************************************************/

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(56, dataPin, clockPin);

uint32_t color = 0;
float color_maximum = 255.0;

//Define analog pins for reading potentiometer input
int PinR = A5;
float valueR = 0;
int PinG = A6;
float valueG = 0;
int PinB = A7;
float valueB = 0;
float analog_resolution = 1023;

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin();
  //Serial.begin(9600);

  // Update LED contents, to start they are all 'off'
  strip.show();
}


void loop() {
  //Read colour levels from analog pins
  valueR = 255.0*analogRead(PinR)/analog_resolution;
  valueG = 255.0*analogRead(PinG)/analog_resolution;
  valueB = 255.0*analogRead(PinB)/analog_resolution;

  //Calculate colour
  color = Color(valueR,valueG,valueB);

//  Serial.println(valueR);          
//  Serial.println(valueG);
//  Serial.println(valueB);
//  Serial.println("");
  
  for(int i=0;i<strip.numPixels();i++)
  {
    strip.setPixelColor(i,color);
  }
  
  strip.show();
  delay(50);
  


}


//Define color function and initial color
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
