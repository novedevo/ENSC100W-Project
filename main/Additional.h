#include <ESP8266WiFi.h>

#ifndef ADDITIONAL_H
#define ADDITIONAL_H

class Time {
private:
    // for internal use only
    byte feedingTimes[4];
    bool fedTimes[4];

public:
    // constructor, takes an array and populates feedingTimes with it
    Time(const byte (&feedingTimes)[4]) {
        for (int i = 0; i < 4; i++) { // cannot assign arrays to one another directly, must use loop
            this->feedingTimes[i] = feedingTimes[i];
            this->fedTimes[i] = 0;
        }
    }

    // Takes comma-deliminated list of exactly four times
    // Standard time formatting
    void setFeedingTimes(String timesAsString);

    // reverses above function
    String getFeedingTimes();

    // checks if it is feeding time,
    // moves on to next feeding time if so
    bool itIsFeedingTime(byte currentTime);

    // sorts, sets any times earlier than the current time
    // to "pet has been fed"
    void prepFeedingTimes(byte currentTime);

    // iterates through feedingTimes and prints each value to Serial
    // for debugging
    void printFeedingTimes();

    // bubblesorts them
    void sortFeedingTimes();

    // sets all fedTimes to false, for use at midnight
    void resetFedTimes();
};

#endif
