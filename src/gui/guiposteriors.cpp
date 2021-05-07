#include "guiposteriors.h"

GuiPosteriors::GuiPosteriors(){
    // Data
    numPosteriors = 3;
    
    // Const text
    GroupBoxPosteriorsText = "Posterior distributions";    // GROUPBOX: GroupBoxPosteriors
    LabelOuterText = "Outer";    // LABEL: LabelOuter
    LabelPosteriorsText = vector<string>({"I1", "I2", "I3"});
    LabelPosteriorsXText = vector<string>({"X1", "X2", "X3"});

    // Define anchors
    AnchorPosterior = {10, 450};            // ANCHOR ID:4

    // Define controls variable
    ScrollPanelPosteriorsScrollOffset = {0, 0};
    ScrollPanelPosteriorsBoundsOffset = {0, 0};            // ScrollPanel: ScrollPanelPosteriors
    
    TextBoxOuterEditMode = vector<bool>(numPosteriors, false);
    TextBoxOuterText = vector<char*>(numPosteriors, nullptr);
    for(int i = 0; i < numPosteriors; i++){
        TextBoxOuterText[i] = (char*) malloc(128*sizeof(char));
        strcpy(TextBoxOuterText[i], "0");
    }

    TextBoxInnersEditMode = vector<vector<bool>>(3, vector<bool>(numPosteriors, false));
    TextBoxInnersText = vector<vector<char*>>(3, vector<char*>(numPosteriors, nullptr));
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < numPosteriors; j++){
            TextBoxInnersText[i][j] = (char*) malloc(128*sizeof(char));
            strcpy(TextBoxInnersText[i][j], "0");
        }
    }

    // Define controls rectangles
    layoutRecsGroupBox = {AnchorPosterior.x + 0, AnchorPosterior.y + 0, 350, 270};    // GroupBox: GroupBoxPosteriors
    layoutRecsScrollPanel = {AnchorPosterior.x + 15, AnchorPosterior.y + 20, 320, 235};    // ScrollPanel: ScrollPanelPosteriors
    layoutRecsLabelOuter = {AnchorPosterior.x + 20, AnchorPosterior.y + 55, 40, 40};    // Label: LabelOuter
    layoutRecsLabelPosteriors = vector<Rectangle>(numPosteriors);
    for(int i = 0; i < numPosteriors; i++){
        layoutRecsLabelPosteriors[i] = (Rectangle){AnchorPosterior.x + 75 + i*40, AnchorPosterior.y + 35, 20, 20};    // Label: LabelPosteriors
    }
    
    layoutRecsLabelX = vector<Rectangle>(numPosteriors);
    for(int i = 0; i < numPosteriors; i++){
        layoutRecsLabelX[i] = (Rectangle){AnchorPosterior.x + 40, AnchorPosterior.y + 115 + i*40, 20, 40};    // Label: LabelPosteriorsX1
    }

    layoutRecsTextBoxOuter = vector<Rectangle>(numPosteriors);
    for(int i = 0; i < numPosteriors; i++){
        layoutRecsTextBoxOuter[i] = (Rectangle){ AnchorPosterior.x + 65 + i*40, AnchorPosterior.y + 55, 40, 40 };    // TextBox: TextBoxOuter0
    }

    layoutRecsTextBoxInners = vector<vector<Rectangle>>(3, vector<Rectangle>(numPosteriors));
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < numPosteriors; j++){
            layoutRecsTextBoxInners[i][j] = (Rectangle){ AnchorPosterior.x + 65 + i*40, AnchorPosterior.y + 115 + j*40, 40, 40 };    // TextBox: TextBoxInners00
        }
    }
}