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
//uint8_t dataPin  = 11;    // Yellow wire on Adafruit Pixels
//uint8_t clockPin = 13;    // Green wire on Adafruit Pixels

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
//If we don't declare dataPin and clockPin, the hardware MOSI and SCK pins are used
Adafruit_WS2801 strip = Adafruit_WS2801(56);//, dataPin, clockPin);

//Variables for reading analog input and calculating colours
uint32_t color = 0;
float color_maximum = 255.0;

//Define analog pins for reading potentiometer input
int input1 = A5;
int input2 = A6;
int input3 = A7;
float analog_resolution = 1023;

//Values for reading colours from analog inputs
float valueR = 0;
float valueG = 0;
float valueB = 0;

//Values for reading brightness and delay from analog inputs
float valueBrightness = 0;
float valueDelay = 0;
float maxdelay = 20;


//Declare interrupt routine variables
const byte interruptPin = 2;
bool rainbow_mode=1;


void setup() 
{
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  strip.begin();
  //Serial.begin(9600);

  // Update LED contents, to start they are all 'off'
  strip.show();

  //Interrupt pin
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changemode, FALLING);
}


void loop() 
{
  if(rainbow_mode==0)
  {
    //Read colour levels from analog pins
    valueR = 255.0*analogRead(input1)/analog_resolution;
    valueG = 255.0*analogRead(input2)/analog_resolution;
    valueB = 255.0*analogRead(input3)/analog_resolution;
  //  Serial.println(valueR);          
  //  Serial.println(valueG);
  //  Serial.println(valueB);
  //  Serial.println("");
  
    //Calculate colour and send to pixels
    color = Color(valueR,valueG,valueB);
    for(int i=0;i<strip.numPixels();i++)
    {
      strip.setPixelColor(i,color);
    }
    strip.show();
    delay(50); 
  }
  else if (rainbow_mode==1)
  {
    rainbowCycle();
  }
}

void changemode()
{
  rainbow_mode=!rainbow_mode;
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

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle()//uint8_t wait) 
{
  int i, j;
  
  for (j=0; j < 256; j++) 
  {     
    // A cycle of all 256 colors in the wheel

    //Read delay and brightness levels from analog pins
    valueDelay = maxdelay*(1+analogRead(input1))/(1+analog_resolution);
    //Serial.println(valueDelay);
    //Serial.println("");
    
    for (i=0; i < strip.numPixels(); i++) 
    {
      // tricky math! we use each pixel as a fraction of the full 256-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the %256 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ( (i*256/strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(valueDelay);
    if (rainbow_mode==0)
    {
      break;
    }
  }
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) 
  {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if (WheelPos < 170) 
  {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else 
  {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
