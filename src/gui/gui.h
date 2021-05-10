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
    
    // Flag that indicates wheter the triangle is being drawn.
    bool drawing;

    // Methods

    /* Check if a textbox in prior or channel matrices was pressed.
	 * Returns true if yes or false otherwise. */
    bool checkTextBoxPressed();

    /* If the user press KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT or KEY_RIGHT, move
	 * the pressed TextBox. */
	void moveAmongTextBoxes();
};

#endif