#include <stdio.h>
#include "mainMenu.h"
#include <stdlib.h>
#include "fish.h"
#include "menu.h"

#define CONFIG 1
#define EXIT 2

/**
 * Initialize the mainMenuData struct.
 */
const MenuData mainMenuData = {

    .title = "ArduFEED",
    .titleFG = "orange",
    .titleBG = "black",
    .titleSize = 2,
    .titlePositionY = 0,
    .titlePositionX = 0,

    .textToSelectPositionY = 6,
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
    .numberOfSelectableOptions = 2,
    .textToDisplay = {"Time:", "Next feed:", "Feeds since:", "Mode:", "Config", "Exit"},
    .selectableOptions = {"Config", "Exit"}
};

/**
 * Getter for the mainMenuData struct.
 * @return mainMenuData
 */
MenuData getMainMenuData() {
    return mainMenuData;
}

/**
 * Calls the appropriate function depending on the main menus current option.
 * @param currentlySelectedOption determines what to execute
 */
void mainMenuOptions(int currentlySelectedOption) {
    switch (currentlySelectedOption) {
        case CONFIG:
            runningMenu(CONFIG_MENU);
            break;
        case EXIT:
            displayClear();
            exit(EXIT_SUCCESS);
    }
}

/**
 * Displays the live clock time on the main menu
 */
void displayMainMenuTime() {
    const int timeIndex = 0;
    int second = clockSecond();
    int previousSecond = 0;
    if (second != previousSecond) {
        previousSecond = second;
        // Display the time value
        char time[LINE_SIZE];
        snprintf(time, LINE_SIZE, "%02i-%02i-%02i", clockHour(), clockMinute(), second);
        // Calculate text position
        const int timePositionX = calcRightSideTextXPosition(mainMenuData.textToDisplay[timeIndex]);
        const int timePositionY = calcTextRealYPosition(2);

        displayColour(mainMenuData.changeableFG, mainMenuData.changeableBG);
        displayText(timePositionX, timePositionY, time, mainMenuData.textSize);
    }
}

/**
 * Displays the text on the main menu that changes based on the actions of the user or
 * the fish feeder.
 * @param feedsSinceAutoStart the number of feeds that have occurred since the feeder has been in auto mode
 * @param currentMode the current mode of the fish feeder
 * @param hours the next feed times hours
 * @param minutes the next feed times minutes
 */
void displayMainMenuChangeableData(const int feedsSinceAutoStart, char* currentMode, const int hours, const int minutes) {
    // Convert integers to strings
    char feedsSince[LINE_SIZE];
    snprintf(feedsSince, LINE_SIZE, "%d", feedsSinceAutoStart);
    char feedTime[LINE_SIZE];
    snprintf(feedTime, LINE_SIZE, "%02i-%02i", hours, minutes);

    displayColour(mainMenuData.changeableFG, mainMenuData.changeableBG);

    // I tried being creative with a for loop and switch statement here to reduce the amount of code, but I think
    // it just made it harder to follow som I'm going to avoid such things.
    for (int i = 1; i <= 3; i++) { // Indexes of feedTime, feedsSince and currentMode text
        int positionX = calcRightSideTextXPosition(mainMenuData.textToDisplay[i]);
        int positionY = calcTextRealYPosition(i+2);
        switch (i) {
            default:
                displayText(positionX, positionY, feedTime, mainMenuData.textSize);
                break;
            case 2:
                displayText(positionX, positionY, feedsSince, mainMenuData.textSize);
                break;
            case 3:
                displayText(positionX, positionY, currentMode, mainMenuData.textSize);
                break;
        }
    }
}


