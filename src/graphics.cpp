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

void pixelCoord2ScreenCoord(double x, double y, Point &p, int window_width, int window_height){

	if(x < window_width/2.0f){
		if(x < 0) p.x = -1;
		else p.x = -1+(x/(window_width/2.0f));
	}else{
		x -= (window_width/2.0f);
		if(x > window_width/2.0f) p.x = 1;
		else p.x = x/(window_width/2.0f);
	}

	if(y < window_height/2.0f){
		if(y < 0) p.y = 1;
		else p.y = 1- (y/(window_height/2.0f));
	}else{
		y -= (window_height/2.0f);
		if(y > window_height/2.0f) p.y = -1;
		else p.y = -(y/(window_height/2.0f));
	}
}

void screenCoord2PixelCoord(double x, double y, Point &p, int window_width, int window_height){
	p.x = ORIGIN_X + (x * (window_width/2.0f));
	p.y = ORIGIN_Y - (y * (window_height/2.0f));
}