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
#include "src/layout.h"
#include "src/information.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include "/home/ramon/raygui/src/raygui.h"

#include <emscripten/emscripten.h>
#include "qif/qif.h"

#include <iostream>

typedef struct LoopVariables{
    Information I;
    Layout L;
}LoopVariables;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void updateDrawFrame(void* V_);     // Update and Draw one frame. Required to run on a browser.
void printError(int error, Layout &L);

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(){
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "QIF-graphics");

    LoopVariables V;
    V.I = Information();
    V.L = Layout();
    V.L.init();
    emscripten_set_main_loop_arg(updateDrawFrame, &V, 0, 1);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        updateDrawFrame(&V);
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
void updateDrawFrame(void* V_){
    // Define controls rectangles
    //----------------------------------------------------------------------------------
    LoopVariables* V = (LoopVariables*) V_;
    Information* I = &(V->I);
    Layout* L = &(V->L);
    int error = NO_ERROR;     // Flag that indicates if an error has been occurred

    // Update
    //----------------------------------------------------------------------------------
        if(L->SpinnerChannelValue != L->recTextBoxChannel.size()){ // If true, spinnerChannel has been changed
            L->CheckBoxDrawingChecked = false;
            I->hyperReady = false;
            L->updateChannel();
        }

        // Check if a TextBox is pressed. If yes, disable drawing.
        if(L->checkTextBoxPressed()){
            L->CheckBoxDrawingChecked = false;
            I->hyperReady = false;
        }

        if(L->CheckBoxDrawingChecked){
            if(I->hyperReady){
                L->updatePosteriors(I->hyper);
            }else{
                // Check if no invalid character has been typed
                if(I->checkPriorText(L->TextBoxPriorText) != NO_ERROR || I->checkChannelText(L->TextBoxChannelText) != NO_ERROR){
                    error = INVALID_VALUE;
                    printError(error, *L);
                    L->CheckBoxDrawingChecked = false;
                }else{
                    // Check if typed numbers represent distributions
                    if(Distribution::isDistribution(I->prior) == false) error = INVALID_PRIOR;
                    else if(Channel::isChannel(I->channel) == false) error = INVALID_CHANNEL;

                    if(error == NO_ERROR){
                        printError(error, *L);

                        Distribution new_prior(I->prior);
                        Channel new_channel(new_prior, I->channel);
                        I->hyper = Hyper(new_channel);

                        I->hyperReady = true;
                        L->updatePosteriors(I->hyper);
                    }else{
                        printError(error, *L);
                        L->CheckBoxDrawingChecked = false;
                    }
                }
            }
        }

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
                GuiPanel(L->recPanelMenu);
                GuiPanel(L->recPanelBody);
            //----------------------------------------------------------------------------------

            // GroupBoxes
            //----------------------------------------------------------------------------------
                GuiGroupBox(L->recGroupBoxPrior, L->GroupBoxPriorText);
                GuiGroupBox(L->recGroupBoxChannel, L->GroupBoxChannelText);
                GuiGroupBox(L->recGroupBoxPosteriors, L->GroupBoxPosteriorsText);
                GuiGroupBox(L->recGroupBoxGain, L->GroupBoxGainText);
                GuiGroupBox(L->recGroupBoxVisualization, L->GroupBoxVisualizationText);
                GuiGroupBox(L->recGroupBoxDrawing, L->GroupBoxDrawingText);
            //----------------------------------------------------------------------------------

            // Spinners
            //----------------------------------------------------------------------------------
                if (GuiSpinner(L->recSpinnerChannel, "", &L->SpinnerChannelValue, 0, 100, L->SpinnerChannelEditMode)) L->SpinnerChannelEditMode = !L->SpinnerChannelEditMode;
            //----------------------------------------------------------------------------------

            // Labels
            //----------------------------------------------------------------------------------
                GuiLabel(L->recLabelOutputs, L->LabelOutputsText);
                GuiLabel(L->recLabelClickDraw, L->LabelClickDrawText);

                // Prior
                for(int i = 0; i < L->LabelPriorText.size(); i++) GuiLabel(L->recLabelPrior[i], &(L->LabelPriorText[i][0]));

                // Channel
                for(int i = 0; i < L->LabelChannelXText.size(); i++) GuiLabel(L->recLabelChannelX[i], &(L->LabelChannelXText[i][0]));
                for(int i = 0; i < L->LabelChannelYText.size(); i++) GuiLabel(L->recLabelChannelY[i], &(L->LabelChannelYText[i][0]));

                // Outer
                GuiLabel(L->recLabelOuterName, L->LabelOuterNameText);
                for(int i = 0; i < L->LabelOuterText.size(); i++) GuiLabel(L->recLabelOuter[i], &(L->LabelOuterText[i][0]));

                // Inners
                for(int i = 0; i < L->LabelInnerText.size(); i++) GuiLabel(L->recLabelInners[i], &(L->LabelInnerText[i][0]));
            //----------------------------------------------------------------------------------

            // CheckBoxes
            //----------------------------------------------------------------------------------
                L->CheckBoxGainChecked = GuiCheckBox(L->recCheckBoxGain, L->CheckBoxGainText, L->CheckBoxGainChecked);
                L->CheckBoxDrawingChecked = GuiCheckBox(L->recCheckBoxDrawing, L->CheckBoxDrawingText, L->CheckBoxDrawingChecked);
            //----------------------------------------------------------------------------------

            // Lines
            //----------------------------------------------------------------------------------
                GuiLine(L->recLine1, NULL);
            //----------------------------------------------------------------------------------

            // StatusBar
            //----------------------------------------------------------------------------------
                GuiStatusBar(L->recStatusBar, &(L->StatusBarDrawingText[0]));
            //----------------------------------------------------------------------------------

            // ScrollPanelChannelScrollOffset = GuiScrollPanel((Rectangle){rec[8].x, rec[8].y, rec[8].width - ScrollPanelChannelBoundsOffset.x, rec[8].height - ScrollPanelChannelBoundsOffset.y }, rec[8], ScrollPanelChannelScrollOffset);
            // ScrollPanelGainScrollOffset = GuiScrollPanel((Rectangle){rec[9].x, rec[9].y, rec[9].width - ScrollPanelGainBoundsOffset.x, rec[9].height - ScrollPanelGainBoundsOffset.y }, rec[9], ScrollPanelGainScrollOffset);
            // ScrollPanelPosteriorsScrollOffset = GuiScrollPanel((Rectangle){rec[33].x, rec[33].y, rec[33].width - ScrollPanelPosteriorsBoundsOffset.x, rec[33].height - ScrollPanelPosteriorsBoundsOffset.y }, rec[33], ScrollPanelPosteriorsScrollOffset);
            
            // TextBoxes
            //----------------------------------------------------------------------------------
                if(L->CheckBoxGainChecked){
                    GuiLabel(L->recLabelActions, L->LabelActionsText);
                    if (GuiSpinner(L->recSpinnerGain, "", &L->SpinnerGainValue, 0, 100, L->SpinnerGainEditMode)) L->SpinnerGainEditMode = !L->SpinnerGainEditMode;

                    // Gain function
                    for(int i = 0; i < L->TextBoxGainText.size(); i++){
                        for(int j = 0; j < L->TextBoxGainText[i].size(); j++){
                            if (GuiTextBox(L->recTextBoxGain[i][j], L->TextBoxGainText[i][j], 128, L->TextBoxGainEditMode[i][j])) L->TextBoxGainEditMode[i][j] = !L->TextBoxGainEditMode[i][j];        
                        }
                    }

                    for(int i = 0; i < L->LabelGainXText.size(); i++) GuiLabel(L->recLabelGainX[i], &(L->LabelGainXText[i][0]));
                    for(int i = 0; i < L->LabelGainWText.size(); i++) GuiLabel(L->recLabelGainW[i], &(L->LabelGainWText[i][0]));
                }
                
                // Channel
                for(int i = 0; i < L->TextBoxChannelText.size(); i++){
                    for(int j = 0; j < L->TextBoxChannelText[i].size(); j++){
                        if (GuiTextBox(L->recTextBoxChannel[i][j], L->TextBoxChannelText[i][j], 128, L->TextBoxChannelEditMode[i][j])) L->TextBoxChannelEditMode[i][j] = !L->TextBoxChannelEditMode[i][j];        
                    }
                }

                // Prior
                for(int i = 0; i < L->TextBoxPriorText.size(); i++){
                    if (GuiTextBox(L->recTextBoxPrior[i], L->TextBoxPriorText[i], 128, L->TextBoxPriorEditMode[i])) L->TextBoxPriorEditMode[i] = !L->TextBoxPriorEditMode[i];        
                }

                GuiLock();
                // Outer
                for(int i = 0; i < L->TextBoxOuterText.size(); i++){
                    if (GuiTextBox(L->recTextBoxOuter[i], L->TextBoxOuterText[i], 128, L->TextBoxOuterEditMode[i])) L->TextBoxOuterEditMode[i] = !L->TextBoxOuterEditMode[i];        
                }

                // Inners
                for(int i = 0; i < L->TextBoxInnersText.size(); i++){
                    for(int j = 0; j < L->TextBoxInnersText[i].size(); j++){
                        if (GuiTextBox(L->recTextBoxInners[i][j], L->TextBoxInnersText[i][j], 128, L->TextBoxInnersEditMode[i][j])) L->TextBoxInnersEditMode[i][j] = !L->TextBoxInnersEditMode[i][j];        
                    }
                }
                GuiUnlock();
            //----------------------------------------------------------------------------------

            // DropDowns
            //----------------------------------------------------------------------------------
                if (GuiDropdownBox(L->recDropDownFile, L->DropDownBoxFileText, &L->DropDownBoxFileActive, L->DropDownBoxFileEditMode)) L->DropDownBoxFileEditMode = !L->DropDownBoxFileEditMode;
                if (GuiDropdownBox(L->recDropDownLoad, L->DropDownLoadText, &L->DropDownLoadActive, L->DropDownLoadEditMode)) L->DropDownLoadEditMode = !L->DropDownLoadEditMode;
                if (GuiDropdownBox(L->recDropDownExport, L->DropDownExportText, &L->DropDownExportActive, L->DropDownExportEditMode)) L->DropDownExportEditMode = !L->DropDownExportEditMode;
            //----------------------------------------------------------------------------------
            
            //----------------------------------------------------------------------------------

        EndDrawing();
    //----------------------------------------------------------------------------------
}

void printError(int error, Layout &L){
    switch(error){
        case INVALID_VALUE:
            L.StatusBarDrawingText = "Some value in prior or channel is invalid!";
            break;
        case INVALID_PRIOR:
            L.StatusBarDrawingText = "The prior distribution is invalid!";
            break;
        case INVALID_CHANNEL:
            L.StatusBarDrawingText = "The channel is invalid!";
            break;
        case NO_ERROR:
            L.StatusBarDrawingText = "Status";
    }
}