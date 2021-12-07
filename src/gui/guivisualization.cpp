#include "guivisualization.h"

GuiVisualization::GuiVisualization(){
    // Const text
    strcpy(GroupBoxVisualizationText, "Visualization");
    strcpy(ButtonDrawText, "Draw");
    
    // Define anchors
    AnchorVisualization = {370, 35};
    
    // Define controls variable
    TextBoxStatusEditMode = false;
    strcpy(TextBoxStatusText, "Status");
    SpinnerEpsilonEditMode = false;
    SpinnerEpsilonValue = 2;
    SliderDeltaValue = 0;
    recomputeInners = false;

    // Define controls rectangles
    recTitle = (Rectangle){AnchorVisualization.x, AnchorVisualization.y, 730, 20};
    recContent = (Rectangle){AnchorVisualization.x, AnchorVisualization.y + 20, 730, 730};
    recButtonDraw = (Rectangle){AnchorVisualization.x + 10, AnchorVisualization.y + 35, 80, 32};
    recTextBoxStatus = (Rectangle){AnchorVisualization.x + 105, AnchorVisualization.y + 35, 615, 32};
    recPanelVisualization = (Rectangle){AnchorVisualization.x + 10, AnchorVisualization.y + 82, 710, 658};
    recCheckboxShowLabels = (Rectangle){recPanelVisualization.x + 10, recPanelVisualization.y + 10, 20, 20};
    recCheckboxShowConvexHull = (Rectangle){recCheckboxShowLabels.x, recCheckboxShowLabels.y + 30, 20, 20};

    float trianglePaddingX = 40;        // PanelVisualization padding in x axis
    float triangleSide = recPanelVisualization.width - 2*trianglePaddingX;
    int triangleHeight = (int) sqrt(pow(triangleSide,2)-pow(triangleSide/2,2));
    int trianglePaddingY = (recPanelVisualization.height - triangleHeight)/2;        // PanelVisualization padding in y axis
    
    trianglePoints[0] = (Vector2) {recPanelVisualization.x + recPanelVisualization.width/2, recPanelVisualization.y + trianglePaddingY};
    trianglePoints[1] = (Vector2) {recPanelVisualization.x + trianglePaddingX, recPanelVisualization.y + recPanelVisualization.height - trianglePaddingY};
    trianglePoints[2] = (Vector2) {recPanelVisualization.x + recPanelVisualization.width - trianglePaddingX, recPanelVisualization.y + recPanelVisualization.height - trianglePaddingY};

    strcpy(LabelPriorCircleText, "\u03C0"); // pi symbol &#x3c0;   \u3c0 
    strcpy(LabelCheckboxShowLabel, "Show labels");
    strcpy(LabelCheckboxShowConvexHull, "Show convex hull");

    for(int i = 0; i < 3; i++){
        LabelTriangleText[i] = "X" + to_string(i+1);
    }
    recLabelTriangle[0] = (Rectangle){trianglePoints[0].x - 10, trianglePoints[0].y - 35, 40, 40};
    recLabelTriangle[1] = (Rectangle){trianglePoints[1].x - 35, trianglePoints[1].y -  2, 40, 40};
    recLabelTriangle[2] = (Rectangle){trianglePoints[2].x +  5, trianglePoints[2].y -  2, 40, 40};
    
    recSpinnerEpsilon = (Rectangle){recPanelVisualization.x + recPanelVisualization.width - 120, recPanelVisualization.y + 10, 100, 20};
    recSliderDelta = (Rectangle){recPanelVisualization.x + recPanelVisualization.width - 120, recPanelVisualization.y + 40, 100, 20};
}
