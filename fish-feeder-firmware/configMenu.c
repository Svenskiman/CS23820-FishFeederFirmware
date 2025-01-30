#include "configMenu.h"
#include "menu.h"
#include "modeMenu.h"
#include "processFeedScheduleFile.h"

#define SET_CLOCK 1
#define SCHEDULE_FEED 2
#define SELECT_MODE 3
#define EXIT 4
#define DELETE 5

/**
 * Initialize the configMenuData struct.
 */
const MenuData configMenuData = {
    .title = "ConfigMenu",
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

    .textToDisplay = {"Set clock", "Schedule feed", "Select mode", "Exit", "Delete data file"},
    .selectableOptions ={"Set clock", "Schedule feed", "Select mode", "Exit", "Delete data file"},
};

/**
 * Getter for the configMenuData struct.
 * @return configMenuData
 */
MenuData getConfigMenuData() {
    return configMenuData;
}

/**
 * Calls the appropriate function depending on the config menus current option.
 * @param currentlySelectedOption determines what to execute
 */
void configMenuOptions(int currentlySelectedOption) {
    switch (currentlySelectedOption) {
        case SET_CLOCK:
            runningMenu(SET_TIME_MENU);
            break;
        case SCHEDULE_FEED:
            runningMenu(SCHEDULE_FEED_MENU);
            break;
        case SELECT_MODE:
            runningMenu(MODE_MENU);
            break;
        case EXIT:
            runningMenu(MAIN_MENU);
            break;
        case DELETE:
            deleteFeedScheduleFile();
            setNumberOfScheduledFeeds(0);
            initializeNextFeed();
            break;
    }
}
