#include <Arduino.h>

// -------------------------------------------Menu System
struct menuState_s{
    int selectedBoard; // 0 - none, 1 - Master, 2-4 Rest
};

// -------------------------------------------Push Button
#define PUSHBUTTON_1_PIN 0
#define PUSHBUTTON_2_PIN 2
#define PUSHBUTTON_3_PIN 16
#define PUSHBUTTON_4_PIN 15 //see issue#2

void setupPushbuttons();

void handleButtonEvent(menuState_s &menuState);