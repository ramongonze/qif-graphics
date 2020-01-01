#ifndef _information
#define _information

#include "graphics.h"

class Information{
public:
	Information();

	Hyper hyper;				// Hyper-distribution
	vector<vector<long double>> channel;			// Channel matrix
	vector<long double> prior;	// Prior distribution

	bool hyperReady;  // Flag that indicates if a hyper distribution has been built

	/* Position of circles in the big triangle.
	 * Position  [0]      -> Prior distribution
	 * Positions [1..n-1] -> Posterior distributions */
	vector<Point> circlesPositions;

	/* Check if the numbers or fractions were typed correctly in the prior distribution.
	 * If so, convert text to long double values and add them to this->prior.
	 * Returns NO_ERROR or INVALID_VALUE */
	int checkPriorText(vector<char*> &prior_);

	/* Check if the numbers or fractions were typed correctly in the channel.
	 * If so, conver text to long double values and add them to this->channel.
	 * Returns NO_ERROR or INVALID_VALUE */
	int checkChannelText(vector<vector<char*>> &channel_);

	/* Check if the numbers in prior TextBoxes represent a probability distribution.
	 * Returns NO_ERROR or INVALID_PRIOR */
	int checkPrior();

	/* Check if the numbers in each row in the channel TextBoxes represent a probability distribution.
	 * Returns NO_ERROR or INVALID_CHANNEL */
	int checkChannel();
	
};

#endif