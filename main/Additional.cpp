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
    return getEstimatedTime(*networkMillis, networkTimeByte);
  }
  else{
    return getCurrentNetworkTimeByte(networkMillis, timeClient);
  }
}

void Time::prepFeedingTimes(byte currentTime, byte (&feedingTimes)[4], bool (&fedTimes)[4]){
    
    DataManip datamanip;
    datamanip.bubbleSort(feedingTimes, 4);
    for (int i = 0; i<4; i++){
      if (feedingTimes[i]<currentTime){
        fedTimes[i] = true;
        Serial.print(feedingTimes[i]);
        Serial.println(" is earlier than current time, thus pet has been fed");
      }
      else continue;
    }
    
}

void DataManip::swap (byte *a, byte *b)  {
  byte temp = *a;
  *a = *b;
  *b = temp;
}

void DataManip::bubbleSort (byte arr[], int n) {
  int i, j;
  for (i = 0; i < n - 1; i++)
    for (j = 0; j < n - i - 1; j++)
      if (arr[j] > arr[j + 1])
        swap (&arr[j], &arr[j + 1]);
}

bool Time::itIsFeedingTime(byte currentTime, byte feedingTimes[4], bool fedTimes[4]){
  byte index = 5;
  for (byte i = 0; i<4; i++){
    if(!(fedTimes[i])&&(feedingTimes[i])){
      index = i;
      break;
      
    }
  }
  Serial.print("Next feeding time is: ");
  Serial.println(feedingTimes[index]);
  if (index<4 && feedingTimes[index] <= currentTime){
    return true;
  }
  else return false;
  
  }
