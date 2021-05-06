/*******************************************************************************************
*
*   Qif-graphics v2.0.0 - QIF Graphics
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

#include "../libs/raylib/src/raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include "../libs/raygui/src/raygui.h"

#include "gui/gui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
static void buttonOpen();                // Button: buttonOpen logic
static void buttonSave();                // Button: buttonSave logic
static void buttonExamples();                // Button: buttonExamples logic
static void buttonHelp();                // Button: buttonHelp logic
static void buttonAbout();                // Button: buttonAbout logic
static void buttonDraw();                // Button: buttonDraw logic

//----------------------------------------------------------------------------------
// Draw Functions Declaration
//----------------------------------------------------------------------------------
static void drawMenu(GuiMenu &menu);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 1045;
    int screenHeight = 730;

    InitWindow(screenWidth, screenHeight, "qif-graphics");

    // qif-graphics: controls initialization
    //----------------------------------------------------------------------------------
    Gui gui = Gui();

    // Const text
    const char *GroupBoxPriorText = "Prior distribution";    // GROUPBOX: GroupBoxPrior
    const char *LabelPriorX1Text = "X1";    // LABEL: LabelPriorX1
    const char *LabelPriorX2Text = "X2";    // LABEL: LabelPriorX2
    const char *LabelPriorX3Text = "X3";    // LABEL: LabelPriorX3
    const char *GroupBoxChannelText = "Channel";    // GROUPBOX: GroupBoxChannel
    const char *LabelOutputsText = "Outputs";    // LABEL: LabelOutputs
    const char *LabelChannelX1Text = "X1";    // LABEL: LabelChannelX1
    const char *LabelChannelX2Text = "X2";    // LABEL: LabelChannelX2
    const char *LabelChannelX3Text = "X3";    // LABEL: LabelChannelX3
    const char *LabelChannelY1Text = "Y1";    // LABEL: LabelChannelY1
    const char *LabelChannelY2Text = "Y2";    // LABEL: LabelChannelY2
    const char *LabelChannelY3Text = "Y3";    // LABEL: LabelChannelY3
    const char *GroupBoxPosteriorsText = "Posterior distributions";    // GROUPBOX: GroupBoxPosteriors
    const char *LabelOuterText = "Outer";    // LABEL: LabelOuter
    const char *LabelPosteriors0Text = "I1";    // LABEL: LabelPosteriors0
    const char *LabelPosteriors1Text = "I2";    // LABEL: LabelPosteriors1
    const char *LabelPosteriors2Text = "I3";    // LABEL: LabelPosteriors2
    const char *LabelPosteriorsX1Text = "X1";    // LABEL: LabelPosteriorsX1
    const char *LabelPosteriorsX2Text = "X2";    // LABEL: LabelPosteriorsX2
    const char *LabelPosteriorsX3Text = "X3";    // LABEL: LabelPosteriorsX3
    const char *GroupBoxVisualizationText = "Visualization";    // GROUPBOX: GroupBoxVisualization
    const char *buttonDrawText = "Draw";    // BUTTON: buttonDraw
    
    // Define anchors
    Vector2 AnchorPrior = { 10, 65 };            // ANCHOR ID:1
    Vector2 AnchorChannel = { 10, 185 };            // ANCHOR ID:2
    Vector2 AnchorVisualization = { 380, 65 };            // ANCHOR ID:3
    Vector2 AnchorPosterior = { 10, 450 };            // ANCHOR ID:4
    
    // Define controls variables
    bool TextBoxPrior0EditMode = false;
    char TextBoxPrior0Text[128] = "0";            // TextBox: TextBoxPrior0
    bool TextBoxPrior1EditMode = false;
    char TextBoxPrior1Text[128] = "0";            // TextBox: TextBoxPrior1
    bool TextBoxPrior2EditMode = false;
    char TextBoxPrior2Text[128] = "0";            // TextBox: TextBoxPrior2
    bool SpinnerChannelEditMode = false;
    int SpinnerChannelValue = 0;            // Spinner: SpinnerChannel
    Vector2 ScrollPanelChannelScrollOffset = { 0, 0 };
    Vector2 ScrollPanelChannelBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelChannel
    bool TextBoxChannel00EditMode = false;
    char TextBoxChannel00Text[128] = "0";            // TextBox: TextBoxChannel00
    bool TextBoxChannel01EditMode = false;
    char TextBoxChannel01Text[128] = "0";            // TextBox: TextBoxChannel01
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
    Vector2 ScrollPanelPosteriorsScrollOffset = { 0, 0 };
    Vector2 ScrollPanelPosteriorsBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelPosteriors
    bool TextBoxOuter0EditMode = false;
    char TextBoxOuter0Text[128] = "0";            // TextBox: TextBoxOuter0
    bool TextBoxOuter1EditMode = false;
    char TextBoxOuter1Text[128] = "0";            // TextBox: TextBoxOuter1
    bool TextBoxOuter2EditMode = false;
    char TextBoxOuter2Text[128] = "0";            // TextBox: TextBoxOuter2
    bool TextBoxInners00EditMode = false;
    char TextBoxInners00Text[128] = "0";            // TextBox: TextBoxInners00
    bool TextBoxInners10EditMode = false;
    char TextBoxInners10Text[128] = "0";            // TextBox: TextBoxInners10
    bool TextBoxInners20EditMode = false;
    char TextBoxInners20Text[128] = "0";            // TextBox: TextBoxInners20
    bool TextBoxInners01EditMode = false;
    char TextBoxInners01Text[128] = "0";            // TextBox: TextBoxInners01
    bool TextBoxInners11EditMode = false;
    char TextBoxInners11Text[128] = "0";            // TextBox: TextBoxInners11
    bool TextBoxInners21EditMode = false;
    char TextBoxInners21Text[128] = "0";            // TextBox: TextBoxInners21
    bool TextBoxInners02EditMode = false;
    char TextBoxInners02Text[128] = "0";            // TextBox: TextBoxInners02
    bool TextBoxInners12EditMode = false;
    char TextBoxInners12Text[128] = "0";            // TextBox: TextBoxInners12
    bool TextBoxInners22EditMode = false;
    char TextBoxInners22Text[128] = "0";            // TextBox: TextBoxInners22
    bool TextBoxStatusEditMode = false;
    char TextBoxStatusText[128] = "Status";            // TextBox: TextBoxStatus

    // Define controls rectangles
    Rectangle layoutRecs[57] = {
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ AnchorPrior.x + 0, AnchorPrior.y + 0, 350, 100 },    // GroupBox: GroupBoxPrior
        (Rectangle){ AnchorPrior.x + 125, AnchorPrior.y + 20, 20, 20 },    // Label: LabelPriorX1
        (Rectangle){ AnchorPrior.x + 165, AnchorPrior.y + 20, 20, 20 },    // Label: LabelPriorX2
        (Rectangle){ AnchorPrior.x + 205, AnchorPrior.y + 20, 20, 20 },    // Label: LabelPriorX3
        (Rectangle){ AnchorPrior.x + 115, AnchorPrior.y + 40, 40, 40 },    // TextBox: TextBoxPrior0
        (Rectangle){ AnchorPrior.x + 155, AnchorPrior.y + 40, 40, 40 },    // TextBox: TextBoxPrior1
        (Rectangle){ AnchorPrior.x + 195, AnchorPrior.y + 40, 40, 40 },    // TextBox: TextBoxPrior2
        (Rectangle){ AnchorChannel.x + 0, AnchorChannel.y + 0, 350, 245 },    // GroupBox: GroupBoxChannel
        (Rectangle){ AnchorChannel.x + 245, AnchorChannel.y + 15, 90, 25 },    // Spinner: SpinnerChannel
        (Rectangle){ AnchorChannel.x + 15, AnchorChannel.y + 55, 320, 175 },    // ScrollPanel: ScrollPanelChannel
        (Rectangle){ AnchorChannel.x + 175, AnchorChannel.y + 15, 58, 25 },    // Label: LabelOutputs
        (Rectangle){ AnchorChannel.x + 40, AnchorChannel.y + 90, 20, 40 },    // Label: LabelChannelX1
        (Rectangle){ AnchorChannel.x + 40, AnchorChannel.y + 130, 20, 40 },    // Label: LabelChannelX2
        (Rectangle){ AnchorChannel.x + 40, AnchorChannel.y + 170, 20, 40 },    // Label: LabelChannelX3
        (Rectangle){ AnchorChannel.x + 75, AnchorChannel.y + 70, 20, 20 },    // Label: LabelChannelY1
        (Rectangle){ AnchorChannel.x + 115, AnchorChannel.y + 70, 20, 20 },    // Label: LabelChannelY2
        (Rectangle){ AnchorChannel.x + 155, AnchorChannel.y + 70, 20, 20 },    // Label: LabelChannelY3
        (Rectangle){ AnchorChannel.x + 65, AnchorChannel.y + 90, 40, 40 },    // TextBox: TextBoxChannel00
        (Rectangle){ AnchorChannel.x + 105, AnchorChannel.y + 90, 40, 40 },    // TextBox: TextBoxChannel01
        (Rectangle){ AnchorChannel.x + 145, AnchorChannel.y + 90, 40, 40 },    // TextBox: TextBoxChannel02
        (Rectangle){ AnchorChannel.x + 65, AnchorChannel.y + 130, 40, 40 },    // TextBox: TextBoxChannel10
        (Rectangle){ AnchorChannel.x + 105, AnchorChannel.y + 130, 40, 40 },    // TextBox: TextBoxChannel11
        (Rectangle){ AnchorChannel.x + 145, AnchorChannel.y + 130, 40, 40 },    // TextBox: TextBoxChannel12
        (Rectangle){ AnchorChannel.x + 65, AnchorChannel.y + 170, 40, 40 },    // TextBox: TextBoxChannel20
        (Rectangle){ AnchorChannel.x + 105, AnchorChannel.y + 170, 40, 40 },    // TextBox: TextBoxChannel21
        (Rectangle){ AnchorChannel.x + 145, AnchorChannel.y + 170, 40, 40 },    // TextBox: TextBoxChannel22
        (Rectangle){ AnchorPosterior.x + 0, AnchorPosterior.y + 0, 350, 270 },    // GroupBox: GroupBoxPosteriors
        (Rectangle){ AnchorPosterior.x + 15, AnchorPosterior.y + 20, 320, 235 },    // ScrollPanel: ScrollPanelPosteriors
        (Rectangle){ AnchorPosterior.x + 20, AnchorPosterior.y + 55, 40, 40 },    // Label: LabelOuter
        (Rectangle){ AnchorPosterior.x + 75, AnchorPosterior.y + 35, 20, 20 },    // Label: LabelPosteriors0
        (Rectangle){ AnchorPosterior.x + 115, AnchorPosterior.y + 35, 20, 20 },    // Label: LabelPosteriors1
        (Rectangle){ AnchorPosterior.x + 155, AnchorPosterior.y + 35, 20, 20 },    // Label: LabelPosteriors2
        (Rectangle){ AnchorPosterior.x + 40, AnchorPosterior.y + 115, 20, 40 },    // Label: LabelPosteriorsX1
        (Rectangle){ AnchorPosterior.x + 40, AnchorPosterior.y + 155, 20, 40 },    // Label: LabelPosteriorsX2
        (Rectangle){ AnchorPosterior.x + 40, AnchorPosterior.y + 195, 20, 40 },    // Label: LabelPosteriorsX3
        (Rectangle){ AnchorPosterior.x + 65, AnchorPosterior.y + 55, 40, 40 },    // TextBox: TextBoxOuter0
        (Rectangle){ AnchorPosterior.x + 105, AnchorPosterior.y + 55, 40, 40 },    // TextBox: TextBoxOuter1
        (Rectangle){ AnchorPosterior.x + 145, AnchorPosterior.y + 55, 40, 40 },    // TextBox: TextBoxOuter2
        (Rectangle){ AnchorPosterior.x + 65, AnchorPosterior.y + 115, 40, 40 },    // TextBox: TextBoxInners00
        (Rectangle){ AnchorPosterior.x + 65, AnchorPosterior.y + 155, 40, 40 },    // TextBox: TextBoxInners10
        (Rectangle){ AnchorPosterior.x + 65, AnchorPosterior.y + 195, 40, 40 },    // TextBox: TextBoxInners20
        (Rectangle){ AnchorPosterior.x + 105, AnchorPosterior.y + 115, 40, 40 },    // TextBox: TextBoxInners01
        (Rectangle){ AnchorPosterior.x + 105, AnchorPosterior.y + 155, 40, 40 },    // TextBox: TextBoxInners11
        (Rectangle){ AnchorPosterior.x + 105, AnchorPosterior.y + 195, 40, 40 },    // TextBox: TextBoxInners21
        (Rectangle){ AnchorPosterior.x + 145, AnchorPosterior.y + 115, 40, 40 },    // TextBox: TextBoxInners02
        (Rectangle){ AnchorPosterior.x + 145, AnchorPosterior.y + 155, 40, 40 },    // TextBox: TextBoxInners12
        (Rectangle){ AnchorPosterior.x + 145, AnchorPosterior.y + 195, 40, 40 },    // TextBox: TextBoxInners22
        (Rectangle){ AnchorVisualization.x + 0, AnchorVisualization.y + 0, 655, 655 },    // GroupBox: GroupBoxVisualization
        (Rectangle){ AnchorVisualization.x + 20, AnchorVisualization.y + 20, 80, 32 },    // Button: buttonDraw
        (Rectangle){ AnchorVisualization.x + 115, AnchorVisualization.y + 20, 520, 32 },    // TextBox: TextBoxStatus
        (Rectangle){ AnchorVisualization.x + 20, AnchorVisualization.y + 72, 615, 563 },    // Panel: PanelVisualization
    };
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    GuiLoadStyle("src/gui/style-qif-graphics.rgs");

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
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
            drawMenu(gui.menu);
            GuiGroupBox(layoutRecs[6], GroupBoxPriorText);
            GuiLabel(layoutRecs[7], LabelPriorX1Text);
            GuiLabel(layoutRecs[8], LabelPriorX2Text);
            GuiLabel(layoutRecs[9], LabelPriorX3Text);
            if (GuiTextBox(layoutRecs[10], TextBoxPrior0Text, 128, TextBoxPrior0EditMode)) TextBoxPrior0EditMode = !TextBoxPrior0EditMode;
            if (GuiTextBox(layoutRecs[11], TextBoxPrior1Text, 128, TextBoxPrior1EditMode)) TextBoxPrior1EditMode = !TextBoxPrior1EditMode;
            if (GuiTextBox(layoutRecs[12], TextBoxPrior2Text, 128, TextBoxPrior2EditMode)) TextBoxPrior2EditMode = !TextBoxPrior2EditMode;
            GuiGroupBox(layoutRecs[13], GroupBoxChannelText);
            if (GuiSpinner(layoutRecs[14], "", &SpinnerChannelValue, 0, 100, SpinnerChannelEditMode)) SpinnerChannelEditMode = !SpinnerChannelEditMode;
            Rectangle viewScrollChannel = GuiScrollPanel((Rectangle){layoutRecs[15].x, layoutRecs[15].y, layoutRecs[15].width - ScrollPanelChannelBoundsOffset.x, layoutRecs[15].height - ScrollPanelChannelBoundsOffset.y }, layoutRecs[15], &ScrollPanelChannelScrollOffset);
            BeginScissorMode(viewScrollChannel.x, viewScrollChannel.y, viewScrollChannel.width, viewScrollChannel.height);
                GuiLabel((Rectangle){layoutRecs[16].x + ScrollPanelChannelScrollOffset.x, layoutRecs[16].y + ScrollPanelChannelScrollOffset.y, layoutRecs[16].width, layoutRecs[16].height}, LabelOutputsText);
                GuiLabel((Rectangle){layoutRecs[17].x + ScrollPanelChannelScrollOffset.x, layoutRecs[17].y + ScrollPanelChannelScrollOffset.y, layoutRecs[17].width, layoutRecs[17].height}, LabelChannelX1Text);
                GuiLabel((Rectangle){layoutRecs[18].x + ScrollPanelChannelScrollOffset.x, layoutRecs[18].y + ScrollPanelChannelScrollOffset.y, layoutRecs[18].width, layoutRecs[18].height}, LabelChannelX2Text);
                GuiLabel((Rectangle){layoutRecs[19].x + ScrollPanelChannelScrollOffset.x, layoutRecs[19].y + ScrollPanelChannelScrollOffset.y, layoutRecs[19].width, layoutRecs[19].height}, LabelChannelX3Text);
                GuiLabel((Rectangle){layoutRecs[20].x + ScrollPanelChannelScrollOffset.x, layoutRecs[20].y + ScrollPanelChannelScrollOffset.y, layoutRecs[20].width, layoutRecs[20].height}, LabelChannelY1Text);
                GuiLabel((Rectangle){layoutRecs[21].x + ScrollPanelChannelScrollOffset.x, layoutRecs[21].y + ScrollPanelChannelScrollOffset.y, layoutRecs[21].width, layoutRecs[21].height}, LabelChannelY2Text);
                GuiLabel((Rectangle){layoutRecs[22].x + ScrollPanelChannelScrollOffset.x, layoutRecs[22].y + ScrollPanelChannelScrollOffset.y, layoutRecs[22].width, layoutRecs[22].height}, LabelChannelY3Text);
                if (GuiTextBox((Rectangle){layoutRecs[23].x + ScrollPanelChannelScrollOffset.x, layoutRecs[23].y + ScrollPanelChannelScrollOffset.y, layoutRecs[23].width, layoutRecs[23].height}, TextBoxChannel00Text, 128, TextBoxChannel00EditMode)) TextBoxChannel00EditMode = !TextBoxChannel00EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[24].x + ScrollPanelChannelScrollOffset.x, layoutRecs[24].y + ScrollPanelChannelScrollOffset.y, layoutRecs[24].width, layoutRecs[24].height}, TextBoxChannel01Text, 128, TextBoxChannel01EditMode)) TextBoxChannel01EditMode = !TextBoxChannel01EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[25].x + ScrollPanelChannelScrollOffset.x, layoutRecs[25].y + ScrollPanelChannelScrollOffset.y, layoutRecs[25].width, layoutRecs[25].height}, TextBoxChannel02Text, 128, TextBoxChannel02EditMode)) TextBoxChannel02EditMode = !TextBoxChannel02EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[26].x + ScrollPanelChannelScrollOffset.x, layoutRecs[26].y + ScrollPanelChannelScrollOffset.y, layoutRecs[26].width, layoutRecs[26].height}, TextBoxChannel10Text, 128, TextBoxChannel10EditMode)) TextBoxChannel10EditMode = !TextBoxChannel10EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[27].x + ScrollPanelChannelScrollOffset.x, layoutRecs[27].y + ScrollPanelChannelScrollOffset.y, layoutRecs[27].width, layoutRecs[27].height}, TextBoxChannel11Text, 128, TextBoxChannel11EditMode)) TextBoxChannel11EditMode = !TextBoxChannel11EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[28].x + ScrollPanelChannelScrollOffset.x, layoutRecs[28].y + ScrollPanelChannelScrollOffset.y, layoutRecs[28].width, layoutRecs[28].height}, TextBoxChannel12Text, 128, TextBoxChannel12EditMode)) TextBoxChannel12EditMode = !TextBoxChannel12EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[29].x + ScrollPanelChannelScrollOffset.x, layoutRecs[29].y + ScrollPanelChannelScrollOffset.y, layoutRecs[29].width, layoutRecs[29].height}, TextBoxChannel20Text, 128, TextBoxChannel20EditMode)) TextBoxChannel20EditMode = !TextBoxChannel20EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[30].x + ScrollPanelChannelScrollOffset.x, layoutRecs[30].y + ScrollPanelChannelScrollOffset.y, layoutRecs[30].width, layoutRecs[30].height}, TextBoxChannel21Text, 128, TextBoxChannel21EditMode)) TextBoxChannel21EditMode = !TextBoxChannel21EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[31].x + ScrollPanelChannelScrollOffset.x, layoutRecs[31].y + ScrollPanelChannelScrollOffset.y, layoutRecs[31].width, layoutRecs[31].height}, TextBoxChannel22Text, 128, TextBoxChannel22EditMode)) TextBoxChannel22EditMode = !TextBoxChannel22EditMode;
            EndScissorMode();
            GuiGroupBox(layoutRecs[32], GroupBoxPosteriorsText);
            Rectangle viewScrollPosteriors = GuiScrollPanel((Rectangle){layoutRecs[33].x, layoutRecs[33].y, layoutRecs[33].width - ScrollPanelPosteriorsBoundsOffset.x, layoutRecs[33].height - ScrollPanelPosteriorsBoundsOffset.y }, layoutRecs[33], &ScrollPanelPosteriorsScrollOffset);
            BeginScissorMode(viewScrollPosteriors.x, viewScrollPosteriors.y, viewScrollPosteriors.width, viewScrollPosteriors.height);
                GuiLabel((Rectangle){layoutRecs[34].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[34].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[34].width, layoutRecs[34].height}, LabelOuterText);
                GuiLabel((Rectangle){layoutRecs[35].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[35].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[35].width, layoutRecs[35].height}, LabelPosteriors0Text);
                GuiLabel((Rectangle){layoutRecs[36].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[36].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[36].width, layoutRecs[36].height}, LabelPosteriors1Text);
                GuiLabel((Rectangle){layoutRecs[37].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[37].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[37].width, layoutRecs[37].height}, LabelPosteriors2Text);
                GuiLabel((Rectangle){layoutRecs[38].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[38].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[38].width, layoutRecs[38].height}, LabelPosteriorsX1Text);
                GuiLabel((Rectangle){layoutRecs[39].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[39].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[39].width, layoutRecs[39].height}, LabelPosteriorsX2Text);
                GuiLabel((Rectangle){layoutRecs[40].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[40].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[40].width, layoutRecs[40].height}, LabelPosteriorsX3Text);
                if (GuiTextBox((Rectangle){layoutRecs[41].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[41].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[41].width, layoutRecs[41].height}, TextBoxOuter0Text, 128, TextBoxOuter0EditMode)) TextBoxOuter0EditMode = !TextBoxOuter0EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[42].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[42].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[42].width, layoutRecs[42].height}, TextBoxOuter1Text, 128, TextBoxOuter1EditMode)) TextBoxOuter1EditMode = !TextBoxOuter1EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[43].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[43].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[43].width, layoutRecs[43].height}, TextBoxOuter2Text, 128, TextBoxOuter2EditMode)) TextBoxOuter2EditMode = !TextBoxOuter2EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[44].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[44].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[44].width, layoutRecs[44].height}, TextBoxInners00Text, 128, TextBoxInners00EditMode)) TextBoxInners00EditMode = !TextBoxInners00EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[45].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[45].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[45].width, layoutRecs[45].height}, TextBoxInners10Text, 128, TextBoxInners10EditMode)) TextBoxInners10EditMode = !TextBoxInners10EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[46].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[46].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[46].width, layoutRecs[46].height}, TextBoxInners20Text, 128, TextBoxInners20EditMode)) TextBoxInners20EditMode = !TextBoxInners20EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[47].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[47].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[47].width, layoutRecs[47].height}, TextBoxInners01Text, 128, TextBoxInners01EditMode)) TextBoxInners01EditMode = !TextBoxInners01EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[48].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[48].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[48].width, layoutRecs[48].height}, TextBoxInners11Text, 128, TextBoxInners11EditMode)) TextBoxInners11EditMode = !TextBoxInners11EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[49].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[49].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[49].width, layoutRecs[49].height}, TextBoxInners21Text, 128, TextBoxInners21EditMode)) TextBoxInners21EditMode = !TextBoxInners21EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[50].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[50].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[50].width, layoutRecs[50].height}, TextBoxInners02Text, 128, TextBoxInners02EditMode)) TextBoxInners02EditMode = !TextBoxInners02EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[51].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[51].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[51].width, layoutRecs[51].height}, TextBoxInners12Text, 128, TextBoxInners12EditMode)) TextBoxInners12EditMode = !TextBoxInners12EditMode;
                if (GuiTextBox((Rectangle){layoutRecs[52].x + ScrollPanelPosteriorsScrollOffset.x, layoutRecs[52].y + ScrollPanelPosteriorsScrollOffset.y, layoutRecs[52].width, layoutRecs[52].height}, TextBoxInners22Text, 128, TextBoxInners22EditMode)) TextBoxInners22EditMode = !TextBoxInners22EditMode;
            EndScissorMode();
            GuiGroupBox(layoutRecs[53], GroupBoxVisualizationText);
            if (GuiButton(layoutRecs[54], buttonDrawText)) buttonDraw(); 
            if (GuiTextBox(layoutRecs[55], TextBoxStatusText, 128, TextBoxStatusEditMode)) TextBoxStatusEditMode = !TextBoxStatusEditMode;
            GuiPanel(layoutRecs[56]);
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Draw Functions Definitions (local)
//------------------------------------------------------------------------------------
static void drawMenu(GuiMenu &menu){
    if (GuiButton(menu.layoutRecs[BUTTON_OPEN], menu.buttonOpenText)) buttonOpen(); 
    if (GuiButton(menu.layoutRecs[BUTTON_SAVE], menu.buttonSaveText)) buttonSave(); 
    if (GuiButton(menu.layoutRecs[BUTTON_EXAMPLES], menu.buttonExamplesText)) buttonExamples(); 
    if (GuiButton(menu.layoutRecs[BUTTON_HELP], menu.buttonHelpText)) buttonHelp(); 
    if (GuiButton(menu.layoutRecs[BUTTON_ABOUT], menu.buttonAboutText)) buttonAbout(); 
    GuiLine(menu.layoutRecs[LINE_MENU], NULL);
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
// Button: buttonOpen logic
static void buttonOpen(){
    // TODO: Implement control logic
}
// Button: buttonSave logic
static void buttonSave(){
    // TODO: Implement control logic
}
// Button: buttonExamples logic
static void buttonExamples(){
    // TODO: Implement control logic
}
// Button: buttonHelp logic
static void buttonHelp(){
    // TODO: Implement control logic
}
// Button: buttonAbout logic
static void buttonAbout(){
    // TODO: Implement control logic
}
// Button: buttonDraw logic
static void buttonDraw(){
    // TODO: Implement control logic
}

