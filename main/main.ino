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

//*       example times 
/*        1:00am    =>  010
          4:30am    =>  043
          12:00pm   =>  120
          8:20pm    =>  202
          1550 hrs  =>  155
          00:00     =>  000
          5:42am    =>  054
          5:49am    =>  054

*       Invalid Times:
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

const byte feedingTimes[4] = {70,120,190,0};  //default initialization
const bool fedTimes[4] = {0,0,0,0};   //TODO: remove?

int numOfTurns = 1;   //number of turns the motor, and thus auger, will make
                      //equivalent to changing portion size
unsigned long currentMillis = 0;    //TODO: remove? replace with millis() calls?
unsigned long millisOnLastTimeCheck = 0;    //TODO: remove?
unsigned long millisOnLastBlynkFeeding = 0;    
byte currentTime = 0;
byte fallbackTime = 0;    //TODO: remove?
byte idealCaseTime = 0;    //TODO: remove?
bool firstRunOnly = true;

bool on = 0;    //TODO: remove?
bool online = 0;    //TODO: remove?



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
    
    explicit BlinkityBlink() : WidgetRTC() {} //to inherit constructor
    
    void nightlyReset();
    
    void updateBlynkFeedingTimes();   //sends data to blynkapp
};



//######################
//END BLINKITYBLINK_H
//SEE BOTTOM OF FILE FOR BLINKITYBLINK_CPP





//Instantiate objects:

//ConfigServer server(80);  //argument represents the port

Time myTime(feedingTimes);    //TODO: give Time class default initializations?
BlynkTimer timer;
//WidgetRTC rtc;      //TODO: remove?
BlinkityBlink wink;   //* inherits from WidgetRTC




//*###########################
//*Asynchronous Blynk Functions:

//To be called when Blynk app sends "Feed Now"
BLYNK_WRITE(V0)
{
  currentMillis = millis();
  if (param.asInt()){
    if (currentMillis-millisOnLastBlynkFeeding >= 10000){ //if it's been more than 10 seconds since last request
      //motor.spinMotor();    //TODO: uncomment when motor functionality is merged.
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

//To be called when Blynk App sends feeding times
BLYNK_WRITE(V1){
  myTime.setFeedingTimes(param.asStr());
  myTime.sortFeedingTimes();
  myTime.prepFeedingTimes(currentTime); //TODO: implement fron wink class?
  myTime.printFeedingTimes();
}

//To be called when Blynk App sends a number of turns
BLYNK_WRITE(V2){
  numOfTurns = param.asInt();
  Serial.println(numOfTurns);
}

//Legacy code from early IFTTT testing
//Was called when google assistant made a request
//now deprecated in favour of different parametres for V0
//TODO: remove?
BLYNK_WRITE(V3){
  if (param.asInt()){
    Serial.print("true");
  } else {Serial.print("false");}
}

//*################################
//End Asynchronous Blynk Functions



//Run once when the board is reset
void setup(){  
    Serial.begin(9600);
    
    Serial.println(" ");  //because of garbage data written to serial when board is reset
    Serial.print("Connecting to ");
    Serial.println(ssid);

    CheckConnection();    //also initiates connection to wifi and blynkapp
    wink.begin();         //begin tracking time
    wink.updateBlynkFeedingTimes();   //send feeding times to blynkApp


    //Setup timer events
    //Values are given in milliseconds
    timer.setInterval(60000L, CheckConnection);
    timer.setInterval(10000L, CheckIfItIsTimeToFeedThePet);
    timer.setInterval(60000L, UpdateBlynkFeedingTimes);
    
}

void loop(){

    if(Blynk.connected()){
      Blynk.run();      //magic blynk code, honestly
    }
    
    timer.run();
    
    if (firstRunOnly && year() != 1970){
      currentTime = wink.blynkTimeAsByte();
      myTime.prepFeedingTimes(currentTime);
      firstRunOnly = false;
    }   //for some reason this didn't work in setup() so it is here
    
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

void CheckConnection(){   // check if connected to Blynk server
                          //! //TODO: Test if this actually works
  if(!Blynk.connected()){
    Serial.println("Not connected to Blynk");
    Blynk.begin(auth, ssid, password);
  } else {
    Serial.println("Connected to Blynk!");
  }
}

void UpdateBlynkFeedingTimes(){ //passthrough function because blynkTimer cannot
                                //take member functions of classes for some reason
  wink.updateBlynkFeedingTimes();
}



//*###################
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

//*##########################
//END BLINKITYBLINK_CPP
