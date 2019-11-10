#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const long utcOffsetInSeconds = -8*60*60;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
    timeClient.begin();
}

void loop(){
    ;
}

byte getCurrentTimeByte(unsigned long* networkMillis){
    timeClient.update();
    *networkMillis = millis();
    return(timeClient.getHours() * 10 + timeClient.getMinutes() / 10);
}

unsigned long millisAtMidnight(unsigned long networkMillis, unsigned char networkTimeByte){
    unsigned long millisAtMidnight = networkMillis - timeByteToMillis(networkTimeByte);
    return millisAtMidnight;
}
