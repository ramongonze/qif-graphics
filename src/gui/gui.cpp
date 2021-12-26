#include "gui.h"

Gui::Gui(){
    menu = GuiMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
    prior = GuiPrior();
    channel = GuiChannel();
    posteriors = GuiPosteriors();
    visualization = GuiVisualization();
    drawing = false;
    showLabels = true;
    showConvexHull = false;
    readFonts();

    for(int i = 0; i < 3; i++)
        helpMessagesActive[i] = false;

    strcpy(helpMessages[HELP_MSG_BUTTON_PRIOR], "Click to generate a random prior distribution");
    strcpy(helpMessages[HELP_MSG_BUTTON_CHANNEL], "Click to generate a random channel with the current number of outputs");
    strcpy(helpMessages[HELP_MSG_BUTTON_DRAW], "If prior and channel were set, click to draw the geometric representation of prior and hyper distributions");
}

void Gui::readFonts(){
    int chars[260];
    
    // Ascii characters
    for(int i = 0; i < 256; i++)
        chars[i] = i;

    // Special symbols
    int specialSymbols[4] = {960, 948, 8250, 8226}; // pi, delta, ›, bullet
    for(int i = 0; i < 4; i++)
        chars[i+256] = specialSymbols[i];    

    defaultFont = LoadFontEx("fonts/OpenSans-Regular.ttf", 20, chars, 260); // Used to get pi symbol
    defaultFontBig = LoadFontEx("fonts/OpenSans-Regular.ttf", 32, chars, 260); // Used to get pi symbol
}

bool Gui::checkPriorTextBoxPressed(){
    for(int i = 0; i < NUMBER_SECRETS; i++)
        if(prior.tBoxPriorEdit[i] == true)
            return true;

    return false;
}

bool Gui::checkChannelTextBoxPressed(){
    for(int i = 0; i < channel.numSecrets[channel.curChannel]; i++)
        for(int j = 0; j < channel.numOutputs[channel.curChannel]; j++)
            if(channel.tBoxChannelEdit[i][j] == true)
                return true;

    return false;
}

bool Gui::checkEpsilonDeltaTextBoxPressed(){
    return channel.tBoxEpsilonEdit || channel.tBoxDeltaEdit;
}

bool Gui::checkAlphaTextBoxPressed(){
    return channel.tBoxAlphaEdit;
}

void Gui::moveAmongPriorTextBoxes(){
    for(int i = 0; i < NUMBER_SECRETS; i++){
        if(prior.tBoxPriorEdit[i] == true){
            if(IsKeyPressed(KEY_TAB)){
                prior.tBoxPriorEdit[i] = false;
                if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
                    prior.tBoxPriorEdit[i] = false;
                    if(i == 0) prior.tBoxPriorEdit[2] = true;
                    else if(i == 1) prior.tBoxPriorEdit[0] = true;
                    else prior.tBoxPriorEdit[1] = true;
                }else{
                    if(i == 0) prior.tBoxPriorEdit[1] = true;
                    else if(i == 1) prior.tBoxPriorEdit[2] = true;
                    else prior.tBoxPriorEdit[0] = true;
                }
            }else if(IsKeyPressed(KEY_LEFT)){
                prior.tBoxPriorEdit[i] = false;
                if(i == 0) prior.tBoxPriorEdit[2] = true;
                else if(i == 1) prior.tBoxPriorEdit[0] = true;
                else prior.tBoxPriorEdit[1] = true;
            }else if(IsKeyPressed(KEY_RIGHT)){
                prior.tBoxPriorEdit[i] = false;
                if(i == 0) prior.tBoxPriorEdit[1] = true;
                else if(i == 1) prior.tBoxPriorEdit[2] = true;
                else prior.tBoxPriorEdit[0] = true;
            }
            return;
        }
    }
}

