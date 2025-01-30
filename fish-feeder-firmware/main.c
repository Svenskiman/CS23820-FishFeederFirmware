//
// Created by Neal Snooke on 12/07/2024.
// Updated by Benjamin Miller on 27/11/2024.
// Version 1.2
//
#include <stdio.h>
#include <stdlib.h>
#include "fish.h"
#include "menu.h"

/**
 * The function that is the entry point for the fish feeder C program main logic
 * it is called by jniSetup() from main, once the GUI thread has been initialised.
 */
void userProcessing() {
    // add a message to the log METHOD_ENTRY category (see fish.h for details)
    logAdd(METHOD_ENTRY, "userProcessing()");

    // Initially set up the clock and feeder
    initialSetup();

    // Call the functions to run and display the main menu
    runningMenu(MAIN_MENU);

    logAdd(METHOD_ENTRY, "userProcessing() done");
}

/**
 * main entry point for the Fish Feeder Emulator program
 * @return
 */
int main() {
    // turn on all console logging for the Java and fish feeder JNI C code. Shouldn't need this
    //logAddInfo(GENERAL | METHOD_ENTRY | JNI_MESSAGES | JFX_MESSAGES | THREAD_NAME | THREAD_ID | STACK_INFO);
    //logAddInfo( GENERAL | JFX_MESSAGES | THREAD_NAME | THREAD_ID );

    //logRemoveInfo(METHOD_ENTRY); // don't show these debug messages in the gui

    // add the GUI debug window showing messages received
    logAddInfo( GUI_INFO_DEBUG);

    // add a log entry for entry to this method
    logAdd(METHOD_ENTRY, "main(). test of Fish Feed Hardware Emulator using a JavaFX GUI and jni");

    // start the JVM and set up the JNI environment
    // this will result in the userProcessing() function being called to run the C part of the program
    if (jniSetup() != 0) return 1;

    // hand over this thread to Java FX to create the fish hardware emulator GUI.
    // FX will terminate the application when the main window is closed
    // or the C application calls the FishFeederEmulator.exit() function.
    javaFx();

    return(EXIT_SUCCESS);
}