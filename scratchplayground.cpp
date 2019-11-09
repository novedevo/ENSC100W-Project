#include <iostream>
//#include <ESP8266WiFi.h>
using namespace std;

unsigned long timeByteToMillis(unsigned char b, unsigned char n, unsigned long networkMillis){
    
    int hours = b/10;
    int minutes = (b%10)*10;
    int networkHours = n/10;
    int networkMinutes = (n%10)*10;
    int deltaHours;
    int deltaMinutes;

    if (n>b){
        int bint = 240+b;
        hours = bint/10;
        minutes = (bint%10)*10;

    }

    deltaHours = hours-networkHours;
    deltaMinutes = minutes-networkMinutes;

    cout<<deltaHours<<endl<<deltaMinutes<<endl;

    int deltaMillis = (deltaHours*60+deltaMinutes)*60*1000;
    cout<<deltaMillis<<endl;
    int newMillis = networkMillis + deltaMillis;
}

int enumerateValues(){
    unsigned char c = 0;
    while (++c>0){
        cout<<c<<endl;
    }
}

int main(){
    unsigned char b = 130;
    unsigned char n = 140;
    unsigned long l = 120312;
    cout << timeByteToMillis(b,n, l);
}
