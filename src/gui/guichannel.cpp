#include "guichannel.h"

GuiChannel::GuiChannel(){
    // Data
    numOutputs = 3;

    // Text
    GroupBoxChannelText = (char*) malloc(128*sizeof(char));
    LabelOutputsText = (char*) malloc(128*sizeof(char));
    buttonRandomText = (char*) malloc(128*sizeof(char));
    strcpy(GroupBoxChannelText, "Channel");
    strcpy(LabelOutputsText, "Outputs");
    strcpy(buttonRandomText, "Gen. Random");
    LabelChannelXText = vector<string>(3);
    LabelChannelXText = {"X1", "X2", "X3"};
    LabelChannelYText = vector<string>(3);
    LabelChannelYText = {"Y1", "Y2", "Y3"};

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
    layoutRecsTitle = {AnchorChannel.x, AnchorChannel.y, 350, 25};
    layoutRecsContent = {layoutRecsTitle.x, layoutRecsTitle.y + 20, 350, 265};
    layoutRecsSpinner = {AnchorChannel.x + 10, AnchorChannel.y + 30, 90, 25};    // Spinner: SpinnerChannel
    layoutRecsScrollPanel = {AnchorChannel.x + 10, AnchorChannel.y + 65, 330, 210};    // ScrollPanel: ScrollPanelChannel
    ScrollPanelChannelContent.y = layoutRecsScrollPanel.height - 20;
    layoutRecsLabelOutputs = {AnchorChannel.x + 175, AnchorChannel.y + 5, 78, 25};    // Label: LabelOutputs
    layoutRecsLabelX = vector<Rectangle>(3);

    for(long unsigned int i = 0; i < layoutRecsLabelX.size(); i++){
        layoutRecsLabelX[i] = (Rectangle){AnchorChannel.x + 40, AnchorChannel.y + 100 + i*TEXTBOX_SIZE, 20, TEXTBOX_SIZE};
    }

    layoutRecsLabelY = vector<Rectangle>(numOutputs);
    for(int i = 0; i < numOutputs; i++){
        layoutRecsLabelY[i] = (Rectangle){AnchorChannel.x + 75 + i*TEXTBOX_SIZE, AnchorChannel.y + 80, 20, 20};
    }

    layoutRecsTextBoxChannel = vector<vector<Rectangle>>(numOutputs, vector<Rectangle>(3));
    for(int i = 0; i < numOutputs; i++){
        for(int j = 0; j < 3; j++){
            layoutRecsTextBoxChannel[i][j] = (Rectangle){AnchorChannel.x + 65 + i*TEXTBOX_SIZE, AnchorChannel.y + 100 + j*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};
        }
    }

    layoutRecsButtonRandom = (Rectangle){layoutRecsTitle.x + layoutRecsTitle.width - 110, layoutRecsTitle.y, 110, 20};
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
            layoutRecsLabelY.push_back((Rectangle){AnchorChannel.x + 75 + i*TEXTBOX_SIZE, AnchorChannel.y + 80, 20, 20});

            TextBoxChannelEditMode.push_back(vector<bool>(3, false));
            TextBoxChannelText.push_back(vector<char*>(3));
            
            char buffer[128];
            sprintf(buffer, "Y%d", i+1);
            LabelChannelYText.push_back(string(buffer));
            for(int j = 0; j < 3; j++){
                layoutRecsTextBoxChannel[i][j] = (Rectangle){AnchorChannel.x + 65 + (i*TEXTBOX_SIZE), AnchorChannel.y + 100 + (j*TEXTBOX_SIZE), TEXTBOX_SIZE, TEXTBOX_SIZE};
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

    for(long unsigned int i = 0; i < channel.size(); i++){
        for(long unsigned int j = 0; j < channel[i].size(); j++){
            sprintf(buffer, "%.3Lf", channel[i][j]);
            strcpy(TextBoxChannelText[i][j], buffer);
        }
    }
}