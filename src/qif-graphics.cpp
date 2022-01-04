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
#include "gui/gui.h"
#include "data.h"
#include "chull.h"
#include "random-response.h"
#include "truncated-geometric.h"
#include "draw.h"

typedef struct WebLoopVariables{
    Gui gui;
    Data data;
    Draw draw;
    bool closeWindow;
    int mode;
} WebLoopVariables;

#if defined(PLATFORM_WEB)
    #include "/home/ramon/opt/emsdk/upstream/emscripten/system/include/emscripten.h"
#endif

//----------------------------------------------------------------------------------
// General Functions Declaration
//----------------------------------------------------------------------------------
void updateDrawFrame(void* vars_);     // Update and Draw one frame
void readFonts(Font* defaultFont, Font* defaultFontBig);
void checkButtonsMouseCollision(Gui &gui);
void checkHelpMessagesActive(Gui &gui, Draw &draw, Vector2 mousePosition);
void checkPriorFlags(Gui &gui, Data &data); // Check compute and ready prior flags
void checkChannelsFlags(Gui &gui, Data &data); // Check compute and ready channels flags
void checkHypersFlags(Gui &gui, Data &data); // Check compute and ready hypers flags

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
    vars.gui.posteriors.resetPosterior(CHANNEL_1);
    GuiSetFont(vars.gui.defaultFont); // Set default font
    vars.draw.initStyle();

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
    Gui* gui = &(vars->gui);
    Data* data = &(vars->data);
    Draw* draw = &(vars->draw);
    bool* closeWindow = &(vars->closeWindow);
    int* mode = &(vars->mode);

    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    *closeWindow = WindowShouldClose();
    Vector2 mousePosition = GetMousePosition();

    if(IsKeyPressed(KEY_Z)){
        TakeScreenshot("pic.png");
    }
    
    // If getting started window is active, nothing can be changed until the window closes
    if(!gui->menu.windowGSActive){

    // Menu
    //----------------------------------------------------------------------------------
    checkButtonsMouseCollision(*gui);
    draw->controls.buttonFile(*gui, *data, closeWindow);
    draw->controls.buttonMode(*gui, *data, mode); // It must be called after buttonFile function
    draw->controls.buttonExamples(*gui, *data);
    //----------------------------------------------------------------------------------

    data->error = NO_ERROR;

    // Prior
    //----------------------------------------------------------------------------------
    // Check if a TextBox is being pressed
    if(gui->checkPriorTBoxPressed()){
        gui->drawing = false;
        data->fileSaved = false;
        data->compute[FLAG_PRIOR] = true;
        data->ready[FLAG_PRIOR] = false;
        draw->controls.updateStatusBar(NO_ERROR, gui->visualization);

        if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
            gui->movePriorTBoxes();
        }
    }
    
    checkPriorFlags(*gui, *data);
    //----------------------------------------------------------------------------------

    // Channels
    //----------------------------------------------------------------------------------
    // Check if spinner of the current channel has been changed
    if(gui->channel.checkChannelSpinner(*mode)){
        gui->drawing = false;
        data->fileSaved = false;
        if(*mode == MODE_REF){
            data->resetAllExceptComputeChannel1();
        }else{
            data->compute[FLAG_CHANNEL_1+gui->channel.curChannel] = true;
            data->ready[FLAG_CHANNEL_1+gui->channel.curChannel] = false;
        }
        draw->controls.updateStatusBar(NO_ERROR, gui->visualization);
    }
    
    if(*mode == MODE_DP_SINGLE){
        if(data->currentDPMechanism != gui->channel.ddDPActive){
            data->currentDPMechanism = gui->channel.ddDPActive;
            gui->drawing = false;
            gui->visualization.recomputeInners = true;
            data->fileSaved = false;
            data->resetAllExceptComputeChannel1();
        }
        
        if(data->currentDPMechanism == MECH_KRR){
            if(gui->drawing){
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", gui->visualization.sliderDeltaValue);            
                if(gui->visualization.spinEpsilonValue != data->epsilon || strcmp(buffer, gui->channel.tBoxDeltaValue)){
                    if(gui->visualization.spinEpsilonValue <= 0)
                        gui->visualization.spinEpsilonValue = 1;

                    // Update epsilon and delta text boxes
                    sprintf(buffer, "%.6f", gui->visualization.sliderDeltaValue);
                    strcpy(gui->channel.tBoxDeltaValue, buffer);
                    sprintf(buffer, "%d", gui->visualization.spinEpsilonValue);
                    strcpy(gui->channel.tBoxEpsilonValue, buffer);

                    gui->visualization.recomputeInners = true;
                    data->fileSaved = false;
                    data->resetAllExceptComputeChannel1();
                }
            }

            if(gui->checkEpDeltaTBoxPressed()){
                gui->drawing = false;
                data->fileSaved = false;
                data->resetAllExceptComputeChannel1();
                draw->controls.updateStatusBar(NO_ERROR, gui->visualization);
            }
        }else if(data->currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
            if(gui->drawing){
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", gui->visualization.sliderAlphaValue);
                if(gui->visualization.sliderAlphaValue != data->alpha){
                    // Update Alpha text box
                    sprintf(buffer, "%.6f", gui->visualization.sliderAlphaValue);
                    strcpy(gui->channel.tBoxAlphaValue, buffer);

                    gui->visualization.recomputeInners = true;
                    data->fileSaved = false;
                    data->resetAllExceptComputeChannel1();
                }
            }

            if(gui->checkAlphaTBoxPressed()){
                gui->drawing = false;
                data->fileSaved = false;
                data->resetAllExceptComputeChannel1();
                draw->controls.updateStatusBar(NO_ERROR, gui->visualization);
            }
        }
    }else if(gui->checkChannelTBoxPressed()){ // Check if a channel text box is being pressed
        gui->drawing = false;
        data->fileSaved = false;
        if(*mode == MODE_REF){
            data->resetAllExceptComputeChannel1();
        }else{
            data->compute[FLAG_CHANNEL_1+gui->channel.curChannel] = true;
            data->ready[FLAG_CHANNEL_1+gui->channel.curChannel] = false;
        }

        draw->controls.updateStatusBar(NO_ERROR, gui->visualization);

        if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
            gui->moveChannelTBoxes();
        }
    }

    checkChannelsFlags(*gui, *data);
    gui->channel.checkModeAndSizes(*mode);
    gui->channel.setScrollContent();
    //----------------------------------------------------------------------------------

    // Hypers
    //----------------------------------------------------------------------------------
    checkHypersFlags(*gui, *data);
    gui->posteriors.setScrollContent(gui->channel.curChannel);
    //----------------------------------------------------------------------------------

    // Visualization
    //----------------------------------------------------------------------------------
    if(gui->drawing){
        // Moving prior with mouse
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && euclidianDistance(data->priorCircle.center, mousePosition) <= PRIOR_RADIUS){
            data->mouseClickedOnPrior = true;
        }
        
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) data->mouseClickedOnPrior = false;

        // User moving prior with mouse pointer
        if(data->mouseClickedOnPrior){
            data->fileSaved = false;
            data->updateHyper(gui->visualization.trianglePoints, *mode);
            for(int i = 0; i < NUMBER_CHANNELS; i++)
                gui->posteriors.numPost[i] = data->hyper[i].num_post;
            data->animation = UPDATE_CIRCLES_BY_MOUSE;
            
            data->buildPriorCircle(gui->visualization.trianglePoints);
            gui->updateRecPriorCircLabel(data->priorCircle);
            gui->updatePriorTBoxes(data->priorObj);

            data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_1, *mode);
            gui->updateRecInnersCircLabel(CHANNEL_1, data->innersCircles[CHANNEL_1]);
            if(*mode == MODE_TWO){
                data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_2, *mode);
                gui->updateRecInnersCircLabel(CHANNEL_2, data->innersCircles[CHANNEL_2]);
            }else if(*mode == MODE_REF){
                data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_3, *mode);
                gui->updateRecInnersCircLabel(CHANNEL_3, data->innersCircles[CHANNEL_3]);
            }
        }

        // User has changed epsilon or delta value
        if(gui->visualization.recomputeInners){
            data->animation = UPDATE_CIRCLES_BY_EPSILON_OR_DELTA;
            data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_1, *mode);
            gui->updateRecInnersCircLabel(CHANNEL_1, data->innersCircles[CHANNEL_1]);
            gui->visualization.recomputeInners = false;
        }
        
        if(data->animationRunning){
            // I assume the prior circle was already built    
            data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_1, *mode);
            gui->updateRecInnersCircLabel(CHANNEL_1, data->innersCircles[CHANNEL_1]);
            if(*mode == MODE_TWO){
                data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_2, *mode);
                gui->updateRecInnersCircLabel(CHANNEL_2, data->innersCircles[CHANNEL_2]);
            }else if(*mode == MODE_REF){
                data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_3, *mode);
                gui->updateRecInnersCircLabel(CHANNEL_3, data->innersCircles[CHANNEL_3]);
            }
        }
    }
    //----------------------------------------------------------------------------------
    
    gui->updateHyperTBoxes(data->hyper[gui->channel.curChannel], gui->channel.curChannel, data->ready[FLAG_HYPER_1+gui->channel.curChannel]);

    // Help messages
    //----------------------------------------------------------------------------------
    gui->checkMouseHover(mousePosition);
    //----------------------------------------------------------------------------------

    if(gui->drawing)
        draw->controls.updateStatusBar(NO_ERROR, gui->visualization);

    } // end of if(!gui->menu.windowGettingStartedActive)

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(BG_BASE_COLOR_DARK); 

        if(gui->menu.windowGSActive) GuiLock();
        draw->drawGuiPrior(*gui, *data);
        draw->drawGuiChannel(*gui, *data);
        draw->drawGuiPosteriors(*gui, *data);
        draw->drawGuiVisualization(*gui, *data);
        draw->drawGuiMenu(*gui, *data, closeWindow);
        draw->initStyle();
        if(*mode != MODE_DP_SINGLE && *mode != MODE_DP_TWO && *mode != MODE_DP_POST_PROCESS){
            checkHelpMessagesActive(*gui, *draw, mousePosition);            
            draw->initStyle();
        }
        if(gui->menu.windowGSActive) GuiUnlock();
        draw->drawGettingStarted(*gui);

    EndDrawing();
    //-----------------------------------------------------------------------------------
}

