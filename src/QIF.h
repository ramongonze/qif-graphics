#ifndef _QIF
#define _QIF

#include "../qif/qif.h"

// Errors *****************************************************************************************/
#define NO_ERROR		0
#define INVALID_CHANNEL 1
#define INVALID_PRIOR	2 
#define INVALID_VALUE	3 // i.e. "1/$2"

class QIF{
public:
	Hyper hyper;				// Hyper-distribution
	vector<vector<long double>> channel;			// Channel matrix
	vector<long double> prior;	// Prior distribution

	/* Position of circles in the big triangle.
	 * Position  [0]      -> Prior distribution
	 * Positions [1..n-1] -> Posterior distributions */
	vector<Point> circlesPositions;

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

	checkChannel();

	void drawCircles(Colors colors, Layout layout);
};

#endif