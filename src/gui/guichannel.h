#ifndef _guichannel
#define _guichannel

#include <vector>
#include <string>
#include <string.h>
#include "../../libs/raylib/src/raylib.h"
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
    vector<vector<bool>> TextBoxChannelEditMode;
    vector<vector<char*>> TextBoxChannelText;

    // Define control rectangles
    Rectangle layoutRecsGroupBox;    // GroupBox: GroupBoxChannel
    Rectangle layoutRecsSpinner;    // Spinner: SpinnerChannel
    Rectangle layoutRecsScrollPanel;    // ScrollPanel: ScrollPanelChannel
    Rectangle layoutRecsLabelOutputs;    // Label: LabelOutputs
    vector<Rectangle> layoutRecsLabelX;
    vector<Rectangle> layoutRecsLabelY;
    vector<vector<Rectangle>> layoutRecsTextBoxChannel;
};

#endif