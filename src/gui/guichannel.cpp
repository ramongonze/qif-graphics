#include "guichannel.h"

GuiChannel::GuiChannel(){
    // Data
    curChannel = CHANNEL_1;
    for(int i = 0; i < NUMBER_CHANNELS; i++)
        numOutputs[i] = 3;

    // Text
    strcpy(LabelChannelTabs[CHANNEL_1], "Ch C");
    strcpy(LabelChannelTabs[CHANNEL_2], "Ch R");
    strcpy(LabelChannelTabs[CHANNEL_3], "Ch CR");

    strcpy(panelChannelText, "Channel C");
    strcpy(LabelOutputsText, "Outputs");
    strcpy(buttonRandomText, "Generate Random");
    for(int i = 0; i < NUMBER_SECRETS; i++){
        LabelChannelXText[i] = "X" + to_string(i+1);
    }
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        LabelChannelYText[i] = "Y" + to_string(i+1);
    }

    // Define anchors
    AnchorChannel = {10, 185};

    // Define controls variables
    SpinnerChannelEditMode = false;
    
    for(int i = 0; i < NUMBER_CHANNELS; i++)
        SpinnerChannelValue[i] = numOutputs[i];
    
    ScrollPanelChannelScrollOffset = {0, 0};
    ScrollPanelChannelBoundsOffset = {0, 0};
    
    for(int k = 0; k < NUMBER_CHANNELS; k++){
        for(int i = 0; i < NUMBER_SECRETS; i++){
            for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
                TextBoxChannelEditMode[i][j] = false;
                strcpy(TextBoxChannelText[k][i][j], "0");
            }
        }
    }
    
    // Define control rectangles
    layoutRecsTitle = (Rectangle){AnchorChannel.x, AnchorChannel.y, 350, 25};
    layoutRecsContent = (Rectangle){layoutRecsTitle.x, layoutRecsTitle.y + 20, 350, 265};
    layoutRecsSpinner = (Rectangle){AnchorChannel.x + 10, AnchorChannel.y + 30, 90, 25};
    layoutRecsScrollPanel = (Rectangle){AnchorChannel.x + 10, AnchorChannel.y + 65, 330, 210};
    ScrollPanelChannelContent.y = layoutRecsScrollPanel.height - 20;
    layoutRecsLabelOutputs = (Rectangle){AnchorChannel.x + 175, AnchorChannel.y + 5, 78, 25};

    for(int i = 0; i < NUMBER_SECRETS; i++){
        layoutRecsLabelX[i] = (Rectangle){AnchorChannel.x + 40, AnchorChannel.y + 100 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            layoutRecsTextBoxChannel[i][j] = (Rectangle){AnchorChannel.x + 65 + j*TEXTBOX_SIZE, AnchorChannel.y + 100 + i*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};
        }
    }

    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        layoutRecsLabelY[i] = (Rectangle){AnchorChannel.x + 75 + i*TEXTBOX_SIZE, AnchorChannel.y + 80, 20, 20};
    }
    
    layoutRecsTabs[CHANNEL_1] = (Rectangle){layoutRecsTitle.x + 0, layoutRecsTitle.y, 56, 20};
    layoutRecsTabs[CHANNEL_2] = (Rectangle){layoutRecsTitle.x + 57, layoutRecsTitle.y, 56, 20};
    layoutRecsTabs[CHANNEL_3] = (Rectangle){layoutRecsTitle.x + 114, layoutRecsTitle.y, 56, 20};

    layoutRecsButtonRandom = (Rectangle){layoutRecsTitle.x + layoutRecsTitle.width - 140, layoutRecsTitle.y, 140, 20};
    ScrollPanelChannelContent.x = layoutRecsTextBoxChannel[0][numOutputs[CHANNEL_1]-1].x + TEXTBOX_SIZE;
}

void GuiChannel::updateChannelBySpinner(int channel){
    if(SpinnerChannelValue[channel] <= 0){
        SpinnerChannelValue[channel] = 1;
    }else if(SpinnerChannelValue[channel] < numOutputs[channel]){
        int diff = numOutputs[channel] - SpinnerChannelValue[channel];
        ScrollPanelChannelContent.x = ScrollPanelChannelContent.x - diff*TEXTBOX_SIZE;
    }else{
        for(int i = 0; i < NUMBER_SECRETS; i++){
            for(int j = numOutputs[channel]; j < SpinnerChannelValue[channel]; j++){
                TextBoxChannelEditMode[i][j] = false;
                strcpy(TextBoxChannelText[channel][i][j], "0");
            }
        }

        for(int j = numOutputs[channel]; j < SpinnerChannelValue[channel]; j++){
            ScrollPanelChannelContent.x += TEXTBOX_SIZE;
        }
    }

    numOutputs[channel] = SpinnerChannelValue[channel];
}

void GuiChannel::updateChannelByTab(int prevChannel, int curChannel){
    int diff = abs(numOutputs[curChannel] - numOutputs[prevChannel]);
    ScrollPanelChannelContent.x = ScrollPanelChannelContent.x + (numOutputs[curChannel] < numOutputs[prevChannel] ? -1 : 1)*diff*TEXTBOX_SIZE;
}

void GuiChannel::updateChannelTextBoxes(int curChannel, vector<vector<long double>> &channel){
    for(int i = 0; i < NUMBER_SECRETS; i++){
        for(int j = 0; j < numOutputs[curChannel]; j++){
            sprintf(TextBoxChannelText[curChannel][i][j], "%.3Lf", channel[i][j]);
        }
    }
}