/*******************************************************************************************
*
*   Qif-graphics v1.0.0 - Tool for QIF (Quantitative Information Flow).
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2019 Inscrypt. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include "/home/ramon/raygui/src/raygui.h"

#include <emscripten/emscripten.h>
#include <vector>
#include <string>

using namespace std;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void updateDrawFrame(void);     // Update and Draw one frame

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


// qif-graphics: controls initialization
//----------------------------------------------------------------------------------
// GroupBoxes
char *GroupBoxPriorText = "Prior";    // GROUPBOX: GroupBoxPrior
char *GroupBoxChannelText = "Channel";    // GROUPBOX: GroupBoxChannel
char *GroupBoxPosteriorsText = "Posteriors";    // GROUPBOX: GroupBoxPosteriors
char *GroupBoxGainText = "Gain function";    // GROUPBOX: GroupBoxGain
char *GroupBoxVisualizationText = "Visualization";    // GROUPBOX: GroupBoxVisualization
char *GroupBoxDrawingText = "Drawing";    // GROUPBOX: GroupBoxDrawing

// Labels
char *LabelOutputsText = "Outputs";    // LABEL: LabelOutputs
char *LabelActionsText = "Actions";    // LABEL: LabelActions
char *LabelClickDrawText = "Click to draw:";    // LABEL: LabelClickDraw
char *LabelOuterNameText = "Outer";    // LABEL: LabelOuter
vector<string> LabelGainXText({"X1", "X2", "X3"}); 
vector<string> LabelGainWText({"W1", "W2", "W3"});
vector<string> LabelChannelXText({"X1", "X2", "X3"});
vector<string> LabelChannelYText({"Y1", "Y2", "Y3"});
vector<string> LabelPriorText({"X1", "X2", "X3"});
vector<string> LabelOuterText({"I1", "I2", "I3"});
vector<string> LabelInnerText({"X1", "X2", "X3"});

// CheckBoxes
char *CheckBoxGainText = "Enable";    // CHECKBOXEX: CheckBoxGain
char *CheckBoxDrawingText = "";    // CHECKBOXEX: CheckBoxDrawing

// StatusBar
char *StatusBarDrawingText = "Status";    // STATUSBAR: StatusBarDrawing

// TextBoxes. Obs: All matrices inverts rows and columns (to make easy adding and removing columns).
vector<vector<bool> > TextBoxGainEditMode(3, vector<bool>(3, false));
vector<vector<string> > TextBoxGainText(3, vector<string>(3, "0"));
vector<vector<bool> > TextBoxChannelEditMode(3, vector<bool>(3, false));
vector<vector<string> > TextBoxChannelText(3, vector<string>(3, "0"));
vector<bool> TextBoxPriorEditMode(3, false);
vector<string> TextBoxPriorText(3, "0");
vector<bool> TextBoxOuterEditMode(3, false);
vector<string> TextBoxOuterText(3, "0");
vector<vector<bool> > TextBoxInnersEditMode(3, vector<bool>(3, false));
vector<vector<string> > TextBoxInnersText(3, vector<string>(3, "0"));

// DropDowns
char *DropDownLoadText = "Load;Prior;Channel;Gain function";    // DROPDOWNBOX: DropDownLoad
char *DropDownExportText = "Export;Prior;Channel;Posteriors;Gain function";    // DROPDOWNBOX: DropDownExport
char *DropDownBoxFileText = "File;Open;Save;Save as";    // DROPDOWNBOX: DropDownBoxFile

// Define anchors
Vector2 anchorGain = { 95, 345 };            // ANCHOR ID:1
Vector2 anchorChannel = { 95, 115 };            // ANCHOR ID:2
Vector2 anchorPrior = { 400, 95 };            // ANCHOR ID:3
Vector2 anchorDrawing = { 150, 570 };            // ANCHOR ID:4
Vector2 anchorOuter = { 535, 75 };            // ANCHOR ID:5
Vector2 anchorInners = { 535, 125 };            // ANCHOR ID:6
Vector2 anchorVisualization = { 365, 270 };            // ANCHOR ID:7

// Define controls variables
bool SpinnerChannelEditMode = false;
int SpinnerChannelValue = 0;            // Spinner: SpinnerChannel
// Vector2 ScrollPanelChannelScrollOffset = { 0, 0 };
// Vector2 ScrollPanelChannelBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelChannel
// Vector2 ScrollPanelGainScrollOffset = { 0, 0 };
// Vector2 ScrollPanelGainBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelGain
bool SpinnerGainEditMode = false;
int SpinnerGainValue = 0;            // Spinner: SpinnerGain
bool CheckBoxGainChecked = false;            // CheckBoxEx: CheckBoxGain
bool CheckBoxDrawingChecked = false;            // CheckBoxEx: CheckBoxDrawing
// Vector2 ScrollPanelPosteriorsScrollOffset = { 0, 0 };
// Vector2 ScrollPanelPosteriorsBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelPosteriors
bool DropDownLoadEditMode = false;
int DropDownLoadActive = 0;            // DropdownBox: DropDownLoad
bool DropDownExportEditMode = false;
int DropDownExportActive = 0;            // DropdownBox: DropDownExport
bool DropDownBoxFileEditMode = false;
int DropDownBoxFileActive = 0;            // DropdownBox: DropDownBoxFile


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(){
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 1200;
    int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "QIF-graphics");

    emscripten_set_main_loop(updateDrawFrame, 0, 1);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        updateDrawFrame();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------

void updateDrawFrame(void){
    // Define controls rectangles

    // Panels
    //--------------------------------------------------------------------------------------
        Rectangle layoutRecsPanelMenu = (Rectangle){ 50, 0, 740, 25 };
        Rectangle layoutRecsPanelBody = (Rectangle){ 50, 25, 740, 600 };
    //--------------------------------------------------------------------------------------

    // GroupBoxes
    //--------------------------------------------------------------------------------------
        Rectangle layoutRecsGroupBoxPrior = (Rectangle){ 365, 40, 95, 215 };
        Rectangle layoutRecsGroupBoxChannel = (Rectangle){ 60, 40, 290, 215 };
        Rectangle layoutRecsGroupBoxPosteriors = (Rectangle){ 475, 40, 305, 215 };
        Rectangle layoutRecsGroupBoxGain = (Rectangle){ 60, 270, 290, 215 };
        Rectangle layoutRecsGroupBoxVisualization = (Rectangle){ 365, 270, 416, 344 };
        Rectangle layoutRecsGroupBoxDrawing = (Rectangle){ 60, 495, 290, 119 };
    //--------------------------------------------------------------------------------------

    // Spinners
    //--------------------------------------------------------------------------------------
        Rectangle layoutRecsSpinnerChannel = (Rectangle){ 265, 50, 80, 20 };    // Spinner: SpinnerChannel
        Rectangle layoutRecsSpinnerGain = (Rectangle){ 265, 280, 80, 20 };    // Spinner: SpinnerGain
    //--------------------------------------------------------------------------------------

    // Labels
    //--------------------------------------------------------------------------------------
        Rectangle layoutRecsLabelOutputs = (Rectangle){ 210, 50, 50, 20 };
        Rectangle layoutRecsLabelActions = (Rectangle){ 210, 280, 50, 20 };
        Rectangle layoutRecsLabelClickDraw = (Rectangle){ anchorDrawing.x + 0, anchorDrawing.y + 0, 75, 25 };
        Rectangle layoutRecsLabelOuterName = (Rectangle){ anchorOuter.x + -40, anchorOuter.y + 0, 40, 35 };

        // Prior
        //--------------------------------------------------------------------------------------        
        vector<Rectangle> layoutRecsLabelPrior({
            (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 0, 15, 35 },    // Label: LabelPriorX1
            (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 35, 15, 35 },    // Label: LabelPriorX2
            (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 70, 15, 35 }
        });
        //--------------------------------------------------------------------------------------        

        // Gain matrix
        //--------------------------------------------------------------------------------------
        vector<Rectangle> layoutRecsLabelGainX({
            (Rectangle){ anchorGain.x + -20, anchorGain.y + 0, 15, 35 },    // Label: LabelGainX1
            (Rectangle){ anchorGain.x + -20, anchorGain.y + 35, 15, 35 },    // Label: LabelGainX2
            (Rectangle){ anchorGain.x + -20, anchorGain.y + 70, 15, 35 }    // Label: LabelGainX3
        });
        vector<Rectangle> layoutRecsLabelGainW({
            (Rectangle){ anchorGain.x + 10, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW1
            (Rectangle){ anchorGain.x + 45, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW2
            (Rectangle){ anchorGain.x + 80, anchorGain.y + -20, 20, 20 }    // Label: LabelGainW3
        });
        //--------------------------------------------------------------------------------------

        // Channel matrix
        //--------------------------------------------------------------------------------------
        vector<Rectangle> layoutRecsLabelChannelX({
            (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 0, 15, 35 },    // Label: LabelChannelX1
            (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 35, 15, 35 },    // Label: LabelChannelX2
            (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 70, 15, 35 }    // Label: LabelChannelX3
        });
        vector<Rectangle> layoutRecsLabelChannelY({
            (Rectangle){ anchorChannel.x + 10, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY1
            (Rectangle){ anchorChannel.x + 45, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY2
            (Rectangle){ anchorChannel.x + 80, anchorChannel.y + -20, 20, 20 }    // Label: LabelChannelY3
        });
        //--------------------------------------------------------------------------------------

        // Outer
        //--------------------------------------------------------------------------------------
        vector<Rectangle> layoutRecsLabelOuter({
            (Rectangle){ anchorOuter.x + 15, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter1
            (Rectangle){ anchorOuter.x + 45, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter2
            (Rectangle){ anchorOuter.x + 80, anchorOuter.y + -20, 20, 20 }    // Label: LabelOuter3
        });
        //--------------------------------------------------------------------------------------

        // Inners matrix
        //--------------------------------------------------------------------------------------
        vector<Rectangle> layoutRecsLabelInners({
            (Rectangle){ anchorInners.x + -20, anchorInners.y + 0, 15, 35 },    // Label: LabelInnerX1
            (Rectangle){ anchorInners.x + -20, anchorInners.y + 35, 15, 35 },    // Label: LabelInnerX2
            (Rectangle){ anchorInners.x + -20, anchorInners.y + 70, 15, 35 }    // Label: LabelInnerX3
        });
        //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------

    // CheckBoxes
    //--------------------------------------------------------------------------------------
        Rectangle layoutRecsCheckBoxGain = (Rectangle){ 65, 280, 17, 17 };    // CheckBoxEx: CheckBoxGain
        Rectangle layoutRecsCheckBoxDrawing = (Rectangle){ anchorDrawing.x + 75, anchorDrawing.y + 0, 25, 25 };    // CheckBoxEx: CheckBoxDrawing
    //--------------------------------------------------------------------------------------

    // Lines
    //--------------------------------------------------------------------------------------
        Rectangle layoutRecsLine1 = (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 35, 105, 12 };
    //--------------------------------------------------------------------------------------

    // Status bar
    //--------------------------------------------------------------------------------------
        Rectangle layoutRecsStatusBar = (Rectangle){ 65, 505, 280, 45 };
    //--------------------------------------------------------------------------------------

    // TextBoxes. Obs: All matrices inverts rows and columns (to make easy adding and removing columns).
    //--------------------------------------------------------------------------------------

        // Prior
        vector<Rectangle> layoutRecsTextBoxPrior({
            (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 0, 35, 35 },    // TextBox: TextBoxPrior1
            (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 35, 35, 35 },    // TextBox: TextBoxPrior2
            (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 70, 35, 35 }    // TextBox: TextBoxPrior3
        });

        // Gain Matrix
        //--------------------------------------------------------------------------------------
        vector<vector<Rectangle> > layoutRecsTextBoxGain(3, vector<Rectangle>(3));
        for(int i = 0; i < layoutRecsTextBoxGain.size(); i++){ // columns
            for(int j = 0; j < layoutRecsTextBoxGain[i].size(); j++){ // rows
                layoutRecsTextBoxGain[i][j] = (Rectangle){ anchorGain.x + (i*35), anchorGain.y + (j*35), 35, 35 };
            }
        }
        //--------------------------------------------------------------------------------------
        
        // Channel matrix
        //--------------------------------------------------------------------------------------
        vector<vector<Rectangle> > layoutRecsTextBoxChannel(3, vector<Rectangle>(3));
        for(int i = 0; i < layoutRecsTextBoxChannel.size(); i++){ // columns
            for(int j = 0; j < layoutRecsTextBoxChannel[i].size(); j++){ // rows
                layoutRecsTextBoxChannel[i][j] = (Rectangle){ anchorChannel.x + (i*35), anchorChannel.y + (j*35), 35, 35 };
            }
        }
        //--------------------------------------------------------------------------------------

        // Outer
        //--------------------------------------------------------------------------------------
        vector<Rectangle> layoutRecsTextBoxOuter({
            (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter1
            (Rectangle){ anchorOuter.x + 35, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter2
            (Rectangle){ anchorOuter.x + 70, anchorOuter.y + 0, 35, 35 }    // TextBox: TextBoxOuter3
        });
        //--------------------------------------------------------------------------------------

        // Inners
        //--------------------------------------------------------------------------------------
        vector<vector<Rectangle> > layoutRecsTextBoxInners(3, vector<Rectangle>(3));
        for(int i = 0; i < layoutRecsTextBoxInners.size(); i++){ // columns
            for(int j = 0; j < layoutRecsTextBoxInners[i].size(); j++){ // rows
                layoutRecsTextBoxInners[i][j] = (Rectangle){ anchorInners.x + (i*35), anchorInners.y + (j*35), 35, 35 };
            }
        }
        //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------

    // DropDowns
    //--------------------------------------------------------------------------------------
        Rectangle layoutRecsDropDownFile = (Rectangle){ 50, 0, 105, 25 };    // DropdownBox: DropDownBoxFile
        Rectangle layoutRecsDropDownLoad = (Rectangle){ 155, 0, 105, 25 };    // DropdownBox: DropDownLoad
        Rectangle layoutRecsDropDownExport = (Rectangle){ 260, 0, 105, 25 };    // DropdownBox: DropDownExport
    //--------------------------------------------------------------------------------------

    // Rectangle layoutRecs[78] = {
    //     // (Rectangle){ 50, 0, 740, 25 },    // Panel: BoxMenu
    //     // (Rectangle){ 50, 25, 740, 600 },    // Panel: BoxBody
    //     // (Rectangle){ 365, 40, 95, 215 },    // GroupBox: GroupBoxPrior
    //     // (Rectangle){ 60, 40, 290, 215 },    // GroupBox: GroupBoxChannel
    //     // (Rectangle){ 475, 40, 305, 215 },    // GroupBox: GroupBoxPosteriors
    //     // (Rectangle){ 60, 270, 290, 215 },    // GroupBox: GroupBoxGain
    //     // (Rectangle){ 265, 50, 80, 20 },    // Spinner: SpinnerChannel
    //     // (Rectangle){ 210, 50, 50, 20 },    // Label: LabelOutputs
    //     (Rectangle){ 65, 75, 280, 175 },    // ScrollPanel: ScrollPanelChannel
    //     (Rectangle){ 65, 305, 280, 175 },    // ScrollPanel: ScrollPanelGain
    //     // (Rectangle){ 265, 280, 80, 20 },    // Spinner: SpinnerGain
    //     // (Rectangle){ 210, 280, 50, 20 },    // Label: LabelActions
    //     // (Rectangle){ anchorGain.x + -20, anchorGain.y + 0, 15, 35 },    // Label: LabelGainX1
    //     // (Rectangle){ anchorGain.x + -20, anchorGain.y + 35, 15, 35 },    // Label: LabelGainX2
    //     // (Rectangle){ anchorGain.x + -20, anchorGain.y + 70, 15, 35 },    // Label: LabelGainX3
    //     // (Rectangle){ anchorGain.x + 10, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW1
    //     // (Rectangle){ anchorGain.x + 45, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW2
    //     // (Rectangle){ anchorGain.x + 80, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW3

    //     // (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 0, 15, 35 },    // Label: LabelChannelX1
    //     // (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 35, 15, 35 },    // Label: LabelChannelX2
    //     // (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 70, 15, 35 },    // Label: LabelChannelX3

    //     // (Rectangle){ anchorChannel.x + 10, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY1
    //     // (Rectangle){ anchorChannel.x + 45, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY2
    //     // (Rectangle){ anchorChannel.x + 80, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY3
    //     // (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 0, 15, 35 },    // Label: LabelPriorX1
    //     // (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 35, 15, 35 },    // Label: LabelPriorX2
    //     // (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 70, 15, 35 },    // Label: LabelPriorX3
    //     // (Rectangle){ 365, 270, 416, 344 },    // GroupBox: GroupBoxVisualization
    //     // (Rectangle){ 60, 495, 290, 119 },    // GroupBox: GroupBoxDrawing
    //     // (Rectangle){ 65, 280, 17, 17 },    // CheckBoxEx: CheckBoxGain
    //     // (Rectangle){ anchorDrawing.x + 75, anchorDrawing.y + 0, 25, 25 },    // CheckBoxEx: CheckBoxDrawing
    //     // (Rectangle){ 65, 505, 280, 45 },    // StatusBar: StatusBarDrawing
    //     // (Rectangle){ anchorDrawing.x + 0, anchorDrawing.y + 0, 75, 25 },    // Label: LabelClickDraw
    //     (Rectangle){ 480, 50, 295, 200 },    // ScrollPanel: ScrollPanelPosteriors
    //     // (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 35, 105, 12 },    // Line: Line069
    //     // (Rectangle){ anchorInners.x + -20, anchorInners.y + 0, 15, 35 },    // Label: LabelInnerX1
    //     // (Rectangle){ anchorInners.x + -20, anchorInners.y + 35, 15, 35 },    // Label: LabelInnerX2
    //     // (Rectangle){ anchorInners.x + -20, anchorInners.y + 70, 15, 35 },    // Label: LabelInnerX3
    //     // (Rectangle){ anchorOuter.x + -40, anchorOuter.y + 0, 40, 35 },    // Label: LabelOuter
    //     // (Rectangle){ anchorOuter.x + 45, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter2
    //     // (Rectangle){ anchorOuter.x + 80, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter3
    //     // (Rectangle){ anchorOuter.x + 15, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter1
    //     // (Rectangle){ 155, 0, 105, 25 },    // DropdownBox: DropDownLoad
    //     // (Rectangle){ 260, 0, 105, 25 },    // DropdownBox: DropDownExport
    //     // (Rectangle){ 50, 0, 105, 25 },    // DropdownBox: DropDownBoxFile
    //     // (Rectangle){ anchorGain.x + 0, anchorGain.y + 0, 35, 35 },    // TextBox: TextBoxGain00
    //     // (Rectangle){ anchorGain.x + 35, anchorGain.y + 0, 35, 35 },    // TextBox: TextBoxGain01
    //     // (Rectangle){ anchorGain.x + 70, anchorGain.y + 0, 35, 35 },    // TextBox: TextBoxGain02
    //     // (Rectangle){ anchorGain.x + 0, anchorGain.y + 35, 35, 35 },    // TextBox: TextBoxGain10
    //     // (Rectangle){ anchorGain.x + 35, anchorGain.y + 35, 35, 35 },    // TextBox: TextBoxGain11
    //     // (Rectangle){ anchorGain.x + 70, anchorGain.y + 35, 35, 35 },    // TextBox: TextBoxGain12
    //     // (Rectangle){ anchorGain.x + 0, anchorGain.y + 70, 35, 35 },    // TextBox: TextBoxGain20
    //     // (Rectangle){ anchorGain.x + 35, anchorGain.y + 70, 35, 35 },    // TextBox: TextBoxGain21
    //     // (Rectangle){ anchorGain.x + 70, anchorGain.y + 70, 35, 35 },    // TextBox: TextBoxGain22

    //     // (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 0, 35, 35 },    // TextBox: TextBoxChannel00
    //     // (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 0, 35, 35 },    // TextBox: TextBoxcChannel01
    //     // (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 0, 35, 35 },    // TextBox: TextBoxChannel02
    //     // (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 35, 35, 35 },    // TextBox: TextBoxChannel10
    //     // (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 35, 35, 35 },    // TextBox: TextBoxChannel11
    //     // (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 35, 35, 35 },    // TextBox: TextBoxChannel12
    //     // (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 70, 35, 35 },    // TextBox: TextBoxChannel20
    //     // (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 70, 35, 35 },    // TextBox: TextBoxChannel21
    //     // (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 70, 35, 35 },    // TextBox: TextBoxChannel22

    //     // (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 0, 35, 35 },    // TextBox: TextBoxPrior1
    //     // (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 35, 35, 35 },    // TextBox: TextBoxPrior2
    //     // (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 70, 35, 35 },    // TextBox: TextBoxPrior3

    //     // (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter1
    //     // (Rectangle){ anchorOuter.x + 35, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter2
    //     // (Rectangle){ anchorOuter.x + 70, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter3

    //     // (Rectangle){ anchorInners.x + 0, anchorInners.y + 0, 35, 35 },    // TextBox: TextBoxInners00
    //     // (Rectangle){ anchorInners.x + 35, anchorInners.y + 0, 35, 35 },    // TextBox: TextBoxInners01
    //     // (Rectangle){ anchorInners.x + 70, anchorInners.y + 0, 35, 35 },    // TextBox: TextBoxInners02
    //     // (Rectangle){ anchorInners.x + 0, anchorInners.y + 35, 35, 35 },    // TextBox: TextBoxInners10
    //     // (Rectangle){ anchorInners.x + 35, anchorInners.y + 35, 35, 35 },    // TextBox: TextBoxInners11
    //     // (Rectangle){ anchorInners.x + 70, anchorInners.y + 35, 35, 35 },    // TextBox: TextBoxInners12
    //     // (Rectangle){ anchorInners.x + 0, anchorInners.y + 70, 35, 35 },    // TextBox: TextBoxInners20
    //     // (Rectangle){ anchorInners.x + 35, anchorInners.y + 70, 35, 35 },    // TextBox: TextBoxInners21
    //     // (Rectangle){ anchorInners.x + 70, anchorInners.y + 70, 35, 35 }    // TextBox: TextBoxInners22
    // };
    //----------------------------------------------------------------------------------

    // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            // Draw controls
            // if (DropDownLoadEditMode || DropDownExportEditMode || DropDownBoxFileEditMode) GuiLock();

            // Panels
            //----------------------------------------------------------------------------------
                GuiPanel(layoutRecsPanelMenu);
                GuiPanel(layoutRecsPanelBody);
            //----------------------------------------------------------------------------------

            // GroupBoxes
            //----------------------------------------------------------------------------------
                GuiGroupBox(layoutRecsGroupBoxPrior, GroupBoxPriorText);
                GuiGroupBox(layoutRecsGroupBoxChannel, GroupBoxChannelText);
                GuiGroupBox(layoutRecsGroupBoxPosteriors, GroupBoxPosteriorsText);
                GuiGroupBox(layoutRecsGroupBoxGain, GroupBoxGainText);
                GuiGroupBox(layoutRecsGroupBoxVisualization, GroupBoxVisualizationText);
                GuiGroupBox(layoutRecsGroupBoxDrawing, GroupBoxDrawingText);
            //----------------------------------------------------------------------------------

            // Spinners
            //----------------------------------------------------------------------------------
                if (GuiSpinner(layoutRecsSpinnerChannel, "", &SpinnerChannelValue, 0, 100, SpinnerChannelEditMode)) SpinnerChannelEditMode = !SpinnerChannelEditMode;
                if (GuiSpinner(layoutRecsSpinnerGain, "", &SpinnerGainValue, 0, 100, SpinnerGainEditMode)) SpinnerGainEditMode = !SpinnerGainEditMode;
            //----------------------------------------------------------------------------------

            // Labels
            //----------------------------------------------------------------------------------
                GuiLabel(layoutRecsLabelOutputs, LabelOutputsText);
                GuiLabel(layoutRecsLabelActions, LabelActionsText);
                GuiLabel(layoutRecsLabelClickDraw, LabelClickDrawText);
                GuiLabel(layoutRecsLabelOuterName, LabelOuterNameText);

                // Prior
                for(int i = 0; i < LabelPriorText.size(); i++) GuiLabel(layoutRecsLabelPrior[i], &(LabelPriorText[i][0]));

                // Gain function
                for(int i = 0; i < LabelGainXText.size(); i++) GuiLabel(layoutRecsLabelGainX[i], &(LabelGainXText[i][0]));
                for(int i = 0; i < LabelGainWText.size(); i++) GuiLabel(layoutRecsLabelGainW[i], &(LabelGainWText[i][0]));

                // Channel
                for(int i = 0; i < LabelChannelXText.size(); i++) GuiLabel(layoutRecsLabelChannelX[i], &(LabelChannelXText[i][0]));
                for(int i = 0; i < LabelChannelYText.size(); i++) GuiLabel(layoutRecsLabelChannelY[i], &(LabelChannelYText[i][0]));

                // Outer
                GuiLabel(layoutRecsLabelOuterName, LabelOuterNameText);
                for(int i = 0; i < LabelOuterText.size(); i++) GuiLabel(layoutRecsLabelOuter[i], &(LabelOuterText[i][0]));

                // Inners
                for(int i = 0; i < LabelInnerText.size(); i++) GuiLabel(layoutRecsLabelInners[i], &(LabelInnerText[i][0]));
            //----------------------------------------------------------------------------------

            // CheckBoxes
            //----------------------------------------------------------------------------------
                CheckBoxGainChecked = GuiCheckBox(layoutRecsCheckBoxGain, CheckBoxGainText, CheckBoxGainChecked);
                CheckBoxDrawingChecked = GuiCheckBox(layoutRecsCheckBoxDrawing, CheckBoxDrawingText, CheckBoxDrawingChecked);
            //----------------------------------------------------------------------------------

            // Lines
            //----------------------------------------------------------------------------------
                GuiLine(layoutRecsLine1, NULL);
            //----------------------------------------------------------------------------------

            // StatusBar
            //----------------------------------------------------------------------------------
                GuiStatusBar(layoutRecsStatusBar, StatusBarDrawingText);
            //----------------------------------------------------------------------------------

            // ScrollPanelChannelScrollOffset = GuiScrollPanel((Rectangle){layoutRecs[8].x, layoutRecs[8].y, layoutRecs[8].width - ScrollPanelChannelBoundsOffset.x, layoutRecs[8].height - ScrollPanelChannelBoundsOffset.y }, layoutRecs[8], ScrollPanelChannelScrollOffset);
            // ScrollPanelGainScrollOffset = GuiScrollPanel((Rectangle){layoutRecs[9].x, layoutRecs[9].y, layoutRecs[9].width - ScrollPanelGainBoundsOffset.x, layoutRecs[9].height - ScrollPanelGainBoundsOffset.y }, layoutRecs[9], ScrollPanelGainScrollOffset);
            // ScrollPanelPosteriorsScrollOffset = GuiScrollPanel((Rectangle){layoutRecs[33].x, layoutRecs[33].y, layoutRecs[33].width - ScrollPanelPosteriorsBoundsOffset.x, layoutRecs[33].height - ScrollPanelPosteriorsBoundsOffset.y }, layoutRecs[33], ScrollPanelPosteriorsScrollOffset);
            
            // TextBoxes
            //----------------------------------------------------------------------------------
                // Gain function
                for(int i = 0; i < TextBoxGainText.size(); i++){
                    for(int j = 0; j < TextBoxGainText[i].size(); j++){
                        if (GuiTextBox(layoutRecsTextBoxGain[i][j], &(TextBoxGainText[i][j][0]), 128, TextBoxGainEditMode[i][j])) TextBoxGainEditMode[i][j] = !TextBoxGainEditMode[i][j];        
                    }
                }
                
                // Channel
                for(int i = 0; i < TextBoxChannelText.size(); i++){
                    for(int j = 0; j < TextBoxChannelText[i].size(); j++){
                        if (GuiTextBox(layoutRecsTextBoxChannel[i][j], &(TextBoxChannelText[i][j][0]), 128, TextBoxChannelEditMode[i][j])) TextBoxChannelEditMode[i][j] = !TextBoxChannelEditMode[i][j];        
                    }
                }

                // Prior
                for(int i = 0; i < TextBoxPriorText.size(); i++){
                    if (GuiTextBox(layoutRecsTextBoxPrior[i], &(TextBoxPriorText[i][0]), 128, TextBoxPriorEditMode[i])) TextBoxPriorEditMode[i] = !TextBoxPriorEditMode[i];        
                }

                // Outer
                for(int i = 0; i < TextBoxOuterText.size(); i++){
                    if (GuiTextBox(layoutRecsTextBoxOuter[i], &(TextBoxOuterText[i][0]), 128, TextBoxOuterEditMode[i])) TextBoxOuterEditMode[i] = !TextBoxOuterEditMode[i];        
                }

                // Inners
                for(int i = 0; i < TextBoxInnersText.size(); i++){
                    for(int j = 0; j < TextBoxInnersText[i].size(); j++){
                        if (GuiTextBox(layoutRecsTextBoxInners[i][j], &(TextBoxInnersText[i][j][0]), 128, TextBoxInnersEditMode[i][j])) TextBoxInnersEditMode[i][j] = !TextBoxInnersEditMode[i][j];        
                    }
                }
            //----------------------------------------------------------------------------------

            // DropDowns
            //----------------------------------------------------------------------------------
                if (GuiDropdownBox(layoutRecsDropDownFile, DropDownBoxFileText, &DropDownBoxFileActive, DropDownBoxFileEditMode)) DropDownBoxFileEditMode = !DropDownBoxFileEditMode;
                if (GuiDropdownBox(layoutRecsDropDownLoad, DropDownLoadText, &DropDownLoadActive, DropDownLoadEditMode)) DropDownLoadEditMode = !DropDownLoadEditMode;
                if (GuiDropdownBox(layoutRecsDropDownExport, DropDownExportText, &DropDownExportActive, DropDownExportEditMode)) DropDownExportEditMode = !DropDownExportEditMode;
            //----------------------------------------------------------------------------------
            
            // GuiUnlock();
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
}