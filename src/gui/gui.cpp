#include "gui.h"

Gui::Gui(){
    menu = GuiMenu();
    prior = GuiPrior();
    channel = GuiChannel();
    posteriors = GuiPosteriors();
    visualization = GuiVisualization();
    drawing = false;
}

bool Gui::checkTextBoxPressed(){
    for(long unsigned int i = 0; i < prior.TextBoxPriorEditMode.size(); i++)
        if(prior.TextBoxPriorEditMode[i] == true)
            return true;

    for(long unsigned int i = 0; i < channel.TextBoxChannelEditMode.size(); i++)
        for(long unsigned int j = 0; j < channel.TextBoxChannelEditMode[i].size(); j++)
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

void Gui::updatePrior(Distribution &_prior, Circle &priorCircle){
    std::ostringstream buffer;
    buffer << std::fixed << std::setprecision(PROB_PRECISION); /* Probabilities precision */

    for(int i = 0; i < _prior.num_el; i++){
        buffer.str(""); buffer.clear();
        buffer << _prior.prob[i];
        strcpy(prior.TextBoxPriorText[i], buffer.str().c_str());
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

void Gui::updatePosteriors(Hyper &hyper, vector<Circle> &innersCircles){
    std::ostringstream buffer;
    buffer << std::fixed << std::setprecision(PROB_PRECISION); /* Probabilities precision */
    
    // Match the number of columns in hyper and layout variables. 
    int diff = hyper.num_post - posteriors.numPosteriors;
    
    if(diff){
        if(diff < 0){
            // Remove gui textboxes if there are more items than posteriors
            for(int i = 0; i < -diff; i++){
                posteriors.layoutRecsTextBoxOuter.pop_back();
                posteriors.layoutRecsTextBoxInners.pop_back();
                posteriors.layoutRecsLabelPosteriors.pop_back();
                posteriors.LabelPosteriorsText.pop_back();
                posteriors.TextBoxOuterEditMode.pop_back();
                posteriors.TextBoxInnersEditMode.pop_back();

                free(posteriors.TextBoxOuterText[posteriors.TextBoxOuterText.size()-1]);
                posteriors.TextBoxOuterText.pop_back();
                
                for(int j = 0; j < 3; j++) free(posteriors.TextBoxInnersText[posteriors.TextBoxInnersText.size()-1][j]);
                posteriors.TextBoxInnersText.pop_back();
            }
        }else if(diff > 0){
            // Add gui textboxes if there are less items than posteriors
            for(int i = 0; i < diff; i++){
                posteriors.layoutRecsTextBoxOuter.push_back((Rectangle){0,0,0,0});
                posteriors.layoutRecsLabelPosteriors.push_back((Rectangle){0,0,0,0});
                posteriors.layoutRecsTextBoxInners.push_back(vector<Rectangle>(3));
                posteriors.TextBoxInnersEditMode.push_back(vector<bool>(3, false));
                posteriors.LabelPosteriorsText.push_back("0");

                posteriors.TextBoxOuterText.push_back(nullptr);
                posteriors.TextBoxOuterText[posteriors.TextBoxOuterText.size()-1] = (char*) malloc(128*sizeof(char));
                posteriors.TextBoxOuterEditMode.push_back(false);

                posteriors.TextBoxInnersText.push_back(vector<char*>(3));
                for(int j = 0; j < 3; j++) posteriors.TextBoxInnersText[posteriors.TextBoxInnersText.size()-1][j] = (char*) malloc(128*sizeof(char));
            }

        }

        // Update gui items 
        for(int i = 0; i < hyper.num_post; i++){
            posteriors.layoutRecsTextBoxOuter[i] = (Rectangle){posteriors.AnchorPosterior.x + 65 + i*TEXTBOX_SIZE, posteriors.AnchorPosterior.y + 55, TEXTBOX_SIZE, TEXTBOX_SIZE};
            posteriors.layoutRecsLabelPosteriors[i] = (Rectangle){posteriors.AnchorPosterior.x + 75 + i*TEXTBOX_SIZE, posteriors.AnchorPosterior.y + 35, 20, 20};    // Label: LabelPosteriors
            
            buffer.str(""); buffer.clear();
            buffer << "I" << i+1;
            posteriors.LabelPosteriorsText[i] = buffer.str(); 

            for(int j = 0; j < 3; j++){
                posteriors.layoutRecsTextBoxInners[i][j] = (Rectangle){posteriors.AnchorPosterior.x + 65 + i*TEXTBOX_SIZE, posteriors.AnchorPosterior.y + 115 + j*TEXTBOX_SIZE, TEXTBOX_SIZE, TEXTBOX_SIZE};    // TextBox: TextBoxInners00
            }
        }

        posteriors.numPosteriors = hyper.num_post;
        posteriors.ScrollPanelPosteriorsContent.x = posteriors.layoutRecsTextBoxInners[posteriors.numPosteriors-1][0].x + TEXTBOX_SIZE;
    }

    // Update values 
    for(int i = 0; i < hyper.num_post; i++){           
        buffer.str(""); buffer.clear();
        buffer << hyper.outer.prob[i];
        strcpy(posteriors.TextBoxOuterText[i], buffer.str().c_str());

        for(int j = 0; j < 3; j++){
            buffer.str(""); buffer.clear();
            buffer << hyper.inners[j][i];
            strcpy(posteriors.TextBoxInnersText[i][j], buffer.str().c_str());
        }

        // Update circle labels and rectangles
        visualization.layoutRecsLabelInnersCircles[i] = (Rectangle){
            (float) innersCircles[i].center.x - 8,
            (float) innersCircles[i].center.y - 11,
            20,
            20
        };
    }    
}
