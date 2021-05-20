#ifndef _gui
#define _gui

#include "guimenu.h"
#include "guiprior.h"
#include "guichannel.h"
#include "guiposteriors.h"
#include "guivisualization.h"
#include <fstream>
#include <cmath>

#define SCREEN_WIDTH 1120
#define SCREEN_HEIGHT 775

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

	/* Update TextBoxes of prior distribution .*/
	void updatePrior(Distribution &prior, Circle &priorCircle);

    void updatePriorRectangle(Circle &priorCircle);

    /* If a hyper-distributin has been built, update outer and inners TextBoxes;.
	 * 
	 * @Parameters:
	 * 		hyper: Hyper-distribution;
	 */
	void updatePosteriors(Hyper &hyper, vector<Circle> &innersCircles);
};

#endif