void Gui::moveAmongChannelTextBoxes(){
    int nRows = channel.numSecrets[channel.curChannel];
    int nColumns = channel.numOutputs[channel.curChannel];

    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nColumns; j++){
            if(channel.tBoxChannelEdit[i][j] == true){
                channel.tBoxChannelEdit[i][j] = false;
                if(IsKeyPressed(KEY_TAB)){
                    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
                        if(j > 0){
                            channel.tBoxChannelEdit[i][j-1] = true;
                        }else if(i > 0){
                            channel.tBoxChannelEdit[i-1][nColumns-1] = true;
                        }else{
                            channel.tBoxChannelEdit[nRows-1][nColumns-1] = true;
                        }
                    }else{
                        if(j < nColumns-1){
                            channel.tBoxChannelEdit[i][j+1] = true;
                        }else if(i < nRows-1){
                            channel.tBoxChannelEdit[i+1][0] = true;
                        }else{
                            channel.tBoxChannelEdit[0][0] = true;
                        }
                    }
                }else if(IsKeyPressed(KEY_UP) && i > 0){
                    channel.tBoxChannelEdit[i-1][j] = true;
                }else if(IsKeyPressed(KEY_DOWN) && i < nRows-1){
                    channel.tBoxChannelEdit[i+1][j] = true;   
                }else if(IsKeyPressed(KEY_LEFT) && j > 0){
                    channel.tBoxChannelEdit[i][j-1] = true;
                }else if(IsKeyPressed(KEY_RIGHT) && j < nColumns-1){
                    channel.tBoxChannelEdit[i][j+1] = true;
                }else{
                    channel.tBoxChannelEdit[i][j] = true;
                }
                return;
            }
        }
    }
}

void Gui::updatePriorTextBoxes(Distribution &prior_){
    vector<string> truncPrior = getStrTruncatedDist(prior_, PROB_PRECISION);
    for(int i = 0; i < prior_.num_el; i++){
        strcpy(prior.tBoxPriorTxt[i], truncPrior[i].c_str());
    }
}

void Gui::updateChannelTextBoxes(Channel &channel_, int channelIdx){
    for(int i = 0; i < channel_.prior.num_el; i++){
        vector<long double> row = vector<long double>(channel_.num_out);
        for(int j = 0; j < channel_.num_out; j++){
            row[j] = channel_.matrix[i][j];
        }
        Distribution rowDist = Distribution(row);
        vector<string> truncDist = getStrTruncatedDist(rowDist, PROB_PRECISION);
        for(int j = 0; j < channel_.num_out; j++){
            strcpy(channel.tBoxChannelTxt[channelIdx][i][j], truncDist[j].c_str());
        }
    }
}

void Gui::updateHyperTextBoxes(Hyper &hyper, int channel, bool ready){
    // If hyper is not ready, fill textboxes with zeros
    if(!ready){
        for(int i = 0; i < NUMBER_SECRETS; i++){
            strcpy(posteriors.TextBoxOuterText[i], "0");    
            for(int j = 0; j < NUMBER_SECRETS; j++){
                strcpy(posteriors.TextBoxInnersText[j][i], "0");
            }
        }
        return;
    }

    // Hyper is ready
    // Outer
    vector<string> truncDist = getStrTruncatedDist(hyper.outer, PROB_PRECISION);
    for(int i = 0; i < hyper.num_post; i++){
        strcpy(posteriors.TextBoxOuterText[i], truncDist[i].c_str());
    }

    // Inners
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
}

void Gui::updateRectanglePriorCircleLabel(Circle &priorCircle){
    visualization.recLabelPriorCircle = (Rectangle) {
        (float) priorCircle.center.x - 8,
        (float) priorCircle.center.y - 15,
        30,
        30
    };
}

void Gui::updateRectangleInnersCircleLabel(int channel, Circle innersCircles[MAX_CHANNEL_OUTPUTS]){
    // Update circle labels and rectangles
    for(int i = 0; i < posteriors.numPosteriors[channel]; i++){
        visualization.recLabelInnersCircles[channel][i] = (Rectangle){
            (float) innersCircles[i].center.x - 8,
            (float) innersCircles[i].center.y - 11,
            20,
            20
        };
    } 
}

void Gui::checkMouseHover(Vector2 mousePosition){
    helpMessagesActive[HELP_MSG_BUTTON_PRIOR] = CheckCollisionPointRec(mousePosition, prior.recButtonRand);
    helpMessagesActive[HELP_MSG_BUTTON_CHANNEL] = (channel.curChannel != CHANNEL_3 && CheckCollisionPointRec(mousePosition, channel.recButtonRand));
    helpMessagesActive[HELP_MSG_BUTTON_DRAW] = CheckCollisionPointRec(mousePosition, visualization.recButtonDraw);
}
