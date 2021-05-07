#include "guichannel.h"

GuiChannel::GuiChannel(){
    // Data
    numOutputs = 3;

    // Text
    GroupBoxChannelText = "Channel";    // GROUPBOX: GroupBoxChannel
    LabelOutputsText = "Outputs";    // LABEL: LabelOutputs
    LabelChannelXText = vector<string>({"X1", "X2", "X3"});
    LabelChannelYText = vector<string>({"Y1", "Y2", "Y3"});

    // Define anchors
    AnchorChannel = {10, 185};            // ANCHOR ID:2

    // Define controls variables
    SpinnerChannelEditMode = false;
    SpinnerChannelValue = numOutputs;            // Spinner: SpinnerChannel
    ScrollPanelChannelScrollOffset = {0, 0};
    ScrollPanelChannelBoundsOffset = {0, 0};            // ScrollPanel: ScrollPanelChannel
    TextBoxChannelEditMode = vector<vector<bool>>(3, vector<bool>(3, false));
    TextBoxChannelText = vector<vector<char*>>(3, vector<char*>(3, nullptr));
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < numOutputs; j++){
            TextBoxChannelText[i][j] = (char*) malloc(128*sizeof(char));
            strcpy(TextBoxChannelText[i][j], "0");
        }
    }
    
    // Define control rectangles
    layoutRecsGroupBox = {AnchorChannel.x + 0, AnchorChannel.y + 0, 350, 245};    // GroupBox: GroupBoxChannel
    layoutRecsSpinner = {AnchorChannel.x + 245, AnchorChannel.y + 15, 90, 25};    // Spinner: SpinnerChannel
    layoutRecsScrollPanel = {AnchorChannel.x + 15, AnchorChannel.y + 55, 320, 175};    // ScrollPanel: ScrollPanelChannel
    layoutRecsLabelOutputs = {AnchorChannel.x + 175, AnchorChannel.y + 15, 58, 25};    // Label: LabelOutputs
    layoutRecsLabelX = vector<Rectangle>(3);
    for(int i = 0; i < layoutRecsLabelX.size(); i++){
        layoutRecsLabelX[i] = (Rectangle){AnchorChannel.x + 40, AnchorChannel.y + 90 + i*40, 20, 40};
    }

    layoutRecsLabelY = vector<Rectangle>(3);
    for(int i = 0; i < layoutRecsLabelY.size(); i++){
        layoutRecsLabelY[i] = (Rectangle){AnchorChannel.x + 75 + i*40, AnchorChannel.y + 70, 20, 20};
    }

    layoutRecsTextBoxChannel = vector<vector<Rectangle>>(3, vector<Rectangle>(3));
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            layoutRecsTextBoxChannel[i][j] = (Rectangle){AnchorChannel.x + 65 + j*40, AnchorChannel.y + 90 + i*40, 40, 40};
        }
    }
}