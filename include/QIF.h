#ifndef _QIF
#define _QIF

#include "../qif/qif.hpp"
#include "layout.h"

class QIF{
public:
	Hyper hyper;				// Hyper-distribution
	MatrixLD channel;			// Channel matrix
	vector<long double> prior;	// Prior distribution

	int numOutputs;				// Current number of outputs in the channel matrix. 
	int oldNumOutputs;			// Number of outputs in the previous frame.
	Vector2 channelPanelScroll;
    Rectangle channelPanelRec;
    Rectangle channelPanelContentRec;

	/* Position of circles in the big triangle.
	 * Position  [0]      -> Prior distribution
	 * Positions [1..n-1] -> Posterior distributions */
	vector<Point> circlesPositions;

	MatrixString matricesTexts[QIF_ELEMENTS];
	MatrixRec matricesRectangles[QIF_ELEMENTS];

	void init();	
	void updatePrior(Layout layout);
	void updateChannel(Layout layout);
	void updateHyper(Layout layout);
	void updateStatic(int windowWidth, int windowHeight);
	
	/* Get strings typed by the user in channel, prior and gain matrices and convert them to
	 * numbers.
	 *
	 * Return:
	 *		true: if an error during conversion happened
	 *		false: otherwise
	 */
	int updateMatricesByText();
	void updateMatricesByNumbers();
	void updateMatricesText();
	int update(Layout layout);

	void drawMatrices(Colors colors, Layout layout);
	void drawCircles(Colors colors, Layout layout);
};

#endif