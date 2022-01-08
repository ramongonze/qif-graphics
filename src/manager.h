#ifndef _manager
#define _manager

#include "../libs/raylib/src/raylib.h"
#include "../libs/raygui/src/raygui.h"
#include "draw.h"
#include "random-response.h"
#include "truncated-geometric.h"

class Manager{
public:
    Manager();

    Gui gui;
    Data data;
    Draw draw;

    void readFonts(Font* defaultFont, Font* defaultFontBig);
    void checkButtonsMouseCollision();
    void checkHelpMessagesActive(Vector2 mousePosition);
    void checkPriorFlags(); // Check compute and ready prior flags
    void checkChannelsFlags(); // Check compute and ready channels flags
    void checkHypersFlags(); // Check compute and ready hypers flags
};

#endif