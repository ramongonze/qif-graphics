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
#define RAYGUI_SUPPORT_ICONS
#include "../libs/raygui/src/raygui.h"
#include <iostream>
#include <string>
#include "gui/gui.h"
#include "data.h"
#include "chull.h"
#include "random-response.h"
#include "truncated-geometric.h"

typedef struct WebLoopVariables{
    Gui gui;
    Data data;
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
void initStyle();
void readFonts(Font* defaultFont, Font* defaultFontBig);
void updateStatusBar(int error, GuiVisualization &visualization);
void checkButtonsMouseCollision(Gui &gui);
void checkHelpMessagesActive(Gui &gui, Vector2 mousePosition);
void checkPriorFlags(Gui &gui, Data &data); // Check compute and ready prior flags
void checkChannelsFlags(Gui &gui, Data &data); // Check compute and ready channels flags
void checkHypersFlags(Gui &gui, Data &data); // Check compute and ready hypers flags

//----------------------------------------------------------------------------------
// Draw Functions Declaration
//----------------------------------------------------------------------------------
void drawGuiMenu(Gui &gui, Data &data, bool* closeWindow);
void drawGuiPrior(Gui &gui, Data &data);
void drawGuiChannel(Gui &gui, Data &data);
void drawGuiPosteriors(Gui &gui, Data &data);
void drawGuiVisualization(Gui &gui, Data &data);
void drawGettingStarted(Gui &gui);
void drawCirclePrior(Gui &gui, Data &data);
void drawCirclesInners(Gui &gui, Data &data, int channel);
void drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title, Color contentColor, Font font);
void drawContentDropBox(Gui &gui, Rectangle layoutTitle, Rectangle layoutContent, Color contentColor, Font font);
void drawGSContent(Gui &gui, Rectangle panel, int option, int imgPadding);
void drawHelpMessage(Gui &gui, Rectangle rec, char message[CHAR_BUFFER_SIZE]);
void drawTab(Gui &gui, int channel, bool active);        // If the tab is currently active

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
void buttonFile(Gui &gui, Data &data, bool* closeWindow);
void buttonMode(Gui &gui, Data &data, int* prevMode);
void buttonExamples(Gui &gui, Data &data);
void buttonGuide(Gui &gui);
void buttonRandomPrior(Gui &gui, Data &data);
void buttonsTabs(Gui &gui, int channel);
void buttonRandomChannel(Gui &gui, Data &data);
void buttonDraw(Gui &gui, Data &data);

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
    initStyle();

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
    buttonFile(*gui, *data, closeWindow);
    buttonMode(*gui, *data, mode); // It must be called after buttonFile function
    buttonExamples(*gui, *data);
    //----------------------------------------------------------------------------------

    data->error = NO_ERROR;

    // Prior
    //----------------------------------------------------------------------------------
    // Check if a TextBox is being pressed
    if(gui->checkPriorTextBoxPressed()){
        gui->drawing = false;
        data->fileSaved = false;
        data->compute[FLAG_PRIOR] = true;
        data->ready[FLAG_PRIOR] = false;
        updateStatusBar(NO_ERROR, gui->visualization);

        if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
            gui->moveAmongPriorTextBoxes();
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
        updateStatusBar(NO_ERROR, gui->visualization);
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
                sprintf(buffer, "%.6f", gui->visualization.SliderDeltaValue);            
                if(gui->visualization.SpinnerEpsilonValue != data->epsilon || strcmp(buffer, gui->channel.tBoxDeltaValue)){
                    if(gui->visualization.SpinnerEpsilonValue <= 0)
                        gui->visualization.SpinnerEpsilonValue = 1;

                    // Update epsilon and delta text boxes
                    sprintf(buffer, "%.6f", gui->visualization.SliderDeltaValue);
                    strcpy(gui->channel.tBoxDeltaValue, buffer);
                    sprintf(buffer, "%d", gui->visualization.SpinnerEpsilonValue);
                    strcpy(gui->channel.tBoxEpsilonValue, buffer);

                    gui->visualization.recomputeInners = true;
                    data->fileSaved = false;
                    data->resetAllExceptComputeChannel1();
                }
            }

            if(gui->checkEpsilonDeltaTextBoxPressed()){
                gui->drawing = false;
                data->fileSaved = false;
                data->resetAllExceptComputeChannel1();
                updateStatusBar(NO_ERROR, gui->visualization);
            }
        }else if(data->currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
            if(gui->drawing){
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", gui->visualization.SliderAlphaValue);
                if(gui->visualization.SliderAlphaValue != data->alpha){
                    // Update Alpha text box
                    sprintf(buffer, "%.6f", gui->visualization.SliderAlphaValue);
                    strcpy(gui->channel.tBoxAlphaValue, buffer);

                    gui->visualization.recomputeInners = true;
                    data->fileSaved = false;
                    data->resetAllExceptComputeChannel1();
                }
            }

            if(gui->checkAlphaTextBoxPressed()){
                gui->drawing = false;
                data->fileSaved = false;
                data->resetAllExceptComputeChannel1();
                updateStatusBar(NO_ERROR, gui->visualization);
            }
        }
    }else if(gui->checkChannelTextBoxPressed()){ // Check if a channel text box is being pressed
        gui->drawing = false;
        data->fileSaved = false;
        if(*mode == MODE_REF){
            data->resetAllExceptComputeChannel1();
        }else{
            data->compute[FLAG_CHANNEL_1+gui->channel.curChannel] = true;
            data->ready[FLAG_CHANNEL_1+gui->channel.curChannel] = false;
        }

        updateStatusBar(NO_ERROR, gui->visualization);

        if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
            gui->moveAmongChannelTextBoxes();
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
                gui->posteriors.numPosteriors[i] = data->hyper[i].num_post;
            data->animation = UPDATE_CIRCLES_BY_MOUSE;
            
            data->buildPriorCircle(gui->visualization.trianglePoints);
            gui->updateRectanglePriorCircleLabel(data->priorCircle);
            gui->updatePriorTextBoxes(data->priorObj);

            data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_1, *mode);
            gui->updateRectangleInnersCircleLabel(CHANNEL_1, data->innersCircles[CHANNEL_1]);
            if(*mode == MODE_TWO){
                data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_2, *mode);
                gui->updateRectangleInnersCircleLabel(CHANNEL_2, data->innersCircles[CHANNEL_2]);
            }else if(*mode == MODE_REF){
                data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_3, *mode);
                gui->updateRectangleInnersCircleLabel(CHANNEL_3, data->innersCircles[CHANNEL_3]);
            }
        }

        // User has changed epsilon or delta value
        if(gui->visualization.recomputeInners){
            data->animation = UPDATE_CIRCLES_BY_EPSILON_OR_DELTA;
            data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_1, *mode);
            gui->updateRectangleInnersCircleLabel(CHANNEL_1, data->innersCircles[CHANNEL_1]);
            gui->visualization.recomputeInners = false;
        }
        
        if(data->animationRunning){
            // I assume the prior circle was already built    
            data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_1, *mode);
            gui->updateRectangleInnersCircleLabel(CHANNEL_1, data->innersCircles[CHANNEL_1]);
            if(*mode == MODE_TWO){
                data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_2, *mode);
                gui->updateRectangleInnersCircleLabel(CHANNEL_2, data->innersCircles[CHANNEL_2]);
            }else if(*mode == MODE_REF){
                data->buildInnerCircles(gui->visualization.trianglePoints, CHANNEL_3, *mode);
                gui->updateRectangleInnersCircleLabel(CHANNEL_3, data->innersCircles[CHANNEL_3]);
            }
        }
    }
    //----------------------------------------------------------------------------------
    
    gui->updateHyperTextBoxes(data->hyper[gui->channel.curChannel], gui->channel.curChannel, data->ready[FLAG_HYPER_1+gui->channel.curChannel]);

    // Help messages
    //----------------------------------------------------------------------------------
    gui->checkMouseHover(mousePosition);
    //----------------------------------------------------------------------------------

    if(gui->drawing)
        updateStatusBar(NO_ERROR, gui->visualization);

    } // end of if(!gui->menu.windowGettingStartedActive)

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(BG_BASE_COLOR_DARK); 

        if(gui->menu.windowGSActive) GuiLock();
        drawGuiPrior(*gui, *data);
        drawGuiChannel(*gui, *data);
        drawGuiPosteriors(*gui, *data);
        drawGuiVisualization(*gui, *data);
        drawGuiMenu(*gui, *data, closeWindow);
        if(*mode != MODE_DP_SINGLE && *mode != MODE_DP_TWO && *mode != MODE_DP_POST_PROCESS)
            checkHelpMessagesActive(*gui, mousePosition);            
        if(gui->menu.windowGSActive) GuiUnlock();
        drawGettingStarted(*gui);

    EndDrawing();
    //-----------------------------------------------------------------------------------
}

