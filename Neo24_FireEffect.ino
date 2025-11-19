/*
################################################################################
# File Name:             Neo24_FireEffect.ino                                             
# Board:                 Arduino Nano        
# Programming Language:	 Wiring / C /Processing / Arduino IDE          
#						
# Objective:             Code made for Neo Pixel Ring 24, XXXXXXXXXXX Project
#										  
# Operation:	           Display fire/burning embers effect
#			
# Author:                Calder - CalderCatLabs 
# Date:                  November 18th, 2025					
################################################################################
Note: When uploading to Arduino Nano Every, the following error may be encountered: 
avrdude: jtagmkII_initialize(): Cannot locate "flash" and "boot" memories in description
This  is often a spurious warning (not an error) which can be safely ignored.
 */


#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS  24

Adafruit_NeoPixel fireStrip(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

// Ember simulation parameters
byte emberBrightness[NUMPIXELS];
int minGlow = 10;
int maxGlow = 80;
int flickerChance = 20;
int flickerBoost = 100;
int speedDelay = 50;

void setup() {
  fireStrip.begin();
  fireStrip.show();
  randomSeed(analogRead(0));

  runDiagnosticAnimation(); //Run startup animation

  // Initialize ember brightness
  for (int i = 0; i < NUMPIXELS; i++) {
    emberBrightness[i] = random(minGlow, maxGlow);
  }
}

void loop() {
  emberEffect();
  fireStrip.show();
  delay(speedDelay);
}

//Diagnostic animation: cycle red, orange, yellow for 10 seconds
void runDiagnosticAnimation() {
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) {
    for (int i = 0; i < NUMPIXELS; i++) {
      byte hue = (i * 10 + (millis() / 10)) % 40; // Hue from red to yellow
      fireStrip.setPixelColor(i, hsvToRgb(hue, 255, 255));
    }
    fireStrip.show();
    delay(50);
  }
  fireStrip.clear();
  fireStrip.show();
}

//Ember flicker effect
void emberEffect() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (random(100) < flickerChance) {
      emberBrightness[i] = constrain(emberBrightness[i] + random(-flickerBoost, flickerBoost), minGlow, 255);
    } else {
      int target = random(minGlow, maxGlow);
      if (emberBrightness[i] < target) emberBrightness[i]++;
      else if (emberBrightness[i] > target) emberBrightness[i]--;
    }

    fireStrip.setPixelColor(i, emberColor(emberBrightness[i]));
  }
}

//Map brightness to red-orange-yellow gradient
uint32_t emberColor(byte brightness) {
  byte hue = map(brightness, minGlow, 255, 0, 40); // Hue 0–40 = red to yellow
  return hsvToRgb(hue, 255, brightness);
}

//Convert HSV to RGB
uint32_t hsvToRgb(byte h, byte s, byte v) {
  float r, g, b;
  float hf = h / 60.0;
  int i = floor(hf);
  float f = hf - i;
  float p = v * (1.0 - s / 255.0);
  float q = v * (1.0 - f * s / 255.0);
  float t = v * (1.0 - (1.0 - f) * s / 255.0);

  switch (i % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
  }

  return fireStrip.Color((int)r, (int)g, (int)b);
}


