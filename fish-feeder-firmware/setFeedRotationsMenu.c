#include "setFeedRotationsMenu.h"
#include <stdio.h>
#include "fish.h"
#include "menu.h"

#define INCREMENT_ROTATIONS 1
#define DECREMENT_ROTATIONS 2
#define EXIT 3

int setFeedRotations = 1;

const MenuData feedRotationsMenuData = {
    .title = "Rotations",
    .titleFG = "orange",
    .titleBG = "black",
    .titleSize = 2,
    .titlePositionY = 0,
    .titlePositionX = 0,

    .textToSelectPositionY = 3,
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
    .numberOfSelectableOptions = 3,
    .textToDisplay = {"Rotations:", "+1 rotation", "-1 rotation", "Exit"},
    .selectableOptions ={"+1 rotation", "-1 rotation", "Exit"}
};

/**
 * Getter for the feedRotationsMenu struct
 * @return feedRotatiosnMenuData
 */
MenuData getFeedRotationsMenuData() {
    return feedRotationsMenuData;
}

/**
 * Getter for the number of feedRotations the user set
 * @return setFeedRotations
 */
int getFeedRotation() {
    return setFeedRotations;
}

/**
 * Resets the feed rotations back to 1 once the user clicks apply
 */
void resetFeedRotations() {
    setFeedRotations = 1;
}

/**
 * Displays the number of feed rotations the feed will have, next to the Rotations: text
 * @param feedRotations the number of rotations the feeder will do
 */
void displayFeedRotations(int feedRotations) {

    // Convert feedRotations from int to char
    char rotations[LINE_SIZE];
    snprintf(rotations, LINE_SIZE, "%d", feedRotations);

    const int rotationsTextIndex = 0;
    const int rotationsPositionX = calcRightSideTextXPosition(feedRotationsMenuData.textToDisplay[rotationsTextIndex]);
    const int rotationsPositionY = calcTextRealYPosition(feedRotationsMenuData.textStartPositionY);

    displayColour(feedRotationsMenuData.changeableFG, feedRotationsMenuData.changeableBG);
    displayText(rotationsPositionX, rotationsPositionY, rotations, feedRotationsMenuData.textSize);
}

/**
 * Calls the appropriate function depending on the menus currently selected option
 * @param currentlySelectedOption
 */
void setFeedRotationsMenuOptions(int currentlySelectedOption) {
    // Feed rotations are capped at 3 to prevent a feed from taking longer
    // than a minute
    const int feedRotationCap = 4;
    switch (currentlySelectedOption) {
        case INCREMENT_ROTATIONS:
            setFeedRotations++;
            if (setFeedRotations == feedRotationCap) {
                setFeedRotations = 3;
            }
            displayFeedRotations(setFeedRotations);
            break;

        case DECREMENT_ROTATIONS:
            setFeedRotations--;
            if (setFeedRotations == 0) {
                setFeedRotations = 1;
            }
            displayFeedRotations(setFeedRotations);
            break;

        case EXIT:
            runningMenu(SCHEDULE_FEED_MENU);
            break;
    }
}

