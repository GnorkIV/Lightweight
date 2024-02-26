#include <Arduino.h>
#include <stdlib.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "RTClib.h"
#include "Buttons.h"

// -------------------------------------------Buttons
//const int buttonPin = 12;  // the number of the pushbutton pin


const int debugLedPin = 2;    // the number of the LED pin

// variables will change:
//int buttonState = 0;  // variable for reading the pushbutton status

// -------------------------------------------Rotary Encoder

/*
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25



AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}
*/


// -------------------------------------------RTC
RTC_PCF8563 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
 
// -------------------------------------------Server Stuff
//const char* ssid = "FRITZ!Box 7530 MX";
//const char* password =  "97931520760499136656";
 
//AsyncWebServer server(80);

// -------------------------------------------Neopixel Stuff
const uint16_t PixelCount = 20; 
const uint8_t PixelPin = 2;

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

NeoPixelBus<NeoRgbwFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
NeoPixelAnimator animations(PixelCount, NEO_CENTISECONDS);

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

    /*
    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC is NOT initialized, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    rtc.start();
    */

    /*
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    }
    
    Serial.println(WiFi.localIP());

    server.on(
        "/post",
        HTTP_POST,
        [](AsyncWebServerRequest * request){},
        NULL,
        [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    
        String data_string = "";
        for (size_t i = 0; i < len; i++) {
            Serial.write(data[i]);
            data_string += (char)data[i];
        }
    
        Serial.println();

        Serial.println("REPEAT:"+ data_string);

        minutes_to_alarm = data_string.toInt(); // ascii to integer

        //Reset LED
        strip.Begin();
        strip.Show();
    
        request->send(200);
    });
    
    server.begin();
    */
    
    /*
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 1000, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
    rotaryEncoder.setAcceleration(250);
    */

    Serial.println();
    Serial.println("Running...");
}

//Animation to define turning off behavior
void TurnOffSetupAnimationSet()
{
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        //uint16_t time = random(100, 400);
        //uint16_t time = pixel*50;
        //uint16_t time = 40 + (5*exp(pow(-((pixel*(3.1/PixelCount))),2)));
        uint16_t time = 1;
        
        RgbwColor originalColor = strip.GetPixelColor(pixel);
        RgbwColor targetColor = RgbwColor(0, 0, 0, 0);
        AnimEaseFunction easing;

        easing = NeoEase::ExponentialOut;

        AnimUpdateCallback animUpdate = [=](const AnimationParam& param)
        {

            float progress = easing(param.progress);

            RgbwColor updatedColor = RgbwColor::LinearBlend(originalColor, targetColor, progress);
            strip.SetPixelColor(pixel, updatedColor);
        };

        animations.StartAnimation(pixel, time, animUpdate);
    }
}

//Animation to define turning on behavior
void TurnOnSetupAnimationSet()
{
    // setup some animations
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        uint16_t time = (3000 - (2500*exp(-pow((((double_t)(0.25*pixel))-((double_t)PixelCount/8)),2))))*2;
        
        // each animation starts with the color that was present
        RgbwColor originalColor = strip.GetPixelColor(pixel);
        // and ends with the targetColor
        RgbwColor targetColor = RgbwColor(63, 255, 0, 255); //order is actually grbw!
        // with the ease function
        AnimEaseFunction easing;

        easing = NeoEase::Gamma;

        AnimUpdateCallback animUpdate = [=](const AnimationParam& param)
        {
            // progress will start at 0.0 and end at 1.0
            // we convert to the curve we want
            float progress = easing(param.progress);

            // use the curve value to apply to the animation
            RgbwColor updatedColor = RgbwColor::LinearBlend(originalColor, targetColor, progress);
            strip.SetPixelColor(pixel, updatedColor);
        };

        // now use the animation properties we just calculated and start the animation
        // which will continue to run and call the update function until it completes
        animations.StartAnimation(pixel, time, animUpdate);
    }
}

void loop() {
    // put your main code here, to run repeatedly:

    //DateTime now = rtc.now();

/*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
*/
/*
    if (rotaryEncoder.encoderChanged())
    {
        Serial.println(rotaryEncoder.readEncoder());
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        Serial.println("button pressed");
    }
*/
    static menuState menuState;
    handleButtonEvent(menuState);
/*
    if (animations.IsAnimating())
    {
        // the normal loop just needs these two to run the active animations
        animations.UpdateAnimations();
        strip.Show();
    }
    else
    {
        Serial.println();
        Serial.println("Setup Next Set...");
        // example function that sets up some animations
        if (direction == SUNRISE) {
            TurnOnSetupAnimationSet(); 
            direction = SUNSET; //
            //digitalWrite(ledPin, LOW);
            Serial.println("Setup Next Set Bright...");
            }
        else if (direction == SUNSET) {
            TurnOffSetupAnimationSet();  
            direction = SUNRISE;
            //digitalWrite(ledPin, HIGH);
            Serial.println("Setup Next Set Dark...");
            }
    }
*/

/*
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
        // turn LED on:
        digitalWrite(ledPin, HIGH);
        Serial.println("Upsi!");
    } else {
        // turn LED off:
        digitalWrite(ledPin, LOW);
    }
    */
    //delay(5000);
}
