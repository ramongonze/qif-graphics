#include "graphics.h"

Point::Point(){
	this->x = 0.0f;
	this->y = 0.0f;
}

Point::Point(long double x, long double y){
	this->x = x;
	this->y = y;
}

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

long double euclidianDistance(Point a, Vector2 b){
	return sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
}

Point pixel2Bary(double x, double y, vector<Vector2> &TrianglePoints){
	Point p;

	long double triangleEdge = TrianglePoints[2].x - TrianglePoints[1].x;
	long double triangleHeight = TrianglePoints[1].y - TrianglePoints[0].y;

	p.x = (x - TrianglePoints[1].x) / triangleEdge;
	p.y = 1 - ((y - TrianglePoints[0].y) / triangleHeight);

	return p;
}

Point bary2Pixel(double x, double y, vector<Vector2> &TrianglePoints){
	Point p;
	
	long double triangleEdge = TrianglePoints[2].x - TrianglePoints[1].x;
	long double triangleHeight = TrianglePoints[1].y - TrianglePoints[0].y;

	p.x = TrianglePoints[1].x + (x * triangleEdge);
	p.y = TrianglePoints[1].y - (y * triangleHeight);

	return p;
}