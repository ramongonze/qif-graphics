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
    AnchorPosterior = {10, 490};            // ANCHOR ID:4

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
    layoutRecsGroupBox = {AnchorPosterior.x + 0, AnchorPosterior.y + 0, 350, 300};    // GroupBox: GroupBoxPosteriors
    layoutRecsScrollPanel = {AnchorPosterior.x + 15, AnchorPosterior.y + 20, 320, 265};    // ScrollPanel: ScrollPanelPosteriors
    ScrollPanelPosteriorsContent.y = layoutRecsScrollPanel.height - 20;
    layoutRecsLabelOuter = {AnchorPosterior.x + 15, AnchorPosterior.y + 55, TEXTBOX_SIZE, TEXTBOX_SIZE};    // Label: LabelOuter
    layoutRecsLabelPosteriors = vector<Rectangle>(numPosteriors);
    for(int i = 0; i < numPosteriors; i++){
        layoutRecsLabelPosteriors[i] = (Rectangle){AnchorPosterior.x + 75 + i*TEXTBOX_SIZE, AnchorPosterior.y + 35, 20, 20};    // Label: LabelPosteriors
    }
    
    layoutRecsLabelX = vector<Rectangle>(numPosteriors);
    for(int i = 0; i < numPosteriors; i++){
        layoutRecsLabelX[i] = (Rectangle){AnchorPosterior.x + 40, AnchorPosterior.y + 115 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};    // Label: LabelPosteriorsX1
    }

    layoutRecsTextBoxOuter = vector<Rectangle>(numPosteriors);
    for(int i = 0; i < numPosteriors; i++){
        layoutRecsTextBoxOuter[i] = (Rectangle){ AnchorPosterior.x + 65 + i*TEXTBOX_SIZE, AnchorPosterior.y + 55, TEXTBOX_SIZE, TEXTBOX_SIZE };    // TextBox: TextBoxOuter0
    }

    layoutRecsTextBoxInners = vector<vector<Rectangle>>(numPosteriors, vector<Rectangle>(3));
    for(int i = 0; i < numPosteriors; i++){
        for(int j = 0; j < 3; j++){
            layoutRecsTextBoxInners[i][j] = (Rectangle){AnchorPosterior.x + 65 + i*TEXTBOX_SIZE, AnchorPosterior.y + 115 + j*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};    // TextBox: TextBoxInners00
        }
    }

    ScrollPanelPosteriorsContent.x = layoutRecsTextBoxInners[numPosteriors-1][0].x + TEXTBOX_SIZE;
}