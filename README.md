# OpenBudget
An open-source budgeting application written in C++ with a Qt 6 front-end (GUI).

## Documentation
The documentation was generated using Doxygen and can be viewed by navigating to the ./Documents/html directory and opening the index.html file.

## Building The Project
The project is built using the Qmake build system and can be built by either importing the project into Qt Creator by selecting the .pro file or through the command line.

### Building From Command Line
1. Create the build output directory
2. CD to the build output directory 
3. Call qmake referencing the openbudget.pro file
4. Call make

Starting from OpenBudget Directory:

    $ mkdir ../Build-OpenBudget
    $ cd ../Build-OpenBudget
    $ qmake ../OpenBudget/openbudget.pro
    $ make



