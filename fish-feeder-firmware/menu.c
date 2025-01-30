#include "menu.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configMenu.h"
#include "fish.h"
#include "mainMenu.h"
#include "modeMenu.h"
#include "processFeedScheduleFile.h"
#include "scheduleFeedMenu.h"
#include "setFeedRotationsMenu.h"
#include "setFeedTimeMenu.h"
#include "setTimeMenu.h"

/**
 * Initial setup for the fish feeder - sets food, time and initializes the next feed time
 */
void initialSetup() {
    logAdd(METHOD_ENTRY, "initialSetup()");
    infoMessage("Welcome to the fish feeder test program");

    // If the text file exists, calls clockWarmStart with the value that was previously created by it
    if (doesFileExist("feedSchedule.txt")) {
        clockWarmStart(getClockWarmStartFromFile());
    }
    else {
        // Set the real time clock to 14:00:00 on the 25th May 1968 if there's no previous time to get
        clockSet(0, 0, 14, 25, 05, 1968);
    }

    foodFill(50); // fill the food container Note: maximum 50%

    initializeNextFeed();
}

/**
 * Displays the menu items on the GUI based on the provided MenuData struct.
 * @param menu the struct containing all the menus data
 */
void displayMenu(MenuData menu) {

    // Display menu title
    displayColour(menu.titleFG, menu.titleBG);
    displayClear();
    displayText(menu.titlePositionX, menu.titlePositionY, menu.title, menu.titleSize);

    // Display left side menu text
    for (int currentLeftTextIndex = 0; currentLeftTextIndex < menu.numberOfThingsToDisplay; currentLeftTextIndex++) {
        int textPositionY = currentLeftTextIndex + menu.textStartPositionY;

        // Have the currently selected option be a different colour
        if (textPositionY == menu.textToSelectPositionY) {
            displayColour(menu.selectedFG, menu.selectedBG);
        }
        // Everything else is the default colour
        else {
            displayColour(menu.notSelectedFG, menu.notSelectedBG);
        }

        displayText(menu.textPositionX, calcTextRealYPosition(textPositionY),
                    menu.textToDisplay[currentLeftTextIndex], menu.textSize);
    }
}

/**
 * Display the appropriate menu based on the passed menu type.
 * @param menuType which menu to display
 */
void chooseMenuToDisplay(int menuType) {
    switch (menuType) {
        // Default executes if something goes seriously wrong
        default:
            printf("chooseMenuToDisplay - Invalid menu type passed");
            exit(EXIT_FAILURE);

        case MAIN_MENU:
            displayMenu(getMainMenuData());
            displayMainMenuChangeableData(getFeedsSinceAutomaticSchedule(), getFeederOperatingMode(),
                                          getNextFeedHours(), getNextFeedMinutes());
            break;

        case CONFIG_MENU:
            displayMenu(getConfigMenuData());
            break;

        case MODE_MENU:
            displayMenu(getModeMenuData());
            break;

        case SCHEDULE_FEED_MENU:
            displayMenu(getScheduleFeedMenuData());
            displaySetFeedScheduleChangeableData(getFeedHour(), getFeedMinute(), getFeedRotation());
            break;

        case SCHEDULE_FEED_TIME_MENU:
            displayMenu(getFeedTimeMenuData());
            displayFeedTime(getFeedHour(), getFeedMinute());
            break;

        case SCHEDULE_FEED_ROTATIONS_MENU:
            displayMenu(getFeedRotationsMenuData());
            displayFeedRotations(getFeedRotation());
            break;

        case SET_TIME_MENU:
            displayMenu(getSetTimeMenuData());
            displaySetTime(getHour(), getMinute());
            break;
    }
}

/**
 * Calls the navigateMenu function with the appropriate params depending on the menu type.
 * @param menuType which menu we are calling navigateMenu for
 * @param currentlySelectedOption the currently selected option, which determines the result of navigateMenu
 * @return currentlySelectedOption so that we can keep track of it after its updated
 */
