#include "gui.h"

Gui::Gui(){
    menu = GuiMenu();
    prior = GuiPrior();
    channel = GuiChannel();
    posteriors = GuiPosteriors();
    visualization = GuiVisualization();
}

bool Gui::checkTextBoxPressed(){
    for(int i = 0; i < prior.TextBoxPriorEditMode.size(); i++)
        if(prior.TextBoxPriorEditMode[i] == true)
            return true;

    for(int i = 0; i < channel.TextBoxChannelEditMode.size(); i++)
        for(int j = 0; j < channel.TextBoxChannelEditMode[i].size(); j++)
            if(channel.TextBoxChannelEditMode[i][j] == true)
                return true;

    return false;
}

void Gui::moveAmongTextBoxes(){
    // Prior
    for(int i = 0; i < 3; i++){
        if(prior.TextBoxPriorEditMode[i] == true){
            if(IsKeyPressed(KEY_TAB)){
                prior.TextBoxPriorEditMode[i] = false;
                if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
                    prior.TextBoxPriorEditMode[i] = false;
                    if(i == 0) prior.TextBoxPriorEditMode[2] = true;
                    else if(i == 1) prior.TextBoxPriorEditMode[0] = true;
                    else prior.TextBoxPriorEditMode[1] = true;
                }else{
                    if(i == 0) prior.TextBoxPriorEditMode[1] = true;
                    else if(i == 1) prior.TextBoxPriorEditMode[2] = true;
                    else prior.TextBoxPriorEditMode[0] = true;
                }
            }else if(IsKeyPressed(KEY_LEFT)){
                prior.TextBoxPriorEditMode[i] = false;
                if(i == 0) prior.TextBoxPriorEditMode[2] = true;
                else if(i == 1) prior.TextBoxPriorEditMode[0] = true;
                else prior.TextBoxPriorEditMode[1] = true;
            }else if(IsKeyPressed(KEY_RIGHT)){
                prior.TextBoxPriorEditMode[i] = false;
                if(i == 0) prior.TextBoxPriorEditMode[1] = true;
                else if(i == 1) prior.TextBoxPriorEditMode[2] = true;
                else prior.TextBoxPriorEditMode[0] = true;
            }

            return;
        }
    }

    int nRows = channel.TextBoxChannelEditMode[0].size();
    int nColumns = channel.TextBoxChannelEditMode.size();

    // Channel
    for(int i = 0; i < nColumns; i++){
        for(int j = 0; j < nRows; j++){
            if(channel.TextBoxChannelEditMode[i][j] == true){
                if(IsKeyPressed(KEY_TAB)){
                    channel.TextBoxChannelEditMode[i][j] = false;
                    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
                        if(i == 0 && j == 0){
                            channel.TextBoxChannelEditMode[nColumns-1][nRows-1] = true; // Select the first text box (row 0 and column 0)
                        }else if(i == 0){
                            channel.TextBoxChannelEditMode[nColumns-1][j-1] = true;
                        }else{
                            channel.TextBoxChannelEditMode[i-1][j] = true;
                        }
                        return;
                    }else{
                        if(i == nColumns-1 && j == nRows-1){
                            channel.TextBoxChannelEditMode[0][0] = true; // Select the first text box (row 0 and column 0)
                        }else if(i == nColumns-1){
                            channel.TextBoxChannelEditMode[0][j+1] = true;
                        }else{
                            channel.TextBoxChannelEditMode[i+1][j] = true;
                        }
                        return;
                    }
                }else if(IsKeyPressed(KEY_UP)){
                    if(j > 0){
                        channel.TextBoxChannelEditMode[i][j] = false;
                        channel.TextBoxChannelEditMode[i][j-1] = true;
                    }
                }else if(IsKeyPressed(KEY_DOWN)){
                    if(j < nRows-1){
                        channel.TextBoxChannelEditMode[i][j] = false;
                        channel.TextBoxChannelEditMode[i][j+1] = true;   
                    }
                }else if(IsKeyPressed(KEY_LEFT)){
                    if(i > 0){
                        channel.TextBoxChannelEditMode[i][j] = false;
                        channel.TextBoxChannelEditMode[i-1][j] = true;
                    }
                }else if(IsKeyPressed(KEY_RIGHT)){
                    if(i < nColumns-1){
                        channel.TextBoxChannelEditMode[i][j] = false;
                        channel.TextBoxChannelEditMode[i+1][j] = true;
                    }
                }                
                return;
            }
        }
    }
}