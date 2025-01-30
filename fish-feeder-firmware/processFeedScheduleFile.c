#include "processFeedScheduleFile.h"
#include <stdio.h>
#include <stdlib.h>

int numberOfScheduledFeeds = 0;

/**
 * Checks if a file with the provided name exists
 * @param fileName the file we want to check the existence of
 * @return true if it exists
 */
bool doesFileExist(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    bool exists = false;
    if (file != NULL) {
        exists = true;
        fclose(file);
    }
    return exists;
}

/**
 * Creates a new text file with the provided name.
 * The first line is reserved for the clock warm start data
 * @param filename the name of the file
 */
void createFeedScheduleFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    // When the file is just being made, this reserves the first line for the result of
    // the clock warm start function
    fprintf(file, "%d", 0);
    fprintf(file, "\n");
    fclose(file);
}

/**
 * Deletes the feedSchedule.txt file
 */
void deleteFeedScheduleFile() {
    const char* fileName = "feedSchedule.txt";
    if (!doesFileExist(fileName)) {
        printf("Can't delete file, it doesn't exist");
        return;
    }
    remove(fileName);
}

/**
 * Appends the passed feed schedule data to the appropriate text file
 * @param feedTimeHour the feed hour
 * @param feedTimeMinute the feed minute
 * @param feedRotationAmount the number of times the feeder should be rotated
 */
void writeScheduleDataToFile(const int feedTimeHour, const int feedTimeMinute, const int feedRotationAmount) {
    const char* fileName = "feedSchedule.txt";

    FILE* file;
    // Creates the file if it doesn't exist
    if (!doesFileExist(fileName)) {
        createFeedScheduleFile(fileName);
    }
    file = fopen(fileName, "a");
    // Appends the data to the file
    // fprintf(file, "\n");
    fprintf(file, "%02i%02i%d", feedTimeHour, feedTimeMinute, feedRotationAmount);
    fclose(file);
}

/**
 * Counts and returns the number of set feed schedules
 * @return the total number of written lines in the feedSchedule file, ignoring the first one
 * holding the clockWarmStart data.
 */
int getNumberOfScheduledFeedsFromFile() {

    const char* fileName = "feedSchedule.txt";

    // Check if file exists, if it doesn't, return 0
    if (!doesFileExist(fileName)) {
        // printf("%s\n", "A feed schedule has never been set - getNumberOfScheduledFeedsFromFile");
        return 0;
    }

    FILE* file = fopen(fileName, "r");
    int currentLine = 1;
    char currentChar;

    // Iterate through every character in a line
    // Once currentChar == new line, increment number of lines
    do {
        currentChar = fgetc(file);
        if (currentChar == '\n') {
            currentLine++;
        }
    } while(currentChar != EOF);
    fclose(file);
    currentLine--; // Subtract 1 to account for the first line holding clock data
    return currentLine;
}

/**
 * Setter method for the number of scheduled feeds
 * @param feeds feeds
 */
void setNumberOfScheduledFeeds(int feeds) {
    numberOfScheduledFeeds = feeds;
}

/**
 * Gets the number of scheduled feeds
 * @return numberOfScheduledFeeds
 */
int getNumberOfScheduledFeeds() {
    return numberOfScheduledFeeds;
}

/**
 * Reads all the contents of the text file except first line into an array of
 * integers that is allocated on the heap.
 * @return a pointer to the array containing all feed schedule data
 */
int* getFeedScheduleData() {

    // Create the array on the heap so we can return a pointer to it
    int *data = malloc(sizeof(int) * numberOfScheduledFeeds);

    const char* fileName = "feedSchedule.txt";

    // If the file doesn't exist, return null
    if (!doesFileExist(fileName)) {
        printf("Feed schedule has never been created");
        return NULL;
    }

    FILE* file = fopen(fileName, "r");

    // Skip the first line in the file:
    // https://stackoverflow.com/questions/2799612/how-to-skip-a-line-when-fscanning-a-text-file
    fscanf(file, "%*[^\n]\n");

    int currentFeedSchedule;
    // Read each integer in the file, can use a for loop for this as we know how many
    // lines of data there should be
    for (int i = 0; i < numberOfScheduledFeeds; i++) {
        fscanf(file, "%d", &currentFeedSchedule);
        data[i] = currentFeedSchedule;
    }
    fclose(file);
    return data;
}

/**
 * Sorts an array of integers of a given size
 * @param length size of the array
 * @param feedTimes the currently unsorted array
 * @return a pointer to an array of times sorted in ascending order
 */
int* quickSortFeedTimes(int length, int* feedTimes) {
    qsort(feedTimes, length, sizeof(int), compare);
    return feedTimes;
}

/**
 *	Comparison function which returns a negative integer value if the first argument
 *	is less than the second, a positive integer value if the first argument is greater
 *	than the second and zero if the arguments are equivalent.
 *	https://en.cppreference.com/w/c/algorithm/qsort
 * @param elementA pointer to an array element to be compared
 * @param elementB pointer to a different array element to be compared
 * @return elementA - elementB
 */
int compare(const void* elementA, const void* elementB) {
    return (*(int*)elementA - *(int*)elementB);
}

/**
 * Gets the clockWarmStart number from the first line of the feedSchedule text file
 * @return a long-long representing the clock offset
 */
long long getClockWarmStartFromFile() {
    const char* fileName = "feedSchedule.txt";

    FILE* file = fopen(fileName, "r");
    long long numberRetrieved;
    // Read the first line of the file
    fscanf(file, "%lld", &numberRetrieved);
    fclose(file);
    return numberRetrieved;
}

/**
 * Replaces the currently stored clock offset number from the first line of the file.
 * This is done by creating a new file, writing all the current files data into the new file,
 * while also replacing the offset number with the new provided one, deleting and then remaining the files.
 * @param clockWarmStartNumber what will replace the first line in the file.
 */
void replaceStoredClockWarmStart(long long clockWarmStartNumber) {
    const char* fileName = "feedSchedule.txt";

    // Creates the file if it doesn't exist, with the new number on the first line
    // this saves time as we don't have to create, delete and then create the file again.
    if (!doesFileExist(fileName)) {
        createFeedScheduleFile(fileName);
        FILE* file = fopen(fileName, "w");
        fprintf(file, "%lld", clockWarmStartNumber);
        fprintf(file, "\n");
        fclose(file);
        return;
    }


    char buffer[128];
    const char* tempName = "temp.txt";

    FILE* file = fopen(fileName, "r");
    FILE* tempFile = fopen(tempName, "w");

    const int lineToReplace = 1;
    int currentLine = 1;
    bool stillWriting = true;


    do {

        fgets(buffer, 128, file);

        if (feof(file)) {
            stillWriting = false;
        }
        if (currentLine == lineToReplace) {
            fprintf(tempFile, "%lld", clockWarmStartNumber);
            fprintf(tempFile, "\n");
        }
        else {
            fputs(buffer, tempFile);
        }
        currentLine++;

    } while(stillWriting == true);

    fclose(file);
    fclose(tempFile);
    remove(fileName);
    rename(tempName, fileName);
}
