#include "Time.h"

unsigned long Time::timeByteToMillis(byte timeByte){
    int hours = timeByte / 10;
    int minutes = (timeByte % 10) * 10;
  
    if (hours >= 24 || minutes >= 60) {   //impossible times in standard 24-hours time
      return 0;
      //throw "invalidTimeByte";
    }
  
    unsigned long timeInMillis = (hours * 60 + minutes) * 60 * 1000;
    return timeInMillis;
}

byte Time::millisToTimeByte(unsigned long timeInMillis) {
  int hours = timeInMillis / 1000 / 60 / 60;
  int minutes = (timeInMillis / 1000 / 60 ) - hours * 60;
  
  if (hours >= 24 || minutes >= 60) {   //impossible times in standard 24-hours time
    return 0;
    //throw "invalidMillisTime";
  }
  
  byte timeByte = hours * 10 + minutes / 10;
  return timeByte;
}

byte Time::getCurrentNetworkTimeByte(unsigned long* networkMillis, NTPClient timeClient) {
    timeClient.update();
    *networkMillis = millis();
    return (timeClient.getHours() * 10 + timeClient.getMinutes() / 10);
}

unsigned long Time::millisAtMidnight(unsigned long networkMillis, unsigned char networkTimeByte) {
  unsigned long millisAtMidnight = networkMillis - timeByteToMillis(networkTimeByte);
  return millisAtMidnight;
}