void initStyle(){
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_LIGHT));
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 0);
    GuiSetStyle(TEXTBOX, TEXT_INNER_PADDING, -4);
    GuiSetStyle(TEXTBOX, BORDER_WIDTH, 0);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_LIGHT));
    GuiSetStyle(TEXTBOX, BORDER_WIDTH, 1);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(VALUEBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(VALUEBOX, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_PRESSED));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_PRESSED));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    GuiSetStyle(DROPDOWNBOX, TEXT_INNER_PADDING, 5);
    GuiSetStyle(LISTVIEW, BORDER_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(SCROLLBAR, BASE_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_LIGHT));
    GuiSetStyle(SCROLLBAR, BORDER_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(SCROLLBAR, BORDER_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(SPINNER, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_RIGHT);
}

void updateStatusBar(int error, GuiVisualization &visualization){
	switch(error){
		case INVALID_VALUE_PRIOR:
			strcpy(visualization.TextBoxStatusText, "Some character in prior distribution is not valid");
			break;
        case INVALID_VALUE_CHANNEL_1:
			strcpy(visualization.TextBoxStatusText, "Some character in channel C is not valid");
			break;
        case INVALID_VALUE_CHANNEL_2:
			strcpy(visualization.TextBoxStatusText, "Some character in channel R is not valid");
			break;
		case INVALID_PRIOR:
			strcpy(visualization.TextBoxStatusText, "The values in prior distribution do not make up a probability distribution");
			break;
		case INVALID_CHANNEL_1:
			strcpy(visualization.TextBoxStatusText, "Some row in channel C is not a probability distribution");
			break;
        case INVALID_CHANNEL_2_D:
			strcpy(visualization.TextBoxStatusText, "Some row in channel D is not a probability distribution");
            break;
        case INVALID_CHANNEL_2_R:
			strcpy(visualization.TextBoxStatusText, "Some row in channel R is not a probability distribution");
			break;
        case INVALID_EPSILON_OR_DELTA:
			strcpy(visualization.TextBoxStatusText, "Invalid epsilon or delta");
			break;
        case INVALID_ALPHA:
			strcpy(visualization.TextBoxStatusText, "Invalid alpha");
			break;
		case NO_ERROR:
			strcpy(visualization.TextBoxStatusText, "Status");
	}
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

void checkHelpMessagesActive(Gui &gui, Vector2 mousePosition){
    if(gui.helpMessagesActive[HELP_MSG_BUTTON_PRIOR])
        drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 220, 60}, gui.helpMessages[HELP_MSG_BUTTON_PRIOR]);

    if(gui.helpMessagesActive[HELP_MSG_BUTTON_CHANNEL])
        drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 300, 60}, gui.helpMessages[HELP_MSG_BUTTON_CHANNEL]);

    if(gui.helpMessagesActive[HELP_MSG_BUTTON_DRAW])
        drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 300, 90}, gui.helpMessages[HELP_MSG_BUTTON_DRAW]);

    initStyle();
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
                gui.updateChannelTextBoxes(data.channelObj[CHANNEL_1], CHANNEL_1);
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
                    gui.updateChannelTextBoxes(data.channelObj[CHANNEL_3], CHANNEL_3);
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
            gui.posteriors.numPosteriors[channel] = data.hyper[channel].num_post;
            data.ready[FLAG_HYPER_1+channel] = true;
            data.compute[FLAG_HYPER_1+channel] = false;
            
            if(gui.channel.curChannel == channel)
                gui.updateHyperTextBoxes(data.hyper[channel], channel, data.ready[FLAG_HYPER_1+channel]);
        }
    }
}

