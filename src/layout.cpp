#include "layout.h"

Layout::Layout(){
	// Controls initialization
	//----------------------------------------------------------------------------------
		// GroupBoxes
		//----------------------------------------------------------------------------------
			GroupBoxPriorText = "Prior";    // GROUPBOX: GroupBoxPrior
			GroupBoxChannelText = "Channel";    // GROUPBOX: GroupBoxChannel
			GroupBoxPosteriorsText = "Posteriors";    // GROUPBOX: GroupBoxPosteriors
			GroupBoxGainText = "Gain function";    // GROUPBOX: GroupBoxGain
			GroupBoxVisualizationText = "Visualization";    // GROUPBOX: GroupBoxVisualization
			GroupBoxDrawingText = "Drawing";    // GROUPBOX: GroupBoxDrawing
		//----------------------------------------------------------------------------------

		// Labels
		//----------------------------------------------------------------------------------
			LabelOutputsText = "Outputs";    // LABEL: LabelOutputs
			LabelActionsText = "Actions";    // LABEL: LabelActions
			LabelClickDrawText = "Click to draw:";    // LABEL: LabelClickDraw
			LabelOuterNameText = "Outer";    // LABEL: LabelOuter
			LabelGainXText = vector<string>({"X1", "X2", "X3"}); 
			LabelGainWText = vector<string>({"W1", "W2", "W3"});
			LabelChannelXText = vector<string>({"X1", "X2", "X3"});
			LabelChannelYText = vector<string>({"Y1", "Y2", "Y3"});
			LabelPriorText = vector<string>({"X1", "X2", "X3"});
			LabelOuterText = vector<string>({"I1", "I2", "I3"});
			LabelInnerText = vector<string>({"X1", "X2", "X3"});
		//----------------------------------------------------------------------------------

		// CheckBoxes
		//----------------------------------------------------------------------------------
			CheckBoxGainText = "Enable";    // CHECKBOXEX: CheckBoxGain
			CheckBoxDrawingText = "";    // CHECKBOXEX: CheckBoxDrawing
		//----------------------------------------------------------------------------------

		// StatusBar
		//----------------------------------------------------------------------------------
			StatusBarDrawingText = "Status";    // STATUSBAR: StatusBarDrawing
		//----------------------------------------------------------------------------------

		// TextBoxes. Obs: All matrices invert rows and columns (to make easy adding and removing columns).
		//----------------------------------------------------------------------------------
			TextBoxGainEditMode = vector<vector<bool>>(3, vector<bool>(3, false));
			TextBoxGainText = vector<vector<string>>(3, vector<string>(3, "0"));

			TextBoxChannelEditMode = vector<vector<bool>>(3, vector<bool>(3, false));
			TextBoxChannelText = vector<vector<string>>(3, vector<string>(3, "0"));

			TextBoxPriorEditMode = vector<bool>(3, false);
			TextBoxPriorText = vector<string>(3, "0");

			TextBoxOuterEditMode = vector<bool>(3, false);
			TextBoxOuterText = vector<string>(3, "0");

			TextBoxInnersEditMode = vector<vector<bool>>(3, vector<bool>(3, false));
			TextBoxInnersText = vector<vector<string>>(3, vector<string>(3, "0"));
		//----------------------------------------------------------------------------------

		// DropDowns
		//----------------------------------------------------------------------------------
			DropDownLoadText = "Load;Prior;Channel;Gain function";    // DROPDOWNBOX: DropDownLoad
			DropDownExportText = "Export;Prior;Channel;Posteriors;Gain function";    // DROPDOWNBOX: DropDownExport
			DropDownBoxFileText = "File;Open;Save;Save as";    // DROPDOWNBOX: DropDownBoxFile
		//----------------------------------------------------------------------------------

		// Define anchors
		//----------------------------------------------------------------------------------
			anchorGain = { 95, 345 };            // ANCHOR ID:1
			anchorChannel = { 95, 115 };            // ANCHOR ID:2
			anchorPrior = { 400, 95 };            // ANCHOR ID:3
			anchorDrawing = { 150, 570 };            // ANCHOR ID:4
			anchorOuter = { 535, 75 };            // ANCHOR ID:5
			anchorInners = { 535, 125 };            // ANCHOR ID:6
			anchorVisualization = { 365, 270 };            // ANCHOR ID:7
		//----------------------------------------------------------------------------------

		// Define controls variables
		//----------------------------------------------------------------------------------
			SpinnerChannelEditMode = false;
			SpinnerChannelValue = 3;            // Spinner: SpinnerChannel
			SpinnerGainEditMode = false;
			SpinnerGainValue = 0;            // Spinner: SpinnerGain
			CheckBoxGainChecked = false;            // CheckBoxEx: CheckBoxGain
			CheckBoxDrawingChecked = false;            // CheckBoxEx: CheckBoxDrawing
			DropDownLoadEditMode = false;
			DropDownLoadActive = 0;            // DropdownBox: DropDownLoad
			DropDownExportEditMode = false;
			DropDownExportActive = 0;            // DropdownBox: DropDownExport
			DropDownBoxFileEditMode = false;
			DropDownBoxFileActive = 0;            // DropdownBox: DropDownBoxFile
			// Vector2 ScrollPanelChannelScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelChannelBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelChannel
			// Vector2 ScrollPanelGainScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelGainBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelGain
			// Vector2 ScrollPanelPosteriorsScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelPosteriorsBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelPosteriors
		//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
}

