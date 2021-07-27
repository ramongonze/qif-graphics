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

typedef struct WebLoopVariables{
    Gui gui;
    Data data;
    bool closeWindow;
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
void printError(int error, GuiVisualization &visualization);
void checkButtonsMouseCollision(Gui &gui);
void calculatePosteriors(Gui &gui, Data &data);

//----------------------------------------------------------------------------------
// Draw Functions Declaration
//----------------------------------------------------------------------------------
void drawGuiMenu(Gui &gui, Data &data, bool *closeWindow);
void drawGuiPrior(Gui &gui, Data &data);
void drawGuiChannel(Gui &gui, Data &data);
void drawGuiPosteriors(Gui &gui);
void drawGuiVisualization(Gui &gui, Data &data);
void drawGettingStarted(Gui &gui);
void drawCircles(Gui &gui, Data &data);
void drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title, Font font);
void drawGSContent(Gui &gui, Rectangle panel, int option, int imgPadding);

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
void buttonFile(Gui &gui, Data &data, bool *closeWindow);
void buttonExamples(Gui &gui);
void buttonHelp(Gui &gui);
void buttonRandomPrior(Gui &gui, Data &data);
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

    // Update
    //----------------------------------------------------------------------------------
    *closeWindow = WindowShouldClose();
    Vector2 mousePosition = GetMousePosition();
    
    // Menu buttons
    checkButtonsMouseCollision(*gui);
    buttonFile(*gui, *data, closeWindow);
    buttonExamples(*gui);
    buttonHelp(*gui);

    // Check if channel spinner value was changed
    if(gui->channel.SpinnerChannelValue != gui->channel.numOutputs){
        gui->drawing = false;
        data->fileSaved = false;
        gui->channel.updateChannelBySpinner();
    }
    
    // Check if a TextBox is being pressed
    if(gui->checkTextBoxPressed()){
        gui->drawing = false;
        data->error = false;
        data->fileSaved = false;
        printError(data->error, gui->visualization);

        if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || 
            IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
            gui->moveAmongTextBoxes();
        }
    }

    // Check if prior circle was moved
    if(gui->drawing){
        // Animation
        if(data->animationRunning){
            data->buildCircles(gui->visualization.trianglePoints);
        }

        // Moving prior with mouse
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && euclidianDistance(data->priorCircle.center, mousePosition) <= PRIOR_RADIUS){
            data->mouseClickedOnPrior = true;
        }

        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) data->mouseClickedOnPrior = false;
        
        if(data->mouseClickedOnPrior){
            data->fileSaved = false;
            data->updateHyper(gui->visualization.trianglePoints);
            data->animation = UPDATE_CIRCLES_BY_MOUSE;
            data->buildCircles(gui->visualization.trianglePoints);
            gui->updatePrior(data->hyper.prior, data->priorCircle);
            gui->updatePosteriors(data->hyper, data->innersCircles);
        }
    }
    
    // If prior and channel are ok, udpate and show posteriors
    calculatePosteriors(*gui, *data);

    //----------------------------------------------------------------------------------

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
        drawGuiPosteriors(*gui);
        drawGuiVisualization(*gui, *data);
        drawGuiMenu(*gui, *data, closeWindow);
        if(gui->menu.windowGettingStartedActive) GuiUnlock();
        drawGettingStarted(*gui);
        //----------------------------------------------------------------------------------

    EndDrawing();
    //------------------------------------------------------'----------------------------
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

void printError(int error, GuiVisualization &visualization){
	switch(error){
		case INVALID_VALUE:
			strcpy(visualization.TextBoxStatusText, "Some value in prior or channel is invalid!");
			break;
		case INVALID_PRIOR:
			strcpy(visualization.TextBoxStatusText, "The prior distribution is invalid!");
			break;
		case INVALID_CHANNEL:
			strcpy(visualization.TextBoxStatusText, "The channel is invalid!");
			break;
		case NO_ERROR:
			strcpy(visualization.TextBoxStatusText, "Status");
	}
}

