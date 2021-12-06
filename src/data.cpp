#include "data.h"

Data::Data(){
    // Flags
    for(int i = 0; i < 8; i++){
        compute[i] = false;
        ready[i] = false;
    }

    for(int i = 0; i < NUMBER_CHANNELS; i++)
        hyper[i] = Hyper();

    prior = vector<long double>(NUMBER_SECRETS, 0);
    channel = vector<vector<vector<long double>>>(NUMBER_CHANNELS, vector<vector<long double>>(MAX_CHANNEL_OUTPUTS, vector<long double>(MAX_CHANNEL_OUTPUTS, 0)));

    epsilon = log(2);
    delta = 0.2;

    validCharacters = string("0123456789./");
    error = NO_ERROR;
    for(int i = 0; i < NUMBER_CHANNELS; i++)
        hyperReady[i] = false;
    mouseClickedOnPrior = false;
    fileSaved = true;
    
    // Calculates the number of frames the animation will have, considering the software is running in 60 FPS
    animation = -1;
    animationRunning = false;
}

long double Data::checkProbText(char valueStr[CHAR_BUFFER_SIZE]){
    string value;
    pair<string, string> newValue;

    try{
        value = string(valueStr);
        
        // Check whether the string is empty or not
        if(value == string(""))
            return INVALID_PROBABILITY;

        for(long unsigned int i = 0; i < value.size(); i++)
            if(validCharacters.find(value[i]) == std::string::npos)
                return INVALID_PROBABILITY;
            
        size_t pos = value.find('/');
        if(pos != string::npos){ // If true, the user is typing a fraction
            string numerator = value.substr(0, pos);
            string denominator = value.substr(pos+1, value.size()-pos-1);
            
            // Remove blank spaces
            numerator.erase(remove(numerator.begin(), numerator.end(), ' '), numerator.end());
            denominator.erase(remove(denominator.begin(), denominator.end(), ' '), denominator.end());
            
            newValue = make_pair(numerator, denominator);
        }else{
            newValue = make_pair("not fraction", value);
        }

        if(newValue.first == "not fraction")
            return std::stold(newValue.second);
        else
            return std::stold(newValue.first)/std::stold(newValue.second);        
    }catch(exception& e){
        return INVALID_PROBABILITY;
    }
}

int Data::checkPriorText(char prior_[NUMBER_SECRETS][CHAR_BUFFER_SIZE]){
    long double value;
    
    try{
        for(int i = 0; i < NUMBER_SECRETS; i++){
            value = checkProbText(prior_[i]);
            if(value == INVALID_PROBABILITY)
                return INVALID_PRIOR;
            this->prior[i] = value;
        }

        if(Distribution::isDistribution(this->prior))
            priorObj = Distribution(this->prior);

        return NO_ERROR;
    }catch(exception& e){
        return INVALID_VALUE_PRIOR;
    }
}

