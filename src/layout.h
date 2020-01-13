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
			char *GroupBoxGainText;
			char *GroupBoxVisualizationText;
			char *GroupBoxDrawingText;
		//----------------------------------------------------------------------------------

		// Labels
		//----------------------------------------------------------------------------------
			char *LabelOutputsText;
			char *LabelActionsText;
			char *LabelClickDrawText;
			char *LabelOuterNameText;
			char *LabelPriorCircleText;

			vector<string> LabelGainXText;
			vector<string> LabelGainWText;
			vector<string> LabelChannelXText;
			vector<string> LabelChannelYText;
			vector<string> LabelPriorText;
			vector<string> LabelOuterText;
			vector<string> LabelInnerText;
			vector<string> LabelTriangleText;
		//----------------------------------------------------------------------------------

		// CheckBoxes
		//----------------------------------------------------------------------------------
			char *CheckBoxGainText;
			char *CheckBoxDrawingText;
		//----------------------------------------------------------------------------------

		// StatusBar
		//----------------------------------------------------------------------------------
			string StatusBarDrawingText;
		//----------------------------------------------------------------------------------

		// TextBoxes. Obs: All matrices invert rows and columns (to make easy adding and removing columns).
		//----------------------------------------------------------------------------------
			vector<vector<bool>> TextBoxGainEditMode;
			vector<vector<char*>> TextBoxGainText;

			vector<vector<bool>> TextBoxChannelEditMode;
			vector<vector<char*>> TextBoxChannelText;

			vector<bool> TextBoxPriorEditMode;
			vector<char*> TextBoxPriorText;

			vector<bool> TextBoxOuterEditMode;
			vector<char*> TextBoxOuterText;

			vector<vector<bool>> TextBoxInnersEditMode;
			vector<vector<char*>> TextBoxInnersText;
		//----------------------------------------------------------------------------------

		// DropDowns
		//----------------------------------------------------------------------------------
			char *DropDownLoadText;
			char *DropDownExportText;
			char *DropDownBoxFileText;
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
			Vector2 anchorGain;
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
			bool SpinnerGainEditMode;
			int SpinnerGainValue;
			bool CheckBoxGainChecked;
			bool CheckBoxDrawingChecked;
			bool DropDownLoadEditMode;
			int DropDownLoadActive;
			bool DropDownExportEditMode;
			int DropDownExportActive;
			bool DropDownBoxFileEditMode;
			int DropDownBoxFileActive;
			// Vector2 ScrollPanelChannelScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelChannelBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelChannel
			// Vector2 ScrollPanelGainScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelGainBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelGain
			// Vector2 ScrollPanelPosteriorsScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelPosteriorsBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelPosteriors
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

        vector<Rectangle> recLabelPrior;
        vector<Rectangle> recLabelGainX;
        vector<Rectangle> recLabelGainW;
        vector<Rectangle> recLabelChannelX;
        vector<Rectangle> recLabelChannelY;
        vector<Rectangle> recLabelOuter;
        vector<Rectangle> recLabelInners;
        vector<Rectangle> recLabelTriangle;
        vector<Rectangle> recLabelInnersCircles;

        Rectangle recCheckBoxGain;
        Rectangle recCheckBoxDrawing;

        Rectangle recLine1;

        Rectangle recStatusBar;

        vector<Rectangle> recTextBoxPrior;
        vector<vector<Rectangle>> recTextBoxGain;
        vector<vector<Rectangle>> recTextBoxChannel;
        vector<Rectangle> recTextBoxOuter;
        vector<vector<Rectangle>> recTextBoxInners;

        Rectangle recDropDownFile;
        Rectangle recDropDownLoad;
        Rectangle recDropDownExport;
	//--------------------------------------------------------------------------------------

    Font alternativeFont;

    // Initialize all attribute values
	void init();

	/* If SpinnerChannel has been changed, update the TextBoxChannel matrices.
	 * It is called if L->SpinnerChannelValue != L->recTextBoxChannel.size(). */
	void updateChannel();

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