int menuShortPress(int menuType, int currentlySelectedOption) {
    switch (menuType) {
        // Default executes if something goes seriously wrong
        default:
            printf("menuShortPress - Invalid menu type passed");
            exit(EXIT_FAILURE);

        case MAIN_MENU:
            currentlySelectedOption = navigateMenu(getMainMenuData(),currentlySelectedOption);
            break;

        case CONFIG_MENU:
            currentlySelectedOption = navigateMenu(getConfigMenuData(), currentlySelectedOption);
            break;

        case MODE_MENU:
            currentlySelectedOption = navigateMenu(getModeMenuData(), currentlySelectedOption);
            break;

        case SCHEDULE_FEED_MENU:
            currentlySelectedOption = navigateMenu(getScheduleFeedMenuData(), currentlySelectedOption);
            break;

        case SCHEDULE_FEED_TIME_MENU:
            currentlySelectedOption = navigateMenu(getFeedTimeMenuData(), currentlySelectedOption);
            break;

        case SCHEDULE_FEED_ROTATIONS_MENU:
            currentlySelectedOption = navigateMenu(getFeedRotationsMenuData(), currentlySelectedOption);
            break;

        case SET_TIME_MENU:
            currentlySelectedOption = navigateMenu(getSetTimeMenuData(), currentlySelectedOption);
            break;
    }
    return currentlySelectedOption;
}

/**
 * Calls the appropriate function for the current menus currently selected option.
 * @param menuType which menus function we are calling
 * @param currentlySelectedOption this decides which function to call
 */
void menuLongPress(int menuType, int currentlySelectedOption) {
    switch (menuType) {
        // Default executes if something goes seriously wrong
        default:
            printf("menuLongPress - Invalid menu type passed");
            exit(EXIT_FAILURE);

        case MAIN_MENU:
            mainMenuOptions(currentlySelectedOption);
            break;

        case CONFIG_MENU:
            configMenuOptions(currentlySelectedOption);
            break;

        case MODE_MENU:
            modeMenuOptions(currentlySelectedOption);
            break;

        case SCHEDULE_FEED_MENU:
            scheduleFeedMenuOptions(currentlySelectedOption);
            break;

        case SCHEDULE_FEED_TIME_MENU:
            setFeedTimeMenuOptions(currentlySelectedOption);
            break;

        case SCHEDULE_FEED_ROTATIONS_MENU:
            setFeedRotationsMenuOptions(currentlySelectedOption);
            break;

        case SET_TIME_MENU:
            setTimeMenuOptions(currentlySelectedOption);
            break;
    }
}

/**
 * The function that is called when the fish feeder is first started, monitors button input and calls the
 * appropriate functions depending on how its pressed.
 * @param menuType the type of menu that is currently 'running'
 */
void runningMenu(int menuType) {

    //Clear the screen
    displayClear();

    //Display the appropriate menu
    chooseMenuToDisplay(menuType);

    // Initialise currentlySelectedOption to 1
    int currentlySelectedOption = 1;
    int runningProgram = 1;

    // Number of quarter seconds when the button hasn't been pressed
    int quarterSecondsSinceNoPress = 0;

    // How many quarter seconds should pass before the display is blanked
    const int blankDisplayAfterQuarterSeconds = 240; // Seconds = half this amount.

    // Tracks whether the screen is currently blanked
    int screenBlanked = 0;

    // A menu should always be running unless the program is stopped
    while (runningProgram) {

        // Displays live main menu information
        if (menuType == MAIN_MENU) {
            displayMainMenuTime();
        }

        // Check if the feeder is in auto or skip next mode
        if (strcmp(getFeederOperatingMode(), "Auto") == 0 || strcmp(getFeederOperatingMode(), "Skipping next") == 0) {
            // Check if its feeding time
            if (clockHour() == getNextFeedHours() && clockMinute() == getNextFeedMinutes()) {
                commenceTheAutomaticFeed(); // Feed the fish
                displayClear(); // Reset display in case it needs to show updated data e.g., the next feed time
                chooseMenuToDisplay(menuType); // Display the correct menu
            }
        }

        // Check for the button state every quarter second
        msleep(250L);
        char *result = buttonState(); // get the button state from the JavaFX application

        // Do what the currently selected option does
        if (isButtonLongPressed(result)) {
            quarterSecondsSinceNoPress = 0;
            menuLongPress(menuType, currentlySelectedOption);
        }

        // Change the currently selected option after a short press
        if (isButtonShortPressed(result)) {
            quarterSecondsSinceNoPress = 0;
            currentlySelectedOption = menuShortPress(menuType, currentlySelectedOption);
        }

        // Keep track of the number of quarter seconds the button has not been pressed
        if (isButtonNotPressed(result)) {
            quarterSecondsSinceNoPress ++;
            // Blank the screen after a period of inactivity
            if (quarterSecondsSinceNoPress == blankDisplayAfterQuarterSeconds) {
                displayClear();
                screenBlanked = 1;
                free(result);
                while(screenBlanked) {
                    // Monitor button state while screen is blanked
                    msleep(250L);
                    char *blankResult = buttonState();
                    // Turn the screen back on if the button is pressed at all
                    if (!isButtonNotPressed(blankResult)) {
                        quarterSecondsSinceNoPress = 0;
                        chooseMenuToDisplay(menuType);
                        currentlySelectedOption = 1; // Resets the current option back to the initially selected one
                        screenBlanked = 0;
                    }
                }
            }
        }
    }
}

