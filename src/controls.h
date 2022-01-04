#ifndef _controls
#define _controls

#include "gui/gui.h"
#include "data.h"
#include "graphics.h"

class Controls{
private:
    /* data */
public:
    // Default constructor
    Controls();

    // Controls Functions Declaration
    //----------------------------------------------------------------------------------
    void buttonFile(Gui &gui, Data &data, bool* closeWindow);
    void buttonMode(Gui &gui, Data &data, int* prevMode);
    void buttonExamples(Gui &gui, Data &data);
    void buttonGuide(Gui &gui);
    void buttonRandomPrior(Gui &gui, Data &data);
    void buttonsTabs(Gui &gui, int channel);
    void buttonRandomChannel(Gui &gui, Data &data);
    void buttonDraw(Gui &gui, Data &data);
    void updateStatusBar(int error, GuiVisualization &visualization);
};

#endif
