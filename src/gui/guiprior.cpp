#include "guiprior.h"

GuiPrior::GuiPrior(){
    // Const text
    GroupBoxPriorText = "Prior distribution";    // GROUPBOX: GroupBoxPrior
    LabelPriorText = vector<string>({"X1", "X2", "X3"});    // LABEL: LabelPriorX1

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
    layoutRecsGroupBox = {AnchorPrior.x + 0, AnchorPrior.y + 0, 350, 100};    // GroupBox: GroupBoxPrior

    layoutRecsLabel = vector<Rectangle>(3);
    for(int i = 0; i < 3; i++){
        layoutRecsLabel[i] = (Rectangle){AnchorPrior.x + 125 + i*40, AnchorPrior.y + 20, 20, 20};    // Label: LabelPriorX1
    }

    layoutRecsTextBox = vector<Rectangle>(3);
    for(int i = 0; i < 3; i++){
        layoutRecsTextBox[i] = (Rectangle){AnchorPrior.x + 115 + i*40, AnchorPrior.y + 40, 40, 40};    // TextBox: TextBoxPrior0
    }
}