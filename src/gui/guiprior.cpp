#include "guiprior.h"

GuiPrior::GuiPrior(){
    // Const text
    GroupBoxPriorText = (char*) malloc(128*sizeof(char));
    buttonRandomText = (char*) malloc(128*sizeof(char));
    strcpy(GroupBoxPriorText, "Prior distribution");
    strcpy(buttonRandomText, "Random");
    LabelPriorText = vector<string>(3);
    LabelPriorText = {"X1", "X2", "X3"};
    
    // Define anchors
    AnchorPrior = {10, 65};            // ANCHOR ID:1

    // Define controls variables
    TextBoxPriorEditMode = vector<bool>({false, false, false});
    TextBoxPriorText = vector<char*>(3, nullptr);
    for(int i = 0; i < 3; i++){
        TextBoxPriorText[i] = (char*) malloc(128*sizeof(char));
        strcpy(TextBoxPriorText[i], "0");
    }

    // Define controls rectangles
    layoutRecsGroupBox = {AnchorPrior.x + 0, AnchorPrior.y + 0, 350, 110};    // GroupBox: GroupBoxPrior

    float xBasePosition = layoutRecsGroupBox.x + (int)((layoutRecsGroupBox.width - 3*TEXTBOX_SIZE)/2);
    float yBasePosition = layoutRecsGroupBox.y + (int)((layoutRecsGroupBox.height-TEXTBOX_SIZE-10)/2);
    layoutRecsLabel = vector<Rectangle>(3);
    for(int i = 0; i < 3; i++){
        layoutRecsLabel[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition, TEXTBOX_SIZE, 20};    // Label: LabelPriorX
    }

    layoutRecsTextBox = vector<Rectangle>(3);
    for(int i = 0; i < 3; i++){
        layoutRecsTextBox[i] = (Rectangle){xBasePosition + i*TEXTBOX_SIZE, yBasePosition + 20, TEXTBOX_SIZE, TEXTBOX_SIZE};    // TextBox: TextBoxPrior0
    }

    layoutRecsButtonRandom = (Rectangle){AnchorPrior.x + 15, AnchorPrior.y + 15, 70, 28};
}