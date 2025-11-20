#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS  24

Adafruit_NeoPixel lightningStrip(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

// Forward declarations
void lightningEffect();
void fadeOut(int fadeTimeMs);
void superStrike();
void backgroundFlicker();
void rollingThunder(bool echo = false);

// Utility: pick a random lightning color variation
uint32_t randomLightningColor(int brightness) {
  int variation = random(0, 3); // 0=blue, 1=violet, 2=teal

  switch (variation) {
    case 0: // Pure blue
      return lightningStrip.Color(0, 0, brightness, brightness / 3);
    case 1: // Violet (red+blue mix)
      return lightningStrip.Color(brightness / 2, 0, brightness, brightness / 4);
    case 2: // Teal (green+blue mix)
      return lightningStrip.Color(0, brightness / 2, brightness, brightness / 4);
    default:
      return lightningStrip.Color(0, 0, brightness, brightness / 3);
  }
}

void setup() {
  lightningStrip.begin();
  lightningStrip.show();
  randomSeed(analogRead(0));
}

void loop() {
  backgroundFlicker();

  if (random(100) < 20) { // frequent strikes
    lightningEffect();
  }
}

//Lightning effect simulation
void lightningEffect() {
  if (random(100) < 2) { // 2% chance super strike
    superStrike();
    return;
  }

  int strikes = random(2, 5); // overlapping arcs

  for (int s = 0; s < strikes; s++) {
    int brightness = random(60, 100);
    int duration   = random(30, 120);
    int startPixel = random(0, NUMPIXELS);
    int length     = random(3, 8);

    for (int j = 0; j < length; j++) {
      int pixel = (startPixel + j) % NUMPIXELS;

      float center = length / 2.0;
      float distance = abs(j - center);
      float falloff = 1.0 - (distance / center);
      if (falloff < 0) falloff = 0;

      int b = (int)(brightness * falloff);

      lightningStrip.setPixelColor(pixel, randomLightningColor(b));
    }
  }

  lightningStrip.show();
  delay(random(40, 120));

  fadeOut(random(2000, 4000)); // fade 2–4s
}

//Fade out
void fadeOut(int fadeTimeMs) {
  const int steps = 50;
  for (int s = steps; s >= 0; s--) {
    float factor = (float)s / steps;

    for (int i = 0; i < NUMPIXELS; i++) {
      uint32_t c = lightningStrip.getPixelColor(i);

      byte w = (c >> 24) & 0xFF;
      byte r = (c >> 16) & 0xFF;
      byte g = (c >>  8) & 0xFF;
      byte b = (c      ) & 0xFF;

      lightningStrip.setPixelColor(
        i,
        (int)(r * factor),
        (int)(g * factor),
        (int)(b * factor),
        (int)(w * factor)
      );
    }

    lightningStrip.show();
    delay(fadeTimeMs / steps);
  }

  lightningStrip.clear();
  lightningStrip.show();
}

//Super strike
void superStrike() {
  int brightness = 180;
  uint32_t color = randomLightningColor(brightness);

  for (int i = 0; i < NUMPIXELS; i++) {
    lightningStrip.setPixelColor(i, color);
  }

  lightningStrip.show();
  delay(random(100, 200));

  fadeOut(random(3000, 5000));

  //rollingThunder();

  if (random(100) < 60) { // echo chance
    delay(random(500, 1500));
    //rollingThunder(true);
  }

  delay(random(800, 1500));
}

//Background flicker
void backgroundFlicker() {
  int flicker = random(5, 20);
  for (int i = 0; i < NUMPIXELS; i++) {
    lightningStrip.setPixelColor(i, lightningStrip.Color(0, 0, flicker, flicker / 4));
  }
  lightningStrip.show();
  delay(random(50, 150));
}

//Rolling thunder wave
/*void rollingThunder(bool echo) {
  int waveBrightness = echo ? 20 : 40;
  int delayPerStep   = 80;

  for (int offset = 0; offset < NUMPIXELS; offset++) {
    lightningStrip.clear();

    for (int i = 0; i < NUMPIXELS; i++) {
      int distance = abs(i - offset);
      if (distance > NUMPIXELS / 2) distance = NUMPIXELS - distance;

      float falloff = 1.0 - (distance / (NUMPIXELS / 4.0));
      if (falloff < 0) falloff = 0;

      int b = (int)(waveBrightness * falloff);

      lightningStrip.setPixelColor(i, lightningStrip.Color(0, 0, b, b / 4));
    }

    lightningStrip.show();
    delay(delayPerStep);
  }

  lightningStrip.clear();
  lightningStrip.show();
}*/
