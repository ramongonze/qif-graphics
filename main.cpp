#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

int main(void){
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int fontHeaders = 14;

    /*
     * Left menu:
     * 
     * | Menu bar : 5%
     * | ------------------------
     * | Prior distribution: 15%
     * | 
     * | ------------------------
     * | Channel: 40%
     * |
     * |
     * |
     * | ------------------------
     * | Gain Function: 40%
     * |
     * |
     * |
     * |___________________________
     */
    const float priorYPer = 0.05;   // Prior Y axis percentage of screen size
    const float channelYPer = 0.20; // Prior Y axis percentage of screen size
    const float gainYPer = 0.60;    // Prior Y axis percentage of screen size

    // Set font
    Font mainFont = LoadFont("l.png");

    InitWindow(screenWidth, screenHeight, "QIF Graphics");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            // DrawTextEx(mainFont, "Prior distribution", Vector2({screenWidth*0.01, screenHeight*priorYPer}), fontHeaders, 1.0, BLACK);
            // DrawText("Prior distribution", screenWidth*0.01, screenHeight*priorYPer, fontHeaders, BLACK);
            // DrawText("Channel", screenWidth*0.01, screenHeight*channelYPer, fontHeaders, BLACK);
            // DrawText("Gain Function", screenWidth*0.01, screenHeight*gainYPer, fontHeaders, BLACK);
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