/*
 * XM4STR33
 * By Sebastian Delmont <sd@notso.net>
 *
 * Based in part on AdaFruit's NeoPixel example files.
 *
 */
 
#include <Adafruit_NeoPixel.h>

#include <Console.h>

//#include "pattern.h"
//#include "pattern_run.h"

#define DEBUG 1

#define LED_PIN 13

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

#define COLOR_BLACK strip.Color(0, 0, 0)
#define COLOR_WHITE strip.Color(255, 255, 255)
#define COLOR_RED   strip.Color(255, 0, 0)
#define COLOR_GREEN strip.Color(0, 255, 0)
#define COLOR_BLUE  strip.Color(0, 0, 255)
#define COLOR_YELLOW  strip.Color(255, 255, 0)
#define COLOR_CYAN  strip.Color(0, 255, 255)
#define COLOR_MAGENTA  strip.Color(255, 0, 255)
#define COLOR_ORANGE strip.Color(255, 180, 0)


unsigned long (*PatternFuncs[])(unsigned long now, unsigned long freq, unsigned long prevStep) = {
  &pattern_Random, &pattern_Glow, &pattern_Blink, &pattern_Run, &pattern_RainbowCycle, &pattern_RainbowFart
};

uint32_t colors[] = {COLOR_BLACK, COLOR_ORANGE, COLOR_RED, COLOR_GREEN};

void setup() {
//  #ifdef DEBUG
//  #Serial.begin(115200);
//  #Serial.println("\r\nStart");
//  #endif

  // initialize serial communication:
  Bridge.begin();
  Console.begin(); 

  randomSeed(analogRead(0));
  
//  while (!Console){
//    ; // wait for Console port to connect.
//  }
  Console.println("You're connected to the Console!!!!");
  // initialize the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  #ifdef DEBUG
//  Console.println("- Setup done");
  #endif
}

void loop() {
  
  unsigned long now = 0;
  int patternIndex = 0;
  unsigned long patternFrequency = TWO_SECONDS;
  unsigned long patternStep = 0;

  unsigned long lastPatternChange = 0;
  
  int incomingByte;
  
  int tick = 0;
  
  while(true) {
    #ifdef DEBUG
    Console.print("Loop   - ");
    #endif

    now = millis();
    
    if (tick == 0) {
      if ((now % 1000) < 50) {
        tick = 1;
        digitalWrite(LED_PIN, HIGH);
      }
    }
    else if (tick == 1) {
      if ((now % 1000) >= 50) {
        tick = 0;
        digitalWrite(LED_PIN, LOW);
      }
    }
    

    if (now - lastPatternChange > 10 * ONE_SECOND) {
      patternIndex = (patternIndex + 1) % sizeof(PatternFuncs);
      lastPatternChange = now; 
    }

//    patternStep = pattern_Random(now, patternFrequency, patternStep);

    patternStep = (*PatternFuncs[patternIndex])(now, patternFrequency, patternStep);

//    if (patternIndex == 0) {
//      patternStep = pattern_Glow(now, patternFrequency, patternStep);
//    }
//    else if (patternIndex == 1) {
//      patternStep = pattern_Blink(now, patternFrequency, patternStep);
//    }  
//    else if (patternIndex == 2) {
//      patternStep = pattern_Run(now, patternFrequency, patternStep);
//    }  
//    else if (patternIndex == 3) {
//      patternStep = pattern_RainbowCycle(now, patternFrequency, patternStep);
//    }  
//    else if (patternIndex == 4) {
//      patternStep = pattern_RainbowFart(now, patternFrequency, patternStep);
//    }  
  
//    #ifdef DEBUG
//    unsigned long t = millis() - now;
//    Console.print("Time: ");
//    Console.print(now);
//    Console.print(" step: ");
//    Console.print(patternStep);
//    Console.print(" (");
//    Console.print(t);
//    Console.print("ms proc time) ");
//    Console.println("");
//    #endif
  }
}

unsigned long pattern_Random(unsigned long now, unsigned long freq, unsigned long prevStep) {
  unsigned long thisStep = CALCULATED_STEP(now, freq, strip.numPixels());

  uint16_t i, y;

  if (now % 4 == 0) {
    for(i = 0; i < strip.numPixels(); i++) {
      y = abs(i - thisStep);
  
      int x = random(0, 64);
      if (x == 0)
        strip.setPixelColor(i, COLOR_WHITE);
      else if (x == 1)
        strip.setPixelColor(i, COLOR_RED);
      else if (x == 4)
        strip.setPixelColor(i, COLOR_BLUE);
      else
        strip.setPixelColor(i, colors[0]);
    }
    strip.show();
  }
  return thisStep;
}

unsigned long pattern_Run(unsigned long now, unsigned long freq, unsigned long prevStep) {
  unsigned long thisStep = CALCULATED_STEP(now, freq, strip.numPixels());

  uint16_t i, y;

  for(i = 0; i < strip.numPixels(); i++) {
    y = abs(i - thisStep);
    if (y < 6)
      strip.setPixelColor(i, colors[0]);
    else if (y > 20 && y < 26)
      strip.setPixelColor(i, colors[1]);
    else
      strip.setPixelColor(i, 0UL);

  }
  strip.show();
  
  return thisStep;
}

unsigned long pattern_Blink(unsigned long now, unsigned long freq, unsigned long prevStep) {
  unsigned long thisStep = CALCULATED_STEP(now, freq, 2);

  uint16_t i;

  for(i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, (thisStep == 0) ? colors[1] : colors[2]);
  }
  strip.show();
  
  return thisStep;
}

unsigned long pattern_Glow(unsigned long now, unsigned long freq, unsigned long prevStep) {
  unsigned long thisStep = CALCULATED_STEP(now, freq, 200);

  uint8_t r = (uint8_t)(colors[1] >> 16),
          g = (uint8_t)(colors[1] >>  8),
          b = (uint8_t)colors[1];
  
  uint32_t real_color = 0;
  if (thisStep > 100)
    real_color = Gamma(r * (200 - thisStep) / 100, g * (200 - thisStep) / 100, b * (200 - thisStep) / 100);  
  else
    real_color = Gamma(r * thisStep / 100, g * thisStep / 100, b * thisStep / 100);  
  
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




