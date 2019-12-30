#ifndef _qif_menu
#define _qif_menu

#include "layout.h"

class Menu{
public:
	int menuActiveOption;
    bool exitSelectMenuWindow;
    bool menuDropEditMode;
    bool dragMenuWindow;
    Vector2 offset;

	Rectangle staticRectangles[MENU_ELEMENTS];

	void init();
	void update(Layout layout);
	void draw(Colors colors);
};

#endif