int Data::checkChannelText(char channel_[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int channel, int numSecrets, int numOutputs){
    long double value;

    try{
        for(int i = 0; i < numSecrets; i++){
            for(int j = 0; j < numOutputs; j++){
                value = checkProbText(channel_[i][j]);
                if(value == INVALID_PROBABILITY)
                    return INVALID_VALUE_CHANNEL_1+channel;
                this->channel[channel][i][j] = value;
            }
        }
        return NO_ERROR;
    }catch(exception& e){
        return INVALID_VALUE_CHANNEL_1+channel;
    }
}

int Data::checkEpsilonDeltaText(char epsilon_[CHAR_BUFFER_SIZE], char delta_[CHAR_BUFFER_SIZE]){
    long double value;
    
    try{
        value = checkProbText(epsilon_);
        if(value == INVALID_PROBABILITY)
            return INVALID_EPSILON_OR_DELTA;
        this->epsilon = value; // Update epsilon value
     
        value = checkProbText(delta_);
        if(value == INVALID_PROBABILITY)
            return INVALID_EPSILON_OR_DELTA;
        this->delta = value; // Update delta value
    }catch(exception& e){
        return INVALID_EPSILON_OR_DELTA;
    }

    return NO_ERROR;
}

void Data::buildPriorCircle(Vector2 TrianglePoints[3]){
    Point p;
    p = dist2Bary(priorObj);
    p = bary2Pixel(p.x, p.y, TrianglePoints);    
    priorCircle.center = Point(p.x, p.y);
    priorCircle.radius = PRIOR_RADIUS;
}
	
void Data::buildInnerCircles(Vector2 TrianglePoints[3], int channel, int mode){
    Point p;

    if(animationRunning && (((mode == MODE_SINGLE || mode == MODE_DP) && animation == STEPS) || (mode != MODE_SINGLE && animation >= 2*STEPS))){
        // First draw of the animation
        for(int i = 0; i < hyper[channel].num_post; i++){
            p = dist2Bary(hyper[channel].inners[0][i], hyper[channel].inners[1][i], hyper[channel].inners[2][i]);
            p = bary2Pixel(p.x, p.y, TrianglePoints);
            // All inners starts in prior position
            innersCircles[channel][i].center = priorCircle.center;
            innersCircles[channel][i].radius = (int)sqrt(hyper[channel].outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);
            
            long double deltaX = p.x - priorCircle.center.x;
            long double deltaY = p.y - priorCircle.center.y;
            xJumpAnimation[channel][i] = deltaX/STEPS;
            yJumpAnimation[channel][i] = deltaY/STEPS;
        }
        
        animation--;
    }else if(animationRunning && animation > 0){
        // The animation is happening
        for(int i = 0; i < hyper[channel].num_post; i++){
            innersCircles[channel][i].center.x += xJumpAnimation[channel][i];
            innersCircles[channel][i].center.y += yJumpAnimation[channel][i];
        }
        animation--;
    }else if((animationRunning && animation == 0) || animation == UPDATE_CIRCLES_BY_MOUSE){
        // The animation has finished or user moved the prior with the mouse
        for(int i = 0; i < hyper[channel].num_post; i++){
            p = dist2Bary(hyper[channel].inners[0][i], hyper[channel].inners[1][i], hyper[channel].inners[2][i]);
            p = bary2Pixel(p.x, p.y, TrianglePoints);
            innersCircles[channel][i].center = Point(p.x, p.y);
            innersCircles[channel][i].radius = (int)sqrt(hyper[channel].outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);
        }
        animationRunning = false;
    }
}

int Data::orientation(Point p1, Point p2, Point p3){
    int val = (p2.y - p1.y) * (p3.x - p2.x) - 
              (p2.x - p1.x) * (p3.y - p2.y); 
  
    if (val == 0) return 0;  // colinear 
  
    if(val >= 0) return 1; // clock wise
    else return 2; // counterclock wise
}

Point Data::pointIntersection(Point A, Point B, Point C, Point D){
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

Point Data::adjustPrior(Vector2 TrianglePoints[3], Vector2 mouse){
    Point mousePosition(mouse.x, WINDOWS_HEIGHT - mouse.y);
    Point TP0(TrianglePoints[0].x, WINDOWS_HEIGHT - TrianglePoints[0].y);
    Point TP1(TrianglePoints[1].x, WINDOWS_HEIGHT - TrianglePoints[1].y);
    Point TP2(TrianglePoints[2].x, WINDOWS_HEIGHT - TrianglePoints[2].y);

    int oL = orientation(TP1, mousePosition, TP0); // Left edge
    int oR = orientation(TP2, mousePosition, TP0); // Right edge
    int oD = orientation(TP1, mousePosition, TP2); // Down edge

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

void Data::updateHyper(Vector2 TrianglePoints[3], int mode){
    Point mousePosition;
    vector<long double> newPrior(NUMBER_SECRETS);

    mousePosition = adjustPrior(TrianglePoints, GetMousePosition());
    mousePosition = pixel2Bary(mousePosition.x, mousePosition.y, TrianglePoints);
    
    bary2Dist(mousePosition, newPrior);
    priorObj = Distribution(newPrior);

    hyper[CHANNEL_1].rebuildHyper(priorObj);

    if(mode == MODE_TWO)
        hyper[CHANNEL_2].rebuildHyper(priorObj);
    else if(mode == MODE_REF)
        hyper[CHANNEL_3].rebuildHyper(priorObj);

    prior = vector<long double>({newPrior[0], newPrior[1], newPrior[2]});
}

void Data::newRandomPrior(){
    srand(unsigned(time(0)));

    int threshold = 100, p;
    for(int i = 0; i < 2; i++){
        p = rand() % threshold;
        threshold -= p;
        prior[i] = p/100.0;
    }
    prior[2] = threshold/100.0;
    
    random_shuffle(prior.begin(), prior.end());
    priorObj = Distribution(prior);
}

void Data::newRandomChannel(int curChannel, int numSecrets, int numOutputs){
    srand(unsigned(time(0)));
    vector<long double> prob(numOutputs);
    channel[curChannel] = vector<vector<long double>>(numSecrets, vector<long double>(numOutputs, 0));

    for(int i = 0; i < numSecrets; i++){
        int threshold = 100, p;
        for(int j = 0; j < numOutputs-1; j++){
            p = rand() % threshold;
            threshold -= p;
            prob[j] = p/100.0;
        }
        prob[numOutputs-1] = threshold/100.0;

        random_shuffle(prob.begin(), prob.end());

        for(int j = 0; j < numOutputs; j++){
            channel[curChannel][i][j] = prob[j];
        }
    }

    if(ready[FLAG_PRIOR] && numSecrets == NUMBER_SECRETS){
        channelObj[curChannel] = Channel(priorObj, channel[curChannel]);
    }else{
        fakePrior = Distribution(numSecrets, "uniform");
        channelObj[curChannel] = Channel(fakePrior, channel[curChannel]);
    }
}

void Data::resetAllExceptComputeChannel1(){
    for(int channel = 0; channel < NUMBER_CHANNELS; channel++){
        ready[FLAG_CHANNEL_1+channel] = false;
        compute[FLAG_CHANNEL_1+channel] = false;
        ready[FLAG_HYPER_1+channel] = false;
        compute[FLAG_HYPER_1+channel] = false;
    }
    compute[FLAG_CHANNEL_1] = true;
}
