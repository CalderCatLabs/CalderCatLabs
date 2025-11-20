/*
################################################################################
# File Name:             Neo24_FireEffect.ino                                             
# Board:                 Arduino Nano        
# Programming Language:	 Wiring / C /Processing / Arduino IDE          
#						
# Objective:             Code made for Neo Pixel Ring 24, XXXXXXXXXXX Project
#										  
# Operation:	           Display smoldering embers effect because I didnt like how the fire one looked
#			
# Author:                Calder - CalderCatLabs 
# Date:                  November 19th, 2025					
################################################################################
Note: When uploading to Arduino Nano Every, the following error may be encountered: 
avrdude: jtagmkII_initialize(): Cannot locate "flash" and "boot" memories in description
This  is often a spurious warning (not an error) which can be safely ignored.
 */


#include <Adafruit_NeoPixel.h>

#define PIN        6       // Data pin connected to NeoPixel ring
#define NUMPIXELS  24      // Number of pixels in the ring

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

// Breathing cycle variables
int breathStep = 0;
bool breathDirection = true;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0)); // Seed randomness for flicker
}

void loop() {
  smolderingCoals();
  delay(80); // Controls flicker speed
}

//Smoldering coals effect
void smolderingCoals() {
  // Breathing cycle: brightness rises and falls slowly
  int breathBase = map(breathStep, 0, 100, 20, 70); // glow range

  for (int i = 0; i < NUMPIXELS; i++) {
    // Random flicker layered on breathing base
    int flicker = random(-8, 8);
    int emberBrightness = constrain(breathBase + flicker, 10, 80);

    // Color palette: deep red with faint orange warmth
    int r = emberBrightness;             // red dominant
    int g = emberBrightness / 6;         // subtle orange tint
    int b = 0;                           // no blue
    int w = 0;                           // no white (keeps colors warm)

    strip.setPixelColor(i, strip.Color(r, g, b, w));
  }

  strip.show();

  // Update breathing cycle
  if (breathDirection) {
    breathStep++;
    if (breathStep >= 100) breathDirection = false;
  } else {
    breathStep--;
    if (breathStep <= 0) breathDirection = true;
  }
}
