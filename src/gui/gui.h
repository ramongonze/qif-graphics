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

    // Load all characters from custom font
    void readFonts();

    /* Check wheter a textbox in prior was pressed.
	 * Returns true if yes or false otherwise. */
    bool checkPriorTextBoxPressed();

    /* Check wheter a textbox in the current channel was pressed.
	 * Returns true if yes or false otherwise. */
    bool checkChannelTextBoxPressed();

    /* If the user press KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT or KEY_RIGHT, move the pressed TextBox in prior. */
    void moveAmongPriorTextBoxes();

    /* If the user press KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT or KEY_RIGHT, move the pressed TextBox in the current channel. */
    void moveAmongChannelTextBoxes();
    
    /* Update TextBoxes of prior distribution. */
	void updatePriorTextBoxes(Distribution &prior_);

    /* If Update channel textboxes for a given channel. */
	void updateChannelTextBoxes(Channel &channel_, int channelIdx);
    
    /* If a hyper-distributin has been built, update outer and inners TextBoxes;. */
	void updateHyperTextBoxes(Hyper &hyper, int channel, bool ready);

    /* Update rectangle of prior circle label. */
    void updateRectanglePriorCircleLabel(Circle &priorCircle);

    /* Update rectangles of inners circle labels. */
    void updateRectangleInnersCircleLabel(int channel, Circle innersCircles[MAX_CHANNEL_OUTPUTS]);

    // Fonts
    Font defaultFont;
    Font defaultFontBig; // Same as defaultFont but with size 32

    // Check mouse position and manage help messages
    void checkMouseHover(Vector2 mousePosition);
};

#endif