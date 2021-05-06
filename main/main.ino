#define BLYNK_TIMEOUT_MS 500 // must be BEFORE BlynkSimpleEsp8266.h
#define BLYNK_HEARTBEAT                                                                                                                \
    17 // must be BEFORE BlynkSimpleEsp8266.h. 17 was experimentally determined to
         // be effective in our situation.
#define BLYNK_PRINT Serial // used internally by Blynk libraries

#include "Additional.h"                 //Additional code, mainly Time class
#include <BlynkSimpleEsp8266.h> //for blynk control over wifi, must only be included in main.ino due to poor library design
#include <ESP8266WiFi.h>                //basic functionality
#include <Stepper.h>
#include <TimeLib.h>
#include <WidgetRTC.h> //for blynk real time clock

//  example times
/*  1:00am   =>  010
    4:30am   =>  043
    12:00pm  =>  120
    8:20pm   =>  202
    1550 hrs =>  155
    00:00    =>  000
    5:42am   =>  054
    5:49am   =>  054

*  Invalid Times:
    Anything where the first two digits aren't between 0 and 23 inclusive,
    or the last digit isn't between 0 and 5 inclusive.

    000 is an invalid time, as it is the default value of our array.
    Feed your cat at 23:50 or 00:10.

    for example:
    240
    239
    006

*/

// declaring constants and variables

// FILL IN YOUR OWN VALUES HERE
const char *ssid = "PLACEHOLDER";
const char *password = "PLACEHOLDER";
const char *auth = "PLACEHOLDER"; // auth key for Blynk

const byte feedingTimes[4] = {70, 120, 190, 0}; // default initialization
const bool fedTimes[4] = {0, 0, 0, 0};

const int stepsPerRevolution = 200;
const int stepperSpeed = 100;
const int STEPPERPIN2 = 2; // Labelled as D4 on silkscreen
const int STEPPERPIN1 = 0; // Labelled as D3 on silkscreen
const int STEPSBACKWARD = -150;
const int STEPSFORWARD = 300;

const bool debugMode = true;

int numOfTurns = 1; // number of turns the motor, and thus auger, will make
                    // equivalent to changing portion size
int stepCount = 0;

unsigned long currentMillis = 0;
unsigned long millisOnLastTimeCheck = 0;
unsigned long millisOnLastBlynkFeeding = 0;
byte currentTime = 0;
byte fallbackTime = 0;
byte idealCaseTime = 0;
bool firstRunOnly = true;

bool on = 0;
bool online = 0;

//########################################################
// BETWEEN THESE LINES USED TO BE THE CONTENTS OF ITS OWN HEADER FILE
// A LIBRARY WE USE IS IMPROPERLY WRITTEN AND THUS MAY ONLY BE INCLUDED IN THE
// MAIN INO FILE
//
// BLINKITYBLINK_H IS DEFINED BELOW

class BlinkityBlink : public WidgetRTC {
private:
    byte currentTimeAsByte = 0;
    signed long long millisAsOfMostRecentMidnight = 0;

    void resetMidnightMillis();
    void updateTime();

public:
    byte blynkTimeAsByte();

    explicit BlinkityBlink() : WidgetRTC() {} // to inherit constructor
    void nightlyReset();
    void updateBlynkFeedingTimes(); // sends data to blynkapp
};

//######################
// END BLINKITYBLINK_H
// SEE BOTTOM OF FILE FOR BLINKITYBLINK_CPP

// Instantiate objects:

Time myTime(feedingTimes);
BlynkTimer timer;
BlinkityBlink wink; // inherits from WidgetRTC
Stepper myStepper(stepsPerRevolution, STEPPERPIN1, STEPPERPIN2);

//*###########################
//*Asynchronous Blynk Functions:

// To be called when Blynk app sends "Feed Now"
BLYNK_WRITE(V0) {
    currentMillis = millis();
    if (param.asInt()) {
        if (currentMillis - millisOnLastBlynkFeeding >= 10000 ||
                debugMode) { // if it's been more than 10 seconds since last request
            spinMotor();
            millisOnLastBlynkFeeding = currentMillis;
            if (param.asInt() == 10) {
                Serial.println("Remote connection from Blynk App ordered pet fed:");
            } else {
                Serial.println("Remote connection from Google Assistant ordered pet fed:");
            }
            Serial.println("Running extra feeding cycle..");
        } else {
            Serial.println("Too little time has elapsed since last feeding.");
            Serial.println("Taking no action.");
        }
    }
}

