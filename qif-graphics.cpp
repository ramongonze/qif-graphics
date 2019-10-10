#include <sstream>
#include <algorithm>
#include <cstring>

#include "include/QIF.h"
#include "include/menu.h"
#include <emscripten/emscripten.h>

void printError(int error, Layout layout, QIF qif);

char buffer[MAX_BUFFER];
bool drawCircles = false; // Flag that indicates if the circles must be drawn or not. Its value is set by a check box
bool hyperReady = false;  // Flag that indicates if a hyper distribution has been built
int error = 0;            // Flag that indicates if an error has been occurred

char test1;
char test2;
// Colors
//--------------------------------------------------------------------------------------
Colors colors(2);

// Variables
// ----------------------------------------------------------------------------------
Layout layout;
QIF qif;

// // Menu
Menu menu;


void updateAndDraw(){
    // General update
    // ----------------------------------------------------------------------------------

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
// Vector2 mousePosition = GetMousePosition();
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground({245, 245, 245, 255});
        layout.draw(colors);
        
        // GuiTextBox((Rectangle){100,100,50,50}, &test1, PROB_PRECISION, true); // X1
        // GuiTextBox((Rectangle){180,100,50,50}, &test2, PROB_PRECISION, false); // X1
        // qif.drawMatrices(colors, layout);
        // if(drawCircles && hyperReady){
        //     qif.drawCircles(colors, layout);
        // }

        // // Error message
        // if(error) printError(error, layout, qif);
    
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

    // // // Menu
    // // Menu menu;
    menu.init();
    // //----------------------------------------------------------------------------------

    // // Window
    // //--------------------------------------------------------------------------------------
    // // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); // Problems with scroll panel
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "QIF Graphics");
    // SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);
    emscripten_set_main_loop(updateAndDraw, 0, 1);

    // Text font
    layout.mainFont = LoadFontEx("SchoolTimes.ttf", layout.headerFontSize, 0, 0);
    GenTextureMipmaps(&(layout.mainFont).texture);
    SetTextureFilter(layout.mainFont.texture, FILTER_POINT);

    SetTargetFPS(5); // Set the program to run at 60 frames-per-second
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