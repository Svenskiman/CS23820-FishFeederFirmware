#pragma once

struct MenuData getMainMenuData();

void mainMenuOptions(int currentlySelectedOption);

void displayMainMenuTime();

void displayMainMenuChangeableData(int feedsSinceAutoStart, char* currentMode, int hours, int minutes);
