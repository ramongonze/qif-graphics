#include "information.h"

Information::Information(){
	this->hyperReady = false;
    this->mouseClickedOnPrior = false;
    this->prior = vector<long double>(3, 0);
    this->channel = vector<vector<long double>>(3, vector<long double>(3, 0));
    
    // Define the camera to look into our 3d world
    this->camera3d = { 0 };
    this->camera3d.position = (Vector3){ 40.0f, 40.0f, 40.0f}; // Camera position
    this->camera3d.target = (Vector3){ 0.0f, 0.0f, 0.0f };     // Camera looking at point
    this->camera3d.up = (Vector3){ 0.0f, 1.0f, 0.0f };         // Camera up vector (rotation towards target)
    this->camera3d.fovy = 45.0f;                               // Camera field-of-view Y
    this->camera3d.type = CAMERA_PERSPECTIVE;                  // Camera mode type  

    this->initialCamera3dPosition = this->camera3d.position;

    // Test
    this->ray = { 0 };                    // Picking line ray
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

int Information::orientation(Point p1, Point p2, Point p3){
    int val = (p2.y - p1.y) * (p3.x - p2.x) - 
              (p2.x - p1.x) * (p3.y - p2.y); 
  
    if (val == 0) return 0;  // colinear 
  
    if(val >= 0) return 1; // clock wise
    else return 2; // counterclock wise
}

Point Information::pointIntersection(Point A, Point B, Point C, Point D){
    // Line AB represented as a1x + b1y = c1 
    long double a1 = B.y - A.y; 
    long double b1 = A.x - B.x; 
    long double c1 = a1*(A.x) + b1*(A.y); 
  
    // Line CD represented as a2x + b2y = c2 
    long double a2 = D.y - C.y; 
    long double b2 = C.x - D.x; 
    long double c2 = a2*(C.x)+ b2*(C.y); 
  
    long double determinant = a1*b2 - a2*b1; 
  
    Point p = Point((b2*c1 - b1*c2)/determinant, (a1*c2 - a2*c1)/determinant);

    return p; 
}

Point Information::adjustPrior(vector<Vector2> &TrianglePoints, Vector2 mouse){
    Point mousePosition(mouse.x, WINDOWS_HEIGHT - mouse.y);
    Point TP0(TrianglePoints[0].x, WINDOWS_HEIGHT - TrianglePoints[0].y);
    Point TP1(TrianglePoints[1].x, WINDOWS_HEIGHT - TrianglePoints[1].y);
    Point TP2(TrianglePoints[2].x, WINDOWS_HEIGHT - TrianglePoints[2].y);

    // cout << "mousePosition: " << mousePosition.x << ", " << mousePosition.y << endl;

    int oL = orientation(TP1, mousePosition, TP0); // Left edge
    int oR = orientation(TP2, mousePosition, TP0); // Right edge
    int oD = orientation(TP1, mousePosition, TP2); // Down edge

    // cout << "oL: " << oL << ", oR: " << oR << ", oD: " << oD << endl;

    // Check if the mosuePoint is colinear with one of the triangle edges.
    if(oL == 0){
        if(mousePosition.y > TP0.y){
            TP0.y = WINDOWS_HEIGHT - TP0.y;
            return TP0;
        }else if(mousePosition.y < TP1.y){
            TP1.y = WINDOWS_HEIGHT - TP1.y;
            return TP1;
        }else{
            mousePosition.y = WINDOWS_HEIGHT - mousePosition.y;
            return mousePosition;
        }
    }

    if(oR == 0){
        if(mousePosition.y > TP0.y){
            TP0.y = WINDOWS_HEIGHT - TP0.y;
            return TP0;
        }else if(mousePosition.y < TP2.y){
            TP2.y = WINDOWS_HEIGHT - TP2.y;
            return TP2;
        }else{
            mousePosition.y = WINDOWS_HEIGHT - mousePosition.y;
            return mousePosition;
        }
    }

    if(oD == 0){
        if(mousePosition.x < TP1.x){
            TP1.y = WINDOWS_HEIGHT - TP1.y;
            return TP1;
        }else if(mousePosition.x > TP2.x){
            TP2.y = WINDOWS_HEIGHT - TP2.y;
            return TP2;
        }else{
            mousePosition.y = WINDOWS_HEIGHT - mousePosition.y;
            return mousePosition;
        }
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

    if(!isInRegion1 && !isInRegion2 && !isInRegion3){
        mousePosition.y = WINDOWS_HEIGHT - mousePosition.y;
        return mousePosition;
    }

    if(isInRegion1 && isInRegion2){
        TP0.y = WINDOWS_HEIGHT - TP0.y;
        return TP0;
    }
    if(isInRegion2 && isInRegion3){
        TP2.y = WINDOWS_HEIGHT - TP2.y;
        return TP2;
    }
    if(isInRegion1 && isInRegion3){
        TP1.y = WINDOWS_HEIGHT - TP1.y;
        return TP1;
    }

    Point p;

    if(isInRegion1) p = pointIntersection(TP0, TP1, mousePosition, TP2);
    if(isInRegion2) p = pointIntersection(TP0, TP2, TP1, mousePosition);
    if(isInRegion3) p = pointIntersection(TP1, TP2, mousePosition, TP0);

    p.y = WINDOWS_HEIGHT - p.y;
    return p;
}

void Information::updateHyper(vector<Vector2> &TrianglePoints){
    
    Point mousePosition;
    vector<long double> newPrior(3);

    mousePosition = adjustPrior(TrianglePoints, GetMousePosition());
    mousePosition = pixel2Bary(mousePosition.x, mousePosition.y, TrianglePoints);
    
    bary2Dist(mousePosition, newPrior);
    Distribution D(newPrior);            
    hyper.rebuildHyper(D);
    this->prior = vector<long double>({newPrior[0], newPrior[1], newPrior[2]});
}

void Information::newRandomPrior(){
    srand(unsigned(time(0)));

    int threshold = 100, p;
    for(int i = 0; i < 2; i++){
        p = rand() % threshold;
        threshold -= p;
        prior[i] = p/100.0;
    }
    prior[2] = threshold/100.0;

    random_shuffle(prior.begin(), prior.end());
}

void Information::newRandomChannel(int num_out){
    srand(unsigned(time(0)));
    vector<long double> prob(num_out);
    this->channel = vector<vector<long double>>(num_out, vector<long double>(3, 0));

    for(int i = 0; i < 3; i++){
        int threshold = 100, p;
        for(int j = 0; j < num_out-1; j++){
            p = rand() % threshold;
            threshold -= p;
            prob[j] = p/100.0;
        }
        prob[num_out-1] = threshold/100.0;

        random_shuffle(prob.begin(), prob.end());

        for(int j = 0; j < channel.size(); j++){
            channel[j][i] = prob[j];
        }
    }
}