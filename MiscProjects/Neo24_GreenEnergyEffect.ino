/*
################################################################################
# File Name:             Neo24_FireEffect.ino                                             
# Board:                 Arduino Nano        
# Programming Language:	 Wiring / C /Processing / Arduino IDE          
#						
# Objective:             Code made for Neo Pixel Ring 24, XXXXXXXXXXX Project
#										  
# Operation:	           Fire but I changed the color to be green lol
#			
# Author:                Calder - CalderCatLabs 
# Date:                  November 19th, 2025					
################################################################################
Note: When uploading to Arduino Nano Every, the following error may be encountered: 
avrdude: jtagmkII_initialize(): Cannot locate "flash" and "boot" memories in description
This  is often a spurious warning (not an error) which can be safely ignored.
 */


#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS  24

Adafruit_NeoPixel emberStrip(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

int breathStep = 0;          // breathing cycle step
bool breathDirection = true; // true = brightening, false = dimming

void setup() {
  emberStrip.begin();
  emberStrip.show();
  randomSeed(analogRead(0));
}

void loop() {
  smolderingEmbersWithBreath();
  delay(80); // flicker speed
}

//Smoldering embers with breathing, darker red/orange tones
void smolderingEmbersWithBreath() {
  // Breathing cycle: brightness rises and falls slowly
  int breathBase = map(breathStep, 0, 100, 20, 70); // lower brightness range

  for (int i = 0; i < NUMPIXELS; i++) {
    // Random flicker intensity layered on breathing base
    int flicker = random(-8, 8);
    int emberBrightness = constrain(breathBase + flicker, 10, 80);

    // Red/orange palette: strong red, small green for orange, no blue, minimal white
    int r = emberBrightness;             // red dominant
    int g = emberBrightness / 5;         // subtle green for orange tint
    int b = 0;                           // no blue
    int w = emberBrightness / 12;        // very faint white glow

    emberStrip.setPixelColor(i, emberStrip.Color(g, r, b, w));
  }

  emberStrip.show();

  // Update breathing cycle
  if (breathDirection) {
    breathStep++;
    if (breathStep >= 100) breathDirection = false;
  } else {
    breathStep--;
    if (breathStep <= 0) breathDirection = true;
  }
}
