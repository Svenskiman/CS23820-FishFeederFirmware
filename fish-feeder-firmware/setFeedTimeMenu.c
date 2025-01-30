#include "setFeedTimeMenu.h"
#include <stdlib.h>
#include "stdio.h"
#include "fish.h"
#include "menu.h"

#define INCREMENT_HOUR 1
#define DECREMENT_HOUR 2
#define INCREMENT_10_MINUTES 3
#define INCREMENT_MINUTE 4
#define DECREMENT_MINUTE 5
#define EXIT 6

int setFeedHour = 00;
int setFeedMinute = 00;

const MenuData feedTimeMenuData = {
    .title = "FeedTime",
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
    .textToDisplay = {"+1 hour", "-1 hour", "+10 minutes", "+1 minute", "-1 minute", "Exit"},
    .selectableOptions ={"+1 hour", "-1 hour", "+10 minutes","+1 minute", "-1 minute", "Exit"}
};

/**
 * Getter for the feedTimeMenuData struct
 * @return feedTimeMenuData
 */
MenuData getFeedTimeMenuData() {
    return feedTimeMenuData;
}

/**
 * Gets the feed hour the user set
 * @return setFeedHour
 */
int getFeedHour() {
    return setFeedHour;
}

/**
 * Gets the feed minutes the user set
 * @return setFeedMinute
 */
int getFeedMinute() {
    return setFeedMinute;
}

/**
 * Resets the feed hour and minutes once the user long presses
 * the apply button
 */
void resetFeedTime() {
    setFeedHour = 00;
    setFeedMinute = 00;
}

/**
 * Displays the time that the user is changing and will be setting this feed
 * schedule to, next to the time text.
 * @param feedHour
 * @param feedMinute
 */
void displayFeedTime(int feedHour, int feedMinute) {
    // Convert hour and minute to correctly formatted chars
    char time[LINE_SIZE];
    snprintf(time, LINE_SIZE, "%02i-%02i", feedHour, feedMinute);

    const int timePositionX = calcRightSideTextXPosition(feedTimeMenuData.textToDisplay[0]);
    const int timePositionY = calcTextRealYPosition(feedTimeMenuData.textStartPositionY);

    displayColour(feedTimeMenuData.changeableFG, feedTimeMenuData.changeableBG);
    displayText(timePositionX, timePositionY, time, feedTimeMenuData.textSize);
}

/**
 * Calls the appropriate function depending on the feedTimeMenus current option.
 * @param currentlySelectedOption determines what to execute
 */
void setFeedTimeMenuOptions(int currentlySelectedOption) {
    switch(currentlySelectedOption) {
        case INCREMENT_HOUR:
            setFeedHour++;
            if (setFeedHour == 24) {
                setFeedHour = 0;
            }
            displayFeedTime(setFeedHour, setFeedMinute);
            break;

        case DECREMENT_HOUR:
            setFeedHour--;
            if (setFeedHour == -1) {
                setFeedHour = 23;
            }
            displayFeedTime(setFeedHour, setFeedMinute);
            break;

        case INCREMENT_10_MINUTES:
            setFeedMinute += 10;
            if (setFeedMinute >= 60) {
                setFeedMinute = 0;
            }
            displayFeedTime(setFeedHour, setFeedMinute);
            break;

        case INCREMENT_MINUTE:
            setFeedMinute++;
            if (setFeedMinute == 60) {
                setFeedMinute = 0;
            }
            displayFeedTime(setFeedHour, setFeedMinute);
            break;

        case DECREMENT_MINUTE:
            setFeedMinute--;
            if (setFeedMinute == -1) {
                setFeedMinute = 59;
            }
            displayFeedTime(setFeedHour, setFeedMinute);
            break;

        case EXIT:
            runningMenu(SCHEDULE_FEED_MENU);
            break;
    }
}