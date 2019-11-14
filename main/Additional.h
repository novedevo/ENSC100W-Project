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
  Time (const byte (&feedingTimes)[4]){
    for(int i = 0; i<4; i++){
      this->feedingTimes[i] = feedingTimes[i];
      this->fedTimes[i] = 0;
    }
    
    //this->feedingTimes = feedingTimes;
    //this->fedTimes = fedTimes;
  }

  //Takes comma-deliminated list of exactly four times
  //Standard time formatting
  void setFeedingTimes(String timesAsString);

  String getFeedingTimes();

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
