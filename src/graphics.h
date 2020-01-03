#ifndef _qif_graphics
#define _qif_graphics

#include "../qif/qif.h"

using namespace std;

// Errors *****************************************************************************************/
#define NO_ERROR		0
#define INVALID_CHANNEL 1
#define INVALID_PRIOR	2 
#define INVALID_VALUE	3 // i.e. "1/$2"

// Settings
#define PROB_PRECISION 3 // Precision of float numbers (# digits after .)

// Prior probability distribution radius **********************************************************/
#define PRIOR_RADIUS 40

#define MIN(A,B) ((A < B) ? (A) : (B))

class Point{
	public:
		long double x;
		long double y;

		Point();
		Point(long double x, long double y);
};

/* Transforms a probability distribution on 3 elements in a barycentric coordinate
 * Parameters:
 * 		prior: Probability distribution
 *
 * Return: A 'Point' structure containing a barycentric coordinate.
 */
Point dist2Bary(Distribution &prior);

/* Transforms a probability distribution on 3 elements in a barycentric coordinate
 * Parameters:
 * 		x1: Probability of the first element
 * 		x2: Probability of the second element
 * 		x3: Probability of the third element
 *
 * Return: A 'Point' structure containing a barycentric coordinate.
 */
Point dist2Bary(long double x1, long double x2, long double x3);

/* Transforms a barycentric coordiante in a probability distribution on 3 elements.
 * Parameters:
 *		p: 	  Point containing a barycentric coordinate
 *		prob: A vector which will receive the output probability distribution
 *
 * Return: true if a probability distribution was succesfully generated or false otherwise.
 */
bool bary2Dist(Point p, vector<long double> &prob);

// Euclidian distance between two points
long double euclidianDistance(Point a, Point b);

/* Transforms a pixel coordinate in barycentric coordinate
 * Parameters:
 *		x: value (in pixels) in x-axis
 *		y: value (in pixels) in y-axis
 *
 * Return: A 'Point' structure containing a barycentric coordinate.
 */
Point pixel2Bary(double x, double y, int window_width, int window_height);

/* Transforms barycentric coordinate in pixel coordinate
 * Parameters:
 *		x: value (in barycentric coordinate) in x-axis
 *		y: value (in barycentric coordinate) in y-axis
 *
 * Return: A 'Point' structure containing a barycentric coordinate.
 */
Point bary2Pixel(double x, double y, int window_width, int window_height);

#endif