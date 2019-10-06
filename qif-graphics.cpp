#include <sstream>
#include <algorithm>
#include <cstring>

#define RAYGUI_STATIC
#include "/home/ramon/raygui/src/raygui.h"
#include "include/QIF.h"
#include "include/menu.h"

void printError(int error, Layout layout, QIF qif);

char buffer[MAX_BUFFER];
bool drawCircles = false; // Flag that indicates if the circles must be drawn or not. Its value is set by a check box
bool hyperReady = false;  // Flag that indicates if a hyper distribution has been built
int error = 0;            // Flag that indicates if an error has been occurred

// Colors
//--------------------------------------------------------------------------------------
Colors colors(2);

// Variables
//----------------------------------------------------------------------------------
Layout layout;
QIF qif;

// Menu
Menu menu;


void updateAndDraw(){
    // test = GetDirectoryFiles(GetWorkingDirectory(), &count);
    // General update
    //----------------------------------------------------------------------------------
    
    Vector2 mousePosition = GetMousePosition();
    int windowWidth       = GetScreenWidth();
    int windowHeight      = GetScreenHeight();

    // Static Rectangles
    layout.update(windowWidth, windowHeight);
    //----------------------------------------------------------------------------------

    if(qif.oldNumOutputs != qif.numOutputs){
        drawCircles = false;
        hyperReady = false;
    }
    qif.oldNumOutputs = qif.numOutputs;

    // Convert text typed by the user to numbers
    if(hyperReady) qif.updateMatricesByNumbers();
    else error = qif.updateMatricesByText();

    // Check hyper
    if(drawCircles){
        hyperReady = true;
        error = qif.update(layout);
        if(error == INVALID_PRIOR || error == INVALID_CHANNEL)
            hyperReady = false;
    }else{
        hyperReady = false;
    }
    menu.update(layout);

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground({245, 245, 245, 255});
        layout.draw(colors);
        qif.drawMatrices(colors, layout);
        if(drawCircles && hyperReady){
            qif.drawCircles(colors, layout);
        }

    // Error message
        if(error) printError(error, layout, qif);
    
    menu.draw(colors);
    // drawCircles = GuiCheckBox(layout.staticRectangles[DRAW_CHECK_BOX], "Draw", drawCircles);

    EndDrawing();
    //----------------------------------------------------------------------------------
    
}

int main(){
    // Initialization
    //--------------------------------------------------------------------------------------
    // char buffer[MAX_BUFFER];
    // bool drawCircles = false; // Flag that indicates if the circles must be drawn or not. Its value is set by a check box
    // bool hyperReady = false;  // Flag that indicates if a hyper distribution has been built
    // int error = 0;            // Flag that indicates if an error has been occurred

    // // Colors
    // //--------------------------------------------------------------------------------------
    // Colors colors(2);

    // // Variables
    // //----------------------------------------------------------------------------------
    // Layout layout;
    layout.init();

    // QIF qif;
    qif.init();

    // // Menu
    // Menu menu;
    menu.init();
    //----------------------------------------------------------------------------------

    // Window
    //--------------------------------------------------------------------------------------
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); // Problems with scroll panel
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "QIF Graphics");
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);

    // Text font
    layout.mainFont = LoadFontEx("School Times.ttf", layout.headerFontSize, 0, 0);
    GenTextureMipmaps(&(layout.mainFont).texture);
    SetTextureFilter(layout.mainFont.texture, FILTER_POINT);

    SetTargetFPS(60); // Set the program to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // char **test;
    // int count;
    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        
        updateAndDraw();
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void printError(int error, Layout layout, QIF qif){
    char buffer[MAX_BUFFER];

    switch(error){
        case INVALID_VALUE:
            strcpy(buffer, "Some value in prior or channel is invalid!");
            break;
        case INVALID_PRIOR:
            strcpy(buffer, "The prior distribution is invalid!");
            break;
        case INVALID_CHANNEL:
            strcpy(buffer, "The channel is invalid!");
            break;
    }

    DrawTextEx(layout.mainFont, buffer, (Vector2){(V1(layout.windowWidth) - strlen(buffer)*7)/2.0f, qif.channelPanelRec.y + qif.channelPanelRec.height + 15}, layout.headerFontSize-5, 1.0, BLACK); 
}