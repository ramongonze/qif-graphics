#include "layout.h"

Layout::Layout(){
	// Controls initialization
	//----------------------------------------------------------------------------------
        // Define anchors
        //----------------------------------------------------------------------------------
            // anchorGain = { 95 + OFFSET_X, 345 + OFFSET_Y };            // ANCHOR ID:1
            anchorChannel = { 95 + OFFSET_X, 115 + OFFSET_Y };            // ANCHOR ID:2
            anchorPrior = { 400 + OFFSET_X, 95 + OFFSET_Y };            // ANCHOR ID:3
            anchorDrawing = { 150 + OFFSET_X, 570 + OFFSET_Y };            // ANCHOR ID:4
            anchorOuter = { 525 + OFFSET_X, 75 + OFFSET_Y };            // ANCHOR ID:5
            anchorInners = { 525 + OFFSET_X, 125 + OFFSET_Y };            // ANCHOR ID:6
            anchorVisualization = { 365 + OFFSET_X, 270 + OFFSET_Y };            // ANCHOR ID:7
        //----------------------------------------------------------------------------------

		// GroupBoxes
		//----------------------------------------------------------------------------------
			GroupBoxPriorText = "Prior";    // GROUPBOX: GroupBoxPrior
			GroupBoxChannelText = "Channel";    // GROUPBOX: GroupBoxChannel
			GroupBoxPosteriorsText = "Posteriors";    // GROUPBOX: GroupBoxPosteriors
			// GroupBoxGainText = "Gain function";    // GROUPBOX: GroupBoxGain
			GroupBoxVisualizationText = "Visualization";    // GROUPBOX: GroupBoxVisualization
			GroupBoxDrawingText = "Drawing";    // GROUPBOX: GroupBoxDrawing
		//----------------------------------------------------------------------------------

		// Labels
		//----------------------------------------------------------------------------------
			LabelTitleText = "QIF Graphics - 1.0.1";
            LabelOutputsText = "Outputs";    // LABEL: LabelOutputs
			// LabelActionsText = "Actions";    // LABEL: LabelActions
			LabelClickDrawText = "Click to draw:";    // LABEL: LabelClickDraw
			LabelOuterNameText = "Outer";    // LABEL: LabelOuter
			// LabelGainXText = vector<string>({"X1", "X2", "X3"}); 
			// LabelGainWText = vector<string>({"W1", "W2", "W3"});
			LabelChannelXText = vector<string>({"X1", "X2", "X3"});
			LabelChannelYText = vector<string>({"Y1", "Y2", "Y3"});
			LabelPriorText = vector<string>({"X1", "X2", "X3"});
			LabelOuterText = vector<string>({"I1", "I2", "I3"});
			LabelInnerText = vector<string>({"X1", "X2", "X3"});
            LabelTriangleText = vector<string>({"X1", "X2", "X3"});
		//----------------------------------------------------------------------------------

        // Buttons
        //--------------------------------------------------------------------------------------
            ButtonPriorText = "Random";
            ButtonChannelText = "Random";
            ButtonPriorClicked = false;
            ButtonChannelClicked = false;
        //--------------------------------------------------------------------------------------

		// CheckBoxes
		//----------------------------------------------------------------------------------
			// CheckBoxGainText = "Enable";    // CHECKBOXEX: CheckBoxGain
			CheckBoxDrawingText = "";    // CHECKBOXEX: CheckBoxDrawing
		//----------------------------------------------------------------------------------

		// StatusBar
		//----------------------------------------------------------------------------------
			StatusBarDrawingText = "Status";    // STATUSBAR: StatusBarDrawing
		//----------------------------------------------------------------------------------

		// TextBoxes. Obs: All matrices invert rows and columns (to make easy adding and removing columns).
		//----------------------------------------------------------------------------------
			// TextBoxGainEditMode = vector<vector<bool>>(3, vector<bool>(3, false));
			TextBoxChannelEditMode = vector<vector<bool>>(3, vector<bool>(3, false));
			TextBoxPriorEditMode = vector<bool>(3, false);
			TextBoxOuterEditMode = vector<bool>(3, false);
			TextBoxInnersEditMode = vector<vector<bool>>(3, vector<bool>(3, false));

			TextBoxPriorText = vector<char*>(3, nullptr);
			TextBoxOuterText = vector<char*>(3, nullptr);
            for(int i = 0; i < 3; i++){
                TextBoxPriorText[i] = (char*) malloc(128*sizeof(char));
                TextBoxOuterText[i] = (char*) malloc(128*sizeof(char));

                strcpy(TextBoxPriorText[i], "0");
                strcpy(TextBoxOuterText[i], "0");
            }

			// TextBoxGainText = vector<vector<char*>>(3, vector<char*>(3, nullptr));
			TextBoxChannelText = vector<vector<char*>>(3, vector<char*>(3, nullptr));
			TextBoxInnersText = vector<vector<char*>>(3, vector<char*>(3, nullptr));
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    // TextBoxGainText[i][j] = (char*) malloc(128*sizeof(char));
                    TextBoxChannelText[i][j] = (char*) malloc(128*sizeof(char));
                    TextBoxInnersText[i][j] = (char*) malloc(128*sizeof(char));

                    // strcpy(TextBoxGainText[i][j], "0");
                    strcpy(TextBoxChannelText[i][j], "0");
                    strcpy(TextBoxInnersText[i][j], "0");
                }
            }
            
            TextBoxHelpPrior = "--> Prior distribution on the set of secrets {x1, x2, x3}\n--> All probabilities can be written as fractions or decimal numbers";
            TextBoxHelpChannel = "--> Informational channel\n--> Each value [xi,yj] corresponds to the conditional probability of the output being yj when the value of the secret is xi\n--> All probabilities can be written as fractions or decimal numbers";
            TextBoxHelpPosteriors = "--> Posterior distributions\n--> It is a distribution on distributions on the set of secrets {x1, x2, x3}\n--> The outer distribution is a distribution on channel outputs\n--> Each value [xk,Il] in the inner distributions corresponds to the conditional probability of xk being the secret when the output from the channel is Il";
            TextBoxHelpVisualization = "--> The blue ball corresponds to the prior distribution and the green ones to inners\n--> Click and drag the prior over the triangle to see what happens with inners when the prior changes";
            // TextBoxHelpGain = "";
		//----------------------------------------------------------------------------------

		// Define controls variables
		//----------------------------------------------------------------------------------
			SpinnerChannelEditMode = false;
			SpinnerChannelValue = 3;            // Spinner: SpinnerChannel
			// SpinnerGainEditMode = false;
			// SpinnerGainValue = 3;            // Spinner: SpinnerGain
			// CheckBoxGainChecked = false;            // CheckBoxEx: CheckBoxGain
			CheckBoxDrawingChecked = false;            // CheckBoxEx: CheckBoxDrawing
		//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
}

