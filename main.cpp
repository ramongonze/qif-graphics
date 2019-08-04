#include "raylib.h"
#include <iostream>
#include <sstream>
#include <algorithm>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_TEXTBOX_EXTENDED
#include "raygui/src/raygui.h"

#include "qif/qif.hpp"
#include "include/graphics.hpp"

// Rectangle array indices
#define NUM_STATIC_OBJECTS 11
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
#define OUTER              10

#define PROB_PRECISION 30    // Precision of float numbers (2 char from integer part and '.')
#define MIN_WIDTH 960        // Min window width
#define MIN_HEIGHT 700       // Min window height
#define MAX_OUTPUTS 20       // Max number of channel outputs
#define MAX_ACTIONS 20       // Max number of actions a gain function can keep
#define MAX_BUFFER 100       // Max number of characters in an input box
#define BOX_WIDTH 50         // Matrices input text width
#define BOX_HEIGHT 30        // Matrices input text height
#define LABEL_X_HOR_GAP -30  // Matrices label horizontal gap to boxes (X1, X2, X3)
#define LABEL_X_VER_GAP +5   // Matrices label horizontal gap to boxes (Y1, Y2, ...)
#define LABEL_Y_HOR_GAP +15  // Matrices label vertical gap to boxes (X1, X2, X3)
#define LABEL_Y_VER_GAP -25  // Matrices label vertical gap to boxes (Y1, Y2, ...)
#define BOX_HOR_GAP 10       // Horizontal gap between two boxes in a matrix
#define BOX_VER_GAP 10       // Vertical gap between two boxes in a matrix

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
Vector2 headerPos[5];       // Header text position
Vector2 mainTrianglePos[3]; // Position of main triangle lines
Vector2 triangleTextPos[3]; // Position of texts 'X1', 'X2' and 'X3'
vector<vector<long double>> channel;
vector<long double> prior;
//--------------------------------------------------------------------------------------

