#include "BlinkityBlink.h"

void BlinkityBlink:: resetMidnightMillis(){
  millisAsOfMostRecentMidnight = millis();
}

void BlinkityBlink:: updateTime(){
  currentTimeAsByte = hour()*10+minute()/10;
}


byte BlinkityBlink:: blynkTimeAsByte(){
  updateTime();
  return currentTimeAsByte;
}

byte BlinkityBlink:: fallbackTimeAsByte(){  //!#############
  return 0; //TODO: IMPLEMENT
}

byte BlinkityBlink:: idealCaseTimeAsByte(){           //!#################
  if(Blynk.connected()){
    return blynkTimeAsByte();
  } else {
    return fallbackTimeAsByte();
  }
}

void BlinkityBlink::nightlyReset(){
  resetMidnightMillis();
}
