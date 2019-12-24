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
    const char *DropdownFileText = "File;Open;Save;Save as";    // DROPDOWNBOX: DropdownFile
    
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
    Vector2 ScrollPanelChannelScrollOffset = { 0, 0 };
    Vector2 ScrollPanelChannelBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelChannel
    Vector2 ScrollPanelGainScrollOffset = { 0, 0 };
    Vector2 ScrollPanelGainBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelGain
    bool SpinnerGainEditMode = false;
    int SpinnerGainValue = 0;            // Spinner: SpinnerGain
    bool ValueChannel00EditMode = false;
    int ValueChannel00Value = 0;            // ValueBOx: ValueChannel00
    bool ValueChannel10EditMode = false;
    int ValueChannel10Value = 0;            // ValueBOx: ValueChannel10
    bool ValueChannel20EditMode = false;
    int ValueChannel20Value = 0;            // ValueBOx: ValueChannel20
    bool ValueChannel21EditMode = false;
    int ValueChannel21Value = 0;            // ValueBOx: ValueChannel21
    bool ValueChannel11EditMode = false;
    int ValueChannel11Value = 0;            // ValueBOx: ValueChannel11
    bool ValueChannel01EditMode = false;
    int ValueChannel01Value = 0;            // ValueBOx: ValueChannel01
    bool ValueChannel02EditMode = false;
    int ValueChannel02Value = 0;            // ValueBOx: ValueChannel02
    bool ValueChannel12EditMode = false;
    int ValueChannel12Value = 0;            // ValueBOx: ValueChannel12
    bool ValueChannel22EditMode = false;
    int ValueChannel22Value = 0;            // ValueBOx: ValueChannel22
    bool ValueGain00EditMode = false;
    int ValueGain00Value = 0;            // ValueBOx: ValueGain00
    bool ValueGain10EditMode = false;
    int ValueGain10Value = 0;            // ValueBOx: ValueGain10
    bool ValueGain20EditMode = false;
    int ValueGain20Value = 0;            // ValueBOx: ValueGain20
    bool ValueGain01EditMode = false;
    int ValueGain01Value = 0;            // ValueBOx: ValueGain01
    bool ValueGain11EditMode = false;
    int ValueGain11Value = 0;            // ValueBOx: ValueGain11
    bool ValueGain21EditMode = false;
    int ValueGain21Value = 0;            // ValueBOx: ValueGain21
    bool ValueGain02EditMode = false;
    int ValueGain02Value = 0;            // ValueBOx: ValueGain02
    bool ValueGain12EditMode = false;
    int ValueGain12Value = 0;            // ValueBOx: ValueGain12
    bool ValueGain22EditMode = false;
    int ValueGain22Value = 0;            // ValueBOx: ValueGain22
    bool ValuePrior1EditMode = false;
    int ValuePrior1Value = 0;            // ValueBOx: ValuePrior1
    bool ValuePrior2EditMode = false;
    int ValuePrior2Value = 0;            // ValueBOx: ValuePrior2
    bool ValuePrior3EditMode = false;
    int ValuePrior3Value = 0;            // ValueBOx: ValuePrior3
    bool CheckBoxGainChecked = false;            // CheckBoxEx: CheckBoxGain
    bool CheckBoxDrawingChecked = false;            // CheckBoxEx: CheckBoxDrawing
    Vector2 ScrollPanelPosteriorsScrollOffset = { 0, 0 };
    Vector2 ScrollPanelPosteriorsBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelPosteriors
    bool ValueOuter1EditMode = false;
    int ValueOuter1Value = 0;            // ValueBOx: ValueOuter1
    bool ValueOuter2EditMode = false;
    int ValueOuter2Value = 0;            // ValueBOx: ValueOuter2
    bool ValueOuter3EditMode = false;
    int ValueOuter3Value = 0;            // ValueBOx: ValueOuter3
    bool ValueInner00EditMode = false;
    int ValueInner00Value = 0;            // ValueBOx: ValueInner00
    bool ValueInner01EditMode = false;
    int ValueInner01Value = 0;            // ValueBOx: ValueInner01
    bool ValueInner02EditMode = false;
    int ValueInner02Value = 0;            // ValueBOx: ValueInner02
    bool ValueInner10EditMode = false;
    int ValueInner10Value = 0;            // ValueBOx: ValueInner10
    bool ValueInner11EditMode = false;
    int ValueInner11Value = 0;            // ValueBOx: ValueInner11
    bool ValueInner12EditMode = false;
    int ValueInner12Value = 0;            // ValueBOx: ValueInner12
    bool ValueInner20EditMode = false;
    int ValueInner20Value = 0;            // ValueBOx: ValueInner20
    bool ValueInner21EditMode = false;
    int ValueInner21Value = 0;            // ValueBOx: ValueInner21
    bool ValueInner22EditMode = false;
    int ValueInner22Value = 0;            // ValueBOx: ValueInner22
    bool DropDownLoadEditMode = false;
    int DropDownLoadActive = 0;            // DropdownBox: DropDownLoad
    bool DropDownExportEditMode = false;
    int DropDownExportActive = 0;            // DropdownBox: DropDownExport
    bool DropdownFileEditMode = false;
    int DropdownFileActive = 0;            // DropdownBox: DropdownFile

    
    //----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 1000;
    int screenHeight = 900;

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
        (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 0, 35, 35 },    // ValueBOx: ValueChannel00
        (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 35, 35, 35 },    // ValueBOx: ValueChannel10
        (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 70, 35, 35 },    // ValueBOx: ValueChannel20
        (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 70, 35, 35 },    // ValueBOx: ValueChannel21
        (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 35, 35, 35 },    // ValueBOx: ValueChannel11
        (Rectangle){ anchorChannel.x + 35, anchorChannel.y + 0, 35, 35 },    // ValueBOx: ValueChannel01
        (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 0, 35, 35 },    // ValueBOx: ValueChannel02
        (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 35, 35, 35 },    // ValueBOx: ValueChannel12
        (Rectangle){ anchorChannel.x + 70, anchorChannel.y + 70, 35, 35 },    // ValueBOx: ValueChannel22
        (Rectangle){ anchorGain.x + 0, anchorGain.y + 0, 35, 35 },    // ValueBOx: ValueGain00
        (Rectangle){ anchorGain.x + 0, anchorGain.y + 35, 35, 35 },    // ValueBOx: ValueGain10
        (Rectangle){ anchorGain.x + 0, anchorGain.y + 70, 35, 35 },    // ValueBOx: ValueGain20
        (Rectangle){ anchorGain.x + 35, anchorGain.y + 0, 35, 35 },    // ValueBOx: ValueGain01
        (Rectangle){ anchorGain.x + 35, anchorGain.y + 35, 35, 35 },    // ValueBOx: ValueGain11
        (Rectangle){ anchorGain.x + 35, anchorGain.y + 70, 35, 35 },    // ValueBOx: ValueGain21
        (Rectangle){ anchorGain.x + 70, anchorGain.y + 0, 35, 35 },    // ValueBOx: ValueGain02
        (Rectangle){ anchorGain.x + 70, anchorGain.y + 35, 35, 35 },    // ValueBOx: ValueGain12
        (Rectangle){ anchorGain.x + 70, anchorGain.y + 70, 35, 35 },    // ValueBOx: ValueGain22
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
        (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 0, 35, 35 },    // ValueBOx: ValuePrior1
        (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 35, 35, 35 },    // ValueBOx: ValuePrior2
        (Rectangle){ anchorPrior.x + 0, anchorPrior.y + 70, 35, 35 },    // ValueBOx: ValuePrior3
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
        (Rectangle){ anchorOuter.x + 0, anchorOuter.y + 0, 35, 35 },    // ValueBOx: ValueOuter1
        (Rectangle){ anchorOuter.x + 35, anchorOuter.y + 0, 35, 35 },    // ValueBOx: ValueOuter2
        (Rectangle){ anchorOuter.x + 70, anchorOuter.y + 0, 35, 35 },    // ValueBOx: ValueOuter3
        (Rectangle){ anchorInners.x + 0, anchorInners.y + 0, 35, 35 },    // ValueBOx: ValueInner00
        (Rectangle){ anchorInners.x + 35, anchorInners.y + 0, 35, 35 },    // ValueBOx: ValueInner01
        (Rectangle){ anchorInners.x + 70, anchorInners.y + 0, 35, 35 },    // ValueBOx: ValueInner02
        (Rectangle){ anchorInners.x + 0, anchorInners.y + 35, 35, 35 },    // ValueBOx: ValueInner10
        (Rectangle){ anchorInners.x + 35, anchorInners.y + 35, 35, 35 },    // ValueBOx: ValueInner11
        (Rectangle){ anchorInners.x + 70, anchorInners.y + 35, 35, 35 },    // ValueBOx: ValueInner12
        (Rectangle){ anchorInners.x + 0, anchorInners.y + 70, 35, 35 },    // ValueBOx: ValueInner20
        (Rectangle){ anchorInners.x + 35, anchorInners.y + 70, 35, 35 },    // ValueBOx: ValueInner21
        (Rectangle){ anchorInners.x + 70, anchorInners.y + 70, 35, 35 },    // ValueBOx: ValueInner22
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
        (Rectangle){ 50, 0, 105, 25 },    // DropdownBox: DropdownFile
    };
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
            // if (DropDownLoadEditMode || DropDownExportEditMode || DropdownFileEditMode) GuiLock();

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
            if (GuiValueBox(layoutRecs[12], "", &ValueChannel00Value, 0, 100, ValueChannel00EditMode)) ValueChannel00EditMode = !ValueChannel00EditMode;
            if (GuiValueBox(layoutRecs[13], "", &ValueChannel10Value, 0, 100, ValueChannel10EditMode)) ValueChannel10EditMode = !ValueChannel10EditMode;
            if (GuiValueBox(layoutRecs[14], "", &ValueChannel20Value, 0, 100, ValueChannel20EditMode)) ValueChannel20EditMode = !ValueChannel20EditMode;
            if (GuiValueBox(layoutRecs[15], "", &ValueChannel21Value, 0, 100, ValueChannel21EditMode)) ValueChannel21EditMode = !ValueChannel21EditMode;
            if (GuiValueBox(layoutRecs[16], "", &ValueChannel11Value, 0, 100, ValueChannel11EditMode)) ValueChannel11EditMode = !ValueChannel11EditMode;
            if (GuiValueBox(layoutRecs[17], "", &ValueChannel01Value, 0, 100, ValueChannel01EditMode)) ValueChannel01EditMode = !ValueChannel01EditMode;
            if (GuiValueBox(layoutRecs[18], "", &ValueChannel02Value, 0, 100, ValueChannel02EditMode)) ValueChannel02EditMode = !ValueChannel02EditMode;
            if (GuiValueBox(layoutRecs[19], "", &ValueChannel12Value, 0, 100, ValueChannel12EditMode)) ValueChannel12EditMode = !ValueChannel12EditMode;
            if (GuiValueBox(layoutRecs[20], "", &ValueChannel22Value, 0, 100, ValueChannel22EditMode)) ValueChannel22EditMode = !ValueChannel22EditMode;
            if (GuiValueBox(layoutRecs[21], "", &ValueGain00Value, 0, 100, ValueGain00EditMode)) ValueGain00EditMode = !ValueGain00EditMode;
            if (GuiValueBox(layoutRecs[22], "", &ValueGain10Value, 0, 100, ValueGain10EditMode)) ValueGain10EditMode = !ValueGain10EditMode;
            if (GuiValueBox(layoutRecs[23], "", &ValueGain20Value, 0, 100, ValueGain20EditMode)) ValueGain20EditMode = !ValueGain20EditMode;
            if (GuiValueBox(layoutRecs[24], "", &ValueGain01Value, 0, 100, ValueGain01EditMode)) ValueGain01EditMode = !ValueGain01EditMode;
            if (GuiValueBox(layoutRecs[25], "", &ValueGain11Value, 0, 100, ValueGain11EditMode)) ValueGain11EditMode = !ValueGain11EditMode;
            if (GuiValueBox(layoutRecs[26], "", &ValueGain21Value, 0, 100, ValueGain21EditMode)) ValueGain21EditMode = !ValueGain21EditMode;
            if (GuiValueBox(layoutRecs[27], "", &ValueGain02Value, 0, 100, ValueGain02EditMode)) ValueGain02EditMode = !ValueGain02EditMode;
            if (GuiValueBox(layoutRecs[28], "", &ValueGain12Value, 0, 100, ValueGain12EditMode)) ValueGain12EditMode = !ValueGain12EditMode;
            if (GuiValueBox(layoutRecs[29], "", &ValueGain22Value, 0, 100, ValueGain22EditMode)) ValueGain22EditMode = !ValueGain22EditMode;
            GuiLabel(layoutRecs[30], LabelGainX1Text);
            GuiLabel(layoutRecs[31], LabelGainX2Text);
            GuiLabel(layoutRecs[32], LabelGainX3Text);
            GuiLabel(layoutRecs[33], LabelGainW1Text);
            GuiLabel(layoutRecs[34], LabelGainW2Text);
            GuiLabel(layoutRecs[35], LabelGainW3Text);
            GuiLabel(layoutRecs[36], LabelChannelX1Text);
            GuiLabel(layoutRecs[37], LabelChannelX2Text);
            GuiLabel(layoutRecs[38], Label039Text);
            GuiLabel(layoutRecs[39], LabelChannelY1Text);
            GuiLabel(layoutRecs[40], LabelChannelY2Text);
            GuiLabel(layoutRecs[41], LabelChannelY3Text);
            if (GuiValueBox(layoutRecs[42], "", &ValuePrior1Value, 0, 100, ValuePrior1EditMode)) ValuePrior1EditMode = !ValuePrior1EditMode;
            if (GuiValueBox(layoutRecs[43], "", &ValuePrior2Value, 0, 100, ValuePrior2EditMode)) ValuePrior2EditMode = !ValuePrior2EditMode;
            if (GuiValueBox(layoutRecs[44], "", &ValuePrior3Value, 0, 100, ValuePrior3EditMode)) ValuePrior3EditMode = !ValuePrior3EditMode;
            GuiLabel(layoutRecs[45], LabelPriorX1Text);
            GuiLabel(layoutRecs[46], LabelPriorX2Text);
            GuiLabel(layoutRecs[47], LabelPriorX3Text);
            GuiGroupBox(layoutRecs[48], GroupBoxVisualizationText);
            GuiGroupBox(layoutRecs[49], GroupBoxDrawingText);
            CheckBoxGainChecked = GuiCheckBox(layoutRecs[50], CheckBoxGainText, CheckBoxGainChecked);
            CheckBoxDrawingChecked = GuiCheckBox(layoutRecs[51], CheckBoxDrawingText, CheckBoxDrawingChecked);
            GuiStatusBar(layoutRecs[52], StatusBarDrawingText);
            GuiLabel(layoutRecs[53], Label054Text);
            // ScrollPanelPosteriorsScrollOffset = GuiScrollPanel((Rectangle){layoutRecs[54].x, layoutRecs[54].y, layoutRecs[54].width - ScrollPanelPosteriorsBoundsOffset.x, layoutRecs[54].height - ScrollPanelPosteriorsBoundsOffset.y }, layoutRecs[54], ScrollPanelPosteriorsScrollOffset);
            if (GuiValueBox(layoutRecs[55], "", &ValueOuter1Value, 0, 100, ValueOuter1EditMode)) ValueOuter1EditMode = !ValueOuter1EditMode;
            if (GuiValueBox(layoutRecs[56], "", &ValueOuter2Value, 0, 100, ValueOuter2EditMode)) ValueOuter2EditMode = !ValueOuter2EditMode;
            if (GuiValueBox(layoutRecs[57], "", &ValueOuter3Value, 0, 100, ValueOuter3EditMode)) ValueOuter3EditMode = !ValueOuter3EditMode;
            if (GuiValueBox(layoutRecs[58], "", &ValueInner00Value, 0, 100, ValueInner00EditMode)) ValueInner00EditMode = !ValueInner00EditMode;
            if (GuiValueBox(layoutRecs[59], "", &ValueInner01Value, 0, 100, ValueInner01EditMode)) ValueInner01EditMode = !ValueInner01EditMode;
            if (GuiValueBox(layoutRecs[60], "", &ValueInner02Value, 0, 100, ValueInner02EditMode)) ValueInner02EditMode = !ValueInner02EditMode;
            if (GuiValueBox(layoutRecs[61], "", &ValueInner10Value, 0, 100, ValueInner10EditMode)) ValueInner10EditMode = !ValueInner10EditMode;
            if (GuiValueBox(layoutRecs[62], "", &ValueInner11Value, 0, 100, ValueInner11EditMode)) ValueInner11EditMode = !ValueInner11EditMode;
            if (GuiValueBox(layoutRecs[63], "", &ValueInner12Value, 0, 100, ValueInner12EditMode)) ValueInner12EditMode = !ValueInner12EditMode;
            if (GuiValueBox(layoutRecs[64], "", &ValueInner20Value, 0, 100, ValueInner20EditMode)) ValueInner20EditMode = !ValueInner20EditMode;
            if (GuiValueBox(layoutRecs[65], "", &ValueInner21Value, 0, 100, ValueInner21EditMode)) ValueInner21EditMode = !ValueInner21EditMode;
            if (GuiValueBox(layoutRecs[66], "", &ValueInner22Value, 0, 100, ValueInner22EditMode)) ValueInner22EditMode = !ValueInner22EditMode;
            GuiLine(layoutRecs[67], NULL);
            GuiLabel(layoutRecs[68], LabelInnerX1Text);
            GuiLabel(layoutRecs[69], LabelInnerX2Text);
            GuiLabel(layoutRecs[70], LabelInnerX3Text);
            GuiLabel(layoutRecs[71], Label072Text);
            GuiLabel(layoutRecs[72], LabelOuter2Text);
            GuiLabel(layoutRecs[73], LabelOuter3Text);
            GuiLabel(layoutRecs[74], LabelOuter1Text);
            if (GuiDropdownBox(layoutRecs[75], DropDownLoadText, &DropDownLoadActive, DropDownLoadEditMode)) DropDownLoadEditMode = !DropDownLoadEditMode;
            if (GuiDropdownBox(layoutRecs[76], DropDownExportText, &DropDownExportActive, DropDownExportEditMode)) DropDownExportEditMode = !DropDownExportEditMode;
            if (GuiDropdownBox(layoutRecs[77], DropdownFileText, &DropdownFileActive, DropdownFileEditMode)) DropdownFileEditMode = !DropdownFileEditMode;
            
            // GuiUnlock();
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
}
