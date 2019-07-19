#include "../include/graphics.hpp"

Point dist2Bary(Distribution &prior){
	Point p;
	p.x = prior.prob[2] + prior.prob[0]/2.0f;
	p.y = prior.prob[0];
	return p;
}

Point dist2Bary(long double x1, long double x2, long double x3){
	Point p;
	p.x = x3 + x1/2.0f;
	p.y = x1;
	return p;
}

bool bary2Dist(Point p, vector<long double> &prob){
	vector<long double> prob_aux(3);
	prob_aux[0] = p.y;
	prob_aux[1] = 1 - p.x - p.y/2.0f;
	prob_aux[2] = p.x - p.y/2.0f;

	if(Distribution::isDistribution(prob_aux)){
		prob[0] = prob_aux[0];
		prob[1] = prob_aux[1];
		prob[2] = prob_aux[2];
		return true;
	}

	return false;
}

long double euclidianDistance(Point a, Point b){
	return sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
}

Point pixel2Bary(double x, double y, int window_width, int window_height){
	Point p;
	p.x = (x - (0.45f * window_width)) / (0.5f*window_width);
	p.y = 1 - (y - (0.40f * window_height)) / (0.50f*window_height);
	return p;
}

Point bary2Pixel(double x, double y, int window_width, int window_height){
	Point p;
	p.x = ORIGIN_X + (x * (window_width/2.0f));
	p.y = ORIGIN_Y - (y * (window_height/2.0f));
	return p;
}