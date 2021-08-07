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
    int numPosteriors[NUMBER_CHANNELS];

    // Const text
    char GroupBoxPosteriorsText[CHAR_BUFFER_SIZE];
    char LabelOuterText[CHAR_BUFFER_SIZE];
    string LabelPosteriorsText[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS];
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
    Rectangle recTitle;
    Rectangle recContent;
    Rectangle recScrollPanel;
    Rectangle recLabelOuter;
    Rectangle recLabelPosteriors[MAX_CHANNEL_OUTPUTS];
    Rectangle recTextBoxOuter[MAX_CHANNEL_OUTPUTS];
    Rectangle recLabelX[NUMBER_SECRETS];
    Rectangle recTextBoxInners[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS];

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    // Reset the number of posterior to 3 and fill with zeros
    void resetPosterior(int channel);

    // Set right scroll bounds for a given channel
    void setScrollContent(int channel);
};

#endif