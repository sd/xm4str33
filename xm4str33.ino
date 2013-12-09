/*
 * XM4STR33
 * By Sebastian Delmont <sd@notso.net>
 *
 * Based in part on AdaFruit's NeoPixel example files.
 *
 */
 
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_DATA_PIN 6

#define NEOPIXEL_PIXEL_COUNT 300

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_PIXEL_COUNT, NEOPIXEL_DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  for (int i = 0; i < 40; i++) {
    colorStrips3(strip.Color(180, 180, 180), strip.Color(0, 0, 0), strip.Color(0, 0, 0), 500);
    colorStrips3(strip.Color(0, 0, 0), strip.Color(180, 180, 180), strip.Color(0, 0, 0), 500);
    colorStrips3(strip.Color(0, 0, 0), strip.Color(0, 0, 0), strip.Color(180, 180, 180), 500);
  }

  rainbowCycle(1);

  for (int i = 0; i < 40; i++) {
    colorStrips3(strip.Color(0, 200, 0), strip.Color(0, 200, 0), strip.Color(255, 0, 0), 500);
    colorStrips3(strip.Color(255, 0, 0), strip.Color(0, 200, 0), strip.Color(0, 200, 0), 500);
    colorStrips3(strip.Color(0, 200, 0), strip.Color(255, 0, 0), strip.Color(0, 200, 0), 500);
  }

  rainbowCycle(1);
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

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
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

