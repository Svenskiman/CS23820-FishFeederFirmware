#include <stdlib.h>
#include <string.h>
#include "modeMenu.h"
#include <stdio.h>
#include "fish.h"
#include "menu.h"
#include "processFeedScheduleFile.h"

#define AUTO 1
#define FEED_NOW 2
#define PAUSE 3
#define SKIP_NEXT 4
#define EXIT 5

char* currentFeederOperatingMode = "Auto";
int feedsSinceAutomaticSchedule = 0;

// Stores the size and contents of the array containing the feed schedule
// so that we don't have to calculate them again unless we should.
int feedDataSize;
int* feedData;

int nextFeedIndex = 0;
int nextFeedHours;
int nextFeedMinutes;
int nextFeedRotations;

/**
 * Initialize the modeMenuData struct.
 */
const MenuData modeMenuData = {
    .title = "ModeMenu",
    .titleFG = "orange",
    .titleBG = "black",
    .titleSize = 2,
    .titlePositionY = 0,
    .titlePositionX = 0,

    .textToSelectPositionY = 2,
    .textToSelectPositionX = 0,

    .selectedFG = "black",
    .selectedBG = "white",
    .notSelectedFG = "white",
    .notSelectedBG = "black",

    .textStartPositionY = 2,
    .textEndPositionY = 6,
    .textPositionX = 0,
    .textSize = 1,

    .numberOfThingsToDisplay = 5,
    .numberOfSelectableOptions = 5,

    .textToDisplay = {"Auto", "Feed now", "Pause", "Skip next", "Exit"},
    .selectableOptions ={"Auto", "Feed now", "Pause", "Skip next", "Exit"},
};

/**
 * Getter for the modeMenuData struct.
 * @return modeMenuData
 */
MenuData getModeMenuData() {
    return modeMenuData;
}

/**
 * Getter for the current feeder operating mode
 * @return currentFeederOperatingMode
 */
char* getFeederOperatingMode() {
    return currentFeederOperatingMode;
}

/**
 * Getter for the number of feeds since the feeder has been in automatic mode
 * @return feedsSinceAutomaticSchedule
 */
int getFeedsSinceAutomaticSchedule() {
    return feedsSinceAutomaticSchedule;
}

/**
 * Gets the integer representing the next feed time and rotation number from
 * the array containing feed schedule data
 * @param index the index of the data we want
 * @param data the array containing the data
 * @return the 5-digit number
 */
int getNextFeedData(int index, int* data) {
    return data[index];
}


/**
 * Sets nextFeedHours, nextFeedMinutes and nextFeedRotations
 * based on the contents of the 5-digit data number
 * @param data the 5-digit number containing the next feed information
 */
void setFeedTimeAndRotations(int data) {
    // Separate time and rotations
    int rotations = data % 10; // Rotations is the last digit of the number
    data = data / 10;

    nextFeedRotations = rotations;
    nextFeedHours = data / 100; // Get hours from time
    nextFeedMinutes = data % 100; // Get minutes from time
}

/**
 * Gets the hour of the next feed
 * @return nextFeedHours
 */
int getNextFeedHours() {
    return nextFeedHours;
}

/**
 * Gets the minute of the next feed
 * @return nextFeedMinutes
 */
int getNextFeedMinutes() {
    return nextFeedMinutes;
}

/**
 * Initializes one complete rotation of the fish feeder.
 * If the button is pressed again whilst the feeder is rotating, end the rotation
 * early. Once the feed has ended, the current feed mode is set back to what
 * it was previously.
 */
void modeFeedNow() {
    // Store previous mode and update the current mode
    char* previousMode = currentFeederOperatingMode;
    currentFeederOperatingMode = "Feeding now";

    const int fullRotation = 360;
    // Caps rotation speed to 1 per 20 seconds by sleeping the motor by 56 milliseconds per step
    // (56 * 360) / 1000 = 20.16 seconds
    const int rotationCap = 56;
    for (int i = 0; i < fullRotation; i++) {
        motorStep();
        msleep(rotationCap);
        char* result1 = buttonState(); // get the button state from the JavaFX application
        if (isButtonShortPressed(result1)) {
            i = fullRotation, currentFeederOperatingMode = previousMode;
        }
        free(result1);
    }
    // Revert feeder mode back to the previous one
    currentFeederOperatingMode = previousMode;
}

/**
 * Sets the current feeder mode to paused
 */
void modePause() {
    currentFeederOperatingMode = "Paused";
}

/**
 * Sets the current mode to Skipping next, and sets the next feed to what comes after
 * the currently set next feed.
 */
void modeSkipNext() {
    // Only allows this mode to be enabled if there's at least 2 set feed times in the schedule
    if (feedDataSize < 2) {
        printf("Theres not enough feed data to do this");
        return;
    }
    currentFeederOperatingMode = "Skipping next";
    nextFeedIndex++;

    // If the end of the schedule has been reached, set the index back to 0
    if (nextFeedIndex == feedDataSize) {
        nextFeedIndex = 0;
    }
    setFeedTimeAndRotations(getNextFeedData(nextFeedIndex, feedData));
}

