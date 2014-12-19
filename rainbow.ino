unsigned long  pattern_RainbowCycle(unsigned long now, unsigned long freq, unsigned long prevOffset) {
  freq = freq * 2;
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
      strip.setPixelColor(i, Gamma(r * percent / 100, g * percent / 100, b * percent / 100));
    }
    strip.show();
  }
  
  return thisStep;
}


