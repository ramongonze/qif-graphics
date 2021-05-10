#include "guivisualization.h"

GuiVisualization::GuiVisualization(){
    // Const text
    GroupBoxVisualizationText = "Visualization";    // GROUPBOX: GroupBoxVisualization
    ButtonDrawText = "Draw";    // BUTTON: buttonDraw
    
    // Define anchors
    AnchorVisualization = {380, 65};            // ANCHOR ID:3
    
    // Define controls variable
    TextBoxStatusEditMode = false;
    strcpy(TextBoxStatusText, "Status");    // TextBox: TextBoxStatus

    // Define controls rectangles
    layoutRecsGroupBoxVisualization = (Rectangle){AnchorVisualization.x + 0, AnchorVisualization.y + 0, 655, 725};    // GroupBox: GroupBoxVisualization;
    layoutRecsButtonDraw = (Rectangle){AnchorVisualization.x + 20, AnchorVisualization.y + 20, 80, 32};    // Button: buttonDraw;
    layoutRecsTextBoxStatus = (Rectangle){AnchorVisualization.x + 115, AnchorVisualization.y + 20, 520, 32};    // TextBox: TextBoxStatus;
    layoutRecsPanelVisualization = (Rectangle){AnchorVisualization.x + 20, AnchorVisualization.y + 72, 615, 633};    // Panel: PanelVisualization;
}