#include <Arduino.h>
// -------------------------------------------Rotary Encoder

#define ROTARY_ENCODER_A_PIN 12
#define ROTARY_ENCODER_B_PIN 13
#define ROTARY_ENCODER_BUTTON_PIN 14

#define DIRECTION_CW 0   // clockwise direction
#define DIRECTION_CCW 1  // counter-clockwise direction

struct RotaryState_s{
    volatile int counter;
    volatile int rotary_direction;
    unsigned long last_time; 
    int prev_counter;
};

void setupRotaryEncoder();

void IRAM_ATTR RotaryEncoderINTERRUPT_handler();

void handleRotaryEncoderButtonEvent();
