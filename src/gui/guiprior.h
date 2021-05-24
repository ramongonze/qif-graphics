#ifndef _guiprior
#define _guiprior

#include <vector>
#include <string>
#include <string.h>
#include "../../libs/raylib/src/raylib.h"
#include "../graphics.h"
using namespace std;

class GuiPrior{
private:
    /* data */
public:
    GuiPrior();

    // Const text
    char *GroupBoxPriorText;    // GROUPBOX: GroupBoxPrior
    char *buttonRandomText;
    vector<string> LabelPriorText;    // LABEL: LabelPriorX1

    // Define anchors
    Vector2 AnchorPrior;            // ANCHOR ID:1

    // Define controls variables
    vector<bool> TextBoxPriorEditMode;
    vector<char*> TextBoxPriorText;

    // Define controls rectangles
    Rectangle layoutRecsTitle;
    Rectangle layoutRecsContent;
    Rectangle layoutRecsPanel;
    vector<Rectangle> layoutRecsLabel;
    vector<Rectangle> layoutRecsTextBox;
    Rectangle layoutRecsButtonRandom;
};

#endif