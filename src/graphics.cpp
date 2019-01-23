#include "../include/graphics.hpp"

void dist2BaryCoord(Distribution &prior, Point &p){
	p.x = prior.prob[2] + prior.prob[0]/2.0f;
	p.y = prior.prob[0];
}

void dist2BaryCoord(long double x1, long double x2, long double x3, Point &p){
	p.x = x3 + x1/2.0f;
	p.y = x1;
}

bool baryCoord2Dist(Point p, vector<long double> &prob){

	p.x -= ORIGIN_X;
	p.y -= ORIGIN_Y;

	prob[0] = p.y;
	prob[1] = 1 - p.x - p.y/2.0f;
	prob[2] = p.x - p.y/2.0f;

	return Distribution::isDistribution(prob);
}

long double euclidianDistance(Point a, Point b){
	return sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
}