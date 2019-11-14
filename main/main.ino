//#include <NTPClient.h>  //for network time
//#include <WiFiUdp.h>    //for udp of network time
#include "BlinkityBlink.h"
//#include <TimeLib.h>
//#include <BlynkSimpleEsp8266.h>   //for blynk control over wifi
//#include <WidgetRTC.h>    //for blynk real time clock
#include "Additional.h"   //Additional code, mainly Time class
#include "Server.h"       //additional code to run webserver
#include <ESP8266WiFi.h>  //basic functionality

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
const char *auth = "2tO0Cw7-eFdDYImgMxbUImfkYbV4-7t5";  //auth key for Blynk

const byte feedingTimes[4] = {183,100,000,94};
const bool fedTimes[4] = {0,0,0,0};

//long utcOffsetInSeconds = -8*60*60;
int numOfTurns = 1;
unsigned long currentMillis = 0;
unsigned long millisOnLastTimeCheck = 0;
unsigned long millisOnLastBlynkFeeding = 0;
//unsigned long networkMillis = 0;
//unsigned long debugNetworkMillis = 0;
//byte networkTimeByte = 0;
//byte debugNetworkTimeByte = 0;
byte currentTime = 0;
byte fallbackTime = 0;
byte idealCaseTime = 0;

//Instantiate objects:
Time myTime(feedingTimes, fedTimes);
//ConfigServer server(80);  //argument represents the port
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//BlynkTimer timer;
WidgetRTC rtc;

BlinkityBlink wink;

//To be called when Blynk app sends "Feed Now"
BLYNK_WRITE(V0)
{
  currentMillis = millis();
  if (param.asInt()){
    if (currentMillis-millisOnLastBlynkFeeding >= 10000){
      //motor.spinMotor();
      millisOnLastBlynkFeeding = currentMillis;
        Serial.println("Remote connection from Blynk App ordered pet fed:");
        Serial.println("Running extra feeding cycle..");
    } else {
      Serial.println("Too little time has elapsed since last feeding.");
      Serial.println("Taking no action.");
    }
  }
}

//To be called when Blynk App sends feeding times
BLYNK_WRITE(V1){
  myTime.setFeedingTimes(param.asStr());
  myTime.sortFeedingTimes();
  myTime.prepFeedingTimes(currentTime);
  myTime.printFeedingTimes();
}

//To be called when Blynk App sends a number of turns
BLYNK_WRITE(V2){
  numOfTurns = param.asInt();
}

//Run once when the board is reset
void setup(){
    
    //Update currentMillis (redundant)
    currentMillis = millis();
    
    Serial.begin(9600);
    
    Serial.println(" ");  //because of garbage data written to serial when board is reset
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    Blynk.begin(auth, ssid, password);
    
    //begin synchronizing time
    wink.begin();

    currentTime = wink.blynkTimeAsByte();

    //server.begin(); //uncomment if webserver functionality is needed
    /*
    timeClient.begin();   //TODO: understand why I needed a networkTimeByte
                          //Don't comment these out or it won't work
    networkTimeByte = myTime.getCurrentNetworkTimeByte(&networkMillis, &timeClient);
    currentTime = networkTimeByte;
    */
    ////Serial.println(networkMillis);
    ////Serial.println(networkTimeByte);

    myTime.prepFeedingTimes(currentTime);
    ////myTime.debugFunction(feedingTimes);
}

void loop(){
    
    Blynk.run();
    Blynk.virtualWrite(V1, myTime.getFeedingTimes());
    currentMillis = millis();
    if (currentMillis-millisOnLastTimeCheck>=10000){
      //TODO: why why why is any of this necessary? 
      //TODO: Can I just use blynk? will that not work or be too much?
      millisOnLastTimeCheck = currentMillis;
      //currentTime = myTime.improvedGetTimeByte(&networkMillis, &timeClient, networkTimeByte);
      
      currentTime = wink.blynkTimeAsByte();
      fallbackTime = wink.fallbackTimeAsByte();
      idealCaseTime = wink.idealCaseTimeAsByte();
      
      Serial.print("Current Blynk time is: ");
      Serial.println(currentTime);

      Serial.print("Fallback time is: ");
      Serial.println(fallbackTime);

      Serial.print("Ideal case time is: ");
      Serial.println(idealCaseTime);

      //Serial.print("Blynk claims time is: ");
      //Serial.println(hour()*10+minute()/10);
  
      //Serial.print("Next feeding time is: ");
  
      if (myTime.itIsFeedingTime(currentTime)){
        Serial.println("Thus, it is feeding time!");
        //motor.spinMotor();
      } else {
        Serial.println("Thus, it is not feeding time.");
      }

    }

    //To reset at midnight:
    if (currentTime == 0){
      myTime.resetFedTimes();
    }

    //server.handleClients(); //uncomment if webserver functionality is needed

    //configurable delay to avoid overheating the arduino
    delay(500);
}
