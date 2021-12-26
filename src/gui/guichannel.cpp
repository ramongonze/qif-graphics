#include "guichannel.h"

GuiChannel::GuiChannel(){
    // Data
    curChannel = CHANNEL_1;
    for(int i = 0; i < NUMBER_CHANNELS; i++){
        numSecrets[i] = 3;
        numOutputs[i] = 3;
    }

    // Text
    strcpy(labelChannelTabs[CHANNEL_1], "Ch C");
    strcpy(labelChannelTabs[CHANNEL_2], "Ch R");
    strcpy(labelChannelTabs[CHANNEL_3], "Ch CR");

    strcpy(panChannelTxt, "Channel C");
    strcpy(labelOutTxt, "Outputs");
    strcpy(labelEpsilonTxt, "Epsilon (ln):");
    strcpy(labelDeltaTxt, "Delta:");
    strcpy(labelAlphaTxt, "Alpha:");
    strcpy(buttonRandTxt, "Generate Random");
    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        labelChannelXTxt[i] = "X" + to_string(i+1);
        labelChannelYTxt[i] = "Y" + to_string(i+1);
        labelChannelYPTxt[i] = "Y'" + to_string(i+1);
        labelChannelZTxt[i] = "Z" + to_string(i+1);
    }    

    // Define anchors
    anchorChannel = {10, 185};

    // Define controls variables
    spinChannelEdit = false;
    tBoxEpsilonEdit = false;
    tBoxDeltaEdit = false;
    tBoxAlphaEdit = false;
    
    for(int i = 0; i < NUMBER_CHANNELS; i++)
        spinChannelValue[i] = numOutputs[i];
    
    strcpy(tBoxEpsilonValue, "2");
    strcpy(tBoxDeltaValue, "0.2");
    strcpy(tBoxAlphaValue, "0.5");

    scrPanOffset = {0, 0};
    scrPanBOffset = {0, 0};

    for(int k = 0; k < NUMBER_CHANNELS; k++){
        for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
            for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
                tBoxChannelEdit[i][j] = false;
                strcpy(tBoxChannelTxt[k][i][j], "0");
            }
        }
    }

    ddDPActive = MECH_KRR;
    ddDPEdit = false;
    strcpy(ddDPOptions[MECH], "Mechanism: ");
    strcpy(ddDPOptions[MECH_KRR], "k-RR");
    strcpy(ddDPOptions[MECH_GEOMETRIC_TRUNCATED], "Geometric truncated");
    
    // Define control rectangles
    recTitle = (Rectangle){anchorChannel.x, anchorChannel.y, 350, 20};
    recContent = (Rectangle){anchorChannel.x, anchorChannel.y + 20, 350, 265};
    recSpinner = (Rectangle){anchorChannel.x + 10, anchorChannel.y + 30, 90, 25};

    recTBoxEpsilon = (Rectangle){anchorChannel.x + 20, anchorChannel.y + 30, 70, 25};
    recTBoxDelta = (Rectangle){anchorChannel.x + 170, anchorChannel.y + 30, 90, 25};
    recTBoxAlpha = recTBoxEpsilon;

    recScrPan = (Rectangle){anchorChannel.x + 10, anchorChannel.y + 65, 330, 210};
    scrPanContent.y = recScrPan.height - 20;
    recLabelOut = (Rectangle){anchorChannel.x + 175, anchorChannel.y + 5, 78, 25};

    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        recLabelX[i] = (Rectangle){anchorChannel.x + 40, anchorChannel.y + 100 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};
        for(int j = 0; j < MAX_CHANNEL_OUTPUTS; j++){
            recTBoxChannel[i][j] = (Rectangle){anchorChannel.x + 65 + j*TEXTBOX_SIZE, anchorChannel.y + 100 + i*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};
        }
    }

    for(int i = 0; i < MAX_CHANNEL_OUTPUTS; i++){
        recLabelY[i] = (Rectangle){anchorChannel.x + 80 + i*TEXTBOX_SIZE, anchorChannel.y + 80, 20, 20};
    }
    
    recTabs[CHANNEL_1] = (Rectangle){anchorChannel.x + 0, anchorChannel.y, 56, 20};
    recTabs[CHANNEL_2] = (Rectangle){anchorChannel.x + 57, anchorChannel.y, 56, 20};
    recTabs[CHANNEL_3] = (Rectangle){anchorChannel.x + 114, anchorChannel.y, 56, 20};

    recButtonRand = (Rectangle){recTitle.x + recTitle.width - 140, recTitle.y, 140, 20};
    scrPanContent.x = recTBoxChannel[0][numOutputs[CHANNEL_1]-1].x + TEXTBOX_SIZE;
}

