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

    //------------------------------------------------------------------------------------
    // Attributes
    //------------------------------------------------------------------------------------

    // Const text
    char panPriorTxt[CHAR_BUFFER_SIZE];
    char buttonRandTxt[CHAR_BUFFER_SIZE];
    string labelPriorTxt[NUMBER_SECRETS];

    // Define anchors
    Vector2 anchorPrior;

    // Define controls variables
    bool tBoxPriorEdit[NUMBER_SECRETS];
    char tBoxPriorTxt[NUMBER_SECRETS][CHAR_BUFFER_SIZE];

    // Define controls rectangles
    Rectangle recPan;
    Rectangle recTitle;
    Rectangle recContent;
    Rectangle recButtonRand;
    Rectangle recLabel[NUMBER_SECRETS];
    Rectangle recTBox[NUMBER_SECRETS];

    //------------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------------

    // Copy the values of a prior array to another
    static void copyPrior(char origin[NUMBER_SECRETS][CHAR_BUFFER_SIZE], char dest[NUMBER_SECRETS][CHAR_BUFFER_SIZE]){
        for(int i = 0; i < NUMBER_SECRETS; i++){
            strcpy(dest[i], origin[i]);
        }
    }
};

#endif