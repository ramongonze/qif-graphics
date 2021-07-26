#include "guiprior.h"

GuiPrior::GuiPrior(){
    // Const text
    strcpy(panelPriorText, "Prior distribution"); // pi symbol &#x3c0;   \u3c0 
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
    layoutRecsTitle = (Rectangle){AnchorPrior.x, AnchorPrior.y, 350, 25};
    layoutRecsContent = (Rectangle){AnchorPrior.x, AnchorPrior.y + 20, 350, 120};
    layoutRecsPanel = (Rectangle){AnchorPrior.x + 10, AnchorPrior.y + 30, layoutRecsContent.width - 20, layoutRecsContent.height - 20};

    float xBasePosition = layoutRecsContent.x + (int)((layoutRecsContent.width - 3*TEXTBOX_SIZE)/2);
    float yBasePosition = layoutRecsContent.y + (int)((layoutRecsContent.height-TEXTBOX_SIZE-20)/2);
    for(int i = 0; i < NUMBER_SECRETS; i++){
        layoutRecsLabel[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition, TEXTBOX_SIZE, 20};
    }

    for(int i = 0; i < NUMBER_SECRETS; i++){
        layoutRecsTextBox[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition + 20, TEXTBOX_SIZE, TEXTBOX_SIZE};
    }

    layoutRecsButtonRandom = (Rectangle){layoutRecsTitle.x + layoutRecsTitle.width - 140, layoutRecsTitle.y, 140, 20};
}
