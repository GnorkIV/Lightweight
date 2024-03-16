#include <Arduino.h>
#include "RTClib.h"
#include "RTC.h"

// -------------------------------------------RTC
RTC_PCF8563 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setupRTC(){

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

}

DateTime getDateTime(){
    return rtc.now();
}