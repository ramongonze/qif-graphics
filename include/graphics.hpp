#ifndef _graphics
#define _graphics

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../qif/qif.hpp"

// Cartesian plan origin
#define ORIGIN_X -0.1 
#define ORIGIN_Y -0.5

// Number of triangles that make up a circle
#define CIRCLES_SIDES 300

// Prior probability distribution radius
#define PRIOR_RADIUS 0.1f

// Main Window dimensions
#define WINDOW_WIDTH 800	
#define WINDOW_HEIGHT 800

using namespace std;

class Point{
	public:
		long double x;
		long double y;

		Point(){
			this->x = 0.0f;
			this->y = 0.0f;
		}

		Point(long double x, long double y){
			this->x = x;
			this->y = y;
		}
};

// Transforms a prior probability distribution on 3 elements in a barycentric point
void dist2BaryCoord(Distribution &prior, Point &p);

// Transforms a prior probability distribution on 3 elements in a barycentric point.
void dist2BaryCoord(long double x1, long double x2, long double x3, Point &p);

/* Transforms a barycentric coordiante in a probability distribution on 3 elements.
 * The coordinate of p is in screen coordinate.
 * It returns true if a probability distribution was sucefully generated or false
 * otherwise. */
bool baryCoord2Dist(Point p, vector<long double> &prob);

// Euclidian distance between two points
long double euclidianDistance(Point a, Point b);

// Transforms a pixel coordinates (window width and height) into screen coordinates (from (-1,-1) to (1,1))
void pixelCoord2ScreenCoord(double x, double y, Point &p, int window_width, int window_height);

// Transforms a pixel coordinates (window width and height) into screen coordinates (from (-1,-1) to (1,1))
void screenCoord2PixelCoord(double x, double y, Point &p, int window_width, int window_height);

// Creates the array buffer of a circle, including the color as the second attribute
void createCircle(double x, double y, double radius, double r, double g, double b, double circleVertices[6 * (CIRCLES_SIDES+2)]);

#endif