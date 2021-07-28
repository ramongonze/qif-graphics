#ifndef _guichannel
#define _guichannel

#include <vector>
#include <string>
#include <string.h>
#include "../../libs/raylib/src/raylib.h"
#include "../graphics.h"
using namespace std;

class GuiChannel{
private:
    /* data */
public:
    GuiChannel();

    //------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------
    
    // Data
    int curChannel;
    int numOutputs[NUMBER_CHANNELS];

    // Text
    char panelChannelText[CHAR_BUFFER_SIZE];
    char LabelOutputsText[CHAR_BUFFER_SIZE];
    char buttonRandomText[CHAR_BUFFER_SIZE];
    string LabelChannelXText[NUMBER_SECRETS];
    string LabelChannelYText[MAX_CHANNEL_OUTPUTS];
    char LabelChannelTabs[NUMBER_CHANNELS][CHAR_BUFFER_SIZE];

    // Define anchors
    Vector2 AnchorChannel;

    // Define controls variables
    bool SpinnerChannelEditMode;
    int SpinnerChannelValue[NUMBER_CHANNELS];
    Vector2 ScrollPanelChannelScrollOffset;
    Vector2 ScrollPanelChannelBoundsOffset;
    Vector2 ScrollPanelChannelContent;
    bool TextBoxChannelEditMode[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS];
    char TextBoxChannelText[NUMBER_CHANNELS][NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

    // Define control rectangles
    Rectangle layoutRecsTitle;
    Rectangle layoutRecsContent;
    Rectangle layoutRecsSpinner;
    Rectangle layoutRecsScrollPanel;
    Rectangle layoutRecsLabelOutputs;
    Rectangle layoutRecsButtonRandom;
    Rectangle layoutRecsLabelX[NUMBER_SECRETS];
    Rectangle layoutRecsLabelY[MAX_CHANNEL_OUTPUTS];
    Rectangle layoutRecsTabs[NUMBER_CHANNELS];
    
    // Important: The matrix is indexed by columns x rows
    Rectangle layoutRecsTextBoxChannel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS];

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    /* If the channel spinner was changed, update the TextBoxChannel matrices
        channel: {0,1,2} current channel.
    */
    void updateChannelBySpinner(int channel);
    
    /* Update channel layouts if channel tab was changed.
        prevChannel: Channel the tab was selected in the previous frame.
        curChannel: Channel the tab is selected in the current frame.
    */
    void updateChannelByTab(int prevChannel, int curChannel);

    // Update channel textboxes text when the random button is pressed
    void updateChannelTextBoxes(int curChannel, vector<vector<long double>> &channel);

    // Copy the values of a channel matrix to another one
    static void copyChannelText(char origin[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], char dest[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int numOutputs){
        for(int i = 0; i < NUMBER_SECRETS; i++){
            for(int j = 0; j < numOutputs; j++){
                strcpy(dest[i][j], origin[i][j]);
            }
        }
    }
};

#endif