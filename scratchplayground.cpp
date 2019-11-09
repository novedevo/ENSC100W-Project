#include <iostream>
//#include <ESP8266WiFi.h>
using namespace std;

//takes standard format time of day as byte, returns time of day in milliseconds since 00:00
unsigned long timeByteToMillis(unsigned char timeByte){
    int hours = timeByte / 10;
    int minutes = (timeByte % 10) * 10;

    if (hours >= 24 || minutes >= 60){    //impossible times in standard 24-hours time
        throw "invalidTimeByte";
    }
    
    unsigned long timeInMillis = (hours * 60 + minutes) * 60 * 1000;
    return timeInMillis;
    
}


//takes time of day in milliseconds since 00:00, returns standard format time of day as a byte
unsigned char millisToTimeByte(unsigned long timeInMillis){ 
    int hours = timeInMillis / 1000 / 60 / 60;
    int minutes = (timeInMillis / 1000 / 60 ) - hours * 60;

    if (hours >= 24 || minutes >= 60){    //impossible times in standard 24-hours time
        throw "invalidMillisTime";
    }

    unsigned char timeByte = hours * 10 + minutes / 10;
    return timeByte;
}

//iterates through all values of a given item, for testing purposes
int iterateThroughValues(void){
    unsigned char c = 0;
    while (++c > 0){
        cout << c << endl;
    }
}

int main(void){
    unsigned char timeByte = 235;
    
    try {
        cout<< static_cast<int>(millisToTimeByte(timeByteToMillis(timeByte))) << endl;
    }
    
    catch(const char* msg){
        cerr << msg << endl;
    }
    
    catch(...){
        cerr << "Unknown Exception" << endl;
    }
}
