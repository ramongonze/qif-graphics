#include "guichannel.h"

GuiChannel::GuiChannel(){
    // Data
    numOutputs = 3;

    // Text
    strcpy(panelChannelText, "Channel");
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
    SpinnerChannelValue = numOutputs;
    ScrollPanelChannelScrollOffset = {0, 0};
    ScrollPanelChannelBoundsOffset = {0, 0};
    for(int i = 0; i < NUMBER_SECRETS; i++){
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            TextBoxChannelEditMode[i][j] = false;
            strcpy(TextBoxChannelText[i][j], "0");
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

    layoutRecsButtonRandom = (Rectangle){layoutRecsTitle.x + layoutRecsTitle.width - 140, layoutRecsTitle.y, 140, 20};
    ScrollPanelChannelContent.x = layoutRecsTextBoxChannel[0][numOutputs-1].x + TEXTBOX_SIZE;
}

void GuiChannel::updateChannelBySpinner(){
    if(SpinnerChannelValue <= 0){
        SpinnerChannelValue = 1;
    }else if(SpinnerChannelValue < numOutputs){
        int diff = numOutputs - SpinnerChannelValue;
        ScrollPanelChannelContent.x = ScrollPanelChannelContent.x - diff*TEXTBOX_SIZE;
    }else{
        for(int i = 0; i < NUMBER_SECRETS; i++){
            for(int j = numOutputs; j < SpinnerChannelValue; j++){
                TextBoxChannelEditMode[i][j] = false;
                strcpy(TextBoxChannelText[i][j], "0");
            }
        }

        for(int j = numOutputs; j < SpinnerChannelValue; j++){
            ScrollPanelChannelContent.x += TEXTBOX_SIZE;
        }
    }

    numOutputs = SpinnerChannelValue;
}

void GuiChannel::updateChannelTextBoxes(vector<vector<long double>> &channel){
    for(int i = 0; i < NUMBER_SECRETS; i++){
        for(int j = 0; j < numOutputs; j++){
            sprintf(TextBoxChannelText[i][j], "%.3Lf", channel[i][j]);
        }
    }
}