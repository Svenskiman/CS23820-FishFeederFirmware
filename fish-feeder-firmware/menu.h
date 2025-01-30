#pragma once

#define MAIN_MENU 10
#define CONFIG_MENU 20
#define MODE_MENU 30
#define SCHEDULE_FEED_MENU 40
#define SCHEDULE_FEED_TIME_MENU 41
#define SCHEDULE_FEED_ROTATIONS_MENU 42
#define SET_TIME_MENU 50

#define MAX_NUMBER_OF_MENU_ITEMS_Y_AXIS 6

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LINE_SIZE 80
#include <stdbool.h>

typedef struct MenuData {

    char *title;
    char *titleFG;
    char *titleBG;
    int titleSize;
    int titlePositionX;
    int titlePositionY;

    int textToSelectPositionY;
    int textToSelectPositionX;

    char *selectedFG;
    char *selectedBG;
    char *notSelectedFG;
    char *notSelectedBG;
    char *changeableFG;
    char *changeableBG;

    int textStartPositionY;
    int textEndPositionY;
    int textPositionX;

    int textSize;

    int numberOfThingsToDisplay;
    int numberOfSelectableOptions;

    char *textToDisplay[MAX_NUMBER_OF_MENU_ITEMS_Y_AXIS];
    char *selectableOptions[MAX_NUMBER_OF_MENU_ITEMS_Y_AXIS];
} MenuData;


void initialSetup();

void displayMenu(MenuData menu);

void chooseMenuToDisplay(int menuType);

int menuShortPress(int menuType, int currentlySelectedOption);

void menuLongPress(int menuType, int currentlySelectedOption);

void runningMenu(int menuType);

int calcRightSideTextXPosition(const char* leftSideText);
int calcTextRealYPosition(const int positionY);

bool isButtonLongPressed(const char *buttonState);

bool isButtonShortPressed(const char *buttonState);

bool isButtonNotPressed(const char *buttonState);


int navigateMenu(MenuData menu, int currentlySelectedOption);