void Layout::init(){
	// Panels
    //--------------------------------------------------------------------------------------
        recPanelMenu = (Rectangle){ 50 + OFFSET_X, 0  + OFFSET_Y, 740, 25 };
        recPanelBody = (Rectangle){ 50 + OFFSET_X, 25 + OFFSET_Y, 740, 600 };
    //--------------------------------------------------------------------------------------

    // GroupBoxes
    //--------------------------------------------------------------------------------------
        recGroupBoxPrior = (Rectangle){ 365 + OFFSET_X, 40 + OFFSET_Y, 95, 215 };
        recGroupBoxChannel = (Rectangle){ 60 + OFFSET_X, 40 + OFFSET_Y, 290, 215 };
        recGroupBoxPosteriors = (Rectangle){ 475 + OFFSET_X, 40 + OFFSET_Y, 305, 215 };
        // recGroupBoxGain = (Rectangle){ 60 + OFFSET_X, 270 + OFFSET_Y, 290, 215 };
        recGroupBoxVisualization = (Rectangle){ 365 + OFFSET_X, 270 + OFFSET_Y, 416, 344 };
        recGroupBoxDrawing = (Rectangle){ 60 + OFFSET_X, 495 + OFFSET_Y, 290, 119 };
    //--------------------------------------------------------------------------------------

    // Triangle
    //--------------------------------------------------------------------------------------
        TrianglePoints = vector<Vector2>(3);
        TrianglePoints = {
            (Vector2) { anchorVisualization.x + 0.5*recGroupBoxVisualization.width, anchorVisualization.y + 28},
            (Vector2) { anchorVisualization.x + 0.1*recGroupBoxVisualization.width, anchorVisualization.y + (recGroupBoxVisualization.height-28)},
            (Vector2) { anchorVisualization.x + 0.9*recGroupBoxVisualization.width, anchorVisualization.y + (recGroupBoxVisualization.height-28)}
        };
    //--------------------------------------------------------------------------------------

    // Spinners
    //--------------------------------------------------------------------------------------
        recSpinnerChannel = (Rectangle){ 265 + OFFSET_X, 50 + OFFSET_Y, 80, 20 };    // Spinner: SpinnerChannel
        // recSpinnerGain = (Rectangle){ 265 + OFFSET_X, 280 + OFFSET_Y, 80, 20 };    // Spinner: SpinnerGain
    //--------------------------------------------------------------------------------------

    // Labels
    //--------------------------------------------------------------------------------------
        recLabelTitle = (Rectangle) { recPanelMenu.x + 5, recPanelMenu.y + 2.5, 70, 20 };
        recLabelOutputs = (Rectangle){ 210 + OFFSET_X, 50 + OFFSET_Y, 50, 20 };
        // recLabelActions = (Rectangle){ 210 + OFFSET_X, 280, 50 + OFFSET_Y, 20 };
        recLabelClickDraw = (Rectangle){ anchorDrawing.x + 0, anchorDrawing.y + 0, 75, 25 };
        recLabelOuterName = (Rectangle){ anchorOuter.x + -40, anchorOuter.y + 0, 40, 35 };

        // Prior
        //--------------------------------------------------------------------------------------        
            recLabelPrior = {
                (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 0, 15, 35 },    // Label: LabelPriorX1
                (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 35, 15, 35 },    // Label: LabelPriorX2
                (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 70, 15, 35 }
            };
            //--------------------------------------------------------------------------------------        
            
            // Update circle label
            LabelPriorCircleText = (char*) malloc(1280 * sizeof(char));
            strcpy(LabelPriorCircleText, "\xCF\x80"); // pi symbol

        // Gain matrix
        //--------------------------------------------------------------------------------------
            // recLabelGainX = {
            //     (Rectangle){ anchorGain.x + -20, anchorGain.y + 0, 15, 35 },    // Label: LabelGainX1
            //     (Rectangle){ anchorGain.x + -20, anchorGain.y + 35, 15, 35 },    // Label: LabelGainX2
            //     (Rectangle){ anchorGain.x + -20, anchorGain.y + 70, 15, 35 }    // Label: LabelGainX3
            // };
            // recLabelGainW = {
            //     (Rectangle){ anchorGain.x + 10, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW1
            //     (Rectangle){ anchorGain.x + 45, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW2
            //     (Rectangle){ anchorGain.x + 80, anchorGain.y + -20, 20, 20 }    // Label: LabelGainW3
            // };
        //--------------------------------------------------------------------------------------

        // Channel matrix
        //--------------------------------------------------------------------------------------
            recLabelChannelX = vector<Rectangle>(3);
            for(int i = 0; i < 3; i++){
                recLabelChannelX[i] = (Rectangle){ anchorChannel.x + -20, anchorChannel.y + i*35, 15, 35 };
            }

            recLabelChannelY = vector<Rectangle>(3);
            for(int i = 0; i < 3; i++){
                recLabelChannelY[i] = (Rectangle){ anchorChannel.x + 10 + 35*i, anchorChannel.y + -20, 20, 20};
            }
            //--------------------------------------------------------------------------------------

            // Outer
            //--------------------------------------------------------------------------------------
            recLabelOuter = vector<Rectangle>(3);
            for(int i = 0; i < 3; i++){
                recLabelOuter[i] = (Rectangle){ anchorOuter.x + 15 + i*35, anchorOuter.y + -20, 20, 20 };
            }
        //--------------------------------------------------------------------------------------

        // Inners matrix
        //--------------------------------------------------------------------------------------
            recLabelInners = {
                (Rectangle){ anchorInners.x + -20, anchorInners.y + 0, 15, 35 },    // Label: LabelInnerX1
                (Rectangle){ anchorInners.x + -20, anchorInners.y + 35, 15, 35 },    // Label: LabelInnerX2
                (Rectangle){ anchorInners.x + -20, anchorInners.y + 70, 15, 35 }    // Label: LabelInnerX3
            };
        //--------------------------------------------------------------------------------------

        // Triangle
        //--------------------------------------------------------------------------------------
            recLabelTriangle = {
                (Rectangle) { TrianglePoints[0].x -7, TrianglePoints[0].y -22 , 20, 20 },
                (Rectangle) { TrianglePoints[1].x -22, TrianglePoints[1].y -10, 20, 20 },
                (Rectangle) { TrianglePoints[2].x +5, TrianglePoints[2].y -10, 20, 20 }
            };
        //--------------------------------------------------------------------------------------            
    //--------------------------------------------------------------------------------------

    // Buttons
    //--------------------------------------------------------------------------------------
        recButtonPrior = (Rectangle) { anchorPrior.x - 13, recSpinnerChannel.y + 3, 50, 20 };
        recButtonChannel = (Rectangle) { anchorChannel.x - 22, recSpinnerChannel.y + 3, 50, 20 };
    //--------------------------------------------------------------------------------------

    // CheckBoxes
    //--------------------------------------------------------------------------------------
        recCheckBoxGain = (Rectangle){ 65 + OFFSET_X, 280 + OFFSET_Y, 17, 17 };    // CheckBoxEx: CheckBoxGain
        recCheckBoxDrawing = (Rectangle){ anchorDrawing.x + 75, anchorDrawing.y + 0, 25, 25 };    // CheckBoxEx: CheckBoxDrawing
    //--------------------------------------------------------------------------------------

    // Lines
    //--------------------------------------------------------------------------------------
        recLine1 = (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 35, 105, 12 };
    //--------------------------------------------------------------------------------------

    // Status bar
    //--------------------------------------------------------------------------------------
        recStatusBar = (Rectangle){ 65 + OFFSET_X, 515 + OFFSET_Y, 280, 35 };
    //--------------------------------------------------------------------------------------

    // TextBoxes. Obs: All matrices invert rows and columns (to make easy adding and removing columns).
    //--------------------------------------------------------------------------------------

        // Prior
        recTextBoxPrior = {
            (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 0, 35, 35 },    // TextBox: TextBoxPrior1
            (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 35, 35, 35 },    // TextBox: TextBoxPrior2
            (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 70, 35, 35 }    // TextBox: TextBoxPrior3
        };

        // Gain Matrix
        //--------------------------------------------------------------------------------------
        // recTextBoxGain = vector<vector<Rectangle>>(3, vector<Rectangle>(3));
        // for(int i = 0; i < recTextBoxGain.size(); i++){ // columns
        //     for(int j = 0; j < recTextBoxGain[i].size(); j++){ // rows
        //         recTextBoxGain[i][j] = (Rectangle){ anchorGain.x + (i*35), anchorGain.y + (j*35), 35, 35 };
        //     }
        // }
        //--------------------------------------------------------------------------------------
        
        // Channel matrix
        //--------------------------------------------------------------------------------------
        recTextBoxChannel = vector<vector<Rectangle>>(3, vector<Rectangle>(3));
        for(int i = 0; i < recTextBoxChannel.size(); i++){ // columns
            for(int j = 0; j < recTextBoxChannel[i].size(); j++){ // rows
                recTextBoxChannel[i][j] = (Rectangle){ anchorChannel.x + (i*35), anchorChannel.y + (j*35), 35, 35 };
            }
        }
        //--------------------------------------------------------------------------------------

        // Outer
        //--------------------------------------------------------------------------------------
        recTextBoxOuter = vector<Rectangle>(3);
        for(int i = 0; i < 3; i++){
            recTextBoxOuter[i] = (Rectangle){ anchorOuter.x + i*35, anchorOuter.y + 0, 35, 35 };
        }
        //--------------------------------------------------------------------------------------

        // Inners
        //--------------------------------------------------------------------------------------
        recTextBoxInners = vector<vector<Rectangle>>(3, vector<Rectangle>(3));
        for(int i = 0; i < recTextBoxInners.size(); i++){ // columns
            for(int j = 0; j < recTextBoxInners[i].size(); j++){ // rows
                recTextBoxInners[i][j] = (Rectangle){ anchorInners.x + (i*35), anchorInners.y + (j*35), 35, 35 };
            }
        }
        //--------------------------------------------------------------------------------------
        
        // Information icons
		//----------------------------------------------------------------------------------
            recTextBoxHelpPrior = { recGroupBoxPrior.x + recGroupBoxPrior.width - 23, recGroupBoxPrior.y + recGroupBoxPrior.height - 23, 18, 18};
            recTextBoxHelpChannel = { recGroupBoxChannel.x + recGroupBoxChannel.width - 23, recGroupBoxChannel.y + recGroupBoxChannel.height - 23, 18, 18};
            recTextBoxHelpPosteriors = { recGroupBoxPosteriors.x + recGroupBoxPosteriors.width - 23, recGroupBoxPosteriors.y + recGroupBoxPosteriors.height - 23, 18, 18};
            recTextBoxHelpVisualization = { recGroupBoxVisualization.x + recGroupBoxVisualization.width - 23, recGroupBoxVisualization.y + recGroupBoxVisualization.height - 23, 18, 18};
            
            recTextBoxHelpTextPrior = { recGroupBoxPrior.x + recGroupBoxPrior.width - 23, recGroupBoxPrior.y + recGroupBoxPrior.height + 5, 300, 50};
            recTextBoxHelpTextChannel = { recGroupBoxChannel.x + recGroupBoxChannel.width - 23, recGroupBoxChannel.y + recGroupBoxChannel.height + 5, 300, 95};
            recTextBoxHelpTextPosteriors = { recGroupBoxPosteriors.x + recGroupBoxPosteriors.width - 5 - 300, recGroupBoxPosteriors.y + recGroupBoxPosteriors.height + 5, 300, 125};
            recTextBoxHelpTextVisualization = { recGroupBoxVisualization.x + recGroupBoxVisualization.width - 5 - 300, recGroupBoxVisualization.y + recGroupBoxVisualization.height - 30 - 65, 300, 65};
		//----------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
}

