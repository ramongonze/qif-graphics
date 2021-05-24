#ifndef _guivisualization
#define _guivisualization

#include "../../libs/raylib/src/raylib.h"
#include <string.h>
#include <string>
#include <vector>
#include <math.h>
#include <iostream>


using namespace std;

class GuiVisualization{
private:
    /* data */
public:
    GuiVisualization();

    // Const text
    char *GroupBoxVisualizationText;    // GROUPBOX: GroupBoxVisualization
    char *ButtonDrawText;    // BUTTON: buttonDraw
    char *LabelPriorCircleText;
    
    // Define anchors
    Vector2 AnchorVisualization;            // ANCHOR ID:3
    
    // Define controls variable
    bool TextBoxStatusEditMode;
    char TextBoxStatusText[128];            // TextBox: TextBoxStatus

    // Define controls rectangles
    Rectangle layoutRecsTitle;
    Rectangle layoutRecsContent;
    Rectangle layoutRecsButtonDraw;
    Rectangle layoutRecsTextBoxStatus;
    Rectangle layoutRecsPanelVisualization;
    Rectangle layoutRecsLabelPriorCircle;
    vector<Rectangle> layoutRecsLabelInnersCircles;
    vector<Rectangle> layoutRecsLabelTriangle;

    /* Triangle vertices
     *       v0
     *       /\
     *   v1 /__\ v2 */
    vector<Vector2> trianglePoints;

    // Triangle vertex labels
    vector<string> LabelTriangleText;

    // Used to draw pi symbol
    Font alternativeFont;
};

#endif