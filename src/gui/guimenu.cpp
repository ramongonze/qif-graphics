#include "guimenu.h"

GuiMenu::GuiMenu(){
    // Data
    windowErrorActive = false;
    dropdownBoxOpen = 0;
    dropDownOpenEditMode = false;

    // Text
    buttonOpenText = (char*) malloc(128*sizeof(char));
    buttonExamplesText = (char*) malloc(128*sizeof(char));
    buttonHelpText = (char*) malloc(128*sizeof(char));
    fileNameToOpen = (char*) malloc(2048*sizeof(char));
    fileNameToSave = (char*) malloc(2048*sizeof(char));

    strcpy(buttonOpenText, "Open");
    strcpy(buttonExamplesText, "Examples");
    strcpy(buttonHelpText, "Help");
    strcpy(fileNameToOpen, "\0");
    strcpy(fileNameToSave, "\0");

    // Define controls rectangles
    layoutRecsButtons = vector<Rectangle>(5);
    layoutRecsButtons[REC_BUTTON_OPEN] = (Rectangle){0, 0, 50, 25};    // Button: buttonOpen
    layoutRecsButtons[REC_BUTTON_EXAMPLES] = (Rectangle){50, 0, 80, 25};    // Button: buttonExamples
    layoutRecsButtons[REC_BUTTON_HELP] = (Rectangle){130, 0, 50, 25};    // Button: buttonHelp
    
    layoutRecsMenu = (Rectangle){0, 0, 1130, 25};    // Line: LineMenu
}