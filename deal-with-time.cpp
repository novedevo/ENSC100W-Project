#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const long utcOffsetInSeconds = -8*60*60;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
    timeClient.begin();
}
byte getCurrentTimeByte(unsigned long* networkMillis){
    timeClient.update();
    *networkMillis = millis();
    return(timeClient.getHours() * 10 + timeClient.getMinutes() / 10);
}

unsigned long timeByteToMillis(byte b, byte n, unsigned long networkMillis){
    
    int hours = b/10;
    int minutes = (b%10)*10;
    int networkHours = n/10;
    int networkMinutes = (b%10)*10;

    int deltaHours = hours-networkHours;
    int deltaMinutes = minutes-networkMinutes;

    int deltaMillis = (deltaHours*60+deltaMinutes)*60*1000;
    int newMillis = networkMillis + deltaMillis;

}