//#include <iostream>
//#include "included.cpp"
//#include "deal-with-time.cpp"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp8266.h>
#include "Additional.h"
#include "Server.h"
//#include<algorithm>
#include <ESP8266WiFi.h>
using namespace std;

//        example times 
/*        1:00am    =>  010
          4:30am    =>  043
          12:00pm   =>  120
          8:20pm    =>  202
          1550 hrs  =>  155
          00:00     =>  000
          5:42am    =>  054
          5:49am    =>  054

        Invalid Times:
          Anything where the first two digits aren't between 0 and 23 inclusive,
          or the last digit isn't between 0 and 5 inclusive.
  
          000 is an invalid time, as it is the default value of our array. 
          Feed your cat at 23:50 or 00:10.
  
          for example:
          240
          239
          006

*/

//declaring constants and variables
const char *ssid     = "Pixel_7859";
const char *password = "redwood13";
const char *auth = "2tO0Cw7-eFdDYImgMxbUImfkYbV4-7t5";

long utcOffsetInSeconds = -8*60*60;
unsigned long currentMillis = 0;
unsigned long millisOnLastTimeCheck = 0;
unsigned long millisOnLastBlynkFeeding = 0;
unsigned long networkMillis = 0;
unsigned long debugNetworkMillis = 0;
const byte feedingTimes[4] = {183,100,000,94};
const bool fedTimes[4] = {0,0,0,0};
byte networkTimeByte = 0;
byte debugNetworkTimeByte = 0;
byte currentTime = 0;

//Instantiate objects:
Time myTime(feedingTimes, fedTimes);
//ConfigServer server(80);  //argument represents the port
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

BLYNK_WRITE(V0)
{
  currentMillis = millis();
  if (param.asInt()){
    if (currentMillis-millisOnLastBlynkFeeding >= 10000){
      //motor.spinMotor();
      millisOnLastBlynkFeeding = currentMillis;
        Serial.println("Remote connection from Blynk App ordered pet fed:");
        Serial.println("Running extra feeding cycle..");
    }else{
      Serial.println("Too little time has elapsed since last feeding.");
      Serial.println("Taking no action.");
    }
  }
}

BLYNK_WRITE(V1){
  myTime.setFeedingTimes(param.asStr());
  myTime.sortFeedingTimes();
  myTime.prepFeedingTimes(currentTime);
  myTime.printFeedingTimes();
}



void setup(){
    currentMillis = millis();
    
    Serial.begin(9600);
    
    Serial.println(" ");
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    Blynk.begin(auth, ssid, password);
    //server.begin();
    
    timeClient.begin();
    networkTimeByte = myTime.getCurrentNetworkTimeByte(&networkMillis, &timeClient);
    currentTime = networkTimeByte;
    
    //Serial.println(networkMillis);
    //Serial.println(networkTimeByte);
    myTime.prepFeedingTimes(networkTimeByte);
    //myTime.debugFunction(feedingTimes);
}

void loop(){
    //Serial.println(millis());
    //Serial.print("Current time from network is: ");
    //debugNetworkTimeByte = myTime.getCurrentNetworkTimeByte(&debugNetworkMillis, &timeClient);
    //if (debugNetworkTimeByte >=240){
    //  Serial.println("network time failed");
    //}
    //else Serial.println((debugNetworkTimeByte));
    //TODO: FIXME: for some reason it starts to overflow (???) after a while of being on and begins to break? resetting fixes it? memory leak? tftftf?
    
    //currentTime = myTime.millisToTimeByte(millis() - myTime.millisAtMidnight(networkMillis, networkTimeByte));

    //Serial.print("Current estimated time from millis() is: ");
    //Serial.println((currentTime));
    
    Blynk.run();
    
    currentMillis = millis();
    if (currentMillis-millisOnLastTimeCheck>=10000){
      millisOnLastTimeCheck = currentMillis;
      currentTime = myTime.improvedGetTimeByte(&networkMillis, &timeClient, networkTimeByte);
      Serial.print("Current ideal case time is: ");
      Serial.println(currentTime);
  
      //Serial.print("Next feeding time is: ");
  
      if (myTime.itIsFeedingTime(currentTime)){
        Serial.println("Thus, it is feeding time!");
        //motor.spinMotor();
        
      }
      else Serial.println("Thus, it is not feeding time.");
    }
    if (currentTime == 0){
      myTime.resetFedTimes();
    }
    //server.handleClients();

    delay(500);
}
