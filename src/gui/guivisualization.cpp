#include "guivisualization.h"

GuiVisualization::GuiVisualization(){
    // Const text
    GroupBoxVisualizationText = (char*) malloc(128*sizeof(char));
    ButtonDrawText = (char*) malloc(128*sizeof(char));
    strcpy(GroupBoxVisualizationText, "Visualization");
    strcpy(ButtonDrawText, "Draw");
    
    // Define anchors
    AnchorVisualization = {380, 35};            // ANCHOR ID:3
    
    // Define controls variable
    TextBoxStatusEditMode = false;
    strcpy(TextBoxStatusText, "Status");    // TextBox: TextBoxStatus

    // Define controls rectangles
    layoutRecsTitle = {AnchorVisualization.x, AnchorVisualization.y, 730, 20};    // GroupBox: GroupBoxVisualization;
    layoutRecsContent = {AnchorVisualization.x, AnchorVisualization.y + 20, 730, 710};
    layoutRecsButtonDraw = (Rectangle){AnchorVisualization.x + 10, AnchorVisualization.y + 35, 80, 32};    // Button: buttonDraw;
    layoutRecsTextBoxStatus = (Rectangle){AnchorVisualization.x + 105, AnchorVisualization.y + 35, 615, 32};    // TextBox: TextBoxStatus;
    layoutRecsPanelVisualization = (Rectangle){AnchorVisualization.x + 10, AnchorVisualization.y + 82, 710, 638};    // Panel: PanelVisualization;

    trianglePoints = vector<Vector2>(3);
    float trianglePaddingX = 40;        // PanelVisualization padding in x axis
    float triangleSide = layoutRecsPanelVisualization.width - 2*trianglePaddingX;
    int triangleHeight = (int) sqrt(pow(triangleSide,2)-pow(triangleSide/2,2));
    int trianglePaddingY = (layoutRecsPanelVisualization.height - triangleHeight)/2;        // PanelVisualization padding in y axis
    
    trianglePoints = {
        (Vector2) {layoutRecsPanelVisualization.x + layoutRecsPanelVisualization.width/2, layoutRecsPanelVisualization.y + trianglePaddingY},
        (Vector2) {layoutRecsPanelVisualization.x + trianglePaddingX, layoutRecsPanelVisualization.y + layoutRecsPanelVisualization.height - trianglePaddingY},
        (Vector2) {layoutRecsPanelVisualization.x + layoutRecsPanelVisualization.width - trianglePaddingX, layoutRecsPanelVisualization.y + layoutRecsPanelVisualization.height - trianglePaddingY}
    };

    layoutRecsLabelInnersCircles = vector<Rectangle>(3);
    int symbols[13] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 73, 88, 960}; // 0 to 9; I; X; pi
    alternativeFont = LoadFontEx("fonts/cmunss.ttf", 32, symbols, 13); // Used to get pi symbol
    LabelPriorCircleText = (char*) malloc(128 * sizeof(char));
    strcpy(LabelPriorCircleText, "\u03C0"); // pi symbol &#x3c0;   \u3c0 

    LabelTriangleText = vector<string>({"X1", "X2", "X3"});
    layoutRecsLabelTriangle = vector<Rectangle>(3);
    layoutRecsLabelTriangle[0] = (Rectangle){trianglePoints[0].x - 10, trianglePoints[0].y - 35, 40, 40};        
    layoutRecsLabelTriangle[1] = (Rectangle){trianglePoints[1].x - 35, trianglePoints[1].y -  2, 40, 40};        
    layoutRecsLabelTriangle[2] = (Rectangle){trianglePoints[2].x +  5, trianglePoints[2].y -  2, 40, 40};
}