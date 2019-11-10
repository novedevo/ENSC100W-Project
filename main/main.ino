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

        Invalid Times:
        Anything where the first two digits aren't between 0 and 23 inclusive,
        or the last digit isn't between 0 and 5 inclusive.

        (0 <first two digits <= 23) && 

        for example:
        240
        239
        006

*/

//declaring constants and variables
const char *ssid     = "Aesthetics_Guest";
const char *password = "TB_Guest";

long utcOffsetInSeconds = -8*60*60;
unsigned long currentMillis = 0;
unsigned long networkMillis = 0;
unsigned long debugNetworkMillis = 0;
unsigned long millisAt000 = 0;
byte feedingTimes[4] = {000,100,230,120};
bool timesFed[4] = {0,0,0,0};
byte networkTimeByte = 0;
byte debugNetworkTimeByte = 0;
byte currentTime = 0;

//store HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;

//Instantiate objects from headers:
Time myTime;
Webserver myWebserver;

// Current time
unsigned long currentMillisTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

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
    
    networkTimeByte = myTime.getCurrentNetworkTimeByte(&networkMillis, timeClient);
    Serial.println(networkMillis);
    Serial.println(networkTimeByte);
}

void loop(){
    ////bubbleSort(feedingTimes, sizeof(feedingTimes)/sizeof(feedingTimes[0]));
    Serial.println(millis());
    Serial.print("Current time from network is: ");
    debugNetworkTimeByte = myTime.getCurrentNetworkTimeByte(&debugNetworkMillis, timeClient);
    Serial.println((debugNetworkTimeByte));
    //TODO: FIXME: for some reason it starts to overflow (???) after a while of being on and begins to break? resetting fixes it? memory leak? tftftf?
    
    currentTime = myTime.millisToTimeByte(millis() - myTime.millisAtMidnight(networkMillis, networkTimeByte));

    Serial.print("Current estimated time from millis() is: ");
    Serial.println((currentTime));

    myWebserver.handleClients(&server);

    delay(1000);
}