void updateHyper(Point &priorPosition, vector<Point> &posteriorsPosition, vector<vector<Rectangle>> (&matricesRectangles)[NUM_STATIC_OBJECTS]){
    /* Update the hyper distribution if the user moves the prior distribution
     * This function assumes that the hyper distribution is already built
     */
    Point p, mousePoint;
    vector<long double> new_prior(3);

    p = dist2Bary(hyper.prior);
    priorPosition = bary2Pixel(p.x, p.y, windowWidth, windowHeight);   
    mousePoint.x = mousePosition.x;
    mousePoint.y = mousePosition.y;
    
    // Check if the user is moving the prior
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && euclidianDistance(priorPosition, mousePoint) <= PRIOR_RADIUS){
        mousePoint = pixel2Bary(mousePoint.x, mousePoint.y, windowWidth, windowHeight);
        if(bary2Dist(mousePoint, hyper.prior.prob)){
            
            // Rounds to a point distribution if one of the probabilities is >= 0.98
            if(hyper.prior.prob[0] >= 0.98) hyper.prior.prob = {1, 0, 0};
            else if(hyper.prior.prob[1] >= 0.98) hyper.prior.prob = {0, 1, 0};
            else if(hyper.prior.prob[2] >= 0.98) hyper.prior.prob = {0, 0, 1};

            hyper.rebuildHyper(hyper.prior);
            p = dist2Bary(hyper.prior);
            priorPosition = bary2Pixel(p.x, p.y, windowWidth, windowHeight);
        }
    }
    
    // Calculates the pixel coordinate for posterior distributions
    posteriorsPosition = vector<Point>(hyper.num_post);
    for(int i = 0; i < hyper.num_post; i++){
        p = dist2Bary(hyper.inners[0][i], hyper.inners[1][i], hyper.inners[2][i]);
        posteriorsPosition[i] = bary2Pixel(p.x, p.y, windowWidth, windowHeight);
    }
    

    // Outer and inners rectangles
    matricesRectangles[OUTER] = vector<vector<Rectangle>>(1, vector<Rectangle>(hyper.num_post));
    matricesRectangles[INNERS] = vector<vector<Rectangle>>(hyper.prior.num_el, vector<Rectangle>(hyper.num_post));
    for(int j = 0; j < hyper.num_post; j++){
        matricesRectangles[OUTER][0][j]  = (Rectangle){TV1(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H1(windowHeight)+ 70                             , BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[INNERS][0][j] = (Rectangle){TV1(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H1(windowHeight)+ 70 + BOX_HEIGHT+BOX_VER_GAP    , BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[INNERS][1][j] = (Rectangle){TV1(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H1(windowHeight)+ 70 + 2*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[INNERS][2][j] = (Rectangle){TV1(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H1(windowHeight)+ 70 + 3*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
    }
}

void updateMenu(Rectangle (&staticRectangles)[NUM_STATIC_OBJECTS], int &menuActiveOption, bool &menuDropEditMode, \
    bool &dragMenuWindow, bool &exitSelectMenuWindow, Vector2 &offset){
    
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if(GuiDropdownBox(staticRectangles[MENU_DROPDOWN], "Menu;Select prior;Select channel;Select gain function", &menuActiveOption, menuDropEditMode)) menuDropEditMode = !menuDropEditMode;
    if(menuActiveOption > 0){
        exitSelectMenuWindow = false;
    }

    if(!exitSelectMenuWindow){
            // cout << "Mouse left button: " << IsMouseButtonPressed(MOUSE_LEFT_BUTTON) << endl;
            // cout << "Collision: " << CheckCollisionPointRec(mousePosition, staticRectangles[MENU_WINDOW_HEADER]) << endl;
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
            if(IsMouseButtonUp(MOUSE_LEFT_BUTTON)) dragMenuWindow = false;
        }

        exitSelectMenuWindow = GuiWindowBox(staticRectangles[MENU_WINDOW], "Select a file");
        if(exitSelectMenuWindow) menuActiveOption = 0;
    }else{
        staticRectangles[MENU_WINDOW] = {windowWidth/2.0f - 200, windowHeight/2.0f - 100, 400.0f, 200.0f}; // Reset window position
        staticRectangles[MENU_WINDOW_HEADER].x = staticRectangles[MENU_WINDOW].x;
        staticRectangles[MENU_WINDOW_HEADER].y = staticRectangles[MENU_WINDOW].y;
        staticRectangles[MENU_WINDOW_HEADER].width = staticRectangles[MENU_WINDOW].width;
        staticRectangles[MENU_WINDOW_HEADER].height = 30;
    }
}

void updateStaticObjects(Rectangle (&staticRectangles)[NUM_STATIC_OBJECTS]){    
    // Headers text positions
    headerPos[0] = {10, H1(windowHeight) + 10};                    // Prior
    headerPos[1] = {10, H2(windowHeight) + 10};                    // Channel
    headerPos[2] = {V1(windowWidth) - 180, H2(windowHeight) + 10}; // Channel outputs
    headerPos[3] = {10, H3(windowHeight) + 10};                    // Gain Function
    headerPos[4] = {V1(windowWidth) + 10, H1(windowHeight) + 10};  // Inner distributions

    // Main Triangle Features
    mainTrianglePos[0] = {TRIANGLEV2(windowWidth, windowHeight), TRIANGLEH2(windowWidth, windowHeight)}; // X1
    mainTrianglePos[1] = {TRIANGLEV1(windowWidth, windowHeight), TRIANGLEH3(windowWidth, windowHeight)}; // X2
    mainTrianglePos[2] = {TRIANGLEV3(windowWidth, windowHeight), TRIANGLEH3(windowWidth, windowHeight)}; // X3
    triangleTextPos[0] = {TRIANGLEV2(windowWidth, windowHeight) - 0.01f * windowWidth, TRIANGLEH2(windowWidth, windowHeight) - 0.03f * windowHeight}; // X1
    triangleTextPos[1] = {TRIANGLEV1(windowWidth, windowHeight) - 0.03f * windowWidth, TRIANGLEH3(windowWidth, windowHeight) - 0.01f * windowHeight}; // X2
    triangleTextPos[2] = {TRIANGLEV3(windowWidth, windowHeight) + 0.01f * windowWidth, TRIANGLEH3(windowWidth, windowHeight) - 0.01f * windowHeight}; // X3

    // Interface rectangles
    staticRectangles[MENU]            = (Rectangle){0.0f, 0.0f, (float)windowWidth, (float)H1(windowHeight)+1};
    staticRectangles[PRIOR]           = (Rectangle){0.0f, (float)H1(windowHeight), (float)V1(windowWidth)+1, (float)(H2(windowHeight) - H1(windowHeight))+1};
    staticRectangles[CHANNEL]         = (Rectangle){0.0f, (float)H2(windowHeight), (float)V1(windowWidth)+1, (float)(H3(windowHeight) - H2(windowHeight))+1};
    staticRectangles[CHANNEL_SPINNER] = (Rectangle){V1(windowWidth) - 90, H2(windowHeight) + 10, 80.0f, 20.0f};
    staticRectangles[GAIN]            = (Rectangle){0.0f, (float)H3(windowHeight), (float)V1(windowWidth)+1, (float)(windowHeight - H3(windowHeight))+1};
    staticRectangles[INNERS]          = (Rectangle){V1(windowWidth), H1(windowHeight), windowWidth - V1(windowWidth), TH1(windowHeight)};
    staticRectangles[MENU_DROPDOWN]   = (Rectangle){1.0f, 1.0f, 180.0f, (float)(H1(windowHeight)-2)};
    staticRectangles[DRAW_CHECK_BOX]  = (Rectangle){V1(windowWidth)/2.0f - 30, H2(windowHeight)+ 120 + 2*(BOX_HEIGHT+2*BOX_VER_GAP), 15.0f, 15.0f};
}

void updateMatricesText(vector<vector<string>> (&matricesTexts)[NUM_STATIC_OBJECTS]){
    /* This fuction assumes that the hyper distribution is already built */

    for(int i = 0; i < 3; i++){
        matricesTexts[PRIOR][0][i] = to_string(hyper.prior.prob[i]);
    }

    // Channel
    matricesTexts[CHANNEL] = vector<vector<string>>(hyper.prior.num_el, vector<string>(hyper.channel.num_out));
    for(int j = 0; j < hyper.channel.num_out; j++){
        matricesTexts[CHANNEL][0][j] = to_string(hyper.channel.matrix[0][j]);
        matricesTexts[CHANNEL][1][j] = to_string(hyper.channel.matrix[1][j]);
        matricesTexts[CHANNEL][2][j] = to_string(hyper.channel.matrix[2][j]);
    }

    // Outer and inners
    matricesTexts[INNERS] = vector<vector<string>>(hyper.prior.num_el, vector<string>(hyper.num_post));
    matricesTexts[OUTER] = vector<vector<string>>(1, vector<string>(hyper.num_post));
    for(int j = 0; j < hyper.num_post; j++){
        matricesTexts[OUTER][0][j]  = to_string(hyper.outer.prob[j]);
        matricesTexts[INNERS][0][j] = to_string(hyper.inners[0][j]);
        matricesTexts[INNERS][1][j] = to_string(hyper.inners[1][j]);
        matricesTexts[INNERS][2][j] = to_string(hyper.inners[2][j]);
    }
}

void initTexts(vector<vector<Rectangle>> (&matricesRectangles)[NUM_STATIC_OBJECTS], vector<vector<string>> (&matricesTexts)[NUM_STATIC_OBJECTS]){
    prior   = vector<long double>({0.0f, 0.0f, 0.0f});
    channel = vector<vector<long double>>({{0.0f, 0.0f, 0.0f}, \
                                           {0.0f, 0.0f, 0.0f}, \
                                           {0.0f, 0.0f, 0.0f}});

    matricesTexts[PRIOR]   = vector<vector<string>>(1, vector<string>(3));
    matricesTexts[CHANNEL] = vector<vector<string>>(3, vector<string>(3));
    for(int i = 0; i < 3; i++){
        matricesTexts[PRIOR][0][i] = "0.000";
        for(int j = 0; j < 3; j++){
            matricesTexts[CHANNEL][i][j] = "0.000";
        }
    }

    matricesRectangles[PRIOR]   = vector<vector<Rectangle>>(1, vector<Rectangle>(3));
    matricesRectangles[CHANNEL] = vector<vector<Rectangle>>(3, vector<Rectangle>(3));
    for(int j = 0; j < 3; j++){
        matricesRectangles[PRIOR][0][j] = (Rectangle){V2(windowWidth) + 30 + (j * (BOX_WIDTH+20)), H1(windowHeight) + 60, BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][0][j] = (Rectangle){V2(windowWidth) + (j*(BOX_HEIGHT + BOX_HOR_GAP)), H2(windowHeight)+ 90, BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][1][j] = (Rectangle){V2(windowWidth) + (j*(BOX_HEIGHT + BOX_HOR_GAP)), H2(windowHeight)+ 90 + BOX_HEIGHT+BOX_VER_GAP, BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][2][j] = (Rectangle){V2(windowWidth) + (j*(BOX_HEIGHT + BOX_HOR_GAP)), H2(windowHeight)+ 90 + 2*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
    }
}

void updateChannel(int oldNumOutputs, int numOutputs, vector<vector<Rectangle>> (&matricesRectangles)[NUM_STATIC_OBJECTS], vector<vector<string>> (&matricesTexts)[NUM_STATIC_OBJECTS], Vector2 channelPanelScroll){
    for(int j = 0; j < oldNumOutputs; j++){
        matricesRectangles[CHANNEL][0][j] = (Rectangle){V2(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)) + channelPanelScroll.x, H2(windowHeight)+ 90                             , BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][1][j] = (Rectangle){V2(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)) + channelPanelScroll.x, H2(windowHeight)+ 90 + (BOX_HEIGHT+BOX_VER_GAP)  , BOX_WIDTH, BOX_HEIGHT};
        matricesRectangles[CHANNEL][2][j] = (Rectangle){V2(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)) + channelPanelScroll.x, H2(windowHeight)+ 90 + 2*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT};
    }

    // Check if the number of outputs was changed
    if(oldNumOutputs < numOutputs){
        for(int j = oldNumOutputs; j < numOutputs; j++){
            matricesTexts[CHANNEL][0].push_back("0.000");
            matricesTexts[CHANNEL][1].push_back("0.000");
            matricesTexts[CHANNEL][2].push_back("0.000");
            matricesRectangles[CHANNEL][0].push_back((Rectangle){channelPanelScroll.x+V2(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H2(windowHeight)+ 90                             , BOX_WIDTH, BOX_HEIGHT});
            matricesRectangles[CHANNEL][1].push_back((Rectangle){channelPanelScroll.x+V2(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H2(windowHeight)+ 90 + BOX_HEIGHT+BOX_VER_GAP    , BOX_WIDTH, BOX_HEIGHT});
            matricesRectangles[CHANNEL][2].push_back((Rectangle){channelPanelScroll.x+V2(windowWidth) + (j*(BOX_WIDTH + BOX_HOR_GAP)), H2(windowHeight)+ 90 + 2*(BOX_HEIGHT+BOX_VER_GAP), BOX_WIDTH, BOX_HEIGHT});
        }
    }else{
        for(int j = oldNumOutputs-1; j >= numOutputs; j--){
            matricesTexts[CHANNEL][0].pop_back();
            matricesTexts[CHANNEL][1].pop_back();
            matricesTexts[CHANNEL][2].pop_back();
            matricesRectangles[CHANNEL][0].pop_back();
            matricesRectangles[CHANNEL][1].pop_back();
            matricesRectangles[CHANNEL][2].pop_back();
        }
    }
}

void updatePrior(vector<vector<Rectangle>> (&matricesRectangles)[NUM_STATIC_OBJECTS]){
    for(int i = 0; i < 3; i++)
        matricesRectangles[PRIOR][0][i] = (Rectangle){V2(windowWidth) + 0.05*windowHeight + (i * (BOX_WIDTH+20)), H1(windowHeight) + 60, BOX_WIDTH, BOX_HEIGHT};
}

int main(){
    // Initialization
    //--------------------------------------------------------------------------------------
    char buffer[MAX_BUFFER];
    char errorBuffer[MAX_BUFFER];
    float headerFontSize = 20;
    bool drawCircles = false; // Flag that indicates if the circles must be drawn or not. Its value is set by a check box
    bool hyperReady = false;  // Flag that indicates if a hyper distribution has been built
    bool error = false;       // Flag that indicates if an error has been occurred

    // Colors
    //--------------------------------------------------------------------------------------
    Color priorColor           = {128, 191, 255, 230};  // Prior circle color
    Color priorBorderColor     = { 51, 153, 255, 230};  // Prior circle border color
    Color posteriorColor       = {255, 222,  78, 230};  // Posterior circle color
    Color posteriorBorderColor = {230, 187,   0, 230};  // Posterior circle border color 
    Color recColor             = {217, 230, 242, 255};  // Rectangles color
    Color recBorderColor       = {  0,   0,   0, 255};  // Rectangle border color

    // Variables
    //----------------------------------------------------------------------------------
    Rectangle staticRectangles[NUM_STATIC_OBJECTS];
    vector<vector<Rectangle>> matricesRectangles[NUM_STATIC_OBJECTS];
    vector<vector<string>> matricesTexts[NUM_STATIC_OBJECTS];

    // Hyper
    vector<Point> posteriorsPosition;
 
    // Prior
    Point priorPosition;

    // Channel
    int numOutputs = 3, oldnumOutputs;
    Vector2 channelPanelScroll = {0.0f, 0.0f};
    Rectangle channelPanelRec, channelPanelContentRec;

    // Menu
    int menuActiveOption = 0;
    bool exitSelectMenuWindow = true, menuDropEditMode = false, dragMenuWindow = false;
    Vector2 offset;
    //----------------------------------------------------------------------------------

    // Window
    //--------------------------------------------------------------------------------------
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); // Problems with scroll panel
    InitWindow(windowWidth, windowHeight, "QIF Graphics");
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);

    // Text font
    Font mainFont = LoadFontEx("School Times.ttf", headerFontSize, 0, 0);
    GenTextureMipmaps(&mainFont.texture);
    SetTextureFilter(mainFont.texture, FILTER_POINT);

    SetTargetFPS(60); // Set the program to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    initTexts(matricesRectangles, matricesTexts);
    int comboBoxActive = 1;


    char **test;
    int count;
    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        test = GetDirectoryFiles(GetWorkingDirectory(), &count);
        for(int i = 0; i < count; i++)
            cout << test[i] << endl;
        cout << "\n---\n";
        // General update
        //----------------------------------------------------------------------------------
        mousePosition = GetMousePosition();
        windowWidth   = GetScreenWidth();
        windowHeight  = GetScreenHeight();

        // Static Rectangles
        updateStaticObjects(staticRectangles);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground({245, 245, 245, 255});

        // Convert
            if(!hyperReady){
                // Check if the user is typing numbers
                try{
                    // Prior
                    for(int i = 0; i < 3; i++){
                        size_t pos = matricesTexts[PRIOR][0][i].find('/');
                        if(pos != string::npos){
                            // The user is typing a fraction
                            string numerator = matricesTexts[PRIOR][0][i].substr(0, pos);
                            string denominator = matricesTexts[PRIOR][0][i].substr(pos+1, matricesTexts[PRIOR][0][i].size()-pos-1);
                            
                            // Remove blank spaces
                            numerator.erase(remove(numerator.begin(), numerator.end(), ' '), numerator.end());
                            denominator.erase(remove(denominator.begin(), denominator.end(), ' '), denominator.end());
                            prior[i] = ((long double)stod(numerator))/stod(denominator);
                        }else{
                            prior[i] = stold(matricesTexts[PRIOR][0][i]);
                        }
                    }

                    // Channel
                    for(int i = 0; i < 3; i++){
                        for(int j = 0; j < numOutputs; j++){
                            size_t pos = matricesTexts[CHANNEL][i][j].find('/');
                            if(pos != string::npos){
                                // The user is typing a fraction
                                string numerator = matricesTexts[CHANNEL][i][j].substr(0, pos);
                                string denominator = matricesTexts[CHANNEL][i][j].substr(pos+1, matricesTexts[CHANNEL][i][j].size()-pos-1);
                                
                                // Remove blank spaces
                                numerator.erase(remove(numerator.begin(), numerator.end(), ' '), numerator.end());
                                denominator.erase(remove(denominator.begin(), denominator.end(), ' '), denominator.end());
                                channel[i][j] = ((long double)stod(numerator))/stod(denominator);
                            }else{
                                channel[i][j] = stold(matricesTexts[CHANNEL][i][j]);
                            }
                        }

                        // If the flow arrives here, there were no error in conversion
                        error = false;
                    }          
                }catch(...){
                    error = true;
                    strcpy(errorBuffer, "Some value in prior or channel is invalid!");
                }
            }else{
                // Update variables prior and channel with hyper values
                for(int i = 0; i < 3; i++){
                    prior[i] = hyper.prior.prob[i];
                    if(channel[0].size() != numOutputs) channel = vector<vector<long double>>(3, vector<long double>(numOutputs));
                    for(int j = 0; j < numOutputs; j++){
                        channel[i][j] = hyper.channel.matrix[i][j];
                    }
                }
            }

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
                DrawTextEx(mainFont, "X1", triangleTextPos[0], headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X2", triangleTextPos[1], headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X3", triangleTextPos[2], headerFontSize, 1.0, BLACK);
            
                if(drawCircles && hyperReady){
                    // Prior distribution
                    DrawCircle(priorPosition.x, priorPosition.y, PRIOR_RADIUS, priorColor);
                    DrawCircleLines(priorPosition.x, priorPosition.y, PRIOR_RADIUS, priorBorderColor);
                    DrawTextEx(mainFont, "prior", Vector2({(float)priorPosition.x-(PRIOR_RADIUS/1.5f), \
                                   (float)priorPosition.y - (0.2f * PRIOR_RADIUS)}), headerFontSize, 1.0, BLACK);

                    // Posterior distributions
                    for(int i = 0; i < hyper.num_post; i++){
                        int radius = (int)sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);
                        DrawCircle(posteriorsPosition[i].x, posteriorsPosition[i].y, radius, posteriorColor);
                        DrawCircleLines(posteriorsPosition[i].x, posteriorsPosition[i].y, radius, posteriorBorderColor);
                        sprintf(buffer, "I%d", i+1);
                        DrawTextEx(mainFont, buffer, Vector2({(float)posteriorsPosition[i].x-7, \
                                   (float)posteriorsPosition[i].y - 10}), headerFontSize, 1.0, BLACK);
                    }
            }

        // Check hyper
            if(drawCircles){
                // Check if the prior distribution and the channel are both valid
                hyperReady = true;
                if(Distribution::isDistribution(prior)){
                    // Check if each line of the channel matrix is a probability distribution
                    if(!Channel::isChannel(channel)){
                        hyperReady = false;
                        error = true;
                        strcpy(errorBuffer, "The channel is invalid!");
                    }

                    if(hyperReady){
                        error = false;
                        Distribution new_prior(prior);
                        Channel new_channel(new_prior, channel);
                        hyper = Hyper(new_channel);
                        updateHyper(priorPosition, posteriorsPosition, matricesRectangles);
                        updateMatricesText(matricesTexts);
                    }
                }else{
                    hyperReady = false;
                    error = true;
                    strcpy(errorBuffer, "The prior distribution is invalid!");
                }
            }else{
                hyperReady = false;
            }

        // Matrices -> Prior, Channel, Gain function, Inners
            // Prior
            for(int i = 0; i < 3; i++){
                sprintf(buffer, "X%d", i+1);
                GuiTextBox(matricesRectangles[PRIOR][0][i], (char*)matricesTexts[PRIOR][0][i].c_str(), PROB_PRECISION, true); // X1
                DrawTextEx(mainFont, buffer, (Vector2){matricesRectangles[PRIOR][0][i].x + LABEL_Y_HOR_GAP, matricesRectangles[PRIOR][0][i].y + LABEL_Y_VER_GAP}, headerFontSize, 1.0, BLACK);
            }
            updatePrior(matricesRectangles);

            // Channel    
            oldnumOutputs = numOutputs;
            GuiSpinner(staticRectangles[CHANNEL_SPINNER], &numOutputs, 1, MAX_OUTPUTS, true);
            if(oldnumOutputs != numOutputs){
                drawCircles = false;
                hyperReady = false;
            }

            channelPanelRec = (Rectangle){0.01f*windowWidth, \
                                          H2(windowHeight) + 55, \
                                          V1(windowWidth)-0.02f*windowWidth, \
                                          5*(BOX_HEIGHT + BOX_VER_GAP)};

            channelPanelContentRec = (Rectangle){channelPanelRec.x, \
                                                 channelPanelRec.y, \
                                                 V2(windowWidth) + (numOutputs*(BOX_WIDTH+BOX_HOR_GAP)), \
                                                 channelPanelRec.height-15};

            Rectangle view = GuiScrollPanel(channelPanelRec, channelPanelContentRec, &channelPanelScroll);
            updateChannel(oldnumOutputs, numOutputs, matricesRectangles, matricesTexts, channelPanelScroll);
            BeginScissorMode(view.x, view.y, view.width, view.height);        
                DrawTextEx(mainFont, "X1", (Vector2){matricesRectangles[CHANNEL][0][0].x + LABEL_X_HOR_GAP, matricesRectangles[CHANNEL][0][0].y + LABEL_X_VER_GAP}, headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X2", (Vector2){matricesRectangles[CHANNEL][1][0].x + LABEL_X_HOR_GAP, matricesRectangles[CHANNEL][1][0].y + LABEL_X_VER_GAP}, headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X3", (Vector2){matricesRectangles[CHANNEL][2][0].x + LABEL_X_HOR_GAP, matricesRectangles[CHANNEL][2][0].y + LABEL_X_VER_GAP}, headerFontSize, 1.0, BLACK);
                for(int j = 0; j < numOutputs; j++){
                    sprintf(buffer, "Y%d", j+1);
                    DrawTextEx(mainFont, buffer, (Vector2){matricesRectangles[CHANNEL][0][j].x + LABEL_Y_HOR_GAP, matricesRectangles[CHANNEL][0][j].y + LABEL_Y_VER_GAP}, headerFontSize, 1.0, BLACK);
                    GuiTextBox(matricesRectangles[CHANNEL][0][j], (char*)matricesTexts[CHANNEL][0][j].c_str(), PROB_PRECISION, true);
                    GuiTextBox(matricesRectangles[CHANNEL][1][j], (char*)matricesTexts[CHANNEL][1][j].c_str(), PROB_PRECISION, true);
                    GuiTextBox(matricesRectangles[CHANNEL][2][j], (char*)matricesTexts[CHANNEL][2][j].c_str(), PROB_PRECISION, true);
                }
            EndScissorMode();
            
            // Inners
            if(drawCircles && hyperReady){
                DrawTextEx(mainFont, "OUTER", (Vector2){matricesRectangles[OUTER][0][0].x + LABEL_X_HOR_GAP - 33, matricesRectangles[OUTER][0][0].y + LABEL_X_VER_GAP}, headerFontSize, 1.0, BLACK);
                Vector2 startPos = {TV1(windowWidth) - LABEL_X_HOR_GAP - 35, H1(windowHeight) + 70 + BOX_HEIGHT + BOX_VER_GAP/2.0f};
                DrawLineV(startPos, (Vector2){startPos.x + (hyper.num_post*(BOX_WIDTH + BOX_HOR_GAP)), startPos.y}, BLACK);

                DrawTextEx(mainFont, "X1", (Vector2){matricesRectangles[INNERS][0][0].x + LABEL_X_HOR_GAP, matricesRectangles[INNERS][0][0].y + LABEL_X_VER_GAP}, headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X2", (Vector2){matricesRectangles[INNERS][1][0].x + LABEL_X_HOR_GAP, matricesRectangles[INNERS][1][0].y + LABEL_X_VER_GAP}, headerFontSize, 1.0, BLACK);
                DrawTextEx(mainFont, "X3", (Vector2){matricesRectangles[INNERS][2][0].x + LABEL_X_HOR_GAP, matricesRectangles[INNERS][2][0].y + LABEL_X_VER_GAP}, headerFontSize, 1.0, BLACK);
                for(int i = 0; i < hyper.num_post; i++){
                    sprintf(buffer, "I%d", i+1);
                    DrawTextEx(mainFont, buffer, (Vector2){matricesRectangles[INNERS][0][i].x + LABEL_Y_HOR_GAP, matricesRectangles[OUTER][0][i].y + LABEL_Y_VER_GAP}, headerFontSize, 1.0, BLACK);
                    GuiTextBox(matricesRectangles[OUTER][0][i], (char*)matricesTexts[OUTER][0][i].c_str(), PROB_PRECISION, false);
                    GuiTextBox(matricesRectangles[INNERS][0][i], (char*)matricesTexts[INNERS][0][i].c_str(), PROB_PRECISION, false);
                    GuiTextBox(matricesRectangles[INNERS][1][i], (char*)matricesTexts[INNERS][1][i].c_str(), PROB_PRECISION, false);
                    GuiTextBox(matricesRectangles[INNERS][2][i], (char*)matricesTexts[INNERS][2][i].c_str(), PROB_PRECISION, false);
                }
            }
        
        // Error message
            if(error){
                DrawTextEx(mainFont, errorBuffer, (Vector2){(V1(windowWidth) - strlen(errorBuffer)*7)/2.0f, channelPanelRec.y + channelPanelRec.height + 15}, headerFontSize-5, 1.0, BLACK); 
            }

        // Menu
            updateMenu(staticRectangles, menuActiveOption, menuDropEditMode, dragMenuWindow, \
                exitSelectMenuWindow, offset);
        
        drawCircles = GuiCheckBox(staticRectangles[DRAW_CHECK_BOX], "Draw", drawCircles);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}