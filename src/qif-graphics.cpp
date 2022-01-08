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
#include "../libs/raygui/src/raygui.h"
#include <iostream>
#include <string>
// #include "gui/gui.h"
// #include "data.h"
#include "chull.h"
// #include "random-response.h"
// #include "truncated-geometric.h"
// #include "draw.h"
#include "manager.h"

typedef struct WebLoopVariables{
    Manager manager;
    int mode;
    bool closeWindow;
} WebLoopVariables;

#if defined(PLATFORM_WEB)
    #include "/home/ramon/opt/emsdk/upstream/emscripten/system/include/emscripten.h"
#endif

//----------------------------------------------------------------------------------
// General Functions Declaration
//----------------------------------------------------------------------------------
void updateDrawFrame(void* vars_);     // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(){
    // Initialization
    //---------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "QIF Graphics");
    WebLoopVariables vars;
    vars.closeWindow = false;
    vars.mode = MODE_SINGLE;
    vars.manager.gui.posteriors.resetPosterior(CHANNEL_1);
    // vars.gui.posteriors.resetPosterior(CHANNEL_1);
    GuiSetFont(vars.manager.gui.defaultFont); // Set default font
    vars.manager.draw.initStyle();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(updateDrawFrame, &vars, 120, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while(!vars.closeWindow){    // Detect window close button or ESC key
        updateDrawFrame(&vars);
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// General Functions Definitions (local)
//------------------------------------------------------------------------------------
void updateDrawFrame(void* vars_){
    WebLoopVariables* vars = (WebLoopVariables*) vars_;
    // Gui* gui = &(vars->gui);
    // Data* data = &(vars->data);
    // Draw* draw = &(vars->draw);
    Manager* manager = &(vars->manager);
    int* mode = &(vars->mode);
    bool* closeWindow = &(vars->closeWindow);

    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    *closeWindow = WindowShouldClose();
    Vector2 mousePosition = GetMousePosition();

    if(IsKeyPressed(KEY_Z)){
        TakeScreenshot("pic.png");
    }
    
    // If getting started window is active, nothing can be changed until the window closes
    if(!manager->gui.menu.windowGSActive){

    // Menu
    //----------------------------------------------------------------------------------
    manager->checkButtonsMouseCollision();
    manager->draw.controls.buttonFile(manager->gui, manager->data, closeWindow);
    manager->draw.controls.buttonMode(manager->gui, manager->data, mode); // It must be called after buttonFile function
    manager->draw.controls.buttonExamples(manager->gui, manager->data);
    //----------------------------------------------------------------------------------

    manager->data.error = NO_ERROR;

    // Prior
    //----------------------------------------------------------------------------------
    // Check if a TextBox is being pressed
    if(manager->gui.checkPriorTBoxPressed()){
        manager->gui.drawing = false;
        manager->data.fileSaved = false;
        manager->data.compute[FLAG_PRIOR] = true;
        manager->data.ready[FLAG_PRIOR] = false;
        manager->draw.controls.updateStatusBar(NO_ERROR, manager->gui.visualization);

        if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
            manager->gui.movePriorTBoxes();
        }
    }
    
    manager->checkPriorFlags();
    //----------------------------------------------------------------------------------

    // Channels
    //----------------------------------------------------------------------------------
    // Check if spinner of the current channel has been changed
    if(manager->gui.channel.checkChannelSpinner(*mode)){
        manager->gui.drawing = false;
        manager->data.fileSaved = false;
        if(*mode == MODE_REF){
            manager->data.resetAllExceptComputeChannel1();
        }else{
            manager->data.compute[FLAG_CHANNEL_1+manager->gui.channel.curChannel] = true;
            manager->data.ready[FLAG_CHANNEL_1+manager->gui.channel.curChannel] = false;
        }
        manager->draw.controls.updateStatusBar(NO_ERROR, manager->gui.visualization);
    }
    
    if(*mode == MODE_DP_SINGLE){
        if(manager->data.currentDPMechanism != manager->gui.channel.ddDPActive){
            manager->data.currentDPMechanism = manager->gui.channel.ddDPActive;
            manager->gui.drawing = false;
            manager->gui.visualization.recomputeInners = true;
            manager->data.fileSaved = false;
            manager->data.resetAllExceptComputeChannel1();
        }
        
        if(manager->data.currentDPMechanism == MECH_KRR){
            if(manager->gui.drawing){
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", manager->gui.visualization.sliderDeltaValue);            
                if(manager->gui.visualization.spinEpsilonValue != manager->data.epsilon || strcmp(buffer, manager->gui.channel.tBoxDeltaValue)){
                    if(manager->gui.visualization.spinEpsilonValue <= 0)
                        manager->gui.visualization.spinEpsilonValue = 1;

                    // Update epsilon and delta text boxes
                    sprintf(buffer, "%.6f", manager->gui.visualization.sliderDeltaValue);
                    strcpy(manager->gui.channel.tBoxDeltaValue, buffer);
                    sprintf(buffer, "%d", manager->gui.visualization.spinEpsilonValue);
                    strcpy(manager->gui.channel.tBoxEpsilonValue, buffer);

                    manager->gui.visualization.recomputeInners = true;
                    manager->data.fileSaved = false;
                    manager->data.resetAllExceptComputeChannel1();
                }
            }

            if(manager->gui.checkEpDeltaTBoxPressed()){
                manager->gui.drawing = false;
                manager->data.fileSaved = false;
                manager->data.resetAllExceptComputeChannel1();
                manager->draw.controls.updateStatusBar(NO_ERROR, manager->gui.visualization);
            }
        }else if(manager->data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
            if(manager->gui.drawing){
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", manager->gui.visualization.sliderAlphaValue);
                if(manager->gui.visualization.sliderAlphaValue != manager->data.alpha){
                    // Update Alpha text box
                    sprintf(buffer, "%.6f", manager->gui.visualization.sliderAlphaValue);
                    strcpy(manager->gui.channel.tBoxAlphaValue, buffer);

                    manager->gui.visualization.recomputeInners = true;
                    manager->data.fileSaved = false;
                    manager->data.resetAllExceptComputeChannel1();
                }
            }

            if(manager->gui.checkAlphaTBoxPressed()){
                manager->gui.drawing = false;
                manager->data.fileSaved = false;
                manager->data.resetAllExceptComputeChannel1();
                manager->draw.controls.updateStatusBar(NO_ERROR, manager->gui.visualization);
            }
        }
    }else if(manager->gui.checkChannelTBoxPressed()){ // Check if a channel text box is being pressed
        manager->gui.drawing = false;
        manager->data.fileSaved = false;
        if(*mode == MODE_REF){
            manager->data.resetAllExceptComputeChannel1();
        }else{
            manager->data.compute[FLAG_CHANNEL_1+manager->gui.channel.curChannel] = true;
            manager->data.ready[FLAG_CHANNEL_1+manager->gui.channel.curChannel] = false;
        }

        manager->draw.controls.updateStatusBar(NO_ERROR, manager->gui.visualization);

        if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
            manager->gui.moveChannelTBoxes();
        }
    }

    manager->checkChannelsFlags();
    manager->gui.channel.checkModeAndSizes(*mode);
    manager->gui.channel.setScrollContent();
    //----------------------------------------------------------------------------------

    // Hypers
    //----------------------------------------------------------------------------------
    manager->checkHypersFlags();
    manager->gui.posteriors.setScrollContent(manager->gui.channel.curChannel);
    //----------------------------------------------------------------------------------

    // Visualization
    //----------------------------------------------------------------------------------
    if(manager->gui.drawing){
        // Moving prior with mouse
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && euclidianDistance(manager->data.priorCircle.center, mousePosition) <= PRIOR_RADIUS){
            manager->data.mouseClickedOnPrior = true;
        }
        
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) manager->data.mouseClickedOnPrior = false;

        // User moving prior with mouse pointer
        if(manager->data.mouseClickedOnPrior){
            manager->data.fileSaved = false;
            manager->data.updateHyper(manager->gui.visualization.trianglePoints, *mode);
            for(int i = 0; i < NUMBER_CHANNELS; i++)
                manager->gui.posteriors.numPost[i] = manager->data.hyper[i].num_post;
            manager->data.animation = UPDATE_CIRCLES_BY_MOUSE;
            
            manager->data.buildPriorCircle(manager->gui.visualization.trianglePoints);
            manager->gui.updateRecPriorCircLabel(manager->data.priorCircle);
            manager->gui.updatePriorTBoxes(manager->data.priorObj);

            manager->data.buildInnerCircles(manager->gui.visualization.trianglePoints, CHANNEL_1, *mode);
            manager->gui.updateRecInnersCircLabel(CHANNEL_1, manager->data.innersCircles[CHANNEL_1]);
            if(*mode == MODE_TWO){
                manager->data.buildInnerCircles(manager->gui.visualization.trianglePoints, CHANNEL_2, *mode);
                manager->gui.updateRecInnersCircLabel(CHANNEL_2, manager->data.innersCircles[CHANNEL_2]);
            }else if(*mode == MODE_REF){
                manager->data.buildInnerCircles(manager->gui.visualization.trianglePoints, CHANNEL_3, *mode);
                manager->gui.updateRecInnersCircLabel(CHANNEL_3, manager->data.innersCircles[CHANNEL_3]);
            }
        }

        // User has changed epsilon or delta value
        if(manager->gui.visualization.recomputeInners){
            manager->data.animation = UPDATE_CIRCLES_BY_EPSILON_OR_DELTA;
            manager->data.buildInnerCircles(manager->gui.visualization.trianglePoints, CHANNEL_1, *mode);
            manager->gui.updateRecInnersCircLabel(CHANNEL_1, manager->data.innersCircles[CHANNEL_1]);
            manager->gui.visualization.recomputeInners = false;
        }
        
        if(manager->data.animationRunning){
            // I assume the prior circle was already built    
            manager->data.buildInnerCircles(manager->gui.visualization.trianglePoints, CHANNEL_1, *mode);
            manager->gui.updateRecInnersCircLabel(CHANNEL_1, manager->data.innersCircles[CHANNEL_1]);
            if(*mode == MODE_TWO){
                manager->data.buildInnerCircles(manager->gui.visualization.trianglePoints, CHANNEL_2, *mode);
                manager->gui.updateRecInnersCircLabel(CHANNEL_2, manager->data.innersCircles[CHANNEL_2]);
            }else if(*mode == MODE_REF){
                manager->data.buildInnerCircles(manager->gui.visualization.trianglePoints, CHANNEL_3, *mode);
                manager->gui.updateRecInnersCircLabel(CHANNEL_3, manager->data.innersCircles[CHANNEL_3]);
            }
        }
    }
    //----------------------------------------------------------------------------------
    
    manager->gui.updateHyperTBoxes(manager->data.hyper[manager->gui.channel.curChannel], manager->gui.channel.curChannel, manager->data.ready[FLAG_HYPER_1+manager->gui.channel.curChannel]);

    // Help messages
    //----------------------------------------------------------------------------------
    manager->gui.checkMouseHover(mousePosition);
    //----------------------------------------------------------------------------------

    if(manager->gui.drawing)
        manager->draw.controls.updateStatusBar(NO_ERROR, manager->gui.visualization);

    } // end of if(!manager->gui.menu.windowGettingStartedActive)

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(BG_BASE_COLOR_DARK); 

        if(manager->gui.menu.windowGSActive) GuiLock();
        manager->draw.drawGuiPrior(manager->gui, manager->data);
        manager->draw.drawGuiChannel(manager->gui, manager->data);
        manager->draw.drawGuiPosteriors(manager->gui, manager->data);
        manager->draw.drawGuiVisualization(manager->gui, manager->data);
        manager->draw.drawGuiMenu(manager->gui, manager->data, closeWindow);
        manager->draw.initStyle();
        if(*mode != MODE_DP_SINGLE && *mode != MODE_DP_TWO && *mode != MODE_DP_POST_PROCESS){
            manager->checkHelpMessagesActive(mousePosition);            
            manager->draw.initStyle();
        }
        if(manager->gui.menu.windowGSActive) GuiUnlock();
        manager->draw.drawGettingStarted(manager->gui);

    EndDrawing();
    //-----------------------------------------------------------------------------------
}
