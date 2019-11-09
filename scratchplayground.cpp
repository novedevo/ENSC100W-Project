#include <iostream>
//#include <ESP8266WiFi.h>
using namespace std;

unsigned long timeByteToMillis(unsigned char timeByte, unsigned char networkTimeByte, unsigned long networkMillis){
    
    int hours = timeByte/10;
    int minutes = (timeByte%10)*10;
    
    /*int networkHours = networkTimeByte/10;
    int networkMinutes = (networkTimeByte%10)*10;
    int deltaHours;
    int deltaMinutes;
*/
    //if (networkTimeByte>timeByte){
    //    int bint = 240+timeByte;
    //    hours = bint/10;
    //    minutes = (bint%10)*10;
   // }

    return(hours*60+minutes)*1000;

/*
    cout<<"networkMillis is: " <<networkMillis<<endl;

    unsigned long millisOffset = networkMillis - (networkHours*60+networkMinutes)*60*1000;
    cout<<"millis offset is: "<< millisOffset<<endl;

    deltaHours = hours-networkHours;
    deltaMinutes = minutes-networkMinutes;
    cout<<"delta hours is: "<<deltaHours<<endl<<"delta minutes is: "<<deltaMinutes<<endl;

    unsigned long deltaMillis = (deltaHours*60+deltaMinutes)*60*1000;
    cout<<"delta millis is: "<<deltaMillis<<endl;

    unsigned long newMillis = networkMillis + deltaMillis-millisOffset;
    return (newMillis);*/
}

unsigned char millisToTimeByte(unsigned long timeInMillis, unsigned char networkTimeByte, unsigned long networkMillis){
    
    int hours = timeInMillis/1000/60/60;
    int minutes = (timeInMillis/1000/60)-hours*60;

    cout<<hours;

    return (hours*10+minutes/10);
    /*int networkHours = networkTimeByte/10;
    int networkMinutes = (networkTimeByte%10)*10;

    unsigned long millisOffset = networkMillis - (networkHours*60+networkMinutes)*60*1000;
    cout<<"millis offset is: "<< millisOffset<<endl;

    unsigned long deltaMillis = timeInMillis-(networkMillis-millisOffset);
    cout<<"deltaMillis is: "<< deltaMillis<<endl;

    int deltaHours = deltaMillis/1000/60/60;
    cout<<"deltaHours is: "<<deltaHours<<endl;

    int deltaMinutes = (deltaMillis/1000/60)-deltaHours*60;
    cout<<"deltaMinutes is: "<<deltaMinutes<<endl;

    unsigned char deltaTimeByte = deltaHours*10+deltaMinutes/10;
    unsigned char timeByte = networkTimeByte + deltaTimeByte;

    return (timeByte);*/
}

int enumerateValues(){
    unsigned char c = 0;
    while (++c>0){
        cout<<c<<endl;
    }
}

int main(){
    unsigned char b = 100;
    unsigned char n = 101;
    int daysofnmillis = 4;
    unsigned long l = daysofnmillis*1000*60*60*24;
    cout <<"current time to millis is: "<< timeByteToMillis(b,n, l)<<endl;
    //cout<<endl;
    cout<< (int)millisToTimeByte(timeByteToMillis(b,n, l), n, l);
}
