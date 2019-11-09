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

unsigned long timeByteToMillis(unsigned char timeByte, unsigned char networkTimeByte, unsigned long networkMillis){
    
    int hours = timeByte/10;
    int minutes = (timeByte%10)*10;
    int networkHours = networkTimeByte/10;
    int networkMinutes = (networkTimeByte%10)*10;
    int deltaHours;
    int deltaMinutes;

    if (networkTimeByte>timeByte){
        int bint = 240+timeByte;
        hours = bint/10;
        minutes = (bint%10)*10;
    }

    int millisoffset = (networkHours*60+networkMinutes)*60*1000

    deltaHours = hours-networkHours;
    deltaMinutes = minutes-networkMinutes;
    ////cout<<deltaHours<<endl<<deltaMinutes<<endl;
    int deltaMillis = (deltaHours*60+deltaMinutes)*60*1000;
    ////cout<<deltaMillis<<endl;
    int newMillis = networkMillis + deltaMillis;
}