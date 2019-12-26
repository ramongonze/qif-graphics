#ifndef _qifLayout
#define _qifLayout

#include "raylib.h"
#include <vector>
#include <string>

using namespace std;

class Layout{
public:
	Layout();
	~Layout();

	// Controls initialization
	//----------------------------------------------------------------------------------
		// GroupBoxes
		//----------------------------------------------------------------------------------
			char *GroupBoxPriorText = "Prior";    // GROUPBOX: GroupBoxPrior
			char *GroupBoxChannelText = "Channel";    // GROUPBOX: GroupBoxChannel
			char *GroupBoxPosteriorsText = "Posteriors";    // GROUPBOX: GroupBoxPosteriors
			char *GroupBoxGainText = "Gain function";    // GROUPBOX: GroupBoxGain
			char *GroupBoxVisualizationText = "Visualization";    // GROUPBOX: GroupBoxVisualization
			char *GroupBoxDrawingText = "Drawing";    // GROUPBOX: GroupBoxDrawing
		//----------------------------------------------------------------------------------

		// Labels
		//----------------------------------------------------------------------------------
			char *LabelOutputsText = "Outputs";    // LABEL: LabelOutputs
			char *LabelActionsText = "Actions";    // LABEL: LabelActions
			char *LabelClickDrawText = "Click to draw:";    // LABEL: LabelClickDraw
			char *LabelOuterNameText = "Outer";    // LABEL: LabelOuter
			vector<string> LabelGainXText = vector<string>({"X1", "X2", "X3"}); 
			vector<string> LabelGainWText = vector<string>({"W1", "W2", "W3"});
			vector<string> LabelChannelXText = vector<string>({"X1", "X2", "X3"});
			vector<string> LabelChannelYText = vector<string>({"Y1", "Y2", "Y3"});
			vector<string> LabelPriorText = vector<string>({"X1", "X2", "X3"});
			vector<string> LabelOuterText = vector<string>({"I1", "I2", "I3"});
			vector<string> LabelInnerText = vector<string>({"X1", "X2", "X3"});
		//----------------------------------------------------------------------------------

		// CheckBoxes
		//----------------------------------------------------------------------------------
			char *CheckBoxGainText = "Enable";    // CHECKBOXEX: CheckBoxGain
			char *CheckBoxDrawingText = "";    // CHECKBOXEX: CheckBoxDrawing
		//----------------------------------------------------------------------------------

		// StatusBar
		//----------------------------------------------------------------------------------
			char *StatusBarDrawingText = "Status";    // STATUSBAR: StatusBarDrawing
		//----------------------------------------------------------------------------------

		// TextBoxes. Obs: All matrices invert rows and columns (to make easy adding and removing columns).
		//----------------------------------------------------------------------------------
			vector<vector<bool> > TextBoxGainEditMode = vector<vector<bool> >(3, vector<bool>(3, false));
			vector<vector<string> > TextBoxGainText = vector<vector<string> >(3, vector<string>(3, "0"));

			vector<vector<bool> > TextBoxChannelEditMode = vector<vector<bool> >(3, vector<bool>(3, false));
			vector<vector<string> > TextBoxChannelText = vector<vector<string> >(3, vector<string>(3, "0"));

			vector<bool> TextBoxPriorEditMode = vector<bool>(3, false);
			vector<string> TextBoxPriorText = vector<string>(3, "0");

			vector<bool> TextBoxOuterEditMode = vector<bool>(3, false);
			vector<string> TextBoxOuterText = vector<string>(3, "0");

			vector<vector<bool> > TextBoxInnersEditMode = vector<vector<bool> >(3, vector<bool>(3, false));
			vector<vector<string> > TextBoxInnersText = vector<vector<string> >(3, vector<string>(3, "0"));
		//----------------------------------------------------------------------------------

		// DropDowns
		//----------------------------------------------------------------------------------
			char *DropDownLoadText = "Load;Prior;Channel;Gain function";    // DROPDOWNBOX: DropDownLoad
			char *DropDownExportText = "Export;Prior;Channel;Posteriors;Gain function";    // DROPDOWNBOX: DropDownExport
			char *DropDownBoxFileText = "File;Open;Save;Save as";    // DROPDOWNBOX: DropDownBoxFile
		//----------------------------------------------------------------------------------

		// Define anchors
		//----------------------------------------------------------------------------------
			Vector2 anchorGain = { 95, 345 };            // ANCHOR ID:1
			Vector2 anchorChannel = { 95, 115 };            // ANCHOR ID:2
			Vector2 anchorPrior = { 400, 95 };            // ANCHOR ID:3
			Vector2 anchorDrawing = { 150, 570 };            // ANCHOR ID:4
			Vector2 anchorOuter = { 535, 75 };            // ANCHOR ID:5
			Vector2 anchorInners = { 535, 125 };            // ANCHOR ID:6
			Vector2 anchorVisualization = { 365, 270 };            // ANCHOR ID:7
		//----------------------------------------------------------------------------------

		// Define controls variables
		//----------------------------------------------------------------------------------
			bool SpinnerChannelEditMode = false;
			int SpinnerChannelValue = 0;            // Spinner: SpinnerChannel
			bool SpinnerGainEditMode = false;
			int SpinnerGainValue = 0;            // Spinner: SpinnerGain
			bool CheckBoxGainChecked = false;            // CheckBoxEx: CheckBoxGain
			bool CheckBoxDrawingChecked = false;            // CheckBoxEx: CheckBoxDrawing
			bool DropDownLoadEditMode = false;
			int DropDownLoadActive = 0;            // DropdownBox: DropDownLoad
			bool DropDownExportEditMode = false;
			int DropDownExportActive = 0;            // DropdownBox: DropDownExport
			bool DropDownBoxFileEditMode = false;
			int DropDownBoxFileActive = 0;            // DropdownBox: DropDownBoxFile
			// Vector2 ScrollPanelChannelScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelChannelBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelChannel
			// Vector2 ScrollPanelGainScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelGainBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelGain
			// Vector2 ScrollPanelPosteriorsScrollOffset = { 0, 0 };
			// Vector2 ScrollPanelPosteriorsBoundsOffset = { 0, 0 };            // ScrollPanel: ScrollPanelPosteriors
		//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------

	// Rectangles

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

	        vector<Rectangle> recLabelPrior;

	        vector<Rectangle> recLabelGainX;
	        vector<Rectangle> recLabelGainW;

	        vector<Rectangle> recLabelChannelX;
	        vector<Rectangle> recLabelChannelY;

	        vector<Rectangle> recLabelOuter;

	        vector<Rectangle> recLabelInners;

	        Rectangle recCheckBoxGain;
	        Rectangle recCheckBoxDrawing;

	        Rectangle recLine1;

	        Rectangle recStatusBar;

	        vector<Rectangle> recTextBoxPrior;
	        vector<vector<Rectangle> > recTextBoxGain;
	        vector<vector<Rectangle> > recTextBoxChannel;
	        vector<Rectangle> recTextBoxOuter;
	        vector<vector<Rectangle> > recTextBoxInners;

	        Rectangle recDropDownFile;
	        Rectangle recDropDownLoad;
	        Rectangle recDropDownExport;
	//--------------------------------------------------------------------------------------

	void init();

};

#endif