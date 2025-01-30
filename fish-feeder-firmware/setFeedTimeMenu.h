#pragma once

struct MenuData getFeedTimeMenuData();
int getFeedHour();
int getFeedMinute();

void resetFeedTime();

void displayFeedTime(int feedHour, int feedMinute);

void setFeedTimeMenuOptions(int currentlySelectedOption);