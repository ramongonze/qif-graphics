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
    int numPost[NUMBER_CHANNELS];

    // Const text
    char gBoxPostTxt[CHAR_BUFFER_SIZE];
    char labelOuterTxt[CHAR_BUFFER_SIZE];
    string labelPostTxt[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS];
    string labelPostXTxt[NUMBER_SECRETS];

    // Define anchors
    Vector2 anchorPost;

    // Define controls variable
    Vector2 scrPanPostOffset;
    Vector2 scrPanPostBOffset;
    Vector2 scrPanPostContent;
    bool tBoxOuterEdit[MAX_CHANNEL_OUTPUTS];
    char tBoxOuterTxt[MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

    bool tBoxInnersEdit[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS];
    char tBoxInnersTxt[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

    // Define controls rectangles
    Rectangle recTitle;
    Rectangle recContent;
    Rectangle recScrPan;
    Rectangle recLabelOuter;
    Rectangle recLabelPost[MAX_CHANNEL_OUTPUTS];
    Rectangle recTBoxOuter[MAX_CHANNEL_OUTPUTS];
    Rectangle recLabelX[NUMBER_SECRETS];
    Rectangle recTBoxInners[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS];

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    // Reset the number of posterior to 3 and fill with zeros
    void resetPosterior(int channel);

    // Set right scroll bounds for a given channel
    void setScrollContent(int channel);
};

#endif