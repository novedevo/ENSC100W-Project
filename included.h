#include <ESP8266WiFi.h>

void swap (byte *a, byte *b)  {  
    byte temp = *a;  
    *a = *b;  
    *b = temp;  
}


//borrowed directly from the internet
void bubbleSort (byte arr[], int n) {   
    int i, j;  
    for (i = 0; i < n-1; i++)
    for (j = 0; j < n-i-1; j++)  
        if (arr[j] > arr[j+1])  
            swap (&arr[j], &arr[j+1]);  
}


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
