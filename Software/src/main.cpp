#include <Arduino.h>
#include <stdlib.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "RTClib.h"
#include "AiEsp32RotaryEncoder.h"

// -------------------------------------------Buttons
//const int buttonPin = 12;  // the number of the pushbutton pin
const int ledPin = 3;    // the number of the LED pin

// variables will change:
//int buttonState = 0;  // variable for reading the pushbutton status

// -------------------------------------------Rotary Encoder
/*#if defined(ESP8266)
#define ROTARY_ENCODER_A_PIN 6
#define ROTARY_ENCODER_B_PIN 7
#define ROTARY_ENCODER_BUTTON_PIN 11
#else
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#endif
#define ROTARY_ENCODER_STEPS 4


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

// -------------------------------------------LED Stuff
const uint16_t PixelCount = 20; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

boolean direction = true;

#define colorSaturation 128

//NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
// for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods 
//NeoPixelBus<NeoRgbwFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);

NeoPixelBus<NeoRgbwFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
NeoPixelAnimator animations(PixelCount, NEO_CENTISECONDS);
// NEO_MILLISECONDS        1    // ~65 seconds max duration, ms updates
// NEO_CENTISECONDS       10    // ~10.9 minutes max duration, centisecond updates
// NEO_DECISECONDS       100    // ~1.8 hours max duration, decisecond updates
// NEO_SECONDS          1000    // ~18.2 hours max duration, second updates
// NEO_DECASECONDS     10000    // ~7.5 days, 10 second updates

RgbwColor white(0,0,0, colorSaturation);
RgbwColor black(0, 0, 0, 0);
RgbwColor red(0, colorSaturation, 0, 0);

void dimcolor();

void SetRandomSeed()
{
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(0);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    Serial.println(seed);
    randomSeed(seed);
}

void setup() {

    Serial.begin(74880);

    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();

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

   for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        RgbwColor color = RgbwColor(0, 0, 0, 0);
        strip.SetPixelColor(pixel, color);
    }
    
    /*
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 1000, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
    rotaryEncoder.setAcceleration(250);
    */

    // initialize the LED pin as an output:
    //pinMode(ledPin, OUTPUT);
    // initialize the pushbutton pin as an input:
    //pinMode(buttonPin, INPUT);

    Serial.println();
    Serial.println("Running...");
}

void DarkSetupAnimationSet()
{
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        //uint16_t time = random(100, 400);
        //uint16_t time = pixel*50;
        //uint16_t time = 40 + (5*exp(pow(-((pixel*(3.1/PixelCount))),2)));
        uint16_t time = sin(pixel)+1;
        
        RgbwColor originalColor = strip.GetPixelColor(pixel);
        RgbwColor targetColor = RgbwColor(0, 0, 0, 0);
        AnimEaseFunction easing;

        switch (random(3))
        {
        case 0:
            easing = NeoEase::CubicIn;
            break;
        case 1:
            easing = NeoEase::CubicOut;
            break;
        case 2:
            easing = NeoEase::QuadraticInOut;
            break;
        }

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

void SetupAnimationSet()
{
    // setup some animations
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {

        // pick a random duration of the animation for this pixel
        // since values are centiseconds, the range is 1 - 4 seconds
        //uint16_t time = random(100, 400);
        uint16_t time = (3000 - (2500*exp(-pow((((double_t)(0.25*pixel))-((double_t)PixelCount/8)),2))))*2;
        //uint16_t time = 1000+(500*sin(pixel*0.4));
        
        // each animation starts with the color that was present
        RgbwColor originalColor = strip.GetPixelColor(pixel);
        // and ends with a random color
        RgbwColor targetColor = RgbwColor(63, 255, 0, 255);
        // with the random ease function
        AnimEaseFunction easing;

        switch (random(3))
        {
        case 0:
            easing = NeoEase::CubicIn;
            break;
        case 1:
            easing = NeoEase::CubicOut;
            break;
        case 2:
            easing = NeoEase::QuadraticInOut;
            break;
        }

        easing = NeoEase::Gamma;


        // we must supply a function that will define the animation, in this example
        // we are using "lambda expression" to define the function inline, which gives
        // us an easy way to "capture" the originalColor and targetColor for the call back.
        //
        // this function will get called back when ever the animation needs to change
        // the state of the pixel, it will provide a animation progress value
        // from 0.0 (start of animation) to 1.0 (end of animation)
        //
        // we use this progress value to define how we want to animate in this case
        // we call RgbColor::LinearBlend which will return a color blended between
        // the values given, by the amount passed, hich is also a float value from 0.0-1.0.
        // then we set the color.
        //
        // There is no need for the MyAnimationState struct as the compiler takes care
        // of those details for us
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
        if (direction) {
            SetupAnimationSet(); 
            direction = false;
            //digitalWrite(ledPin, LOW);
            Serial.println("Setup Next Set Bright...");
            }
        else {
            DarkSetupAnimationSet();  
            direction = true;
            //digitalWrite(ledPin, HIGH);
            Serial.println("Setup Next Set Dark...");
            }
    }
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
