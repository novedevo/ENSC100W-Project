//#include <NTPClient.h>  //for network time
//#include <WiFiUdp.h>    //for udp of network time
//#include "BlinkityBlink.h"
#include <TimeLib.h>
#include <BlynkSimpleEsp8266.h>   //for blynk control over wifi
#include <WidgetRTC.h>    //for blynk real time clock
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
const char *ssid     = "Aesthetics_Guest";
const char *password = "TB_Guest";
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
bool firstRunOnly = true;

//bool on = 0;
//bool online = 0;



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

    byte fallbackTimeAsByte();

    byte idealCaseTimeAsByte();
    
    explicit BlinkityBlink() : WidgetRTC() {}
    void nightlyReset();
    void updateBlynkFeedingTimes();
    
};



//######################
//END BLINKITYBLINK_H
//SEE BOTTOM OF FILE FOR BLINKITYBLINK_CPP





//Instantiate objects:
Time myTime(feedingTimes, fedTimes);
//ConfigServer server(80);  //argument represents the port
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

BlynkTimer timer;
//WidgetRTC rtc;
BlinkityBlink wink;



//Asynchronous Blynk Functions:

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

//End Asynchronous Blynk Functions



//Run once when the board is reset
void setup(){
    
    //Update currentMillis (redundant)
    currentMillis = millis();
    
    Serial.begin(9600);
    
    Serial.println(" ");  //because of garbage data written to serial when board is reset
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    Blynk.begin(auth, ssid, password);
    //delay(1000);
    //while(!Blynk.connect() && millis()<20000){delay(50);}
    //Blynk.run();
    //delay(1000);
    //begin synchronizing time
    wink.begin();
    //delay(1000);
    //Serial.println(Blynk.connected());
    //Serial.println(hour());

    //for (int i = 0; i<10; i++){
    //  Serial.println(wink.blynkTimeAsByte());
    //}

    //while(year()==1970){
    //  Serial.println("Waiting to get time...");
    //  delay(1000);
    //}

    //currentTime = wink.blynkTimeAsByte();

    //Serial.println(" ");
    
    //server.begin(); //uncomment if webserver functionality is needed
    /*
    timeClient.begin();   //TODO: understand why I needed a networkTimeByte
                          //Don't comment these out or it won't work
    networkTimeByte = myTime.getCurrentNetworkTimeByte(&networkMillis, &timeClient);
    currentTime = networkTimeByte;
    */
    ////Serial.println(networkMillis);
    ////Serial.println(networkTimeByte);
    //myTime.prepFeedingTimes(currentTime);
    ////myTime.debugFunction(feedingTimes);
    //Serial.println(year());

    //timer.setInterval(5000L, CheckConnection);
}

void loop(){
    //Serial.println(year());
    //if(Blynk.connected()){
      Blynk.run();
    //}
    
    //timer.run();
    //currentTime = wink.blynkTimeAsByte();
    
    if (firstRunOnly && year() != 1970){
      currentTime = wink.blynkTimeAsByte();
      myTime.prepFeedingTimes(currentTime);
      firstRunOnly = false;
    }
    
    //Serial.print("after blynk.run()... ");
    //Serial.println(year());
    //Serial.println(wink.blynkTimeAsByte());
    wink.updateBlynkFeedingTimes();
    
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

byte BlinkityBlink::fallbackTimeAsByte(){  //!#############
  return 254; //TODO: IMPLEMENT
}

byte BlinkityBlink::idealCaseTimeAsByte(){           //!#################
  if(Blynk.connected()){
    return blynkTimeAsByte();
  } else {
    return fallbackTimeAsByte();
  }
}

void BlinkityBlink::nightlyReset(){
  resetMidnightMillis();
}

void BlinkityBlink::updateBlynkFeedingTimes(){
  Blynk.virtualWrite(V1, myTime.getFeedingTimes());
}



//##########################
//END BLINKITYBLINK_CPP
