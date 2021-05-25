#include "guiposteriors.h"

GuiPosteriors::GuiPosteriors(){
    // Data
    numPosteriors = 3;
    
    // Const text
    strcpy(GroupBoxPosteriorsText, "Posterior distributions");
    strcpy(LabelOuterText, "Outer");

    for(int i = 0; i < NUMBER_SECRETS; i++){
        LabelPosteriorsXText[i] = "X" + to_string(i);
    }
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        LabelPosteriorsText[i] = "I" + to_string(i+1);
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
    layoutRecsTitle = (Rectangle){AnchorPosterior.x, AnchorPosterior.y, 350, 25};
    layoutRecsContent = (Rectangle){AnchorPosterior.x, AnchorPosterior.y + 20, 350, 285};
    layoutRecsScrollPanel = (Rectangle){AnchorPosterior.x + 10, AnchorPosterior.y + 30, 330, 265};
    ScrollPanelPosteriorsContent.y = layoutRecsScrollPanel.height - 20;
    layoutRecsLabelOuter = (Rectangle){AnchorPosterior.x + 10, AnchorPosterior.y + 65, TEXTBOX_SIZE, TEXTBOX_SIZE};
    
    for(int i = 0; i < NUMBER_SECRETS; i++){
        layoutRecsLabelX[i] = (Rectangle){AnchorPosterior.x + 40, AnchorPosterior.y + 125 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};
    }
    
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        layoutRecsLabelPosteriors[i] = (Rectangle){AnchorPosterior.x + 75 + i*TEXTBOX_SIZE, AnchorPosterior.y + 45, 20, 20};
        layoutRecsTextBoxOuter[i] = (Rectangle){AnchorPosterior.x + 65 + i*TEXTBOX_SIZE, AnchorPosterior.y + 65, TEXTBOX_SIZE, TEXTBOX_SIZE};
    }

    for(int i = 0; i < NUMBER_SECRETS; i++){
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            layoutRecsTextBoxInners[i][j] = (Rectangle){AnchorPosterior.x + 65 + j*TEXTBOX_SIZE, AnchorPosterior.y + 125 + i*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};
        }
    }

    ScrollPanelPosteriorsContent.x = layoutRecsTextBoxInners[0][numPosteriors-1].x + TEXTBOX_SIZE;
}