void checkButtonsMouseCollision(Gui &gui){
    // Active the button the mouse is over it
    int i = -1;
    Vector2 mousePoint = GetMousePosition();

    #if !defined(PLATFORM_WEB)
        if((gui.menu.ddEdit[BUTTON_FILE] || gui.menu.ddEdit[BUTTON_MODE] || gui.menu.ddEdit[BUTTON_EXAMPLES])
            &&
            (CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_FILE]) || CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_MODE]) ||
            CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_EXAMPLES]) || CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_GUIDE]))){
            i = 0; // File button is the first one
        }
    #else
        if((gui.menu.dropdownEditMode[BUTTON_MODE] || gui.menu.dropdownEditMode[BUTTON_EXAMPLES])
            &&
            (CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_MODE]) || CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_EXAMPLES]) ||
            CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_GUIDE]))){
            i = 1; // Mode button is the first one
        }
    #endif
    
    if(i >= 0){
        for(; i < 3; i++){
            if(i != BUTTON_MODE)
                gui.menu.ddActive[i] = 0;
                
            if(CheckCollisionPointRec(mousePoint, gui.menu.recButtons[i]))
                gui.menu.ddEdit[i] = true;
            else
                gui.menu.ddEdit[i] = false;
        }
    }
}

void checkHelpMessagesActive(Gui &gui, Draw &draw, Vector2 mousePosition){
    if(gui.helpMessagesActive[HELP_MSG_BUTTON_PRIOR])
        draw.drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 220, 60}, gui.helpMessages[HELP_MSG_BUTTON_PRIOR]);

    if(gui.helpMessagesActive[HELP_MSG_BUTTON_CHANNEL])
        draw.drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 300, 60}, gui.helpMessages[HELP_MSG_BUTTON_CHANNEL]);

    if(gui.helpMessagesActive[HELP_MSG_BUTTON_DRAW])
        draw.drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 300, 90}, gui.helpMessages[HELP_MSG_BUTTON_DRAW]);
}

