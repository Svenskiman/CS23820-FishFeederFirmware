#pragma once

struct MenuData getModeMenuData();
char* getFeederOperatingMode();
int getFeedsSinceAutomaticSchedule();
int getNextFeedData(int index, int* data);
int getNextFeedHours();
int getNextFeedMinutes();

void setFeedTimeAndRotations(int data);

int calcIndexOfTheNextFeed(int size, int* data, int currentTime);

int convertCurrentTime();
void initializeNextFeed();
void reinitializeNextFeedIfPaused();

void modeMenuOptions(int currentlySelectedOption);
void commenceTheAutomaticFeed();
void modeFeedNow();
void modePause();
void modeSkipNext();


