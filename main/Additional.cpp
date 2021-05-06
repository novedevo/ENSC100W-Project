#include "Additional.h"

void Time::setFeedingTimes(String timesAsString) {
    const char *str = timesAsString.c_str(); // must be const, otherwise incorrect conversion
    char *p = strtok((char *)str, ","); // casts above to a char, then seperates it at commas

    for (byte i = 0; i < 4; i++) { // reset arrays
        feedingTimes[i] = 0;
        fedTimes[i] = 0;
    }
    byte index = 0;

    while (p != nullptr && index < 4) { // while still in our array
        feedingTimes[index++] = atoi(p); // atoi converts from char array to integer
        p = strtok(NULL, ",");           // seperates at character
    }
}

String Time::getFeedingTimes() {
    String returned = "";
    for (byte i = 0; i < 3; i++) {
        returned += feedingTimes[i];
        returned += ",";
    }
    returned += feedingTimes[3];

    return returned;
}

void Time::sortFeedingTimes() {
    // bubbleSort
    byte i, j;
    for (i = 0; i < 4 - 1; i++)
        for (j = 0; j < 4 - i - 1; j++)
            if (feedingTimes[j] > feedingTimes[j + 1]) {
                std::swap(feedingTimes[j], feedingTimes[j + 1]);
                std::swap(fedTimes[j], fedTimes[j + 1]);
            }
}

void Time::printFeedingTimes() {
    for (byte i = 0; i < 4; i++) {
        Serial.println(feedingTimes[i]);
    }
}

void Time::resetFedTimes() {
    for (byte i = 0; i < 4; i++) {
        fedTimes[i] = false;
    }
}

void Time::prepFeedingTimes(byte currentTime) {
    sortFeedingTimes();
      for (byte i = 0; i < 4; i++) {
        if (feedingTimes[i] < currentTime) { // if we have already passed this feeding time
            fedTimes[i] = true; // denote that the pet has been fed
            Serial.print(feedingTimes[i]);
            Serial.println(" is earlier than current time, thus pet has been fed");
        }
    }
}

bool Time::itIsFeedingTime(byte currentTime) {
    byte index = 5;
    for (byte i = 0; i < 4; i++) { // using byte to save space, should only ever
                                  // iterate through 4 values
        if (!(fedTimes[i]) && (feedingTimes[i])) {
            index = i;
            break;
        }
    }

    if (index >= 4) { // if, for all times, either the cat has been fed or the time = 0
        Serial.println("No feeding times until tomorrow.");
        return false;
    }

    Serial.print("Next feeding time is: ");
    Serial.println(feedingTimes[index]);

    if (feedingTimes[index] <= currentTime) { // if the next feeding time is <= the current time
        fedTimes[index] = true; // denote that the cat has been fed
        return true;
    } else {
        return false;
    }
}
