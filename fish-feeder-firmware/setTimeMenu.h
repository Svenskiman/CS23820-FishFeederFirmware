#pragma once

struct MenuData getSetTimeMenuData() ;
int getHour();
int getMinute();
void resetTime();

void displaySetTime(int hour, int minute);
void setTimeMenuOptions(int currentlySelectedOption);