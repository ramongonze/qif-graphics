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

    // Fonts
    Font defaultFont;
    Font defaultFontBig; // Same as defaultFont but with size 32
    
    bool drawing; // Flag that indicates wheter the triangle is being drawn.
    bool helpMessagesActive[3]; // Flags used to show help messages
    bool showLabels; // Flag used in visualization to show or not circles labels
    bool showCHull; // Flag used in visualization to show or convex hull of inners

    char helpMessages[3][CHAR_BUFFER_SIZE*2];

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    // Load all characters from custom font
    void readFonts();

    /* Check whether a textbox in prior was pressed.
	 * Returns true if yes or false otherwise. */
    bool checkPriorTBoxPressed();

    /* Check wheter a textbox in the current channel was pressed.
	 * Returns true if yes or false otherwise. */
    bool checkChannelTBoxPressed();

    /* Chechk whether delta or epsilon textbox (of MODE_DP) was pressed.
     * Returns true if yes or false otherwise. */
    bool checkEpDeltaTBoxPressed();
    
    /* Chechk whether alpha textbox was pressed.
     * Returns true if yes or false otherwise. */
    bool checkAlphaTBoxPressed();

    /* If the user press KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT or KEY_RIGHT, move the pressed TextBox in prior. */
    void movePriorTBoxes();

    /* If the user press KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT or KEY_RIGHT, move the pressed TextBox in the current channel. */
    void moveChannelTBoxes();
    
    /* Update TextBoxes of prior distribution. */
	void updatePriorTBoxes(Distribution &prior_);

    /* If Update channel textboxes for a given channel. */
	void updateChannelTBoxes(Channel &channel_, int channelIdx);
    
    /* If a hyper-distributin has been built, update outer and inners TextBoxes;. */
	void updateHyperTBoxes(Hyper &hyper, int channel, bool ready);

    /* Update rectangle of prior circle label. */
    void updateRecPriorCircLabel(Circle &priorCircle);

    /* Update rectangles of inners circle labels. */
    void updateRecInnersCircLabel(int channel, Circle innersCircles[MAX_CHANNEL_OUTPUTS]);

    // Check mouse position and manage help messages
    void checkMouseHover(Vector2 mousePosition);
};

#endif