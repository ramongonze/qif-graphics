#ifndef _qif_layout
#define _qif_layout

#include "config.h"

class Layout{
public:
	int windowWidth;
	int windowHeight;
	float headerFontSize;

	Font mainFont;

	Vector2 mousePosition;	// Mouse position in pixels
	Vector2 headersPos[5];  // Header text position
	Vector2 trianglePos[3]; // Triangle points positions
	Vector2 triangleTextPos[3]; // Position of texts 'X1', 'X2' and 'X3'

	Rectangle staticRectangles[GUI_ELEMENTS];
    MatrixRec matricesRectangles[GUI_ELEMENTS];
    MatrixString matricesTexts[GUI_ELEMENTS];

    void init();
    void update(int windowWidth, int windowHeight);
    void draw(Colors colors);
};

#endif