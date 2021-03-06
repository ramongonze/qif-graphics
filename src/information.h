#ifndef _information
#define _information

#include "graphics.h"
#include <exception>
#include <algorithm> // std::random_shuffle
#include <ctime> // std::time
#include <cstdlib> // std::srand

class Information{
public:
	Information();

	Hyper hyper;				// Hyper-distribution
	vector<vector<long double>> channel;			// Channel matrix
	vector<long double> prior;	// Prior distribution

	bool hyperReady;  // Flag that indicates wheter a hyper distribution has been built.
	bool mouseClickedOnPrior; // Flag that indicates wheter the mouse was clicked in the previous frame on the prior circle.

	Circle priorCircle;
	vector<Circle> innersCircles;

	/* Check if the numbers or fractions were typed correctly in the prior distribution.
	 * If so, convert text to long double values and add them to this->prior.
	 * Returns NO_ERROR or INVALID_VALUE */
	int checkPriorText(vector<char*> &prior_);

	/* Check if the numbers or fractions were typed correctly in the channel.
	 * If so, conver text to long double values and add them to this->channel.
	 * Returns NO_ERROR or INVALID_VALUE */
	int checkChannelText(vector<vector<char*>> &channel_);

	/* Create circle points and radius.
	 * It asssumes that the hyper distribution attribute has already been built. */
	void buildCircles(vector<Vector2> &TrianglePoints);

	/* Given three points, returns 0 if the orientation is colinear, 1 if it is
	 * clock wise or 2 if it is counterclock wise. */
	int orientation(Point p1, Point p2, Point p3);

	/* Point intersection between lines AB and CD. */
	Point pointIntersection(Point A, Point B, Point C, Point D);

	/* Adjust the new prior distribution. If the mouse position is outside the triangle,
	 * moves the prior circle to the closest point of an edge from the triangle. */
	Point adjustPrior(vector<Vector2> &TrianglePoints, Vector2 mouse);

	/* Update the hyper distribution if the user moves the prior distribution
     * This function assumes that the hyper distribution has already been built. 
	 *
	 * @Parameters:
	 *		TrianglePoints: From Layout object.
	 *
     */
	void updateHyper(vector<Vector2> &TrianglePoints);

	/* Generates a new random prior and keeps it in attribute 'prior'. */
	void newRandomPrior();

	/* Generates a new random channel and keeps it in attribute 'channel'.
	 *
	 * @Parameters:
	 *		num_out: Number of outputs in L->TextBoxesChannelText.
	*/
	void newRandomChannel(int num_out);
};

#endif