void checkButtonsMouseCollision(Gui &gui){
    // Active the button the mouse is over it
    if(gui.menu.dropdownFileEditMode || gui.menu.dropdownExamplesEditMode || gui.menu.dropdownHelpEditMode){
        Vector2 mousePoint = GetMousePosition();

        if(CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[REC_BUTTON_FILE])){
            gui.menu.dropdownBoxFileActive = BUTTON_FILE_OPTION_FILE;
            gui.menu.dropdownFileEditMode = true;
            gui.menu.dropdownBoxExamplesActive = BUTTON_EXAMPLES_OPTION_EXAMPLES;
            gui.menu.dropdownExamplesEditMode = false;
            gui.menu.dropdownBoxHelpActive = BUTTON_HELP_OPTION_HELP;
            gui.menu.dropdownHelpEditMode = false;
        }

        if(CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[REC_BUTTON_EXAMPLES])){
            gui.menu.dropdownBoxExamplesActive = BUTTON_EXAMPLES_OPTION_EXAMPLES;
            gui.menu.dropdownExamplesEditMode = true;
            gui.menu.dropdownBoxFileActive = BUTTON_FILE_OPTION_FILE;
            gui.menu.dropdownFileEditMode = false;
            gui.menu.dropdownBoxHelpActive = BUTTON_HELP_OPTION_HELP;
            gui.menu.dropdownHelpEditMode = false;
        }

        if(CheckCollisionPointRec(mousePoint, gui.menu.layoutRecsButtons[REC_BUTTON_HELP])){
            gui.menu.dropdownBoxHelpActive = BUTTON_HELP_OPTION_HELP;
            gui.menu.dropdownHelpEditMode = true;
            gui.menu.dropdownBoxFileActive = BUTTON_FILE_OPTION_FILE;
            gui.menu.dropdownFileEditMode = false;
            gui.menu.dropdownBoxExamplesActive = BUTTON_EXAMPLES_OPTION_EXAMPLES;
            gui.menu.dropdownExamplesEditMode = false;
        }
    } 
}

void calculatePosteriors(Gui &gui, Data &data){
    data.error = NO_ERROR;

    // Check if prior and channel are ok
    if(data.checkPriorText(gui.prior.TextBoxPriorText) == NO_ERROR && data.checkChannelText(gui.channel.TextBoxChannelText, gui.channel.numOutputs) == NO_ERROR){
        // Check if typed numbers represent distributions
        if(Distribution::isDistribution(data.prior) == false) data.error = INVALID_PRIOR;
        else if(Channel::isChannel(data.channel) == false) data.error = INVALID_CHANNEL;

        if(data.error == NO_ERROR){
            Distribution newPrior(data.prior);
            Channel newChannel(newPrior, data.channel);
            data.hyper = Hyper(newChannel);
            gui.updatePriorRectangle(data.priorCircle);
            gui.updatePosteriors(data.hyper, data.innersCircles);
        }else{
            gui.posteriors.resetPosteriors();
        }
    }else{
        data.error = INVALID_VALUE;
        gui.posteriors.resetPosteriors();
    }
}

