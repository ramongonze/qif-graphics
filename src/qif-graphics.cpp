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

typedef struct WebLoopVariables{
    Gui gui;
    Data data;
    bool closeWindow;
    int mode;
} WebLoopVariables;

#if defined(PLATFORM_WEB)
    #include "/home/ramon/lib/emsdk/upstream/emscripten/system/include/emscripten.h"
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
void drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title, Font font);
void drawGSContent(Gui &gui, Rectangle panel, int option, int imgPadding);
void drawHelpMessage(Gui &gui, Rectangle rec, char message[CHAR_BUFFER_SIZE]);
void drawTab(Gui &gui, int channel, bool active);        // If the tab is currently active

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
void buttonFile(Gui &gui, Data &data, bool* closeWindow);
void buttonMode(Gui &gui, Data &data, int* prevMode);
void buttonExamples(Gui &gui, Data &data);
void buttonHelp(Gui &gui);
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

    // Update
    //----------------------------------------------------------------------------------
    *closeWindow = WindowShouldClose();
    Vector2 mousePosition = GetMousePosition();
    
    // Menu
    //----------------------------------------------------------------------------------
    checkButtonsMouseCollision(*gui);
    buttonFile(*gui, *data, closeWindow);
    buttonMode(*gui, *data, mode);
    buttonExamples(*gui, *data);
    buttonHelp(*gui);
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
    
    // Check if a TextBox is being pressed
    if(gui->checkChannelTextBoxPressed()){
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

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(BG_BASE_COLOR_DARK); 
        
        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        // Draw controls
        if(gui->menu.windowGettingStartedActive) GuiLock();
        drawGuiPrior(*gui, *data);
        drawGuiChannel(*gui, *data);
        drawGuiPosteriors(*gui, *data);
        drawGuiVisualization(*gui, *data);
        drawGuiMenu(*gui, *data, closeWindow);
        checkHelpMessagesActive(*gui, mousePosition);
        if(gui->menu.windowGettingStartedActive) GuiUnlock();
        drawGettingStarted(*gui);
        //----------------------------------------------------------------------------------

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
        case INVALID_CHANNEL_2:
			strcpy(visualization.TextBoxStatusText, "Some row in channel R is not a probability distribution");
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
    if((gui.menu.dropdownEditMode[BUTTON_FILE] || gui.menu.dropdownEditMode[BUTTON_MODE] || gui.menu.dropdownEditMode[BUTTON_EXAMPLES] || gui.menu.dropdownEditMode[BUTTON_HELP])
        &&
        (CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[BUTTON_FILE]) || CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[BUTTON_MODE]) ||
         CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[BUTTON_EXAMPLES]) || CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[BUTTON_HELP]))){
        i = 0; // File button is the first one
    }
#else
    if((gui.menu.dropdownEditMode[BUTTON_MODE] || gui.menu.dropdownEditMode[BUTTON_EXAMPLES] || gui.menu.dropdownEditMode[BUTTON_HELP])
        &&
        (CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[BUTTON_MODE]) || CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[BUTTON_EXAMPLES]) ||
         CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[BUTTON_HELP]))){
        i = 1; // Mode button is the first one
    }
