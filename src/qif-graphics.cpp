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

Vector3 point2DToXZPlane(Vector2 point);
void drawGridPlane(int slices, float spacing, Vector3 origin, const char* plane);
void drawAxisArrows(Vector3 origin, int slices, Camera3D camera);
void draw3DLabels(Vector3 origin, int slices, Camera3D camera);
void drawCircleInTriangle(Distribution& dist, Color color, float radius);
void drawVg(Distribution& dist, Color color, float radius);
void drawTent(Hyper& hyper);
Vector3 distTo3D(Gain& gain, Vector3 origin);
void fixCameraPos(Camera* camera); // Back it to be in the bounds

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "QIF Graphics");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){20.0f, 20.0f, 20.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    Rectangle anchor = {300.0f, 100.0f, 600.0f, 600.0f};
    Vector3 origin = {0.0f, 0.0f, 0.0f};

    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    // vector<long double> dist = {1.0f/4, 1.0f/2, 1.0f/4};
    vector<long double> dist = {1.0f/3, 1.0f/3, 1.0f/3};
    Distribution prior(dist);
    std::vector<std::vector<long double>> M(
        {{1.0f/2, 1.0f/2,      0,      0},
         {     0, 1.0f/4, 1.0f/2, 1.0f/4},
         {1.0f/2, 1.0f/3, 1.0f/6,      0}}
    );

    // Channel that leaks everythign
    // std::vector<std::vector<long double>> M(
    //     {{1,0,0},
    //      {0,1,0},
    //      {0,0,1}}
    // );

    // Channel that leaks nothing
    // std::vector<std::vector<long double>> M(
    //     {{0.0f/3, 2.0f/3, 1.0f/3},
    //      {1.0f/3, 1.0f/3, 1.0f/3},
    //      {2.0f/3, 1.0f/3, 0.0f/3}}
    // );
    Channel channel(prior, M);
    Hyper hyper(channel);

    // Bayes leakage
    std::vector<std::vector<long double>> g(
        {{1, 0, 0},
         {0, 1, 0},
         {0, 0, 1}}
    );
    Gain gain(prior, g);

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(!(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON) && !IsKeyDown(KEY_LEFT_ALT))){
            UpdateCamera(&camera);          // Update camera
            fixCameraPos(&camera);
        }

        // Camera bounds
        if(camera.position.x)


        if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f + anchor.x, 0.0f + anchor.y, 0.0f };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                drawGridPlane(11, 1.0f, origin, "xz");
                drawGridPlane(11, 1.0f, origin, "xy");
                drawGridPlane(11, 1.0f, origin, "yz");
                drawAxisArrows(origin, 11, camera);

                // Baricentric plane is XZ
                // I'm assuming that 10 squares in the grid corresponds to '1' in all axis
                DrawTriangle3D((Vector3){0,0,10}, (Vector3){5,0,0}, (Vector3){10,0,10}, BG_BASE_COLOR_LIGHT);
                DrawTriangle3D((Vector3){10,0,10}, (Vector3){5,0,0}, (Vector3){0,0,10}, BG_BASE_COLOR_LIGHT);
                
                DrawLine3D((Vector3){0,0,10}, (Vector3){5,0,0}, BLACK);
                DrawLine3D((Vector3){5,0,0}, (Vector3){10,0,10}, BLACK);
                DrawLine3D((Vector3){0,0,10}, (Vector3){10,0,10}, BLACK);

                // Prior 
                drawCircleInTriangle(prior, PRIOR_COLOR, 1);    // Circle

                // Inners
                for(int i = 0; i < hyper.num_post; i++){
                    std::vector<long double> v({hyper.inners[0][i],hyper.inners[1][i], hyper.inners[2][i]});
                    Distribution dist(v);
                    float radius = sqrt(hyper.outer.prob[i]);

                    drawCircleInTriangle(dist, INNERS1_COLOR, radius);
                }

                drawTent(hyper);

            EndMode3D();

            draw3DLabels(origin, 10, camera);
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

void drawGridPlane(int slices, float spacing, Vector3 origin, const char* plane){
    /*
    *        Y |
    *          |
    *          |
    *          |_________ X
    *         /
    *        /
    *    Z  / 
    */
    for (int i = 0; i <= slices; i++){
        if(strcmp(plane, "xz") == 0){
            DrawLine3D(
                (Vector3){origin.x + (float)i*spacing, origin.y, origin.z + 0},
                (Vector3){origin.x + (float)i*spacing, origin.y, origin.z + (float)slices*spacing},
                LIGHTGRAY
            );
            DrawLine3D(
                (Vector3){origin.x + 0, origin.y, origin.z + (float)i*spacing},
                (Vector3){origin.x + (float)slices*spacing, origin.y, origin.z + (float)i*spacing},
                LIGHTGRAY
            );
        }else if(strcmp(plane, "xy") == 0){
            DrawLine3D(
                (Vector3){origin.x + (float)i*spacing, origin.y + 0, origin.z},
                (Vector3){origin.x + (float)i*spacing, origin.y + (float)slices*spacing, origin.z},
                LIGHTGRAY
            );
            DrawLine3D(
                (Vector3){origin.x + 0, origin.y + (float)i*spacing, origin.z},
                (Vector3){origin.x + (float)slices*spacing, origin.y + (float)i*spacing, origin.z},
                LIGHTGRAY
            );
        }else if(strcmp(plane, "yz") == 0){
            DrawLine3D(
                (Vector3){origin.x , origin.y + (float)i*spacing, origin.z + 0},
                (Vector3){origin.x , origin.y + (float)i*spacing, origin.z + (float)slices*spacing},
                LIGHTGRAY
            );
            DrawLine3D(
                (Vector3){origin.x, origin.y + 0, origin.z + (float)i*spacing},
                (Vector3){origin.x, origin.y + (float)slices*spacing, origin.z + (float)i*spacing},
                LIGHTGRAY
            );
        }        
    }
}