void checkPriorFlags(Gui &gui, Data &data){
    if(data.compute[FLAG_PRIOR]){
        if(data.checkPriorText(gui.prior.tBoxPriorTxt) == NO_ERROR){
            if(Distribution::isDistribution(data.prior)){
                data.priorObj = Distribution(data.prior);
                data.ready[FLAG_PRIOR] = true;
            }else{
                data.error = INVALID_PRIOR;
                data.ready[FLAG_PRIOR] = false;
            }
        }else{
            data.error = INVALID_VALUE_PRIOR;
            data.ready[FLAG_PRIOR] = false;
        }
        
        data.compute[FLAG_PRIOR] = false;

        if(data.error == INVALID_PRIOR || data.error == INVALID_VALUE_PRIOR){
            for(int channel = 0; channel < NUMBER_CHANNELS; channel++){
                data.ready[FLAG_HYPER_1+channel] = false;
                gui.posteriors.resetPosterior(channel);
                
                if(data.ready[FLAG_CHANNEL_1+channel]){
                    data.ready[FLAG_CHANNEL_1+channel] = false;
                    data.compute[FLAG_CHANNEL_1+channel] = true;
                }
            }
        }
    }else if(!data.ready[FLAG_PRIOR]){
        data.error = INVALID_PRIOR;
    }
}

