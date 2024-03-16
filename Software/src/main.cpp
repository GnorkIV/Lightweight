#include <Arduino.h>
#include <stdlib.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "RTClib.h"
#include "Rotary_Encoder.h"
#include "Buttons.h"
#include "server.h"
#include "RTC.h"

enum directionType {
    SUNSET,
    SUNRISE
};
directionType direction = SUNRISE;


//NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
// for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods 

// Neopixel Time Info:
// NEO_MILLISECONDS        1    // ~65 seconds max duration, ms updates
// NEO_CENTISECONDS       10    // ~10.9 minutes max duration, centisecond updates
// NEO_DECISECONDS       100    // ~1.8 hours max duration, decisecond updates
// NEO_SECONDS          1000    // ~18.2 hours max duration, second updates
// NEO_DECASECONDS     10000    // ~7.5 days, 10 second updates

void setup() {

    Serial.begin(74880);

    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    // strip.Begin();
    // strip.Show();
    
    setupPushbuttons();
    setupRotaryEncoder();
    setupRTC();
    setupServer();

    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN), RotaryEncoderINTERRUPT_handler, RISING);

    Serial.println();
    Serial.println("Running...");
}

void loop() {


    // if (animations.IsAnimating())
    // {
    //     // the normal loop just needs these two to run the active animations
    //     animations.UpdateAnimations();
    //     strip.Show();
    // }
    // else
    // {
    //     Serial.println();
    //     Serial.println("Setup Next Set...");
    //     // example function that sets up some animations
    //     if (direction == SUNRISE) {
    //         TurnOnSetupAnimationSet(); 
    //         direction = SUNSET; //
    //         //digitalWrite(ledPin, LOW);
    //         Serial.println("Setup Next Set Bright...");
    //         }
    //     else if (direction == SUNSET) {
    //         TurnOffSetupAnimationSet();  
    //         direction = SUNRISE;
    //         //digitalWrite(ledPin, HIGH);
    //         Serial.println("Setup Next Set Dark...");
    //         }
    // }
    
    static menuState_s menuState;
    handleButtonEvent(menuState);
    handleRotaryEncoderButtonEvent();

    // Serial.println( getDateTime().second() );

    //delay(5000);
}