void Layout::updateChannelBySpinner(){
    if(SpinnerChannelValue <= 0){
        SpinnerChannelValue = 1;
    }else if(SpinnerChannelValue > 7){ // Temporary, because ScrollPanel is not working
        SpinnerChannelValue = 7;
    }else if(SpinnerChannelValue < recTextBoxChannel.size()){
        int diff = abs((int)SpinnerChannelValue - (int)recTextBoxChannel.size());
        for(int i = 0; i < diff; i++){
            recTextBoxChannel.pop_back();
            TextBoxChannelEditMode.pop_back();
            recLabelChannelY.pop_back();
            LabelChannelYText.pop_back();

            for(int j = 0; j < 3; j++) free(TextBoxChannelText[TextBoxChannelText.size()-1][j]);
            TextBoxChannelText.pop_back();
        }
    }else{
        for(int i = recTextBoxChannel.size(); i < SpinnerChannelValue; i++){
            recTextBoxChannel.push_back(vector<Rectangle>(3));
            recLabelChannelY.push_back((Rectangle){anchorChannel.x + (10 + 35*i), anchorChannel.y + -20, 20, 20});

            TextBoxChannelEditMode.push_back(vector<bool>(3, false));
            TextBoxChannelText.push_back(vector<char*>(3));
            
            char buffer[5];
            sprintf(buffer, "Y%d", i+1);
            LabelChannelYText.push_back(string(buffer));
            for(int j = 0; j < 3; j++){
                recTextBoxChannel[i][j] = (Rectangle){ anchorChannel.x + (i*35), anchorChannel.y + (j*35), 35, 35 };
                TextBoxChannelEditMode[i][j] = false;
                TextBoxChannelText[i][j] = (char*) malloc(128*sizeof(char));
                strcpy(TextBoxChannelText[i][j], "0");
            }
        }
    }
}

