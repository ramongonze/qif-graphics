#include "raylib.h"
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

using namespace std;

int main(void){
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1024;
    int screenHeight = 768;
    float headerFontSize = 20;

    /*
     * Left menu:
     * 
     * Remaining percentage: 1%
     *
     * | Menu bar : 4%
     * | ------------------------|
     * | Prior distribution: 15% |
     * |                         |
     * | ------------------------|
     * | Channel: 40%            |
     * |                         |
     * |                         |
     * |                         |
     * | ------------------------|
     * | Gain Function: 40%      |
     * |                         |
     * |                         |
     * |                         |
     * |_________________________|
     *              40%                     60%
     */

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
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

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
        mainTriangleV1 = {0.45f*screenWidth, 0.90f*screenHeight};
        mainTriangleV2 = {0.70f*screenWidth, 0.40f*screenHeight};
        mainTriangleV3 = {0.95f*screenWidth, 0.90f*screenHeight};
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