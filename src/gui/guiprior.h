#ifndef _guiprior
#define _guiprior

#include "../../libs/raylib/src/raylib.h"

class GuiPrior{
private:
    /* data */
public:
    // Const text
    const char *GroupBoxPriorText = "Prior distribution";    // GROUPBOX: GroupBoxPrior
    const char *LabelPriorText[3] = {"X1", "X2", "X3"};    // LABEL: LabelPriorX1

    // Define anchors
    Vector2 AnchorPrior = {10, 65};            // ANCHOR ID:1

    // Define controls variables
    bool TextBoxPriorEditMode[3] = {false, false, false};
    char TextBoxPriorText[3][128] = {"0", "0", "0"};

    // Define controls rectangles
    Rectangle layoutRecsGroupBox = {AnchorPrior.x + 0, AnchorPrior.y + 0, 350, 100};    // GroupBox: GroupBoxPrior

    Rectangle layoutRecsLabel[3] = {
        (Rectangle){AnchorPrior.x + 125, AnchorPrior.y + 20, 20, 20},    // Label: LabelPriorX1
        (Rectangle){AnchorPrior.x + 165, AnchorPrior.y + 20, 20, 20},    // Label: LabelPriorX2
        (Rectangle){AnchorPrior.x + 205, AnchorPrior.y + 20, 20, 20}    // Label: LabelPriorX3
    };

    Rectangle layoutRecsTextBox[3] = {
        (Rectangle){AnchorPrior.x + 115, AnchorPrior.y + 40, 40, 40},    // TextBox: TextBoxPrior0
        (Rectangle){AnchorPrior.x + 155, AnchorPrior.y + 40, 40, 40},    // TextBox: TextBoxPrior1
        (Rectangle){AnchorPrior.x + 195, AnchorPrior.y + 40, 40, 40}    // TextBox: TextBoxPrior2
    };
};

#endif