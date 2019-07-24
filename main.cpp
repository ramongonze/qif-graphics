#include "raylib.h"
#include <iostream>
#include <sstream>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_TEXTBOX_EXTENDED
#include "raygui/src/raygui.h"

#include "qif/qif.hpp"
#include "include/graphics.hpp"

// Rectangle array indices
#define PRIOR              0
#define CHANNEL            1
#define GAIN               2
#define INNERS             3
#define DRAW_CHECK_BOX     4
#define MENU               5
#define MENU_DROPDOWN      6
#define MENU_WINDOW        7
#define MENU_WINDOW_HEADER 8
#define CHANNEL_SPINNER    9

#define PROB_PRECISION 3              // Precision of float numbers
#define MIN_WIDTH 960                 // Min window width
#define MIN_HEIGHT 700                // Min window height
#define MAX_OUTPUTS 20                // Max number of channel outputs
#define MAX_ACTIONS 20                // Max number of actions a gain function can keep
#define MAX_BUFFER 100                // Max number of characters in an input box
#define BOX_WIDTH 50                  // Matrices input text width
#define BOX_HEIGHT 30                 // Matrices input text height
#define LABEL_HOR_X_GAP(pos) pos - 30 // Matrices label horizontal gap to boxes (X1, X2, X3)
#define LABEL_HOR_Y_GAP(pos) pos + 5  // Matrices label vertical gap to boxes (X1, X2, X3)
#define LABEL_VER_X_GAP(pos) pos - 30 // Matrices label horizontal gap to boxes (Y1, Y2, ...)
#define LABEL_VER_Y_GAP(pos) pos + 5  // Matrices label vertical gap to boxes (Y1, Y2, ...)
#define BOX_HOR_GAP 30                // Horizontal gap between two boxes in a matrix
#define BOX_VER_GAP 10                // Vertical gap between two boxes in a matrix

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
Vector2 mousePosition;
Hyper hyper;
Vector2 headerPos[5]; // Header text position
Vector2 mainTrianglePos[3]; // Position of main triangle lines
Vector2 xTextPos[3];        // Position of texts 'X1', 'X2' and 'X3'
//--------------------------------------------------------------------------------------

void updateHyper(Point &priorPosition, vector<Point> &posteriorsPosition){
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

void updateMenu(Rectangle (&staticRectangles)[10], int &menuActiveOption, bool &menuDropEditMode, \
    bool &dragMenuWindow, bool &exitSelectMenuWindow, Vector2 &offset){
    
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if(GuiDropdownBox(staticRectangles[MENU_DROPDOWN], "Menu;Select prior;Select channel;Select gain function", &menuActiveOption, menuDropEditMode)) menuDropEditMode = !menuDropEditMode;
    if(menuActiveOption > 0){
        exitSelectMenuWindow = false;
    }

    if(!exitSelectMenuWindow){
        if(!dragMenuWindow && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, staticRectangles[MENU_WINDOW_HEADER])){
            dragMenuWindow = true;
            offset.x = mousePosition.x - staticRectangles[MENU_WINDOW].x;
            offset.y = mousePosition.y - staticRectangles[MENU_WINDOW].y;
        }

        if(dragMenuWindow){
            staticRectangles[MENU_WINDOW].x = mousePosition.x - offset.x;
            staticRectangles[MENU_WINDOW].y = mousePosition.y - offset.y;
            staticRectangles[MENU_WINDOW_HEADER].x = staticRectangles[MENU_WINDOW].x;
            staticRectangles[MENU_WINDOW_HEADER].y = staticRectangles[MENU_WINDOW].y;
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragMenuWindow = false;
        }

        exitSelectMenuWindow = GuiWindowBox(staticRectangles[MENU_WINDOW], "Select a file");
        if(exitSelectMenuWindow) menuActiveOption = 0;
    }else{
        staticRectangles[MENU_WINDOW] = {windowWidth/2 - 200, windowHeight/2 - 100, 400, 200}; // Reset window position
        staticRectangles[MENU_WINDOW_HEADER].x = staticRectangles[MENU_WINDOW].x;
        staticRectangles[MENU_WINDOW_HEADER].y = staticRectangles[MENU_WINDOW].y;
    }
}

