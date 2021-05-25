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

    // Define controls rectangles
    layoutRecsTitle = (Rectangle){AnchorVisualization.x, AnchorVisualization.y, 730, 20};
    layoutRecsContent = (Rectangle){AnchorVisualization.x, AnchorVisualization.y + 20, 730, 730};
    layoutRecsButtonDraw = (Rectangle){AnchorVisualization.x + 10, AnchorVisualization.y + 35, 80, 32};
    layoutRecsTextBoxStatus = (Rectangle){AnchorVisualization.x + 105, AnchorVisualization.y + 35, 615, 32};
    layoutRecsPanelVisualization = (Rectangle){AnchorVisualization.x + 10, AnchorVisualization.y + 82, 710, 658};

    float trianglePaddingX = 40;        // PanelVisualization padding in x axis
    float triangleSide = layoutRecsPanelVisualization.width - 2*trianglePaddingX;
    int triangleHeight = (int) sqrt(pow(triangleSide,2)-pow(triangleSide/2,2));
    int trianglePaddingY = (layoutRecsPanelVisualization.height - triangleHeight)/2;        // PanelVisualization padding in y axis
    
    trianglePoints[0] = (Vector2) {layoutRecsPanelVisualization.x + layoutRecsPanelVisualization.width/2, layoutRecsPanelVisualization.y + trianglePaddingY};
    trianglePoints[1] = (Vector2) {layoutRecsPanelVisualization.x + trianglePaddingX, layoutRecsPanelVisualization.y + layoutRecsPanelVisualization.height - trianglePaddingY};
    trianglePoints[2] = (Vector2) {layoutRecsPanelVisualization.x + layoutRecsPanelVisualization.width - trianglePaddingX, layoutRecsPanelVisualization.y + layoutRecsPanelVisualization.height - trianglePaddingY};

    int symbols[13] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 73, 88, 960}; // 0 to 9; I; X; pi
    alternativeFont = LoadFontEx("fonts/cmunss.ttf", 32, symbols, 13); // Used to get pi symbol
    strcpy(LabelPriorCircleText, "\u03C0"); // pi symbol &#x3c0;   \u3c0 

    for(int i = 0; i < 3; i++){
        LabelTriangleText[i] = "X" + to_string(i+1);
    }
    layoutRecsLabelTriangle[0] = (Rectangle){trianglePoints[0].x - 10, trianglePoints[0].y - 35, 40, 40};        
    layoutRecsLabelTriangle[1] = (Rectangle){trianglePoints[1].x - 35, trianglePoints[1].y -  2, 40, 40};        
    layoutRecsLabelTriangle[2] = (Rectangle){trianglePoints[2].x +  5, trianglePoints[2].y -  2, 40, 40};
}