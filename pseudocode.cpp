#include <iostream>
#include <included.h>
#include <deal-with-time.cpp>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include<algorithm>
#include <ESP8266WiFi.h>
using namespace std;

//*     example times 
/*      12:00am   =>  000
        1:00am    =>  010
        4:30am    =>  043
        12:00pm   =>  120
        8:20pm    =>  202
        1550 hrs  =>  155
        00:00     =>  000
        5:42am    =>  054
        5:49am    =>  054
*/

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

int main(){
    placeholder();
}