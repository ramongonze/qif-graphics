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
    strcpy(LabelEpsilonText, "Epsilon (ln):");
    strcpy(LabelDeltaText, "Delta:");
    strcpy(LabelAlphaText, "Alpha:");
    strcpy(buttonRandomText, "Generate Random");
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        LabelChannelXText[i] = "X" + to_string(i+1);
        LabelChannelYText[i] = "Y" + to_string(i+1);
        LabelChannelYPText[i] = "Y'" + to_string(i+1);
        LabelChannelZText[i] = "Z" + to_string(i+1);
    }    

    // Define anchors
    AnchorChannel = {10, 185};

    // Define controls variables
    SpinnerChannelEditMode = false;
    TextBoxEpsilonEditMode = false;
    TextBoxDeltaEditMode = false;
    TextBoxAlphaEditMode = false;
    
    for(int i = 0; i < NUMBER_CHANNELS; i++)
        SpinnerChannelValue[i] = numOutputs[i];
    
    strcpy(TextBoxEpsilonValue, "2");
    strcpy(TextBoxDeltaValue, "0.2");
    strcpy(TextBoxAlphaValue, "0.5");

    ScrollPanelScrollOffset = {0, 0};
    ScrollPanelBoundsOffset = {0, 0};

    for(int k = 0; k < NUMBER_CHANNELS; k++){
        for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
            for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
                TextBoxChannelEditMode[i][j] = false;
                strcpy(TextBoxChannelText[k][i][j], "0");
            }
        }
    }

    dropdownBoxDPMechanismActive = MECH_KRR;
    dropdownBoxDPMechanismEditMode = false;
    strcpy(dropdownBoxDPMechanismOptions[MECH], "Mechanism: ");
    strcpy(dropdownBoxDPMechanismOptions[MECH_KRR], "k-RR");
    strcpy(dropdownBoxDPMechanismOptions[MECH_GEOMETRIC_TRUNCATED], "Geometric truncated");
    
    // Define control rectangles
    recTitle = (Rectangle){AnchorChannel.x, AnchorChannel.y, 350, 20};
    recContent = (Rectangle){AnchorChannel.x, AnchorChannel.y + 20, 350, 265};
    recSpinner = (Rectangle){AnchorChannel.x + 10, AnchorChannel.y + 30, 90, 25};

    recTextBoxEpsilon = (Rectangle){AnchorChannel.x + 20, AnchorChannel.y + 30, 70, 25};
    recTextBoxDelta = (Rectangle){AnchorChannel.x + 170, AnchorChannel.y + 30, 90, 25};
    recTextBoxAlpha = recTextBoxEpsilon;

    recScrollPanel = (Rectangle){AnchorChannel.x + 10, AnchorChannel.y + 65, 330, 210};
    ScrollPanelContent.y = recScrollPanel.height - 20;
    recLabelOutputs = (Rectangle){AnchorChannel.x + 175, AnchorChannel.y + 5, 78, 25};

    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        recLabelX[i] = (Rectangle){AnchorChannel.x + 40, AnchorChannel.y + 100 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            recTextBoxChannel[i][j] = (Rectangle){AnchorChannel.x + 65 + j*TEXTBOX_SIZE, AnchorChannel.y + 100 + i*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};
        }
    }

    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        recLabelY[i] = (Rectangle){AnchorChannel.x + 80 + i*TEXTBOX_SIZE, AnchorChannel.y + 80, 20, 20};
    }
    
    recTabs[CHANNEL_1] = (Rectangle){AnchorChannel.x + 0, AnchorChannel.y, 56, 20};
    recTabs[CHANNEL_2] = (Rectangle){AnchorChannel.x + 57, AnchorChannel.y, 56, 20};
    recTabs[CHANNEL_3] = (Rectangle){AnchorChannel.x + 114, AnchorChannel.y, 56, 20};

    recButtonRandom = (Rectangle){recTitle.x + recTitle.width - 140, recTitle.y, 140, 20};
    ScrollPanelContent.x = recTextBoxChannel[0][numOutputs[CHANNEL_1]-1].x + TEXTBOX_SIZE;
}

bool GuiChannel::checkChannelSpinner(int mode){
    if(SpinnerChannelValue[curChannel] != numOutputs[curChannel] && !SpinnerChannelEditMode){
        updateChannelBySpinner(curChannel, mode);
        return true;
    }
    return false;
}

void GuiChannel::updateChannelBySpinner(int channel, int mode){
    if(SpinnerChannelValue[channel] <= 0){
        SpinnerChannelValue[channel] = 1;
    }else if(SpinnerChannelValue[channel] > 50){
        SpinnerChannelValue[channel] = 50;
    }else if(SpinnerChannelValue[channel] < numOutputs[channel]){
        int diff = numOutputs[channel] - SpinnerChannelValue[channel];
        ScrollPanelContent.x = ScrollPanelContent.x - diff*TEXTBOX_SIZE;
    }else{
        for(int i = 0; i < numSecrets[channel]; i++){
            for(int j = numOutputs[channel]; j < SpinnerChannelValue[channel]; j++){
                TextBoxChannelEditMode[i][j] = false;
                strcpy(TextBoxChannelText[channel][i][j], "0");
            }
        }

        ScrollPanelContent.x = ScrollPanelContent.x + (SpinnerChannelValue[channel]-numOutputs[channel])*TEXTBOX_SIZE;
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
    }else if(mode == MODE_DP_SINGLE){
        numSecrets[CHANNEL_1] = 3;
    }
}

void GuiChannel::setScrollContent(){
    ScrollPanelContent.y = recScrollPanel.height - 20 + (max(0,numSecrets[curChannel]-3))*TEXTBOX_SIZE; // Secrets
    ScrollPanelContent.x = recTextBoxChannel[0][numOutputs[curChannel]-1].x - 10 + TEXTBOX_SIZE; // Outputs
}

void GuiChannel::resetChannel(int channel){
    for(int i = 0; i < numSecrets[channel]; i++)
        for(int j = 0; j < numOutputs[channel]; j++)
            strcpy(TextBoxChannelText[channel][i][j], "0");
}

void GuiChannel::updateDropwdownDPMechanism(){
    string newText = string(dropdownBoxDPMechanismOptions[MECH]);
    newText = newText + " " + string(dropdownBoxDPMechanismOptions[dropdownBoxDPMechanismActive]);
    for(int i = 1; i < NUM_MECH+1; i++)
        newText = newText + ";" + dropdownBoxDPMechanismOptions[i];
    strcpy(dropdownBoxDPMechanismText, newText.c_str());
}