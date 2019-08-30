#include "../include/QIF.hpp"
#include <iostream>

void QIF::init(){
	numOutputs = 3; // Initial number of outputs in the channel matrix
	prior   = vector<long double>({0.0f, 0.0f, 0.0f});
    channel = MatrixLD({{0.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f}});

    // Init texts inside QIF matrices
	matricesTexts[PRIOR]   = MatrixString(1, vector<string>(3));
    matricesTexts[CHANNEL] = MatrixString(3, vector<string>(3));
    for(int i = 0; i < 3; i++){
        matricesTexts[PRIOR][0][i] = "0.000";
        for(int j = 0; j < 3; j++){
            matricesTexts[CHANNEL][i][j] = "0.000";
        }
    }

    // Init rectangle positions of QIF matrices
    matricesRectangles[PRIOR]   = MatrixRec(1, vector<Rectangle>(3));
    matricesRectangles[CHANNEL] = MatrixRec(3, vector<Rectangle>(3));
    for(int j = 0; j < 3; j++){
        matricesRectangles[PRIOR][0][j]   = (Rectangle){V2(WINDOW_WIDTH) + 	   30 + (j * (BOX_WIDTH+20)),	  	   		    		    H1(WINDOW_HEIGHT) + 60, BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][0][j] = (Rectangle){V2(WINDOW_WIDTH) + (j*(BOX_HEIGHT + BOX_HOR_GAP)),	 			    		    H2(WINDOW_HEIGHT) + 90, BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][1][j] = (Rectangle){V2(WINDOW_WIDTH) + (j*(BOX_HEIGHT + BOX_HOR_GAP)), 	   H2(WINDOW_HEIGHT) + 90 + BOX_HEIGHT+BOX_VER_GAP, BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][2][j] = (Rectangle){V2(WINDOW_WIDTH) + (j*(BOX_HEIGHT + BOX_HOR_GAP)), H2(WINDOW_HEIGHT) + 90 + 2*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
    }

    channelPanelScroll = {0.0f, 0.0f};
    channelPanelRec = (Rectangle){0.01f*WINDOW_WIDTH, \
                                  H2(WINDOW_HEIGHT) + 55, \
                                  V1(WINDOW_WIDTH)-0.02f*WINDOW_WIDTH, \
                                  5*(BOX_HEIGHT + BOX_VER_GAP)};

    channelPanelContentRec = (Rectangle){channelPanelRec.x, \
                                         channelPanelRec.y, \
                                         V2(WINDOW_WIDTH) + (numOutputs*(BOX_WIDTH+BOX_HOR_GAP)), \
                                         channelPanelRec.height-15};
}

void QIF::updatePrior(Layout layout){
    for(int i = 0; i < 3; i++)
        matricesRectangles[PRIOR][0][i] = (Rectangle){V2(layout.windowWidth) + 0.05*layout.windowHeight + (i * (BOX_WIDTH+20)), H1(layout.windowHeight) + 60, BOX_WIDTH, BOX_HEIGHT};
}

void QIF::updateChannel(Layout layout){
    for(int j = 0; j < oldNumOutputs; j++){
        matricesRectangles[CHANNEL][0][j] = (Rectangle){V2(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)) + channelPanelScroll.x, H2(layout.windowHeight)+ 90                             , BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][1][j] = (Rectangle){V2(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)) + channelPanelScroll.x, H2(layout.windowHeight)+ 90 + (BOX_HEIGHT+BOX_VER_GAP)  , BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][2][j] = (Rectangle){V2(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)) + channelPanelScroll.x, H2(layout.windowHeight)+ 90 + 2*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
    }

    // Check if the number of outputs was changed
    if(oldNumOutputs < numOutputs){
        for(int j = oldNumOutputs; j < numOutputs; j++){
            matricesTexts[CHANNEL][0].push_back("0.000");
            matricesTexts[CHANNEL][1].push_back("0.000");
            matricesTexts[CHANNEL][2].push_back("0.000");
            matricesRectangles[CHANNEL][0].push_back((Rectangle){channelPanelScroll.x+V2(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H2(layout.windowHeight)+ 90                             , BOX_WIDTH, BOX_HEIGHT});
            matricesRectangles[CHANNEL][1].push_back((Rectangle){channelPanelScroll.x+V2(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H2(layout.windowHeight)+ 90 + BOX_HEIGHT+BOX_VER_GAP    , BOX_WIDTH, BOX_HEIGHT});
            matricesRectangles[CHANNEL][2].push_back((Rectangle){channelPanelScroll.x+V2(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H2(layout.windowHeight)+ 90 + 2*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT});
        }
    }else{
        for(int j = oldNumOutputs-1; j >= numOutputs; j--){
            matricesTexts[CHANNEL][0].pop_back();
            matricesTexts[CHANNEL][1].pop_back();
            matricesTexts[CHANNEL][2].pop_back();
            matricesRectangles[CHANNEL][0].pop_back();
            matricesRectangles[CHANNEL][1].pop_back();
            matricesRectangles[CHANNEL][2].pop_back();
        }
    }

    channelPanelRec = (Rectangle){0.01f*layout.windowWidth, \
								  H2(layout.windowHeight) + 55, \
								  V1(layout.windowWidth)-0.02f*layout.windowWidth, \
								  5*(BOX_HEIGHT + BOX_VER_GAP)};

	channelPanelContentRec = (Rectangle){channelPanelRec.x, \
										 channelPanelRec.y, \
										 V2(layout.windowWidth) + (numOutputs*(BOX_WIDTH+BOX_HOR_GAP)), \
										 channelPanelRec.height-15};
}

void QIF::updateHyper(Layout layout){
    /* Update the hyper distribution if the user moves the prior distribution
     * This function assumes that the hyper distribution is already built */
    Point p, mousePoint;

    p = dist2Bary(hyper.prior);
    circlesPositions[0] = bary2Pixel(p.x, p.y, layout.windowWidth, layout.windowHeight);
    // priorPosition = bary2Pixel(p.x, p.y, layout.windowWidth, layout.windowHeight);   
    mousePoint.x = layout.mousePosition.x;
    mousePoint.y = layout.mousePosition.y;
    
    // Check if the user is moving the prior
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && euclidianDistance(circlesPositions[0], mousePoint) <= PRIOR_RADIUS){
        mousePoint = pixel2Bary(mousePoint.x, mousePoint.y, layout.windowWidth, layout.windowHeight);
        if(bary2Dist(mousePoint, hyper.prior.prob)){
            
            // Rounds to a point distribution if one of the probabilities is >= 0.98
            if(hyper.prior.prob[0] >= 0.98) hyper.prior.prob = {1, 0, 0};
            else if(hyper.prior.prob[1] >= 0.98) hyper.prior.prob = {0, 1, 0};
            else if(hyper.prior.prob[2] >= 0.98) hyper.prior.prob = {0, 0, 1};

            hyper.rebuildHyper(hyper.prior);
            p = dist2Bary(hyper.prior);
            circlesPositions[0] = bary2Pixel(p.x, p.y, layout.windowWidth, layout.windowHeight);
        }
    }
    
    // Calculates the pixel coordinate for posterior distributions
    unsigned int oldNumPost = circlesPositions.size();
    if(oldNumPost < (hyper.num_post+1)){
    	for(int i = 0; i < (hyper.num_post - oldNumPost + 1); i++)
    		circlesPositions.push_back(Point(0,0));
    }else if(circlesPositions.size() > (hyper.num_post+1)){
    	for(int i = 0; i < (hyper.num_post - oldNumPost + 1); i++)
    		circlesPositions.pop_back();
    }

    // posteriorsPosition.~vector<Point>();
    // posteriorsPosition = vector<Point>(hyper.num_post);
    for(int i = 1; i < hyper.num_post+1; i++){
        p = dist2Bary(hyper.inners[0][i], hyper.inners[1][i], hyper.inners[2][i]);
        circlesPositions[i] = bary2Pixel(p.x, p.y, layout.windowWidth, layout.windowHeight);
    }
    
    // Free memory
    for(int i = 0; i < matricesRectangles[OUTER].size(); i++) matricesRectangles[OUTER][i].~vector<Rectangle>();
    matricesRectangles[OUTER].~MatrixRec();
    for(int i = 0; i < matricesRectangles[INNERS].size(); i++) matricesRectangles[INNERS][i].~vector<Rectangle>();
    matricesRectangles[INNERS].~MatrixRec();

    // Outer and inners rectangles
    matricesRectangles[OUTER] = MatrixRec(1, vector<Rectangle>(hyper.num_post));
    matricesRectangles[INNERS] = MatrixRec(hyper.prior.num_el, vector<Rectangle>(hyper.num_post));
    for(int j = 0; j < hyper.num_post; j++){
        matricesRectangles[OUTER][0][j]  = (Rectangle){TV1(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H1(layout.windowHeight)+ 70                             , BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[INNERS][0][j] = (Rectangle){TV1(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H1(layout.windowHeight)+ 70 + BOX_HEIGHT+BOX_VER_GAP    , BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[INNERS][1][j] = (Rectangle){TV1(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H1(layout.windowHeight)+ 70 + 2*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[INNERS][2][j] = (Rectangle){TV1(layout.windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H1(layout.windowHeight)+ 70 + 3*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
    }
}

void QIF::updateMatricesText(){
    /* This fuction assumes that the hyper distribution is already built */

    for(int i = 0; i < 3; i++){
        matricesTexts[PRIOR][0][i] = to_string(hyper.prior.prob[i]);
    }

	// Channel ************************************************************************************/
    
    // Free memory
    for(int i = 0; i < matricesTexts[CHANNEL].size(); i++) matricesTexts[CHANNEL][i].~vector<string>();
    matricesTexts[CHANNEL].~MatrixString();

    matricesTexts[CHANNEL] = MatrixString(hyper.prior.num_el, vector<string>(hyper.channel.num_out));
    for(int j = 0; j < hyper.channel.num_out; j++){
        matricesTexts[CHANNEL][0][j] = to_string(hyper.channel.matrix[0][j]);
        matricesTexts[CHANNEL][1][j] = to_string(hyper.channel.matrix[1][j]);
        matricesTexts[CHANNEL][2][j] = to_string(hyper.channel.matrix[2][j]);
    }

    // Outer and inners **************************************************************************/

    // Free memory 
    for(int i = 0; i < matricesTexts[INNERS].size(); i++) matricesTexts[INNERS][i].~vector<string>();
    matricesTexts[INNERS].~MatrixString();
    for(int i = 0; i < matricesTexts[OUTER].size(); i++) matricesTexts[OUTER][i].~vector<string>();
    matricesTexts[OUTER].~MatrixString();

    matricesTexts[INNERS] = MatrixString(hyper.prior.num_el, vector<string>(hyper.num_post));
    matricesTexts[OUTER] = MatrixString(1, vector<string>(hyper.num_post));
    for(int j = 0; j < hyper.num_post; j++){
        matricesTexts[OUTER][0][j]  = to_string(hyper.outer.prob[j]);
        matricesTexts[INNERS][0][j] = to_string(hyper.inners[0][j]);
        matricesTexts[INNERS][1][j] = to_string(hyper.inners[1][j]);
        matricesTexts[INNERS][2][j] = to_string(hyper.inners[2][j]);
    }
}

int QIF::update(Layout layout){
	updatePrior(layout);
	updateChannel(layout);

	if(Distribution::isDistribution(prior) == false)
		return INVALID_PRIOR;

    // Check if each line of the channel matrix is a probability distribution
    if(Channel::isChannel(channel) == false)
        return INVALID_CHANNEL;

    Distribution new_prior(prior);
    Channel new_channel(new_prior, channel);
    hyper.~Hyper();
    hyper = Hyper(new_channel);
    
    updateHyper(layout);
    updateMatricesText();

    return 0;
}

int QIF::updateMatricesByText(){
    // Check if the user is typing numbers
    try{
        // Prior
        for(int i = 0; i < 3; i++){
            size_t pos = matricesTexts[PRIOR][0][i].find('/');
            if(pos != string::npos){
                // The user is typing a fraction
                string numerator = matricesTexts[PRIOR][0][i].substr(0, pos);
                string denominator = matricesTexts[PRIOR][0][i].substr(pos+1, matricesTexts[PRIOR][0][i].size()-pos-1);
                
                // Remove blank spaces
                numerator.erase(remove(numerator.begin(), numerator.end(), ' '), numerator.end());
                denominator.erase(remove(denominator.begin(), denominator.end(), ' '), denominator.end());
                prior[i] = ((long double)stod(numerator))/stod(denominator);
            }else{
                prior[i] = stold(matricesTexts[PRIOR][0][i]);
            }
        }

        // Channel
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < numOutputs; j++){
                size_t pos = matricesTexts[CHANNEL][i][j].find('/');
                if(pos != string::npos){
                    // The user is typing a fraction
                    string numerator = matricesTexts[CHANNEL][i][j].substr(0, pos);
                    string denominator = matricesTexts[CHANNEL][i][j].substr(pos+1, matricesTexts[CHANNEL][i][j].size()-pos-1);
                    
                    // Remove blank spaces
                    numerator.erase(remove(numerator.begin(), numerator.end(), ' '), numerator.end());
                    denominator.erase(remove(denominator.begin(), denominator.end(), ' '), denominator.end());
                    channel[i][j] = ((long double)stod(numerator))/stod(denominator);
                }else{
                    channel[i][j] = stold(matricesTexts[CHANNEL][i][j]);
                }
            }
        }          
        
        // If the flow arrives here, there were no error in conversion
        return 0;
    }catch(...){
    	return INVALID_VALUE;
    }
}

void QIF::updateMatricesByNumbers(){
	// Update variables prior and channel with hyper values
    for(int i = 0; i < 3; i++){
        prior[i] = hyper.prior.prob[i];
        if(channel[0].size() != numOutputs){
            for(int i = 0; i < channel.size(); i++) channel[i].~vector<long double>();
            channel.~MatrixLD();
            channel = MatrixLD(3, vector<long double>(numOutputs));
        }
        channel = hyper.channel.matrix;
    }
}

void QIF::drawMatrices(Colors colors, Layout layout){
	char buffer[MAX_BUFFER];

    // Prior
    for(int i = 0; i < 3; i++){
        sprintf(buffer, "X%d", i+1);
        GuiTextBox(matricesRectangles[PRIOR][0][i], (char*)matricesTexts[PRIOR][0][i].c_str(), PROB_PRECISION, true); // X1
        DrawTextEx(layout.mainFont, buffer, (Vector2){matricesRectangles[PRIOR][0][i].x + LABEL_Y_HOR_GAP, matricesRectangles[PRIOR][0][i].y + LABEL_Y_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
    }

    // Channel
    Rectangle view = GuiScrollPanel(channelPanelRec, channelPanelContentRec, &channelPanelScroll);
    BeginScissorMode(view.x, view.y, view.width, view.height);        
        DrawTextEx(layout.mainFont, "X1", (Vector2){matricesRectangles[CHANNEL][0][0].x + LABEL_X_HOR_GAP, matricesRectangles[CHANNEL][0][0].y + LABEL_X_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
        DrawTextEx(layout.mainFont, "X2", (Vector2){matricesRectangles[CHANNEL][1][0].x + LABEL_X_HOR_GAP, matricesRectangles[CHANNEL][1][0].y + LABEL_X_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
        DrawTextEx(layout.mainFont, "X3", (Vector2){matricesRectangles[CHANNEL][2][0].x + LABEL_X_HOR_GAP, matricesRectangles[CHANNEL][2][0].y + LABEL_X_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
        for(int j = 0; j < numOutputs; j++){
            sprintf(buffer, "Y%d", j+1);
            DrawTextEx(layout.mainFont, buffer, (Vector2){matricesRectangles[CHANNEL][0][j].x + LABEL_Y_HOR_GAP, matricesRectangles[CHANNEL][0][j].y + LABEL_Y_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
            GuiTextBox(matricesRectangles[CHANNEL][0][j], (char*)matricesTexts[CHANNEL][0][j].c_str(), PROB_PRECISION, true);
            GuiTextBox(matricesRectangles[CHANNEL][1][j], (char*)matricesTexts[CHANNEL][1][j].c_str(), PROB_PRECISION, true);
            GuiTextBox(matricesRectangles[CHANNEL][2][j], (char*)matricesTexts[CHANNEL][2][j].c_str(), PROB_PRECISION, true);
        }
    EndScissorMode();
}

void QIF::drawCircles(Colors colors, Layout layout){
	char buffer[MAX_BUFFER];

	// Prior distribution
    DrawCircle(circlesPositions[0].x, circlesPositions[0].y, PRIOR_RADIUS, colors.priorColor);
    DrawCircleLines(circlesPositions[0].x, circlesPositions[0].y, PRIOR_RADIUS, colors.priorBorderColor);
    DrawTextEx(layout.mainFont, "prior", Vector2({(float)circlesPositions[0].x-(PRIOR_RADIUS/1.5f), \
                   (float)circlesPositions[0].y - (0.2f * PRIOR_RADIUS)}), layout.headerFontSize, 1.0, BLACK);

    // Posterior distributions
    // It goes until num_post+1 because the first position is the prior distribution
    for(int i = 1; i < hyper.num_post+1; i++){
        int radius = (int)sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);
        DrawCircle(circlesPositions[i].x, circlesPositions[i].y, radius, colors.posteriorColor);
        DrawCircleLines(circlesPositions[i].x, circlesPositions[i].y, radius, colors.posteriorBorderColor);
        sprintf(buffer, "I%d", i+1);
        DrawTextEx(layout.mainFont, buffer, Vector2({(float)circlesPositions[i].x-7, \
                   (float)circlesPositions[i].y - 10}), layout.headerFontSize, 1.0, BLACK);
    }

    // Inners matrix
    DrawTextEx(layout.mainFont, "OUTER", (Vector2){matricesRectangles[OUTER][0][0].x + LABEL_X_HOR_GAP - 33, matricesRectangles[OUTER][0][0].y + LABEL_X_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
    Vector2 startPos = {TV1(layout.windowWidth) - LABEL_X_HOR_GAP - 35, H1(layout.windowHeight) + 70 + BOX_HEIGHT + BOX_VER_GAP/2.0f};
    DrawLineV(startPos, (Vector2){startPos.x + (hyper.num_post*(BOX_WIDTH + BOX_HOR_GAP)), startPos.y}, BLACK);

    DrawTextEx(layout.mainFont, "X1", (Vector2){matricesRectangles[INNERS][0][0].x + LABEL_X_HOR_GAP, matricesRectangles[INNERS][0][0].y + LABEL_X_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
    DrawTextEx(layout.mainFont, "X2", (Vector2){matricesRectangles[INNERS][1][0].x + LABEL_X_HOR_GAP, matricesRectangles[INNERS][1][0].y + LABEL_X_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
    DrawTextEx(layout.mainFont, "X3", (Vector2){matricesRectangles[INNERS][2][0].x + LABEL_X_HOR_GAP, matricesRectangles[INNERS][2][0].y + LABEL_X_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
    for(int i = 0; i < hyper.num_post; i++){
        sprintf(buffer, "I%d", i+1);
        DrawTextEx(layout.mainFont, buffer, (Vector2){matricesRectangles[INNERS][0][i].x + LABEL_Y_HOR_GAP, matricesRectangles[OUTER][0][i].y + LABEL_Y_VER_GAP}, layout.headerFontSize, 1.0, BLACK);
        GuiTextBox(matricesRectangles[OUTER][0][i], (char*)matricesTexts[OUTER][0][i].c_str(), PROB_PRECISION, false);
        GuiTextBox(matricesRectangles[INNERS][0][i], (char*)matricesTexts[INNERS][0][i].c_str(), PROB_PRECISION, false);
        GuiTextBox(matricesRectangles[INNERS][1][i], (char*)matricesTexts[INNERS][1][i].c_str(), PROB_PRECISION, false);
        GuiTextBox(matricesRectangles[INNERS][2][i], (char*)matricesTexts[INNERS][2][i].c_str(), PROB_PRECISION, false);
    }
}