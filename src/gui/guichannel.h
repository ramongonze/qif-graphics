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

    // Data
    int numOutputs;

    // Text
    const char *GroupBoxChannelText;    // GROUPBOX: GroupBoxChannel
    const char *LabelOutputsText;    // LABEL: LabelOutputs
    vector<string> LabelChannelXText;
    vector<string> LabelChannelYText;

    // Define anchors
    Vector2 AnchorChannel;            // ANCHOR ID:2

    // Define controls variables
    bool SpinnerChannelEditMode;
    int SpinnerChannelValue;            // Spinner: SpinnerChannel
    Vector2 ScrollPanelChannelScrollOffset;
    Vector2 ScrollPanelChannelBoundsOffset;            // ScrollPanel: ScrollPanelChannel
    Vector2 ScrollPanelChannelContent;      // Width and height of scroll panel content
    vector<vector<bool>> TextBoxChannelEditMode;
    vector<vector<char*>> TextBoxChannelText;

    // Define control rectangles
    Rectangle layoutRecsGroupBox;    // GroupBox: GroupBoxChannel
    Rectangle layoutRecsSpinner;    // Spinner: SpinnerChannel
    Rectangle layoutRecsScrollPanel;    // ScrollPanel: ScrollPanelChannel
    Rectangle layoutRecsLabelOutputs;    // Label: LabelOutputs
    vector<Rectangle> layoutRecsLabelX;
    vector<Rectangle> layoutRecsLabelY;
    
    // Important: The matrix is indexed by columns x rows
    vector<vector<Rectangle>> layoutRecsTextBoxChannel;

    // Methods
    
    // If the channel spinner was changed, update the TextBoxChannel matrices
    void updateChannelBySpinner();
};

#endif