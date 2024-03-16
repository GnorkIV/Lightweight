#include <Arduino.h>
#include "Buttons.h"

menuState_s menuState = {0};
int pushButtonPinArr[] = {PUSHBUTTON_1_PIN, PUSHBUTTON_2_PIN, PUSHBUTTON_3_PIN, PUSHBUTTON_4_PIN};

void setupPushbuttons(){
    for(int i = 0; i < 4; i++){
        pinMode(pushButtonPinArr[i], INPUT);
    }
}

void handleButtonEvent(menuState_s &menuState){
    static bool buttonStateLastStep[] = {1,1,1,1}; //0 means pressed!

    // Debug outputs
    // Serial.print(digitalRead(PUSHBUTTON_1_PIN));
    // Serial.print(digitalRead(PUSHBUTTON_2_PIN));
    // Serial.print(digitalRead(PUSHBUTTON_3_PIN));
    // Serial.println(digitalRead(PUSHBUTTON_4_PIN));

    //check GPIOs
    for(int i = 0; i < 4; i++){
        if( (digitalRead(pushButtonPinArr[i]) == 0) && !(buttonStateLastStep[i] == 0) ){
            buttonStateLastStep[i] = digitalRead(pushButtonPinArr[i]);
            (menuState).selectedBoard = i +1;
            Serial.print("button ");
            Serial.print((menuState).selectedBoard);
            Serial.println(" pressed");
            break;  //ignore potential additional clicks
        }
        buttonStateLastStep[i] = digitalRead(pushButtonPinArr[i]);
    }
}