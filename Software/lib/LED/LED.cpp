#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "LED.h"

NeoPixelBus<NeoRgbwFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
NeoPixelAnimator animations(PixelCount, NEO_CENTISECONDS);

//Animation to define turning off behavior
void TurnOffSetupAnimationSet(){
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
void TurnOnSetupAnimationSet(){
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