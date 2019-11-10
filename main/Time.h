#include <NTPClient.h>
#include <WiFiUdp.h>
//#include<algorithm>
#include <ESP8266WiFi.h>

#ifndef TIME_H
#define TIME_H

class Time{
private:
public:

  //takes standard format time of day as byte, returns time of day in milliseconds since 00:00
  unsigned long timeByteToMillis(byte timeByte);
  
  //takes time of day in milliseconds since 00:00, returns standard format time of day as a byte
  byte millisToTimeByte(unsigned long timeInMillis);
  
  byte getCurrentNetworkTimeByte(unsigned long* networkMillis, NTPClient timeClient);
  
  unsigned long millisAtMidnight(unsigned long networkMillis, unsigned char networkTimeByte);  
};

#endif
