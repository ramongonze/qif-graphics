#include "layout.h"

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
        recLabelChannelX = {
            (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 0, 15, 35 },    // Label: LabelChannelX1
            (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 35, 15, 35 },    // Label: LabelChannelX2
            (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 70, 15, 35 }    // Label: LabelChannelX3
        };
        recLabelChannelY = {
            (Rectangle){ anchorChannel.x + 10, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY1
            (Rectangle){ anchorChannel.x + 45, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY2
            (Rectangle){ anchorChannel.x + 80, anchorChannel.y + -20, 20, 20 }    // Label: LabelChannelY3
        };
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
        recTextBoxGain = vector<vector<Rectangle> >(3, vector<Rectangle>(3));
        for(int i = 0; i < recTextBoxGain.size(); i++){ // columns
            for(int j = 0; j < recTextBoxGain[i].size(); j++){ // rows
                recTextBoxGain[i][j] = (Rectangle){ anchorGain.x + (i*35), anchorGain.y + (j*35), 35, 35 };
            }
        }
        //--------------------------------------------------------------------------------------
        
        // Channel matrix
        //--------------------------------------------------------------------------------------
        recTextBoxChannel = vector<vector<Rectangle> >(3, vector<Rectangle>(3));
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
        recTextBoxInners = vector<vector<Rectangle> >(3, vector<Rectangle>(3));
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