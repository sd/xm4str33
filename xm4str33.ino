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

// not sure why, but my 'millis' calls seem to be half as fast as expected, hence the timing * 2
#define CALCULATED_STEP(now,timing,steps) (( ((now) * 100) / ( ((timing) * 100) / (steps) ) ) % (steps) )

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
#define COLOR_YELLOW  strip.Color(255, 180, 0)
#define COLOR_CYAN  strip.Color(0, 255, 255)
#define COLOR_MAGENTA  strip.Color(255, 0, 255)
#define COLOR_ORANGE strip.Color(255, 180, 0)



unsigned long (*PatternFuncs[])(unsigned long now, unsigned long timing, unsigned long prevStep) = {
  &pattern_Random, &pattern_Stripes, &pattern_Run, &pattern_Flag // &pattern_RainbowCycle, &pattern_RainbowFart
};

// uint32_t colors[] = {COLOR_RED, COLOR_WHITE, COLOR_BLUE};
uint32_t colors[] = {COLOR_YELLOW, COLOR_BLUE, COLOR_RED};

void setup() {
 #ifdef DEBUG
 Serial.begin(115200);
 Serial.println("\r\nStart");
 #endif

  // initialize serial communication:
  // Bridge.begin();
  // Console.begin(); 

  randomSeed(analogRead(0));
  
//  while (!Console){
//    ; // wait for Console port to connect.
//  }
  // Console.println("You're connected to the Console!!!!");
  // initialize the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  #ifdef DEBUG
//  Console.println("- Setup done");
  #endif
}

bool patternIsFirstPass = false;

