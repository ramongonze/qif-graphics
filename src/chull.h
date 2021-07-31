/* A C++ program to find convex hull of a set of points.
 * Implementation get from https://cp-algorithms.com/geometry/grahams-scan-convex-hull.html.
 */

#ifndef _chull
#define _chull

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

struct pt {
    double x, y;
};

// Compare two points
bool cmp(pt a, pt b);

// Check wheter points are clockwise
bool cw(pt a, pt b, pt c);

// Check wheter points are coiunter clockwise
bool ccw(pt a, pt b, pt c);

// Given a set of points return its convex hull
void convexHull(vector<pt>& a);

#endif
