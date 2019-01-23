#ifndef _graphics
#define _graphics

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../qif/qif.hpp"

#define ORIGIN_X -0.5
#define ORIGIN_Y -0.5

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

#endif