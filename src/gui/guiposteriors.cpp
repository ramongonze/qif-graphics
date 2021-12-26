#include "guiposteriors.h"

GuiPosteriors::GuiPosteriors(){
    // Data
    for(int i = 0; i < NUMBER_CHANNELS; i++)
        numPost[i] = 3;
    
    // Const text
    strcpy(gBoxPostTxt, "Hyper-distribution [\u03C0\u203AC]");
    strcpy(labelOuterTxt, "Outer");

    for(int i = 0; i < NUMBER_SECRETS; i++){
        labelPostXTxt[i] = "X" + to_string(i+1);
    }
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        labelPostTxt[CHANNEL_1][i] = "\u03B4" + to_string(i+1);
        labelPostTxt[CHANNEL_2][i] = "\u03B4\'" + to_string(i+1);
        labelPostTxt[CHANNEL_3][i] = "\u03B4\'" + to_string(i+1);
    }

    // Define anchors
    anchorPost = {10, 480};

    // Define controls variable
    scrPanPostOffset = {0, 0};
    scrPanPostBOffset = {0, 0};

    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        tBoxOuterEdit[i] = false;
        strcpy(tBoxOuterTxt[i], "0");
    }

    for(int i = 0; i < NUMBER_SECRETS; i++){
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            tBoxInnersEdit[i][j] = false;
            strcpy(tBoxInnersTxt[i][j], "0");
        }
    }

    // Define controls rectangles
    recTitle = (Rectangle){anchorPost.x, anchorPost.y, 350, 20};
    recContent = (Rectangle){anchorPost.x, anchorPost.y + 20, 350, 285};
    recScrPan = (Rectangle){anchorPost.x + 10, anchorPost.y + 30, 330, 265};
    scrPanPostContent.y = recScrPan.height - 20;
    recLabelOuter = (Rectangle){anchorPost.x + 10, anchorPost.y + 65, TEXTBOX_SIZE, TEXTBOX_SIZE};
    
    for(int i = 0; i < NUMBER_SECRETS; i++){
        recLabelX[i] = (Rectangle){anchorPost.x + 40, anchorPost.y + 125 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};
    }
    
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        recLabelPost[i] = (Rectangle){anchorPost.x + 75 + i*TEXTBOX_SIZE, anchorPost.y + 45, 20, 20};
        recTBoxOuter[i] = (Rectangle){anchorPost.x + 65 + i*TEXTBOX_SIZE, anchorPost.y + 65, TEXTBOX_SIZE, TEXTBOX_SIZE};
    }

    for(int i = 0; i < NUMBER_SECRETS; i++){
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            recTBoxInners[i][j] = (Rectangle){anchorPost.x + 65 + j*TEXTBOX_SIZE, anchorPost.y + 125 + i*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};
        }
    }

    scrPanPostContent.x = recTBoxInners[0][2].x + TEXTBOX_SIZE;
}

void GuiPosteriors::resetPosterior(int channel){
    numPost[channel] = NUMBER_SECRETS;

    for(int i = 0; i < NUMBER_SECRETS; i++){
        strcpy(tBoxOuterTxt[i], "0");
        for(int j = 0; j < numPost[channel]; j++){
            strcpy(tBoxInnersTxt[i][j], "0");
        }
    }
}

void GuiPosteriors::setScrollContent(int channel){
    scrPanPostContent.x = recTBoxInners[0][numPost[channel]-1].x - 10 + TEXTBOX_SIZE;
}