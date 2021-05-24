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

#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    typedef struct WebLoopVariables{
        Gui gui;
        Data data;
        bool closeWindow;
    } WebLoopVariables;
    #include "/home/ramon/lib/emsdk/upstream/emscripten/system/include/emscripten.h"
#endif

//----------------------------------------------------------------------------------
// General Functions Declaration
//----------------------------------------------------------------------------------
void initStyle();
void printError(int error, GuiVisualization &visualization);
void drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title);
void UpdateDrawFrame(void* vars_);     // Update and Draw one frame

//----------------------------------------------------------------------------------
// Draw Functions Declaration
//----------------------------------------------------------------------------------
void drawGuiMenu(Gui &gui, Data &data, bool *closeWindow);
void drawGuiPrior(Gui &gui, Data &data);
void drawGuiChannel(Gui &gui, Data &data);
void drawGuiPosteriors(GuiPosteriors &posteriors);
void drawGuiVisualization(Gui &gui, Data &data);
void drawCircles(Gui &gui, Data &data);

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
void buttonFile(Gui &gui, Data &data, bool *closeWindow);
void buttonExamples();
void buttonHelp();
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
    GuiLoadStyle("src/gui/style-qif-graphics.rgs");
    initStyle();

#if defined(PLATFORM_WEB)
    WebLoopVariables vars;
    vars.closeWindow = false;
    emscripten_set_main_loop_arg(UpdateDrawFrame, &vars, 60, 1);
#else
    bool closeWindow = false;
    //----------------------------------------------------------------------------------
    Gui gui = Gui();
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    Data data = Data();
    //--------------------------------------------------------------------------------------

    // Main game loop
    while(!closeWindow){    // Detect window close button or ESC key
        UpdateDrawFrame(&vars);
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

void drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title){
    DrawRectangleRec(layoutTitle, TITLES_BASE_COLOR);
    DrawRectangleRec(layoutContent, GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)));
    DrawTextEx(GuiGetFont(), title, (Vector2){layoutTitle.x + 10, layoutTitle.y}, GuiGetFont().baseSize, 1, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
}

void UpdateDrawFrame(void* vars_){
    WebLoopVariables* vars = (WebLoopVariables*) vars_;
    Gui* gui = &(vars->gui);
    Data* data = &(vars->data);
    bool* closeWindow = &(vars->closeWindow);

    // Update
    //----------------------------------------------------------------------------------
    *closeWindow = WindowShouldClose();
    Vector2 mousePosition = GetMousePosition();
    
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
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
            euclidianDistance(data->priorCircle.center, mousePosition) <= PRIOR_RADIUS){
            data->mouseClickedOnPrior = true;
        }

        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) data->mouseClickedOnPrior = false;
        
        if(data->mouseClickedOnPrior){
            data->fileSaved = false;
            data->updateHyper(gui->visualization.trianglePoints);
            data->buildCircles(gui->visualization.trianglePoints);
            gui->updatePrior(data->hyper.prior, data->priorCircle);
            gui->updatePosteriors(data->hyper, data->innersCircles);
        }
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(BG_BASE_COLOR_DARK); 

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        // Draw controls
        drawGuiPrior(*gui, *data);
        drawGuiChannel(*gui, *data);
        drawGuiPosteriors(gui->posteriors);
        drawGuiVisualization(*gui, *data);
        drawGuiMenu(*gui, *data, closeWindow);
        //----------------------------------------------------------------------------------

    EndDrawing();
    //----------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------------
// Draw Functions Definitions (local)
//------------------------------------------------------------------------------------
void drawGuiMenu(Gui &gui, Data &data, bool *closeWindow){
    DrawRectangleRec(gui.menu.layoutRecsMenu, MENU_BASE_COLOR_NORMAL);
    
    // Button File
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if (GuiDropdownBox(gui.menu.layoutRecsButtons[REC_BUTTON_FILE], 120, gui.menu.buttonFileText, &(gui.menu.dropdownBoxFileActive), gui.menu.dropdownFileEditMode)) gui.menu.dropdownFileEditMode = !gui.menu.dropdownFileEditMode;
    initStyle();
    buttonFile(gui, data, closeWindow);

    // Button Examples
    if (GuiButton(gui.menu.layoutRecsButtons[REC_BUTTON_EXAMPLES], gui.menu.buttonExamplesText)) buttonExamples(); 

    // Button Help
    if (GuiButton(gui.menu.layoutRecsButtons[REC_BUTTON_HELP], gui.menu.buttonHelpText)) buttonHelp(); 
}