// To be called when Blynk App sends feeding times
BLYNK_WRITE(V1) {
    myTime.setFeedingTimes(param.asStr());
    myTime.sortFeedingTimes();
    myTime.prepFeedingTimes(currentTime);
    myTime.printFeedingTimes();
}

// To be called when Blynk App sends a number of turns
BLYNK_WRITE(V2) {
    numOfTurns = param.asInt();
    Serial.print("Turns changed to ");
    Serial.println(numOfTurns);
}

//*################################
// End Asynchronous Blynk Functions

// Run once whenever the board is reset
void setup() {
    Serial.begin(9600);

    Serial.println(" "); // because of garbage data written to serial when board is reset
    Serial.print("Connecting to ");
    Serial.println(ssid);

    Blynk.begin(auth, ssid, password);
    CheckConnection(); // also initiates connection to wifi and blynkapp
    wink.begin();      // begin tracking time
    wink.updateBlynkFeedingTimes(); // send feeding times to blynkApp

    myStepper.setSpeed(stepperSpeed);

    // Setup timer events
    // Values are given in milliseconds
    timer.setInterval(60000L, CheckConnection);
    timer.setInterval(10000L, CheckIfItIsTimeToFeedThePet);
    timer.setInterval(60000L, UpdateBlynkFeedingTimes);
}

void loop() {

    if (Blynk.connected()) {
        Blynk.run(); // magic asynchronous blynk code
    }

    timer.run(); //more magic switch

    if (firstRunOnly && year() > 1970) { // Time.lib returns unix time 0 (Jan 1 1970) by default
        currentTime = wink.blynkTimeAsByte();
        myTime.prepFeedingTimes(currentTime);
        firstRunOnly = false;
    }

    // To reset at midnight:
    if (hour() == 0) {
        myTime.resetFedTimes();
    }
}

void spinMotor() {
    for (int i = 0; i < numOfTurns; i++) {

        myStepper.step(STEPSBACKWARD);
        yield(); // If we don't yield, the watchdog bites, throws a stack trace, and
                 // resets the module.
        myStepper.step(STEPSBACKWARD);
        yield();
        myStepper.step(STEPSFORWARD);
        yield(); // maximum safe length of time at 100 speed is around 500 steps, so
                 // we must yield a lot
        myStepper.step(STEPSFORWARD);
        yield();
    }
}

void CheckIfItIsTimeToFeedThePet() {
    Serial.print("Current Blynk time is: ");
    Serial.println(wink.blynkTimeAsByte());

    if (myTime.itIsFeedingTime(wink.blynkTimeAsByte())) {
        Serial.println("Thus, it is feeding time!");
        spinMotor();
    } else {
        Serial.println("Thus, it is not feeding time.");
    }
}

void CheckConnection() { // check if connected to Blynk server
    if (!Blynk.connected()) {
        Serial.println("Not connected to Blynk");
        if (Blynk.connect(10000)) { // timeout in milliseconds
            Serial.println("Connected to Blynk!");

        } else {
            Serial.println("Not connected, failing through to backup...");
        }
    } else {
        Serial.println("Connected to Blynk!");
    }
}

void UpdateBlynkFeedingTimes() { // passthrough function because blynkTimer
                                 // cannot take member functions of classes
    wink.updateBlynkFeedingTimes();
}

//*###################
// BLINKITYBLINK_CPP IS DEFINED BELOW

void BlinkityBlink::resetMidnightMillis() {
    millisAsOfMostRecentMidnight = millis();
}

void BlinkityBlink::updateTime() {
    currentTimeAsByte = hour() * 10 + minute() / 10; // integer math tricks
}

byte BlinkityBlink::blynkTimeAsByte() {
    updateTime();
    return currentTimeAsByte;
}

void BlinkityBlink::nightlyReset() { resetMidnightMillis(); }

void BlinkityBlink::updateBlynkFeedingTimes() {
    Blynk.virtualWrite(V1, myTime.getFeedingTimes());
}

//*##########################
// END BLINKITYBLINK_CPP
