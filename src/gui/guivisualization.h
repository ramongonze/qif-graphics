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
    bool SpinnerEpsilonEditMode;
    int SpinnerEpsilonValue;
    float SliderDeltaValue;
    float SliderAlphaValue;
    bool recomputeInners; // Used to recompute inners circles if the user changed epsilon or delta value

    // Define controls rectangles
    Rectangle recTitle;
    Rectangle recContent;
    Rectangle recButtonDraw;
    Rectangle recTextBoxStatus;
    Rectangle recPanelVisualization;
    Rectangle recLabelPriorCircle;
    Rectangle recLabelInnersCircles[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS];
    Rectangle recLabelTriangle[3];
    Rectangle recCheckboxShowLabels;
    Rectangle recCheckboxShowConvexHull;
    Rectangle recSpinnerEpsilon;
    Rectangle recSliderDelta;
    Rectangle recSliderAlpha;

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