void updateStaticObjects(Rectangle (&staticRectangles)[10],  vector<vector<Rectangle>> (&matricesRectangles)[4]){    
    // Headers text positions
    headerPos[0] = {10, H1(windowHeight) + 10};                   // Prior
    headerPos[1] = {10, H2(windowHeight) + 10};                   // Channel
    headerPos[2] = {V1(windowWidth)/2, H2(windowHeight) + 10};    // Channel outputs
    headerPos[3] = {10, H3(windowHeight) + 10};                   // Gain Function
    headerPos[4] = {V1(windowWidth) + 10, H1(windowHeight) + 10}; // Inner distributions

    // Main Triangle Features
    mainTrianglePos[0] = {TRIANGLEV2(windowWidth, windowHeight), TRIANGLEH2(windowWidth, windowHeight)}; // X1
    mainTrianglePos[1] = {TRIANGLEV1(windowWidth, windowHeight), TRIANGLEH3(windowWidth, windowHeight)}; // X2
    mainTrianglePos[2] = {TRIANGLEV3(windowWidth, windowHeight), TRIANGLEH3(windowWidth, windowHeight)}; // X3
    xTextPos[0] = {TRIANGLEV2(windowWidth, windowHeight) - 0.01f * windowWidth, TRIANGLEH2(windowWidth, windowHeight) - 0.03f * windowHeight}; // X1
    xTextPos[1] = {TRIANGLEV1(windowWidth, windowHeight) - 0.03f * windowWidth, TRIANGLEH3(windowWidth, windowHeight) - 0.01f * windowHeight}; // X2
    xTextPos[2] = {TRIANGLEV3(windowWidth, windowHeight) + 0.01f * windowWidth, TRIANGLEH3(windowWidth, windowHeight) - 0.01f * windowHeight}; // X3

    // Interface rectangles
    staticRectangles[MENU]            = (Rectangle){0, 0, windowWidth, H1(windowHeight)};
    staticRectangles[PRIOR]           = (Rectangle){0, H1(windowHeight), V1(windowWidth), H2(windowHeight) - H1(windowHeight)};
    staticRectangles[CHANNEL]         = (Rectangle){0, H2(windowHeight), V1(windowWidth), H3(windowHeight) - H2(windowHeight)};
    staticRectangles[GAIN]            = (Rectangle){0, H3(windowHeight), V1(windowWidth), windowHeight - H3(windowHeight)};
    staticRectangles[INNERS]          = (Rectangle){V1(windowWidth), H1(windowHeight), windowWidth - V1(windowWidth), TH1(windowHeight)};
    staticRectangles[MENU_DROPDOWN]   = (Rectangle){1, 1, 180, H1(windowHeight)-2};
    staticRectangles[DRAW_CHECK_BOX]  = (Rectangle){(int)(V2(windowWidth) + (0.06f*windowWidth) + BOX_WIDTH/2), (int)(0.51f*windowHeight), 15, 15};
    staticRectangles[CHANNEL_SPINNER] = (Rectangle){V1(windowWidth)/2 + 90, H2(windowHeight) + 10, 70, 20};

    // Prior rectangles
    for(int i = 0; i < 3; i++)
        matricesRectangles[PRIOR][0][i] = (Rectangle){(int)(V2(windowWidth) + 30 + (i * (BOX_WIDTH+20))), (int)(H1(windowHeight) + 60), BOX_WIDTH, BOX_HEIGHT};

    // Channel rectangles
    matricesRectangles[CHANNEL] = vector<vector<Rectangle>>(hyper.prior->num_el, vector<Rectangle>(hyper.channel->num_out));
    for(int j = 0; j < hyper.channel->num_out; j++){
        matricesRectangles[CHANNEL][0][j] = (Rectangle){(int)(V2(windowWidth) + (j*(BOX_HEIGHT + BOX_HOR_GAP))), (int)(H2(windowHeight)+ 90), BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][1][j] = (Rectangle){(int)(V2(windowWidth) + (j*(BOX_HEIGHT + BOX_HOR_GAP))), (int)(H2(windowHeight)+ 90 + BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][2][j] = (Rectangle){(int)(V2(windowWidth) + (j*(BOX_HEIGHT + BOX_HOR_GAP))), (int)(H2(windowHeight)+ 90 + 2*(BOX_HEIGHT+BOX_VER_GAP)), BOX_WIDTH, BOX_HEIGHT};
    }

    // Inners rectangles
    matricesRectangles[INNERS] = vector<vector<Rectangle>>(hyper.prior->num_el, vector<Rectangle>(hyper.num_post));
    for(int j = 0; j < hyper.num_post; j++){
        matricesRectangles[INNERS][0][j] = (Rectangle){(int)(TV1(windowWidth) + (j*(BOX_HEIGHT + 20))), (int)(H1(windowHeight)+70), BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[INNERS][1][j] = (Rectangle){(int)(TV1(windowWidth) + (j*(BOX_HEIGHT + 20))), (int)(H1(windowHeight)+70+BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[INNERS][2][j] = (Rectangle){(int)(TV1(windowWidth) + (j*(BOX_HEIGHT + 20))), (int)(H1(windowHeight)+70+2*BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT};
    }
}

void updateMatricesText(vector<vector<string>> (&matricesTexts)[4]){
    ostringstream os;
    os << fixed << setprecision(PROB_PRECISION) << "";

    for(int i = 0; i < 3; i++){
        matricesTexts[PRIOR][0][i] = to_string(hyper.prior->prob[i]);
    }

    // Channel
    matricesTexts[CHANNEL] = vector<vector<string>>(hyper.prior->num_el, vector<string>(hyper.channel->num_out));
    for(int j = 0; j < hyper.channel->num_out; j++){
        matricesTexts[CHANNEL][0][j] = to_string(hyper.channel->matrix[0][j]);
        matricesTexts[CHANNEL][1][j] = to_string(hyper.channel->matrix[1][j]);
        matricesTexts[CHANNEL][2][j] = to_string(hyper.channel->matrix[2][j]);
    }

    // Inners
    matricesTexts[INNERS] = vector<vector<string>>(hyper.prior->num_el, vector<string>(hyper.num_post));
    for(int j = 0; j < hyper.num_post; j++){
        matricesTexts[INNERS][0][j] = to_string(hyper.inners[0][j]);
        matricesTexts[INNERS][1][j] = to_string(hyper.inners[1][j]);
        matricesTexts[INNERS][2][j] = to_string(hyper.inners[2][j]);
    }
}

int main(){
    // Initialization
    //--------------------------------------------------------------------------------------
    char buffer[MAX_BUFFER];
    float headerFontSize = 20;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    bool drawCircles = false; // Flag that indicates if the circles must be drawn or not. Its value is set by a check box

    // Colors
    Color priorColor           = {128, 191, 255, 230};  // Prior circle color
    Color priorBorderColor     = { 51, 153, 255, 230};  // Prior circle border color
    Color posteriorColor       = {255, 222,  78, 230};  // Posterior circle color
    Color posteriorBorderColor = {230, 187,   0, 230};  // Posterior circle border color 
    Color recColor             = {217, 230, 242, 255};  // Rectangles color
    Color recBorderColor       = { 13,  26,  38, 255};  // Rectangle border color

    // Variables
    //----------------------------------------------------------------------------------
    Rectangle staticRectangles[10];
    vector<vector<Rectangle>> matricesRectangles[4];
    matricesRectangles[PRIOR] = vector<vector<Rectangle>>(1, vector<Rectangle>(3)); // The number of secretes is always 3
    
    vector<vector<string>> matricesTexts[4];
    matricesTexts[PRIOR] = vector<vector<string>>(1, vector<string>(3)); // The number of secretes is always 3

    // Hyper
    hyper = Hyper("prior", "channel");
    vector<Point> posteriorsPosition;
    vector<string> distributionTexts; // Posteriors names (y1, ..., yn)
 
    // Prior
    Point priorPosition;
    Rectangle priorBox[3];                               

    // Channel
    int numOutputs = hyper.channel->num_out;
    
    // Menu
    int menuActiveOption = 0;
    bool menuDropEditMode = false;
    bool exitSelectMenuWindow = true, dragMenuWindow = false;
    Vector2 offset;
    //----------------------------------------------------------------------------------

    InitWindow(windowWidth, windowHeight, "QIF Graphics");
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);

    // Set font
    Font mainFont = LoadFontEx("CaviarDreams.ttf", headerFontSize, 0, 0);
    GenTextureMipmaps(&mainFont.texture);
    SetTextureFilter(mainFont.texture, FILTER_POINT);

    SetTargetFPS(60); // Set the program to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int comboBoxActive = 1;
    updateHyper(priorPosition, posteriorsPosition);
    updateMatricesText(matricesTexts);

    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        // Update     
        //----------------------------------------------------------------------------------
        // General
        mousePosition = GetMousePosition();
        windowWidth   = GetScreenWidth();
        windowHeight  = GetScreenHeight();
        
        // Static Rectangles
        updateStaticObjects(staticRectangles, matricesRectangles);

        // QIF -----------
        if(drawCircles){
            updateHyper(priorPosition, posteriorsPosition);
            updateMatricesText(matricesTexts);
        }
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            
        // Static Objects
            // Rectangles
                DrawRectangleRec(staticRectangles[MENU],    recColor); DrawRectangleLinesEx(staticRectangles[MENU],    1, recBorderColor);
                DrawRectangleRec(staticRectangles[PRIOR],   recColor); DrawRectangleLinesEx(staticRectangles[PRIOR],   1, recBorderColor);
                DrawRectangleRec(staticRectangles[CHANNEL], recColor); DrawRectangleLinesEx(staticRectangles[CHANNEL], 1, recBorderColor);
                DrawRectangleRec(staticRectangles[GAIN],    recColor); DrawRectangleLinesEx(staticRectangles[GAIN],    1, recBorderColor);
                DrawRectangleRec(staticRectangles[INNERS],  recColor); DrawRectangleLinesEx(staticRectangles[INNERS],  1, recBorderColor);

            // Text
                DrawTextEx(mainFont, "Prior distribution" , headerPos[0], headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "Channel"            , headerPos[1], headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "Outputs: "          , headerPos[2], headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "Gain Function"      , headerPos[3], headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "Inner distributions", headerPos[4], headerFontSize, 1.0, BLACK);

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
                        sprintf(buffer, "Y%d", i+1);
                        DrawTextEx(mainFont, buffer, Vector2({posteriorsPosition[i].x-(radius/2.0f), \
                                   posteriorsPosition[i].y - (0.4f * radius)}), headerFontSize, 1.0, BLACK);
                    }
            }

        // Matrices -> Prior, Channel, Gain function, Inners
            // Prior
            for(int i = 0; i < hyper.prior->num_el; i++){
                sprintf(buffer, "X%d", i+1);
                GuiTextBox(matricesRectangles[PRIOR][0][i], (char*)matricesTexts[PRIOR][0][i].c_str(), MAX_BUFFER, true); // X1
                DrawTextEx(mainFont, buffer, (Vector2){matricesRectangles[PRIOR][0][i].x + 15, (int)(matricesRectangles[PRIOR][0][i].y - 0.03 * windowHeight)}, headerFontSize, 1.0, BLACK);
            }

            // Channel
            GuiSpinner(staticRectangles[CHANNEL_SPINNER], &numOutputs, 0, MAX_OUTPUTS, true);
            DrawTextEx(mainFont, "X1", (Vector2){LABEL_HOR_X_GAP(matricesRectangles[CHANNEL][0][0].x), (int)(LABEL_HOR_Y_GAP(matricesRectangles[CHANNEL][0][0].y))}, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X2", (Vector2){LABEL_HOR_X_GAP(matricesRectangles[CHANNEL][1][0].x), (int)(LABEL_HOR_Y_GAP(matricesRectangles[CHANNEL][1][0].y))}, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X3", (Vector2){LABEL_HOR_X_GAP(matricesRectangles[CHANNEL][2][0].x), (int)(LABEL_HOR_Y_GAP(matricesRectangles[CHANNEL][2][0].y))}, headerFontSize, 1.0, BLACK);
            for(int i = 0; i < hyper.channel->num_out; i++){
                sprintf(buffer, "Y%d", i+1);
                DrawTextEx(mainFont, buffer, (Vector2){matricesRectangles[CHANNEL][0][i].x+15, matricesRectangles[CHANNEL][0][i].y-0.03*windowHeight}, headerFontSize, 1.0, BLACK);
                GuiTextBox(matricesRectangles[CHANNEL][0][i], (char*)matricesTexts[CHANNEL][0][i].c_str(), MAX_BUFFER, true);
                GuiTextBox(matricesRectangles[CHANNEL][1][i], (char*)matricesTexts[CHANNEL][1][i].c_str(), MAX_BUFFER, true);
                GuiTextBox(matricesRectangles[CHANNEL][2][i], (char*)matricesTexts[CHANNEL][2][i].c_str(), MAX_BUFFER, true);
            }

            // Inners
            if(drawCircles){
                DrawTextEx(mainFont, "X1", (Vector2){matricesRectangles[INNERS][0][0].x - 30, (int)(matricesRectangles[INNERS][0][0].y + 5)}, headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X2", (Vector2){matricesRectangles[INNERS][1][0].x - 30, (int)(matricesRectangles[INNERS][1][0].y + 5)}, headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X3", (Vector2){matricesRectangles[INNERS][2][0].x - 30, (int)(matricesRectangles[INNERS][2][0].y + 5)}, headerFontSize, 1.0, BLACK);
                for(int i = 0; i < hyper.num_post; i++){
                    sprintf(buffer, "I%d", i+1);
                    DrawTextEx(mainFont, buffer, (Vector2){matricesRectangles[INNERS][0][i].x+15, matricesRectangles[INNERS][0][i].y-0.03*windowHeight}, headerFontSize, 1.0, BLACK);
                    GuiTextBox(matricesRectangles[INNERS][0][i], (char*)matricesTexts[INNERS][0][i].c_str(), MAX_BUFFER, true);
                    GuiTextBox(matricesRectangles[INNERS][1][i], (char*)matricesTexts[INNERS][1][i].c_str(), MAX_BUFFER, true);
                    GuiTextBox(matricesRectangles[INNERS][2][i], (char*)matricesTexts[INNERS][2][i].c_str(), MAX_BUFFER, true);
                }
            }

            drawCircles = GuiCheckBox(staticRectangles[DRAW_CHECK_BOX], "Draw", drawCircles);
            
        // Menu
            updateMenu(staticRectangles, menuActiveOption, menuDropEditMode, dragMenuWindow, \
                exitSelectMenuWindow, offset);

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