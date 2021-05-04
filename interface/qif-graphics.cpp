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

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
static void ButtonExamples();
static void ButtonAbout();
static void ButtonDraw();
static void ButtonOpen();
static void ButtonSave();

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 973;
    int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "qif-graphics");

    // qif-graphics: controls initialization
    //----------------------------------------------------------------------------------
    Vector2 AnchorPrior = { 10, 60 };
    Vector2 anchorChannel = { 10, 180 };
    Vector2 AnchorVisualization = { 340, 60 };
    Vector2 AnchorPosterior = { 10, 420 };
    
    int ToggleGroup004Active = 0;
    bool TextBoxChannel00EditMode = false;
    char TextBoxChannel00Text[128] = "0";
    bool TextBoxChannel10EditMode = false;
    char TextBoxChannel10Text[128] = "0";
    bool TextBoxChannel20EditMode = false;
    char TextBoxChannel20Text[128] = "0";
    bool TextBoxChannel01EditMode = false;
    char TextBoxChannel01Text[128] = "0";
    bool TextBoxChannel11EditMode = false;
    char TextBoxChannel11Text[128] = "0";
    bool TextBoxChannel21EditMode = false;
    char TextBoxChannel21Text[128] = "0";
    bool SpinnerChannelEditMode = false;
    int SpinnerChannelValue = 0;
    bool TextBoxChannel02EditMode = false;
    char TextBoxChannel02Text[128] = "0";
    bool TextBoxChannel03EditMode = false;
    char TextBoxChannel03Text[128] = "0";
    bool TextBoxChannel04EditMode = false;
    char TextBoxChannel04Text[128] = "0";
    bool TextBoxChannel05EditMode = false;
    char TextBoxChannel05Text[128] = "0";
    bool TextBoxPrior00EditMode = false;
    char TextBoxPrior00Text[128] = "0";
    bool TextBoxPrior01EditMode = false;
    char TextBoxPrior01Text[128] = "0";
    bool TextBoxPrior02EditMode = false;
    char TextBoxPrior02Text[128] = "0";
    bool TextBoxChannel12EditMode = false;
    char TextBoxChannel12Text[128] = "0";
    bool TextBoxChannel22EditMode = false;
    char TextBoxChannel22Text[128] = "0";
    bool TextBoxChannel13EditMode = false;
    char TextBoxChannel13Text[128] = "0";
    bool TextBoxChannel23EditMode = false;
    char TextBoxChannel23Text[128] = "0";
    bool TextBoxChannel14EditMode = false;
    char TextBoxChannel14Text[128] = "0";
    bool TextBoxChannel24EditMode = false;
    char TextBoxChannel24Text[128] = "0";
    bool TextBoxChannel15EditMode = false;
    char TextBoxChannel15Text[128] = "0";
    bool TextBoxChannel25EditMode = false;
    char TextBoxChannel25Text[128] = "0";
    bool TextBoxOuter00EditMode = false;
    char TextBoxOuter00Text[128] = "0";
    bool TextBoxInner00EditMode = false;
    char TextBoxInner00Text[128] = "0";
    bool TextBoxInner10EditMode = false;
    char TextBoxInner10Text[128] = "0";
    bool TextBoxInner20EditMode = false;
    char TextBoxInner20Text[128] = "0";
    bool TextBoxInner01EditMode = false;
    char TextBoxInner01Text[128] = "0";
    bool TextBoxInner11EditMode = false;
    char TextBoxInner11Text[128] = "0";
    bool TextBoxInner21EditMode = false;
    char TextBoxInner21Text[128] = "0";
    bool TextBoxInner02EditMode = false;
    char TextBoxInner02Text[128] = "0";
    bool TextBoxInner12EditMode = false;
    char TextBoxInner12Text[128] = "0";
    bool TextBoxInner22EditMode = false;
    char TextBoxInner22Text[128] = "0";
    bool TextBoxInner03EditMode = false;
    char TextBoxInner03Text[128] = "0";
    bool TextBoxInner13EditMode = false;
    char TextBoxInner13Text[128] = "0";
    bool TextBoxInner23EditMode = false;
    char TextBoxInner23Text[128] = "0";
    bool TextBoxInner04EditMode = false;
    char TextBoxInner04Text[128] = "0";
    bool TextBoxInner14EditMode = false;
    char TextBoxInner14Text[128] = "0";
    bool TextBoxInner24EditMode = false;
    char TextBoxInner24Text[128] = "0";
    bool TextBoxInner05EditMode = false;
    char TextBoxInner05Text[128] = "0";
    bool TextBoxInner15EditMode = false;
    char TextBoxInner15Text[128] = "0";
    bool TextBoxInner25EditMode = false;
    char TextBoxInner25Text[128] = "0";
    bool TextBoxOuter01EditMode = false;
    char TextBoxOuter01Text[128] = "0";
    bool TextBoxOuter02EditMode = false;
    char TextBoxOuter02Text[128] = "0";
    bool TextBoxOuter03EditMode = false;
    char TextBoxOuter03Text[128] = "0";
    bool TextBoOuter04EditMode = false;
    char TextBoOuter04Text[128] = "0";
    bool TextBoxOuter05EditMode = false;
    char TextBoxOuter05Text[128] = "0";
    bool TextBoxStatusEditMode = false;
    char TextBoxStatusText[128] = "Status";

    Rectangle layoutRecs[82] = {
        (Rectangle){ AnchorPrior.x + 0, AnchorPrior.y + 0, 310, 100 },
        (Rectangle){ anchorChannel.x + 0, anchorChannel.y + 0, 310, 220 },
        (Rectangle){ AnchorVisualization.x + 0, AnchorVisualization.y + 0, 623, 623 },
        (Rectangle){ AnchorPosterior.x + 0, AnchorPosterior.y + 0, 310, 263 },
        (Rectangle){ 563, -33, 41, 25 },
        (Rectangle){ anchorChannel.x + 50, anchorChannel.y + 80, 40, 40 },
        (Rectangle){ anchorChannel.x + 50, anchorChannel.y + 120, 40, 40 },
        (Rectangle){ anchorChannel.x + 50, anchorChannel.y + 160, 40, 40 },
        (Rectangle){ anchorChannel.x + 90, anchorChannel.y + 80, 40, 40 },
        (Rectangle){ anchorChannel.x + 90, anchorChannel.y + 120, 40, 40 },
        (Rectangle){ anchorChannel.x + 90, anchorChannel.y + 160, 40, 40 },
        (Rectangle){ anchorChannel.x + 200, anchorChannel.y + 15, 90, 25 },
        (Rectangle){ anchorChannel.x + 130, anchorChannel.y + 80, 40, 40 },
        (Rectangle){ anchorChannel.x + 170, anchorChannel.y + 80, 40, 40 },
        (Rectangle){ anchorChannel.x + 210, anchorChannel.y + 80, 40, 40 },
        (Rectangle){ anchorChannel.x + 250, anchorChannel.y + 80, 40, 40 },
        (Rectangle){ AnchorPrior.x + 90, AnchorPrior.y + 40, 40, 40 },
        (Rectangle){ AnchorPrior.x + 130, AnchorPrior.y + 40, 40, 40 },
        (Rectangle){ AnchorPrior.x + 170, AnchorPrior.y + 40, 40, 40 },
        (Rectangle){ anchorChannel.x + 130, anchorChannel.y + 120, 40, 40 },
        (Rectangle){ anchorChannel.x + 130, anchorChannel.y + 160, 40, 40 },
        (Rectangle){ anchorChannel.x + 170, anchorChannel.y + 120, 40, 40 },
        (Rectangle){ anchorChannel.x + 170, anchorChannel.y + 160, 40, 40 },
        (Rectangle){ anchorChannel.x + 210, anchorChannel.y + 120, 40, 40 },
        (Rectangle){ anchorChannel.x + 210, anchorChannel.y + 160, 40, 40 },
        (Rectangle){ anchorChannel.x + 250, anchorChannel.y + 120, 40, 40 },
        (Rectangle){ anchorChannel.x + 250, anchorChannel.y + 160, 40, 40 },
        (Rectangle){ anchorChannel.x + 130, anchorChannel.y + 15, 58, 25 },
        (Rectangle){ 0, 40, 973, 10 },
        (Rectangle){ 190, 5, 80, 32 },
        (Rectangle){ 280, 5, 80, 32 },
        (Rectangle){ AnchorPosterior.x + 50, AnchorPosterior.y + 65, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 50, AnchorPosterior.y + 123, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 50, AnchorPosterior.y + 163, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 50, AnchorPosterior.y + 203, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 90, AnchorPosterior.y + 123, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 90, AnchorPosterior.y + 163, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 90, AnchorPosterior.y + 203, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 130, AnchorPosterior.y + 123, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 130, AnchorPosterior.y + 163, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 130, AnchorPosterior.y + 203, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 170, AnchorPosterior.y + 123, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 170, AnchorPosterior.y + 163, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 170, AnchorPosterior.y + 203, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 210, AnchorPosterior.y + 123, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 210, AnchorPosterior.y + 163, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 210, AnchorPosterior.y + 203, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 250, AnchorPosterior.y + 123, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 250, AnchorPosterior.y + 163, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 250, AnchorPosterior.y + 203, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 5, AnchorPosterior.y + 72, 40, 25 },
        (Rectangle){ AnchorPosterior.x + 90, AnchorPosterior.y + 65, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 130, AnchorPosterior.y + 65, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 170, AnchorPosterior.y + 65, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 210, AnchorPosterior.y + 65, 40, 40 },
        (Rectangle){ AnchorPosterior.x + 250, AnchorPosterior.y + 65, 40, 40 },
        (Rectangle){ AnchorVisualization.x + 20, AnchorVisualization.y + 20, 80, 32 },
        (Rectangle){ AnchorVisualization.x + 115, AnchorVisualization.y + 20, 488, 32 },
        (Rectangle){ AnchorVisualization.x + 20, AnchorVisualization.y + 72, 583, 531 },
        (Rectangle){ AnchorPrior.x + 140, AnchorPrior.y + 20, 20, 20 },
        (Rectangle){ AnchorPrior.x + 180, AnchorPrior.y + 20, 20, 20 },
        (Rectangle){ AnchorPrior.x + 100, AnchorPrior.y + 20, 20, 20 },
        (Rectangle){ anchorChannel.x + 60, anchorChannel.y + 60, 20, 20 },
        (Rectangle){ anchorChannel.x + 100, anchorChannel.y + 60, 20, 20 },
        (Rectangle){ anchorChannel.x + 140, anchorChannel.y + 60, 20, 20 },
        (Rectangle){ anchorChannel.x + 180, anchorChannel.y + 60, 20, 20 },
        (Rectangle){ anchorChannel.x + 220, anchorChannel.y + 60, 20, 20 },
        (Rectangle){ anchorChannel.x + 260, anchorChannel.y + 60, 20, 20 },
        (Rectangle){ AnchorPosterior.x + 60, AnchorPosterior.y + 45, 20, 20 },
        (Rectangle){ AnchorPosterior.x + 100, AnchorPosterior.y + 45, 20, 20 },
        (Rectangle){ AnchorPosterior.x + 140, AnchorPosterior.y + 45, 20, 20 },
        (Rectangle){ AnchorPosterior.x + 180, AnchorPosterior.y + 45, 20, 20 },
        (Rectangle){ AnchorPosterior.x + 220, AnchorPosterior.y + 45, 20, 20 },
        (Rectangle){ AnchorPosterior.x + 260, AnchorPosterior.y + 45, 20, 20 },
        (Rectangle){ anchorChannel.x + 25, anchorChannel.y + 80, 20, 40 },
        (Rectangle){ anchorChannel.x + 25, anchorChannel.y + 120, 20, 40 },
        (Rectangle){ anchorChannel.x + 25, anchorChannel.y + 160, 20, 40 },
        (Rectangle){ AnchorPosterior.x + 25, AnchorPosterior.y + 203, 20, 40 },
        (Rectangle){ AnchorPosterior.x + 25, AnchorPosterior.y + 163, 20, 40 },
        (Rectangle){ AnchorPosterior.x + 25, AnchorPosterior.y + 123, 20, 40 },
        (Rectangle){ 10, 5, 80, 32 },
        (Rectangle){ 100, 5, 80, 32 },
    };
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    GuiLoadStyle("style-qif-graphics.rgs");

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            GuiGroupBox(layoutRecs[0], "Prior distribution");
            GuiGroupBox(layoutRecs[1], "Channel");
            GuiGroupBox(layoutRecs[2], "Visualization");
            GuiGroupBox(layoutRecs[3], "Posterior distributions");
            ToggleGroup004Active = GuiToggleGroup(layoutRecs[4], "ONE;TWO;THREE", ToggleGroup004Active);
            if (GuiTextBox(layoutRecs[5], TextBoxChannel00Text, 128, TextBoxChannel00EditMode)) TextBoxChannel00EditMode = !TextBoxChannel00EditMode;
            if (GuiTextBox(layoutRecs[6], TextBoxChannel10Text, 128, TextBoxChannel10EditMode)) TextBoxChannel10EditMode = !TextBoxChannel10EditMode;
            if (GuiTextBox(layoutRecs[7], TextBoxChannel20Text, 128, TextBoxChannel20EditMode)) TextBoxChannel20EditMode = !TextBoxChannel20EditMode;
            if (GuiTextBox(layoutRecs[8], TextBoxChannel01Text, 128, TextBoxChannel01EditMode)) TextBoxChannel01EditMode = !TextBoxChannel01EditMode;
            if (GuiTextBox(layoutRecs[9], TextBoxChannel11Text, 128, TextBoxChannel11EditMode)) TextBoxChannel11EditMode = !TextBoxChannel11EditMode;
            if (GuiTextBox(layoutRecs[10], TextBoxChannel21Text, 128, TextBoxChannel21EditMode)) TextBoxChannel21EditMode = !TextBoxChannel21EditMode;
            if (GuiSpinner(layoutRecs[11], NULL, &SpinnerChannelValue, 0, 100, SpinnerChannelEditMode)) SpinnerChannelEditMode = !SpinnerChannelEditMode;
            if (GuiTextBox(layoutRecs[12], TextBoxChannel02Text, 128, TextBoxChannel02EditMode)) TextBoxChannel02EditMode = !TextBoxChannel02EditMode;
            if (GuiTextBox(layoutRecs[13], TextBoxChannel03Text, 128, TextBoxChannel03EditMode)) TextBoxChannel03EditMode = !TextBoxChannel03EditMode;
            if (GuiTextBox(layoutRecs[14], TextBoxChannel04Text, 128, TextBoxChannel04EditMode)) TextBoxChannel04EditMode = !TextBoxChannel04EditMode;
            if (GuiTextBox(layoutRecs[15], TextBoxChannel05Text, 128, TextBoxChannel05EditMode)) TextBoxChannel05EditMode = !TextBoxChannel05EditMode;
            if (GuiTextBox(layoutRecs[16], TextBoxPrior00Text, 128, TextBoxPrior00EditMode)) TextBoxPrior00EditMode = !TextBoxPrior00EditMode;
            if (GuiTextBox(layoutRecs[17], TextBoxPrior01Text, 128, TextBoxPrior01EditMode)) TextBoxPrior01EditMode = !TextBoxPrior01EditMode;
            if (GuiTextBox(layoutRecs[18], TextBoxPrior02Text, 128, TextBoxPrior02EditMode)) TextBoxPrior02EditMode = !TextBoxPrior02EditMode;
            if (GuiTextBox(layoutRecs[19], TextBoxChannel12Text, 128, TextBoxChannel12EditMode)) TextBoxChannel12EditMode = !TextBoxChannel12EditMode;
            if (GuiTextBox(layoutRecs[20], TextBoxChannel22Text, 128, TextBoxChannel22EditMode)) TextBoxChannel22EditMode = !TextBoxChannel22EditMode;
            if (GuiTextBox(layoutRecs[21], TextBoxChannel13Text, 128, TextBoxChannel13EditMode)) TextBoxChannel13EditMode = !TextBoxChannel13EditMode;
            if (GuiTextBox(layoutRecs[22], TextBoxChannel23Text, 128, TextBoxChannel23EditMode)) TextBoxChannel23EditMode = !TextBoxChannel23EditMode;
            if (GuiTextBox(layoutRecs[23], TextBoxChannel14Text, 128, TextBoxChannel14EditMode)) TextBoxChannel14EditMode = !TextBoxChannel14EditMode;
            if (GuiTextBox(layoutRecs[24], TextBoxChannel24Text, 128, TextBoxChannel24EditMode)) TextBoxChannel24EditMode = !TextBoxChannel24EditMode;
            if (GuiTextBox(layoutRecs[25], TextBoxChannel15Text, 128, TextBoxChannel15EditMode)) TextBoxChannel15EditMode = !TextBoxChannel15EditMode;
            if (GuiTextBox(layoutRecs[26], TextBoxChannel25Text, 128, TextBoxChannel25EditMode)) TextBoxChannel25EditMode = !TextBoxChannel25EditMode;
            GuiLabel(layoutRecs[27], "Outputs");
            GuiLine(layoutRecs[28], NULL);
            if (GuiButton(layoutRecs[29], "Examples")) ButtonExamples(); 
            if (GuiButton(layoutRecs[30], "About")) ButtonAbout(); 
            if (GuiTextBox(layoutRecs[31], TextBoxOuter00Text, 128, TextBoxOuter00EditMode)) TextBoxOuter00EditMode = !TextBoxOuter00EditMode;
            if (GuiTextBox(layoutRecs[32], TextBoxInner00Text, 128, TextBoxInner00EditMode)) TextBoxInner00EditMode = !TextBoxInner00EditMode;
            if (GuiTextBox(layoutRecs[33], TextBoxInner10Text, 128, TextBoxInner10EditMode)) TextBoxInner10EditMode = !TextBoxInner10EditMode;
            if (GuiTextBox(layoutRecs[34], TextBoxInner20Text, 128, TextBoxInner20EditMode)) TextBoxInner20EditMode = !TextBoxInner20EditMode;
            if (GuiTextBox(layoutRecs[35], TextBoxInner01Text, 128, TextBoxInner01EditMode)) TextBoxInner01EditMode = !TextBoxInner01EditMode;
            if (GuiTextBox(layoutRecs[36], TextBoxInner11Text, 128, TextBoxInner11EditMode)) TextBoxInner11EditMode = !TextBoxInner11EditMode;
            if (GuiTextBox(layoutRecs[37], TextBoxInner21Text, 128, TextBoxInner21EditMode)) TextBoxInner21EditMode = !TextBoxInner21EditMode;
            if (GuiTextBox(layoutRecs[38], TextBoxInner02Text, 128, TextBoxInner02EditMode)) TextBoxInner02EditMode = !TextBoxInner02EditMode;
            if (GuiTextBox(layoutRecs[39], TextBoxInner12Text, 128, TextBoxInner12EditMode)) TextBoxInner12EditMode = !TextBoxInner12EditMode;
            if (GuiTextBox(layoutRecs[40], TextBoxInner22Text, 128, TextBoxInner22EditMode)) TextBoxInner22EditMode = !TextBoxInner22EditMode;
            if (GuiTextBox(layoutRecs[41], TextBoxInner03Text, 128, TextBoxInner03EditMode)) TextBoxInner03EditMode = !TextBoxInner03EditMode;
            if (GuiTextBox(layoutRecs[42], TextBoxInner13Text, 128, TextBoxInner13EditMode)) TextBoxInner13EditMode = !TextBoxInner13EditMode;
            if (GuiTextBox(layoutRecs[43], TextBoxInner23Text, 128, TextBoxInner23EditMode)) TextBoxInner23EditMode = !TextBoxInner23EditMode;
            if (GuiTextBox(layoutRecs[44], TextBoxInner04Text, 128, TextBoxInner04EditMode)) TextBoxInner04EditMode = !TextBoxInner04EditMode;
            if (GuiTextBox(layoutRecs[45], TextBoxInner14Text, 128, TextBoxInner14EditMode)) TextBoxInner14EditMode = !TextBoxInner14EditMode;
            if (GuiTextBox(layoutRecs[46], TextBoxInner24Text, 128, TextBoxInner24EditMode)) TextBoxInner24EditMode = !TextBoxInner24EditMode;
            if (GuiTextBox(layoutRecs[47], TextBoxInner05Text, 128, TextBoxInner05EditMode)) TextBoxInner05EditMode = !TextBoxInner05EditMode;
            if (GuiTextBox(layoutRecs[48], TextBoxInner15Text, 128, TextBoxInner15EditMode)) TextBoxInner15EditMode = !TextBoxInner15EditMode;
            if (GuiTextBox(layoutRecs[49], TextBoxInner25Text, 128, TextBoxInner25EditMode)) TextBoxInner25EditMode = !TextBoxInner25EditMode;
            GuiLabel(layoutRecs[50], "Outer");
            if (GuiTextBox(layoutRecs[51], TextBoxOuter01Text, 128, TextBoxOuter01EditMode)) TextBoxOuter01EditMode = !TextBoxOuter01EditMode;
            if (GuiTextBox(layoutRecs[52], TextBoxOuter02Text, 128, TextBoxOuter02EditMode)) TextBoxOuter02EditMode = !TextBoxOuter02EditMode;
            if (GuiTextBox(layoutRecs[53], TextBoxOuter03Text, 128, TextBoxOuter03EditMode)) TextBoxOuter03EditMode = !TextBoxOuter03EditMode;
            if (GuiTextBox(layoutRecs[54], TextBoOuter04Text, 128, TextBoOuter04EditMode)) TextBoOuter04EditMode = !TextBoOuter04EditMode;
            if (GuiTextBox(layoutRecs[55], TextBoxOuter05Text, 128, TextBoxOuter05EditMode)) TextBoxOuter05EditMode = !TextBoxOuter05EditMode;
            if (GuiButton(layoutRecs[56], "Draw")) ButtonDraw(); 
            if (GuiTextBox(layoutRecs[57], TextBoxStatusText, 128, TextBoxStatusEditMode)) TextBoxStatusEditMode = !TextBoxStatusEditMode;
            GuiPanel(layoutRecs[58]);
            GuiLabel(layoutRecs[59], "X2");
            GuiLabel(layoutRecs[60], "X3");
            GuiLabel(layoutRecs[61], "X1");
            GuiLabel(layoutRecs[62], "Y1");
            GuiLabel(layoutRecs[63], "Y2");
            GuiLabel(layoutRecs[64], "Y3");
            GuiLabel(layoutRecs[65], "Y4");
            GuiLabel(layoutRecs[66], "Y5");
            GuiLabel(layoutRecs[67], "Y6");
            GuiLabel(layoutRecs[68], "I1");
            GuiLabel(layoutRecs[69], "I2");
            GuiLabel(layoutRecs[70], "I3");
            GuiLabel(layoutRecs[71], "I4");
            GuiLabel(layoutRecs[72], "I5");
            GuiLabel(layoutRecs[73], "I6");
            GuiLabel(layoutRecs[74], "X1");
            GuiLabel(layoutRecs[75], "X2");
            GuiLabel(layoutRecs[76], "X3");
            GuiLabel(layoutRecs[77], "X3");
            GuiLabel(layoutRecs[78], "X2");
            GuiLabel(layoutRecs[79], "X1");
            if (GuiButton(layoutRecs[80], "Open")) ButtonOpen(); 
            if (GuiButton(layoutRecs[81], "Save")) ButtonSave(); 
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
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
static void ButtonExamples()
{
    // TODO: Implement control logic
}
static void ButtonAbout()
{
    // TODO: Implement control logic
}
static void ButtonDraw()
{
    // TODO: Implement control logic
}
static void ButtonOpen()
{
    // TODO: Implement control logic
}
static void ButtonSave()
{
    // TODO: Implement control logic
}

