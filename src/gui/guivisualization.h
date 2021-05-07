#ifndef _guivisualization
#define _guivisualization

#include "../../libs/raylib/src/raylib.h"
#include <string.h>

class GuiVisualization{
private:
    /* data */
public:
    GuiVisualization();

    // Const text
    const char *GroupBoxVisualizationText;    // GROUPBOX: GroupBoxVisualization
    const char *ButtonDrawText;    // BUTTON: buttonDraw
    
    // Define anchors
    Vector2 AnchorVisualization;            // ANCHOR ID:3
    
    // Define controls variable
    bool TextBoxStatusEditMode;
    char TextBoxStatusText[128];            // TextBox: TextBoxStatus

    // Define controls rectangles
    Rectangle layoutRecsGroupBoxVisualization;
    Rectangle layoutRecsButtonDraw;
    Rectangle layoutRecsTextBoxStatus;
    Rectangle layoutRecsPanelVisualization;
};

#endif