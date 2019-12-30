/**************************************************************************************************
*
* QIF-Graphics configuration
* This header contains definitions about graphics constants, update values and other.
* It also include all libraries required for qif-graphics.
*
* --------- LAYOUT ---------
*
* Left menu:
* 
* Remaining percentage: 1%
*
*      ----------------------------------------------------------------            _
*      | Menu bar : 4%                                                |            _| 4%
* [H1] | ------------------------|------------------------------------| _          _
*      | Prior distribution: 15% |    INNNERS      |           |      | _| 28%      |
*      |  |                      |------------------------------------|- - -[TH1]   |
* [H2] | ------------------------|     |           |           |      | _| 8%       |
*      | Channel: 40%            |                / \  - - - - - - - -|-_- -[TH2]   |
*      |                         |     |         / | \         |      |  |          |
*      |  |                      |              /     \               |  |          | 
*      |                         |     |       /   |   \       |      |  |          | 96%
*      |  |                      |            /         \             |  | 50%      |
* [H3] | ------------------------| [H3]|     /     |     \     |      |  |          |
*      | Gain Function: 40%      |          /             \           | _|          |
*      |                         |     |   /_______|_______\- -|- - - | _   [TH3]   |
*      |  |                      |                                    |  |          |
*      |                         |     |           |           |      |  | 10%      |
*      |__|______________________|[7,5%][_________50%__________][7,5%]| _|         _|
*        [V2]                  [V1]   [TV1]      [TV2]       [TV3]
*      |_________________________|____________________________________|
*                   35%                             65%
* 
* PS: The percentages in the interface are realted to the screen width and height.
*
**************************************************************************************************/
#ifndef _qif_config
#define _qif_config

// Standard libraries ****************************************************************************/
#include <vector>

// QIF Library ************************************************************************************/
#include "graphics.h"

// Window configuration ***************************************************************************/
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 900

// Typedefs ***************************************************************************************/
typedef vector<vector<long double>> MatrixLD;	// 2 dimensional matrix
typedef vector<vector<Rectangle>> MatrixRec;	// 2 dimensional matrix
typedef vector<vector<string>> MatrixString;	// 2 dimensional matrix

// Errors *****************************************************************************************/
#define INVALID_CHANNEL 1
#define INVALID_PRIOR	2 
#define INVALID_VALUE	3 // i.e. "1/$2"

// Rectangle array indices ************************************************************************/
#define QIF_ELEMENTS		7 // Number of QIF elements in rectangles and text matrices
#define PRIOR              	0
#define CHANNEL            	1
#define CHANNEL_SPINNER    	2
#define OUTER              	3
#define INNERS             	4
#define GAIN               	5
#define GAIN_SPINNER 		6

#define GUI_ELEMENTS		(QIF_ELEMENTS + 1)
#define DRAW_CHECK_BOX     	7

#define MENU_ELEMENTS		4
#define MENU               	0
#define MENU_DROPDOWN      	1
#define MENU_WINDOW        	2
#define MENU_WINDOW_HEADER 	3

// Layout constants *******************************************************************************/
#define PROB_PRECISION    30 	// Precision of float numbers (2 char from integer part and '.')
#define MIN_WIDTH        960 	// Min window width
#define MIN_HEIGHT 		 700 	// Min window height
#define MAX_OUTPUTS 	  20 	// Max number of channel outputs
#define MAX_ACTIONS 	  20 	// Max number of actions a gain function can keep
#define MAX_BUFFER 		 100 	// Max number of characters in an input box
#define BOX_WIDTH		  50 	// Matrices input text width
#define BOX_HEIGHT 		  30 	// Matrices input text height
#define LABEL_X_HOR_GAP  -30 	// Matrices label horizontal gap to boxes (X1, X2, X3)
#define LABEL_X_VER_GAP   +5 	// Matrices label horizontal gap to boxes (Y1, Y2, ...)
#define LABEL_Y_HOR_GAP  +15 	// Matrices label vertical gap to boxes (X1, X2, X3)
#define LABEL_Y_VER_GAP  -25 	// Matrices label vertical gap to boxes (Y1, Y2, ...)
#define BOX_HOR_GAP 	  10 	// Horizontal gap between two boxes in a matrix
#define BOX_VER_GAP 	  10 	// Vertical gap between two boxes in a matrix

// Classes ****************************************************************************************/
class Colors{
public:
	Colors(int i);

	Color priorColor;           // Prior circle color
	Color priorBorderColor;     // Prior circle border color
	Color posteriorColor;       // Posterior circle color
	Color posteriorBorderColor; // Posterior circle border color 
	Color recColor;             // Rectangles color
	Color recBorderColor;       // Rectangle border color	
};

#endif