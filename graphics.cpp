#include "graphics.hpp"

Point* dist2BaryCoord(Distribution &prior){
	Point *baryc = (Point*) malloc(sizeof(Point));

	baryc->x = prior.prob[2] + prior.prob[0]/2.0f;
	baryc->y = prior.prob[0];

	return baryc;
}

Point* dist2BaryCoord(long double x, long double y, long double z){
	Point *baryc = (Point*) malloc(sizeof(Point));

	baryc->x = z + x/2.0f;
	baryc->y = x;

	return baryc;
}

Distribution baryCoord2Dist(Point p){

	vector<long double> dist(3);
	dist[0] = p.y;
	dist[1] = 1 - p.x - p.y/2.0f;
	dist[2] = p.x - p.y/2.0f;

	Distribution D(dist);

	return D;
}

long double distance(Point a, Point b){
	return sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
}