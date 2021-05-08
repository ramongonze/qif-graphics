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
#include "data.h"

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
static void drawGuiPosteriors(GuiPosteriors &posteriors);
static void drawGuiVisualization(GuiVisualization &visualization);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 1045;
    int screenHeight = 730;

    InitWindow(screenWidth, screenHeight, "QIF Graphics");

    // qif-graphics: controls initialization
    //----------------------------------------------------------------------------------
    Gui gui = Gui();
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    Data data = Data();
    //--------------------------------------------------------------------------------------

    GuiLoadStyle("src/gui/style-qif-graphics.rgs");

    // Main game loop
    while(!WindowShouldClose()){    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        int error = NO_ERROR;     // Flag that indicates if an error has been occurred
        Vector2 mousePosition = GetMousePosition();
        int numPost;

        if(gui.channel.SpinnerChannelValue != gui.channel.numOutputs){
            data.hyperReady = false;
            gui.channel.updateChannelBySpinner();
        }

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
            drawGuiPosteriors(gui.posteriors);
            drawGuiVisualization(gui.visualization);
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
    if (GuiButton(menu.layoutRecsButtons[REC_BUTTON_OPEN], menu.buttonOpenText)) buttonOpen(); 
    if (GuiButton(menu.layoutRecsButtons[REC_BUTTON_SAVE], menu.buttonSaveText)) buttonSave(); 
    if (GuiButton(menu.layoutRecsButtons[REC_BUTTON_EXAMPLES], menu.buttonExamplesText)) buttonExamples(); 
    if (GuiButton(menu.layoutRecsButtons[REC_BUTTON_HELP], menu.buttonHelpText)) buttonHelp(); 
    if (GuiButton(menu.layoutRecsButtons[REC_BUTTON_ABOUT], menu.buttonAboutText)) buttonAbout(); 
    GuiLine(menu.layoutRecsLine, NULL);
}

static void drawGuiPrior(GuiPrior &prior){
    GuiGroupBox(prior.layoutRecsGroupBox, prior.GroupBoxPriorText);
    for(int i = 0; i < 3; i++){
        GuiLabel(prior.layoutRecsLabel[i], prior.LabelPriorText[i].c_str());
        if (GuiTextBox(prior.layoutRecsTextBox[i], prior.TextBoxPriorText[i], 128, prior.TextBoxPriorEditMode[i])) prior.TextBoxPriorEditMode[i] = !prior.TextBoxPriorEditMode[i];

    }
}

static void drawGuiChannel(GuiChannel &channel){
    GuiGroupBox(channel.layoutRecsGroupBox, channel.GroupBoxChannelText);
    if (GuiSpinner(channel.layoutRecsSpinner, "", &(channel.SpinnerChannelValue), 0, 100, channel.SpinnerChannelEditMode)) channel.SpinnerChannelEditMode = !channel.SpinnerChannelEditMode;
    Rectangle viewScrollChannel = GuiScrollPanel(
        (Rectangle){channel.layoutRecsScrollPanel.x, channel.layoutRecsScrollPanel.y, channel.layoutRecsScrollPanel.width - channel.ScrollPanelChannelBoundsOffset.x, channel.layoutRecsScrollPanel.height - channel.ScrollPanelChannelBoundsOffset.y },
        (Rectangle){channel.layoutRecsScrollPanel.x, channel.layoutRecsScrollPanel.y, channel.ScrollPanelChannelContent.x, channel.ScrollPanelChannelContent.y},
        &(channel.ScrollPanelChannelScrollOffset)
    );
    BeginScissorMode(viewScrollChannel.x, viewScrollChannel.y, viewScrollChannel.width, viewScrollChannel.height);
        GuiLabel((Rectangle){channel.layoutRecsLabelOutputs.x + channel.ScrollPanelChannelScrollOffset.x, channel.layoutRecsLabelOutputs.y + channel.ScrollPanelChannelScrollOffset.y, channel.layoutRecsLabelOutputs.width, channel.layoutRecsLabelOutputs.height}, channel.LabelOutputsText);
        for(int i = 0; i < channel.numOutputs; i++){
            GuiLabel((Rectangle){channel.layoutRecsLabelY[i].x + channel.ScrollPanelChannelScrollOffset.x, channel.layoutRecsLabelY[i].y + channel.ScrollPanelChannelScrollOffset.y, channel.layoutRecsLabelY[i].width, channel.layoutRecsLabelY[i].height}, channel.LabelChannelYText[i].c_str());
            for(int j = 0; j < 3; j++){
                if (GuiTextBox((Rectangle){channel.layoutRecsTextBoxChannel[i][j].x + channel.ScrollPanelChannelScrollOffset.x, channel.layoutRecsTextBoxChannel[i][j].y + channel.ScrollPanelChannelScrollOffset.y, channel.layoutRecsTextBoxChannel[i][j].width, channel.layoutRecsTextBoxChannel[i][j].height}, channel.TextBoxChannelText[i][j], 128, channel.TextBoxChannelEditMode[i][j]))channel.TextBoxChannelEditMode[i][j] = !channel.TextBoxChannelEditMode[i][j];
            }
        }

        for(int i = 0; i < 3; i++){
            GuiLabel((Rectangle){channel.layoutRecsLabelX[i].x + channel.ScrollPanelChannelScrollOffset.x, channel.layoutRecsLabelX[i].y + channel.ScrollPanelChannelScrollOffset.y, channel.layoutRecsLabelX[i].width, channel.layoutRecsLabelX[i].height}, channel.LabelChannelXText[i].c_str());
        }
    EndScissorMode();
}

