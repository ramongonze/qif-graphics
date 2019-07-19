#include "raylib.h"
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_TEXTBOX_EXTENDED
#include "raygui/src/raygui.h"

#include "qif/qif.hpp"
#include "include/graphics.hpp"

#define MAX_POSTERIOR 15 // Max number of posterior distributions
#define MAX_INPUT_BOX 100 // Max number of characters in an input box

using namespace std;

/*
 * Left menu:
 * 
 * Remaining percentage: 1%
 *
 * ----------------------------------------------------------------            _
 * | Menu bar : 4%                                                |            _| 4%
 * | ------------------------|------------------------------------| _          _
 * | Prior distribution: 15% |                                    |  |          |
 * |                         |                                    |  | 36%      |
 * | ------------------------|                                    | _|          |
 * | Channel: 40%            |                                    | _           |
 * |                         |                / \ - - - - - - - - |  |          |
 * |                         |               /   \                |  |          | 
 * |                         |              /     \               |  |          | 96%
 * |                         |             /       \              |  | 50%      |
 * | ------------------------|           /           \            |  |          |
 * | Gain Function: 40%      |          /             \           | _|          |
 * |                         |        /_________________\ - - - - | _           |
 * |                         |                                    |  |          |
 * |                         |                                    |  | 10%      |
 * |_________________________|[_5%_][_________50%__________][_5%_]| _|         _|
 *
 * |_________________________|____________________________________|
 *              40%                             60%
 *
 * OBS: The percentages in the interface are realted to the screen width and height.
 *
 */

void updateHyper(Hyper &hyper, Point &priorPosition, vector<Point> &posteriorsPosition, \
    int screenWidth, int screenHeight){
    // Update the hyper distribution if the user moves the prior distribution
    Point p, mousePosition;
    vector<long double> new_prior(3);

    p = dist2Bary(*(hyper.prior));
    priorPosition = bary2Pixel(p.x, p.y, screenWidth, screenHeight);   
    Vector2 aux = GetMousePosition();
    mousePosition.x = aux.x; mousePosition.y = aux.y;

    // Check if the user is moving the prior
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && euclidianDistance(priorPosition, mousePosition) <= PRIOR_RADIUS){
        mousePosition = pixel2Bary(mousePosition.x, mousePosition.y, screenWidth, screenHeight);
        if(bary2Dist(mousePosition, hyper.prior->prob)){
            
            // Rounds to a point distribution if one of the probabilities is >= 0.98
            if(hyper.prior->prob[0] >= 0.98) hyper.prior->prob = {1, 0, 0};
            else if(hyper.prior->prob[1] >= 0.98) hyper.prior->prob = {0, 1, 0};
            else if(hyper.prior->prob[2] >= 0.98) hyper.prior->prob = {0, 0, 1};

            // cout << "Here: " << hyper.prior->prob[0] << "," << hyper.prior->prob[1] << "," \
            // << hyper.prior->prob[2] << ": " << Distribution::isDistribution(hyper.prior->prob) <<  endl;
            hyper.rebuildHyper(*(hyper.prior));
            // printf("chegou depois\n");
            // cout << "After: " << hyper.prior->prob[0] << endl;
            p = dist2Bary(*(hyper.prior));
            priorPosition = bary2Pixel(p.x, p.y, screenWidth, screenHeight);
        }
    }
    
    // Calculates the pixel coordinate for posterior distributions
    posteriorsPosition.resize(hyper.num_post);
    for(int i = 0; i < hyper.num_post; i++){
        long double x1 = hyper.inners[0][i];
        long double x2 = hyper.inners[1][i];
        long double x3 = hyper.inners[2][i];
        p = dist2Bary(x1, x2, x3);
        posteriorsPosition[i] = bary2Pixel(p.x, p.y, screenWidth, screenHeight);
    }
}

void drawQIFCircles(Hyper &hyper){
    int i;
}


