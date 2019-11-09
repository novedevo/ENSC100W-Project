#include <iostream>
//#include <ESP8266WiFi.h>
using namespace std;

unsigned long timeByteToMillis(unsigned char timeByte){
    if(timeByte>=240){
        throw "invalidTimeStampError";
        return 0;
        }

    int hours = timeByte/10;
    int minutes = (timeByte%10)*10;
    return(hours*60+minutes)*60*1000;
    
}

unsigned char millisToTimeByte(unsigned long timeInMillis){
    int hours = timeInMillis/1000/60/60;
    int minutes = (timeInMillis/1000/60)-hours*60;
    return (hours*10+minutes/10);
}

int enumerateValues(){
    unsigned char c = 0;
    while (++c>0){
        cout<<c<<endl;
    }
}

int main(){
    unsigned char b = 241;
    unsigned char n = 101;
    int daysofnmillis = 4;
    unsigned long l = daysofnmillis*1000*60*60*24;
    //cout <<"current time to millis is: "<< timeByteToMillis(b)<<endl;
    //cout<<endl;
    try{
        cout<< (int)millisToTimeByte(timeByteToMillis(b));
    }
    catch(const char* msg){
        cerr<<msg;
    }
}
