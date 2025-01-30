#pragma once
#include <stdbool.h>

bool doesFileExist(const char *fileName);
void createFeedScheduleFile(const char* filename);
void deleteFeedScheduleFile();

int* getFeedScheduleData();
int getNumberOfScheduledFeedsFromFile();
void setNumberOfScheduledFeeds(int feeds);
int getNumberOfScheduledFeeds();
long long getClockWarmStartFromFile();

void writeScheduleDataToFile(int feedTimeHour, int feedTimeMinute, int feedRotationAmount);
void replaceStoredClockWarmStart(long long clockWarmStartNumber);

int* quickSortFeedTimes(int length, int* feedTimes);
int compare(const void* elementA, const void* elementB);