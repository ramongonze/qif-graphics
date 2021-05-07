#ifndef _gui
#define _gui

#include "guimenu.h"
#include "guiprior.h"
#include "guichannel.h"
#include "guiposteriors.h"
#include "guivisualization.h"

class Gui{
private:
    /* data */
public:
    Gui();

    GuiMenu menu;
    GuiPrior prior;
    GuiChannel channel;
    GuiPosteriors posteriors;
    GuiVisualization visualization;
};

#endif