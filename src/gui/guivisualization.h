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
    char gBoxVisTxt[CHAR_BUFFER_SIZE];
    char buttonDrawTxt[CHAR_BUFFER_SIZE];
    char labelPriorCircTxt[CHAR_BUFFER_SIZE];
    
    // Define anchors
    Vector2 anchorVis;
    
    // Define controls variable
    bool tBoxStatusEdit;
    char tBoxStatusTxt[CHAR_BUFFER_SIZE];
    bool spinEpsilonEdit;
    int spinEpsilonValue;
    float sliderDeltaValue;
    float sliderAlphaValue;
    bool recomputeInners; // Used to recompute inners circles if the user changed epsilon or delta value

    // Define controls rectangles
    Rectangle recTitle;
    Rectangle recContent;
    Rectangle recButtonDraw;
    Rectangle recTBoxStatus;
    Rectangle recPanVis;
    Rectangle recLabelPriorCirc;
    Rectangle recLabelInnersCirc[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS];
    Rectangle recLabelTriangle[3];
    Rectangle recCBoxShowLabels;
    Rectangle recCBoxShowCHull;
    Rectangle recSpinEpsilon;
    Rectangle recSliderDelta;
    Rectangle recSliderAlpha;

    /* Triangle vertices
     *       v0
     *       /\
     *   v1 /__\ v2 */
    Vector2 trianglePoints[3];

    // Triangle vertex labels
    string labelTriangleTxt[3];
    char labelCBoxShowLabel[CHAR_BUFFER_SIZE];
    char labelCBoxShowCHull[CHAR_BUFFER_SIZE];
};

#endif