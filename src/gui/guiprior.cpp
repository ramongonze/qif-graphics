#include "guiprior.h"

GuiPrior::GuiPrior(){
    // Const text
    strcpy(panPriorTxt, "Prior distribution \u03C0"); // pi symbol &#x3c0;   \u3c0 
    strcpy(buttonRandTxt, "Generate Random");
    for(int i = 0; i < NUMBER_SECRETS; i++){
        labelPriorTxt[i] = "X" + to_string(i+1);
    }
    
    // Define anchors
    anchorPrior = {10, 35};

    // Define controls variables
    for(int i = 0; i < NUMBER_SECRETS; i++){
        tBoxPriorEdit[i] = false;
        strcpy(tBoxPriorTxt[i], "0");
    }

    // Define controls rectangles
    recTitle = (Rectangle){anchorPrior.x, anchorPrior.y, 350, 20};
    recContent = (Rectangle){anchorPrior.x, anchorPrior.y + 20, 350, 120};
    recPan = (Rectangle){anchorPrior.x + 10, anchorPrior.y + 30, recContent.width - 20, recContent.height - 20};

    float xBasePosition = recContent.x + (int)((recContent.width - 3*TEXTBOX_SIZE)/2);
    float yBasePosition = recContent.y + (int)((recContent.height-TEXTBOX_SIZE-20)/2);
    for(int i = 0; i < NUMBER_SECRETS; i++){
        recLabel[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition, TEXTBOX_SIZE, 20};
    }

    for(int i = 0; i < NUMBER_SECRETS; i++){
        recTBox[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition + 20, TEXTBOX_SIZE, TEXTBOX_SIZE};
    }

    recButtonRand = (Rectangle){recTitle.x + recTitle.width - 140, recTitle.y, 140, 20};
}
