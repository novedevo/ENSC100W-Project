#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const long utcOffsetInSeconds = -8*60*60;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
    timeClient.begin();
}
byte getCurrentTime(){
    timeClient.update();
    return(timeClient.getHours() * 10 + timeClient.getMinutes() / 10);
}