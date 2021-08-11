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
    int numSecrets[NUMBER_CHANNELS];
    int numOutputs[NUMBER_CHANNELS];

    // Text
    char panelChannelText[CHAR_BUFFER_SIZE];
    char LabelOutputsText[CHAR_BUFFER_SIZE];
    char buttonRandomText[CHAR_BUFFER_SIZE];
    string LabelChannelXText[MAX_CHANNEL_OUTPUTS];
    string LabelChannelYText[MAX_CHANNEL_OUTPUTS];
    string LabelChannelZText[MAX_CHANNEL_OUTPUTS];
    char LabelChannelTabs[NUMBER_CHANNELS][CHAR_BUFFER_SIZE];

    // Define anchors
    Vector2 AnchorChannel;

    // Define controls variables
    bool SpinnerChannelEditMode;
    int SpinnerChannelValue[NUMBER_CHANNELS];
    Vector2 ScrollPanelScrollOffset;
    Vector2 ScrollPanelBoundsOffset;
    Vector2 ScrollPanelContent;
    bool TextBoxChannelEditMode[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS];
    char TextBoxChannelText[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

    // Define control rectangles
    Rectangle recTitle;
    Rectangle recContent;
    Rectangle recSpinner;
    Rectangle recScrollPanel;
    Rectangle recLabelOutputs;
    Rectangle recButtonRandom;
    Rectangle recLabelX[MAX_CHANNEL_OUTPUTS];
    Rectangle recLabelY[MAX_CHANNEL_OUTPUTS];
    Rectangle recTabs[NUMBER_CHANNELS];
    
    // Important: The matrix is indexed by columns x rows
    Rectangle recTextBoxChannel[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS];

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    /* Check wheter the spinner of the current channel has been changed.
       If yes, call updatedChannelBySpinner.
       If no, does nothing.

       @Parameters
            mode: {MODE_SINGLE, MODE_TWO, MODE_REF}

        @Returns
            true if the spinner has been changed and false otherwise.       
    */
    bool checkChannelSpinner(int mode);

    /* If the channel spinner was changed, update the TextBoxChannel matrices
        channel: {0,1,2} current channel.
        mode: {MODE_SINGLE, MODE_TWO, MODE_REF}
    */
    void updateChannelBySpinner(int channel, int mode);
    
    // Update channel textboxes text when the random button is pressed according to current active channel 
    void updateChannelTextBoxes(vector<vector<long double>> &channel);

    // Copy the values of a channel matrix to another one
    static void copyChannelText(char origin[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], char dest[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int numSecrets, int numOutputs){
        for(int i = 0; i < numSecrets; i++){
            for(int j = 0; j < numOutputs; j++){
                strcpy(dest[i][j], origin[i][j]);
            }
        }
    }

    // Check if the current mode and channels sizes are compatible. If not, fix it.
    void checkModeAndSizes(int mode);

    // Set right scroll bounds for the current active channel
    void setScrollContent();

    // Set all values for the given channel to zero
    void resetChannel(int channel);
};

#endif