void Layout::updateChannelTextBoxes(vector<vector<long double>> &channel){
    char buffer[128];

    for(int i = 0; i < channel.size(); i++){
        for(int j = 0; j < channel[i].size(); j++){
            sprintf(buffer, "%.2Lf", channel[i][j]);
            strcpy(TextBoxChannelText[i][j], buffer);
        }
    }
}

void Layout::updatePosteriors(Hyper &hyper, vector<Circle> &innersCircles, bool onlyText){

    std::ostringstream buffer;
    buffer << std::fixed << std::setprecision(PROB_PRECISION); /* Probabilities precision */
    
    if(onlyText == true){
        // Update values
        for(int i = 0; i < hyper.num_post; i++){
            buffer.str(""); buffer.clear();
            buffer << hyper.outer.prob[i];
            strcpy(TextBoxOuterText[i], buffer.str().c_str());

            for(int j = 0; j < 3; j++){
                buffer.str(""); buffer.clear();
                buffer << hyper.inners[j][i];
                strcpy(TextBoxInnersText[i][j], buffer.str().c_str());
            }
        }
    }else{
        // Match the number of columns in hyper and layout variables. 
        int diff = abs((int)hyper.num_post - (int)recLabelOuter.size());
        if(hyper.channel.num_out < recTextBoxOuter.size()){
            for(int i = 0; i < diff; i++){
                recTextBoxOuter.pop_back();
                recTextBoxInners.pop_back();
                recLabelOuter.pop_back();
                LabelOuterText.pop_back();
                TextBoxOuterEditMode.pop_back();
                TextBoxInnersEditMode.pop_back();

                free(TextBoxOuterText[TextBoxOuterText.size()-1]);
                TextBoxOuterText.pop_back();
                
                for(int j = 0; j < 3; j++) free(TextBoxInnersText[TextBoxInnersText.size()-1][j]);
                TextBoxInnersText.pop_back();
            }
        }else if(hyper.channel.num_out > recTextBoxOuter.size()){
            for(int i = 0; i < diff; i++){
                recTextBoxOuter.push_back((Rectangle){0,0,0,0});
                recLabelOuter.push_back((Rectangle){0,0,0,0});
                recTextBoxInners.push_back(vector<Rectangle>(3));
                TextBoxInnersEditMode.push_back(vector<bool>(3, false));
                LabelOuterText.push_back("0");

                TextBoxOuterText.push_back(nullptr);
                TextBoxOuterText[TextBoxOuterText.size()-1] = (char*) malloc(128*sizeof(char));
                TextBoxOuterEditMode.push_back(false);

                TextBoxInnersText.push_back(vector<char*>(3));
                for(int j = 0; j < 3; j++) TextBoxInnersText[TextBoxInnersText.size()-1][j] = (char*) malloc(128*sizeof(char));
            }
        }

        // Update values
        for(int i = 0; i < hyper.num_post; i++){
            recTextBoxOuter[i] = (Rectangle){ anchorOuter.x + i*35, anchorOuter.y + 0, 35, 35 };
            recLabelOuter[i] = (Rectangle){ anchorOuter.x + 15 + i*35, anchorOuter.y + -20, 20, 20 };
            
            buffer.str(""); buffer.clear();
            buffer << "I" << i+1;
            LabelOuterText[i] = buffer.str(); 
            
            buffer.str(""); buffer.clear();
            buffer << hyper.outer.prob[i];
            strcpy(TextBoxOuterText[i], buffer.str().c_str());

            for(int j = 0; j < 3; j++){
                recTextBoxInners[i][j] = (Rectangle){ anchorInners.x + i*35, anchorInners.y + j*35, 35, 35 };
                buffer.str(""); buffer.clear();
                buffer << hyper.inners[j][i];
                strcpy(TextBoxInnersText[i][j], buffer.str().c_str());
            }
        }

        recLabelInnersCircles = vector<Rectangle>(hyper.channel.num_out);
    }

    // Update line width
    recLine1.width = 35 * hyper.num_post;

    // Update circle labels and rectangles
    for(int i = 0; i < hyper.num_post; i++){
        recLabelInnersCircles[i] = (Rectangle) { innersCircles[i].center.x - 5, innersCircles[i].center.y -10, 20, 20 };
    }
}

