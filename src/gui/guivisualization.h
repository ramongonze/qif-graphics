#ifndef _guivisualization
#define _guivisualization

#include "../../libs/raylib/src/raylib.h"
#include "../graphics.h"
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

    //------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------

    // Const text
    char GroupBoxVisualizationText[CHAR_BUFFER_SIZE];
    char ButtonDrawText[CHAR_BUFFER_SIZE];
    char LabelPriorCircleText[CHAR_BUFFER_SIZE];
    
    // Define anchors
    Vector2 AnchorVisualization;
    
    // Define controls variable
    bool TextBoxStatusEditMode;
    char TextBoxStatusText[CHAR_BUFFER_SIZE];

    // Define controls rectangles
    Rectangle layoutRecsTitle;
    Rectangle layoutRecsContent;
    Rectangle layoutRecsButtonDraw;
    Rectangle layoutRecsTextBoxStatus;
    Rectangle layoutRecsPanelVisualization;
    Rectangle layoutRecsLabelPriorCircle;
    Rectangle layoutRecsLabelInnersCircles[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS];
    Rectangle layoutRecsLabelTriangle[3];
    Rectangle layoutRecsCheckboxShowLabels;
    Rectangle layoutRecsCheckboxShowConvexHull;

    /* Triangle vertices
     *       v0
     *       /\
     *   v1 /__\ v2 */
    Vector2 trianglePoints[3];

    // Triangle vertex labels
    string LabelTriangleText[3];
    char LabelCheckboxShowLabel[CHAR_BUFFER_SIZE];
    char LabelCheckboxShowConvexHull[CHAR_BUFFER_SIZE];
};

#endif