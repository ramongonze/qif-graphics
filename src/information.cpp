#include "information.h"
#include <iostream>
Information::Information(){
	this->hyperReady = false;
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