#include "setTimeMenu.h"
#include <stdio.h>
#include "fish.h"
#include "menu.h"
#include "modeMenu.h"
#include "processFeedScheduleFile.h"

#define INCREMENT_HOUR 1
#define DECREMENT_HOUR 2
#define INCREMENT_MINUTE 3
#define DECREMENT_MINUTE 4
#define APPLY 5
#define EXIT 6

int hour = 00;
int minute = 00;


const MenuData setTimeMenuData = {
    .title = "Set Time",
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
    .changeableFG = "cyan",
    .changeableBG = "black",

    .textStartPositionY = 2,
    .textEndPositionY = 7,
    .textPositionX = 0,
    .textSize = 1,

    .numberOfThingsToDisplay = 6,
    .numberOfSelectableOptions = 6,
    .textToDisplay = {"+1 hour", "-1 hour", "+1 minute", "-1 minute", "Apply", "Exit"},
    .selectableOptions ={"+1 hour", "-1 hour", "+1 minute", "-1 minute", "Apply", "Exit"}
};

/**
 * Getter for the setTimeMenuData struct
 * @return setTimeMenuData
 */
MenuData getSetTimeMenuData() {
    return setTimeMenuData;
}

/**
 * Getter for the hour the user set
 * @return hour
 */
int getHour() {
    return hour;
}

/**
 * Getter for the minute the user set
 * @return minute
 */
int getMinute() {
    return minute;
}

/**
 * Resets the time once the user presses apply
 */
void resetTime() {
    hour = 00;
    minute = 00;
}

/**
 * Displays the changeable time the user is setting for the fish feeders clock to be set to
 * @param hour the hour the clock will be set to
 * @param minute the minute the clock will be set to
 */
void displaySetTime(int hour, int minute) {
    // Convert hour and minute to correctly formatted chars
    char time[LINE_SIZE];
    snprintf(time, LINE_SIZE, "%02i-%02i", hour, minute);

    const int timeTextIndex = 0;
    const int timePositionX = calcRightSideTextXPosition(setTimeMenuData.textToDisplay[timeTextIndex]);
    const int timePositionY = calcTextRealYPosition(setTimeMenuData.textStartPositionY);

    displayColour(setTimeMenuData.changeableFG, setTimeMenuData.changeableBG);
    displayText(timePositionX, timePositionY, time, setTimeMenuData.textSize);
}

/**
 * Calls the appropriate function depending on the setTimeMenus current option.
 * @param currentlySelectedOption determines what to execute
 */
void setTimeMenuOptions(int currentlySelectedOption) {
    switch(currentlySelectedOption) {
        case INCREMENT_HOUR:
            hour++;
        if (hour == 24) {
            hour = 0;
        }
        displaySetTime(hour, minute);
        break;

        case DECREMENT_HOUR:
            hour--;
        if (hour == -1) {
            hour = 23;
        }
        displaySetTime(hour, minute);
        break;

        case INCREMENT_MINUTE:
            minute++;
        if (minute == 60) {
            minute = 0;
        }
        displaySetTime(hour, minute);
        break;

        case DECREMENT_MINUTE:
            minute--;
        if (minute == -1) {
            minute = 59;
        }
        displaySetTime(hour, minute);
        break;

        case APPLY:
            // Sets the clock and replaces the stored clock offset in the text file
            clockSet(0, minute, hour, clockDay(), clockMonth(), clockYear());
            replaceStoredClockWarmStart(clockWarmStart(0));

            // Resets the changeable time
            resetTime();
            displaySetTime(hour, minute);

            // Initializes the next feed as the feeders current time has changed
            initializeNextFeed();
            break;

        case EXIT:
            runningMenu(CONFIG_MENU);
        break;
    }
}
