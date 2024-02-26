#include <Arduino.h>

// -------------------------------------------Menu System
typedef struct menuState{
    int selectedBoard = 0; // 0 - none, 1 - Master, 2-4 Rest
} menuState;

// -------------------------------------------Push Button
#define PUSHBUTTON_1_PIN 0
#define PUSHBUTTON_2_PIN 2
// #define PUSHBUTTON_3_PIN 16
// #define PUSHBUTTON_4_PIN 15 //see issue#2

int pushButtonPinArr[] = {PUSHBUTTON_1_PIN, PUSHBUTTON_2_PIN};

void setupPushbuttons(){
    for(int i = 0; i < 2; i++){
        pinMode(pushButtonPinArr[i], INPUT);
    }
}

void handleButtonEvent(menuState &menuState){
    static bool buttonStateLastStep[] = {1,1}; //0 means pressed!

    // Debug outputs
    // Serial.print(digitalRead(PUSHBUTTON_1_PIN));
    // Serial.println(digitalRead(PUSHBUTTON_2_PIN));
    // Serial.print(digitalRead(PUSHBUTTON_3_PIN));
    // Serial.println(digitalRead(PUSHBUTTON_4_PIN));

    //check GPIOs
    for(int i = 0; i < 2; i++){
        if( (digitalRead(pushButtonPinArr[i]) == 0) && !(buttonStateLastStep[i] == 0) ){
            buttonStateLastStep[i] = digitalRead(pushButtonPinArr[i]);
            (menuState).selectedBoard = i +1;
            Serial.print("button ");
            Serial.print((menuState).selectedBoard);
            Serial.println("pressed");
            break;  //ignore potential additional clicks
        }
        buttonStateLastStep[i] = digitalRead(pushButtonPinArr[i]);
    }
}