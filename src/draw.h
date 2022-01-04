#ifndef _draw
#define _draw

#include "../libs/raylib/src/raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "../libs/raygui/src/raygui.h"
#include "gui/gui.h"
#include "data.h"
#include "chull.h"
#include "controls.h"

class Draw{
public:
    //------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------
    Controls controls;
    
    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    Draw();
    
    // Default style
    void initStyle();

    // Draw Functions Definitions
    //------------------------------------------------------------------------------------
    void drawGuiMenu(Gui &gui, Data &data, bool* closeWindow);
    void drawGuiPrior(Gui &gui, Data &data);
    void drawGuiChannel(Gui &gui, Data &data);
    void drawGuiPosteriors(Gui &gui, Data &data);
    void drawGuiVisualization(Gui &gui, Data &data);
    void drawGettingStarted(Gui &gui);
    void drawCirclePrior(Gui &gui, Data &data);
    void drawCirclesInners(Gui &gui, Data &data, int channel);
    void drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title, Color contentColor, Font font);
    void drawContentDropBox(Gui &gui, Rectangle layoutTitle, Rectangle layoutContent, Color contentColor, Font font);
    void drawGSContent(Gui &gui, Rectangle panel, int option, int imgPadding);
    void drawHelpMessage(Gui &gui, Rectangle rec, char message[CHAR_BUFFER_SIZE]);
    void drawTab(Gui &gui, int channel, bool active);        // If the tab is currently active
};
#endif