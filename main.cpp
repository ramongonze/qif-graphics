#include "raylib.h"
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_TEXTBOX_EXTENDED
#include "raygui/src/raygui.h"

#include "qif/qif.hpp"
#include "include/graphics.hpp"

#define MAX_OUTPUTS 20    // Max number of channel outputs
#define MAX_ACTIONS 20    // Max number of actions a gain function can keep
#define MAX_INPUT_BOX 100 // Max number of characters in an input box
#define BOX_WIDTH 50      // Matrices input text width
#define BOX_HEIGHT 30     // Matrices input text height

using namespace std;

/*
 * Left menu:
 * 
 * Remaining percentage: 1%
 *
 *      ----------------------------------------------------------------            _
 *      | Menu bar : 4%                                                |            _| 4%
 * [H1] | ------------------------|------------------------------------| _          _
 *      | Prior distribution: 15% |    INNNERS      |           |      | _| 28%      |
 *      |  |                      |------------------------------------|- - -[TH1]   |
 * [H2] | ------------------------|     |           |           |      | _| 8%       |
 *      | Channel: 40%            |                / \  - - - - - - - -|-_- -[TH2]   |
 *      |                         |     |         / | \         |      |  |          |
 *      |  |                      |              /     \               |  |          | 
 *      |                         |     |       /   |   \       |      |  |          | 96%
 *      |  |                      |            /         \             |  | 50%      |
 * [H3] | ------------------------| [H3]|     /     |     \     |      |  |          |
 *      | Gain Function: 40%      |          /             \           | _|          |
 *      |                         |     |   /_______|_______\- -|- - - | _   [TH3]   |
 *      |  |                      |                                    |  |          |
 *      |                         |     |           |           |      |  | 10%      |
 *      |__|______________________|[7,5%][_________50%__________][7,5%]| _|         _|
 *        [V2]                  [V1]   [TV1]      [TV2]       [TV3]
 *      |_________________________|____________________________________|
 *                   35%                             65%
 * 
 * OBS: The percentages in the interface are realted to the screen width and height.
 *
 */

// Global variables
//--------------------------------------------------------------------------------------
int windowWidth  = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;
//--------------------------------------------------------------------------------------

void updateHyper(Hyper &hyper, Point &priorPosition, vector<Point> &posteriorsPosition){
    // Update the hyper distribution if the user moves the prior distribution
    Point p, mousePosition;
    vector<long double> new_prior(3);

    p = dist2Bary(*(hyper.prior));
    priorPosition = bary2Pixel(p.x, p.y, windowWidth, windowHeight);   
    Vector2 aux = GetMousePosition();
    mousePosition.x = aux.x; mousePosition.y = aux.y;
    // Check if the user is moving the prior
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && euclidianDistance(priorPosition, mousePosition) <= PRIOR_RADIUS){
        mousePosition = pixel2Bary(mousePosition.x, mousePosition.y, windowWidth, windowHeight);
        if(bary2Dist(mousePosition, hyper.prior->prob)){
            
            // Rounds to a point distribution if one of the probabilities is >= 0.98
            if(hyper.prior->prob[0] >= 0.98) hyper.prior->prob = {1, 0, 0};
            else if(hyper.prior->prob[1] >= 0.98) hyper.prior->prob = {0, 1, 0};
            else if(hyper.prior->prob[2] >= 0.98) hyper.prior->prob = {0, 0, 1};

            hyper.rebuildHyper(*(hyper.prior));
            p = dist2Bary(*(hyper.prior));
            priorPosition = bary2Pixel(p.x, p.y, windowWidth, windowHeight);
        }
    }
    
    // Calculates the pixel coordinate for posterior distributions
    posteriorsPosition.resize(hyper.num_post);
    for(int i = 0; i < hyper.num_post; i++){
        p = dist2Bary(hyper.inners[0][i], hyper.inners[1][i], hyper.inners[2][i]);
        posteriorsPosition[i] = bary2Pixel(p.x, p.y, windowWidth, windowHeight);
    }
}

void updateInterface(){
      int i;
}

void drawQIFCircles(Hyper &hyper){
    int i;
}


