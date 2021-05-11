#include "guimenu.h"

GuiMenu::GuiMenu(){
    // Const text
    buttonOpenText = "Open";    // BUTTON: buttonOpen
    buttonSaveText = "Save";    // BUTTON: buttonSave
    buttonExamplesText = "Examples";    // BUTTON: buttonExamples
    buttonHelpText = "Help";    // BUTTON: buttonHelp
    buttonAboutText = "About";    // BUTTON: buttonAbout

    // Define controls rectangles
    layoutRecsButtons = vector<Rectangle>(6);
    layoutRecsButtons[REC_BUTTON_OPEN] = (Rectangle){10, 10, 80, 32};    // Button: buttonOpen
    layoutRecsButtons[REC_BUTTON_SAVE] = (Rectangle){100, 10, 80, 32};    // Button: buttonSave
    layoutRecsButtons[REC_BUTTON_EXAMPLES] = (Rectangle){190, 10, 80, 32};    // Button: buttonExamples
    layoutRecsButtons[REC_BUTTON_HELP] = (Rectangle){280, 10, 80, 32};    // Button: buttonHelp
    layoutRecsButtons[REC_BUTTON_ABOUT] = (Rectangle){370, 10, 80, 32};    // Button: buttonAbout
    
    layoutRecsLine = (Rectangle){0, 45, 1130, 10};    // Line: LineMenu
}