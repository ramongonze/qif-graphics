#ifndef _graphics
#define _graphics

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../qif/qif.hpp"

// Main Window dimensions
#define WINDOW_WIDTH 1024	
#define WINDOW_HEIGHT 768

// Cartesian plan origin
#define ORIGIN_X 0.45f*WINDOW_WIDTH
#define ORIGIN_Y 0.90f*WINDOW_HEIGHT

// Number of triangles that make up a circle
#define CIRCLES_SIDES 300

// Prior probability distribution radius
#define PRIOR_RADIUS 40

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
void dist2Bary(Distribution &prior, Point &p);

// Transforms a prior probability distribution on 3 elements in a barycentric point.
void dist2Bary(long double x1, long double x2, long double x3, Point &p);

/* Transforms a barycentric coordiante in a probability distribution on 3 elements.
 * The coordinate of p is in screen coordinate.
 * It returns true if a probability distribution was sucefully generated or false
 * otherwise. */
bool bary2Dist(Point p, vector<long double> &prob);

// Euclidian distance between two points
long double euclidianDistance(Point a, Point b);

// Transforms a pixel coordinates (window width and height) into screen coordinates (from (-1,-1) to (1,1))
void pixel2Bary(double x, double y, Point &p, int window_width, int window_height);

// Transforms barycentric coordinate (x, y in [0,1]) into pixel coordinates (window width and height)
void bary2Pixel(double x, double y, Point &p, int window_width, int window_height);

#endif