//------------------------------------------------------------------------------------
// Draw Functions Definitions (local)
//------------------------------------------------------------------------------------
void drawGuiMenu(Gui &gui, Data &data, bool* closeWindow){
    DrawRectangleRec(gui.menu.recMenu, MENU_BASE_COLOR_NORMAL);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

#if !defined(PLATFORM_WEB)
    // Button File
    if(!gui.menu.ddEdit[BUTTON_FILE]) gui.menu.ddActive[BUTTON_FILE] = BUTTON_FILE_OPTION_FILE;        // Reset selection
    if(GuiDropdownBox(gui.menu.recButtons[BUTTON_FILE], 120, gui.menu.buttonFileText, &(gui.menu.ddActive[BUTTON_FILE]), gui.menu.ddEdit[BUTTON_FILE])) gui.menu.ddEdit[BUTTON_FILE] = !gui.menu.ddEdit[BUTTON_FILE];
#endif

    // Button Mode
    gui.menu.updateButtonModeString(gui.menu.ddActive[BUTTON_MODE]);
    
    if(GuiDropdownBox(gui.menu.recButtons[BUTTON_MODE], 330, gui.menu.buttonModeText, &(gui.menu.ddActive[BUTTON_MODE]), gui.menu.ddEdit[BUTTON_MODE])) gui.menu.ddEdit[BUTTON_MODE] = !gui.menu.ddEdit[BUTTON_MODE];

    // Button Examples
    if(!gui.menu.ddEdit[BUTTON_EXAMPLES]) gui.menu.ddActive[BUTTON_EXAMPLES] = BUTTON_EXAMPLES_OPTION_EXAMPLES;        // Reset selection
    if(GuiDropdownBox(gui.menu.recButtons[BUTTON_EXAMPLES], 320, gui.menu.buttonExamplesText, &(gui.menu.ddActive[BUTTON_EXAMPLES]), gui.menu.ddEdit[BUTTON_EXAMPLES])) gui.menu.ddEdit[BUTTON_EXAMPLES] = !gui.menu.ddEdit[BUTTON_EXAMPLES];
    
    // Button Guide
    if(GuiButton(gui.menu.recButtons[BUTTON_GUIDE], gui.menu.buttonGuideText)) buttonGuide(gui);

    initStyle();
}

void drawGuiPrior(Gui &gui, Data &data){
    // if(gui.drawing && gui.menu.dropdownBoxActive[BUTTON_MODE] != MODE_SINGLE)
    if(gui.drawing)
        drawContentPanel(gui.prior.recTitle, gui.prior.recContent, gui.prior.panPriorTxt, PRIOR_COLOR_L1, gui.defaultFont);
    else
        drawContentPanel(gui.prior.recTitle, gui.prior.recContent, gui.prior.panPriorTxt, GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), gui.defaultFont);

    DrawRectangleRec(gui.prior.recPan, WHITE);
    DrawRectangleLinesEx(gui.prior.recPan, 1, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
    if(GuiButton(gui.prior.recButtonRand, gui.prior.buttonRandTxt)) buttonRandomPrior(gui, data); 
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

    for(int i = 0; i < NUMBER_SECRETS; i++){
        GuiLabel(gui.prior.recLabel[i], gui.prior.labelPriorTxt[i].c_str());
        if(GuiTextBox(gui.prior.recTBox[i], gui.prior.tBoxPriorTxt[i], CHAR_BUFFER_SIZE, gui.prior.tBoxPriorEdit[i])) gui.prior.tBoxPriorEdit[i] = !gui.prior.tBoxPriorEdit[i];
    }
}

