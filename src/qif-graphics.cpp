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

#include <iostream>
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
static void drawGuiMenu(GuiMenu &menu);
static void drawGuiPrior(GuiPrior &prior);
static void drawGuiChannel(GuiChannel &channel);

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
    Vector2 AnchorVisualization = { 380, 65 };            // ANCHOR ID:3
    Vector2 AnchorPosterior = { 10, 450 };            // ANCHOR ID:4
    
    // Define controls variable
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
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
        (Rectangle){ 0, 0 },    // Filling
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
            drawGuiMenu(gui.menu);
            drawGuiPrior(gui.prior);
            drawGuiChannel(gui.channel);
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
static void drawGuiMenu(GuiMenu &menu){
    if (GuiButton(menu.layoutRecs[BUTTON_OPEN], menu.buttonOpenText)) buttonOpen(); 
    if (GuiButton(menu.layoutRecs[BUTTON_SAVE], menu.buttonSaveText)) buttonSave(); 
    if (GuiButton(menu.layoutRecs[BUTTON_EXAMPLES], menu.buttonExamplesText)) buttonExamples(); 
    if (GuiButton(menu.layoutRecs[BUTTON_HELP], menu.buttonHelpText)) buttonHelp(); 
    if (GuiButton(menu.layoutRecs[BUTTON_ABOUT], menu.buttonAboutText)) buttonAbout(); 
    GuiLine(menu.layoutRecs[LINE_MENU], NULL);
}

static void drawGuiPrior(GuiPrior &prior){
    GuiGroupBox(prior.layoutRecsGroupBox, prior.GroupBoxPriorText);
    for(int i = 0; i < 3; i++){
        GuiLabel(prior.layoutRecsLabel[i], prior.LabelPriorText[i]);
        if (GuiTextBox(prior.layoutRecsTextBox[i], prior.TextBoxPriorText[i], 128, prior.TextBoxPriorEditMode[i])) prior.TextBoxPriorEditMode[i] = !prior.TextBoxPriorEditMode[i];

    }
}

static void drawGuiChannel(GuiChannel &channel){
    GuiGroupBox(channel.layoutRecsGroupBox, channel.GroupBoxChannelText);
    if (GuiSpinner(channel.layoutRecsSpinner, "", &(channel.SpinnerChannelValue), 0, 100, channel.SpinnerChannelEditMode)) channel.SpinnerChannelEditMode = !channel.SpinnerChannelEditMode;
    Rectangle viewScrollChannel = GuiScrollPanel(
        (Rectangle){channel.layoutRecsScrollPanel.x, channel.layoutRecsScrollPanel.y, channel.layoutRecsScrollPanel.width - channel.ScrollPanelChannelBoundsOffset.x, channel.layoutRecsScrollPanel.height - channel.ScrollPanelChannelBoundsOffset.y },
        channel.layoutRecsScrollPanel,
        &(channel.ScrollPanelChannelScrollOffset)
    );
    BeginScissorMode(viewScrollChannel.x, viewScrollChannel.y, viewScrollChannel.width, viewScrollChannel.height);
        GuiLabel((Rectangle){channel.layoutRecsLabelOutputs.x + channel.ScrollPanelChannelScrollOffset.x, channel.layoutRecsLabelOutputs.y + channel.ScrollPanelChannelScrollOffset.y, channel.layoutRecsLabelOutputs.width, channel.layoutRecsLabelOutputs.height}, channel.LabelOutputsText);
        for(int i = 0; i < 3; i++){
            GuiLabel((Rectangle){channel.layoutRecsLabelX[i].x + channel.ScrollPanelChannelScrollOffset.x, channel.layoutRecsLabelX[i].y + channel.ScrollPanelChannelScrollOffset.y, channel.layoutRecsLabelX[i].width, channel.layoutRecsLabelX[i].height}, channel.LabelChannelXText[i].c_str());
        
            for(int j = 0; j < channel.numOutputs; j++){
                if (GuiTextBox((Rectangle){channel.layoutRecsTextBoxChannel[i][j].x + channel.ScrollPanelChannelScrollOffset.x, channel.layoutRecsTextBoxChannel[i][j].y + channel.ScrollPanelChannelScrollOffset.y, channel.layoutRecsTextBoxChannel[i][j].width, channel.layoutRecsTextBoxChannel[i][j].height}, channel.TextBoxChannelText[i][j], 128, channel.TextBoxChannelEditMode[i][j]))channel.TextBoxChannelEditMode[i][j] = !channel.TextBoxChannelEditMode[i][j];
            }
        }

        for(int j = 0; j < channel.numOutputs; j++){
            GuiLabel((Rectangle){channel.layoutRecsLabelY[j].x + channel.ScrollPanelChannelScrollOffset.x, channel.layoutRecsLabelY[j].y + channel.ScrollPanelChannelScrollOffset.y, channel.layoutRecsLabelY[j].width, channel.layoutRecsLabelY[j].height}, channel.LabelChannelYText[j].c_str());
        }
        
    EndScissorMode();
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

