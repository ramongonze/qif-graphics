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
#include "layout.h"
#include "information.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include "/home/ramon/raygui/src/raygui.h"

// #include <emscripten/emscripten.h>
#include "../qif/qif.h"

#include <iostream>

typedef struct LoopVariables{
	Information I;
	Layout L;
}LoopVariables;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void updateDrawFrame(void* V_);     // Update and Draw one frame. Required to run on a browser.
void printError(int error, Layout &L); // Changes status bar's text.
void drawCircles(Information &I, Layout &L); // Draw prior and inner circles.

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(){
	// Initialization
	//---------------------------------------------------------------------------------------
	int screenWidth = WINDOWS_WIDTH;
	int screenHeight = WINDOWS_HEIGHT;


	InitWindow(screenWidth, screenHeight, "QIF-graphics");

	LoopVariables V;
	V.I = Information();
	SetCameraMode(V.I.camera3d, CAMERA_FREE); // Set a free camera mode

	V.L = Layout();
	V.L.init();
	V.L.alternativeFont = LoadFont("fonts/dejavu.fnt"); // Used to get pi symbol
	
	// emscripten_set_main_loop_arg(updateDrawFrame, &V, 0, 1);

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

	UnloadFont(V.L.alternativeFont);

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
	Vector2 mousePosition;
	int numPost;

	// Update
	//----------------------------------------------------------------------------------
		if(L->SpinnerChannelValue != L->recTextBoxChannel.size()){ // If true, spinnerChannel has been changed
			L->CheckBoxDrawingChecked = false;
			I->hyperReady = false;
			L->updateChannelBySpinner();
		}

		if(L->SpinnerGainValue != L->recTextBoxGain.size()){ // If true, spinnerChannel has been changed
			L->CheckBoxDrawingChecked = false;
			I->hyperReady = false;
			L->updateGainBySpinner();
		}

		// Buttons
		if(L->ButtonPriorClicked){
			L->ButtonPriorClicked = false;
			L->CheckBoxDrawingChecked = false;
			I->hyperReady = false;
			I->newRandomPrior();
			Distribution newPrior(I->prior);
			L->updatePrior(newPrior, I->priorCircle);
		}

		if(L->ButtonChannelClicked){
			L->ButtonChannelClicked = false;
			L->CheckBoxDrawingChecked = false;
			I->hyperReady = false;
			I->newRandomChannel(L->TextBoxChannelText.size());
			L->updateChannelTextBoxes(I->channel);
		}

		// Check if a TextBox is being pressed.
		int textBoxPressed = L->checkTextBoxPressed();
		if(textBoxPressed){
			L->CheckBoxDrawingChecked = false;
			I->hyperReady = false;
			printError(NO_ERROR, *L);

			if(IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || 
				IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
				L->moveAmongTextBoxes(textBoxPressed);
			}
		}

		if(L->CheckBoxDrawingChecked){
			if(I->hyperReady){
				mousePosition = GetMousePosition();
				if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
					euclidianDistance(I->priorCircle.center, mousePosition) <= PRIOR_RADIUS){
					I->mouseClickedOnPrior = true;
				}

				if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) I->mouseClickedOnPrior = false;
				
				if(I->mouseClickedOnPrior){
					I->updateHyper(L->TrianglePoints);
					I->buildCircles(L->TrianglePoints);
					L->updatePrior(I->hyper.prior, I->priorCircle);
					L->updatePosteriors(I->hyper, I->innersCircles, true);
				}
			}else{
				// Check if no invalid character has been typed
				if(I->checkPriorText(L->TextBoxPriorText) != NO_ERROR || I->checkChannelText(L->TextBoxChannelText) != NO_ERROR){
					error = INVALID_VALUE;
					L->CheckBoxDrawingChecked = false;
				}else{
					// Check if typed numbers represent distributions
					if(Distribution::isDistribution(I->prior) == false) error = INVALID_PRIOR;
					else if(Channel::isChannel(I->channel) == false) error = INVALID_CHANNEL;

					if(error == NO_ERROR){
						Distribution newPrior(I->prior);
						Channel newChannel(newPrior, I->channel);
						I->hyper = Hyper(newChannel);

						I->hyperReady = true;
						I->buildCircles(L->TrianglePoints);
						L->updatePrior(I->hyper.prior, I->priorCircle);
						L->updatePosteriors(I->hyper, I->innersCircles, false);
					}else{
						L->CheckBoxDrawingChecked = false;
					}
					
				}
				
				printError(error, *L);
			}
		}

		// I'm not using L->TextBoxOuterText.size() directly because the number of inners can decrease
		// when user moves the prior distribution, so we might not draw all the TextBoxes.
		if(I->hyperReady) numPost = I->hyper.num_post; 
		else numPost = L->TextBoxOuterText.size();

		UpdateCamera(&(I->camera3d));          // Update camera
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
				if (GuiSpinner(L->recSpinnerChannel, "", &L->SpinnerChannelValue, 1, 7, L->SpinnerChannelEditMode)) L->SpinnerChannelEditMode = !L->SpinnerChannelEditMode;
			//----------------------------------------------------------------------------------

			// Labels
			//----------------------------------------------------------------------------------
				GuiLabel(L->recLabelTitle, L->LabelTitleText);
				GuiLabel(L->recLabelOutputs, L->LabelOutputsText);
				GuiLabel(L->recLabelClickDraw, L->LabelClickDrawText);

				// Prior
				for(int i = 0; i < L->LabelPriorText.size(); i++) GuiLabel(L->recLabelPrior[i], &(L->LabelPriorText[i][0]));

				// Channel
				for(int i = 0; i < L->LabelChannelXText.size(); i++) GuiLabel(L->recLabelChannelX[i], &(L->LabelChannelXText[i][0]));
				for(int i = 0; i < L->LabelChannelYText.size(); i++) GuiLabel(L->recLabelChannelY[i], &(L->LabelChannelYText[i][0]));

				// Outer
				GuiLabel(L->recLabelOuterName, L->LabelOuterNameText);
				for(int i = 0; i < numPost; i++) GuiLabel(L->recLabelOuter[i], &(L->LabelOuterText[i][0]));

				// Inners
				for(int i = 0; i < L->recLabelInners.size(); i++) GuiLabel(L->recLabelInners[i], &(L->LabelInnerText[i][0]));
			//----------------------------------------------------------------------------------

			// Buttons
			//--------------------------------------------------------------------------------------
				if(GuiButton(L->recButtonPrior, L->ButtonPriorText)) L->ButtonPriorClicked = true;
				if(GuiButton(L->recButtonChannel, L->ButtonChannelText)) L->ButtonChannelClicked = true;
			//--------------------------------------------------------------------------------------

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
				for(int i = 0; i < numPost; i++){
					if (GuiTextBox(L->recTextBoxOuter[i], L->TextBoxOuterText[i], 128, L->TextBoxOuterEditMode[i])) L->TextBoxOuterEditMode[i] = !L->TextBoxOuterEditMode[i];        
				}

				// Inners
				for(int i = 0; i < numPost; i++){
					for(int j = 0; j < L->TextBoxInnersText[i].size(); j++){
						if (GuiTextBox(L->recTextBoxInners[i][j], L->TextBoxInnersText[i][j], 128, L->TextBoxInnersEditMode[i][j])) L->TextBoxInnersEditMode[i][j] = !L->TextBoxInnersEditMode[i][j];        
					}
				}
				GuiUnlock();

			//----------------------------------------------------------------------------------
			// DropDowns
			//----------------------------------------------------------------------------------
				// if (GuiDropdownBox(L->recDropDownFile, L->DropDownBoxFileText, &L->DropDownBoxFileActive, L->DropDownBoxFileEditMode)) L->DropDownBoxFileEditMode = !L->DropDownBoxFileEditMode;
				// if (GuiDropdownBox(L->recDropDownLoad, L->DropDownLoadText, &L->DropDownLoadActive, L->DropDownLoadEditMode)) L->DropDownLoadEditMode = !L->DropDownLoadEditMode;
				// if (GuiDropdownBox(L->recDropDownExport, L->DropDownExportText, &L->DropDownExportActive, L->DropDownExportEditMode)) L->DropDownExportEditMode = !L->DropDownExportEditMode;
			//----------------------------------------------------------------------------------
			
			// Visualization
			//----------------------------------------------------------------------------------
				if(L->CheckBoxDrawingChecked){
					// Triangle
					//----------------------------------------------------------------------------------
						DrawTriangleLines(L->TrianglePoints[0], L->TrianglePoints[1], L->TrianglePoints[2], BLACK);
					//----------------------------------------------------------------------------------

					// Labels
					//----------------------------------------------------------------------------------
						// Triangle
						for(int i = 0; i < L->LabelTriangleText.size(); i++) GuiLabel(L->recLabelTriangle[i], &(L->LabelTriangleText[i][0]));
					//----------------------------------------------------------------------------------

					// Circles
					drawCircles(*I, *L);
				}

				// 3D
				//----------------------------------------------------------------------------------
					// if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
					// 	I->ray = GetMouseRay(GetMousePosition(), I->camera3d);
					// }

					// Temporary
					DrawLine(L->recGroupBoxVisualization.x, L->recGroupBoxVisualization.y + L->recGroupBoxVisualization.height/2.0f, 
							 L->recGroupBoxVisualization.x + L->recGroupBoxVisualization.width, L->recGroupBoxVisualization.y + L->recGroupBoxVisualization.height/2.0f, RED);
					DrawLine(L->recGroupBoxVisualization.x + L->recGroupBoxVisualization.width/2.0f, L->recGroupBoxVisualization.y, 
							 L->recGroupBoxVisualization.x + L->recGroupBoxVisualization.width/2.0f, L->recGroupBoxVisualization.y + L->recGroupBoxVisualization.height, RED);
					//  Vector2 v = GetWorldToScreen(I->ray.direction, I->camera3d);
					//  printf("Direction 3D: %.2f, %.2f, %.2f\n", I->ray.direction.x, I->ray.direction.y, I->ray.direction.z);
					//  v = GetWorldToScreen(I->ray.position, I->camera3d);
					//  printf("Position 3D: %.2f, %.2f, %.2f\n\n", I->ray.position.x, I->ray.position.y, I->ray.position.z);

					I->camera3d.target = (Vector3){0.0f, 0.0f, 0.0f};
					BeginMode3D(I->camera3d);
						// Draw3Grid(20, 0.7f, (Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.75f, 0.75f, 0.75f});
						Draw3Grid(20, 0.7f, (Vector3){ORIGIN_3D_X, 
													  ORIGIN_3D_Y, 
													  ORIGIN_3D_Z},
													  (Vector3){0.75f, 0.75f, 0.75f});
					EndMode3D();
				//----------------------------------------------------------------------------------
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

void drawCircles(Information &I, Layout &L){
	// Prior
	DrawCircleGradient(I.priorCircle.center.x, I.priorCircle.center.y, I.priorCircle.radius, (Color){128, 191, 255, 190}, (Color){0, 102, 204, 190});
	
	DrawTextEx(L.alternativeFont, L.LabelPriorCircleText, (Vector2) { L.recLabelPriorCircle.x, L.recLabelPriorCircle.y }, L.alternativeFont.baseSize, 1.0, BLACK);

	// Inners
	for(int i = 0; i < I.innersCircles.size(); i++){
		DrawCircleGradient(I.innersCircles[i].center.x, I.innersCircles[i].center.y, I.innersCircles[i].radius, (Color){153, 230, 153, 190}, (Color){40, 164, 40, 190});
		GuiLabel(L.recLabelInnersCircles[i], &(L.LabelOuterText[i][0]));
	}
}