bool GuiChannel::checkChannelSpinner(int mode){
    if(spinChannelValue[curChannel] != numOutputs[curChannel] && !spinChannelEdit){
        updateChannelBySpinner(curChannel, mode);
        return true;
    }
    return false;
}

void GuiChannel::updateChannelBySpinner(int channel, int mode){
    if(spinChannelValue[channel] <= 0){
        spinChannelValue[channel] = 1;
    }else if(spinChannelValue[channel] > 50){
        spinChannelValue[channel] = 50;
    }else if(spinChannelValue[channel] < numOutputs[channel]){
        int diff = numOutputs[channel] - spinChannelValue[channel];
        scrPanContent.x = scrPanContent.x - diff*TEXTBOX_SIZE;
    }else{
        for(int i = 0; i < numSecrets[channel]; i++){
            for(int j = numOutputs[channel]; j < spinChannelValue[channel]; j++){
                tBoxChannelEdit[i][j] = false;
                strcpy(tBoxChannelTxt[channel][i][j], "0");
            }
        }

        scrPanContent.x = scrPanContent.x + (spinChannelValue[channel]-numOutputs[channel])*TEXTBOX_SIZE;
    }

    numOutputs[channel] = spinChannelValue[channel];

    if(mode == MODE_REF){
        if(channel == CHANNEL_1){
            if(numSecrets[CHANNEL_2] < numOutputs[CHANNEL_1]){
                for(int j = 0; j < numOutputs[CHANNEL_2]; j++){
                    for(int i = numSecrets[CHANNEL_2]; i < numOutputs[CHANNEL_1]; i++){
                        tBoxChannelEdit[i][j] = false;
                        strcpy(tBoxChannelTxt[CHANNEL_2][i][j], "0");
                    }
                }
            }
            numSecrets[CHANNEL_2] = numOutputs[CHANNEL_1];
        }else{
            if(numOutputs[CHANNEL_3] < numOutputs[CHANNEL_2]){
                for(int i = 0; i < numSecrets[CHANNEL_3]; i++){
                    for(int j = numOutputs[CHANNEL_3]; j < numOutputs[CHANNEL_2]; j++){
                        tBoxChannelEdit[i][j] = false;
                        strcpy(tBoxChannelTxt[CHANNEL_3][i][j], "0");
                    }
                }
            }
            spinChannelValue[CHANNEL_3] = numOutputs[CHANNEL_2];
            numOutputs[CHANNEL_3] = numOutputs[CHANNEL_2];
        }
    }
}

void GuiChannel::updateChannelTextBoxes(vector<vector<long double>> &channel){
    for(int i = 0; i < numSecrets[curChannel]; i++){
        for(int j = 0; j < numOutputs[curChannel]; j++){
            sprintf(tBoxChannelTxt[curChannel][i][j], "%.3Lf", channel[i][j]);
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
    scrPanContent.y = recScrPan.height - 20 + (max(0,numSecrets[curChannel]-3))*TEXTBOX_SIZE; // Secrets
    scrPanContent.x = recTBoxChannel[0][numOutputs[curChannel]-1].x - 10 + TEXTBOX_SIZE; // Outputs
}

void GuiChannel::resetChannel(int channel){
    for(int i = 0; i < numSecrets[channel]; i++)
        for(int j = 0; j < numOutputs[channel]; j++)
            strcpy(tBoxChannelTxt[channel][i][j], "0");
}

void GuiChannel::updateDDDP(){
    string newText = string(ddDPOptions[MECH]);
    newText = newText + " " + string(ddDPOptions[ddDPActive]);
    for(int i = 1; i < NUM_MECH+1; i++)
        newText = newText + ";" + ddDPOptions[i];
    strcpy(ddDPTxt, newText.c_str());
}