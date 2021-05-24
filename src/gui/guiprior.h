#ifndef _guiprior
#define _guiprior

#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include "../../libs/raylib/src/raylib.h"
#include "../graphics.h"
using namespace std;

class GuiPrior{
private:
    /* data */
public:
    GuiPrior();

    // Const text
    char *panelPriorText;
    char *buttonRandomText;
    string LabelPriorText[NUMBER_SECRETS];

    // Define anchors
    Vector2 AnchorPrior;

    // Define controls variables
    bool TextBoxPriorEditMode[NUMBER_SECRETS];
    char* TextBoxPriorText[NUMBER_SECRETS];

    // Define controls rectangles
    Rectangle layoutRecsPanel;
    Rectangle layoutRecsTitle;
    Rectangle layoutRecsContent;
    Rectangle layoutRecsButtonRandom;
    Rectangle layoutRecsLabel[NUMBER_SECRETS];
    Rectangle layoutRecsTextBox[NUMBER_SECRETS];

    // Copy the values of a prior array to another
    static void copyPrior(char* origin[NUMBER_SECRETS], char* dest[NUMBER_SECRETS]){
        for(int i = 0; i < NUMBER_SECRETS; i++){
            strcpy(dest[i], origin[i]);
        }
    }
};

#endif