#include "guiprior.h"

GuiPrior::GuiPrior(){
    // Const text
    GroupBoxPriorText = (char*) malloc(128*sizeof(char));
    buttonRandomText = (char*) malloc(128*sizeof(char));
    strcpy(GroupBoxPriorText, "Prior distribution");
    strcpy(buttonRandomText, "Gen. Random");
    LabelPriorText = vector<string>(3);
    LabelPriorText = {"X1", "X2", "X3"};
    
    // Define anchors
    AnchorPrior = {10, 35};            // ANCHOR ID:1

    // Define controls variables
    TextBoxPriorEditMode = vector<bool>({false, false, false});
    TextBoxPriorText = vector<char*>(3, nullptr);
    for(int i = 0; i < 3; i++){
        TextBoxPriorText[i] = (char*) malloc(128*sizeof(char));
        strcpy(TextBoxPriorText[i], "0");
    }

    // Define controls rectangles
    layoutRecsTitle = (Rectangle){AnchorPrior.x, AnchorPrior.y, 350, 25};
    layoutRecsContent = (Rectangle){AnchorPrior.x, AnchorPrior.y + 20, 350, 120};    // GroupBox: GroupBoxPrior
    layoutRecsPanel = (Rectangle){AnchorPrior.x + 10, AnchorPrior.y + 30, layoutRecsContent.width - 20, layoutRecsContent.height - 20};

    float xBasePosition = layoutRecsContent.x + (int)((layoutRecsContent.width - 3*TEXTBOX_SIZE)/2);
    float yBasePosition = layoutRecsContent.y + (int)((layoutRecsContent.height-TEXTBOX_SIZE-20)/2);
    layoutRecsLabel = vector<Rectangle>(3);
    for(int i = 0; i < 3; i++){
        layoutRecsLabel[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition, TEXTBOX_SIZE, 20};    // Label: LabelPriorX
    }

    layoutRecsTextBox = vector<Rectangle>(3);
    for(int i = 0; i < 3; i++){
        layoutRecsTextBox[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition + 20, TEXTBOX_SIZE, TEXTBOX_SIZE};    // TextBox: TextBoxPrior0
    }

    layoutRecsButtonRandom = (Rectangle){layoutRecsTitle.x + layoutRecsTitle.width - 110, layoutRecsTitle.y, 110, 20};
}