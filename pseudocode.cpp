#include<iostream>
//#include<algorithm>
//#include <ESP8266WiFi.h>
using namespace std;

//example times 
//12:00am = 000
//1:00am = 010
//4:30am = 043
//12:00pm = 120
//8:20pm = 202
//1550 hours = 155
//00:00 = 000

void setup(){
    WiFiServer server(80);
    unsigned char feedingTimes[4] = {0,1,250,12};
    bool timesFed[4] = {0,0,0,0};
    auto currentTime = getCurrentTime();
}

int toUnsignedChar(void){
    pass;   //placeholder
}

auto getCurrentTime(){
    return process(get(nist.website:time));
}

//#include <string.h>

// Swaps a with b
void swap(unsigned char *a, unsigned char *b)  {  
    unsigned char temp = *a;  
    *a = *b;  
    *b = temp;  
}  

void bubbleSort(unsigned char arr[], int n) {   //borrowed directly from the internet  
    int i, j;  
    for (i = 0; i < n-1; i++)
    for (j = 0; j < n-i-1; j++)  
        if (arr[j] > arr[j+1])  
            swap(&arr[j], &arr[j+1]);  
}  


void placeholder(){

    bubbleSort(feedingTimes, sizeof(feedingTimes)/sizeof(feedingTimes[0]));

    if (!timesFed[i] && feedingTimes[i]>=toUnsignedChar(currentTime))

    interrupt( (postRequest)){
        if (numberOfTurns){
            this->numberOfTurns = numberOfTurns;
        }
        if (feedNow){
            stepper.turn(numberOfTurns);
        }
        if (newFeedTime){
            
            feedingTimes[i] = newFeedTime;
        }

    }
}

int main(){
    placeholder();
}