void drawAxisArrows(Vector3 origin, int slices, Camera3D camera){
    // slices: The same used to draw grid
    // Y axis
    DrawLine3D(
        (Vector3){origin.x, origin.y, origin.z},
        (Vector3){origin.x, origin.y + slices+2, origin.z},
        BLACK
    );
}   

void draw3DLabels(Vector3 origin, int slices, Camera3D camera){
    Vector2 originScreenPos;
    // Vg arrow label
    originScreenPos = GetWorldToScreen((Vector3){0.5f, origin.y+slices+2, origin.z}, camera);
    DrawText("Vg", (int)originScreenPos.x - MeasureText("Vg", 20)/2, (int)originScreenPos.y, 20, BLACK);
    
    // Secrets
    // X1
    originScreenPos = GetWorldToScreen((Vector3){origin.x+slices/2, 0.5f, origin.z}, camera);
    DrawText("X1\n(1,0,0)", (int)originScreenPos.x - MeasureText("X1\n(1,0,0)", 20)/2, (int)originScreenPos.y, 20, GRAY);    
    // X2
    originScreenPos = GetWorldToScreen((Vector3){origin.x, 0.5f, origin.z+slices}, camera);
    DrawText("X2\n(0,1,0)", (int)originScreenPos.x - MeasureText("X2\n(0,1,0)", 20)/2, (int)originScreenPos.y, 20, GRAY);
    // X3
    originScreenPos = GetWorldToScreen((Vector3){origin.x+slices, 0.5f, origin.z+slices}, camera);
    DrawText("X3\n(0,0,1)", (int)originScreenPos.x - MeasureText("X3\n(0,0,1)", 20)/2, (int)originScreenPos.y, 20, GRAY);
}

void drawCircleInTriangle(Distribution& dist, Color color, float radius){
    Point p = dist2Bary(dist);
    DrawCircle3D((Vector3){p.x*10, 0, (1-p.y)*10}, radius, (Vector3){1,0,0}, 90, color);
    DrawCylinder((Vector3){p.x*10, 0, (1-p.y)*10}, radius, radius, 0.01, 60, color);
}

void drawVg(Distribution& dist, Color color, float radius){
    long double vulnerability = bayesVulnerability(dist);    // Vulnerability
    Point p = dist2Bary(dist);
    DrawSphere((Vector3){p.x*10, vulnerability*10, (1-p.y)*10}, radius, color);
}

Vector3 distTo3D(Gain& gain, Vector3 origin){
    Point p = dist2Bary(gain.prior);
    long double vulnerability = gVulnerability(gain);    // Vulnerability
    Vector3 point3D = {origin.x+p.x*10, origin.y+vulnerability*10, origin.z+(1-p.y)*10};
    return point3D;
}

void fixCameraPos(Camera* camera){
    if(camera->position.x < -35) camera->position.x = -35;
    if(camera->position.x > 35) camera->position.x = 35;
    if(camera->position.y < -35) camera->position.y = -35;
    if(camera->position.y > 35) camera->position.y = 35;
    if(camera->position.z < -35) camera->position.z = -35;
    if(camera->position.z > 35) camera->position.z = 35;
}

void drawTent(Hyper& hyper){
    // Prior
    Point p = dist2Bary(hyper.prior);
    long double vulnerability = bayesVulnerability(hyper.prior);    // Vulnerability
    Vector3 priorPoint = {p.x*10, vulnerability*10, (1-p.y)*10};    

    vector<Vector3> innersPoints(hyper.num_post);
    for(int i = 0; i < hyper.num_post; i++){
        std::vector<long double> v({hyper.inners[0][i],hyper.inners[1][i], hyper.inners[2][i]});
        Distribution dist = Distribution(v);
        p = dist2Bary(dist);
        vulnerability = bayesVulnerability(dist);    // Vulnerability
        innersPoints[i] = (Vector3){(float)p.x*10, (float)vulnerability*10, (1-p.y)*10};
    }

    for(int i = 0; i < hyper.num_post-1; i++){
        DrawTriangle3D(priorPoint, innersPoints[i], innersPoints[i+1], INNERS1_COLOR_L1);
        DrawTriangle3D(priorPoint, innersPoints[i+1], innersPoints[i], INNERS1_COLOR_L1);
        DrawLine3D(priorPoint, innersPoints[i], INNERS1_COLOR_D2);
        DrawLine3D(priorPoint, innersPoints[i+1], INNERS1_COLOR_D2);
        DrawLine3D(innersPoints[i], innersPoints[i+1], INNERS1_COLOR_D2);
    }
    DrawTriangle3D(priorPoint, innersPoints[hyper.num_post-1], innersPoints[0], INNERS1_COLOR_L1);
    DrawTriangle3D(priorPoint, innersPoints[0], innersPoints[hyper.num_post-1], INNERS1_COLOR_L1);
    DrawLine3D(innersPoints[hyper.num_post-1], priorPoint, INNERS1_COLOR_D2);
}
