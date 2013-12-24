/*
 * XM4STR33
 * By Sebastian Delmont <sd@notso.net>
 *
 * Based in part on AdaFruit's NeoPixel example files.
 *
 */
 
#include <Adafruit_NeoPixel.h>

//#include "pattern.h"
//#include "pattern_run.h"

#define DEBUG 1

#define NEOPIXEL_DATA_PIN 6

#define NEOPIXEL_PIXEL_COUNT 300

// not sure why, but my 'millis' calls seem to be half as fast as expected, hence the * 50 instead of * 100
#define CALCULATED_STEP(now,freq,steps) ((now * 100 / (freq * 50 / steps)) % steps)

#define ONE_SECOND 1000
#define ONE_SECONDS 1000
#define TWO_SECONDS 2000
#define ONE_AND_HALF_SECOND 1500
#define THREE_SECONDS 3000
#define FOUR_SECONDS 4000


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_PIXEL_COUNT, NEOPIXEL_DATA_PIN, NEO_GRB + NEO_KHZ800);

//Pattern * pattern;

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("\r\nStart");
  #endif

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  #ifdef DEBUG
  Serial.println("setup done");
  #endif
}

#define COLOR_BLACK strip.Color(0, 0, 0)
#define COLOR_WHITE strip.Color(255, 255, 255)
#define COLOR_RED   strip.Color(255, 0, 0)
#define COLOR_GREEN strip.Color(0, 255, 0)
#define COLOR_BLUE  strip.Color(0, 0, 255)
#define COLOR_YELLOW  strip.Color(255, 255, 0)
#define COLOR_CYAN  strip.Color(0, 255, 255)
#define COLOR_MAGENTA  strip.Color(255, 0, 255)

// Gamma correction improves appearance of midrange colors
const uint8_t PROGMEM gamma8[] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
      1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
      3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
      7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
     13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
     20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
     30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
     42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
     58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
     76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
     97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
    122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
    150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
    182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
    218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255
};
#define ADJUSTED_COLOR(R,G,B) strip.Color(pgm_read_byte(&gamma8[R]), pgm_read_byte(&gamma8[G]), pgm_read_byte(&gamma8[B]))

unsigned long (*patternFunc)(unsigned long now, unsigned long freq, unsigned long prevStep) = &pattern_RainbowFart;
unsigned long patternFrequency = FOUR_SECONDS;
unsigned long patternStep = 0;

void loop() {
  
  unsigned long now = 0;
  
  while(true) {
    #ifdef DEBUG
    Serial.print("loop   - ");
    #endif
    
    now = millis();
    patternStep = (*patternFunc)(now, patternFrequency, patternStep);

    #ifdef DEBUG
    unsigned long t = millis() - now;
    Serial.print("Time: ");
    Serial.print(now);
    Serial.print(" step: ");
    Serial.print(patternStep);
    Serial.print(" (");
    Serial.print(t);
    Serial.print("ms proc time) ");
    Serial.println("");
    #endif
  }
}


void pattern_WhiteFade(unsigned long now) {
//  for(i = 0; i < strip.numPixels(); i++) {
//    strip.setPixelColor(i, Wheel((i + j) & 255));
//  }
}

void pattern_WhiteScroll(unsigned long now) {
  for (int i = 0; i < 40; i++) {
    colorStrips3(strip.Color(180, 180, 180), strip.Color(0, 0, 0), strip.Color(0, 0, 0), 500);
    colorStrips3(strip.Color(0, 0, 0), strip.Color(180, 180, 180), strip.Color(0, 0, 0), 500);
    colorStrips3(strip.Color(0, 0, 0), strip.Color(0, 0, 0), strip.Color(180, 180, 180), 500);
  }
}

void pattern_RedGreenScroll(unsigned long now) {
  for (int i = 0; i < 40; i++) {
    colorStrips3(strip.Color(0, 200, 0), strip.Color(0, 200, 0), strip.Color(255, 0, 0), 500);
    colorStrips3(strip.Color(255, 0, 0), strip.Color(0, 200, 0), strip.Color(0, 200, 0), 500);
    colorStrips3(strip.Color(0, 200, 0), strip.Color(255, 0, 0), strip.Color(0, 200, 0), 500);
  }
}