//------------------------------------------------------------------------------------
// Draw Functions Definitions (local)
//------------------------------------------------------------------------------------
void drawGuiMenu(Gui &gui, Data &data, bool *closeWindow){
    DrawRectangleRec(gui.menu.layoutRecsMenu, MENU_BASE_COLOR_NORMAL);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

#if !defined(PLATFORM_WEB)
    // Button File
    if(!gui.menu.dropdownFileEditMode) gui.menu.dropdownBoxFileActive = BUTTON_FILE_OPTION_FILE;        // Reset selection
    if(GuiDropdownBox(gui.menu.layoutRecsButtons[REC_BUTTON_FILE], 120, gui.menu.buttonFileText, &(gui.menu.dropdownBoxFileActive), gui.menu.dropdownFileEditMode)) gui.menu.dropdownFileEditMode = !gui.menu.dropdownFileEditMode;
#endif

    // Button Examples
    if(!gui.menu.dropdownExamplesEditMode) gui.menu.dropdownBoxExamplesActive = BUTTON_EXAMPLES_OPTION_EXAMPLES;        // Reset selection
    if(GuiDropdownBox(gui.menu.layoutRecsButtons[REC_BUTTON_EXAMPLES], 320, gui.menu.buttonExamplesText, &(gui.menu.dropdownBoxExamplesActive), gui.menu.dropdownExamplesEditMode)) gui.menu.dropdownExamplesEditMode = !gui.menu.dropdownExamplesEditMode;
    
    // Button Help
    if(!gui.menu.dropdownHelpEditMode) gui.menu.dropdownBoxHelpActive = BUTTON_HELP_OPTION_HELP;        // Reset selection
    if(GuiDropdownBox(gui.menu.layoutRecsButtons[REC_BUTTON_HELP], 170, gui.menu.buttonHelpText, &(gui.menu.dropdownBoxHelpActive), gui.menu.dropdownHelpEditMode)) gui.menu.dropdownHelpEditMode = !gui.menu.dropdownHelpEditMode;

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
    drawContentPanel(gui.channel.layoutRecsTitle, gui.channel.layoutRecsContent, gui.channel.panelChannelText, gui.defaultFont);
    
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
    if(GuiButton(gui.channel.layoutRecsButtonRandom, gui.channel.buttonRandomText)) buttonRandomChannel(gui, data); 
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
    if(GuiSpinner(gui.channel.layoutRecsSpinner, gui.channel.LabelOutputsText, &(gui.channel.SpinnerChannelValue), 0, 100, gui.channel.SpinnerChannelEditMode)) gui.channel.SpinnerChannelEditMode = !gui.channel.SpinnerChannelEditMode;
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScrollChannel = GuiScrollPanel(
        (Rectangle){gui.channel.layoutRecsScrollPanel.x, gui.channel.layoutRecsScrollPanel.y, gui.channel.layoutRecsScrollPanel.width - gui.channel.ScrollPanelChannelBoundsOffset.x, gui.channel.layoutRecsScrollPanel.height - gui.channel.ScrollPanelChannelBoundsOffset.y },
        (Rectangle){gui.channel.layoutRecsScrollPanel.x, gui.channel.layoutRecsScrollPanel.y, gui.channel.ScrollPanelChannelContent.x, gui.channel.ScrollPanelChannelContent.y},
        &(gui.channel.ScrollPanelChannelScrollOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScrollChannel.x, viewScrollChannel.y, viewScrollChannel.width, viewScrollChannel.height);
        GuiLabel((Rectangle){gui.channel.layoutRecsLabelOutputs.x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelOutputs.y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelOutputs.width, gui.channel.layoutRecsLabelOutputs.height}, gui.channel.LabelOutputsText);
        for(int i = 0; i < NUMBER_SECRETS; i++){
            GuiLabel((Rectangle){gui.channel.layoutRecsLabelX[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelX[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelX[i].width, gui.channel.layoutRecsLabelX[i].height}, gui.channel.LabelChannelXText[i].c_str());
            for(int j = 0; j < gui.channel.numOutputs; j++){
                if(GuiTextBox((Rectangle){gui.channel.layoutRecsTextBoxChannel[i][j].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsTextBoxChannel[i][j].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsTextBoxChannel[i][j].width, gui.channel.layoutRecsTextBoxChannel[i][j].height}, gui.channel.TextBoxChannelText[i][j], CHAR_BUFFER_SIZE, gui.channel.TextBoxChannelEditMode[i][j])) gui.channel.TextBoxChannelEditMode[i][j] = !gui.channel.TextBoxChannelEditMode[i][j];
            }
        }

        for(int i = 0; i < gui.channel.numOutputs; i++){
            GuiLabel((Rectangle){gui.channel.layoutRecsLabelY[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelY[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelY[i].width, gui.channel.layoutRecsLabelY[i].height}, gui.channel.LabelChannelYText[i].c_str());
        }
    EndScissorMode();
}

void drawGuiPosteriors(Gui &gui){
    drawContentPanel(gui.posteriors.layoutRecsTitle, gui.posteriors.layoutRecsContent, gui.posteriors.GroupBoxPosteriorsText, gui.defaultFont);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScrollPosteriors = GuiScrollPanel(
        (Rectangle){gui.posteriors.layoutRecsScrollPanel.x, gui.posteriors.layoutRecsScrollPanel.y, gui.posteriors.layoutRecsScrollPanel.width - gui.posteriors.ScrollPanelPosteriorsBoundsOffset.x, gui.posteriors.layoutRecsScrollPanel.height - gui.posteriors.ScrollPanelPosteriorsBoundsOffset.y },
        (Rectangle){gui.posteriors.layoutRecsScrollPanel.x, gui.posteriors.layoutRecsScrollPanel.y, gui.posteriors.ScrollPanelPosteriorsContent.x, gui.posteriors.ScrollPanelPosteriorsContent.y},
        &(gui.posteriors.ScrollPanelPosteriorsScrollOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScrollPosteriors.x, viewScrollPosteriors.y, viewScrollPosteriors.width, viewScrollPosteriors.height);
        GuiLabel((Rectangle){gui.posteriors.layoutRecsLabelOuter.x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsLabelOuter.y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsLabelOuter.width, gui.posteriors.layoutRecsLabelOuter.height}, gui.posteriors.LabelOuterText);

        for(int i = 0; i < gui.posteriors.numPosteriors; i++){
            GuiLabel((Rectangle){gui.posteriors.layoutRecsLabelPosteriors[i].x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsLabelPosteriors[i].y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsLabelPosteriors[i].width, gui.posteriors.layoutRecsLabelPosteriors[i].height}, gui.posteriors.LabelPosteriorsText[i].c_str());
            GuiTextBox((Rectangle){gui.posteriors.layoutRecsTextBoxOuter[i].x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsTextBoxOuter[i].y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsTextBoxOuter[i].width, gui.posteriors.layoutRecsTextBoxOuter[i].height}, gui.posteriors.TextBoxOuterText[i], CHAR_BUFFER_SIZE, gui.posteriors.TextBoxOuterEditMode[i]);
        }

        for(int i = 0; i < NUMBER_SECRETS; i++){
            GuiLabel((Rectangle){gui.posteriors.layoutRecsLabelX[i].x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsLabelX[i].y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsLabelX[i].width, gui.posteriors.layoutRecsLabelX[i].height}, gui.posteriors.LabelPosteriorsXText[i].c_str());
            for(int j = 0; j < gui.posteriors.numPosteriors; j++){
                GuiTextBox((Rectangle){gui.posteriors.layoutRecsTextBoxInners[i][j].x + gui.posteriors.ScrollPanelPosteriorsScrollOffset.x, gui.posteriors.layoutRecsTextBoxInners[i][j].y + gui.posteriors.ScrollPanelPosteriorsScrollOffset.y, gui.posteriors.layoutRecsTextBoxInners[i][j].width, gui.posteriors.layoutRecsTextBoxInners[i][j].height}, gui.posteriors.TextBoxInnersText[i][j], CHAR_BUFFER_SIZE, gui.posteriors.TextBoxInnersEditMode[i][j]);
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
        // Triangle
        DrawTriangleLines(gui.visualization.trianglePoints[0], gui.visualization.trianglePoints[1], gui.visualization.trianglePoints[2], BLACK);
        for(int i = 0; i < 3; i++){
            DrawTextEx(gui.defaultFontBig, &(gui.visualization.LabelTriangleText[i][0]), (Vector2){gui.visualization.layoutRecsLabelTriangle[i].x, gui.visualization.layoutRecsLabelTriangle[i].y}, 32, 0, BLACK);
        }
        
        // Circles
        drawCircles(gui, data);
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

void drawCircles(Gui &gui, Data &data){
	// Prior
    DrawCircle(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR);
    DrawCircleLines(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR_LINES);
    DrawTextEx(gui.defaultFontBig, gui.visualization.LabelPriorCircleText, (Vector2) {gui.visualization.layoutRecsLabelPriorCircle.x, gui.visualization.layoutRecsLabelPriorCircle.y}, gui.defaultFontBig.baseSize, 1.0, BLACK);

	// Inners
	for(int i = 0; i < data.hyper.num_post; i++){
        DrawCircle(data.innersCircles[i].center.x, data.innersCircles[i].center.y, data.innersCircles[i].radius, INNERS_COLOR);
        DrawCircleLines(data.innersCircles[i].center.x, data.innersCircles[i].center.y, data.innersCircles[i].radius, INNERS_COLOR_LINES);
        
        if(data.hyper.outer.prob[i] < 0.1f)
            DrawTextEx(gui.defaultFontBig, &(gui.posteriors.LabelPosteriorsText[i][0]), (Vector2) {gui.visualization.layoutRecsLabelInnersCircles[i].x-25, gui.visualization.layoutRecsLabelInnersCircles[i].y-25}, 26, 1.0, BLACK);
        else
            DrawTextEx(gui.defaultFontBig, &(gui.posteriors.LabelPosteriorsText[i][0]), (Vector2) {gui.visualization.layoutRecsLabelInnersCircles[i].x-5, gui.visualization.layoutRecsLabelInnersCircles[i].y-5}, 26, 1.0, BLACK);
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
    DrawTextureEx(gui.menu.gsImages[option], (Vector2){panel.x+10, panel.y+imgPadding}, 0.0f, 0.45f, WHITE);

    initStyle();
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
void buttonFile(Gui &gui, Data &data, bool *closeWindow){
    switch(gui.menu.dropdownBoxFileActive){
        case BUTTON_FILE_OPTION_OPEN:
        #if !defined(PLATFORM_WEB)
            char newPrior[NUMBER_SECRETS][CHAR_BUFFER_SIZE];
            char newChannel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];
            int newNumOutputs;

            if(gui.menu.readQIFFile(newPrior, newChannel, &newNumOutputs) == NO_ERROR){
                // If the current number of outputs is different from the file's, update it
                if(gui.channel.SpinnerChannelValue != newNumOutputs){
                    gui.channel.SpinnerChannelValue = newNumOutputs;
                    gui.drawing = false;
                    gui.channel.updateChannelBySpinner();
                }
                GuiPrior::copyPrior(newPrior, gui.prior.TextBoxPriorText);
                GuiChannel::copyChannelText(newChannel, gui.channel.TextBoxChannelText, newNumOutputs);
                gui.drawing = false;
                strcpy(gui.visualization.TextBoxStatusText, "Status");
            }
        #endif       
            break;
        case BUTTON_FILE_OPTION_SAVE:
        #if !defined(PLATFORM_WEB)
            gui.menu.saveQIFFile(
                gui.prior.TextBoxPriorText,
                gui.channel.TextBoxChannelText,
                gui.channel.numOutputs,
                strcmp(gui.menu.fileName, "\0") == 0 ? true : false
            );
            if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;   
        #endif
            break;
        case BUTTON_FILE_OPTION_SAVEAS:
        #if !defined(PLATFORM_WEB)
            gui.menu.saveQIFFile(gui.prior.TextBoxPriorText, gui.channel.TextBoxChannelText, gui.channel.numOutputs, true);
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
                        gui.channel.TextBoxChannelText,
                        gui.channel.numOutputs,
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

    gui.menu.dropdownBoxFileActive = BUTTON_FILE_OPTION_FILE;
}

void buttonExamples(Gui &gui){
    switch(gui.menu.dropdownBoxExamplesActive){
        char newChannel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

        case BUTTON_EXAMPLES_OPTION_CH_0:
            if(gui.channel.SpinnerChannelValue != 3){
                gui.channel.SpinnerChannelValue = 3;
                gui.drawing = false;
                gui.channel.updateChannelBySpinner();
            }

            // Set identity matrix
            for(int i = 0; i < NUMBER_SECRETS; i++){
                for(int j = 0; j < 3; j++){
                    if(i == j) strcpy(newChannel[i][j], "1");
                    else strcpy(newChannel[i][j], "0");
                }
            }

            GuiChannel::copyChannelText(newChannel, gui.channel.TextBoxChannelText, 3);
            gui.drawing = false;
            break;
        case BUTTON_EXAMPLES_OPTION_CH_1:
            if(gui.channel.SpinnerChannelValue != 1){
                gui.channel.SpinnerChannelValue = 1;
                gui.drawing = false;
                gui.channel.updateChannelBySpinner();
            }

            // Set channel that leaks nothing
            for(int i = 0; i < NUMBER_SECRETS; i++){
                strcpy(newChannel[i][0], "1");
            }

            GuiChannel::copyChannelText(newChannel, gui.channel.TextBoxChannelText, 1);
            gui.drawing = false;
            break;
        default:
            break;
    }
}

void buttonHelp(Gui &gui){
    switch(gui.menu.dropdownBoxHelpActive){
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
    if(data.error == INVALID_PRIOR){
        data.error = NO_ERROR;
        strcpy(gui.visualization.TextBoxStatusText, "Status");
    }
    data.newRandomPrior();
    gui.drawing = false;
    data.fileSaved = false;
    Distribution newPrior(data.prior);
    gui.updatePrior(newPrior, data.priorCircle);
}

void buttonRandomChannel(Gui &gui, Data &data){
    if(data.error == INVALID_CHANNEL){
        data.error = NO_ERROR;
        strcpy(gui.visualization.TextBoxStatusText, "Status");
    }
    
    data.newRandomChannel(gui.channel.numOutputs);
    gui.drawing = false;
    data.fileSaved = false;
    gui.channel.updateChannelTextBoxes(data.channel);
}

void buttonDraw(Gui &gui, Data &data){
    if(data.error == NO_ERROR){
        gui.drawing = true;
        data.animationRunning = true;
        data.animation = STEPS;
        data.buildCircles(gui.visualization.trianglePoints);
    }else{
        printError(data.error, gui.visualization);
    }    
}
