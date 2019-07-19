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
 */
Point pixel2Bary(double x, double y, int window_width, int window_height);

/* Transforms barycentric coordinate in pixel coordinate
 * Parameters:
 *		x: value (in barycentric coordinate) in x-axis
 *		y: value (in barycentric coordinate) in y-axis
 */
Point bary2Pixel(double x, double y, int window_width, int window_height);

#endif