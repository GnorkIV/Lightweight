#include <Arduino.h>
#include "Rotary_Encoder.h"

RotaryState_s RotaryEncoder = {0,0,0,0};

void setupRotaryEncoder(){
    pinMode(ROTARY_ENCODER_A_PIN, INPUT);
    pinMode(ROTARY_ENCODER_B_PIN, INPUT);
    pinMode(ROTARY_ENCODER_BUTTON_PIN, INPUT);
}

void IRAM_ATTR RotaryEncoderINTERRUPT_handler() {
  if ((millis() - (RotaryEncoder).last_time) < 10)  // debounce time is 50ms
    return;

  if (digitalRead(ROTARY_ENCODER_B_PIN) == HIGH) {
    // The encoder is rotating in counter-clockwise direction => decrease the counter
    (RotaryEncoder).counter--;
    (RotaryEncoder).rotary_direction = DIRECTION_CW;
  } else {
    // The encoder is rotating in clockwise direction => increase the counter
    (RotaryEncoder).counter++;
    (RotaryEncoder).rotary_direction = DIRECTION_CCW;
  }

  (RotaryEncoder).last_time = millis();

    if (RotaryEncoder.prev_counter != RotaryEncoder.counter) {
        Serial.print("Rotary Encoder:: direction: ");
    if (RotaryEncoder.rotary_direction == DIRECTION_CW)
        Serial.print("CLOCKWISE");
    else
        Serial.print("ANTICLOCKWISE");

        Serial.print(" - count: ");
        Serial.println(RotaryEncoder.counter);

        RotaryEncoder.prev_counter = RotaryEncoder.counter;
    }
}

void handleRotaryEncoderButtonEvent(){
    static bool buttonStateLastStep = 1; //0 means pressed!

    // Serial.println(digitalRead(ROTARY_ENCODER_BUTTON_PIN));
    //check GPIOs

    if( (digitalRead(ROTARY_ENCODER_BUTTON_PIN) == 0) && !(buttonStateLastStep == 0) ){
        buttonStateLastStep = digitalRead(ROTARY_ENCODER_BUTTON_PIN);
        Serial.println("RotaryEncoder pressed");
    }
    buttonStateLastStep = digitalRead(ROTARY_ENCODER_BUTTON_PIN);
}