void drawGuiPrior(Gui &gui, Data &data){
    drawContentPanel(gui.prior.layoutRecsTitle, gui.prior.layoutRecsContent, gui.prior.panelPriorText);
    DrawRectangleRec(gui.prior.layoutRecsPanel, WHITE);
    DrawRectangleLinesEx(gui.prior.layoutRecsPanel, 1, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR));
    if (GuiButton(gui.prior.layoutRecsButtonRandom, gui.prior.buttonRandomText)) buttonRandomPrior(gui, data); 
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

    for(int i = 0; i < NUMBER_SECRETS; i++){
        GuiLabel(gui.prior.layoutRecsLabel[i], gui.prior.LabelPriorText[i].c_str());
        if (GuiTextBox(gui.prior.layoutRecsTextBox[i], gui.prior.TextBoxPriorText[i], CHAR_BUFFER_SIZE, gui.prior.TextBoxPriorEditMode[i])) gui.prior.TextBoxPriorEditMode[i] = !gui.prior.TextBoxPriorEditMode[i];
    }
}

void drawGuiChannel(Gui &gui, Data &data){
    drawContentPanel(gui.channel.layoutRecsTitle, gui.channel.layoutRecsContent, gui.channel.GroupBoxChannelText);
    
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR));
    if (GuiButton(gui.channel.layoutRecsButtonRandom, gui.channel.buttonRandomText)) buttonRandomChannel(gui, data); 
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
    if (GuiSpinner(gui.channel.layoutRecsSpinner, gui.channel.LabelOutputsText, &(gui.channel.SpinnerChannelValue), 0, 100, gui.channel.SpinnerChannelEditMode)) gui.channel.SpinnerChannelEditMode = !gui.channel.SpinnerChannelEditMode;
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
        for(int i = 0; i < gui.channel.numOutputs; i++){
            GuiLabel((Rectangle){gui.channel.layoutRecsLabelY[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelY[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelY[i].width, gui.channel.layoutRecsLabelY[i].height}, gui.channel.LabelChannelYText[i].c_str());
            for(int j = 0; j < 3; j++){
                if (GuiTextBox((Rectangle){gui.channel.layoutRecsTextBoxChannel[i][j].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsTextBoxChannel[i][j].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsTextBoxChannel[i][j].width, gui.channel.layoutRecsTextBoxChannel[i][j].height}, gui.channel.TextBoxChannelText[i][j], CHAR_BUFFER_SIZE, gui.channel.TextBoxChannelEditMode[i][j]))gui.channel.TextBoxChannelEditMode[i][j] = !gui.channel.TextBoxChannelEditMode[i][j];
            }
        }

        for(int i = 0; i < 3; i++){
            GuiLabel((Rectangle){gui.channel.layoutRecsLabelX[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelX[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelX[i].width, gui.channel.layoutRecsLabelX[i].height}, gui.channel.LabelChannelXText[i].c_str());
        }
    EndScissorMode();
}

void drawGuiPosteriors(GuiPosteriors &posteriors){
    drawContentPanel(posteriors.layoutRecsTitle, posteriors.layoutRecsContent, posteriors.GroupBoxPosteriorsText);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScrollPosteriors = GuiScrollPanel(
        (Rectangle){posteriors.layoutRecsScrollPanel.x, posteriors.layoutRecsScrollPanel.y, posteriors.layoutRecsScrollPanel.width - posteriors.ScrollPanelPosteriorsBoundsOffset.x, posteriors.layoutRecsScrollPanel.height - posteriors.ScrollPanelPosteriorsBoundsOffset.y },
        (Rectangle){posteriors.layoutRecsScrollPanel.x, posteriors.layoutRecsScrollPanel.y, posteriors.ScrollPanelPosteriorsContent.x, posteriors.ScrollPanelPosteriorsContent.y},
        &(posteriors.ScrollPanelPosteriorsScrollOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScrollPosteriors.x, viewScrollPosteriors.y, viewScrollPosteriors.width, viewScrollPosteriors.height);
        GuiLabel((Rectangle){posteriors.layoutRecsLabelOuter.x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsLabelOuter.y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsLabelOuter.width, posteriors.layoutRecsLabelOuter.height}, posteriors.LabelOuterText);

        for(int i = 0; i < posteriors.numPosteriors; i++){
            GuiLabel((Rectangle){posteriors.layoutRecsLabelPosteriors[i].x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsLabelPosteriors[i].y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsLabelPosteriors[i].width, posteriors.layoutRecsLabelPosteriors[i].height}, posteriors.LabelPosteriorsText[i].c_str());
        }

        for(int i = 0; i < 3; i++){
            GuiLabel((Rectangle){posteriors.layoutRecsLabelX[i].x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsLabelX[i].y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsLabelX[i].width, posteriors.layoutRecsLabelX[i].height}, posteriors.LabelPosteriorsXText[i].c_str());
        }

        for(int i = 0; i < posteriors.numPosteriors; i++){
            GuiTextBox((Rectangle){posteriors.layoutRecsTextBoxOuter[i].x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsTextBoxOuter[i].y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsTextBoxOuter[i].width, posteriors.layoutRecsTextBoxOuter[i].height}, posteriors.TextBoxOuterText[i], CHAR_BUFFER_SIZE, posteriors.TextBoxOuterEditMode[i]);
        }

        for(int i = 0; i < posteriors.numPosteriors; i++){
            for(int j = 0; j < 3; j++){
                GuiTextBox((Rectangle){posteriors.layoutRecsTextBoxInners[i][j].x + posteriors.ScrollPanelPosteriorsScrollOffset.x, posteriors.layoutRecsTextBoxInners[i][j].y + posteriors.ScrollPanelPosteriorsScrollOffset.y, posteriors.layoutRecsTextBoxInners[i][j].width, posteriors.layoutRecsTextBoxInners[i][j].height}, posteriors.TextBoxInnersText[i][j], CHAR_BUFFER_SIZE, posteriors.TextBoxInnersEditMode[i][j]);
            }
        }
    EndScissorMode();
}

void drawGuiVisualization(Gui &gui, Data &data){
    drawContentPanel(gui.visualization.layoutRecsTitle, gui.visualization.layoutRecsContent, gui.visualization.GroupBoxVisualizationText);
    if (GuiButton(gui.visualization.layoutRecsButtonDraw, gui.visualization.ButtonDrawText)) buttonDraw(gui, data);
    
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
            DrawTextEx(gui.visualization.alternativeFont, &(gui.visualization.LabelTriangleText[i][0]), (Vector2){gui.visualization.layoutRecsLabelTriangle[i].x, gui.visualization.layoutRecsLabelTriangle[i].y}, 32, 0, BLACK);
        }
        
        // Circles
        drawCircles(gui, data);
    }
}

void drawCircles(Gui &gui, Data &data){
	// Prior
    DrawCircle(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR);
    DrawCircleLines(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR_LINES);
	DrawTextEx(gui.visualization.alternativeFont, gui.visualization.LabelPriorCircleText , (Vector2) {gui.visualization.layoutRecsLabelPriorCircle.x, gui.visualization.layoutRecsLabelPriorCircle.y}, gui.visualization.alternativeFont.baseSize, 1.0, BLACK);

	// Inners
	for(long unsigned int i = 0; i < data.innersCircles.size(); i++){
        DrawCircle(data.innersCircles[i].center.x, data.innersCircles[i].center.y, data.innersCircles[i].radius, INNERS_COLOR);
        DrawCircleLines(data.innersCircles[i].center.x, data.innersCircles[i].center.y, data.innersCircles[i].radius, INNERS_COLOR_LINES);
        DrawTextEx(gui.visualization.alternativeFont, &(gui.posteriors.LabelPosteriorsText[i][0]), (Vector2) {gui.visualization.layoutRecsLabelInnersCircles[i].x, gui.visualization.layoutRecsLabelInnersCircles[i].y}, 26, 1.0, BLACK);
	}
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
void buttonFile(Gui &gui, Data &data, bool *closeWindow){
    char* newPrior[NUMBER_SECRETS];
    vector<vector<char*>> newChannel;

    switch(gui.menu.dropdownBoxFileActive){
        case BUTTON_FILE_OPTION_OPEN:
            if(gui.menu.readQIFFile(newPrior, newChannel) == NO_ERROR){
                // If the current number of outputs is different from the file's, update it
                int newNumOutputs = (int) newChannel.size();
                if(gui.channel.SpinnerChannelValue != newNumOutputs){
                    gui.channel.SpinnerChannelValue = newNumOutputs;
                    gui.drawing = false;
                    gui.channel.updateChannelBySpinner();
                }
                // gui.prior.TextBoxPriorText = newPrior;
                GuiPrior::copyPrior(newPrior, gui.prior.TextBoxPriorText);
                gui.channel.TextBoxChannelText = newChannel;
                gui.drawing = false;
            }            
            break;
        case BUTTON_FILE_OPTION_SAVE:
            gui.menu.saveQIFFile(
                gui.prior.TextBoxPriorText,
                gui.channel.TextBoxChannelText,
                strcmp(gui.menu.fileName, "\0") == 0 ? true : false
            );
            if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;   
            break;

        case BUTTON_FILE_OPTION_SAVEAS:
            gui.menu.saveQIFFile(gui.prior.TextBoxPriorText, gui.channel.TextBoxChannelText, true);
            if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;
            break;

        case BUTTON_FILE_OPTION_EXIT:
            if(data.fileSaved){
                *closeWindow = true;
            }else{
                
                // int ret;
                // FILE *file = popen("zenity --no-wrap --title=\"QIF Graphics\" --question --text=\"Do you want to save changes you made?\"", "r");
                // ret = WEXITSTATUS(pclose(file));        // Get the user input Yes=0 or No=1

                // if(ret == 0){
                //     // Yes
                //     gui.menu.saveQIFFile(
                //         gui.prior.TextBoxPriorText,
                //         gui.channel.TextBoxChannelText,
                //         strcmp(gui.menu.fileName, "\0") == 0 ? true : false
                //     );
                //     if(strcmp(gui.menu.fileName, "\0")){
                //         data.fileSaved = true;
                //         *closeWindow = true;  
                //     }
                // }else{
                //     // No
                //     *closeWindow = true;
                // }
            }
            break;

        default:
            break;
    }

    gui.menu.dropdownBoxFileActive = BUTTON_FILE_OPTION_FILE;
}

void buttonExamples(){
    // TODO: Implement control logic
}

void buttonHelp(){
    // TODO: Implement control logic
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
    data.error = NO_ERROR;

    // Check if prior and channel are ok
    if(data.checkPriorText(gui.prior.TextBoxPriorText) == NO_ERROR && data.checkChannelText(gui.channel.TextBoxChannelText) == NO_ERROR){
        // Check if typed numbers represent distributions
        if(Distribution::isDistribution(data.prior) == false) data.error = INVALID_PRIOR;
        else if(Channel::isChannel(data.channel) == false) data.error = INVALID_CHANNEL;

        if(data.error == NO_ERROR){
            Distribution newPrior(data.prior);
            Channel newChannel(newPrior, data.channel);
            data.hyper = Hyper(newChannel);

            data.buildCircles(gui.visualization.trianglePoints);

            gui.updatePriorRectangle(data.priorCircle);
            gui.updatePosteriors(data.hyper, data.innersCircles);
            gui.drawing = true;
        }else{
            gui.drawing = false;
        }
    }else{
        data.error = INVALID_VALUE;
        gui.drawing = false;
    }
    
    printError(data.error, gui.visualization);
}
