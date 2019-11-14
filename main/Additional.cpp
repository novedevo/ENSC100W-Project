#include "Additional.h"
/*
unsigned long Time::timeByteToMillis(byte timeByte){  //TODO: rewrite? understand?
    int hours = timeByte / 10;
    int minutes = (timeByte % 10) * 10;
  
    if (hours >= 24 || minutes >= 60) {   //impossible times in standard 24-hours time
      return 0;
      //throw "invalidTimeByte";  //commented out because arduino doesn't like exceptions?
      //uncomment if I figure out a way to get that working?
    }
  
    unsigned long timeInMillis = (hours * 60 + minutes) * 60 * 1000;
    return timeInMillis;
}*/

void Time::setFeedingTimes(String timesAsString){
      const char *str = timesAsString.c_str();  //must be const, otherwise incorrect conversion
      char* p = strtok((char*)str, ",");  //casts above to a char, then seperates it at comma
      
      for(int i = 0; i<4; i++){ //reset arrays
        feedingTimes[i] = 0;
        fedTimes[i] = 0;
      }
      int index = 0;  //TODO: why is this necessary
    
      while (p != nullptr && index < 4) { //while still in our array
        feedingTimes[index++] = atoi(p);  //atoi converts from char array to integer
        p = strtok(NULL, ",");  //seperates at character
        //TODO: understand strtok and atoi a bit better
      }
  }

String Time::getFeedingTimes(){
  String returned = "";
  for (int i = 0; i<3; i++){
    returned += feedingTimes[i];
    returned += ",";
  }
  returned += feedingTimes[3];
  
  return returned;
}

void Time::sortFeedingTimes(){
    //implementation of bubbleSort
    int i, j;
    for (i = 0; i < 4 - 1; i++)
      for (j = 0; j < 4 - i - 1; j++)
        if (feedingTimes[j] > feedingTimes[j + 1]){
          std::swap (feedingTimes[j], feedingTimes[j + 1]);
          std::swap (fedTimes[j], fedTimes[j + 1]);
        }
}

void Time::printFeedingTimes(){
  for (int i = 0; i < 4; i++){
    Serial.println(feedingTimes[i]);
  }
}
/*
byte Time::millisToTimeByte(unsigned long timeInMillis) { //TODO: rewrite? understand?
  int hours = timeInMillis / 1000 / 60 / 60;
  int minutes = (timeInMillis / 1000 / 60 ) - hours * 60;

  if (minutes>=60){
    return 0;
    //throw "invalidMillisTime"
  }
  
  if (hours >= 24) {   //impossible times in standard 24-hours time
    hours = hours%24;
    //throw "invalidMillisTime";  //arduino doesn't like exceptions
  }
  
  byte timeByte = hours * 10 + minutes / 10;
  return timeByte;
}

//TODO: rewrite? understand?
byte Time::getCurrentNetworkTimeByte(unsigned long* networkMillis, NTPClient* timeClient) {
    Serial.println("Time Client updating");
    timeClient->update();   //who knows why this is necessary, oh well
    Serial.println("Time client updated");
    *networkMillis = millis();  //updates the last time that the network was used

    Serial.println("Getting time...");
    int hours = timeClient->getHours();
    int minutes = timeClient->getMinutes();
    Serial.println("Got times.");

    byte timeByte = hours * 10 + minutes / 10;

    if (hours >= 24 || minutes >= 60){
      Serial.println("Got Invalid Time");
      Serial.println(timeByte);
      return (byte)250;
    }
    else return timeByte;
}

//TODO: understand? rewrite? obviate?
//TODO: unsigned? why does that even work
//why does vscode say unsigned is defined as signed tftftf
unsigned long Time::millisAtMidnight(unsigned long networkMillis, unsigned char networkTimeByte) {
  unsigned long millisAtMidnight = networkMillis - timeByteToMillis(networkTimeByte);
  return millisAtMidnight;
}

//TODO: bring networkmillis, networkTimeByte into Time or remove them altogether
byte Time::getEstimatedTime(unsigned long networkMillis, byte networkTimeByte){
  return millisToTimeByte(millis() - millisAtMidnight(networkMillis, networkTimeByte));
}

//TODO: fix why why why
byte Time::improvedGetTimeByte(unsigned long* networkMillis, NTPClient* timeClient, byte networkTimeByte){
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("No WiFi connection, falling back to millis()");
    return getEstimatedTime(*networkMillis, networkTimeByte);
  }
  else{
    return getCurrentNetworkTimeByte(networkMillis, timeClient);
  }
}
*/
void Time::resetFedTimes(){
  for (int i = 0; i<4; i++){
    fedTimes[i] = false;
  }
}

void Time::prepFeedingTimes(byte currentTime){
  Serial.println(currentTime);
  sortFeedingTimes();
  printFeedingTimes();
  for (int i = 0; i<4; i++){
    //Serial.println(i);
    if (feedingTimes[i]<currentTime){   //if we have already passed this feeding time
      fedTimes[i] = true;               //denote that the pet has been fed
      Serial.print(feedingTimes[i]);  
      Serial.println(" is earlier than current time, thus pet has been fed");
    } else {
      //int index = 5;  //TODO: why is this here
      continue;
    }
  }
}

bool Time::itIsFeedingTime(byte currentTime){
  index = 5;  //TODO: why is this here, why does index need to be Time-scoped
  for (byte i = 0; i < 4; i++){   //using byte to save space, should only ever iterate through 4 values
    if(!(fedTimes[i])&&(feedingTimes[i])){
      index = i;
      break;
    }
  }

  if (index >= 4){  //if, for all times, either the cat has been fed or the time = 0
    Serial.println("No feeding times until tomorrow.");
    return false;
  }

  Serial.print("Next feeding time is: ");
  Serial.println(feedingTimes[index]);

  if (feedingTimes[index] <= currentTime){  //if the next feeding time is <= the current time
    fedTimes[index] = true;   //denote that the cat has been fed
    return true;
  } else {
    return false;
  }
}
