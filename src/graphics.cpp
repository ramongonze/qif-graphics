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
	if(x < 0.0f)
		p.x = (int)((1.0f+x) * (window_width/2.0f));
	else
		p.x = (int)(x * (window_width/2.0f)) + window_width/2;

	if(y < 0.0f)
		p.y = -(int)(y * (window_height/2.0f)) + window_width/2;	
	else
		p.y = (int)((1.0f-y) * (window_height/2.0f));
}

void createCircle(double x, double y, double radius, double r, double g, double b, double circleVertices[CIRCLES_SIDES]){
	// Parameters r,g,b are the circle's color

	int numberOfVertices = CIRCLES_SIDES + 2;
    
	double twicePi = 2.0f * M_PI;
    
	double *circleVerticesX = (double*) malloc(numberOfVertices * sizeof(double));
	double *circleVerticesY = (double*) malloc(numberOfVertices * sizeof(double));
	
	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	
	for(int i = 1; i < numberOfVertices; i++){
		double ang = twicePi/CIRCLES_SIDES * i;
		circleVerticesX[i] = x + (radius * cos(ang));
		circleVerticesY[i] = y + (radius * sin(ang));
	}
	
	for(int i = 0; i < numberOfVertices; i++){
		circleVertices[i * 6] = circleVerticesX[i];
		circleVertices[(i * 6) + 1] = circleVerticesY[i];
		circleVertices[(i * 6) + 2] = 0.0f; // Set z position always to 0.0f
		
		if(i == 0){
			circleVertices[(i * 6) + 3] = r + 0.2;
			circleVertices[(i * 6) + 4] = g + 0.2;
			circleVertices[(i * 6) + 5] = b + 0.2;
		}else{
			circleVertices[(i * 6) + 3] = r;
			circleVertices[(i * 6) + 4] = g;
			circleVertices[(i * 6) + 5] = b;
		}
	}
	
	free(circleVerticesX);
	free(circleVerticesY);
}