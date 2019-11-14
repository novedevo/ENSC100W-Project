//#include <NTPClient.h>
//#include <WiFiUdp.h>
//#include<algorithm>
#include <ESP8266WiFi.h>

#ifndef ADDITIONAL_H
#define ADDITIONAL_H

class Time{

private:
  //for internal use only
  byte index = 5;
  byte feedingTimes[4];
  bool fedTimes[4];

public:

  //constructor, takes two arrays and defines them
  Time (const byte (&feedingTimes)[4], const bool (&fedTimes)[4]){
    for(int i = 0; i<4; i++){
      this->feedingTimes[i] = feedingTimes[i];
      this->fedTimes[i] = fedTimes[i];
    }
    
    //this->feedingTimes = feedingTimes;
    //this->fedTimes = fedTimes;
  }

  //Takes comma-deliminated list of exactly four times
  //Standard time formatting
  void setFeedingTimes(String timesAsString);

  String getFeedingTimes();
/*
  //takes standard format time of day as byte
  //returns time of day in milliseconds since 00:00
  unsigned long timeByteToMillis(byte timeByte);
  
  //takes time of day in milliseconds since 00:00
  //returns standard format time of day as a byte
  byte millisToTimeByte(unsigned long timeInMillis);
  
  //returns time of day as byte from network
  byte getCurrentNetworkTimeByte(unsigned long* networkMillis, NTPClient* timeClient);
  
  //returns what the millis would have said at midnight
  //TODO: why does it work if it is unsigned, what if millis wasn't running at midnight?
  unsigned long millisAtMidnight(unsigned long networkMillis, unsigned char networkTimeByte);  

  //Returns time from network if it's connected to wifi, otherwise fallbacks to millis
  //TODO: test this over a long period of time, figure out max variation, do tests!!
  byte improvedGetTimeByte(unsigned long* networkMillis, 
                           NTPClient* timeClient, byte networkTimeByte);

  //returns time as calculated from millis
  byte getEstimatedTime(unsigned long networkMillis, byte networkTimeByte);
*/
  //checks if it is feeding time
  bool itIsFeedingTime(byte currentTime);

  //sorts, sets any times earlier than the current time
  //to "pet has been fed"
  void prepFeedingTimes(byte currentTime);

  //iterates through feedingTimes and prints each value to Serial
  //for debugging
  void printFeedingTimes();

  //bubblesorts them
  void sortFeedingTimes();

  //sets all fedTimes to false, for use at midnight or whenever
  void resetFedTimes();
};

#endif
