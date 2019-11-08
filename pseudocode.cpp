#include <iostream>
#include <included.h>
#include <deal-with-time.cpp>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include<algorithm>
#include <ESP8266WiFi.h>
using namespace std;

//*     example times 
//      12:00am = 000
//      1:00am = 010
//      4:30am = 043
//      12:00pm = 120
//      8:20pm = 202
//      1550 hours = 155
//      00:00 = 000


//declaring constants and variables
const char *ssid     = "YOUR_SSID";
const char *password = "YOUR_PASS";

long utcOffsetInSeconds = -8*60*60;
unsigned long currentMillis;
byte feedingTimes[4] = {000,100,230,120};
bool timesFed[4] = {0,0,0,0};
byte currentTime = 0;

void setup(){
    currentMillis = millis();
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
    timeClient.begin();
    WiFiServer server(80);
    
    currentTime = getCurrentTime();
}


// Swaps a with b



void placeholder(){

    bubbleSort(feedingTimes, sizeof(feedingTimes)/sizeof(feedingTimes[0]));
    currentTime = toUnsignedChar(millis());
    if (!timesFed[i] && feedingTimes[i]>=toUnsignedChar(currentTime)){;}

    interrupt( (postRequest)){
        if (numberOfTurns){
            this->numberOfTurns = numberOfTurns;
        }
        if (feedNow){
            stepper.turn(numberOfTurns);
        }
        if (newFeedTime){
            
            feedingTimes[i] = newFeedTime;
        }

    }
}

void placeholder2(){
    unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= interval) {;}
    previousMillis = currentMillis;
    delay(5*1000*60);
}

int main(){
    placeholder();
}