void checkChannelsFlags(Gui &gui, Data &data){
    // If prior is not ready, there is no reason to compute channel
    if(!data.ready[FLAG_PRIOR])
        return;

    int mode = gui.menu.ddActive[BUTTON_MODE];
    
    if(mode == MODE_DP_SINGLE){
        if(data.compute[FLAG_CHANNEL_1]){
            int ret = NO_ERROR;
            if(data.currentDPMechanism == MECH_KRR)
                ret = data.checkEpsilonDeltaText(gui.channel.tBoxEpsilonValue, gui.channel.tBoxDeltaValue);
            else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED)
                ret = data.checkAlphaText(gui.channel.tBoxAlphaValue);

            if(ret == NO_ERROR){
                if(data.currentDPMechanism == MECH_KRR){
                    random_response rr;
                    data.channel[CHANNEL_1] = rr.get_channel(3, log(data.epsilon), data.delta);
                }else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
                    truncated_geometric tg;
                    data.channel[CHANNEL_1] = tg.get_channel(2, data.alpha);
                }

                data.channelObj[CHANNEL_1] = Channel(data.priorObj, data.channel[CHANNEL_1]);
                gui.updateChannelTBoxes(data.channelObj[CHANNEL_1], CHANNEL_1);
                data.ready[FLAG_CHANNEL_1] = true;
                data.compute[FLAG_HYPER_1] = true;
            }else{
                data.error = ret;
                gui.posteriors.resetPosterior(CHANNEL_1);
                gui.channel.resetChannel(CHANNEL_1);
                data.ready[FLAG_HYPER_1] = false;
            }
            data.compute[FLAG_CHANNEL_1] = false;
        }else if(!data.ready[FLAG_CHANNEL_1]){
            if(data.currentDPMechanism == MECH_KRR)
                data.error = INVALID_EPSILON_OR_DELTA;
            else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED)
                data.error = INVALID_ALPHA;
        }
    }else{
        for(int channel = 0; channel < NUMBER_CHANNELS; channel++){
            if(data.compute[FLAG_CHANNEL_1+channel]){
                if(channel == CHANNEL_3){
                    // Multiply channels C and R
                    data.channelObj[CHANNEL_3] = composeChannels(data.channelObj[CHANNEL_1], data.channelObj[CHANNEL_2]);
                    data.channel[CHANNEL_3] = data.channelObj[CHANNEL_3].matrix;
                    gui.updateChannelTBoxes(data.channelObj[CHANNEL_3], CHANNEL_3);
                    data.ready[FLAG_CHANNEL_3] = true;
                    data.compute[FLAG_HYPER_3] = true; // Set hyper to be computed
                }else if(data.checkChannelText(gui.channel.tBoxChannelTxt[channel], channel, gui.channel.numSecrets[channel], gui.channel.numOutputs[channel]) == NO_ERROR){    
                    if(Channel::isChannel(data.channel[channel])){
                        if(channel == CHANNEL_2 && gui.menu.ddActive[BUTTON_MODE] == MODE_REF){
                            data.fakePrior = Distribution(gui.channel.numSecrets[CHANNEL_2], "uniform");
                            data.channelObj[CHANNEL_2] = Channel(data.fakePrior, data.channel[CHANNEL_2]);
                            if(data.ready[FLAG_CHANNEL_1])
                                data.compute[FLAG_CHANNEL_3] = true;
                        }else{
                            data.channelObj[channel] = Channel(data.priorObj, data.channel[channel]);
                            data.compute[FLAG_HYPER_1+channel] = true; // Set hyper to be computed
                            if(channel == CHANNEL_1 && gui.menu.ddActive[BUTTON_MODE] == MODE_REF)
                                data.compute[FLAG_CHANNEL_2] = true;
                        }
                        data.ready[FLAG_CHANNEL_1+channel] = true;
                    }else{
                        data.error = INVALID_CHANNEL_1 + channel;   
                    }
                }else{
                    data.error = INVALID_VALUE_CHANNEL_1 + channel;
                }

                data.compute[FLAG_CHANNEL_1+channel] = false;

                if(data.error == INVALID_CHANNEL_1+channel || data.error == INVALID_VALUE_CHANNEL_1+channel){
                    data.ready[FLAG_CHANNEL_1+channel] = false;
                    data.ready[FLAG_HYPER_1+channel] = false;
                    data.compute[FLAG_HYPER_1+channel] = false;
                    gui.posteriors.resetPosterior(channel);

                    if(mode == MODE_REF){
                        data.ready[FLAG_CHANNEL_3] = false;
                        data.compute[FLAG_CHANNEL_3] = false;
                        data.ready[FLAG_HYPER_3] = false;
                        data.compute[FLAG_HYPER_3] = false;
                        gui.posteriors.resetPosterior(CHANNEL_3);
                        gui.channel.resetChannel(CHANNEL_3);
                    }
                }
            }else{
                if(channel == CHANNEL_1 && !data.ready[FLAG_CHANNEL_1]){
                    data.error = INVALID_CHANNEL_1;
                }else if(channel == CHANNEL_2 && data.ready[FLAG_CHANNEL_1] && !data.ready[FLAG_CHANNEL_2] && gui.menu.ddActive[BUTTON_MODE] != MODE_SINGLE){
                    if(gui.menu.ddActive[BUTTON_MODE] == MODE_TWO) data.error = INVALID_CHANNEL_2_D;
                    else data.error = INVALID_CHANNEL_2_R;                
                }
            }
        }
    }    
}

void checkHypersFlags(Gui &gui, Data &data){
    // If prior is not ready, there is no reason to compute the hyper
    if(!data.ready[FLAG_PRIOR])
        return;

    for(int channel = 0; channel < NUMBER_CHANNELS; channel++){
        // If channel is not ready, there is no reason to compute the hyper
        if(!data.ready[FLAG_CHANNEL_1+channel]){
            data.ready[FLAG_HYPER_1+channel] = false;
            continue;
        }

        if(data.compute[FLAG_HYPER_1+channel]){
            data.hyper[channel] = Hyper(data.channelObj[channel]);
            gui.posteriors.numPost[channel] = data.hyper[channel].num_post;
            data.ready[FLAG_HYPER_1+channel] = true;
            data.compute[FLAG_HYPER_1+channel] = false;
            
            if(gui.channel.curChannel == channel)
                gui.updateHyperTBoxes(data.hyper[channel], channel, data.ready[FLAG_HYPER_1+channel]);
        }
    }
}
