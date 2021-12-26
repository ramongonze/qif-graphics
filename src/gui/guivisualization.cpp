#include "guivisualization.h"

GuiVisualization::GuiVisualization(){
    // Const text
    strcpy(gBoxVisTxt, "Visualization");
    strcpy(buttonDrawTxt, "Draw");
    
    // Define anchors
    anchorVis = {370, 35};
    
    // Define controls variable
    tBoxStatusEdit = false;
    strcpy(tBoxStatusTxt, "Status");
    spinEpsilonEdit = false;
    spinEpsilonValue = 2;
    sliderDeltaValue = 0;
    sliderAlphaValue = 0.5;
    recomputeInners = false;

    // Define controls rectangles
    recTitle = (Rectangle){anchorVis.x, anchorVis.y, 730, 20};
    recContent = (Rectangle){anchorVis.x, anchorVis.y + 20, 730, 730};
    recButtonDraw = (Rectangle){anchorVis.x + 10, anchorVis.y + 35, 80, 32};
    recTBoxStatus = (Rectangle){anchorVis.x + 105, anchorVis.y + 35, 615, 32};
    recPanVis = (Rectangle){anchorVis.x + 10, anchorVis.y + 82, 710, 658};
    recCBoxShowLabels = (Rectangle){recPanVis.x + 10, recPanVis.y + 10, 20, 20};
    recCBoxShowCHull = (Rectangle){recCBoxShowLabels.x, recCBoxShowLabels.y + 30, 20, 20};

    float trianglePaddingX = 40;        // PanelVisualization padding in x axis
    float triangleSide = recPanVis.width - 2*trianglePaddingX;
    int triangleHeight = (int) sqrt(pow(triangleSide,2)-pow(triangleSide/2,2));
    int trianglePaddingY = (recPanVis.height - triangleHeight)/2;        // PanelVisualization padding in y axis
    
    trianglePoints[0] = (Vector2) {recPanVis.x + recPanVis.width/2, recPanVis.y + trianglePaddingY};
    trianglePoints[1] = (Vector2) {recPanVis.x + trianglePaddingX, recPanVis.y + recPanVis.height - trianglePaddingY};
    trianglePoints[2] = (Vector2) {recPanVis.x + recPanVis.width - trianglePaddingX, recPanVis.y + recPanVis.height - trianglePaddingY};

    strcpy(labelPriorCircTxt, "\u03C0"); // pi symbol &#x3c0;   \u3c0 
    strcpy(labelCBoxShowLabel, "Show labels");
    strcpy(labelCBoxShowCHull, "Show convex hull");

    for(int i = 0; i < 3; i++){
        labelTriangleTxt[i] = "X" + to_string(i+1);
    }
    recLabelTriangle[0] = (Rectangle){trianglePoints[0].x - 10, trianglePoints[0].y - 35, 40, 40};
    recLabelTriangle[1] = (Rectangle){trianglePoints[1].x - 35, trianglePoints[1].y -  2, 40, 40};
    recLabelTriangle[2] = (Rectangle){trianglePoints[2].x +  5, trianglePoints[2].y -  2, 40, 40};
    
    recSpinEpsilon = (Rectangle){recPanVis.x + recPanVis.width - 120, recPanVis.y + 10, 100, 20};
    recSliderDelta = sum2Rec(recSpinEpsilon, 0, 30, 0, 0);
    recSliderAlpha = recSpinEpsilon;
}
