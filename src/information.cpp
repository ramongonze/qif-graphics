#include "information.h"
#include <iostream>

Information::Information(){
	this->hyperReady = false;
    this->mouseClickedOnPrior = false;
}

int Information::checkPriorText(vector<char*> &prior_){
    vector<pair<string, string>> newPrior(prior_.size());
    vector<string> priorStr(prior_.size());

    for(int i = 0; i < priorStr.size(); i++){
    	priorStr[i] = string(prior_[i]);
    }

    try{
        for(int i = 0; i < prior_.size(); i++){
            size_t pos = priorStr[i].find('/');
            if(pos != string::npos){ // If true, the user is typing a fraction
                string numerator = priorStr[i].substr(0, pos);
                string denominator = priorStr[i].substr(pos+1, priorStr[i].size()-pos-1);
                
                // Remove blank spaces
                numerator.erase(remove(numerator.begin(), numerator.end(), ' '), numerator.end());
                denominator.erase(remove(denominator.begin(), denominator.end(), ' '), denominator.end());
                
                newPrior[i] = make_pair(numerator, denominator);
            }else{
            	newPrior[i] = make_pair("$", priorStr[i]);
            }
        }
        // Update values
        this->prior = vector<long double>(priorStr.size());
        for(int i = 0; i < priorStr.size(); i++){
        	if(newPrior[i].first == "$"){
        		this->prior[i] = std::stold(newPrior[i].second);
        	}else{
        		this->prior[i] = std::stold(newPrior[i].first)/std::stold(newPrior[i].second);
        	}
        }
        
        return NO_ERROR;
    }catch(exception& e){
        return INVALID_VALUE;
    }
}

int Information::checkChannelText(vector<vector<char*>> &channel_){
    vector<vector<pair<string, string>>> newChannel(channel_.size(), vector<pair<string, string>>(channel_[0].size()));
    vector<vector<string>> channelStr(channel_.size(), vector<string>(channel_[0].size()));

    for(int i = 0; i < channelStr.size(); i++){
    	for(int j = 0; j < channelStr[i].size(); j++){
    		channelStr[i][j] = string(channel_[i][j]);
    	}
    }

    try{
        for(int i = 0; i < channelStr.size(); i++){
            for(int j = 0; j < channelStr[i].size(); j++){
                size_t pos = channelStr[i][j].find('/');
                if(pos != string::npos){ // If true, the user is typing a fraction
                    string numerator = channelStr[i][j].substr(0, pos);
                    string denominator = channelStr[i][j].substr(pos+1, channelStr[i][j].size()-pos-1);
                    
                    // Remove blank spaces
                    numerator.erase(remove(numerator.begin(), numerator.end(), ' '), numerator.end());
                    denominator.erase(remove(denominator.begin(), denominator.end(), ' '), denominator.end());

                    newChannel[i][j] = make_pair(numerator, denominator);
                }else{
                    newChannel[i][j] = make_pair("$", channelStr[i][j]);
                }
            }
        }

        // Update values. Columns and rows are inverted in channelStr.
        this->channel = vector<vector<long double>>(channelStr[0].size(), vector<long double>(channelStr.size()));
        for(int i = 0; i < channelStr.size(); i++){
        	for(int j = 0; j < channelStr[i].size(); j++){
        		if(newChannel[i][j].first == "$"){
        			this->channel[j][i] = std::stold(newChannel[i][j].second);
        		}else{
        			this->channel[j][i] = std::stold(newChannel[i][j].first)/std::stold(newChannel[i][j].second);
        		}
        	}
        }

        return NO_ERROR;
    }catch(exception& e){
        return INVALID_VALUE;
    }
}

void Information::buildCircles(vector<Vector2> &TrianglePoints){
    Point p;
    
    // Prior
    p = dist2Bary(hyper.prior);
    p = bary2Pixel(p.x, p.y, TrianglePoints);    
    priorCircle.center = Point(p.x, p.y);
    priorCircle.radius = PRIOR_RADIUS;

    // Inners
    innersCircles = vector<Circle>(hyper.num_post);
    for(int i = 0; i < hyper.num_post; i++){
        p = dist2Bary(hyper.inners[0][i], hyper.inners[1][i], hyper.inners[2][i]);
        p = bary2Pixel(p.x, p.y, TrianglePoints);
        
        innersCircles[i].center = Point(p.x, p.y);
        innersCircles[i].radius = (int) sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);
    }
}