void drawGuiChannel(Gui &gui, Data &data){
    int mode = gui.menu.ddActive[BUTTON_MODE];
    int curChannel = gui.channel.curChannel;
    Color contentColor = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));

    if(mode == MODE_SINGLE){
        strcpy(gui.channel.panChannelTxt, "Channel C");
        if(gui.drawing) contentColor = INNERS1_COLOR_D1;
        drawContentPanel(gui.channel.recTitle, gui.channel.recContent, gui.channel.panChannelTxt, contentColor, gui.defaultFont);
    }else if(mode == MODE_DP_SINGLE || mode == MODE_DP_POST_PROCESS){
        if(gui.drawing) contentColor = INNERS1_COLOR_D1;
        DrawRectangleRec(gui.channel.recContent, contentColor);
    }else{
        strcpy(gui.channel.panChannelTxt, "");
        if(mode == MODE_TWO){
            strcpy(gui.channel.labelChannelTabs[CHANNEL_2], "Ch D");
        }else if(mode == MODE_REF){
            strcpy(gui.channel.labelChannelTabs[CHANNEL_2], "Ch R");
        }
        
        if(gui.drawing){
            if(curChannel == CHANNEL_1) contentColor = INNERS1_COLOR_D1;
            else if(curChannel == CHANNEL_3 || (curChannel == CHANNEL_2 && mode == MODE_TWO)) contentColor = INNERS2_COLOR;
        }
        drawContentPanel(gui.channel.recTitle, gui.channel.recContent, gui.channel.panChannelTxt, contentColor, gui.defaultFont);

        for(int i = 0; i < NUMBER_CHANNELS - (mode == MODE_TWO ? 1 : 0); i++)
            drawTab(gui, i, curChannel == i);

        initStyle();
    }

    if(mode != MODE_DP_SINGLE && curChannel != CHANNEL_3){
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
        if(GuiButton(gui.channel.recButtonRand, gui.channel.buttonRandTxt)) buttonRandomChannel(gui, data); 
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    }

    if(mode != MODE_DP_SINGLE && curChannel != CHANNEL_3){
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));

        if(gui.drawing && (curChannel == CHANNEL_1 || (curChannel == CHANNEL_3) || (mode == MODE_TWO && curChannel == CHANNEL_2))){
            GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
            GuiSetStyle(VALUEBOX, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(VALUEBOX, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(VALUEBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        }

        if(GuiSpinner(gui.channel.recSpinner, gui.channel.labelOutTxt, &(gui.channel.spinChannelValue[curChannel]), 0, 50, gui.channel.spinChannelEdit)) gui.channel.spinChannelEdit = !gui.channel.spinChannelEdit;
        GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    }

    if(mode == MODE_DP_SINGLE){
        if(gui.drawing){
            GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        }
        
        if(data.currentDPMechanism == MECH_KRR){
            GuiLabel(gui.channel.recTBoxEpsilon, gui.channel.labelEpsilonTxt);
            GuiLabel(gui.channel.recTBoxDelta, gui.channel.labelDeltaTxt);        
            if(GuiTextBox(sum2Rec(gui.channel.recTBoxEpsilon, 80, 0, 0, 0), gui.channel.tBoxEpsilonValue, gui.defaultFont.baseSize, gui.channel.tBoxEpsilonEdit)) gui.channel.tBoxEpsilonEdit = !gui.channel.tBoxEpsilonEdit;
            if(GuiTextBox(sum2Rec(gui.channel.recTBoxDelta, 80, 0, 0, 0), gui.channel.tBoxDeltaValue, gui.defaultFont.baseSize, gui.channel.tBoxDeltaEdit)) gui.channel.tBoxDeltaEdit = !gui.channel.tBoxDeltaEdit;

        }else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
            GuiLabel(gui.channel.recTBoxAlpha, gui.channel.labelAlphaTxt);
            if(GuiTextBox(sum2Rec(gui.channel.recTBoxAlpha, 80, 0, 0, 0), gui.channel.tBoxAlphaValue, gui.defaultFont.baseSize, gui.channel.tBoxAlphaEdit)) gui.channel.tBoxAlphaEdit = !gui.channel.tBoxAlphaEdit;
        }

        if(gui.drawing){
            GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
            GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
            GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        }
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScroll = GuiScrollPanel(
        sum2Rec(gui.channel.recScrPan, 0, 0, -gui.channel.scrPanBOffset.x, -gui.channel.scrPanBOffset.y),
        (Rectangle){gui.channel.recScrPan.x, gui.channel.recScrPan.y, gui.channel.scrPanContent.x, gui.channel.scrPanContent.y},
        &(gui.channel.scrPanOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScroll.x, viewScroll.y, viewScroll.width, viewScroll.height);
        if(mode != MODE_DP_SINGLE)
            GuiLabel(sum2Rec(gui.channel.recLabelOut, gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelOutTxt);

        // Secrets
        for(int i = 0; i < gui.channel.numSecrets[curChannel]; i++){
            if(mode == MODE_SINGLE || mode == MODE_TWO || curChannel == CHANNEL_1 || curChannel == CHANNEL_3){
                GuiLabel(sum2Rec(gui.channel.recLabelX[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelXTxt[i].c_str());
            }else{
                GuiLabel(sum2Rec(gui.channel.recLabelX[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelYTxt[i].c_str());
            }

            for(int j = 0; j < gui.channel.numOutputs[curChannel]; j++){
                if(curChannel == CHANNEL_3 || mode == MODE_DP_SINGLE) GuiLock();
                if(GuiTextBox(sum2Rec(gui.channel.recTBoxChannel[i][j], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.tBoxChannelTxt[curChannel][i][j], CHAR_BUFFER_SIZE, gui.channel.tBoxChannelEdit[i][j])) gui.channel.tBoxChannelEdit[i][j] = !gui.channel.tBoxChannelEdit[i][j];
                if(curChannel == CHANNEL_3 || mode == MODE_DP_SINGLE) GuiUnlock();
            }
        }

        // Outputs labels
        if(curChannel == CHANNEL_1 || curChannel == CHANNEL_3){
            for(int i = 0; i < gui.channel.numOutputs[curChannel]; i++)
                GuiLabel(sum2Rec(gui.channel.recLabelY[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelYTxt[i].c_str());
        }else if(curChannel == CHANNEL_2 && mode == MODE_TWO){
            for(int i = 0; i < gui.channel.numOutputs[curChannel]; i++)
                GuiLabel(sum2Rec(gui.channel.recLabelY[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelYPTxt[i].c_str());
        }else if(curChannel == CHANNEL_2 && mode == MODE_REF){
            for(int i = 0; i < gui.channel.numOutputs[curChannel]; i++)
                GuiLabel(sum2Rec(gui.channel.recLabelY[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelYTxt[i].c_str());
        }
    EndScissorMode();

    // DP mechanisms
    if(mode == MODE_DP_SINGLE || mode == MODE_DP_POST_PROCESS){
        strcpy(gui.channel.panChannelTxt, "Channel C");
        if(gui.drawing) contentColor = INNERS1_COLOR_D1;
        drawContentDropBox(gui, gui.channel.recTitle, gui.channel.recContent, contentColor, gui.defaultFont);
    }
}

void drawGuiPosteriors(Gui &gui, Data &data){
    int mode = gui.menu.ddActive[BUTTON_MODE];
    int curChannel = gui.channel.curChannel;

    if(curChannel == CHANNEL_1){
        strcpy(gui.posteriors.GroupBoxPosteriorsText, "Hyper-distribution [\u03C0\u203AC]");
    }else if(curChannel == CHANNEL_2){
        if(mode == MODE_REF){
            strcpy(gui.posteriors.GroupBoxPosteriorsText, "");
        }else{
            strcpy(gui.posteriors.GroupBoxPosteriorsText, "Hyper-distribution [\u03C0\u203AD]");
        }
    }else if(curChannel == CHANNEL_3){
        strcpy(gui.posteriors.GroupBoxPosteriorsText, "Hyper-distribution [\u03C0\u203ACR]");
    }
    
    Color contentColor = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));
    if(gui.drawing){
        if(curChannel == CHANNEL_1) contentColor = INNERS1_COLOR_D1;
        else if(curChannel == CHANNEL_3 || (curChannel == CHANNEL_2 && mode == MODE_TWO)) contentColor = INNERS2_COLOR;
    }
    drawContentPanel(gui.posteriors.recTitle, gui.posteriors.recContent, gui.posteriors.GroupBoxPosteriorsText, contentColor, gui.defaultFont);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScrollPosteriors = GuiScrollPanel(
        sum2Rec(gui.posteriors.recScrollPanel, 0, 0, - gui.posteriors.ScrollPanelPosteriorsBoundsOffset.x, -gui.posteriors.ScrollPanelPosteriorsBoundsOffset.y),
        (Rectangle){gui.posteriors.recScrollPanel.x, gui.posteriors.recScrollPanel.y, gui.posteriors.ScrollPanelPosteriorsContent.x, gui.posteriors.ScrollPanelPosteriorsContent.y},
        &(gui.posteriors.ScrollPanelPosteriorsScrollOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScrollPosteriors.x, viewScrollPosteriors.y, viewScrollPosteriors.width, viewScrollPosteriors.height);
        if(mode != MODE_REF || curChannel != CHANNEL_2){
            GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
            GuiLabel(sum2Rec(gui.posteriors.recLabelOuter, gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, 0, 0), gui.posteriors.LabelOuterText);

            for(int i = 0; i < gui.posteriors.numPosteriors[curChannel]; i++){
                GuiLabel(sum2Rec(gui.posteriors.recLabelPosteriors[i], gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, 0, 0), gui.posteriors.LabelPosteriorsText[curChannel][i].c_str());
                GuiTextBox(sum2Rec(gui.posteriors.recTextBoxOuter[i], gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, 0, 0), gui.posteriors.TextBoxOuterText[i], CHAR_BUFFER_SIZE, gui.posteriors.TextBoxOuterEditMode[i]);
            }

            for(int i = 0; i < NUMBER_SECRETS; i++){
                GuiLabel(sum2Rec(gui.posteriors.recLabelX[i], gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, 0, 0), gui.posteriors.LabelPosteriorsXText[i].c_str());
                for(int j = 0; j < gui.posteriors.numPosteriors[curChannel]; j++){
                    GuiTextBox(sum2Rec(gui.posteriors.recTextBoxInners[i][j], gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, 0, 0), gui.posteriors.TextBoxInnersText[i][j], CHAR_BUFFER_SIZE, gui.posteriors.TextBoxInnersEditMode[i][j]);
                }
            }
        }
    EndScissorMode();
}

void drawGuiVisualization(Gui &gui, Data &data){
    drawContentPanel(gui.visualization.recTitle, gui.visualization.recContent, gui.visualization.GroupBoxVisualizationText, GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), gui.defaultFont);
    if(GuiButton(gui.visualization.recButtonDraw, gui.visualization.ButtonDrawText)) buttonDraw(gui, data);
    
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 4);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if(data.error != NO_ERROR) DrawRectangleRec(gui.visualization.recTextBoxStatus, WHITE);
    DrawRectangleRec(gui.visualization.recTextBoxStatus, WHITE);
    
    if(strcmp(gui.visualization.TextBoxStatusText, "Status")) GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(RED));
    GuiTextBox(gui.visualization.recTextBoxStatus, gui.visualization.TextBoxStatusText, CHAR_BUFFER_SIZE, gui.visualization.TextBoxStatusEditMode);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 0);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    
    GuiPanel(gui.visualization.recPanelVisualization);

    if(gui.drawing){
        // Checkboxes
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        gui.showLabels = GuiCheckBox(gui.visualization.recCheckboxShowLabels, gui.visualization.LabelCheckboxShowLabel, gui.showLabels);
        gui.showConvexHull = GuiCheckBox(gui.visualization.recCheckboxShowConvexHull, gui.visualization.LabelCheckboxShowConvexHull, gui.showConvexHull);

        int mode = gui.menu.ddActive[BUTTON_MODE];
        
        // Sliders
        if(mode == MODE_DP_SINGLE){
            if(data.currentDPMechanism == MECH_KRR){
                GuiLabel(sum2Rec(gui.visualization.recSpinnerEpsilon, -gui.visualization.recSpinnerEpsilon.width-5, 0, 0, 0), "Epsilon (ln):");
                
                GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
                GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
                GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
                GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
                GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
                if(GuiSpinner(gui.visualization.recSpinnerEpsilon, NULL, &(gui.visualization.SpinnerEpsilonValue), 1, 10000, gui.visualization.SpinnerEpsilonEditMode)) gui.visualization.SpinnerEpsilonEditMode = !gui.visualization.SpinnerEpsilonEditMode;
                GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
                GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
                GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
                GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
                GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
                GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
                GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
                GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

                GuiLabel(sum2Rec(gui.visualization.recSliderDelta, -gui.visualization.recSliderDelta.width+15, 0, 0, 0), "Delta:");
                GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_FOCUSED_TRANSP));
                gui.visualization.SliderDeltaValue = GuiSlider(gui.visualization.recSliderDelta, NULL, NULL, gui.visualization.SliderDeltaValue, 0, 1);
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", gui.visualization.SliderDeltaValue);
                GuiLabel(gui.visualization.recSliderDelta, buffer);
            }else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
                GuiLabel(sum2Rec(gui.visualization.recSliderAlpha, -gui.visualization.recSliderAlpha.width+15, 0, 0, 0), "Alpha:");
                GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_FOCUSED_TRANSP));
                gui.visualization.SliderAlphaValue = GuiSlider(gui.visualization.recSliderAlpha, NULL, NULL, gui.visualization.SliderAlphaValue, 0, 1);
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", gui.visualization.SliderAlphaValue);
                GuiLabel(gui.visualization.recSliderAlpha, buffer);
            }
        }

        // Triangle
        DrawTriangle(gui.visualization.trianglePoints[0], gui.visualization.trianglePoints[1], gui.visualization.trianglePoints[2], BG_BASE_COLOR_LIGHT2);
        DrawTriangleLines(gui.visualization.trianglePoints[0], gui.visualization.trianglePoints[1], gui.visualization.trianglePoints[2], BLACK);
        for(int i = 0; i < NUMBER_SECRETS; i++){
            DrawTextEx(gui.defaultFontBig, &(gui.visualization.LabelTriangleText[i][0]), (Vector2){gui.visualization.recLabelTriangle[i].x, gui.visualization.recLabelTriangle[i].y}, 32, 0, BLACK);
        }

        // Circles
        drawCirclePrior(gui, data);
        drawCirclesInners(gui, data, CHANNEL_1);

        if(mode == MODE_TWO)
            drawCirclesInners(gui, data, CHANNEL_2);
        else if(mode == MODE_REF)
            drawCirclesInners(gui, data, CHANNEL_3);
    }
}

void drawGettingStarted(Gui &gui){
    if(gui.menu.windowGSActive){
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(BG_BASE_COLOR_DARK));
        GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(BLACK));
        gui.menu.windowGSActive = !GuiWindowBox(gui.menu.recGS, "Getting started");
        initStyle();

        // List of options
        GuiSetStyle(LISTVIEW, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(LISTVIEW, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(LISTVIEW, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        gui.menu.gsMenuActive = GuiListView(gui.menu.recGSMenu, gui.menu.gsMenuOptions, &gui.menu.gsMenuScrollIndex, gui.menu.gsMenuActive);

        // Visualization panel
        DrawRectangleRec(gui.menu.recGSPanel, WHITE);
        if(gui.menu.gsMenuActive > -1){
            drawGSContent(gui, gui.menu.recGSPanel, gui.menu.gsMenuActive, gui.menu.imgPadding[gui.menu.gsMenuActive]);
        }
    }
}

void drawCirclePrior(Gui &gui, Data &data){
    DrawCircle(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR);
    DrawCircleLines(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR_LINES);
    if(gui.showLabels) DrawTextEx(gui.defaultFontBig, gui.visualization.LabelPriorCircleText, (Vector2) {gui.visualization.recLabelPriorCircle.x, gui.visualization.recLabelPriorCircle.y}, gui.defaultFontBig.baseSize, 1.0, BLACK);
}

void drawCirclesInners(Gui &gui, Data &data, int channel){
	for(int i = 0; i < data.hyper[channel].num_post; i++){
        Color colorFill, colorLines;
        if(channel == CHANNEL_1){
            colorFill = INNERS1_COLOR;
            colorLines = INNERS1_COLOR_LINES;
        }else{
            colorFill = INNERS2_COLOR;
            colorLines = INNERS2_COLOR_LINES;
        }

        DrawCircle(data.innersCircles[channel][i].center.x, data.innersCircles[channel][i].center.y, data.innersCircles[channel][i].radius, colorFill);
        DrawCircleLines(data.innersCircles[channel][i].center.x, data.innersCircles[channel][i].center.y, data.innersCircles[channel][i].radius, colorLines);
        
        // Decide to write label inside or outside the circle
        float threshold;
        if(channel == CHANNEL_1)
            threshold = 0.13f;
        else
            threshold = 0.20f;
        
        if(gui.showLabels){
            if(data.hyper[channel].outer.prob[i] < threshold)
                DrawTextEx(gui.defaultFontBig, &(gui.posteriors.LabelPosteriorsText[channel][i][0]), (Vector2) {gui.visualization.recLabelInnersCircles[channel][i].x-25, gui.visualization.recLabelInnersCircles[channel][i].y-25}, 26, 1.0, BLACK);
            else
                DrawTextEx(gui.defaultFontBig, &(gui.posteriors.LabelPosteriorsText[channel][i][0]), (Vector2) {gui.visualization.recLabelInnersCircles[channel][i].x-5, gui.visualization.recLabelInnersCircles[channel][i].y-5}, 26, 1.0, BLACK);
        }

        if(gui.showConvexHull){
            // Find convex hull using inners circles
            vector<pt> points = vector<pt>(gui.posteriors.numPosteriors[channel]);
            for(int i = 0; i < gui.posteriors.numPosteriors[channel]; i++){
                points[i].x = data.innersCircles[channel][i].center.x;
                points[i].y = data.innersCircles[channel][i].center.y;
            }

            convexHull(points);
            int n = (int) points.size();
            for(int i = 0; i < n-1; i++){
                if(channel == CHANNEL_1){
                    DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y, INNERS1_COLOR_LINES);
                    DrawTriangle(
                        (Vector2){(float)points[0].x, (float)points[0].y},
                        (Vector2){(float)points[i].x, (float)points[i].y},
                        (Vector2){(float)points[i+1].x, (float)points[i+1].y},
                        CH1_COLOR
                    );
                }else{
                    DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y, INNERS2_COLOR_LINES);
                    DrawTriangle(
                        (Vector2){(float)points[0].x, (float)points[0].y},
                        (Vector2){(float)points[i].x, (float)points[i].y},
                        (Vector2){(float)points[i+1].x, (float)points[i+1].y},
                        CH2_COLOR
                    );
                }

            }

            // Line from last point to the first
            if(channel == CHANNEL_1)
                DrawLine(points[n-1].x, points[n-1].y, points[0].x, points[0].y, INNERS1_COLOR_LINES);
            else
                DrawLine(points[n-1].x, points[n-1].y, points[0].x, points[0].y, INNERS2_COLOR_LINES);
        }
	}
}

void drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title, Color contentColor, Font font){
    DrawRectangleRec(layoutTitle, TITLES_BASE_COLOR);
    DrawRectangleRec(layoutContent, contentColor);
    DrawTextEx(font, title, (Vector2){layoutTitle.x + 10, layoutTitle.y}, font.baseSize, 1, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
}

void drawContentDropBox(Gui &gui, Rectangle layoutTitle, Rectangle layoutContent, Color contentColor, Font font){
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if(GuiDropdownBox(layoutTitle, layoutTitle.width, gui.channel.ddDPTxt, &(gui.channel.ddDPActive), gui.channel.ddDPEdit)) gui.channel.ddDPEdit = !gui.channel.ddDPEdit;
    gui.channel.updateDDDP();
    initStyle();
}

void drawGSContent(Gui &gui, Rectangle panel, int option, int imgPadding){
    /* option: GS_PRIOR, GS_CHANNEL, GS_HYPER, GS_VISUALIZATION, GS_REFINEMENT
       imgPadding: Space between description and image
    */

    // Description
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_DISABLED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_DISABLED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_INNER_PADDING, 0);
    
    gui.menu.scrPanContent.y = gui.menu.recGS.height + gui.menu.gsOptionYOffset[option];

    Rectangle viewScroll = GuiScrollPanel(
        sum2Rec(gui.menu.recScrPan, 0, 0, -gui.menu.scrPanBOffset.x, -gui.menu.scrPanBOffset.y),
        (Rectangle){gui.menu.recScrPan.x, gui.menu.recScrPan.y, gui.menu.scrPanContent.x, gui.menu.scrPanContent.y},
        &(gui.menu.scrPanOffset)
    );
    
    BeginScissorMode(viewScroll.x, viewScroll.y, viewScroll.width, viewScroll.height);
        GuiTextBoxMulti(
            (Rectangle){panel.x+10+gui.menu.scrPanOffset.x, panel.y+10+gui.menu.scrPanOffset.y, panel.width-30, (float)gui.menu.gsContentHeight[option]},
            gui.menu.gsDescriptionTexts[option],
            gui.defaultFont.baseSize,
            false
        );
        
        // Image
        if(strcmp(gui.menu.imagesSrc[option], "")){
            DrawTextureEx(gui.menu.gsImages[option], (Vector2){panel.x+10+gui.menu.scrPanOffset.x, panel.y+imgPadding+gui.menu.scrPanOffset.y}, 0.0f, 0.43f, WHITE);
        }
    EndScissorMode();


    initStyle();
}

void drawHelpMessage(Gui &gui, Rectangle rec, char message[CHAR_BUFFER_SIZE]){
    GuiSetStyle(TEXTBOX, BORDER_COLOR_DISABLED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_DISABLED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_INNER_PADDING, 5);
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(BG_BASE_COLOR_LIGHT));
    int oldState = GuiGetState();
    GuiSetState(GUI_STATE_DISABLED);
    GuiTextBoxMulti(rec, message, gui.defaultFont.baseSize, false);
    GuiSetState(oldState);
}

void drawTab(Gui &gui, int channel, bool active){
    int oldState = GuiGetState();
    int mode = gui.menu.ddActive[BUTTON_MODE]; // Mode can be only MODE_TWO and MODE_REF

    if(gui.drawing){
        // Text color
        if(channel == CHANNEL_1 || channel == CHANNEL_3 || (channel == CHANNEL_2 && mode == MODE_TWO)){
            GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        }else if(channel == CHANNEL_2 && mode == MODE_REF){
            GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
            GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
            GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        }

        // Button color
        if(channel == CHANNEL_1){
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(INNERS1_COLOR_L1));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(INNERS1_COLOR_D2));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(INNERS1_COLOR_L1));
        }else if(channel == CHANNEL_3){
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(INNERS2_COLOR_L2));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(INNERS2_COLOR_D1));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(INNERS2_COLOR_L2));
        }

        if(channel == CHANNEL_2 && mode == MODE_TWO){
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(INNERS2_COLOR_L2));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(INNERS2_COLOR_D1));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(INNERS2_COLOR_L2));
        }else if(channel == CHANNEL_2 && mode == MODE_REF){
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_LIGHT));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(BG_BASE_COLOR_DARK));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_LIGHT));
        }
    }else{
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_LIGHT));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
    }
    
    if(active) GuiSetState(GUI_STATE_PRESSED);

    if(GuiButton(gui.channel.recTabs[channel], gui.channel.labelChannelTabs[channel])) buttonsTabs(gui, channel);
    GuiSetState(oldState);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_PRESSED));
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
void buttonFile(Gui &gui, Data &data, bool* closeWindow){
#if !defined(PLATFORM_WEB)
    int option = gui.menu.ddActive[BUTTON_FILE];

    if(option == BUTTON_FILE_OPTION_OPEN){
        gui.drawing = false;
        int retRead = gui.menu.readQIFFile(gui.prior.tBoxPriorTxt, gui.channel.tBoxChannelTxt, gui.channel.numSecrets, gui.channel.numOutputs);

        if(retRead == INVALID_QIF_FILE){
            // Open a dialog error
            system("zenity --error --no-wrap --text=\"Invalid QIF graphics file\"");
        }else{
            // Update channels spinners
            gui.channel.spinChannelValue[CHANNEL_1] = gui.channel.numOutputs[CHANNEL_1];
            gui.channel.spinChannelValue[CHANNEL_2] = gui.channel.numOutputs[CHANNEL_2];
            
            // Reset prior
            data.ready[FLAG_PRIOR] = false;
            data.compute[FLAG_PRIOR] = true;

            // Reset channels
            data.resetAllExceptComputeChannel1();

            for(int channel = 0; channel < NUMBER_CHANNELS; channel++)
                gui.posteriors.resetPosterior(FLAG_CHANNEL_1+channel);

            // Update current mode
            gui.channel.checkModeAndSizes(retRead);
            gui.menu.ddActive[BUTTON_MODE] = retRead;

            if(retRead == MODE_TWO) data.compute[FLAG_CHANNEL_2] = true;

            updateStatusBar(NO_ERROR, gui.visualization);
        }
    }else if(option == BUTTON_FILE_OPTION_SAVE){
        gui.menu.saveQIFFile(
            gui.prior.tBoxPriorTxt,
            gui.channel.tBoxChannelTxt,
            gui.channel.numSecrets,
            gui.channel.numOutputs,
            gui.menu.ddActive[BUTTON_MODE],
            strcmp(gui.menu.fileName, "\0") == 0 ? true : false
        );
        if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;   
    }else if(option == BUTTON_FILE_OPTION_SAVEAS){
        gui.menu.saveQIFFile(
            gui.prior.tBoxPriorTxt,
            gui.channel.tBoxChannelTxt,
            gui.channel.numSecrets,
            gui.channel.numOutputs,
            gui.menu.ddActive[BUTTON_MODE],
            true
        );
        if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;
    }else if(option == BUTTON_FILE_OPTION_EXIT){
        if(data.fileSaved){
            *closeWindow = true;
        }else{
            int ret;
            FILE *file = popen("zenity --no-wrap --title=\"QIF Graphics\" --question --text=\"Do you want to save changes you made?\"", "r");
            ret = WEXITSTATUS(pclose(file));        // Get the user input Yes=0 or No=1

            if(ret == 0){
                // Yes
                gui.menu.saveQIFFile(
                    gui.prior.tBoxPriorTxt,
                    gui.channel.tBoxChannelTxt,
                    gui.channel.numSecrets,
                    gui.channel.numOutputs,
                    gui.menu.ddActive[BUTTON_MODE],
                    strcmp(gui.menu.fileName, "\0") == 0 ? true : false
                );
                if(strcmp(gui.menu.fileName, "\0")){
                    data.fileSaved = true;
                    *closeWindow = true;  
                }
            }else{
                // No
                *closeWindow = true;
            }
        }
    }
#endif
    gui.menu.ddActive[BUTTON_FILE] = BUTTON_FILE_OPTION_FILE;
}

