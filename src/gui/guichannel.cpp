#include "guichannel.h"

GuiChannel::GuiChannel(){
    // Data
    curChannel = CHANNEL_1;
    for(int i = 0; i < NUMBER_CHANNELS; i++){
        numSecrets[i] = 3;
        numOutputs[i] = 3;
    }

    // Text
    strcpy(LabelChannelTabs[CHANNEL_1], "Ch C");
    strcpy(LabelChannelTabs[CHANNEL_2], "Ch R");
    strcpy(LabelChannelTabs[CHANNEL_3], "Ch CR");

    strcpy(panelChannelText, "Channel C");
    strcpy(LabelOutputsText, "Outputs");
    strcpy(buttonRandomText, "Generate Random");
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        LabelChannelXText[i] = "X" + to_string(i+1);
        LabelChannelYText[i] = "Y" + to_string(i+1);
        LabelChannelZText[i] = "Z" + to_string(i+1);
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
        for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
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

    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
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

bool GuiChannel::checkChannelSpinner(int mode){
    if(SpinnerChannelValue[curChannel] != numOutputs[curChannel]){
        updateChannelBySpinner(curChannel, mode);
        return true;
    }
    return false;
}

void GuiChannel::updateChannelBySpinner(int channel, int mode){
    if(SpinnerChannelValue[channel] <= 0){
        SpinnerChannelValue[channel] = 1;
    }else if(SpinnerChannelValue[channel] < numOutputs[channel]){
        int diff = numOutputs[channel] - SpinnerChannelValue[channel];
        ScrollPanelChannelContent.x = ScrollPanelChannelContent.x - diff*TEXTBOX_SIZE;
    }else{
        for(int i = 0; i < numSecrets[channel]; i++){
            for(int j = numOutputs[channel]; j < SpinnerChannelValue[channel]; j++){
                TextBoxChannelEditMode[i][j] = false;
                strcpy(TextBoxChannelText[channel][i][j], "0");
            }
        }

        ScrollPanelChannelContent.x = ScrollPanelChannelContent.x + (SpinnerChannelValue[channel]-numOutputs[channel])*TEXTBOX_SIZE;
    }

    numOutputs[channel] = SpinnerChannelValue[channel];

    if(mode == MODE_REF){
        if(channel == CHANNEL_1){
            if(numSecrets[CHANNEL_2] < numOutputs[CHANNEL_1]){
                for(int j = 0; j < numOutputs[CHANNEL_2]; j++){
                    for(int i = numSecrets[CHANNEL_2]; i < numOutputs[CHANNEL_1]; i++){
                        TextBoxChannelEditMode[i][j] = false;
                        strcpy(TextBoxChannelText[CHANNEL_2][i][j], "0");
                    }
                }
            }
            numSecrets[CHANNEL_2] = numOutputs[CHANNEL_1];
        }else{
            if(numOutputs[CHANNEL_3] < numOutputs[CHANNEL_2]){
                for(int i = 0; i < numSecrets[CHANNEL_3]; i++){
                    for(int j = numOutputs[CHANNEL_3]; j < numOutputs[CHANNEL_2]; j++){
                        TextBoxChannelEditMode[i][j] = false;
                        strcpy(TextBoxChannelText[CHANNEL_3][i][j], "0");
                    }
                }
            }
            SpinnerChannelValue[CHANNEL_3] = numOutputs[CHANNEL_2];
            numOutputs[CHANNEL_3] = numOutputs[CHANNEL_2];
        }
    }
}

void GuiChannel::updateChannelTextBoxes(vector<vector<long double>> &channel){
    for(int i = 0; i < numSecrets[curChannel]; i++){
        for(int j = 0; j < numOutputs[curChannel]; j++){
            sprintf(TextBoxChannelText[curChannel][i][j], "%.3Lf", channel[i][j]);
        }
    }
}

void GuiChannel::checkModeAndSizes(int mode){
    if(mode == MODE_TWO){
        numSecrets[CHANNEL_2] = 3;
    }else if(mode == MODE_REF){
        updateChannelBySpinner(CHANNEL_1, mode);
        updateChannelBySpinner(CHANNEL_2, mode);
    }
}

void GuiChannel::setScrollContent(){
    ScrollPanelChannelContent.y = layoutRecsScrollPanel.height - 20 + (max(0,numSecrets[curChannel]-3))*TEXTBOX_SIZE; // Secrets
    ScrollPanelChannelContent.x = layoutRecsTextBoxChannel[0][numOutputs[curChannel]-1].x - 10 + TEXTBOX_SIZE; // Outputs
}