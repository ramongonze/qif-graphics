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

#define GUI_FILE_DIALOG_IMPLEMENTATION
#include "gui/gui_file_dialog.h"

#include <iostream>
#include "gui/gui.h"
#include "data.h"

//----------------------------------------------------------------------------------
// General classes
//----------------------------------------------------------------------------------
class Windows{
public:
    GuiFileDialogState fileDialogStateMenuOpen;

    Windows(){
        fileDialogStateMenuOpen = InitGuiFileDialog(OPEN_WINDOW_WIDTH, OPEN_WINDOW_HEIGHT, GetWorkingDirectory(), false);
    }
};


//----------------------------------------------------------------------------------
// General Functions Declaration
//----------------------------------------------------------------------------------
void printError(int error, GuiVisualization &visualization);

//----------------------------------------------------------------------------------
// Draw Functions Declaration
//----------------------------------------------------------------------------------
void drawGuiMenu(Gui &gui, Windows &windows);
void drawGuiPrior(Gui &gui, Data &data);
void drawGuiChannel(Gui &gui, Data &data);
void drawGuiPosteriors(GuiPosteriors &posteriors);
void drawGuiVisualization(Gui &gui, Data &data);
void drawCircles(Gui &gui, Data &data);

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
void buttonOpen(Windows &windows);
void buttonSave();
void buttonExamples();
void buttonHelp();
void buttonAbout();
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
    Windows windows;

    //----------------------------------------------------------------------------------
    Gui gui = Gui();
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    Data data = Data();
    //--------------------------------------------------------------------------------------

    GuiLoadStyle("src/gui/style-qif-graphics.rgs");
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 0);
    GuiSetStyle(TEXTBOX, TEXT_INNER_PADDING, -4);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    
    // Main game loop
    while(!WindowShouldClose()){    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        Vector2 mousePosition = GetMousePosition();
        
        // Check if channel spinner value was changed
        if(gui.channel.SpinnerChannelValue != gui.channel.numOutputs){
            gui.drawing = false;
            gui.channel.updateChannelBySpinner();
        }

        // Check if a TextBox is being pressed
        if(gui.checkTextBoxPressed()){
            gui.drawing = false;
            data.error = false;
            printError(data.error, gui.visualization);

            if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || 
				IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
				gui.moveAmongTextBoxes();
			}
        }

        // Check if prior circle was moved
        if(gui.drawing){
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
                euclidianDistance(data.priorCircle.center, mousePosition) <= PRIOR_RADIUS){
                data.mouseClickedOnPrior = true;
            }

            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) data.mouseClickedOnPrior = false;
            
            if(data.mouseClickedOnPrior){
                data.updateHyper(gui.visualization.trianglePoints);
                data.buildCircles(gui.visualization.trianglePoints);
                gui.updatePrior(data.hyper.prior, data.priorCircle);
                gui.updatePosteriors(data.hyper, data.innersCircles);
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
            drawGuiPrior(gui, data);
            drawGuiChannel(gui, data);
            drawGuiPosteriors(gui.posteriors);
            drawGuiVisualization(gui, data);
            drawGuiMenu(gui, windows);
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
// General Functions Definitions (local)
//------------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------------
// Draw Functions Definitions (local)
//------------------------------------------------------------------------------------
void drawGuiMenu(Gui &gui, Windows &windows){
    if (GuiButton(gui.menu.layoutRecsButtons[REC_BUTTON_OPEN], gui.menu.buttonOpenText)) buttonOpen(windows);
    if (GuiButton(gui.menu.layoutRecsButtons[REC_BUTTON_SAVE], gui.menu.buttonSaveText)) buttonSave(); 
    if (GuiButton(gui.menu.layoutRecsButtons[REC_BUTTON_EXAMPLES], gui.menu.buttonExamplesText)) buttonExamples(); 
    if (GuiButton(gui.menu.layoutRecsButtons[REC_BUTTON_HELP], gui.menu.buttonHelpText)) buttonHelp(); 
    if (GuiButton(gui.menu.layoutRecsButtons[REC_BUTTON_ABOUT], gui.menu.buttonAboutText)) buttonAbout(); 
    GuiLine(gui.menu.layoutRecsLine, NULL);
    
    if (windows.fileDialogStateMenuOpen.SelectFilePressed){
        // Load qif graphicss file
        if (IsFileExtension(windows.fileDialogStateMenuOpen.fileNameText, ".qifg")){
            strcpy(gui.menu.fileNameToLoad, TextFormat("%s/%s", windows.fileDialogStateMenuOpen.dirPathText, windows.fileDialogStateMenuOpen.fileNameText));
        }

        windows.fileDialogStateMenuOpen.SelectFilePressed = false;
    }
    GuiFileDialog(&(windows.fileDialogStateMenuOpen));
}

void drawGuiPrior(Gui &gui, Data &data){
    GuiGroupBox(gui.prior.layoutRecsGroupBox, gui.prior.GroupBoxPriorText);
    if (GuiButton(gui.prior.layoutRecsButtonRandom, gui.prior.buttonRandomText)) buttonRandomPrior(gui, data); 
    for(int i = 0; i < 3; i++){
        GuiLabel(gui.prior.layoutRecsLabel[i], gui.prior.LabelPriorText[i].c_str());
        if (GuiTextBox(gui.prior.layoutRecsTextBox[i], gui.prior.TextBoxPriorText[i], 128, gui.prior.TextBoxPriorEditMode[i])) gui.prior.TextBoxPriorEditMode[i] = !gui.prior.TextBoxPriorEditMode[i];
    }
}

void drawGuiChannel(Gui &gui, Data &data){
    GuiGroupBox(gui.channel.layoutRecsGroupBox, gui.channel.GroupBoxChannelText);
    if (GuiButton(gui.channel.layoutRecsButtonRandom, gui.channel.buttonRandomText)) buttonRandomChannel(gui, data); 

    if (GuiSpinner(gui.channel.layoutRecsSpinner, "", &(gui.channel.SpinnerChannelValue), 0, 100, gui.channel.SpinnerChannelEditMode)) gui.channel.SpinnerChannelEditMode = !gui.channel.SpinnerChannelEditMode;
    Rectangle viewScrollChannel = GuiScrollPanel(
        (Rectangle){gui.channel.layoutRecsScrollPanel.x, gui.channel.layoutRecsScrollPanel.y, gui.channel.layoutRecsScrollPanel.width - gui.channel.ScrollPanelChannelBoundsOffset.x, gui.channel.layoutRecsScrollPanel.height - gui.channel.ScrollPanelChannelBoundsOffset.y },
        (Rectangle){gui.channel.layoutRecsScrollPanel.x, gui.channel.layoutRecsScrollPanel.y, gui.channel.ScrollPanelChannelContent.x, gui.channel.ScrollPanelChannelContent.y},
        &(gui.channel.ScrollPanelChannelScrollOffset)
    );
    BeginScissorMode(viewScrollChannel.x, viewScrollChannel.y, viewScrollChannel.width, viewScrollChannel.height);
        GuiLabel((Rectangle){gui.channel.layoutRecsLabelOutputs.x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelOutputs.y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelOutputs.width, gui.channel.layoutRecsLabelOutputs.height}, gui.channel.LabelOutputsText);
        for(int i = 0; i < gui.channel.numOutputs; i++){
            GuiLabel((Rectangle){gui.channel.layoutRecsLabelY[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelY[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelY[i].width, gui.channel.layoutRecsLabelY[i].height}, gui.channel.LabelChannelYText[i].c_str());
            for(int j = 0; j < 3; j++){
                if (GuiTextBox((Rectangle){gui.channel.layoutRecsTextBoxChannel[i][j].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsTextBoxChannel[i][j].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsTextBoxChannel[i][j].width, gui.channel.layoutRecsTextBoxChannel[i][j].height}, gui.channel.TextBoxChannelText[i][j], 128, gui.channel.TextBoxChannelEditMode[i][j]))gui.channel.TextBoxChannelEditMode[i][j] = !gui.channel.TextBoxChannelEditMode[i][j];
            }
        }

        for(int i = 0; i < 3; i++){
            GuiLabel((Rectangle){gui.channel.layoutRecsLabelX[i].x + gui.channel.ScrollPanelChannelScrollOffset.x, gui.channel.layoutRecsLabelX[i].y + gui.channel.ScrollPanelChannelScrollOffset.y, gui.channel.layoutRecsLabelX[i].width, gui.channel.layoutRecsLabelX[i].height}, gui.channel.LabelChannelXText[i].c_str());
        }
    EndScissorMode();
}

void drawGuiPosteriors(GuiPosteriors &posteriors){
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

void drawGuiVisualization(Gui &gui, Data &data){
    GuiGroupBox(gui.visualization.layoutRecsGroupBoxVisualization, gui.visualization.GroupBoxVisualizationText);
    if (GuiButton(gui.visualization.layoutRecsButtonDraw, gui.visualization.ButtonDrawText)) buttonDraw(gui, data);
    
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 4);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if(data.error != NO_ERROR) DrawRectangleRec(gui.visualization.layoutRecsTextBoxStatus, (Color){DARKBLUE.r, DARKBLUE.g, DARKBLUE.b, 60});
    GuiTextBox(gui.visualization.layoutRecsTextBoxStatus, gui.visualization.TextBoxStatusText, 128, gui.visualization.TextBoxStatusEditMode);
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
    DrawCircle(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, (Color){0, 102, 204, 210});
    DrawCircleLines(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, (Color){0, 102, 204, 240});
	DrawTextEx(gui.visualization.alternativeFont, gui.visualization.LabelPriorCircleText , (Vector2) {gui.visualization.layoutRecsLabelPriorCircle.x, gui.visualization.layoutRecsLabelPriorCircle.y}, gui.visualization.alternativeFont.baseSize, 1.0, BLACK);

	// Inners
	for(long unsigned int i = 0; i < data.innersCircles.size(); i++){
        DrawCircle(data.innersCircles[i].center.x, data.innersCircles[i].center.y, data.innersCircles[i].radius, (Color){40, 164, 40, 210});
        DrawCircleLines(data.innersCircles[i].center.x, data.innersCircles[i].center.y, data.innersCircles[i].radius, (Color){40, 164, 40, 240});
        DrawTextEx(gui.visualization.alternativeFont, &(gui.posteriors.LabelPosteriorsText[i][0]), (Vector2) {gui.visualization.layoutRecsLabelInnersCircles[i].x, gui.visualization.layoutRecsLabelInnersCircles[i].y}, 26, 1.0, BLACK);
	}
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
void buttonOpen(Windows &windows){
    windows.fileDialogStateMenuOpen.fileDialogActive = true;
}

void buttonSave(){
    // TODO: Implement control logic
}

void buttonExamples(){
    // TODO: Implement control logic
}

void buttonHelp(){
    // TODO: Implement control logic
}

void buttonAbout(){
    // TODO: Implement control logic
}

void buttonRandomPrior(Gui &gui, Data &data){
    if(data.error == INVALID_PRIOR){
        data.error = NO_ERROR;
        strcpy(gui.visualization.TextBoxStatusText, "Status");
    }
    data.newRandomPrior();
    gui.drawing = false;
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
