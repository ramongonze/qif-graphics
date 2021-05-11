#include "guichannel.h"

GuiChannel::GuiChannel(){
    // Data
    numOutputs = 3;

    // Text
    GroupBoxChannelText = "Channel";    // GROUPBOX: GroupBoxChannel
    LabelOutputsText = "Outputs";    // LABEL: LabelOutputs
    LabelChannelXText = vector<string>({"X1", "X2", "X3"});
    LabelChannelYText = vector<string>({"Y1", "Y2", "Y3"});
    buttonRandomText = "Random";

    // Define anchors
    AnchorChannel = {10, 195};            // ANCHOR ID:2

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
    layoutRecsGroupBox = {AnchorChannel.x + 0, AnchorChannel.y + 0, 350, 275};    // GroupBox: GroupBoxChannel
    layoutRecsSpinner = {AnchorChannel.x + 245, AnchorChannel.y + 15, 90, 25};    // Spinner: SpinnerChannel
    layoutRecsScrollPanel = {AnchorChannel.x + 15, AnchorChannel.y + 55, 320, 205};    // ScrollPanel: ScrollPanelChannel
    ScrollPanelChannelContent.y = layoutRecsScrollPanel.height - 20;
    layoutRecsLabelOutputs = {AnchorChannel.x + 175, AnchorChannel.y + 15, 58, 25};    // Label: LabelOutputs
    layoutRecsLabelX = vector<Rectangle>(3);

    int xBasePosition = layoutRecsScrollPanel.x + 10;
    for(int i = 0; i < layoutRecsLabelX.size(); i++){
        layoutRecsLabelX[i] = (Rectangle){AnchorChannel.x + 40, AnchorChannel.y + 90 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};
    }

    layoutRecsLabelY = vector<Rectangle>(numOutputs);
    for(int i = 0; i < numOutputs; i++){
        layoutRecsLabelY[i] = (Rectangle){AnchorChannel.x + 75 + i*TEXTBOX_SIZE, AnchorChannel.y + 70, 20, 20};
    }

    layoutRecsTextBoxChannel = vector<vector<Rectangle>>(numOutputs, vector<Rectangle>(3));
    for(int i = 0; i < numOutputs; i++){
        for(int j = 0; j < 3; j++){
            layoutRecsTextBoxChannel[i][j] = (Rectangle){AnchorChannel.x + 65 + i*TEXTBOX_SIZE, AnchorChannel.y + 90 + j*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};
        }
    }

    layoutRecsButtonRandom = (Rectangle){AnchorChannel.x + 15, AnchorChannel.y + 15, 70, 28};

    ScrollPanelChannelContent.x = layoutRecsTextBoxChannel[numOutputs-1][0].x + TEXTBOX_SIZE;
}

void GuiChannel::updateChannelBySpinner(){
    if(SpinnerChannelValue <= 0){
        SpinnerChannelValue = 1;
    }else if(SpinnerChannelValue < numOutputs){
        int diff = numOutputs - SpinnerChannelValue;
        for(int i = 0; i < diff; i++){
            layoutRecsTextBoxChannel.pop_back();
            TextBoxChannelEditMode.pop_back();
            layoutRecsLabelY.pop_back();
            LabelChannelYText.pop_back();

            for(int j = 0; j < 3; j++) free(TextBoxChannelText[TextBoxChannelText.size()-1][j]);
            TextBoxChannelText.pop_back();

            ScrollPanelChannelContent.x -= TEXTBOX_SIZE;
        }
    }else{
        for(int i = numOutputs; i < SpinnerChannelValue; i++){
            layoutRecsTextBoxChannel.push_back(vector<Rectangle>(3));
            layoutRecsLabelY.push_back((Rectangle){AnchorChannel.x + 75 + i*TEXTBOX_SIZE, AnchorChannel.y + 70, 20, 20});

            TextBoxChannelEditMode.push_back(vector<bool>(3, false));
            TextBoxChannelText.push_back(vector<char*>(3));
            
            char buffer[5];
            sprintf(buffer, "Y%d", i+1);
            LabelChannelYText.push_back(string(buffer));
            for(int j = 0; j < 3; j++){
                layoutRecsTextBoxChannel[i][j] = (Rectangle){AnchorChannel.x + 65 + (i*TEXTBOX_SIZE), AnchorChannel.y + 90 + (j*TEXTBOX_SIZE), TEXTBOX_SIZE, TEXTBOX_SIZE};
                TextBoxChannelEditMode[i][j] = false;
                TextBoxChannelText[i][j] = (char*) malloc(128*sizeof(char));
                strcpy(TextBoxChannelText[i][j], "0");
            }

            ScrollPanelChannelContent.x += TEXTBOX_SIZE;
        }
    }

    numOutputs = SpinnerChannelValue;
}

void GuiChannel::updateChannelTextBoxes(vector<vector<long double>> &channel){
    char buffer[128];

    for(int i = 0; i < channel.size(); i++){
        for(int j = 0; j < channel[i].size(); j++){
            sprintf(buffer, "%.3Lf", channel[i][j]);
            strcpy(TextBoxChannelText[i][j], buffer);
        }
    }
}