#ifndef _qif_graphics
#define _qif_graphics

#include <vector>
#include "../qif/qif.hpp"
// #include "/home/ramon/raylib/src/raylib.h"
#define RAYGUI_STATIC
#include "raygui.h"
// #define RAYGUI_STATIC
// #include "/home/ramon/raygui/src/raygui.h"

using namespace std;

// Interface dimensions ***************************************************************************/
#define V1(width)  (0.35f *  width) // % window width that divides the matrices and the graphics
#define V2(width)  (0.05f *  width) // Channel and gain matrices position
#define H1(height) (0.04f * height) // Percentage of prior rectangle 
#define H2(height) (0.19f * height) // Percentage of channel rectangle
#define H3(height) (0.58f * height) // Percentage of gain rectangle
#define TV1(width) (((0.50f-V1(width)/width)/2.0f + V1(width)/width) * width)
#define TV2(width) (((0.50f-V1(width)/width)/2.0f + V1(width)/width + 0.25f) * width)
#define TV3(width) (((0.50f-V1(width)/width)/2.0f + V1(width)/width + 0.50f) * width)
#define TH1(height) ( H1(height) + (0.28f * height))
#define TH2(height) (TH1(height) + (0.08f * height))
#define TH3(height) (TH2(height) + (0.50f * height))

// Macros to define an equilateral triangle (its height is 50% of min(window_width, window_height))
#define TRIANGLEV1(width, height) (V1(width) + ((width-V1(width)) - MIN(width,height)/2)/2)
#define TRIANGLEV2(width, height) (TRIANGLEV1(width, height) + MIN(width,height)/4)
#define TRIANGLEV3(width, height) (TRIANGLEV1(width, height) + MIN(width,height)/2)

#define TRIANGLEH2(width, height) (TH1(height) + ((height-TH1(height)) - (MIN(width,height)/2))/2)
#define TRIANGLEH3(width, height) (TRIANGLEH2(width, height) + MIN(width,height)/2)

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