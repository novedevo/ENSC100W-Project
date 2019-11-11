//#include <iostream>
//#include "included.cpp"
//#include "deal-with-time.cpp"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Additional.h"
#include "Server.h"
//#include<algorithm>
#include <ESP8266WiFi.h>
using namespace std;

//      example times 
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

long utcOffsetInSeconds = -8*60*60;
unsigned long currentMillis = 0;
unsigned long networkMillis = 0;
unsigned long debugNetworkMillis = 0;
byte feedingTimes[4] = {183,100,000,120};
bool fedTimes[4] = {0,0,0,0};
byte networkTimeByte = 0;
byte debugNetworkTimeByte = 0;
byte currentTime = 0;

//Instantiate objects from headers:
Time myTime;
Webserver myWebserver;

WiFiServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
    Serial.begin(9600);
    //currentMillis = millis();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
    
    timeClient.begin();
    
    networkTimeByte = myTime.getCurrentNetworkTimeByte(&networkMillis, &timeClient);
    Serial.println(networkMillis);
    Serial.println(networkTimeByte);
    myTime.prepFeedingTimes(networkTimeByte, feedingTimes, fedTimes);
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

    Serial.print("Current ideal case time is: ");
    Serial.println(myTime.improvedGetTimeByte(&networkMillis, &timeClient, networkTimeByte));

    //Serial.print("Next feeding time is: ");

    if (myTime.itIsFeedingTime(currentTime, feedingTimes, fedTimes)){
      Serial.println("Thus, it is feeding time!");
    }
    else Serial.println("Thus, it is not feeding time.");

    //myWebserver.handleClients(&server);

    

    delay(1000);
}