unsigned long pattern_Run(unsigned long now, unsigned long freq, unsigned long prevOffset, uint32_t color0, uint32_t color1) {
  unsigned long offset = CALCULATED_STEP(now, freq, strip.numPixels());

  uint16_t i, y;

  for(i = 0; i < strip.numPixels(); i++) {
    y = abs(i - offset);
    if (y < 6)
      strip.setPixelColor(i, color0);
    else if (y > 20 && y < 26)
      strip.setPixelColor(i, color1);
    else
      strip.setPixelColor(i, 0UL);

  }
  strip.show();
  
  return offset;
}

unsigned long  pattern_RainbowCycle(unsigned long now, unsigned long freq, unsigned long prevOffset) {
  unsigned long offset = CALCULATED_STEP(now, freq, 256);
  
//  if (offset != prevOffset) {
    for(int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + offset) % 256));
    }
    strip.show();
//  }
  return offset;
}

unsigned long  pattern_RainbowFart(unsigned long now, unsigned long freq, unsigned long prevStep) {
  int pixels = strip.numPixels();
  unsigned long thisStep = CALCULATED_STEP(now, freq, pixels * 2);
  
  if (thisStep < pixels) {  
    unsigned long reverse = pixels - thisStep;
    
    for(int i = 0; i < pixels; i++) {
      if (i >= reverse)
        strip.setPixelColor(i, Wheel(i % 256));
      else  
        strip.setPixelColor(i, 0UL);
    }
    strip.show();
  }
  else if (thisStep >= pixels) {
    int percent = 100 - ((thisStep - pixels) * 100 / pixels);
    for(int i = 0; i < pixels; i++) {
      uint8_t color0 = Wheel(i % 256);
      uint8_t r = (uint8_t)((color0 & 0x00FF0000UL) >> 16),
              g = (uint8_t)((color0 & 0x0000FF00UL) >>  8),
              b = (uint8_t)(color0 & 0x000000FFUL);
      strip.setPixelColor(i, ADJUSTED_COLOR(r * percent / 100, g * percent / 100, b * percent / 100));
    }
    strip.show();
  }
  
  return thisStep;
}

unsigned long pattern_Blink(unsigned long now, unsigned long freq, unsigned long prevStep, uint32_t color0, uint32_t color1) {
  unsigned long thisStep = CALCULATED_STEP(now, freq, 2);

  uint16_t i;

  for(i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, (thisStep == 0) ? color0 : color1);
  }
  strip.show();
  
  return thisStep;
}

unsigned long pattern_Glow(unsigned long now, unsigned long freq, unsigned long prevStep, uint32_t color0, uint32_t color1) {
  unsigned long thisStep = CALCULATED_STEP(now, freq, 200);

  uint8_t r = (uint8_t)(color0 >> 16),
          g = (uint8_t)(color0 >>  8),
          b = (uint8_t)color0;
  
  uint32_t real_color = 0;
  if (thisStep > 100)
    real_color = ADJUSTED_COLOR(r * (200 - thisStep) / 100, g * (200 - thisStep) / 100, b * (200 - thisStep) / 100);  
  else
    real_color = ADJUSTED_COLOR(r * thisStep / 100, g * thisStep / 100, b * thisStep / 100);  
  
  uint16_t i;

  for(i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, real_color);
  }
  strip.show();
  
  return thisStep;
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorStrips2(uint32_t c, uint32_t c2, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (i % 2 == 0) {
        strip.setPixelColor(i, c);
    }
    else {
        strip.setPixelColor(i, c2);
    }
  }
  strip.show();
  delay(wait);
}

// Fill the dots one after the other with a color
void colorStrips3(uint32_t c, uint32_t c2, uint32_t c3, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (i % 3 == 0) {
        strip.setPixelColor(i, c);
    }
    else if (i % 3 == 1) {
        strip.setPixelColor(i, c2);
    }
    else {
        strip.setPixelColor(i, c3);
    }
  }
  strip.show();
  delay(wait);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

