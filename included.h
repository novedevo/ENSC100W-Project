#include <ESP8266WiFi.h>

void swap(byte *a, byte *b)  {  
    byte temp = *a;  
    *a = *b;  
    *b = temp;  
}  

void bubbleSort(byte arr[], int n) {   //borrowed directly from the internet  
    int i, j;  
    for (i = 0; i < n-1; i++)
    for (j = 0; j < n-i-1; j++)  
        if (arr[j] > arr[j+1])  
            swap(&arr[j], &arr[j+1]);  
}  

byte getCurrentTime(){
    return nistToOurTimestamp(get(nist.website:time));
}