#include "gui.h"

Gui::Gui(){
    menu = GuiMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
    prior = GuiPrior();
    channel = GuiChannel();
    posteriors = GuiPosteriors();
    visualization = GuiVisualization();
    drawing = false;
}

bool Gui::checkTextBoxPressed(){
    for(int i = 0; i < NUMBER_SECRETS; i++)
        if(prior.TextBoxPriorEditMode[i] == true)
            return true;

    for(int i = 0; i < NUMBER_SECRETS; i++)
        for(int j = 0; j < channel.numOutputs; j++)
            if(channel.TextBoxChannelEditMode[i][j] == true)
                return true;

    return false;
}

void Gui::moveAmongTextBoxes(){
    // Prior
    for(int i = 0; i < NUMBER_SECRETS; i++){
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

    int nRows = NUMBER_SECRETS;
    int nColumns = channel.numOutputs;

    // Channel
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nColumns; j++){
            if(channel.TextBoxChannelEditMode[i][j] == true){
                channel.TextBoxChannelEditMode[i][j] = false;
                if(IsKeyPressed(KEY_TAB)){
                    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
                        if(j > 0){
                            channel.TextBoxChannelEditMode[i][j-1] = true;
                        }else if(i > 0){
                            channel.TextBoxChannelEditMode[i-1][nColumns-1] = true;
                        }else{
                            channel.TextBoxChannelEditMode[nRows-1][nColumns-1] = true;
                        }
                    }else{
                        if(j < nColumns-1){
                            channel.TextBoxChannelEditMode[i][j+1] = true;
                        }else if(i < nRows-1){
                            channel.TextBoxChannelEditMode[i+1][0] = true;
                        }else{
                            channel.TextBoxChannelEditMode[0][0] = true;
                        }
                    }
                }else if(IsKeyPressed(KEY_UP) && i > 0){
                    channel.TextBoxChannelEditMode[i-1][j] = true;
                }else if(IsKeyPressed(KEY_DOWN) && i < nRows-1){
                    channel.TextBoxChannelEditMode[i+1][j] = true;   
                }else if(IsKeyPressed(KEY_LEFT) && j > 0){
                    channel.TextBoxChannelEditMode[i][j-1] = true;
                }else if(IsKeyPressed(KEY_RIGHT) && j < nColumns-1){
                    channel.TextBoxChannelEditMode[i][j+1] = true;
                }else{
                    channel.TextBoxChannelEditMode[i][j] = true;
                }
                return;
            }
        }
    }
}

void Gui::updatePrior(Distribution &prior_, Circle &priorCircle){
    vector<string> truncPrior = getStrTruncatedDist(prior_, PROB_PRECISION);
    for(int i = 0; i < NUMBER_SECRETS; i++){
        strcpy(prior.TextBoxPriorText[i], truncPrior[i].c_str());
    }

    updatePriorRectangle(priorCircle);
}

void Gui::updatePriorRectangle(Circle &priorCircle){
    visualization.layoutRecsLabelPriorCircle = (Rectangle) {
        (float) priorCircle.center.x - 8,
        (float) priorCircle.center.y - 15,
        30,
        30
    };
}

void Gui::updatePosteriors(Hyper &hyper, Circle innersCircles[MAX_CHANNEL_OUTPUTS]){
    // Match the number of columns in hyper and layout variables
    if(hyper.num_post > posteriors.numPosteriors){
        for(int i = 0; i < NUMBER_SECRETS; i++){
            for(int j = posteriors.numPosteriors; j < hyper.num_post; j++){
                posteriors.TextBoxInnersEditMode[i][j] = false;
            }
        }

        for(int i = posteriors.numPosteriors; i < hyper.num_post; i++){
            posteriors.TextBoxOuterEditMode[i] = false;
        }
    }

    // Update gui items
    posteriors.numPosteriors = hyper.num_post;
    posteriors.ScrollPanelPosteriorsContent.x = posteriors.layoutRecsTextBoxInners[0][posteriors.numPosteriors-1].x + TEXTBOX_SIZE;

    // Update values 
    vector<string> truncDist = getStrTruncatedDist(hyper.outer, PROB_PRECISION);
    for(int i = 0; i < hyper.num_post; i++){
        strcpy(posteriors.TextBoxOuterText[i], truncDist[i].c_str());
    }

    for(int i = 0; i < hyper.num_post; i++){
        vector<long double> inner = vector<long double>(NUMBER_SECRETS);
        for(int j = 0; j < NUMBER_SECRETS; j++){
            inner[j] = hyper.inners[j][i];
        }
        Distribution innerDist = Distribution(inner);
        vector<string> truncDist = getStrTruncatedDist(innerDist, PROB_PRECISION);
        for(int j = 0; j < NUMBER_SECRETS; j++){
            strcpy(posteriors.TextBoxInnersText[j][i], truncDist[j].c_str());
        }
    }

    // Update circle labels and rectangles
    for(int i = 0; i < hyper.num_post; i++){
        visualization.layoutRecsLabelInnersCircles[i] = (Rectangle){
            (float) innersCircles[i].center.x - 8,
            (float) innersCircles[i].center.y - 11,
            20,
            20
        };
    } 
}