bool Information::updateHyper(vector<Vector2> &TrianglePoints){
    Vector2 mouse;
    Point mousePosition;
    vector<long double> newPrior(3);

    mouse = GetMousePosition();
    mousePosition = Point(mouse.x, mouse.y);

    // Check if the user is moving the prior
    if(){
        mousePosition = pixel2Bary(mousePosition.x, mousePosition.y, TrianglePoints);
        
        // Check if the mouse position represents a valid probability distribution
        if(bary2Dist(mousePosition, newPrior)){   
            // Rounds to a point distribution if one of the probabilities is >= 0.98
            if(newPrior[0] >= 0.98) newPrior = {1, 0, 0};
            else if(newPrior[1] >= 0.98) newPrior = {0, 1, 0};
            else if(newPrior[2] >= 0.98) newPrior = {0, 0, 1};

            Distribution D(newPrior);            
            hyper.rebuildHyper(D);
            this->prior = vector<long double>(newPrior);
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

Vector2 adjustPrior(vector<Vector2> &TrianglePoints, Vector2 mousePosition){
    int oL = orientation(TP[1], mousePosition, TP[0]); // Left edge
    int oR = orientation(TP[2], mousePosition, TP[0]); // Right edge
    int oD = orientation(TP[1], mousePosition, TP[2]); // Down edge

    // Check if the mosuePoint is colinear with one of the triangle edges.
    if(oL == 0){
       if(mousePosition.y > TrianglePoints[0].y) return TrianglePoints[0];
       else if(mousePosition.y < TrianglePoints[1].y) return TrianglePoints[1];
       else return mousePosition;
    }

    if(oR == 0){
        if(mousePosition.y > TrianglePoints[0].y) return TrianglePoints[0];
        else if(mousePosition.y < TrianglePoints[2].u) return TrianglePoints[2];
        else return mousePosition;
    }

    if(oD == 0){
        if(mousePosition.x < TrianglePoints[1].x) return TrianglePoints[1];
        else if(mousePosition.x > TrianglePoints[2].x) return TrianglePoints[2];
        else return mousePosition;
    }

    /*
     *            /\
     * Region 1  /  \ Region 2
     *          /    \
     *         /______\
     *         Region 3
     *
     * Region 1: Above the left edge
     * Region 2: Above the right edge
     * Region 3: Below the down edge
     *
    */

    bool isInRegion1, isInRegion2, isInRegion3;

    if(oL == 1) isInRegion1 = true;
    else isInRegion1 = false;

    if(oR == 2) isInRegion2 = true;
    else isInRegion2 = false;

    if(oD == 2) isInRegion3 = true;
    else isInRegion3 = false;

    if(isInRegion1 && isInRegion2) return TrianglePoints[0];
    if(isInRegion2 && isInRegion3) return TrianglePoints[2];
    if(isInRegion1 && isInRegion3) return TrianglePoints[1];

    if(isInRegion1){
        if(mousePosition.y > TrianglePoints[0].y) return TrianglePoints[0];
        else if(mousePosition.y < TrianglePoints[1].y) return TrianglePoints[1];
        else{
            
        }
    }

    if(isInRegion2){
        if(mousePosition.y > TrianglePoints[0].y) return TrianglePoints[0];
        else if(mousePosition.y < TrianglePoints[1].y) return TrianglePoints[1];
        else{

        }
    }

}

int orientation(Point p1, Point p2, Point p3){
    int val = (p2.y - p1.y) * (p3.x - p2.x) - 
              (p2.x - p1.x) * (p3.y - p2.y); 
  
    if (val == 0) return 0;  // colinear 
  
    if(val > 0) return 1; // clock wise
    else return 2; // counterclock wise
} 