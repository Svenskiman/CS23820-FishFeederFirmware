#include "scheduleFeedMenu.h"
#include <stdio.h>
#include "fish.h"
#include "menu.h"
#include "modeMenu.h"
#include "processFeedScheduleFile.h"
#include "setFeedRotationsMenu.h"
#include "setFeedTimeMenu.h"

#define SET_TIME 1
#define SET_ROTATIONS 2
#define APPLY 3
#define EXIT 4

/**
 * Initialize the scheduleFeedMenuData struct.
 */
const MenuData scheduleFeedMenuData = {
    .title = "Schedule",
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
    .textEndPositionY = 5,
    .textPositionX = 0,

    .textSize = 1,

    .numberOfThingsToDisplay = 4,
    .numberOfSelectableOptions = 4,
    .textToDisplay = {"Set time:", "Set rotations:", "Apply", "Exit"},
    .selectableOptions ={"Set time:", "Set rotations:", "Apply", "Exit"}
};

/**
 * Getter method for the scheduleFeed menus structure
 * @return
 */
MenuData getScheduleFeedMenuData() {
    return scheduleFeedMenuData;
}

/**
 * Displays the feed time and number of rotations the user has set and would be setting
 * by long pressing on the apply button
 * @param feedTimeHour the feed hour
 * @param feedTimeMinute the feed minutes
 * @param feedRotationAmount the number of rotations the feeder will carry out
 */
void displaySetFeedScheduleChangeableData(int feedTimeHour, int feedTimeMinute, int feedRotationAmount) {

    // Convert time and rotation ints to formatted chars
    char feedTime[LINE_SIZE];
    snprintf(feedTime, LINE_SIZE, "%02i-%02i", feedTimeHour, feedTimeMinute);
    char feedRotation[LINE_SIZE];
    snprintf(feedRotation, LINE_SIZE, "%d", feedRotationAmount);

    const int timeTextIndex = 0;
    const int rotationTextIndex = 1;

    // Calc X and Y positions
    const int feedTimePositionX = calcRightSideTextXPosition(scheduleFeedMenuData.textToDisplay[timeTextIndex]);
    const int feedTimePositionY = calcTextRealYPosition(timeTextIndex+scheduleFeedMenuData.textStartPositionY);
    const int feedRotationPositionX = calcRightSideTextXPosition(scheduleFeedMenuData.textToDisplay[rotationTextIndex]);
    const int feedRotationPositionY = calcTextRealYPosition(rotationTextIndex+scheduleFeedMenuData.textStartPositionY);

    displayColour(scheduleFeedMenuData.changeableFG, scheduleFeedMenuData.changeableBG);
    displayText(feedTimePositionX,feedTimePositionY, feedTime, scheduleFeedMenuData.textSize);
    displayText(feedRotationPositionX,feedRotationPositionY, feedRotation, scheduleFeedMenuData.textSize);
}

/**
 * Calls the appropriate function depending on the scheduleFeedMenus current option
 * @param currentlySelectedOption determines what to execute
 */
void scheduleFeedMenuOptions(int currentlySelectedOption) {
    switch (currentlySelectedOption) {
        case SET_TIME:
            runningMenu(SCHEDULE_FEED_TIME_MENU);
            break;

        case SET_ROTATIONS:
            runningMenu(SCHEDULE_FEED_ROTATIONS_MENU);
            break;

        case APPLY:
            // Writes the data to the file
            writeScheduleDataToFile(getFeedHour(), getFeedMinute(), getFeedRotation());
            // Resets the data the user set
            resetFeedTime();
            resetFeedRotations();

            initializeNextFeed();
            // re-calls the display with the reset data
            displaySetFeedScheduleChangeableData(getFeedHour(), getFeedMinute(), getFeedRotation());
            break;

        case EXIT:
            runningMenu(CONFIG_MENU);
            break;
    }
}

