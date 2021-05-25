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

    //------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------

    // Data
    int numPosteriors;

    // Const text
    char GroupBoxPosteriorsText[CHAR_BUFFER_SIZE];
    char LabelOuterText[CHAR_BUFFER_SIZE];
    string LabelPosteriorsText[MAX_CHANNEL_OUTPUTS];
    string LabelPosteriorsXText[NUMBER_SECRETS];

    // Define anchors
    Vector2 AnchorPosterior;

    // Define controls variable
    Vector2 ScrollPanelPosteriorsScrollOffset;
    Vector2 ScrollPanelPosteriorsBoundsOffset;
    Vector2 ScrollPanelPosteriorsContent;
    bool TextBoxOuterEditMode[MAX_CHANNEL_OUTPUTS];
    char TextBoxOuterText[MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

    bool TextBoxInnersEditMode[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS];
    char TextBoxInnersText[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

    // Define controls rectangles
    Rectangle layoutRecsTitle;
    Rectangle layoutRecsContent;
    Rectangle layoutRecsScrollPanel;
    Rectangle layoutRecsLabelOuter;
    Rectangle layoutRecsLabelPosteriors[MAX_CHANNEL_OUTPUTS];
    Rectangle layoutRecsTextBoxOuter[MAX_CHANNEL_OUTPUTS];
    Rectangle layoutRecsLabelX[NUMBER_SECRETS];
    Rectangle layoutRecsTextBoxInners[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS];
};

#endif