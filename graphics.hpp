#ifndef _graphics
#define _graphics

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "qif/qif.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

typedef struct Point{
	long double x;
	long double y;
}Point;

// Transforms a prior probability distribution on 3 elements in a barycentric point
Point* dist2BaryCoord(Distribution &prior);

// Transforms a prior probability distribution on 3 elements in a barycentric point.
Point* dist2BaryCoord(long double x, long double y, long double z);

// Transforms a barycentric coordiante in a probability distribution on 3 elements.
Distribution baryCoord2Dist(Point p);

// Euclidian distance between two points
long double distance(Point a, Point b);

#endif