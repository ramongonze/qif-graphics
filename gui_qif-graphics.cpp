/*******************************************************************************************
*
*   Qif-graphics v1.0.0 - Tool for QIF (Quantitative
Information Flow).
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
// Const text
const char *GroupBoxPriorText = "Prior";    // GROUPBOX: GroupBoxPrior
const char *GroupBoxChannelText = "Channel";    // GROUPBOX: GroupBoxChannel
const char *GroupBoxPosteriorsText = "Posteriors";    // GROUPBOX: GroupBoxPosteriors
const char *GroupBoxGainText = "Gain function";    // GROUPBOX: GroupBoxGain
const char *LabelOutputsText = "Outputs";    // LABEL: LabelOutputs
const char *LabelActionsText = "Actions";    // LABEL: LabelActions
const char *LabelGainX1Text = "X1";    // LABEL: LabelGainX1
const char *LabelGainX2Text = "X2";    // LABEL: LabelGainX2
const char *LabelGainX3Text = "X3";    // LABEL: LabelGainX3
const char *LabelGainW1Text = "W1";    // LABEL: LabelGainW1
const char *LabelGainW2Text = "W2";    // LABEL: LabelGainW2
const char *LabelGainW3Text = "W3";    // LABEL: LabelGainW3
const char *LabelChannelX1Text = "X1";    // LABEL: LabelChannelX1
const char *LabelChannelX2Text = "X2";    // LABEL: LabelChannelX2
const char *Label039Text = "X3";    // LABEL: Label039
const char *LabelChannelY1Text = "Y1";    // LABEL: LabelChannelY1
const char *LabelChannelY2Text = "Y2";    // LABEL: LabelChannelY2
const char *LabelChannelY3Text = "Y3";    // LABEL: LabelChannelY3
const char *LabelPriorX1Text = "X1";    // LABEL: LabelPriorX1
const char *LabelPriorX2Text = "X2";    // LABEL: LabelPriorX2
const char *LabelPriorX3Text = "X3";    // LABEL: LabelPriorX3
const char *GroupBoxVisualizationText = "Visualization";    // GROUPBOX: GroupBoxVisualization
const char *GroupBoxDrawingText = "Drawing";    // GROUPBOX: GroupBoxDrawing
const char *CheckBoxGainText = "Enable";    // CHECKBOXEX: CheckBoxGain
const char *CheckBoxDrawingText = "";    // CHECKBOXEX: CheckBoxDrawing
const char *StatusBarDrawingText = "Status";    // STATUSBAR: StatusBarDrawing
const char *Label054Text = "Click to draw:";    // LABEL: Label054
const char *LabelInnerX1Text = "X1";    // LABEL: LabelInnerX1
const char *LabelInnerX2Text = "X2";    // LABEL: LabelInnerX2
const char *LabelInnerX3Text = "X3";    // LABEL: LabelInnerX3
const char *Label072Text = "Outer";    // LABEL: Label072
const char *LabelOuter2Text = "I2";    // LABEL: LabelOuter2
const char *LabelOuter3Text = "I3";    // LABEL: LabelOuter3
const char *LabelOuter1Text = "I1";    // LABEL: LabelOuter1
const char *DropDownLoadText = "Load;Prior;Channel;Gain function";    // DROPDOWNBOX: DropDownLoad
const char *DropDownExportText = "Export;Prior;Channel;Posteriors;Gain function";    // DROPDOWNBOX: DropDownExport
const char *DropdownBox077Text = "File;Open;Save;Save as";    // DROPDOWNBOX: DropdownBox077

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
bool DropdownBox077EditMode = false;
int DropdownBox077Active = 0;            // DropdownBox: DropdownBox077
vector<vector<bool> > TextBoxGainEditMode(3, vector<bool>(3, false));
vector<vector<string> > TextBoxGainText(3, vector<string>(3, "0"));
// bool TextBoxGain00EditMode = false;
// char TextBoxGain00Text[128] = "0";            // TextBox: TextBoxGain00
// bool TextBoxGain01EditMode = false;
// char TextBoxGain01Text[128] = "0";            // TextBox: TextBoxGain01
// bool TextBoxGain02EditMode = false;
// char TextBoxGain02Text[128] = "0";            // TextBox: TextBoxGain02
// bool TextBoxGain10EditMode = false;
// char TextBoxGain10Text[128] = "0";            // TextBox: TextBoxGain10
// bool TextBoxGain11EditMode = false;
// char TextBoxGain11Text[128] = "0";            // TextBox: TextBoxGain11
// bool TextBoxGain12EditMode = false;
// char TextBoxGain12Text[128] = "0";            // TextBox: TextBoxGain12
// bool TextBoxGain20EditMode = false;
// char TextBoxGain20Text[128] = "0";            // TextBox: TextBoxGain20
// bool TextBoxGain21EditMode = false;
// char TextBoxGain21Text[128] = "0";            // TextBox: TextBoxGain21
// bool TextBoxGain22EditMode = false;
// char TextBoxGain22Text[128] = "0";            // TextBox: TextBoxGain22
bool TextBoxChannel00EditMode = false;
char TextBoxChannel00Text[128] = "0";            // TextBox: TextBoxChannel00
bool TextBoxcChannel01EditMode = false;
char TextBoxcChannel01Text[128] = "0";            // TextBox: TextBoxcChannel01
bool TextBoxChannel02EditMode = false;
char TextBoxChannel02Text[128] = "0";            // TextBox: TextBoxChannel02
bool TextBoxChannel10EditMode = false;
char TextBoxChannel10Text[128] = "0";            // TextBox: TextBoxChannel10
bool TextBoxChannel11EditMode = false;
char TextBoxChannel11Text[128] = "0";            // TextBox: TextBoxChannel11
bool TextBoxChannel12EditMode = false;
char TextBoxChannel12Text[128] = "0";            // TextBox: TextBoxChannel12
bool TextBoxChannel20EditMode = false;
char TextBoxChannel20Text[128] = "0";            // TextBox: TextBoxChannel20
bool TextBoxChannel21EditMode = false;
char TextBoxChannel21Text[128] = "0";            // TextBox: TextBoxChannel21
bool TextBoxChannel22EditMode = false;
char TextBoxChannel22Text[128] = "0";            // TextBox: TextBoxChannel22
bool TextBoxPrior1EditMode = false;
char TextBoxPrior1Text[128] = "0";            // TextBox: TextBoxPrior1
bool TextBoxPrior2EditMode = false;
char TextBoxPrior2Text[128] = "0";            // TextBox: TextBoxPrior2
bool TextBoxPrior3EditMode = false;
char TextBoxPrior3Text[128] = "0";            // TextBox: TextBoxPrior3
bool TextBoxOuter1EditMode = false;
char TextBoxOuter1Text[128] = "0";            // TextBox: TextBoxOuter1
bool TextBoxOuter2EditMode = false;
char TextBoxOuter2Text[128] = "0";            // TextBox: TextBoxOuter2
bool TextBoxOuter3EditMode = false;
char TextBoxOuter3Text[128] = "0";            // TextBox: TextBoxOuter3
bool TextBoxInners00EditMode = false;
char TextBoxInners00Text[128] = "0";            // TextBox: TextBoxInners00
bool TextBoxInners01EditMode = false;
char TextBoxInners01Text[128] = "0";            // TextBox: TextBoxInners01
bool TextBoxInners02EditMode = false;
char TextBoxInners02Text[128] = "0";            // TextBox: TextBoxInners02
bool TextBoxInners10EditMode = false;
char TextBoxInners10Text[128] = "0";            // TextBox: TextBoxInners10
bool TextBoxInners11EditMode = false;
char TextBoxInners11Text[128] = "0";            // TextBox: TextBoxInners11
bool TextBoxInners12EditMode = false;
char TextBoxInners12Text[128] = "0";            // TextBox: TextBoxInners12
bool TextBoxInners20EditMode = false;
char TextBoxInners20Text[128] = "0";            // TextBox: TextBoxInners20
bool TextBoxInners21EditMode = false;
char TextBoxInners21Text[128] = "0";            // TextBox: TextBoxInners21
bool TextBoxInners22EditMode = false;
char TextBoxInners22Text[128] = "0";            // TextBox: TextBoxInners22


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 1200;
    int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "qif-graphics");

    emscripten_set_main_loop(updateDrawFrame, 0, 1);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
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
    Rectangle layoutRecs[78] = {
        (Rectangle){ 50, 0, 740, 25 },    // Panel: BoxMenu
        (Rectangle){ 50, 25, 740, 600 },    // Panel: BoxBody
        (Rectangle){ 365, 40, 95, 215 },    // GroupBox: GroupBoxPrior
        (Rectangle){ 60, 40, 290, 215 },    // GroupBox: GroupBoxChannel
        (Rectangle){ 475, 40, 305, 215 },    // GroupBox: GroupBoxPosteriors
        (Rectangle){ 60, 270, 290, 215 },    // GroupBox: GroupBoxGain
        (Rectangle){ 265, 50, 80, 20 },    // Spinner: SpinnerChannel
        (Rectangle){ 210, 50, 50, 20 },    // Label: LabelOutputs
        (Rectangle){ 65, 75, 280, 175 },    // ScrollPanel: ScrollPanelChannel
        (Rectangle){ 65, 305, 280, 175 },    // ScrollPanel: ScrollPanelGain
        (Rectangle){ 265, 280, 80, 20 },    // Spinner: SpinnerGain
        (Rectangle){ 210, 280, 50, 20 },    // Label: LabelActions
        (Rectangle){ anchorGain.x + -20, anchorGain.y + 0, 15, 35 },    // Label: LabelGainX1
        (Rectangle){ anchorGain.x + -20, anchorGain.y + 35, 15, 35 },    // Label: LabelGainX2
        (Rectangle){ anchorGain.x + -20, anchorGain.y + 70, 15, 35 },    // Label: LabelGainX3
        (Rectangle){ anchorGain.x + 10, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW1
        (Rectangle){ anchorGain.x + 45, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW2
        (Rectangle){ anchorGain.x + 80, anchorGain.y + -20, 20, 20 },    // Label: LabelGainW3
        (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 0, 15, 35 },    // Label: LabelChannelX1
        (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 35, 15, 35 },    // Label: LabelChannelX2
        (Rectangle){ anchorChannel.x + -20, anchorChannel.y + 70, 15, 35 },    // Label: Label039
        (Rectangle){ anchorChannel.x + 10, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY1
        (Rectangle){ anchorChannel.x + 45, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY2
        (Rectangle){ anchorChannel.x + 80, anchorChannel.y + -20, 20, 20 },    // Label: LabelChannelY3
        (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 0, 15, 35 },    // Label: LabelPriorX1
        (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 35, 15, 35 },    // Label: LabelPriorX2
        (Rectangle){ anchorPrior.x + -20, anchorPrior.y + 70, 15, 35 },    // Label: LabelPriorX3
        (Rectangle){ 365, 270, 416, 344 },    // GroupBox: GroupBoxVisualization
        (Rectangle){ 60, 495, 290, 119 },    // GroupBox: GroupBoxDrawing
        (Rectangle){ 65, 280, 17, 17 },    // CheckBoxEx: CheckBoxGain
        (Rectangle){ anchorDrawing.x + 75, anchorDrawing.y + 0, 25, 25 },    // CheckBoxEx: CheckBoxDrawing
        (Rectangle){ 65, 505, 280, 45 },    // StatusBar: StatusBarDrawing
        (Rectangle){ anchorDrawing.x + 0, anchorDrawing.y + 0, 75, 25 },    // Label: Label054
        (Rectangle){ 480, 50, 295, 200 },    // ScrollPanel: ScrollPanelPosteriors
        (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 35, 105, 12 },    // Line: Line069
        (Rectangle){ anchorInners.x + -20, anchorInners.y + 0, 15, 35 },    // Label: LabelInnerX1
        (Rectangle){ anchorInners.x + -20, anchorInners.y + 35, 15, 35 },    // Label: LabelInnerX2
        (Rectangle){ anchorInners.x + -20, anchorInners.y + 70, 15, 35 },    // Label: LabelInnerX3
        (Rectangle){ anchorOuter.x + -40, anchorOuter.y + 0, 40, 35 },    // Label: Label072
        (Rectangle){ anchorOuter.x + 45, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter2
        (Rectangle){ anchorOuter.x + 80, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter3
        (Rectangle){ anchorOuter.x + 15, anchorOuter.y + -20, 20, 20 },    // Label: LabelOuter1
        (Rectangle){ 155, 0, 105, 25 },    // DropdownBox: DropDownLoad
        (Rectangle){ 260, 0, 105, 25 },    // DropdownBox: DropDownExport
        (Rectangle){ 50, 0, 105, 25 },    // DropdownBox: DropdownBox077
        (Rectangle){ anchorGain.x + 0, anchorGain.y + 0, 35, 35 },    // TextBox: TextBoxGain00
        (Rectangle){ anchorGain.x + 35, anchorGain.y + 0, 35, 35 },    // TextBox: TextBoxGain01
        (Rectangle){ anchorGain.x + 70, anchorGain.y + 0, 35, 35 },    // TextBox: TextBoxGain02
        (Rectangle){ anchorGain.x + 0, anchorGain.y + 35, 35, 35 },    // TextBox: TextBoxGain10
        (Rectangle){ anchorGain.x + 35, anchorGain.y + 35, 35, 35 },    // TextBox: TextBoxGain11
        (Rectangle){ anchorGain.x + 70, anchorGain.y + 35, 35, 35 },    // TextBox: TextBoxGain12
        (Rectangle){ anchorGain.x + 0, anchorGain.y + 70, 35, 35 },    // TextBox: TextBoxGain20
        (Rectangle){ anchorGain.x + 35, anchorGain.y + 70, 35, 35 },    // TextBox: TextBoxGain21
        (Rectangle){ anchorGain.x + 70, anchorGain.y + 70, 35, 35 },    // TextBox: TextBoxGain22
        (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 0, 35, 35 },    // TextBox: TextBoxChannel00
        (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 0, 35, 35 },    // TextBox: TextBoxcChannel01
        (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 0, 35, 35 },    // TextBox: TextBoxChannel02
        (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 35, 35, 35 },    // TextBox: TextBoxChannel10
        (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 35, 35, 35 },    // TextBox: TextBoxChannel11
        (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 35, 35, 35 },    // TextBox: TextBoxChannel12
        (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 70, 35, 35 },    // TextBox: TextBoxChannel20
        (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 70, 35, 35 },    // TextBox: TextBoxChannel21
        (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 70, 35, 35 },    // TextBox: TextBoxChannel22
        (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 0, 35, 35 },    // TextBox: TextBoxPrior1
        (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 35, 35, 35 },    // TextBox: TextBoxPrior2
        (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 70, 35, 35 },    // TextBox: TextBoxPrior3
        (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter1
        (Rectangle){ anchorOuter.x + 35, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter2
        (Rectangle){ anchorOuter.x + 70, anchorOuter.y + 0, 35, 35 },    // TextBox: TextBoxOuter3
        (Rectangle){ anchorInners.x + 0, anchorInners.y + 0, 35, 35 },    // TextBox: TextBoxInners00
        (Rectangle){ anchorInners.x + 35, anchorInners.y + 0, 35, 35 },    // TextBox: TextBoxInners01
        (Rectangle){ anchorInners.x + 70, anchorInners.y + 0, 35, 35 },    // TextBox: TextBoxInners02
        (Rectangle){ anchorInners.x + 0, anchorInners.y + 35, 35, 35 },    // TextBox: TextBoxInners10
        (Rectangle){ anchorInners.x + 35, anchorInners.y + 35, 35, 35 },    // TextBox: TextBoxInners11
        (Rectangle){ anchorInners.x + 70, anchorInners.y + 35, 35, 35 },    // TextBox: TextBoxInners12
        (Rectangle){ anchorInners.x + 0, anchorInners.y + 70, 35, 35 },    // TextBox: TextBoxInners20
        (Rectangle){ anchorInners.x + 35, anchorInners.y + 70, 35, 35 },    // TextBox: TextBoxInners21
        (Rectangle){ anchorInners.x + 70, anchorInners.y + 70, 35, 35 }    // TextBox: TextBoxInners22
    };
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
            // if (DropDownLoadEditMode || DropDownExportEditMode || DropdownBox077EditMode) GuiLock();

            GuiPanel(layoutRecs[0]);
            GuiPanel(layoutRecs[1]);
            GuiGroupBox(layoutRecs[2], GroupBoxPriorText);
            GuiGroupBox(layoutRecs[3], GroupBoxChannelText);
            GuiGroupBox(layoutRecs[4], GroupBoxPosteriorsText);
            GuiGroupBox(layoutRecs[5], GroupBoxGainText);
            if (GuiSpinner(layoutRecs[6], "", &SpinnerChannelValue, 0, 100, SpinnerChannelEditMode)) SpinnerChannelEditMode = !SpinnerChannelEditMode;
            GuiLabel(layoutRecs[7], LabelOutputsText);
            // ScrollPanelChannelScrollOffset = GuiScrollPanel((Rectangle){layoutRecs[8].x, layoutRecs[8].y, layoutRecs[8].width - ScrollPanelChannelBoundsOffset.x, layoutRecs[8].height - ScrollPanelChannelBoundsOffset.y }, layoutRecs[8], ScrollPanelChannelScrollOffset);
            // ScrollPanelGainScrollOffset = GuiScrollPanel((Rectangle){layoutRecs[9].x, layoutRecs[9].y, layoutRecs[9].width - ScrollPanelGainBoundsOffset.x, layoutRecs[9].height - ScrollPanelGainBoundsOffset.y }, layoutRecs[9], ScrollPanelGainScrollOffset);
            if (GuiSpinner(layoutRecs[10], "", &SpinnerGainValue, 0, 100, SpinnerGainEditMode)) SpinnerGainEditMode = !SpinnerGainEditMode;
            GuiLabel(layoutRecs[11], LabelActionsText);
            GuiLabel(layoutRecs[12], LabelGainX1Text);
            GuiLabel(layoutRecs[13], LabelGainX2Text);
            GuiLabel(layoutRecs[14], LabelGainX3Text);
            GuiLabel(layoutRecs[15], LabelGainW1Text);
            GuiLabel(layoutRecs[16], LabelGainW2Text);
            GuiLabel(layoutRecs[17], LabelGainW3Text);
            GuiLabel(layoutRecs[18], LabelChannelX1Text);
            GuiLabel(layoutRecs[19], LabelChannelX2Text);
            GuiLabel(layoutRecs[20], Label039Text);
            GuiLabel(layoutRecs[21], LabelChannelY1Text);
            GuiLabel(layoutRecs[22], LabelChannelY2Text);
            GuiLabel(layoutRecs[23], LabelChannelY3Text);
            GuiLabel(layoutRecs[24], LabelPriorX1Text);
            GuiLabel(layoutRecs[25], LabelPriorX2Text);
            GuiLabel(layoutRecs[26], LabelPriorX3Text);
            GuiGroupBox(layoutRecs[27], GroupBoxVisualizationText);
            GuiGroupBox(layoutRecs[28], GroupBoxDrawingText);
            CheckBoxGainChecked = GuiCheckBox(layoutRecs[29], CheckBoxGainText, CheckBoxGainChecked);
            CheckBoxDrawingChecked = GuiCheckBox(layoutRecs[30], CheckBoxDrawingText, CheckBoxDrawingChecked);
            GuiStatusBar(layoutRecs[31], StatusBarDrawingText);
            GuiLabel(layoutRecs[32], Label054Text);
            // ScrollPanelPosteriorsScrollOffset = GuiScrollPanel((Rectangle){layoutRecs[33].x, layoutRecs[33].y, layoutRecs[33].width - ScrollPanelPosteriorsBoundsOffset.x, layoutRecs[33].height - ScrollPanelPosteriorsBoundsOffset.y }, layoutRecs[33], ScrollPanelPosteriorsScrollOffset);
            GuiLine(layoutRecs[34], NULL);
            GuiLabel(layoutRecs[35], LabelInnerX1Text);
            GuiLabel(layoutRecs[36], LabelInnerX2Text);
            GuiLabel(layoutRecs[37], LabelInnerX3Text);
            GuiLabel(layoutRecs[38], Label072Text);
            GuiLabel(layoutRecs[39], LabelOuter2Text);
            GuiLabel(layoutRecs[40], LabelOuter3Text);
            GuiLabel(layoutRecs[41], LabelOuter1Text);
            
            for(int i = 0; i < TextBoxGainText.size(); i++){
                for(int j = 0; j < TextBoxGainText[i].size(); j++){
                    if (GuiTextBox(layoutRecs[45+(3*i)+j], &(TextBoxGainText[i][j][0]), 128, TextBoxGainEditMode[i][j])) TextBoxGainEditMode[i][j] = !TextBoxGainEditMode[i][j];        
                }
            }

            // if (GuiTextBox(layoutRecs[45], TextBoxGain00Text, 128, TextBoxGain00EditMode)) TextBoxGain00EditMode = !TextBoxGain00EditMode;
            // if (GuiTextBox(layoutRecs[46], TextBoxGain01Text, 128, TextBoxGain01EditMode)) TextBoxGain01EditMode = !TextBoxGain01EditMode;
            // if (GuiTextBox(layoutRecs[47], TextBoxGain02Text, 128, TextBoxGain02EditMode)) TextBoxGain02EditMode = !TextBoxGain02EditMode;
            // if (GuiTextBox(layoutRecs[48], TextBoxGain10Text, 128, TextBoxGain10EditMode)) TextBoxGain10EditMode = !TextBoxGain10EditMode;
            // if (GuiTextBox(layoutRecs[49], TextBoxGain11Text, 128, TextBoxGain11EditMode)) TextBoxGain11EditMode = !TextBoxGain11EditMode;
            // if (GuiTextBox(layoutRecs[50], TextBoxGain12Text, 128, TextBoxGain12EditMode)) TextBoxGain12EditMode = !TextBoxGain12EditMode;
            // if (GuiTextBox(layoutRecs[51], TextBoxGain20Text, 128, TextBoxGain20EditMode)) TextBoxGain20EditMode = !TextBoxGain20EditMode;
            // if (GuiTextBox(layoutRecs[52], TextBoxGain21Text, 128, TextBoxGain21EditMode)) TextBoxGain21EditMode = !TextBoxGain21EditMode;
            // if (GuiTextBox(layoutRecs[53], TextBoxGain22Text, 128, TextBoxGain22EditMode)) TextBoxGain22EditMode = !TextBoxGain22EditMode;

            if (GuiTextBox(layoutRecs[54], TextBoxChannel00Text, 128, TextBoxChannel00EditMode)) TextBoxChannel00EditMode = !TextBoxChannel00EditMode;
            if (GuiTextBox(layoutRecs[55], TextBoxcChannel01Text, 128, TextBoxcChannel01EditMode)) TextBoxcChannel01EditMode = !TextBoxcChannel01EditMode;
            if (GuiTextBox(layoutRecs[56], TextBoxChannel02Text, 128, TextBoxChannel02EditMode)) TextBoxChannel02EditMode = !TextBoxChannel02EditMode;
            if (GuiTextBox(layoutRecs[57], TextBoxChannel10Text, 128, TextBoxChannel10EditMode)) TextBoxChannel10EditMode = !TextBoxChannel10EditMode;
            if (GuiTextBox(layoutRecs[58], TextBoxChannel11Text, 128, TextBoxChannel11EditMode)) TextBoxChannel11EditMode = !TextBoxChannel11EditMode;
            if (GuiTextBox(layoutRecs[59], TextBoxChannel12Text, 128, TextBoxChannel12EditMode)) TextBoxChannel12EditMode = !TextBoxChannel12EditMode;
            if (GuiTextBox(layoutRecs[60], TextBoxChannel20Text, 128, TextBoxChannel20EditMode)) TextBoxChannel20EditMode = !TextBoxChannel20EditMode;
            if (GuiTextBox(layoutRecs[61], TextBoxChannel21Text, 128, TextBoxChannel21EditMode)) TextBoxChannel21EditMode = !TextBoxChannel21EditMode;
            if (GuiTextBox(layoutRecs[62], TextBoxChannel22Text, 128, TextBoxChannel22EditMode)) TextBoxChannel22EditMode = !TextBoxChannel22EditMode;
            if (GuiTextBox(layoutRecs[63], TextBoxPrior1Text, 128, TextBoxPrior1EditMode)) TextBoxPrior1EditMode = !TextBoxPrior1EditMode;
            if (GuiTextBox(layoutRecs[64], TextBoxPrior2Text, 128, TextBoxPrior2EditMode)) TextBoxPrior2EditMode = !TextBoxPrior2EditMode;
            if (GuiTextBox(layoutRecs[65], TextBoxPrior3Text, 128, TextBoxPrior3EditMode)) TextBoxPrior3EditMode = !TextBoxPrior3EditMode;
            if (GuiTextBox(layoutRecs[66], TextBoxOuter1Text, 128, TextBoxOuter1EditMode)) TextBoxOuter1EditMode = !TextBoxOuter1EditMode;
            if (GuiTextBox(layoutRecs[67], TextBoxOuter2Text, 128, TextBoxOuter2EditMode)) TextBoxOuter2EditMode = !TextBoxOuter2EditMode;
            if (GuiTextBox(layoutRecs[68], TextBoxOuter3Text, 128, TextBoxOuter3EditMode)) TextBoxOuter3EditMode = !TextBoxOuter3EditMode;
            if (GuiTextBox(layoutRecs[69], TextBoxInners00Text, 128, TextBoxInners00EditMode)) TextBoxInners00EditMode = !TextBoxInners00EditMode;
            if (GuiTextBox(layoutRecs[70], TextBoxInners01Text, 128, TextBoxInners01EditMode)) TextBoxInners01EditMode = !TextBoxInners01EditMode;
            if (GuiTextBox(layoutRecs[71], TextBoxInners02Text, 128, TextBoxInners02EditMode)) TextBoxInners02EditMode = !TextBoxInners02EditMode;
            if (GuiTextBox(layoutRecs[72], TextBoxInners10Text, 128, TextBoxInners10EditMode)) TextBoxInners10EditMode = !TextBoxInners10EditMode;
            if (GuiTextBox(layoutRecs[73], TextBoxInners11Text, 128, TextBoxInners11EditMode)) TextBoxInners11EditMode = !TextBoxInners11EditMode;
            if (GuiTextBox(layoutRecs[74], TextBoxInners12Text, 128, TextBoxInners12EditMode)) TextBoxInners12EditMode = !TextBoxInners12EditMode;
            if (GuiTextBox(layoutRecs[75], TextBoxInners20Text, 128, TextBoxInners20EditMode)) TextBoxInners20EditMode = !TextBoxInners20EditMode;
            if (GuiTextBox(layoutRecs[76], TextBoxInners21Text, 128, TextBoxInners21EditMode)) TextBoxInners21EditMode = !TextBoxInners21EditMode;
            if (GuiTextBox(layoutRecs[77], TextBoxInners22Text, 128, TextBoxInners22EditMode)) TextBoxInners22EditMode = !TextBoxInners22EditMode;
            if (GuiDropdownBox(layoutRecs[42], DropDownLoadText, &DropDownLoadActive, DropDownLoadEditMode)) DropDownLoadEditMode = !DropDownLoadEditMode;
            if (GuiDropdownBox(layoutRecs[43], DropDownExportText, &DropDownExportActive, DropDownExportEditMode)) DropDownExportEditMode = !DropDownExportEditMode;
            if (GuiDropdownBox(layoutRecs[44], DropdownBox077Text, &DropdownBox077Active, DropdownBox077EditMode)) DropdownBox077EditMode = !DropdownBox077EditMode;
            
            // GuiUnlock();
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
}