void Layout::init(){
	// Panels
    //--------------------------------------------------------------------------------------
        recPanelMenu = (Rectangle){ 50, 0, 740, 25 };
        recPanelBody = (Rectangle){ 50, 25, 740, 600 };
    //--------------------------------------------------------------------------------------

    // GroupBoxes
    //--------------------------------------------------------------------------------------
        recGroupBoxPrior = (Rectangle){ 365, 40, 95, 215 };
        recGroupBoxChannel = (Rectangle){ 60, 40, 290, 215 };
        recGroupBoxPosteriors = (Rectangle){ 475, 40, 305, 215 };
        recGroupBoxGain = (Rectangle){ 60, 270, 290, 215 };
        recGroupBoxVisualization = (Rectangle){ 365, 270, 416, 344 };
        recGroupBoxDrawing = (Rectangle){ 60, 495, 290, 119 };
    //--------------------------------------------------------------------------------------

    // Spinners
    //--------------------------------------------------------------------------------------
        recSpinnerChannel = (Rectangle){ 265, 50, 80, 20 };    // Spinner: SpinnerChannel
        recSpinnerGain = (Rectangle){ 265, 280, 80, 20 };    // Spinner: SpinnerGain
    //--------------------------------------------------------------------------------------

    // Labels
    //--------------------------------------------------------------------------------------
        recLabelOutputs = (Rectangle){ 210, 50, 50, 20 };
        recLabelActions = (Rectangle){ 210, 280, 50, 20 };
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

        // Gain matrix
        //--------------------------------------------------------------------------------------
        recLabelGainX = {
            (Rectangle){ anchorGain.x + -20, anchorGain.y + 0, 15, 35 },    // Label: LabelGainX1
            (Rectangle){ anchorGain.x + -20, anchorGain.y + 35, 15, 35 },    // Label: LabelGainX2
            (Rectangle){ anchorGain.x + -20, anchorGain.y + 70, 15, 35 }    // Label: LabelGainX3
        };
        recLabelGainW = {
            (Rectangle){ anchorGain.x + 10, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW1
            (Rectangle){ anchorGain.x + 45, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW2
            (Rectangle){ anchorGain.x + 80, anchorGain.y + -20, 20, 20 }    // Label: LabelGainW3
        };
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
        recLabelOuter = {
            (Rectangle){ anchorOuter.x + 15, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter1
            (Rectangle){ anchorOuter.x + 45, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter2
            (Rectangle){ anchorOuter.x + 80, anchorOuter.y + -20, 20, 20 }    // Label: LabelOuter3
        };
        //--------------------------------------------------------------------------------------

        // Inners matrix
        //--------------------------------------------------------------------------------------
        recLabelInners = {
            (Rectangle){ anchorInners.x + -20, anchorInners.y + 0, 15, 35 },    // Label: LabelInnerX1
            (Rectangle){ anchorInners.x + -20, anchorInners.y + 35, 15, 35 },    // Label: LabelInnerX2
            (Rectangle){ anchorInners.x + -20, anchorInners.y + 70, 15, 35 }    // Label: LabelInnerX3
        };
        //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------

    // CheckBoxes
    //--------------------------------------------------------------------------------------
        recCheckBoxGain = (Rectangle){ 65, 280, 17, 17 };    // CheckBoxEx: CheckBoxGain
        recCheckBoxDrawing = (Rectangle){ anchorDrawing.x + 75, anchorDrawing.y + 0, 25, 25 };    // CheckBoxEx: CheckBoxDrawing
    //--------------------------------------------------------------------------------------

    // Lines
    //--------------------------------------------------------------------------------------
        recLine1 = (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 35, 105, 12 };
    //--------------------------------------------------------------------------------------

    // Status bar
    //--------------------------------------------------------------------------------------
        recStatusBar = (Rectangle){ 65, 505, 280, 45 };
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
        recTextBoxGain = vector<vector<Rectangle>>(3, vector<Rectangle>(3));
        for(int i = 0; i < recTextBoxGain.size(); i++){ // columns
            for(int j = 0; j < recTextBoxGain[i].size(); j++){ // rows
                recTextBoxGain[i][j] = (Rectangle){ anchorGain.x + (i*35), anchorGain.y + (j*35), 35, 35 };
            }
        }
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
        recTextBoxOuter = {
            (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter1
            (Rectangle){ anchorOuter.x + 35, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter2
            (Rectangle){ anchorOuter.x + 70, anchorOuter.y + 0, 35, 35 }    // TextBox: TextBoxOuter3
        };
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
    //--------------------------------------------------------------------------------------

    // DropDowns
    //--------------------------------------------------------------------------------------
        recDropDownFile = (Rectangle){ 50, 0, 105, 25 };    // DropdownBox: DropDownBoxFile
        recDropDownLoad = (Rectangle){ 155, 0, 105, 25 };    // DropdownBox: DropDownLoad
        recDropDownExport = (Rectangle){ 260, 0, 105, 25 };    // DropdownBox: DropDownExport
    //--------------------------------------------------------------------------------------
}

void Layout::updateChannel(){
    if(SpinnerChannelValue <= 0){
        SpinnerChannelValue = 1;
    }else if(SpinnerChannelValue > 7){ // Temporary, because ScrollPanel is not working
        SpinnerChannelValue = 7;
    }else if(SpinnerChannelValue < recTextBoxChannel.size()){
        int diff = abs((int)SpinnerChannelValue - (int)recTextBoxChannel.size());
        for(int i = 0; i < diff; i++){
            recTextBoxChannel.pop_back();
            TextBoxChannelEditMode.pop_back();
            TextBoxChannelText.pop_back();
            recLabelChannelY.pop_back();
            LabelChannelYText.pop_back();
        }
    }else{
        for(int i = recTextBoxChannel.size(); i < SpinnerChannelValue; i++){
            recTextBoxChannel.push_back(vector<Rectangle>(3));
            TextBoxChannelEditMode.push_back(vector<bool>(3));
            TextBoxChannelText.push_back(vector<string>(3));
            recLabelChannelY.push_back((Rectangle){anchorChannel.x + (10 + 35*i), anchorChannel.y + -20, 20, 20});
            
            char buffer[5];
            sprintf(buffer, "Y%d", i+1);
            LabelChannelYText.push_back(string(buffer));
            for(int j = 0; j < 3; j++){
                recTextBoxChannel[i][j] = (Rectangle){ anchorChannel.x + (i*35), anchorChannel.y + (j*35), 35, 35 };
                TextBoxChannelEditMode[i][j] = false;
                TextBoxChannelText[i][j] = "0";
            }
        }
    }
}