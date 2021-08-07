#include "guiprior.h"

GuiPrior::GuiPrior(){
    // Const text
    strcpy(panelPriorText, "Prior distribution \u03C0"); // pi symbol &#x3c0;   \u3c0 
    strcpy(buttonRandomText, "Generate Random");
    for(int i = 0; i < NUMBER_SECRETS; i++){
        LabelPriorText[i] = "X" + to_string(i+1);
    }
    
    // Define anchors
    AnchorPrior = {10, 35};

    // Define controls variables
    for(int i = 0; i < NUMBER_SECRETS; i++){
        TextBoxPriorEditMode[i] = false;
        strcpy(TextBoxPriorText[i], "0");
    }

    // Define controls rectangles
    recTitle = (Rectangle){AnchorPrior.x, AnchorPrior.y, 350, 20};
    recContent = (Rectangle){AnchorPrior.x, AnchorPrior.y + 20, 350, 120};
    recPanel = (Rectangle){AnchorPrior.x + 10, AnchorPrior.y + 30, recContent.width - 20, recContent.height - 20};

    float xBasePosition = recContent.x + (int)((recContent.width - 3*TEXTBOX_SIZE)/2);
    float yBasePosition = recContent.y + (int)((recContent.height-TEXTBOX_SIZE-20)/2);
    for(int i = 0; i < NUMBER_SECRETS; i++){
        recLabel[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition, TEXTBOX_SIZE, 20};
    }

    for(int i = 0; i < NUMBER_SECRETS; i++){
        recTextBox[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition + 20, TEXTBOX_SIZE, TEXTBOX_SIZE};
    }

    recButtonRandom = (Rectangle){recTitle.x + recTitle.width - 140, recTitle.y, 140, 20};
}
