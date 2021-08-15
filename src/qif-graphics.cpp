/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera free
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "../libs/raylib/src/raylib.h"
#include "../libs/raylib/src/rlgl.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "../libs/raygui/src/raygui.h"
#include <iostream>
#include <string>
#include "gui/gui.h"
#include "data.h"
#include "chull.h"

void DrawGridEx(int slices, float spacing, Vector3 pos, char* plane);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 20.0f, 20.0f, 20.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    Rectangle anchor = {300.0f, 100.0f, 600.0f, 600.0f};
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);          // Update camera

        if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f + anchor.x, 0.0f + anchor.y, 0.0f };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                DrawGridEx(10, 1.0f, (Vector3){0,0,0}, "xz");
                DrawGridEx(10, 1.0f, (Vector3){0,5,-5}, "xy");
                DrawGridEx(10, 1.0f, (Vector3){-5,5,0}, "yz");

            EndMode3D();

            string s = to_string((int)camera.position.x) + string(",") + to_string((int)camera.position.y) + string(",") + to_string((int)camera.position.z);
            DrawText(s.c_str(), 100, 100, 30, BLACK);

            // DrawText("Free camera default controls:", 20, 20, 10, BLACK);
            // DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
            // DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
            // DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
            // DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
            // DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
/*
 *        Y 
 *          |
 *          |
 *          |_________ X
 *         /
 *        /
 *    Z  / 
*/
void DrawGridEx(int slices, float spacing, Vector3 pos, char* plane){
    int halfSlices = slices/2;

    for (int i = -halfSlices; i <= halfSlices; i++){
        if(strcmp(plane, "xz") == 0){
            DrawLine3D(
                (Vector3){pos.x + (float)i*spacing, pos.y, pos.z + (float)-halfSlices*spacing},
                (Vector3){pos.x + (float)i*spacing, pos.y, pos.z + (float)halfSlices*spacing},
                GRAY
            );
            DrawLine3D(
                (Vector3){pos.x + (float)-halfSlices*spacing, pos.y, pos.z + (float)i*spacing},
                (Vector3){pos.x + (float)halfSlices*spacing, pos.y, pos.z + (float)i*spacing},
                GRAY
            );
        }else if(strcmp(plane, "xy") == 0){
            DrawLine3D(
                (Vector3){pos.x + (float)i*spacing, pos.y + (float)-halfSlices*spacing, pos.z},
                (Vector3){pos.x + (float)i*spacing, pos.y + (float)halfSlices*spacing, pos.z},
                GRAY
            );
            DrawLine3D(
                (Vector3){pos.x + (float)-halfSlices*spacing, pos.y + (float)i*spacing, pos.z},
                (Vector3){pos.x + (float)halfSlices*spacing, pos.y + (float)i*spacing, pos.z},
                GRAY
            );
        }else if(strcmp(plane, "yz") == 0){
            DrawLine3D(
                (Vector3){pos.x , pos.y + (float)i*spacing, pos.z + (float)-halfSlices*spacing},
                (Vector3){pos.x , pos.y + (float)i*spacing, pos.z + (float)halfSlices*spacing},
                GRAY
            );
            DrawLine3D(
                (Vector3){pos.x, pos.y + (float)-halfSlices*spacing, pos.z + (float)i*spacing},
                (Vector3){pos.x, pos.y + (float)halfSlices*spacing, pos.z + (float)i*spacing},
                GRAY
            );
        }        
    }
}
