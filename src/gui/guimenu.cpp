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
    layoutRecsButtons[REC_BUTTON_OPEN] = (Rectangle){0, 0, 50, 25};    // Button: buttonOpen
    layoutRecsButtons[REC_BUTTON_SAVE] = (Rectangle){50, 0, 50, 25};    // Button: buttonSave
    layoutRecsButtons[REC_BUTTON_EXAMPLES] = (Rectangle){100, 0, 80, 25};    // Button: buttonExamples
    layoutRecsButtons[REC_BUTTON_HELP] = (Rectangle){180, 0, 50, 25};    // Button: buttonHelp
    layoutRecsButtons[REC_BUTTON_ABOUT] = (Rectangle){230, 0, 60, 25};    // Button: buttonAbout
    
    layoutRecsMenu = (Rectangle){0, 0, 1130, 25};    // Line: LineMenu
}