void loop() {
  
  unsigned long now = 0;
  int patternIndex = 0;
  unsigned long patternTiming = TWO_SECONDS;
  unsigned long cycleTiming = 6 * ONE_SECOND;
  unsigned long patternStep = 0;

  unsigned long lastPatternChange = 0;
  
  int incomingByte;
  
  int tick = 0;
  
  while(true) {
    #ifdef DEBUG
    Serial.print("Loop   - ");
    #endif

    now = millis();
    
    // if (tick == 0) {
    //   if ((now % 1000) < 50) {
    //     tick = 1;
    //     digitalWrite(LED_PIN, HIGH);
    //   }
    // }
    // else if (tick == 1) {
    //   if ((now % 1000) >= 50) {
    //     tick = 0;
    //     digitalWrite(LED_PIN, LOW);
    //   }
    // }
    

    if (now - lastPatternChange > cycleTiming) {
      patternIndex = (patternIndex + 1) % (sizeof(PatternFuncs)/2);
      lastPatternChange = now; 
      patternIsFirstPass = true;
    }

    // patternStep = pattern_Flag(now, patternTiming, patternStep);

    // patternStep = pattern_Glow(now, patternTiming, patternStep);
    
    patternStep = (*PatternFuncs[patternIndex])(now, patternTiming, patternStep);

    patternIsFirstPass = false;
  
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

long randomBaseSeed = 0;
unsigned long pattern_Random(unsigned long now, unsigned long timing, unsigned long prevStep) {
  unsigned long thisStep = CALCULATED_STEP(now, timing, 15);

  uint16_t i, y;

  // if (patternIsFirstPass) {
  //   randomBaseSeed = analogRead(0);
  // }

  randomSeed(randomBaseSeed + thisStep + 1);

  for(i = 0; i < NEOPIXEL_PIXEL_COUNT; i++) {
    int x = random(0, 100);

    if ((x >= 0) && (x < 10))
      strip.setPixelColor(i, colors[0]);
    else if ((x >= 10) && (x < 20))
      strip.setPixelColor(i, colors[1]);
    else if ((x >= 20) && (x < 30))
      strip.setPixelColor(i, colors[2]);
    // else if ((x >= 99) && (x < 100))
    //   strip.setPixelColor(i, COLOR_WHITE);
    else
      strip.setPixelColor(i, COLOR_BLACK);
  }
  strip.show();

  return thisStep;
}

#define RUN_SEGMENT_LENGTH 48
#define RUN_SPACING_LENGTH 64
unsigned long pattern_Run(unsigned long now, unsigned long timing, unsigned long prevStep) {
  unsigned long thisStep = CALCULATED_STEP(now, timing, NEOPIXEL_PIXEL_COUNT + (RUN_SPACING_LENGTH * 3));

  uint16_t i, y;

  for(i = 0; i < NEOPIXEL_PIXEL_COUNT; i++) {
    y = thisStep - i;

    if (y > 0 && y < RUN_SEGMENT_LENGTH)
      strip.setPixelColor(i, colors[0]);
    else if (y > RUN_SPACING_LENGTH && y < (RUN_SPACING_LENGTH + RUN_SEGMENT_LENGTH))
      strip.setPixelColor(i, colors[1]);
    else if (y > RUN_SPACING_LENGTH * 2 && y < (RUN_SPACING_LENGTH * 2 + RUN_SEGMENT_LENGTH))
      strip.setPixelColor(i, colors[2]);

    else
      strip.setPixelColor(i, 0UL);

  }
  strip.show();
  
  return thisStep;
}

#define STRIPES_SIZE 20
unsigned long pattern_Stripes(unsigned long now, unsigned long timing, unsigned long prevStep) {
  unsigned long thisStep = CALCULATED_STEP(now, timing, 60);

  uint16_t i, y, color;

  for(i = 0; i < NEOPIXEL_PIXEL_COUNT; i++) {
    y = (i + thisStep) % (STRIPES_SIZE * 3);
    y = y / STRIPES_SIZE; // which color within the pattern
    color = sizeof(colors)/4 - y - 1;
    
    strip.setPixelColor(i, colors[color]);
  }
  strip.show();
  
  return thisStep;
}

#define GLOW_STEPS_PER_COLOR 40

unsigned long pattern_Glow(unsigned long now, unsigned long timing, unsigned long prevStep) {
  unsigned long totalColors = (sizeof(colors)/4);

  unsigned long fullStep = CALCULATED_STEP(now, timing, GLOW_STEPS_PER_COLOR * totalColors);
  unsigned long thisStep = fullStep % GLOW_STEPS_PER_COLOR;

  uint8_t r = (uint8_t)(colors[(fullStep / GLOW_STEPS_PER_COLOR)] >> 16),
          g = (uint8_t)(colors[(fullStep / GLOW_STEPS_PER_COLOR)] >>  8),
          b = (uint8_t)colors[(fullStep / GLOW_STEPS_PER_COLOR)];
  
  uint32_t real_color = 0;
  if (thisStep >= 100)
    real_color = Gamma(r * (GLOW_STEPS_PER_COLOR - thisStep) / GLOW_STEPS_PER_COLOR, g * (GLOW_STEPS_PER_COLOR - thisStep) / GLOW_STEPS_PER_COLOR, b * (GLOW_STEPS_PER_COLOR - thisStep) / GLOW_STEPS_PER_COLOR);  
  else
    real_color = Gamma(r * thisStep / GLOW_STEPS_PER_COLOR, g * thisStep / GLOW_STEPS_PER_COLOR, b * thisStep / GLOW_STEPS_PER_COLOR);  
  
  uint16_t i;

  for(i = 0; i < NEOPIXEL_PIXEL_COUNT; i++) {
    strip.setPixelColor(i, real_color);
  }
  strip.show();
  
  return thisStep;
}

#define FLAG_BOTTOM 140
#define FLAG_MIDDLE 250
#define FLAG_TOP 300
unsigned long pattern_Flag(unsigned long now, unsigned long timing, unsigned long prevStep) {
  unsigned long thisStep = CALCULATED_STEP(now, timing, 15);

  uint16_t i, y, x;

  randomSeed(randomBaseSeed + thisStep + 1);

  for(i = 0; i < NEOPIXEL_PIXEL_COUNT; i++) {
    x = random(0, 100);
    if (x <= 5) {
      strip.setPixelColor(i, COLOR_WHITE);
    }
    else {
      if (i <= FLAG_BOTTOM)
        strip.setPixelColor(i, colors[2]);
      else if (i <= FLAG_MIDDLE)
        strip.setPixelColor(i, colors[1]);
      else if (i <= FLAG_TOP)
        strip.setPixelColor(i, colors[0]);
      else
        strip.setPixelColor(i, 0UL);
    }
  }
  strip.show();
  
  return thisStep;
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i < NEOPIXEL_PIXEL_COUNT; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorStrips2(uint32_t c, uint32_t c2, uint8_t wait) {
  for(uint16_t i=0; i < NEOPIXEL_PIXEL_COUNT; i++) {
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
  for(uint16_t i=0; i < NEOPIXEL_PIXEL_COUNT; i++) {
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
    for(i=0; i < NEOPIXEL_PIXEL_COUNT; i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}




