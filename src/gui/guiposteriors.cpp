#include "guiposteriors.h"

GuiPosteriors::GuiPosteriors(){
    // Data
    for(int i = 0; i < NUMBER_CHANNELS; i++)
        numPosteriors[i] = 3;
    
    // Const text
    strcpy(GroupBoxPosteriorsText, "Hyper-distribution [\u03C0\u203AC]");
    strcpy(LabelOuterText, "Outer");

    for(int i = 0; i < NUMBER_SECRETS; i++){
        LabelPosteriorsXText[i] = "X" + to_string(i+1);
    }
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        LabelPosteriorsText[CHANNEL_1][i] = "\u03B4" + to_string(i+1);
        LabelPosteriorsText[CHANNEL_2][i] = "\u03B4\'" + to_string(i+1);
        LabelPosteriorsText[CHANNEL_3][i] = "\u03B4\'" + to_string(i+1);
    }

    // Define anchors
    AnchorPosterior = {10, 480};

    // Define controls variable
    ScrollPanelPosteriorsScrollOffset = {0, 0};
    ScrollPanelPosteriorsBoundsOffset = {0, 0};

    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        TextBoxOuterEditMode[i] = false;
        strcpy(TextBoxOuterText[i], "0");
    }

    for(int i = 0; i < NUMBER_SECRETS; i++){
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            TextBoxInnersEditMode[i][j] = false;
            strcpy(TextBoxInnersText[i][j], "0");
        }
    }

    // Define controls rectangles
    recTitle = (Rectangle){AnchorPosterior.x, AnchorPosterior.y, 350, 20};
    recContent = (Rectangle){AnchorPosterior.x, AnchorPosterior.y + 20, 350, 285};
    recScrollPanel = (Rectangle){AnchorPosterior.x + 10, AnchorPosterior.y + 30, 330, 265};
    ScrollPanelPosteriorsContent.y = recScrollPanel.height - 20;
    recLabelOuter = (Rectangle){AnchorPosterior.x + 10, AnchorPosterior.y + 65, TEXTBOX_SIZE, TEXTBOX_SIZE};
    
    for(int i = 0; i < NUMBER_SECRETS; i++){
        recLabelX[i] = (Rectangle){AnchorPosterior.x + 40, AnchorPosterior.y + 125 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};
    }
    
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        recLabelPosteriors[i] = (Rectangle){AnchorPosterior.x + 75 + i*TEXTBOX_SIZE, AnchorPosterior.y + 45, 20, 20};
        recTextBoxOuter[i] = (Rectangle){AnchorPosterior.x + 65 + i*TEXTBOX_SIZE, AnchorPosterior.y + 65, TEXTBOX_SIZE, TEXTBOX_SIZE};
    }

    for(int i = 0; i < NUMBER_SECRETS; i++){
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            recTextBoxInners[i][j] = (Rectangle){AnchorPosterior.x + 65 + j*TEXTBOX_SIZE, AnchorPosterior.y + 125 + i*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};
        }
    }

    ScrollPanelPosteriorsContent.x = recTextBoxInners[0][2].x + TEXTBOX_SIZE;
}

void GuiPosteriors::resetPosterior(int channel){
    numPosteriors[channel] = NUMBER_SECRETS;

    for(int i = 0; i < NUMBER_SECRETS; i++){
        strcpy(TextBoxOuterText[i], "0");
        for(int j = 0; j < numPosteriors[channel]; j++){
            strcpy(TextBoxInnersText[i][j], "0");
        }
    }
}

void GuiPosteriors::setScrollContent(int channel){
    ScrollPanelPosteriorsContent.x = recTextBoxInners[0][numPosteriors[channel]-1].x - 10 + TEXTBOX_SIZE;
}