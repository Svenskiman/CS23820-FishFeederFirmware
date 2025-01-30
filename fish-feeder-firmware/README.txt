----------------------------------------------
To successfully build the project for Windows:
----------------------------------------------
The CMakeLists.txt file has two elements:
 - include_directories
 - target_link_libraries

These elements need to have their Windows entries uncommented
and the other platforms entries commented out or deleted.

Windows also requires a path to these libraries to be set either in the run configuration in CLion
(or as a path entry if the program is to be executed from the terminal) to prevent it finding
incompatible versions of the jvm components if java is installed elsewhere in the system.
*nix based platforms naturally look first in the provided custom runtime folder so don't need
any additional paths to be set.

----------------------------------------------
Project files:
----------------------------------------------
main.c - functions that are the main entry point for starting the fish feeder emulator program

menu.c - functions that monitor the button press, call the appropriate functions depending
on which menu the button was pressed, display the appropriate menu, menu text positioning and
emulator initial setup.
menu.h - a MenuData structure which each menu should have a different initialisation of, determining
how a specific menu and its contents should be displayed and navigated. As well as, pre-processor
definitions for text size, screenwidth etc.

mainMenu.c/h - the first menu that is displayed when the emulator starts. Displays the live time (either
set by the user or defaulting to 1968), next feed time, feeds since automatic feed start, mode and access to config menu.

configMenu.c/h - config menu data and access to other menus e.g., set clock, schedule feed and mode select.

setTimeMenu.c/h - functions that allow the user to input and set the current time (hours:minutes) of the emulator program.

scheduleFeedMenu.c/h - display 'this' schedules feed time and rotations, apply them or access to menus
that allow the user to set the feed time and rotation number.
setFeedRotationsMenu.c/h - input and display 'this' schedules number of feed rotations.
setFeedTimeMenu.c/h - inout and display 'this' scheduled feed hour and feed minute.

modeMenu.c/h - functions that change the current feeder mode: Auto, Paused, Manual or Skip next.
The default mode is auto, also includes functions that determine and initialise the next scheduled feed.

processFeedScheduleFile.c/h - functions that deal with creating, writing to, reading from and sorting the data from
the feedSchedule.txt file.

fish.c/h - functions that allow for manipulating the emulators graphical display, etc.
----------------------------------------------
Standard libraries/packages and versions:
----------------------------------------------
Cmake minimum version: 3.25
C11 with the cmake flags:
 - Wall
 - ansi
 - Wextra
 - Wpedantic

C11 standard library, specifically:
 - stdbool.h
 - stdio.h
 - stdlib.h
 - string.h

Revised fish.c and fish.h files - 29/11/2024