#ifndef _qifLayout
#define _qifLayout

#include "raylib.h"
#include "graphics.h"
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <string.h>

#define OFFSET_X -45
#define OFFSET_Y 0

using namespace std;

class Layout{
public:
	Layout();
	// Controls initialization
	//----------------------------------------------------------------------------------
		// GroupBoxes
		//----------------------------------------------------------------------------------
			char *GroupBoxPriorText;
			char *GroupBoxChannelText;
			char *GroupBoxPosteriorsText;
			// char *GroupBoxGainText;
			char *GroupBoxVisualizationText;
			char *GroupBoxDrawingText;
		//----------------------------------------------------------------------------------

		// Labels
		//----------------------------------------------------------------------------------
			char *LabelOutputsText;
			// char *LabelActionsText;
			char *LabelClickDrawText;
			char *LabelOuterNameText;
			char *LabelPriorCircleText;
			char *LabelTitleText;

			// vector<string> LabelGainXText;
			// vector<string> LabelGainWText;
			vector<string> LabelChannelXText;
			vector<string> LabelChannelYText;
			vector<string> LabelPriorText;
			vector<string> LabelOuterText;
			vector<string> LabelInnerText;
			vector<string> LabelTriangleText;
		//----------------------------------------------------------------------------------

		// Buttons
		//--------------------------------------------------------------------------------------
			char *ButtonPriorText;
			char *ButtonChannelText;
			bool ButtonPriorClicked;
			bool ButtonChannelClicked;
		//--------------------------------------------------------------------------------------

		// CheckBoxes
		//----------------------------------------------------------------------------------
			// char *CheckBoxGainText;
			char *CheckBoxDrawingText;
		//----------------------------------------------------------------------------------

		// StatusBar
		//----------------------------------------------------------------------------------
			string StatusBarDrawingText;
		//----------------------------------------------------------------------------------

		// TextBoxes. Obs: All matrices invert rows and columns (to make easy adding and removing columns).
		//----------------------------------------------------------------------------------
			// vector<vector<bool>> TextBoxGainEditMode;
			// vector<vector<char*>> TextBoxGainText;

			vector<vector<bool>> TextBoxChannelEditMode;
			vector<vector<char*>> TextBoxChannelText;

			vector<bool> TextBoxPriorEditMode;
			vector<char*> TextBoxPriorText;

			vector<bool> TextBoxOuterEditMode;
			vector<char*> TextBoxOuterText;

			vector<vector<bool>> TextBoxInnersEditMode;
			vector<vector<char*>> TextBoxInnersText;

			char *TextBoxHelpPrior;
			char *TextBoxHelpChannel;
			char *TextBoxHelpPosteriors;
			char *TextBoxHelpVisualization;
			// char *TextBoxHelpGain;
		//----------------------------------------------------------------------------------

		/* Triangle
		 *       v0
		 *       /\
		 *   v1 /__\ v2 */
		//----------------------------------------------------------------------------------
			vector<Vector2> TrianglePoints;
		//----------------------------------------------------------------------------------


		// Define anchors
		//----------------------------------------------------------------------------------
			// Vector2 anchorGain;
			Vector2 anchorChannel;
			Vector2 anchorPrior;
			Vector2 anchorDrawing;
			Vector2 anchorOuter;
			Vector2 anchorInners;
			Vector2 anchorVisualization;
		//----------------------------------------------------------------------------------

		// Define controls variables
		//----------------------------------------------------------------------------------
			bool SpinnerChannelEditMode;
			int SpinnerChannelValue;
			// bool SpinnerGainEditMode;
			// int SpinnerGainValue;
			// bool CheckBoxGainChecked;
			bool CheckBoxDrawingChecked;
		//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------

	// Rectangles
	//----------------------------------------------------------------------------------
        Rectangle recPanelMenu;
        Rectangle recPanelBody;

        Rectangle recGroupBoxPrior;
        Rectangle recGroupBoxChannel;
        Rectangle recGroupBoxPosteriors;
        Rectangle recGroupBoxGain;
        Rectangle recGroupBoxVisualization;
        Rectangle recGroupBoxDrawing;

        Rectangle recSpinnerChannel;
        Rectangle recSpinnerGain;

        Rectangle recLabelOutputs;
        Rectangle recLabelActions;
        Rectangle recLabelClickDraw;
        Rectangle recLabelOuterName;
        Rectangle recLabelPriorCircle;
        Rectangle recLabelTitle;

        vector<Rectangle> recLabelPrior;
        vector<Rectangle> recLabelGainX;
        vector<Rectangle> recLabelGainW;
        vector<Rectangle> recLabelChannelX;
        vector<Rectangle> recLabelChannelY;
        vector<Rectangle> recLabelOuter;
        vector<Rectangle> recLabelInners;
        vector<Rectangle> recLabelTriangle;
        vector<Rectangle> recLabelInnersCircles;

        Rectangle recButtonPrior;
        Rectangle recButtonChannel;

        Rectangle recCheckBoxGain;
        Rectangle recCheckBoxDrawing;

        Rectangle recLine1;

        Rectangle recStatusBar;

        vector<Rectangle> recTextBoxPrior;
        vector<vector<Rectangle>> recTextBoxGain;
        vector<vector<Rectangle>> recTextBoxChannel;
        vector<Rectangle> recTextBoxOuter;
        vector<vector<Rectangle>> recTextBoxInners;

		Rectangle recTextBoxHelpPrior;
        Rectangle recTextBoxHelpChannel;
		Rectangle recTextBoxHelpPosteriors;
		Rectangle recTextBoxHelpVisualization;
		// Rectangle recTextBoxHelpGain;
		
		Rectangle recTextBoxHelpTextPrior;
		Rectangle recTextBoxHelpTextChannel;
		Rectangle recTextBoxHelpTextPosteriors;
		Rectangle recTextBoxHelpTextVisualization;
		// Rectangle recTextBoxHelpTextGain;

	//--------------------------------------------------------------------------------------

    Font alternativeFont;

    // Initialize all attribute values
	void init();

	/* If SpinnerChannel has been changed, update the TextBoxChannel matrices.
	 * It is called if L->SpinnerChannelValue != L->recTextBoxChannel.size(). */
	void updateChannelBySpinner();

	/* Given a channel, update the channel matrix text boxes. */
	void updateChannelTextBoxes(vector<vector<long double>> &channel);

	/* If a hyper-distributin has been built, update outer and inners TextBoxes;.
	 * 
	 * @Parameters:
	 * 		hyper: Hyper-distribution;
	 *  	onlyText: true if only the text must be updated or false if the layout
	 *				  (rectangles, labels, ...) must be updated.
	 */
	void updatePosteriors(Hyper &hyper, vector<Circle> &innersCircles, bool onlyText);

	/* Update TextBoxes of prior distribution .*/
	void updatePrior(Distribution &prior, Circle &priorCircle);

	/* Check if a textbox in prior, channel or gain matrices has been pressed.
	 * Returns true if yes or false otherwise. */
	bool checkTextBoxPressed();

	/* If the user press KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT or KEY_RIGHT, move
	 * the pressed TextBox. */
	void moveAmongTextBoxes();
};

#endif