#endif
    if(i >= 0){
        for(; i < 4; i++){
            if(i != BUTTON_MODE)
                gui.menu.dropdownBoxActive[i] = 0;
                
            if(CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[i]))
                gui.menu.dropdownEditMode[i] = true;
            else
                gui.menu.dropdownEditMode[i] = false;
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
        if(data.checkPriorText(gui.prior.TextBoxPriorText) == NO_ERROR){
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
    
    for(int channel = 0; channel < NUMBER_CHANNELS; channel++){
        if(data.compute[FLAG_CHANNEL_1+channel]){
            if(channel == CHANNEL_3){
                // Multiply channels C and R
                data.channelObj[CHANNEL_3] = composeChannels(data.channelObj[CHANNEL_1], data.channelObj[CHANNEL_2]);
                data.channel[CHANNEL_3] = data.channelObj[CHANNEL_3].matrix;
                gui.updateChannelTextBoxes(data.channelObj[CHANNEL_3], CHANNEL_3);
                data.ready[FLAG_CHANNEL_3] = true;
                data.compute[FLAG_HYPER_3] = true; // Set hyper to be computed
            }else if(data.checkChannelText(gui.channel.TextBoxChannelText[channel], channel, gui.channel.numSecrets[channel], gui.channel.numOutputs[channel]) == NO_ERROR){    
                if(Channel::isChannel(data.channel[channel])){
                    if(channel == CHANNEL_2 && gui.menu.dropdownBoxActive[BUTTON_MODE] == MODE_REF){
                        data.fakePrior = Distribution(gui.channel.numSecrets[CHANNEL_2], "uniform");
                        data.channelObj[CHANNEL_2] = Channel(data.fakePrior, data.channel[CHANNEL_2]);
                        if(data.ready[FLAG_CHANNEL_1])
                            data.compute[FLAG_CHANNEL_3] = true;
                    }else{
                        data.channelObj[channel] = Channel(data.priorObj, data.channel[channel]);
                        data.compute[FLAG_HYPER_1+channel] = true; // Set hyper to be computed
                        if(channel == CHANNEL_1 && gui.menu.dropdownBoxActive[BUTTON_MODE] == MODE_REF)
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
            }
        }else{
            if(channel == CHANNEL_1 && !data.ready[FLAG_CHANNEL_1]){
                data.error = INVALID_CHANNEL_1;
            }else if(channel == CHANNEL_2 && data.ready[FLAG_CHANNEL_1] && !data.ready[FLAG_CHANNEL_2] && gui.menu.dropdownBoxActive[BUTTON_MODE] != MODE_SINGLE){
                data.error = INVALID_CHANNEL_2;
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
    DrawRectangleRec(gui.menu.layoutRecsMenu, MENU_BASE_COLOR_NORMAL);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

#if !defined(PLATFORM_WEB)
    // Button File
    if(!gui.menu.dropdownEditMode[BUTTON_FILE]) gui.menu.dropdownBoxActive[BUTTON_FILE] = BUTTON_FILE_OPTION_FILE;        // Reset selection
    if(GuiDropdownBox(gui.menu.layoutRecsButtons[BUTTON_FILE], 120, gui.menu.buttonFileText, &(gui.menu.dropdownBoxActive[BUTTON_FILE]), gui.menu.dropdownEditMode[BUTTON_FILE])) gui.menu.dropdownEditMode[BUTTON_FILE] = !gui.menu.dropdownEditMode[BUTTON_FILE];
#endif

    // Button Mode
    if(gui.menu.dropdownBoxActive[BUTTON_MODE] == BUTTON_MODE_OPTION_SINGLE){
        gui.channel.curChannel = CHANNEL_1;
        strcpy(gui.menu.buttonModeText, "Mode;#112#Single channel;#000#Two channels;#000#Refinement");
    }else if(gui.menu.dropdownBoxActive[BUTTON_MODE] == BUTTON_MODE_OPTION_TWO){
        if(gui.channel.curChannel == CHANNEL_3) gui.channel.curChannel = CHANNEL_2;
        strcpy(gui.menu.buttonModeText, "Mode;#000#Single channel;#112#Two channels;#000#Refinement");
    }else if(gui.menu.dropdownBoxActive[BUTTON_MODE] == BUTTON_MODE_OPTION_REF){
        strcpy(gui.menu.buttonModeText, "Mode;#000#Single channel;#000#Two channels;#112#Refinement");
    }
    if(GuiDropdownBox(gui.menu.layoutRecsButtons[BUTTON_MODE], 160, gui.menu.buttonModeText, &(gui.menu.dropdownBoxActive[BUTTON_MODE]), gui.menu.dropdownEditMode[BUTTON_MODE])) gui.menu.dropdownEditMode[BUTTON_MODE] = !gui.menu.dropdownEditMode[BUTTON_MODE];

    // Button Examples
    if(!gui.menu.dropdownEditMode[BUTTON_EXAMPLES]) gui.menu.dropdownBoxActive[BUTTON_EXAMPLES] = BUTTON_EXAMPLES_OPTION_EXAMPLES;        // Reset selection
    if(GuiDropdownBox(gui.menu.layoutRecsButtons[BUTTON_EXAMPLES], 320, gui.menu.buttonExamplesText, &(gui.menu.dropdownBoxActive[BUTTON_EXAMPLES]), gui.menu.dropdownEditMode[BUTTON_EXAMPLES])) gui.menu.dropdownEditMode[BUTTON_EXAMPLES] = !gui.menu.dropdownEditMode[BUTTON_EXAMPLES];
    
    // Button Help
    if(!gui.menu.dropdownEditMode[BUTTON_HELP]) gui.menu.dropdownBoxActive[BUTTON_HELP] = BUTTON_HELP_OPTION_HELP;        // Reset selection
    if(GuiDropdownBox(gui.menu.layoutRecsButtons[BUTTON_HELP], 170, gui.menu.buttonHelpText, &(gui.menu.dropdownBoxActive[BUTTON_HELP]), gui.menu.dropdownEditMode[BUTTON_HELP])) gui.menu.dropdownEditMode[BUTTON_HELP] = !gui.menu.dropdownEditMode[BUTTON_HELP];

    initStyle();
}

void drawGuiPrior(Gui &gui, Data &data){
    drawContentPanel(gui.prior.layoutRecsTitle, gui.prior.layoutRecsContent, gui.prior.panelPriorText, gui.defaultFont);
    DrawRectangleRec(gui.prior.layoutRecsPanel, WHITE);
    DrawRectangleLinesEx(gui.prior.layoutRecsPanel, 1, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
    if(GuiButton(gui.prior.layoutRecsButtonRandom, gui.prior.buttonRandomText)) buttonRandomPrior(gui, data); 
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

    for(int i = 0; i < NUMBER_SECRETS; i++){
        GuiLabel(gui.prior.layoutRecsLabel[i], gui.prior.LabelPriorText[i].c_str());
        if(GuiTextBox(gui.prior.layoutRecsTextBox[i], gui.prior.TextBoxPriorText[i], CHAR_BUFFER_SIZE, gui.prior.TextBoxPriorEditMode[i])) gui.prior.TextBoxPriorEditMode[i] = !gui.prior.TextBoxPriorEditMode[i];
    }
}

void drawGuiChannel(Gui &gui, Data &data){
    int mode = gui.menu.dropdownBoxActive[BUTTON_MODE];
    if(mode == MODE_SINGLE){
        strcpy(gui.channel.panelChannelText, "Channel C");
        drawContentPanel(gui.channel.layoutRecsTitle, gui.channel.layoutRecsContent, gui.channel.panelChannelText, gui.defaultFont);
    }else{
        strcpy(gui.channel.panelChannelText, "");
        drawContentPanel(gui.channel.layoutRecsTitle, gui.channel.layoutRecsContent, gui.channel.panelChannelText, gui.defaultFont);
        
        for(int i = 0; i < NUMBER_CHANNELS - (mode == MODE_TWO ? 1 : 0); i++)
            drawTab(gui, i, gui.channel.curChannel == i);
    }

    if(gui.channel.curChannel != CHANNEL_3){
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
        if(GuiButton(gui.channel.layoutRecsButtonRandom, gui.channel.buttonRandomText)) buttonRandomChannel(gui, data); 
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    }

    if(gui.channel.curChannel != CHANNEL_3){
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
        if(GuiSpinner(gui.channel.layoutRecsSpinner, gui.channel.LabelOutputsText, &(gui.channel.SpinnerChannelValue[gui.channel.curChannel]), 0, 50, gui.channel.SpinnerChannelEditMode)) gui.channel.SpinnerChannelEditMode = !gui.channel.SpinnerChannelEditMode;
        GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScrollChannel = GuiScrollPanel(
        (Rectangle){gui.channel.layoutRecsScrollPanel.x, gui.channel.layoutRecsScrollPanel.y, gui.channel.layoutRecsScrollPanel.width - gui.channel.ScrollPanelChannelBoundsOffset.x, gui.channel.layoutRecsScrollPanel.height - gui.channel.ScrollPanelChannelBoundsOffset.y },
        (Rectangle){gui.channel.layoutRecsScrollPanel.x, gui.channel.layoutRecsScrollPanel.y, gui.channel.ScrollPanelChannelContent.x, gui.channel.ScrollPanelChannelContent.y},
        &(gui.channel.ScrollPanelChannelScrollOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScrollChannel.x, viewScrollChannel.y, viewScrollChannel.width, viewScrollChannel.height);
        GuiLabel((Rectangle){gui.channel.layoutRecsLabelOutputs.x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelOutputs.y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelOutputs.width, gui.channel.layoutRecsLabelOutputs.height}, gui.channel.LabelOutputsText);
        for(int i = 0; i < gui.channel.numSecrets[gui.channel.curChannel]; i++){
            if(mode == MODE_SINGLE || mode == MODE_TWO || gui.channel.curChannel == CHANNEL_1 || gui.channel.curChannel == CHANNEL_3){
                GuiLabel((Rectangle){gui.channel.layoutRecsLabelX[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelX[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelX[i].width, gui.channel.layoutRecsLabelX[i].height}, gui.channel.LabelChannelXText[i].c_str());
            }else{
                GuiLabel((Rectangle){gui.channel.layoutRecsLabelX[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelX[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelX[i].width, gui.channel.layoutRecsLabelX[i].height}, gui.channel.LabelChannelYText[i].c_str());
            }

            for(int j = 0; j < gui.channel.numOutputs[gui.channel.curChannel]; j++){
                if(gui.channel.curChannel == CHANNEL_3) GuiLock();
                if(GuiTextBox((Rectangle){gui.channel.layoutRecsTextBoxChannel[i][j].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsTextBoxChannel[i][j].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsTextBoxChannel[i][j].width, gui.channel.layoutRecsTextBoxChannel[i][j].height}, gui.channel.TextBoxChannelText[gui.channel.curChannel][i][j], CHAR_BUFFER_SIZE, gui.channel.TextBoxChannelEditMode[i][j])) gui.channel.TextBoxChannelEditMode[i][j] = !gui.channel.TextBoxChannelEditMode[i][j];
                if(gui.channel.curChannel == CHANNEL_3) GuiUnlock();
            }
        }

        if(mode == MODE_SINGLE || mode == MODE_TWO || gui.channel.curChannel == CHANNEL_1){
            for(int i = 0; i < gui.channel.numOutputs[gui.channel.curChannel]; i++)
                GuiLabel((Rectangle){gui.channel.layoutRecsLabelY[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelY[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelY[i].width, gui.channel.layoutRecsLabelY[i].height}, gui.channel.LabelChannelYText[i].c_str());
        }else{
            for(int i = 0; i < gui.channel.numOutputs[gui.channel.curChannel]; i++)
                GuiLabel((Rectangle){gui.channel.layoutRecsLabelY[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelY[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelY[i].width, gui.channel.layoutRecsLabelY[i].height}, gui.channel.LabelChannelZText[i].c_str());
        }
    EndScissorMode();
}

void drawGuiPosteriors(Gui &gui, Data &data){
    int mode = gui.menu.dropdownBoxActive[BUTTON_MODE];
    if(gui.channel.curChannel == CHANNEL_1){
        strcpy(gui.posteriors.GroupBoxPosteriorsText, "Hyper-distribution [\u03C0\u203AC]");
    }else if(gui.channel.curChannel == CHANNEL_2){
        if(mode == MODE_REF){
            strcpy(gui.posteriors.GroupBoxPosteriorsText, "");
        }else{
            strcpy(gui.posteriors.GroupBoxPosteriorsText, "Hyper-distribution [\u03C0\u203AR]");
        }
    }else if(gui.channel.curChannel == CHANNEL_3){
        strcpy(gui.posteriors.GroupBoxPosteriorsText, "Hyper-distribution [\u03C0\u203ACR]");
    }
    
    drawContentPanel(gui.posteriors.layoutRecsTitle, gui.posteriors.layoutRecsContent, gui.posteriors.GroupBoxPosteriorsText, gui.defaultFont);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScrollPosteriors = GuiScrollPanel(
        (Rectangle){gui.posteriors.layoutRecsScrollPanel.x, gui.posteriors.layoutRecsScrollPanel.y, gui.posteriors.layoutRecsScrollPanel.width - gui.posteriors.ScrollPanelPosteriorsBoundsOffset.x, gui.posteriors.layoutRecsScrollPanel.height - gui.posteriors.ScrollPanelPosteriorsBoundsOffset.y },
        (Rectangle){gui.posteriors.layoutRecsScrollPanel.x, gui.posteriors.layoutRecsScrollPanel.y, gui.posteriors.ScrollPanelPosteriorsContent.x, gui.posteriors.ScrollPanelPosteriorsContent.y},
        &(gui.posteriors.ScrollPanelPosteriorsScrollOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScrollPosteriors.x, viewScrollPosteriors.y, viewScrollPosteriors.width, viewScrollPosteriors.height);
        if(mode != MODE_REF || gui.channel.curChannel != CHANNEL_2){
            GuiLabel((Rectangle){gui.posteriors.layoutRecsLabelOuter.x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsLabelOuter.y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsLabelOuter.width, gui.posteriors.layoutRecsLabelOuter.height}, gui.posteriors.LabelOuterText);

            for(int i = 0; i < gui.posteriors.numPosteriors[gui.channel.curChannel]; i++){
                GuiLabel((Rectangle){gui.posteriors.layoutRecsLabelPosteriors[i].x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsLabelPosteriors[i].y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsLabelPosteriors[i].width, gui.posteriors.layoutRecsLabelPosteriors[i].height}, gui.posteriors.LabelPosteriorsText[gui.channel.curChannel][i].c_str());
                GuiTextBox((Rectangle){gui.posteriors.layoutRecsTextBoxOuter[i].x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsTextBoxOuter[i].y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsTextBoxOuter[i].width, gui.posteriors.layoutRecsTextBoxOuter[i].height}, gui.posteriors.TextBoxOuterText[i], CHAR_BUFFER_SIZE, gui.posteriors.TextBoxOuterEditMode[i]);
            }

            for(int i = 0; i < NUMBER_SECRETS; i++){
                GuiLabel((Rectangle){gui.posteriors.layoutRecsLabelX[i].x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsLabelX[i].y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsLabelX[i].width, gui.posteriors.layoutRecsLabelX[i].height}, gui.posteriors.LabelPosteriorsXText[i].c_str());
                for(int j = 0; j < gui.posteriors.numPosteriors[gui.channel.curChannel]; j++){
                    GuiTextBox((Rectangle){gui.posteriors.layoutRecsTextBoxInners[i][j].x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsTextBoxInners[i][j].y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsTextBoxInners[i][j].width, gui.posteriors.layoutRecsTextBoxInners[i][j].height}, gui.posteriors.TextBoxInnersText[i][j], CHAR_BUFFER_SIZE, gui.posteriors.TextBoxInnersEditMode[i][j]);
                }
            }
        }
    EndScissorMode();
}

void drawGuiVisualization(Gui &gui, Data &data){
    drawContentPanel(gui.visualization.layoutRecsTitle, gui.visualization.layoutRecsContent, gui.visualization.GroupBoxVisualizationText, gui.defaultFont);
    if(GuiButton(gui.visualization.layoutRecsButtonDraw, gui.visualization.ButtonDrawText)) buttonDraw(gui, data);
    
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 4);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if(data.error != NO_ERROR) DrawRectangleRec(gui.visualization.layoutRecsTextBoxStatus, WHITE);
    DrawRectangleRec(gui.visualization.layoutRecsTextBoxStatus, WHITE);
    
    if(strcmp(gui.visualization.TextBoxStatusText, "Status")) GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(RED));
    GuiTextBox(gui.visualization.layoutRecsTextBoxStatus, gui.visualization.TextBoxStatusText, CHAR_BUFFER_SIZE, gui.visualization.TextBoxStatusEditMode);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 0);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    
    GuiPanel(gui.visualization.layoutRecsPanelVisualization);

    if(gui.drawing){
        // Checkboxes
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        gui.showLabels = GuiCheckBox(gui.visualization.layoutRecsCheckboxShowLabels, gui.visualization.LabelCheckboxShowLabel, gui.showLabels);
        gui.showConvexHull = GuiCheckBox(gui.visualization.layoutRecsCheckboxShowConvexHull, gui.visualization.LabelCheckboxShowConvexHull, gui.showConvexHull);

        // Triangle
        DrawTriangle(gui.visualization.trianglePoints[0], gui.visualization.trianglePoints[1], gui.visualization.trianglePoints[2], BG_BASE_COLOR_LIGHT2);
        DrawTriangleLines(gui.visualization.trianglePoints[0], gui.visualization.trianglePoints[1], gui.visualization.trianglePoints[2], BLACK);
        for(int i = 0; i < NUMBER_SECRETS; i++){
            DrawTextEx(gui.defaultFontBig, &(gui.visualization.LabelTriangleText[i][0]), (Vector2){gui.visualization.layoutRecsLabelTriangle[i].x, gui.visualization.layoutRecsLabelTriangle[i].y}, 32, 0, BLACK);
        }
        
        int mode = gui.menu.dropdownBoxActive[BUTTON_MODE];

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
    if(gui.menu.windowGettingStartedActive){
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(BG_BASE_COLOR_DARK));
        gui.menu.windowGettingStartedActive = !GuiWindowBox(gui.menu.layoutRecsGettingStarted, "Getting started");
        initStyle();

        // List of options
        GuiSetStyle(LISTVIEW, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(LISTVIEW, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(LISTVIEW, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        gui.menu.gettingStartedMenuActive = GuiListView(gui.menu.layoutRecsGettingStartedMenu, gui.menu.gettingStartedMenuOptions, &gui.menu.gettingStartedMenuScrollIndex, gui.menu.gettingStartedMenuActive);

        // Visualization panel
        DrawRectangleRec(gui.menu.layoutRecsGettingStartedPanel, WHITE);
        if(gui.menu.gettingStartedMenuActive > -1){
            drawGSContent(gui, gui.menu.layoutRecsGettingStartedPanel, gui.menu.gettingStartedMenuActive, gui.menu.imgPadding[gui.menu.gettingStartedMenuActive]);
        }
    }
}

void drawCirclePrior(Gui &gui, Data &data){
    DrawCircle(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR);
    DrawCircleLines(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR_LINES);
    if(gui.showLabels) DrawTextEx(gui.defaultFontBig, gui.visualization.LabelPriorCircleText, (Vector2) {gui.visualization.layoutRecsLabelPriorCircle.x, gui.visualization.layoutRecsLabelPriorCircle.y}, gui.defaultFontBig.baseSize, 1.0, BLACK);
}

void drawCirclesInners(Gui &gui, Data &data, int channel){
	for(int i = 0; i < data.hyper[channel].num_post; i++){
        Color colorFill, colorLines;
        if(channel == CHANNEL_1){
            colorFill = INNERS_COLOR;
            colorLines = INNERS_COLOR_LINES;
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
                DrawTextEx(gui.defaultFontBig, &(gui.posteriors.LabelPosteriorsText[channel][i][0]), (Vector2) {gui.visualization.layoutRecsLabelInnersCircles[channel][i].x-25, gui.visualization.layoutRecsLabelInnersCircles[channel][i].y-25}, 26, 1.0, BLACK);
            else
                DrawTextEx(gui.defaultFontBig, &(gui.posteriors.LabelPosteriorsText[channel][i][0]), (Vector2) {gui.visualization.layoutRecsLabelInnersCircles[channel][i].x-5, gui.visualization.layoutRecsLabelInnersCircles[channel][i].y-5}, 26, 1.0, BLACK);
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
                    DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y, INNERS_COLOR_LINES);
                    DrawTriangle(
                        (Vector2){(float)points[0].x, (float)points[0].y},
                        (Vector2){(float)points[i].x, (float)points[i].y},
                        (Vector2){(float)points[i+1].x, (float)points[i+1].y},
                        CH_COLOR
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
                DrawLine(points[n-1].x, points[n-1].y, points[0].x, points[0].y, INNERS_COLOR_LINES);
            else
                DrawLine(points[n-1].x, points[n-1].y, points[0].x, points[0].y, INNERS2_COLOR_LINES);
        }
	}
}

void drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title, Font font){
    DrawRectangleRec(layoutTitle, TITLES_BASE_COLOR);
    DrawRectangleRec(layoutContent, GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)));
    DrawTextEx(font, title, (Vector2){layoutTitle.x + 10, layoutTitle.y}, font.baseSize, 1, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
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
    GuiTextBoxMulti(
        (Rectangle){panel.x+10, panel.y+10, panel.width-20, panel.height-20},
        gui.menu.gsDescriptionTexts[option],
        gui.defaultFont.baseSize,
        false
    );

    // Image
    if(option != GS_VISUALIZATION)
        DrawTextureEx(gui.menu.gsImages[option], (Vector2){panel.x+10, panel.y+imgPadding}, 0.0f, 0.45f, WHITE);
    else
        DrawTextureEx(gui.menu.gsImages[option], (Vector2){panel.x+60, panel.y+imgPadding}, 0.0f, 0.39f, WHITE);

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
    if(active){
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_LIGHT));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetState(GUI_STATE_PRESSED);
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
    if(GuiButton(gui.channel.layoutRecsTabs[channel], gui.channel.LabelChannelTabs[channel])) buttonsTabs(gui, channel);
    GuiSetState(oldState);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_PRESSED));
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
void buttonFile(Gui &gui, Data &data, bool* closeWindow){
    switch(gui.menu.dropdownBoxActive[BUTTON_FILE]){
        case BUTTON_FILE_OPTION_OPEN:
        #if !defined(PLATFORM_WEB)
            char newPrior[NUMBER_SECRETS][CHAR_BUFFER_SIZE];
            char newChannel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];
            int newNumOutputs;

            if(gui.menu.readQIFFile(newPrior, newChannel, &newNumOutputs) == NO_ERROR){
                // If the current number of outputs is different from the file's, update it
                if(gui.channel.SpinnerChannelValue[gui.channel.curChannel] != newNumOutputs){
                    gui.channel.SpinnerChannelValue[gui.channel.curChannel] = newNumOutputs;
                    gui.drawing = false;
                    gui.channel.updateChannelBySpinner(gui.channel.curChannel, gui.menu.dropdownBoxActive[BUTTON_MODE]);
                }
                GuiPrior::copyPrior(newPrior, gui.prior.TextBoxPriorText);
                GuiChannel::copyChannelText(newChannel, gui.channel.TextBoxChannelText[gui.channel.curChannel], gui.channel.numSecrets[gui.channel.curChannel], newNumOutputs);
                gui.drawing = false;
                strcpy(gui.visualization.TextBoxStatusText, "Status");
            }
        #endif       
            break;
        case BUTTON_FILE_OPTION_SAVE:
        #if !defined(PLATFORM_WEB)
            gui.menu.saveQIFFile(
                gui.prior.TextBoxPriorText,
                gui.channel.TextBoxChannelText[gui.channel.curChannel],
                gui.channel.numOutputs[gui.channel.curChannel],
                strcmp(gui.menu.fileName, "\0") == 0 ? true : false
            );
            if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;   
        #endif
            break;
        case BUTTON_FILE_OPTION_SAVEAS:
        #if !defined(PLATFORM_WEB)
            gui.menu.saveQIFFile(gui.prior.TextBoxPriorText, gui.channel.TextBoxChannelText[gui.channel.curChannel], gui.channel.numOutputs[gui.channel.curChannel], true);
            if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;
        #endif
            break;
        case BUTTON_FILE_OPTION_EXIT:
        #if !defined(PLATFORM_WEB)
            if(data.fileSaved){
                *closeWindow = true;
            }else{
            #if !defined(PLATFORM_WEB)
                int ret;
                FILE *file = popen("zenity --no-wrap --title=\"QIF Graphics\" --question --text=\"Do you want to save changes you made?\"", "r");
                ret = WEXITSTATUS(pclose(file));        // Get the user input Yes=0 or No=1

                if(ret == 0){
                    // Yes
                    gui.menu.saveQIFFile(
                        gui.prior.TextBoxPriorText,
                        gui.channel.TextBoxChannelText[gui.channel.curChannel],
                        gui.channel.numOutputs[gui.channel.curChannel],
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
            #endif
            }
        #endif
            break;
        default:
            break;
    }

    gui.menu.dropdownBoxActive[BUTTON_FILE] = BUTTON_FILE_OPTION_FILE;
}

void buttonMode(Gui &gui, Data &data, int* prevMode){
    int curMode = gui.menu.dropdownBoxActive[BUTTON_MODE];

    if(*prevMode != curMode){
        gui.channel.checkModeAndSizes(curMode);

        // Note: Channel 1 doesn't need to be changed in any mode
        gui.drawing = false;
        updateStatusBar(NO_ERROR, gui.visualization);

        // Set prior to be computed
        data.ready[FLAG_PRIOR] = false;
        data.compute[FLAG_PRIOR] = true;

        data.resetAllExceptComputeChannel1();

        if(curMode == MODE_TWO)
            data.compute[FLAG_CHANNEL_2] = true;
    }

    *prevMode = curMode;
}

void buttonExamples(Gui &gui, Data &data){
    int example = gui.menu.dropdownBoxActive[BUTTON_EXAMPLES];
    
    // If none of the options were seleted, skip.
    if(example != BUTTON_EXAMPLES_OPTION_CH_0 && example != BUTTON_EXAMPLES_OPTION_CH_1)
        return;

    char newChannel[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];
    int curChannel = gui.channel.curChannel;

    gui.channel.SpinnerChannelValue[curChannel] = (example == BUTTON_EXAMPLES_OPTION_CH_0) ? gui.channel.numSecrets[curChannel] : 1;    
    gui.channel.updateChannelBySpinner(gui.channel.curChannel, gui.menu.dropdownBoxActive[BUTTON_MODE]);

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

    GuiChannel::copyChannelText(newChannel, gui.channel.TextBoxChannelText[curChannel], gui.channel.numSecrets[curChannel], gui.channel.numOutputs[curChannel]);
    if(gui.menu.dropdownBoxActive[BUTTON_MODE] == MODE_REF){
        data.resetAllExceptComputeChannel1();
    }else{
        data.compute[FLAG_CHANNEL_1+curChannel] = true;
        data.ready[FLAG_CHANNEL_1+curChannel] = false;
    }
    gui.drawing = false;
}

void buttonHelp(Gui &gui){
    switch(gui.menu.dropdownBoxActive[BUTTON_HELP]){
        case BUTTON_HELP_OPTION_GETTING_STARTED:
            gui.menu.windowGettingStartedActive = true;
            break;
        case BUTTON_HELP_OPTION_ABOUT:
            // TODO
            break;
        default:
            break;
    }
}

void buttonRandomPrior(Gui &gui, Data &data){
    data.newRandomPrior();
    gui.drawing = false;
    data.fileSaved = false;
    data.compute[FLAG_PRIOR] = false;
    data.ready[FLAG_PRIOR] = true;
    gui.updatePriorTextBoxes(data.priorObj);
    
    // Mark all channels (expect 3) to be recomputed and consequently, their corresponding hyper
    for(int channel = 0; channel < NUMBER_CHANNELS-1; channel++){
        data.ready[FLAG_HYPER_1+channel] = false;
        data.compute[FLAG_HYPER_1+channel] = false;
        data.ready[FLAG_CHANNEL_1+channel] = false;
        data.compute[FLAG_CHANNEL_1+channel] = true;
    }
}

void buttonsTabs(Gui &gui, int channel){
    gui.channel.curChannel = channel;
    updateStatusBar(NO_ERROR, gui.visualization);
}

void buttonRandomChannel(Gui &gui, Data &data){
    if(data.error == INVALID_CHANNEL_1 || data.error == INVALID_CHANNEL_2)
        updateStatusBar(NO_ERROR, gui.visualization);
    
    data.newRandomChannel(gui.channel.curChannel, gui.channel.numSecrets[gui.channel.curChannel], gui.channel.numOutputs[gui.channel.curChannel]);
    gui.drawing = false;
    data.fileSaved = false;
    gui.channel.updateChannelTextBoxes(data.channel[gui.channel.curChannel]);

    if(gui.menu.dropdownBoxActive[BUTTON_MODE] == MODE_REF){
        data.resetAllExceptComputeChannel1();
    }else{
        data.compute[FLAG_CHANNEL_1+gui.channel.curChannel] = true;
        data.ready[FLAG_CHANNEL_1+gui.channel.curChannel] = false;
    }
}

void buttonDraw(Gui &gui, Data &data){
    int mode = gui.menu.dropdownBoxActive[BUTTON_MODE];
    if(mode == MODE_SINGLE){
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