/**
 * Returns the true if the button state matches
 * @param buttonState the button
 * @return whether the button is in this state
 */
bool isButtonLongPressed(const char *buttonState) {
    return strcmp(buttonState, "LONG_PRESS") == 0;
}

/**
 * Returns the true if the button state matches
 * @param buttonState the button
 * @return whether the button is in this state
 */
bool isButtonShortPressed(const char *buttonState) {
    return strcmp(buttonState, "SHORT_PRESS") == 0;
}

/**
 * Returns the true if the button state matches
 * @param buttonState the button
 * @return whether the button is in this state
 */
bool isButtonNotPressed(const char *buttonState) {
    return strcmp(buttonState, "NO_PRESS") == 0;
}

/**
 * Calculates the integer to position text slightly to the right
 * of some other text.
 * @param leftSideText the text we want to put the new text next to
 * @return an int specifying what pixels to display the texts X positon oin
 */
int calcRightSideTextXPosition(const char* leftSideText) {
    return CHAR_WIDTH*(strlen(leftSideText)+1);
}

/**
 * Calculates the integer to correctly position text on the Y axis
 * @param positionY the 'Y position'
 * @return the exact pixels to place the text
 */
int calcTextRealYPosition(const int positionY) {
    return CHAR_HEIGHT * positionY;
}

/**
 * Updates the currently selected option and highlights it on the GUI.
 * @param menu the struct containing all the menus data
 * @param currentlySelectedOption which option is currently selected on the menu
 * @return currentlySelectedOption so that we can keep track of it after its updated
 */
int navigateMenu(MenuData menu, int currentlySelectedOption) {

    // If last option is currently selected, wrap around to the first option
    if (currentlySelectedOption == menu.numberOfSelectableOptions) {
        
        // The index's of the string text to de-select and select
        const int firstOption = 0;
        const int lastOption = menu.numberOfSelectableOptions - 1;

        // De-select last text
        displayColour(menu.notSelectedFG, menu.notSelectedBG);
        displayText(menu.textPositionX, calcTextRealYPosition(menu.textEndPositionY),
                    menu.selectableOptions[lastOption], menu.textSize);

        // Select the first text to be selected
        displayColour(menu.selectedFG, menu.selectedBG);
        displayText(menu.textPositionX, calcTextRealYPosition(menu.textToSelectPositionY),
                    menu.selectableOptions[firstOption], menu.textSize);
    }

    // De-select current option and select the next option
    else {
        // The index's of the string text to de-select and select
        const int toDeSelect = currentlySelectedOption - 1;
        const int toSelect = currentlySelectedOption;

        // The Y position of the text to be de-selected and selected
        // Takes the index of the string text and adds the Y position of the initial text to be selected - just works...
        const int toDeSelectYPosition = toDeSelect + menu.textToSelectPositionY;
        const int toSelectYPosition = toSelect + menu.textToSelectPositionY;

        // De-select text
        displayColour(menu.notSelectedFG, menu.notSelectedBG);
        displayText(menu.textPositionX, calcTextRealYPosition(toDeSelectYPosition),
                    menu.selectableOptions[toDeSelect], menu.textSize);

        // Select new text
        displayColour(menu.selectedFG, menu.selectedBG);
        displayText(menu.textPositionX, calcTextRealYPosition(toSelectYPosition),
                    menu.selectableOptions[toSelect], menu.textSize);
    }

    currentlySelectedOption++;

    // Set current option back to 1 when it goes out of bounds
    if (currentlySelectedOption > menu.numberOfSelectableOptions) {
        currentlySelectedOption = 1;
    }

    return currentlySelectedOption;
}
