#include "raylib.h"
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

#include "qif/qif.hpp"
#include "include/graphics.hpp"

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
 * |                         |                                    |  |          |
 * |                         |                                    |  | 50%      | 
 * |                         |                                    |  |          | 96%
 * |                         |                                    |  |          |
 * | ------------------------|                                    |  |          |
 * | Gain Function: 40%      |                                    | _|          |
 * |                         |                                    |  _          |
 * |                         |                                    |   |         |
 * |                         |                                    |   |10%      |
 * |_________________________|[_5%_][_________50%__________][_5%_]|  _|        _|
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
    Point p;
    
    dist2BaryCoord(*(hyper.prior), p);
    screenCoord2PixelCoord(p.x, p.y, priorPosition, screenWidth, screenHeight);
    
    posteriorsPosition.resize(hyper.num_post);
    for(int i = 0; i < hyper.num_post; i++){
        long double x1 = hyper.inners[0][i];
        long double x2 = hyper.inners[1][i];
        long double x3 = hyper.inners[2][i];
        dist2BaryCoord(x1, x2, x3, p);
        screenCoord2PixelCoord(p.x, p.y, posteriorsPosition[i], screenWidth, screenHeight);
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

    // QIF
    //----------------------------------------------------------------------------------
    Point priorPosition;
    vector<Point> posteriorsPosition;
    Hyper hyper("prior", "channel");
    Color priorColor = {128, 191, 255, 160};
    Color priorLineColor = {51, 153, 255, 160};
    Color posteriorColor = {255, 222, 78, 160};
    Color posteriorLineColor = {230, 187, 0, 160};

    //----------------------------------------------------------------------------------

    // Variables
    //----------------------------------------------------------------------------------
    // Text
    Vector2 priorFontPosition, channelFontPosition, gainFontPosition;
    
    // Rectangles
    Color recColor = {51, 153, 255, 180};
    Color recLinesColor = {0, 115, 230, 180};
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
        gainRecSize = {0.4f*screenWidth, (float)0.41f*screenHeight};

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

            // Main Triangle
            DrawTriangleLines(mainTriangleV1, mainTriangleV2, mainTriangleV3, BLACK); 
            DrawTextEx(mainFont, "X1", x1Position, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X2", x2Position, headerFontSize, 1.0, BLACK);
            DrawTextEx(mainFont, "X3", x3Position, headerFontSize, 1.0, BLACK);

            // cout << priorPosition.x << "," << priorPosition.y << endl;
            // Prior distribution
            DrawCircle(priorPosition.x, priorPosition.y, PRIOR_RADIUS, priorColor);
            DrawCircleLines(priorPosition.x, priorPosition.y, PRIOR_RADIUS, priorLineColor);
            // Posterior distributions
            for(int i = 0; i < hyper.num_post; i++){
                int radius = (int)sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);
                DrawCircle(posteriorsPosition[i].x, posteriorsPosition[i].y, radius, posteriorColor);
                DrawCircleLines(posteriorsPosition[i].x, posteriorsPosition[i].y, radius, posteriorLineColor);
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