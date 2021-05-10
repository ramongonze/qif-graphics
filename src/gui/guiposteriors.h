#ifndef _guiposteriors
#define _guiposteriors

#include <vector>
#include <string>
#include <string.h>
#include "../../libs/raylib/src/raylib.h"
#include "../graphics.h"
using namespace std;

class GuiPosteriors{
private:
    /* data */
public:
    GuiPosteriors();

    // Data
    int numPosteriors;

    // Const text
    const char *GroupBoxPosteriorsText;    // GROUPBOX: GroupBoxPosteriors
    const char *LabelOuterText;    // LABEL: LabelOuter
    vector<string> LabelPosteriorsText;     // LabelPosteriors
    vector<string> LabelPosteriorsXText;    // LabelPosteriorsX

    // Define anchors
    Vector2 AnchorPosterior;            // ANCHOR ID:4

    // Define controls variable
    Vector2 ScrollPanelPosteriorsScrollOffset;
    Vector2 ScrollPanelPosteriorsBoundsOffset;            // ScrollPanel: ScrollPanelPosteriors
    Vector2 ScrollPanelPosteriorsContent;      // Width and height of scroll panel content
    vector<bool> TextBoxOuterEditMode;
    vector<char*> TextBoxOuterText;

    vector<vector<bool>> TextBoxInnersEditMode;
    vector<vector<char*>> TextBoxInnersText;

    // Define controls rectangles
    Rectangle layoutRecsGroupBox;    // GroupBox: GroupBoxPosteriors
    Rectangle layoutRecsScrollPanel;    // ScrollPanel: ScrollPanelPosteriors
    Rectangle layoutRecsLabelOuter;    // Label: LabelOuter
    vector<Rectangle> layoutRecsLabelPosteriors;
    vector<Rectangle> layoutRecsTextBoxOuter;
    vector<Rectangle> layoutRecsLabelX;
    vector<vector<Rectangle>> layoutRecsTextBoxInners;
};

#endif