void buttonMode(Gui &gui, Data &data, int* prevMode){
    int curMode = gui.menu.ddActive[BUTTON_MODE];

    if(*prevMode != curMode){
        gui.channel.checkModeAndSizes(curMode);

        // Note: Channel 1 doesn't need to be changed in any mode
        gui.drawing = false;
        updateStatusBar(NO_ERROR, gui.visualization);

        // Set prior to be computed
        data.ready[FLAG_PRIOR] = false;
        data.compute[FLAG_PRIOR] = true;

        data.resetAllExceptComputeChannel1();

        if(curMode == MODE_SINGLE || curMode == MODE_DP_SINGLE){
            gui.channel.recTitle = (Rectangle){gui.channel.anchorChannel.x, gui.channel.anchorChannel.y, 350, 20};
        }else if(curMode == MODE_TWO){
            gui.channel.recTitle = (Rectangle){gui.channel.anchorChannel.x+112, gui.channel.anchorChannel.y, 350-112, 20};
            data.compute[FLAG_CHANNEL_2] = true;
        }else if(curMode == MODE_REF){
            gui.channel.recTitle = (Rectangle){gui.channel.anchorChannel.x+168, gui.channel.anchorChannel.y, 350-168, 20};
        }
    }

    *prevMode = curMode;
}

