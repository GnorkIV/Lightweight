#include <Arduino.h>
#include <ESP8266WiFi.h>
// -------------------------------------------Rotary Encoder

#define ROTARY_ENCODER_A_PIN 12
#define ROTARY_ENCODER_B_PIN 13
#define ROTARY_ENCODER_BUTTON_PIN 14

#define DIRECTION_CW 0   // clockwise direction
#define DIRECTION_CCW 1  // counter-clockwise direction

volatile int counter = 0;
volatile int rotary_direction = DIRECTION_CW;
volatile unsigned long last_time;  // for debouncing
int prev_counter;

void setupRotaryEncoder(){
    pinMode(ROTARY_ENCODER_A_PIN, INPUT);
    pinMode(ROTARY_ENCODER_B_PIN, INPUT);
    pinMode(ROTARY_ENCODER_BUTTON_PIN, INPUT);
}

void IRAM_ATTR INTERRUPT_handler() {
  if ((millis() - last_time) < 10)  // debounce time is 50ms
    return;

  if (digitalRead(ROTARY_ENCODER_B_PIN) == HIGH) {
    // The encoder is rotating in counter-clockwise direction => decrease the counter
    counter--;
    rotary_direction = DIRECTION_CCW;
  } else {
    // The encoder is rotating in clockwise direction => increase the counter
    counter++;
    rotary_direction = DIRECTION_CW;
  }

  last_time = millis();
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
