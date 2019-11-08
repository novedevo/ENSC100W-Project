#include <iostream>
#include <included.h>
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

////void setup(){
    float currentMillis = millis();
    WiFiServer server(80);
    unsigned char feedingTimes[4] = {0,1,250,12};
    bool timesFed[4] = {0,0,0,0};
    unsigned char currentTime = getCurrentTime();
////}


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