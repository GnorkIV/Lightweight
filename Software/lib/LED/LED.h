#include <Arduino.h>
#include <stdlib.h>
#include <NeoPixelBus.h>

#ifndef _LEDH_
#define _LEDH_

// -------------------------------------------LED Function defines

struct ledStrip_s{
    boolean status;     // 0 - off, 1 - on
    int state;          // 0 - light, 1 - color
    int brightness;     // 0-255
    RgbwColor color;    // RgbwColor(0, 0, 0, 0) BGRW
};

// -------------------------------------------Neopixel Stuff
const uint16_t PixelCount = 10; 
const uint8_t PixelPin = 2;

void setupLED();
void TurnOnSetupAnimationSet();
void TurnOffSetupAnimationSet();
void TurnOn(ledStrip_s &Strip);
void TurnOff();
void setColor(ledStrip_s &Strip);

#endif