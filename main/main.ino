#define BLYNK_TIMEOUT_MS  500  // must be BEFORE BlynkSimpleEsp8266.h doesn't work !!!
#define BLYNK_HEARTBEAT   17   // must be BEFORE BlynkSimpleEsp8266.h works OK as 17s
#define BLYNK_PRINT Serial    

#include <TimeLib.h>
#include <BlynkSimpleEsp8266.h>   //for blynk control over wifi
#include <WidgetRTC.h>    //for blynk real time clock
#include "Additional.h"   //Additional code, mainly Time class
#include "Server.h"       //additional code to run webserver
#include <ESP8266WiFi.h>  //basic functionality

//TODO: Implement persistant timestamp capabilities?
//use blynk?

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
const char *ssid     = "Aesthetics_Guest";
const char *password = "TB_Guest";
const char *auth = "***REMOVED***";  //auth key for Blynk

const byte feedingTimes[4] = {70,120,190,0};
const bool fedTimes[4] = {0,0,0,0};

int numOfTurns = 1;
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
//BETWEEN THESE LINES USED TO BE THE CONTENTS OF ITS OWN HEADER FILE
//A LIBRARY I WAS USING IS TERRIBLY CODED AND THUS MAY ONLY BE INCLUDED IN THE MAIN INO FILE
//
//BLINKITYBLINK_H IS DEFINED BELOW




class BlinkityBlink : public WidgetRTC {
  private:
    byte currentTimeAsByte = 0;
    signed long long millisAsOfMostRecentMidnight = 0;

    void resetMidnightMillis();

    void updateTime();
    
  public:
    byte blynkTimeAsByte();
    
    explicit BlinkityBlink() : WidgetRTC() {}
    
    void nightlyReset();
    
    void updateBlynkFeedingTimes();
};



//######################
//END BLINKITYBLINK_H
//SEE BOTTOM OF FILE FOR BLINKITYBLINK_CPP





//Instantiate objects:

//ConfigServer server(80);  //argument represents the port

Time myTime(feedingTimes);
BlynkTimer timer;
//WidgetRTC rtc;
BlinkityBlink wink;




//###########################
//Asynchronous Blynk Functions:

//To be called when Blynk app sends "Feed Now"
BLYNK_WRITE(V0)
{
  currentMillis = millis();
  if (param.asInt()){
    if (currentMillis-millisOnLastBlynkFeeding >= 10000){
      //motor.spinMotor();
      millisOnLastBlynkFeeding = currentMillis;
        if(param.asInt() == 10) {
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
  Serial.println(numOfTurns);
}

BLYNK_WRITE(V3){
  if (param.asInt()){
    Serial.print("true");
  } else {Serial.print("false");}
}

//################################
//End Asynchronous Blynk Functions



//Run once when the board is reset
void setup(){  
    Serial.begin(9600);
    
    Serial.println(" ");  //because of garbage data written to serial when board is reset
    Serial.print("Connecting to ");
    Serial.println(ssid);

    CheckConnection();
    wink.begin();
    wink.updateBlynkFeedingTimes();

    timer.setInterval(60000L, CheckConnection);
    timer.setInterval(10000L, CheckIfItIsTimeToFeedThePet);
    timer.setInterval(60000L, UpdateBlynkFeedingTimes);
    
}

void loop(){
    if(Blynk.connected()){
      Blynk.run();
    }
    
    timer.run();
    
    if (firstRunOnly && year() != 1970){
      currentTime = wink.blynkTimeAsByte();
      myTime.prepFeedingTimes(currentTime);
      firstRunOnly = false;
    }
    
    //To reset at midnight:
    if (hour() == 0){
      myTime.resetFedTimes();
    }

    //server.handleClients(); //uncomment if webserver functionality is needed
}

void CheckIfItIsTimeToFeedThePet(){
  Serial.print("Current Blynk time is: ");
  Serial.println(wink.blynkTimeAsByte());
  
  if (myTime.itIsFeedingTime(wink.blynkTimeAsByte())){
    Serial.println("Thus, it is feeding time!");
    //motor.spinMotor();
  } else {
    Serial.println("Thus, it is not feeding time.");
  }
}

void CheckConnection(){    // check every 11s if connected to Blynk server
  if(!Blynk.connected()){
    Serial.println("Not connected to Blynk");
    Blynk.begin(auth, ssid, password);
  } else {
    Serial.println("Connected to Blynk!");
  }
}

void UpdateBlynkFeedingTimes(){
  wink.updateBlynkFeedingTimes();
}

//###################
//BLINKITYBLINK_CPP IS DEFINED BELOW


void BlinkityBlink::resetMidnightMillis(){
  millisAsOfMostRecentMidnight = millis();
}

void BlinkityBlink::updateTime(){
  currentTimeAsByte = hour()*10+minute()/10;
}

byte BlinkityBlink::blynkTimeAsByte(){
  updateTime();
  return currentTimeAsByte;
}

void BlinkityBlink::nightlyReset(){
  resetMidnightMillis();
}

void BlinkityBlink::updateBlynkFeedingTimes(){
  Blynk.virtualWrite(V1, myTime.getFeedingTimes());
}

//##########################
//END BLINKITYBLINK_CPP
