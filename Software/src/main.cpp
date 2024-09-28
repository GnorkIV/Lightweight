#include <Arduino.h>
#include <stdlib.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "RTClib.h"
#include "Rotary_Encoder.h"
#include "Buttons.h"
#include "server.h"
#include "RTC.h"
#include "LED.h"

#define NumberOfStrips 4

enum directionType {
    SUNSET,
    SUNRISE
};
directionType direction = SUNRISE;

static menuState_s menuState;
static ledStrip_s selecteStrip;

ledStrip_s Strip[NumberOfStrips];


// Struct in LED.h
// struct ledStrip_s{
//    boolean status;     // 0 - off, 1 - on
//    int state;          // 0 - light, 1 - color
//    int brightness;     // 0-255
//    RgbwColor color;    // RgbwColor(0, 0, 0, 0) BGRW
//};

// NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
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
    
    setupPushbuttons();
    setupRotaryEncoder();
    setupRTC();
    // setupServer();
    // setupLED();

    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN), RotaryEncoderINTERRUPT_handler, RISING);

    Serial.println("Setup LED Strips");

    selecteStrip = {0, 0, 128, RgbwColor(63/2, 255/2, 0, 255/2)};

    // for (int i=0; i<NumberOfStrips; i++){
    //     Strip[i] = {0, 0, 128, RgbwColor(63/2, 255/2, 0, 255/2)};
    // }

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
    

    handleButtonEvent(menuState, selecteStrip);
    // selecteStrip = Strip[menuState.selectedBoard-1];
    // selecteStrip = Strip[0];
    handleRotaryEncoderButtonEvent(selecteStrip);

    //selecteStrip.status = 1;
    // Serial.println(selecteStrip.state);

/*
    if (menuState.justSelected){
        menuState.justSelected = false;
        selecteStrip.status = !(selecteStrip.status);
        // save selected strip back
        Strip[menuState.selectedBoard-1] = selecteStrip;
        // Serial.println(menuState.selectedBoard);
        if (Strip[0].status && (menuState.selectedBoard == 1)){
            TurnOn(Strip[0]);
            // Serial.println("Turn On");
        }else{
            TurnOff();
            // Serial.println("Turn Off");
        }
    }
*/

    // Serial.print(menuState.selectedBoard);
    Serial.println( selecteStrip.status );
    Serial.println( selecteStrip.brightness );
    // Serial.println( getDateTime().second() );

    // // Update Local Strip
    UpdateLED(selecteStrip);

    //delay(5000);
}
