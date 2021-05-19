#include "guimenu.h"

GuiMenu::GuiMenu(){
    // Data
    windowErrorActive = false;

    // Text
    buttonOpenText = (char*) malloc(128*sizeof(char));
    buttonSaveText = (char*) malloc(128*sizeof(char));
    buttonExamplesText = (char*) malloc(128*sizeof(char));
    buttonHelpText = (char*) malloc(128*sizeof(char));
    buttonAboutText = (char*) malloc(128*sizeof(char));
    fileNameToOpen = (char*) malloc(2048*sizeof(char));
    fileNameToSave = (char*) malloc(2048*sizeof(char));

    strcpy(buttonOpenText, "Open");
    strcpy(buttonSaveText, "Save");
    strcpy(buttonExamplesText, "Examples");
    strcpy(buttonHelpText, "Help");
    strcpy(buttonAboutText, "About");
    strcpy(fileNameToOpen, "\0");
    strcpy(fileNameToSave, "\0");

    // Define controls rectangles
    layoutRecsButtons = vector<Rectangle>(6);
    layoutRecsButtons[REC_BUTTON_OPEN] = (Rectangle){10, 10, 80, 32};    // Button: buttonOpen
    layoutRecsButtons[REC_BUTTON_SAVE] = (Rectangle){100, 10, 80, 32};    // Button: buttonSave
    layoutRecsButtons[REC_BUTTON_EXAMPLES] = (Rectangle){190, 10, 80, 32};    // Button: buttonExamples
    layoutRecsButtons[REC_BUTTON_HELP] = (Rectangle){280, 10, 80, 32};    // Button: buttonHelp
    layoutRecsButtons[REC_BUTTON_ABOUT] = (Rectangle){370, 10, 80, 32};    // Button: buttonAbout
    
    layoutRecsLine = (Rectangle){0, 45, 1130, 10};    // Line: LineMenu
}