/**
 * Converts the current times hour and minutes into a singular 5 digit
 * integer to match the format of the array containing the feed schedule data
 * @return the currentTime as a 5-digit number
 */
int convertCurrentTime() {
    int hours = clockHour() * 100;
    int minutes = clockMinute();
    // Make the time 5 digits long to account for the rotation numbers digit
    int time = (hours + minutes) * 10;
    return time;
}

/**
 * Determines the index of the next scheduled feeds data depending on the
 * current time.
 * Code taken and then altered from: Approach 2: Using Two Pointers
 * https://www.geeksforgeeks.org/find-closest-number-array/
 * @param size the size of the array
 * @param data the array containing the sorted data
 * @param currentTime the current time formatted into 5 digits
 * @return the next scheduled feeds index
 */
int calcIndexOfTheNextFeed(int size, int* data, int currentTime) {
    bool currentTimeIsBigger = false;

    // Check if the current time is 'larger' than the 'largest' from the file
    if (data[size-1] < currentTime) {
        currentTimeIsBigger = true;
    }

    if (!currentTimeIsBigger) {
        int leftPointer = 0;
        int rightPointer = size - 1;

        // Look for the closest 'larger' time in the array using 2 pointers
        while (leftPointer <= rightPointer) {
            if (abs(data[leftPointer] - currentTime) <= abs(data[rightPointer] - currentTime)
                && abs(data[leftPointer]) > currentTime) // Condition that ensures the time is larger, not smaller
                {
                rightPointer--;
                }
            else {
                leftPointer++;
            }
        }
        return leftPointer;
    }
    // If the current time is the 'largest', returns the index of the 'smallest' time from the file
    // as that would be the next feed time.
    return 0;
}

/**
 * Rotates the feeder for the specified number of rotations for the
 * specified feed time.
 */
void commenceTheAutomaticFeed() {
    const int fullRotation = 360;
    // Ensures the max rotation speed is roughly 1 rotation per 20 seconds
    const int rotationCap = 56;

    // Rotate the feeder for the specified number of rotations
    for (int i = 0; i < nextFeedRotations; i++) {
        // Each rotation is 360 degrees
        for (int j = 0; j < fullRotation; j++) {
            motorStep();
            msleep(rotationCap); // Sleep for 56 milliseconds
        }
    }

    feedsSinceAutomaticSchedule++;
    nextFeedIndex++;

    // Loop schedule back to the beginning after the last feed
    if (nextFeedIndex > feedDataSize - 1) {
        nextFeedIndex = 0;
    }
    // Update the feed time and rotations
    setFeedTimeAndRotations(getNextFeedData(nextFeedIndex, feedData));
    // Change mode back to auto if it was in mode skipping next
    if (strcmp(currentFeederOperatingMode, "Skipping next") == 0) {
        currentFeederOperatingMode = "Auto";
    }
}

/**
 * Initializes the data regarding the next scheduled feed
 */
void initializeNextFeed() {

    // If there is no feed schedule file, set the next feed time to 0000 with 0 rotations.
    if (!doesFileExist("feedSchedule.txt")) {
        feedDataSize = 0;
        setFeedTimeAndRotations(00000);
    }

    else {
        setNumberOfScheduledFeeds(getNumberOfScheduledFeedsFromFile());
        feedDataSize = getNumberOfScheduledFeeds();

        if (feedDataSize == 0) {
            // printf("There is no set feed schedule");
            return;
        }

        // Gets and sorts schedule data
        int* unsortedData = getFeedScheduleData();
        feedData = quickSortFeedTimes(feedDataSize, unsortedData);
        // Uses the sorted data to determine the next feed time
        nextFeedIndex = calcIndexOfTheNextFeed(feedDataSize, feedData, convertCurrentTime());
        const int closestTime = getNextFeedData(nextFeedIndex, feedData);
        setFeedTimeAndRotations(closestTime);
    }
}

/**
 * Calls the initializeNextFeed function if the feeder has been paused.
 * This is because while the feeder has been paused, more schedules could have been created, or the
 * time for what was the next feed could have already elapsed.
 */
void reinitializeNextFeedIfPaused() {
    if (strcmp(currentFeederOperatingMode, "Paused") == 0) {
        initializeNextFeed();
    }
}

/**
 * Calls the appropriate function depending on the mode menus current option.
 * @param currentlySelectedOption determines what to execute
 */
void modeMenuOptions(int currentlySelectedOption) {
    switch (currentlySelectedOption) {
        case AUTO:
            reinitializeNextFeedIfPaused();
            currentFeederOperatingMode = "Auto";
        break;
        case FEED_NOW:
            modeFeedNow();
        break;
        case PAUSE:
            modePause();
        break;
        case SKIP_NEXT:
            reinitializeNextFeedIfPaused();
            modeSkipNext();
        break;
        case EXIT:
            runningMenu(CONFIG_MENU);
        break;
    }
}

