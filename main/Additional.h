#include <NTPClient.h>
#include <WiFiUdp.h>
//#include<algorithm>
#include <ESP8266WiFi.h>

#ifndef ADDITIONAL_H
#define ADDITIONAL_H

class Time{
private:
  byte index = 5;
  byte feedingTimes[4];
  bool fedTimes[4];
public:

  Time(const byte (&feedingTimes)[4], const bool (&fedTimes)[4]){
    for(int i = 0; i<4; i++){
      this->feedingTimes[i] = feedingTimes[i];
      this->fedTimes[i] = fedTimes[i];
    }
    
    //this->feedingTimes = feedingTimes;
    //this->fedTimes = fedTimes;
  }

  //takes standard format time of day as byte
  //returns time of day in milliseconds since 00:00
  unsigned long timeByteToMillis(byte timeByte);
  
  //takes time of day in milliseconds since 00:00
  //returns standard format time of day as a byte
  byte millisToTimeByte(unsigned long timeInMillis);
  
  byte getCurrentNetworkTimeByte(unsigned long* networkMillis, NTPClient* timeClient);
  
  unsigned long millisAtMidnight(unsigned long networkMillis, unsigned char networkTimeByte);  

  byte improvedGetTimeByte(unsigned long* networkMillis, 
                           NTPClient* timeClient, byte networkTimeByte);

  byte getEstimatedTime(unsigned long networkMillis, byte networkTimeByte);

  bool itIsFeedingTime(byte currentTime);

  void prepFeedingTimes(byte currentTime);
};

class DataManip{
  private:
  public:
    void bubbleSort (byte arr[], int n);
};


#endif
