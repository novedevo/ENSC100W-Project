#include "Additional.h"

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

void Time::setFeedingTimes(String timesAsString){
      //String pinValue = param.asStr();
      //char *str = new char[pinValue.length() + 1];
      const char *str = timesAsString.c_str();
      //Serial.println(str);  
      char* p = strtok((char*)str, ",");
      
      for(int i = 0; i<4; i++){
        feedingTimes[i] = 0;
        fedTimes[i] = 0;
      }
      int index = 0;
    
      while (p != nullptr && index < 4) {
        feedingTimes[index++] = atoi(p);
        p = strtok(NULL, ",");
      }
  }

void Time::sortFeedingTimes(){
    int i, j;
    for (i = 0; i < 4 - 1; i++)
      for (j = 0; j < 4 - i - 1; j++)
        if (feedingTimes[j] > feedingTimes[j + 1])
          std::swap (feedingTimes[j], feedingTimes[j + 1]);
          std::swap (fedTimes[j], fedTimes[j + 1]);
}

void Time::printFeedingTimes(){
  for (int i = 0; i < 4; i++){
    Serial.println(feedingTimes[i]);
  }
}

byte Time::millisToTimeByte(unsigned long timeInMillis) {
  int hours = timeInMillis / 1000 / 60 / 60;
  int minutes = (timeInMillis / 1000 / 60 ) - hours * 60;

  if (minutes>=60){
    return 0;
  }
  
  if (hours >= 24) {   //impossible times in standard 24-hours time
    hours = hours%24;
    //throw "invalidMillisTime";
  }
  
  byte timeByte = hours * 10 + minutes / 10;
  return timeByte;
}

byte Time::getCurrentNetworkTimeByte(unsigned long* networkMillis, NTPClient* timeClient) {
    timeClient->update();
    *networkMillis = millis();
    int hours = timeClient->getHours();
    int minutes = timeClient->getMinutes();
    byte timeByte = hours*10+minutes/10;
    if (hours>=24 || minutes>= 60){
      return (byte)250;
    }
    else return timeByte;
}

unsigned long Time::millisAtMidnight(unsigned long networkMillis, unsigned char networkTimeByte) {
  unsigned long millisAtMidnight = networkMillis - timeByteToMillis(networkTimeByte);
  return millisAtMidnight;
}

byte Time::getEstimatedTime(unsigned long networkMillis, byte networkTimeByte){
  return millisToTimeByte(millis() - millisAtMidnight(networkMillis, networkTimeByte));
}

byte Time::improvedGetTimeByte(unsigned long* networkMillis, NTPClient* timeClient, byte networkTimeByte){
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("No WiFi connection, falling back to millis()");
    return getEstimatedTime(*networkMillis, networkTimeByte);
  }
  else{
    return getCurrentNetworkTimeByte(networkMillis, timeClient);
  }
}

void Time::resetFedTimes(){
  for (int i = 0; i<4; i++){
    fedTimes[i] = false;
  }
}

void Time::prepFeedingTimes(byte currentTime){
    
    sortFeedingTimes();
    for (int i = 0; i<4; i++){
      if (feedingTimes[i]<currentTime){
        fedTimes[i] = true;
        Serial.print(feedingTimes[i]);
        Serial.println(" is earlier than current time, thus pet has been fed");
      }
      else {
        //int index = 5;
        break;
      }
    }
    
}
void DataManip::bubbleSort (byte arr[], int n) {
  int i, j;
  for (i = 0; i < n - 1; i++)
    for (j = 0; j < n - i - 1; j++)
      if (arr[j] > arr[j + 1])
        std::swap (arr[j], arr[j + 1]);
}

bool Time::itIsFeedingTime(byte currentTime)
{
  index = 5;
  for (byte i = 0; i<4; i++){
    if(!(fedTimes[i])&&(feedingTimes[i])){
      index = i;
      break;
    }
  }
  if (index>4){
    Serial.println("No feeding times until tomorrow.");
    return false;
  }
  Serial.print("Next feeding time is: ");
  Serial.println(feedingTimes[index]);
  if (feedingTimes[index] <= currentTime){
    fedTimes[index] = true;
    return true;
  }
  else {
    return false;
  }
  
  }
