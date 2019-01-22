#ifndef _graphics
#define _graphics

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../qif/qif.hpp"

#define ORIGIN_X -0.5
#define ORIGIN_Y -0.5

using namespace std;

typedef struct Point{
	long double x;
	long double y;
}Point;

// Transforms a prior probability distribution on 3 elements in a barycentric point
Point* dist2BaryCoord(Distribution &prior);

// Transforms a prior probability distribution on 3 elements in a barycentric point.
Point* dist2BaryCoord(long double x1, long double x2, long double x3);

// Transforms a barycentric coordiante in a probability distribution on 3 elements.
Distribution baryCoord2Dist(Point p);

// Euclidian distance between two points
long double euclidianDistance(Point a, Point b);

#endif