void Layout::updatePrior(Distribution &prior, Circle &priorCircle){
    std::ostringstream buffer;
    buffer << std::fixed << std::setprecision(PROB_PRECISION); /* Probabilities precision */

    for(int i = 0; i < prior.num_el; i++){
        buffer.str(""); buffer.clear();
        buffer << prior.prob[i];
        strcpy(TextBoxPriorText[i], buffer.str().c_str());
    }

    recLabelPriorCircle = (Rectangle) { priorCircle.center.x - 6, priorCircle.center.y - 10, 20, 20 };
}

bool Layout::checkTextBoxPressed(){
    for(int i = 0; i < TextBoxPriorEditMode.size(); i++){
        if(TextBoxPriorEditMode[i] == true) return true;
    }

    for(int i = 0; i < TextBoxChannelEditMode.size(); i++){
        for(int j = 0; j < TextBoxChannelEditMode[i].size(); j++){
            if(TextBoxChannelEditMode[i][j] == true) return true;
        }
    }

    // for(int i = 0; i < TextBoxGainEditMode.size(); i++){
    //     for(int j = 0; j < TextBoxGainEditMode[i].size(); j++){
    //         if(TextBoxGainEditMode[i][j] == true) return true;
    //     }
    // }

    return false;
}

void Layout::moveAmongTextBoxes(){
    // Prior
    for(int i = 0; i < 3; i++){
        if(TextBoxPriorEditMode[i] == true){
            if(IsKeyPressed(KEY_TAB)){
                TextBoxPriorEditMode[i] = false;
                if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
                    TextBoxPriorEditMode[i] = false;
                    if(i == 0) TextBoxPriorEditMode[2] = true;
                    else if(i == 1) TextBoxPriorEditMode[0] = true;
                    else TextBoxPriorEditMode[1] = true;
                }else{
                    if(i == 0) TextBoxPriorEditMode[1] = true;
                    else if(i == 1) TextBoxPriorEditMode[2] = true;
                    else TextBoxPriorEditMode[0] = true;
                }
            }else if(IsKeyPressed(KEY_UP)){
                TextBoxPriorEditMode[i] = false;
                if(i == 0) TextBoxPriorEditMode[2] = true;
                else if(i == 1) TextBoxPriorEditMode[0] = true;
                else TextBoxPriorEditMode[1] = true;
            }else if(IsKeyPressed(KEY_DOWN)){
                TextBoxPriorEditMode[i] = false;
                if(i == 0) TextBoxPriorEditMode[1] = true;
                else if(i == 1) TextBoxPriorEditMode[2] = true;
                else TextBoxPriorEditMode[0] = true;
            }

            return;
        }
    }

    int nRows = TextBoxChannelEditMode[0].size();
    int nColumns = TextBoxChannelEditMode.size();

    // Channel
    for(int i = 0; i < nColumns; i++){
        for(int j = 0; j < nRows; j++){
            if(TextBoxChannelEditMode[i][j] == true){
                if(IsKeyPressed(KEY_TAB)){
                    TextBoxChannelEditMode[i][j] = false;
                    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
                        if(i == 0 && j == 0){
                            TextBoxChannelEditMode[nColumns-1][nRows-1] = true; // Select the first text box (row 0 and column 0)
                        }else if(i == 0){
                            TextBoxChannelEditMode[nColumns-1][j-1] = true;
                        }else{
                            TextBoxChannelEditMode[i-1][j] = true;
                        }
                        return;
                    }else{
                        if(i == nColumns-1 && j == nRows-1){
                            TextBoxChannelEditMode[0][0] = true; // Select the first text box (row 0 and column 0)
                        }else if(i == nColumns-1){
                            TextBoxChannelEditMode[0][j+1] = true;
                        }else{
                            TextBoxChannelEditMode[i+1][j] = true;
                        }
                        return;
                    }
                }else if(IsKeyPressed(KEY_UP)){
                    if(j > 0){
                        TextBoxChannelEditMode[i][j] = false;
                        TextBoxChannelEditMode[i][j-1] = true;
                    }
                }else if(IsKeyPressed(KEY_DOWN)){
                    if(j < nRows-1){
                        TextBoxChannelEditMode[i][j] = false;
                        TextBoxChannelEditMode[i][j+1] = true;   
                    }
                }else if(IsKeyPressed(KEY_LEFT)){
                    if(i > 0){
                        TextBoxChannelEditMode[i][j] = false;
                        TextBoxChannelEditMode[i-1][j] = true;
                    }
                }else if(IsKeyPressed(KEY_RIGHT)){
                    if(i < nColumns-1){
                        TextBoxChannelEditMode[i][j] = false;
                        TextBoxChannelEditMode[i+1][j] = true;
                    }
                }                
                return;
            }
        }
    }
}