int main(void){
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = WINDOW_WIDTH;
    int screenHeight = WINDOW_HEIGHT;
    float headerFontSize = 20;

    // Gui style
    // GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0x000000ff);

    // QIF
    //----------------------------------------------------------------------------------
    Point priorPosition;
    vector<Point> posteriorsPosition;
    vector<string> distributionTexts; // Posteriors names (y1, ..., yn)
    char priorDistribution[3][MAX_INPUT_BOX] = {"", "", ""};
    bool priorDistActiveBox[3] = {false, false, false};
    Hyper hyper("prior", "channel");
    Color priorColor = {128, 191, 255, 230};
    Color priorLineColor = {51, 153, 255, 230};
    Color posteriorColor = {255, 222, 78, 230};
    Color posteriorLineColor = {230, 187, 0, 230};

    //----------------------------------------------------------------------------------

    // Variables
    //----------------------------------------------------------------------------------
    // Text
    Vector2 priorFontPosition, channelFontPosition, gainFontPosition;
    char posterior[MAX_POSTERIOR]; // Buffer to print y1, ..., yn inside the posterior circles
    
    // Rectangles
    Color recColor = {148, 148, 184, 180};
    Color recLinesColor = {102, 102, 153, 180};
    Vector2 menuRecPosition, menuRecSize;
    Vector2 priorRecPosition, priorRecSize;
    Vector2 channelRecPosition, channelRecSize;
    Vector2 gainRecPosition, gainRecSize;

    // Main Triangle
    Vector2 mainTriangleV1, mainTriangleV2, mainTriangleV3;
    Vector2 x1Position, x2Position, x3Position;
    //----------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "QIF Graphics");

    // Set font
    Font mainFont = LoadFontEx("CaviarDreams.ttf", headerFontSize, 0, 0);
    GenTextureMipmaps(&mainFont.texture);
    SetTextureFilter(mainFont.texture, FILTER_POINT);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        // Update     
        //----------------------------------------------------------------------------------
        // Interface ---------
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        // QIF -----------
        updateHyper(hyper, priorPosition, posteriorsPosition, screenWidth, screenHeight);
        // Text
        priorFontPosition = {screenWidth*0.01f, screenHeight*0.05f};
        channelFontPosition = {screenWidth*0.01f, screenHeight*0.20f};
        gainFontPosition = {screenWidth*0.01f, screenHeight*0.60f};

        // Rectangles features
        menuRecPosition = {0.0f, 0.0f};
        menuRecSize = {(float)screenWidth, screenHeight*0.04f}; // Width and Height
        
        priorRecPosition = {0.0f, 0.04f*screenHeight};
        priorRecSize = {0.4f*screenWidth, 0.15f*screenHeight};

        channelRecPosition = {0.0f, 0.19f*screenHeight};
        channelRecSize = {0.4f*screenWidth, 0.4f*screenHeight};

        gainRecPosition = {0.0f, 0.59f*screenHeight};
        gainRecSize = {0.4f*screenWidth, 0.41f*screenHeight};

        // GUI
        // priorDistActiveBox[0] = GuiTextBoxIsActive((Rectangle){0.05f*screenWidth,0.10f*screenHeight,65,30});
        // priorDistActiveBox[1] = GuiTextBoxIsActive((Rectangle){0.13f*screenWidth,0.10f*screenHeight,65,30});
        // priorDistActiveBox[2] = GuiTextBoxIsActive((Rectangle){0.21f*screenWidth,0.10f*screenHeight,65,30});
        // printf("%d, %d, %d\n", priorDistActiveBox[0], priorDistActiveBox[1], priorDistActiveBox[2]);

        // Main Triangle Features
        mainTriangleV1 = {0.70f*screenWidth, 0.40f*screenHeight};
        mainTriangleV2 = {0.45f*screenWidth, 0.90f*screenHeight};
        mainTriangleV3 = {0.95f*screenWidth, 0.90f*screenHeight};
        x1Position = {0.69f*screenWidth, 0.37f*screenHeight};
        x2Position = {0.42f*screenWidth, 0.89f*screenHeight};
        x3Position = {0.96f*screenWidth, 0.89f*screenHeight};
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            // Rectangles
            DrawRectangleV(menuRecPosition, menuRecSize, recColor);
            DrawRectangleLines(menuRecPosition.x, menuRecPosition.y, menuRecSize.x, menuRecSize.y, recLinesColor);
            DrawRectangleV(priorRecPosition, priorRecSize, recColor);
            DrawRectangleLines(priorRecPosition.x, priorRecPosition.y, priorRecSize.x, priorRecSize.y, recLinesColor);
            DrawRectangleV(channelRecPosition, channelRecSize, recColor);
            DrawRectangleLines(channelRecPosition.x, channelRecPosition.y, channelRecSize.x, channelRecSize.y, recLinesColor);
            DrawRectangleV(gainRecPosition, gainRecSize, recColor);
            DrawRectangleLines(gainRecPosition.x, gainRecPosition.y, gainRecSize.x, gainRecSize.y, recLinesColor);

            // Text
            DrawTextEx(mainFont, "Prior distribution", priorFontPosition, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "Channel", channelFontPosition, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "Gain Function", gainFontPosition, headerFontSize, 1.0, BLACK);

            // GUI
            GuiTextBox((Rectangle){0.05f*screenWidth,0.10f*screenHeight,65,30}, priorDistribution[0], MAX_INPUT_BOX, true); // X1
            GuiTextBox((Rectangle){0.13f*screenWidth,0.10f*screenHeight,65,30}, priorDistribution[1], MAX_INPUT_BOX, true); // X2
            GuiTextBox((Rectangle){0.21f*screenWidth,0.10f*screenHeight,65,30}, priorDistribution[2], MAX_INPUT_BOX, true); // X3

            // Main Triangle
            DrawTriangleLines(mainTriangleV1, mainTriangleV2, mainTriangleV3, BLACK); 
            DrawTextEx(mainFont, "X1", x1Position, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X2", x2Position, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X3", x3Position, headerFontSize, 1.0, BLACK);

            // Prior distribution
            DrawCircle(priorPosition.x, priorPosition.y, PRIOR_RADIUS, priorColor);
            DrawCircleLines(priorPosition.x, priorPosition.y, PRIOR_RADIUS, priorLineColor);
            DrawTextEx(mainFont, "prior", Vector2({priorPosition.x-(PRIOR_RADIUS/2.0f), \
                priorPosition.y - (0.2f * PRIOR_RADIUS)}), headerFontSize, 1.0, BLACK);

            // Posterior distributions
            for(int i = 0; i < hyper.num_post; i++){
                int radius = (int)sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);
                DrawCircle(posteriorsPosition[i].x, posteriorsPosition[i].y, radius, posteriorColor);
                DrawCircleLines(posteriorsPosition[i].x, posteriorsPosition[i].y, radius, posteriorLineColor);
                sprintf(posterior, "Y%d", i+1);
                DrawTextEx(mainFont, posterior, Vector2({posteriorsPosition[i].x-(radius/2.0f), \
                posteriorsPosition[i].y - (0.4f * radius)}), headerFontSize, 1.0, BLACK);
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