void buttonExamples(Gui &gui, Data &data){
    int example = gui.menu.ddActive[BUTTON_EXAMPLES];
    
    // If none of the options were seleted, skip.
    if(example != BUTTON_EXAMPLES_OPTION_CH_0 && example != BUTTON_EXAMPLES_OPTION_CH_1)
        return;

    int curChannel = gui.channel.curChannel;

    // If some option was selected when the current channel is CR, skip.
    if(gui.menu.ddActive[BUTTON_MODE] == MODE_REF && curChannel == CHANNEL_3)
        return;

    char newChannel[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

    gui.channel.spinChannelValue[curChannel] = (example == BUTTON_EXAMPLES_OPTION_CH_0) ? gui.channel.numSecrets[curChannel] : 1;    
    gui.channel.updateChannelBySpinner(gui.channel.curChannel, gui.menu.ddActive[BUTTON_MODE]);

    if(example == BUTTON_EXAMPLES_OPTION_CH_0){
        // Set identity matrix
        for(int i = 0; i < gui.channel.numSecrets[curChannel]; i++){
            for(int j = 0; j < gui.channel.numOutputs[curChannel]; j++){
                if(i == j) strcpy(newChannel[i][j], "1");
                else strcpy(newChannel[i][j], "0");
            }
        }
    }

    if(example == BUTTON_EXAMPLES_OPTION_CH_1){
        // Set channel that leaks nothing
        for(int i = 0; i < gui.channel.numSecrets[curChannel]; i++){
            strcpy(newChannel[i][0], "1");
        }
    }

    GuiChannel::copyChannelText(newChannel, gui.channel.tBoxChannelTxt[curChannel], gui.channel.numSecrets[curChannel], gui.channel.numOutputs[curChannel]);
    if(gui.menu.ddActive[BUTTON_MODE] == MODE_REF){
        data.resetAllExceptComputeChannel1();
    }else{
        data.compute[FLAG_CHANNEL_1+curChannel] = true;
        data.ready[FLAG_CHANNEL_1+curChannel] = false;
    }
    gui.drawing = false;
}

void buttonGuide(Gui &gui){
    gui.menu.windowGSActive = true;
}

void buttonRandomPrior(Gui &gui, Data &data){
    data.newRandomPrior();
    gui.drawing = false;
    data.fileSaved = false;
    data.compute[FLAG_PRIOR] = false;
    data.ready[FLAG_PRIOR] = true;
    gui.updatePriorTextBoxes(data.priorObj);
    
    int mode = gui.menu.ddActive[BUTTON_MODE];
    
    // Reset channel 1 and set it to be computed
    data.ready[FLAG_CHANNEL_1] = false;
    data.compute[FLAG_CHANNEL_1] = true;
    data.ready[FLAG_HYPER_1] = false;
    data.compute[FLAG_HYPER_1] = false;

    if(mode == MODE_TWO || mode == MODE_REF){
        // Reset channel 2 and set it to be computed
        data.ready[FLAG_CHANNEL_2] = false;
        data.compute[FLAG_CHANNEL_2] = true;
        data.ready[FLAG_HYPER_2] = false;
        data.compute[FLAG_HYPER_2] = false;
    }

    if(mode == MODE_REF){
        // Reset channel 3
        data.ready[FLAG_CHANNEL_3] = false;
        data.compute[FLAG_CHANNEL_3] = false;
        data.ready[FLAG_HYPER_3] = false;
        data.compute[FLAG_HYPER_3] = false;
    }
}

void buttonsTabs(Gui &gui, int channel){
    gui.channel.curChannel = channel;
    updateStatusBar(NO_ERROR, gui.visualization);
}

void buttonRandomChannel(Gui &gui, Data &data){
    if(data.error == INVALID_CHANNEL_1 || data.error == INVALID_CHANNEL_2_D || data.error == INVALID_CHANNEL_2_R)
        updateStatusBar(NO_ERROR, gui.visualization);
    
    data.newRandomChannel(gui.channel.curChannel, gui.channel.numSecrets[gui.channel.curChannel], gui.channel.numOutputs[gui.channel.curChannel]);
    gui.drawing = false;
    data.fileSaved = false;
    gui.channel.updateChannelTextBoxes(data.channel[gui.channel.curChannel]);

    if(gui.menu.ddActive[BUTTON_MODE] == MODE_REF){
        data.resetAllExceptComputeChannel1();
    }else{
        data.compute[FLAG_CHANNEL_1+gui.channel.curChannel] = true;
        data.ready[FLAG_CHANNEL_1+gui.channel.curChannel] = false;
    }
}

void buttonDraw(Gui &gui, Data &data){
    int mode = gui.menu.ddActive[BUTTON_MODE];
    if(mode == MODE_SINGLE || mode == MODE_DP_SINGLE){
        if(!data.ready[FLAG_HYPER_1]){
            updateStatusBar(data.error, gui.visualization);
            return;
        }

        gui.drawing = true;
        data.animationRunning = true;
        data.animation = STEPS;

        data.buildPriorCircle(gui.visualization.trianglePoints);
        gui.updateRectanglePriorCircleLabel(data.priorCircle);
        gui.updatePriorTextBoxes(data.priorObj);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_1, mode);
        gui.updateRectangleInnersCircleLabel(CHANNEL_1, data.innersCircles[CHANNEL_1]);

        if(data.currentDPMechanism == MECH_KRR){
            gui.visualization.SpinnerEpsilonValue = stoi(gui.channel.tBoxEpsilonValue);
            gui.visualization.SliderDeltaValue = stof(gui.channel.tBoxDeltaValue);
        }else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
            gui.visualization.SliderAlphaValue = stof(gui.channel.tBoxAlphaValue);
        }
    }else if(mode == MODE_TWO){
        if(!data.ready[FLAG_HYPER_1] || !data.ready[FLAG_HYPER_2]){
            updateStatusBar(data.error, gui.visualization);
            return;
        }

        gui.drawing = true;
        data.animationRunning = true;
        data.animation = 2*STEPS+1;

        data.buildPriorCircle(gui.visualization.trianglePoints);
        gui.updateRectanglePriorCircleLabel(data.priorCircle);
        gui.updatePriorTextBoxes(data.priorObj);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_1, mode);
        gui.updateRectangleInnersCircleLabel(CHANNEL_1, data.innersCircles[CHANNEL_1]);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_2, mode);
        gui.updateRectangleInnersCircleLabel(CHANNEL_2, data.innersCircles[CHANNEL_2]);
    }else if(mode == MODE_REF){
        if(!data.ready[FLAG_HYPER_1] || !data.ready[FLAG_HYPER_3]){
            updateStatusBar(data.error, gui.visualization);
            return;
        }

        gui.drawing = true;
        data.animationRunning = true;
        data.animation = 2*STEPS+1;

        data.buildPriorCircle(gui.visualization.trianglePoints);
        gui.updateRectanglePriorCircleLabel(data.priorCircle);
        gui.updatePriorTextBoxes(data.priorObj);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_1, mode);
        gui.updateRectangleInnersCircleLabel(CHANNEL_1, data.innersCircles[CHANNEL_1]);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_3, mode);
        gui.updateRectangleInnersCircleLabel(CHANNEL_3, data.innersCircles[CHANNEL_3]);
    }
}
