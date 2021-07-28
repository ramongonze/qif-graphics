#ifndef _gui
#define _gui

#include "guimenu.h"
#include "guiprior.h"
#include "guichannel.h"
#include "guiposteriors.h"
#include "guivisualization.h"
#include <fstream>
#include <cmath>

#define SCREEN_WIDTH 1110
#define SCREEN_HEIGHT 795

#define HELP_MSG_BUTTON_PRIOR 0
#define HELP_MSG_BUTTON_CHANNEL 1
#define HELP_MSG_BUTTON_DRAW 2

class Gui{
private:
    /* data */
public:
    Gui();

    //------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------

    GuiMenu menu;
    GuiPrior prior;
    GuiChannel channel;
    GuiPosteriors posteriors;
    GuiVisualization visualization;
    
    // Flag that indicates wheter the triangle is being drawn.
    bool drawing;

    // Flags used to show help messages
    bool helpMessagesActive[3];
    char helpMessages[3][CHAR_BUFFER_SIZE*2];

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    void readFonts();

    /* Check if a textbox in prior or channel matrices was pressed.
	 * Returns true if yes or false otherwise. */
    bool checkTextBoxPressed();

    /* If the user press KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT or KEY_RIGHT, move
	 * the pressed TextBox. */
	void moveAmongTextBoxes();

	/* Update TextBoxes of prior distribution .*/
	void updatePrior(Distribution &prior_, Circle &priorCircle);

    void updatePriorRectangle(Circle &priorCircle);

    /* If a hyper-distributin has been built, update outer and inners TextBoxes;.
	 * 
	 * @Parameters:
	 * 		hyper: Hyper-distribution;
	 */
	void updatePosteriors(Hyper &hyper, Circle innersCircles[MAX_CHANNEL_OUTPUTS]);

    // Fonts
    Font defaultFont;
    Font defaultFontBig; // Same as defaultFont but with size 32

    // Check mouse position and manage help messages
    void checkMouseHover(Vector2 mousePosition);
};

#endif