static void drawGuiPosteriors(GuiPosteriors &posteriors){
    GuiGroupBox(posteriors.layoutRecsGroupBox, posteriors.GroupBoxPosteriorsText);
    Rectangle viewScrollPosteriors = GuiScrollPanel(
        (Rectangle){posteriors.layoutRecsScrollPanel.x, posteriors.layoutRecsScrollPanel.y, posteriors.layoutRecsScrollPanel.width - posteriors.ScrollPanelPosteriorsBoundsOffset.x, posteriors.layoutRecsScrollPanel.height - posteriors.ScrollPanelPosteriorsBoundsOffset.y },
        (Rectangle){posteriors.layoutRecsScrollPanel.x, posteriors.layoutRecsScrollPanel.y, posteriors.ScrollPanelPosteriorsContent.x, posteriors.ScrollPanelPosteriorsContent.y},
        &(posteriors.ScrollPanelPosteriorsScrollOffset)
    );
    BeginScissorMode(viewScrollPosteriors.x, viewScrollPosteriors.y, viewScrollPosteriors.width, viewScrollPosteriors.height);
        GuiLabel((Rectangle){posteriors.layoutRecsLabelOuter.x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsLabelOuter.y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsLabelOuter.width, posteriors.layoutRecsLabelOuter.height}, posteriors.LabelOuterText);

        for(int i = 0; i < posteriors.numPosteriors; i++){
            GuiLabel((Rectangle){posteriors.layoutRecsLabelPosteriors[i].x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsLabelPosteriors[i].y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsLabelPosteriors[i].width, posteriors.layoutRecsLabelPosteriors[i].height}, posteriors.LabelPosteriorsText[i].c_str());
        }

        for(int i = 0; i < 3; i++){
            GuiLabel((Rectangle){posteriors.layoutRecsLabelX[i].x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsLabelX[i].y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsLabelX[i].width, posteriors.layoutRecsLabelX[i].height}, posteriors.LabelPosteriorsXText[i].c_str());
        }

        for(int i = 0; i < posteriors.numPosteriors; i++){
            GuiTextBox((Rectangle){posteriors.layoutRecsTextBoxOuter[i].x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsTextBoxOuter[i].y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsTextBoxOuter[i].width, posteriors.layoutRecsTextBoxOuter[i].height}, posteriors.TextBoxOuterText[i], 128, posteriors.TextBoxOuterEditMode[i]);
        }

        for(int i = 0; i < posteriors.numPosteriors; i++){
            for(int j = 0; j < 3; j++){
                GuiTextBox((Rectangle){posteriors.layoutRecsTextBoxInners[i][j].x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsTextBoxInners[i][j].y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsTextBoxInners[i][j].width, posteriors.layoutRecsTextBoxInners[i][j].height}, posteriors.TextBoxInnersText[i][j], 128, posteriors.TextBoxInnersEditMode[i][j]);
            }
        }
    EndScissorMode();
}

static void drawGuiVisualization(GuiVisualization &visualization){
    GuiGroupBox(visualization.layoutRecsGroupBoxVisualization, visualization.GroupBoxVisualizationText);
    if (GuiButton(visualization.layoutRecsButtonDraw, visualization.ButtonDrawText)) buttonDraw(); 
    GuiTextBox(visualization.layoutRecsTextBoxStatus, visualization.TextBoxStatusText, 128, visualization.TextBoxStatusEditMode);
    GuiPanel(visualization.layoutRecsPanelVisualization);
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