int main(){
    // Initialization
    //--------------------------------------------------------------------------------------
    char buffer[5];
    float headerFontSize = 20;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    // Gui style
    // GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0x000000ff);

    // Colors
    Color priorColor           = {128, 191, 255, 230};  // Prior circle color
    Color priorBorderColor     = { 51, 153, 255, 230};  // Prior circle border color
    Color posteriorColor       = {255, 222,  78, 230};  // Posterior circle color
    Color posteriorBorderColor = {230, 187,   0, 230};  // Posterior circle border color 
    Color recColor             = {217, 230, 242, 255};  // Rectangles color
    Color recBorderColor       = { 13,  26,  38, 255};  // Rectangle border color

    Rectangle drawCheckBoxRec;
    bool drawCircles = false; // Flag that indicates if the circles must be drawn or not. Its value is set by a check box

    // QIF
    //----------------------------------------------------------------------------------
    // Hyper
    Hyper hyper("prior", "channel");
    vector<Point> posteriorsPosition;
    vector<string> distributionTexts; // Posteriors names (y1, ..., yn)
 
    // Prior
    Point priorPosition;
    Rectangle priorBox[3];                               

    // Channel
    int numOutputs = hyper.channel->num_out;
    char channel[3][MAX_OUTPUTS][MAX_INPUT_BOX], prior[3][MAX_INPUT_BOX];
    Rectangle channelSpinner;

    vector<vector<Rectangle> > channelBox = vector<vector<Rectangle> >(3, vector<Rectangle>(hyper.channel->num_out));

    // Gain Function
    // int numActions = 1;
    // char gain[MAX_ACTIONS][3][MAX_INPUT_BOX];
    // Rectangle gainSpinner;
    // vector<vector<Rectangle> > gainBox = vector<vector<Rectangle> >(numActions, vector<Rectangle>(3));
    // for(int i = 0; i < numActions; i++){
    //     gainBox[i][0] = (Rectangle){(int)(V2(windowWidth) + (i*0.06f*windowWidth)), (int)(0.73f*windowHeight), BOX_WIDTH, BOX_HEIGHT};
    //     gainBox[i][1] = (Rectangle){(int)(V2(windowWidth) + (i*0.06f*windowWidth)), (int)(0.78f*windowHeight), BOX_WIDTH, BOX_HEIGHT};
    //     gainBox[i][2] = (Rectangle){(int)(V2(windowWidth) + (i*0.06f*windowWidth)), (int)(0.83f*windowHeight), BOX_WIDTH, BOX_HEIGHT};
    //     strcpy(gain[i][0], "");
    //     strcpy(gain[i][1], "");
    //     strcpy(gain[i][2], "");
    // }

    // Inner Distributions
    char inners[3][MAX_OUTPUTS][MAX_INPUT_BOX];
    vector<vector<Rectangle> > innersBox = vector<vector<Rectangle> >(3, vector<Rectangle>(hyper.num_post));


    //----------------------------------------------------------------------------------

    // Variables
    //----------------------------------------------------------------------------------
    
    // General
    Vector2 mousePosition;

    // Menu
    Rectangle menuDropdownBox;
    Rectangle menuWindow = {windowWidth/2 - 200, windowHeight/2 - 100, 400, 200};
    Rectangle menuWindowHeader = {menuWindow.x, menuWindow.y, 400, 20};
    int menuActiveOption = 0;
    bool menuDropEditMode = false;
    bool exitSelectMenuWindow = true, dragMenuWindow = false;
    Vector2 offset;

    // Text
    Vector2 headerPos[5];
    char posterior[MAX_OUTPUTS]; // Buffer to print y1, ..., yn inside the posterior circles
    
    // Rectangles
    Rectangle menuRec, priorRec, channelRec, gainRec, innerRec;

    // Main Triangle
    Vector2 mainTrianglePos[3]; // Position of main triangle lines
    Vector2 xTextPos[3];        // Position of texts 'X1', 'X2' and 'X3'
    //----------------------------------------------------------------------------------

    InitWindow(windowWidth, windowHeight, "QIF Graphics");

    // Set font
    Font mainFont = LoadFontEx("CaviarDreams.ttf", headerFontSize, 0, 0);
    GenTextureMipmaps(&mainFont.texture);
    SetTextureFilter(mainFont.texture, FILTER_POINT);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    int comboBoxActive = 1;
    updateHyper(hyper, priorPosition, posteriorsPosition);
    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        // Update     
        //----------------------------------------------------------------------------------
        // General
        mousePosition = GetMousePosition();

        // Interface ---------
        windowWidth = GetScreenWidth();
        windowHeight = GetScreenHeight();
        
        // Menu
        menuDropdownBox = {1, 1, 180, H1(windowHeight)-2};

        // QIF -----------
        if(drawCircles) updateHyper(hyper, priorPosition, posteriorsPosition);
            // Prior
            for(int i = 0; i < 3; i++)
                priorBox[i] = (Rectangle){(int)(V2(windowWidth) + 30 + (i * (BOX_WIDTH+20))), (int)(H1(windowHeight) + 60), BOX_WIDTH, BOX_HEIGHT};

            // Channel
            for(int j = 0; j < hyper.channel->num_out; j++){
                channelBox[0][j] = (Rectangle){(int)(V2(windowWidth) + (j*(BOX_HEIGHT + 30))), (int)(H2(windowHeight)+ 90), BOX_WIDTH, BOX_HEIGHT};
                channelBox[1][j] = (Rectangle){(int)(V2(windowWidth) + (j*(BOX_HEIGHT + 30))), (int)(H2(windowHeight)+ 90 + BOX_HEIGHT+10), BOX_WIDTH, BOX_HEIGHT};
                channelBox[2][j] = (Rectangle){(int)(V2(windowWidth) + (j*(BOX_HEIGHT + 30))), (int)(H2(windowHeight)+ 90 + 2*(BOX_HEIGHT+10)), BOX_WIDTH, BOX_HEIGHT};
                sprintf(channel[0][j], "%.3Lf", hyper.channel->matrix[0][j]);
                sprintf(channel[1][j], "%.3Lf", hyper.channel->matrix[1][j]);
                sprintf(channel[2][j], "%.3Lf", hyper.channel->matrix[2][j]);
            }

            // Inners
            for(int j = 0; j < hyper.num_post; j++){
                innersBox[0][j] = (Rectangle){(int)(TV1(windowWidth) + (j*(BOX_HEIGHT + 20))), (int)(H1(windowHeight)+70), BOX_WIDTH, BOX_HEIGHT};
                innersBox[1][j] = (Rectangle){(int)(TV1(windowWidth) + (j*(BOX_HEIGHT + 20))), (int)(H1(windowHeight)+70+BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT};
                innersBox[2][j] = (Rectangle){(int)(TV1(windowWidth) + (j*(BOX_HEIGHT + 20))), (int)(H1(windowHeight)+70+2*BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT};
                sprintf(inners[0][j], "%.3Lf", hyper.inners[0][j]);
                sprintf(inners[1][j], "%.3Lf", hyper.inners[1][j]);
                sprintf(inners[2][j], "%.3Lf", hyper.inners[2][j]);
            }

        // Buttons
        channelSpinner = {V1(windowWidth)/2 + 90, H2(windowHeight) + 10, 70, 20};
        drawCheckBoxRec = {(int)(V2(windowWidth) + (0.06f*windowWidth) + BOX_WIDTH/2), (int)(0.51f*windowHeight), 15, 15};

        // Text
        headerPos[0] = {10, H1(windowHeight) + 10};                 // Prior
        headerPos[1] = {10, H2(windowHeight) + 10};                 // Channel
        headerPos[2] = {V1(windowWidth)/2, H2(windowHeight) + 10};  // Channel outputs
        headerPos[3] = {10, H3(windowHeight) + 10};                 // Gain Function
        headerPos[4] = {V1(windowWidth) + 10, H1(windowHeight) + 10};                 // Inner distributions

        // Rectangles features
        menuRec    = (Rectangle){0, 0, windowWidth, H1(windowHeight)};
        priorRec   = (Rectangle){0, H1(windowHeight), V1(windowWidth), H2(windowHeight) - H1(windowHeight)};
        channelRec = (Rectangle){0, H2(windowHeight), V1(windowWidth), H3(windowHeight) - H2(windowHeight)};
        gainRec    = (Rectangle){0, H3(windowHeight), V1(windowWidth), windowHeight - H3(windowHeight)};
        innerRec   = (Rectangle){V1(windowWidth), H1(windowHeight), windowWidth - V1(windowWidth), TH1(windowHeight)};

        // GUI
        for(int i = 0; i < 3; i++){
            sprintf(prior[i], "%.3Lf", hyper.prior->prob[i]);

            for(int j = 0; j < hyper.channel->num_out; j++){
                sprintf(channel[i][j], "%.3Lf", hyper.channel->matrix[i][j]);    
            }
        }

        // Main Triangle Features
        mainTrianglePos[0] = {TV2(windowWidth), TH2(windowHeight)}; // X1
        mainTrianglePos[1] = {TV1(windowWidth), TH3(windowHeight)}; // X2
        mainTrianglePos[2] = {TV3(windowWidth), TH3(windowHeight)}; // X3
        xTextPos[0] = {TV2(windowWidth) - 0.01f * windowWidth, TH2(windowHeight) - 0.03f * windowHeight}; // X1
        xTextPos[1] = {TV1(windowWidth) - 0.03f * windowWidth, TH3(windowHeight) - 0.01f * windowHeight}; // X2
        xTextPos[2] = {TV3(windowWidth) + 0.01f * windowWidth, TH3(windowHeight) - 0.01f * windowHeight}; // X3

        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        // Rectangles
            DrawRectangleRec(menuRec, recColor);    DrawRectangleLinesEx(menuRec, 1, recBorderColor);
            DrawRectangleRec(priorRec, recColor);   DrawRectangleLinesEx(priorRec, 1, recBorderColor);
            DrawRectangleRec(channelRec, recColor); DrawRectangleLinesEx(channelRec, 1, recBorderColor);
            DrawRectangleRec(gainRec, recColor);    DrawRectangleLinesEx(gainRec, 1, recBorderColor);
            DrawRectangleRec(innerRec, recColor);   DrawRectangleLinesEx(innerRec, 1, recBorderColor);

        // Text
            DrawTextEx(mainFont, "Prior distribution" , headerPos[0], headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "Channel"            , headerPos[1], headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "Outputs: "          , headerPos[2], headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "Gain Function"      , headerPos[3], headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "Inner distributions", headerPos[4], headerFontSize, 1.0, BLACK);

        // GUI
            // Prior
            for(int i = 0; i < 3; i++){
                sprintf(buffer, "X%d", i+1);
                GuiTextBox(priorBox[i], prior[i], MAX_INPUT_BOX, true); // X1
                DrawTextEx(mainFont, buffer, (Vector2){priorBox[i].x + 15, (int)(priorBox[i].y - 0.03 * windowHeight)}, headerFontSize, 1.0, BLACK);
            }

            // Channel
            GuiSpinner(channelSpinner, &numOutputs, 0, MAX_OUTPUTS, true);
            DrawTextEx(mainFont, "X1", (Vector2){channelBox[0][0].x - 30, (int)(channelBox[0][0].y + 5)}, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X2", (Vector2){channelBox[1][0].x - 30, (int)(channelBox[1][0].y + 5)}, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X3", (Vector2){channelBox[2][0].x - 30, (int)(channelBox[2][0].y + 5)}, headerFontSize, 1.0, BLACK);
            for(int i = 0; i < hyper.channel->num_out; i++){
                sprintf(buffer, "Y%d", i+1);
                DrawTextEx(mainFont, buffer, (Vector2){channelBox[0][i].x+15, channelBox[0][i].y-0.03*windowHeight}, headerFontSize, 1.0, BLACK);
                GuiTextBox(channelBox[0][i], channel[0][i], MAX_INPUT_BOX, true);
                GuiTextBox(channelBox[1][i], channel[1][i], MAX_INPUT_BOX, true);
                GuiTextBox(channelBox[2][i], channel[2][i], MAX_INPUT_BOX, true);
            }

            // Inners
            if(drawCircles){
                DrawTextEx(mainFont, "X1", (Vector2){innersBox[0][0].x - 30, (int)(innersBox[0][0].y + 5)}, headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X2", (Vector2){innersBox[1][0].x - 30, (int)(innersBox[1][0].y + 5)}, headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X3", (Vector2){innersBox[2][0].x - 30, (int)(innersBox[2][0].y + 5)}, headerFontSize, 1.0, BLACK);
                for(int i = 0; i < hyper.num_post; i++){
                    sprintf(buffer, "I%d", i+1);
                    DrawTextEx(mainFont, buffer, (Vector2){innersBox[0][i].x+15, innersBox[0][i].y-0.03*windowHeight}, headerFontSize, 1.0, BLACK);
                    GuiTextBox(innersBox[0][i], inners[0][i], MAX_INPUT_BOX, true);
                    GuiTextBox(innersBox[1][i], inners[1][i], MAX_INPUT_BOX, true);
                    GuiTextBox(innersBox[2][i], inners[2][i], MAX_INPUT_BOX, true);
                }
            }

            drawCircles = GuiCheckBox(drawCheckBoxRec, "Draw", drawCircles);
        


        // Main Triangle
            DrawTriangleLines(mainTrianglePos[0], mainTrianglePos[1], mainTrianglePos[2], BLACK); 
            DrawTextEx(mainFont, "X1", xTextPos[0], headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X2", xTextPos[1], headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X3", xTextPos[2], headerFontSize, 1.0, BLACK);
        
            if(drawCircles){
                // Prior distribution
                DrawCircle(priorPosition.x, priorPosition.y, PRIOR_RADIUS, priorColor);
                DrawCircleLines(priorPosition.x, priorPosition.y, PRIOR_RADIUS, priorBorderColor);
                DrawTextEx(mainFont, "prior", Vector2({priorPosition.x-(PRIOR_RADIUS/2.0f), \
                               priorPosition.y - (0.2f * PRIOR_RADIUS)}), headerFontSize, 1.0, BLACK);

                // Posterior distributions
                for(int i = 0; i < hyper.num_post; i++){
                    int radius = (int)sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);
                    DrawCircle(posteriorsPosition[i].x, posteriorsPosition[i].y, radius, posteriorColor);
                    DrawCircleLines(posteriorsPosition[i].x, posteriorsPosition[i].y, radius, posteriorBorderColor);
                    sprintf(posterior, "Y%d", i+1);
                    DrawTextEx(mainFont, posterior, Vector2({posteriorsPosition[i].x-(radius/2.0f), \
                               posteriorsPosition[i].y - (0.4f * radius)}), headerFontSize, 1.0, BLACK);
                }
            }
            
        // Menu
            GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
            if(GuiDropdownBox(menuDropdownBox, "Menu;Select prior;Select channel;Select gain function", &menuActiveOption, menuDropEditMode)) menuDropEditMode = !menuDropEditMode;
            if(menuActiveOption > 0){
                exitSelectMenuWindow = false;
            }

            if(!exitSelectMenuWindow){
                if(!dragMenuWindow && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, menuWindowHeader)){
                    dragMenuWindow = true;
                    offset.x = mousePosition.x - menuWindow.x;
                    offset.y = mousePosition.y - menuWindow.y;
                }

                if(dragMenuWindow){
                    menuWindow.x = mousePosition.x - offset.x;
                    menuWindow.y = mousePosition.y - offset.y;
                    menuWindowHeader.x = menuWindow.x;
                    menuWindowHeader.y = menuWindow.y;
                    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragMenuWindow = false;
                }

                exitSelectMenuWindow = GuiWindowBox(menuWindow, "Select a file");
                if(exitSelectMenuWindow) menuActiveOption = 0;
            }else{
                menuWindow = {windowWidth/2 - 200, windowHeight/2 - 100, 400, 200}; // Reset window position
                menuWindowHeader.x = menuWindow.x;
                menuWindowHeader.y = menuWindow.y;
            }

            ClearBackground({245, 245, 245, 255});
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}