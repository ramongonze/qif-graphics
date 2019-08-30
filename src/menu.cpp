#include "../include/menu.hpp"

void Menu::init(){
	menuActiveOption 	 = 0;
    exitSelectMenuWindow = true;
    menuDropEditMode 	 = false;
    dragMenuWindow 		 = false;

	staticRectangles[MENU]          	 = (Rectangle){0.0f, 0.0f, (float)WINDOW_WIDTH, (float)H1(WINDOW_HEIGHT)+1};
    staticRectangles[MENU_DROPDOWN] 	 = (Rectangle){1.0f, 1.0f, 180.0f, (float)(H1(WINDOW_HEIGHT)-2)};
    staticRectangles[MENU_WINDOW] 		 = (Rectangle){WINDOW_WIDTH/2.0f - 200, WINDOW_HEIGHT/2.0f - 100, 400.0f, 200.0f}; // Reset window position
    staticRectangles[MENU_WINDOW_HEADER] = (Rectangle){WINDOW_WIDTH/2.0f - 200, WINDOW_HEIGHT/2.0f - 100, 400.0f, 30.0f}; // Reset window position
}

void Menu::update(Layout layout){
	if(menuActiveOption > 0){
		exitSelectMenuWindow = false;
	}

	if(!exitSelectMenuWindow){
		if(!dragMenuWindow && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(layout.mousePosition, staticRectangles[MENU_WINDOW_HEADER])){
			dragMenuWindow = true;
			offset.x = layout.mousePosition.x - staticRectangles[MENU_WINDOW].x;
			offset.y = layout.mousePosition.y - staticRectangles[MENU_WINDOW].y;
		}

		if(dragMenuWindow){
			staticRectangles[MENU_WINDOW].x = layout.mousePosition.x - offset.x;
			staticRectangles[MENU_WINDOW].y = layout.mousePosition.y - offset.y;
			staticRectangles[MENU_WINDOW_HEADER].x = staticRectangles[MENU_WINDOW].x;
			staticRectangles[MENU_WINDOW_HEADER].y = staticRectangles[MENU_WINDOW].y;
			if(IsMouseButtonUp(MOUSE_LEFT_BUTTON)) dragMenuWindow = false;
		}
	}else{
		staticRectangles[MENU_WINDOW] = {layout.windowWidth/2.0f - 200, layout.windowHeight/2.0f - 100, 400.0f, 200.0f}; // Reset window position
		staticRectangles[MENU_WINDOW_HEADER].x = staticRectangles[MENU_WINDOW].x;
		staticRectangles[MENU_WINDOW_HEADER].y = staticRectangles[MENU_WINDOW].y;
		staticRectangles[MENU_WINDOW_HEADER].width = staticRectangles[MENU_WINDOW].width;
		staticRectangles[MENU_WINDOW_HEADER].height = 30;
	}

	staticRectangles[MENU]            = (Rectangle){0.0f, 0.0f, (float)layout.windowWidth, (float)H1(layout.windowHeight)+1};
    staticRectangles[MENU_DROPDOWN]   = (Rectangle){1.0f, 1.0f, 180.0f, (float)(H1(layout.windowHeight)-2)};
}

void Menu::draw(Colors colors){
	DrawRectangleRec(staticRectangles[MENU], colors.recColor);
	DrawRectangleLinesEx(staticRectangles[MENU], 1, colors.recBorderColor);

	GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
	if(GuiDropdownBox(staticRectangles[MENU_DROPDOWN], "Menu;Select prior;Select channel;Select gain function", &menuActiveOption, menuDropEditMode)){
		menuDropEditMode = !menuDropEditMode;
	}

	if(!exitSelectMenuWindow){
		exitSelectMenuWindow = GuiWindowBox(staticRectangles[MENU_WINDOW], "Select a file");
		if(exitSelectMenuWindow) menuActiveOption = 0;
	}

}