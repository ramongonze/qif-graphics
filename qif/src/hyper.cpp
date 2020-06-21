#include "hyper.h"

void Hyper::buildJoint(){
	joint = std::vector<std::vector<long double> >(prior.num_el, std::vector<long double>(channel.num_out));

	for(int i = 0; i < prior.num_el; i++){
		for(int j = 0; j < channel.num_out; j++){
			joint[i][j] = channel.matrix[i][j] * prior.prob[i];
		}
	}
}

void Hyper::buildOuter(){
	outer.num_el = channel.num_out;
	outer.prob = std::vector<long double>(outer.num_el);

	/* outer.prob[x] = sum of column x in joint matrix */
	for(int j = 0; j < channel.num_out; j++){
		outer.prob[j] = 0.0f;
		for(int i = 0; i < prior.num_el; i++){
			outer.prob[j] += joint[i][j];
		}
	}
}

void Hyper::buildInners(){
	inners = std::vector<std::vector<long double> >(prior.num_el, std::vector<long double>(channel.num_out));

	for(int i = 0; i < prior.num_el; i++){
		for(int j = 0; j < channel.num_out; j++){
			if(outer.prob[j] == 0){
				inners[i][j] = 0;
			}else{
				inners[i][j] = joint[i][j]/outer.prob[j];
			}
		}
	}
}

void Hyper::reduceHyper(){
	/* shouldErase[i] = true if the column i of the hyper matrix 
	 * should be erased or shouldErase[i] = false otherwise */
	std::vector<bool> shouldErase(channel.num_out, false);

	int curInner = 0;
	for(int i = 0; i < channel.num_out; i++){
		if(!shouldErase[i]){

			/* Check if the posterior i has 0 probability of occuring */
			if(outer.prob[i] < EPS){
				shouldErase[i] = true;
				continue;
			}

			/* Create the set of old labels containing only the new label */
			labels.insert(std::make_pair(curInner,std::set<int>({i})));
			
			for(int j = i+1; j < channel.num_out; j++){
				if(shouldErase[j])
					continue;

				/* Check if the probability distributions of inners i and j are equal */
				bool isEqual = true;
				for(int k = 0; k < prior.num_el; k++){
					if(fabs(inners[k][i] - inners[k][j]) > EPS){
						isEqual = false;
						break;
					}
				}

				/* If the column j is equal to the column i, add j to the set of
				 * old labels of i and check delete it */
				if(isEqual){
					labels[curInner].insert(j);
					outer.prob[i] += outer.prob[j];
					shouldErase[j] = true;
				}
			}

			curInner++;
		}
	}

	/* Delete all columns i where shouldErase[i] = true */
	for(int j = channel.num_out-1; j >= 0; j--){
		if(shouldErase[j]){
			outer.prob.erase(outer.prob.begin() + j);
			for(int i = 0; i < prior.num_el; i++){
				inners[i].erase(inners[i].begin()+j);
			}
		}
	}

	outer.num_el = outer.prob.size();
	num_post = outer.num_el;
}

Hyper::Hyper(){
	num_post = 0;
	prior = Distribution();
	channel = Channel();
	joint = std::vector<std::vector<long double> >(0, std::vector<long double>(0));
	inners = std::vector<std::vector<long double> >(0, std::vector<long double>(0));
}

Hyper::Hyper(std::string prior_file, std::string channel_file){
	prior = Distribution(prior_file);
	channel = Channel(prior, channel_file);

	buildJoint();
	buildOuter();
	buildInners();
	reduceHyper();
}

Hyper::Hyper(Channel &channel){
	this->prior = Distribution(channel.prior.prob);
	this->channel = Channel(this->prior, channel.matrix);

	buildJoint();
	buildOuter();
	buildInners();
	reduceHyper();
}

void Hyper::rebuildHyper(Distribution &prior){
	if(prior.num_el == 0 && prior.prob.size() == 0 && channel.num_out == 0 && channel.matrix.size() == 0){
		fprintf(stderr, "The hyper-distribution can not be rebuilt if it has not been created!\n");
		exit(EXIT_FAILURE);
	}

	/* Check if the new prior is valid looking at the current channel */
	if(prior.num_el != this->channel.prior.num_el){
		fprintf(stderr, "The number of elements in the new prior differs from the number of rows in the current channel!\n");
		exit(EXIT_FAILURE);
	}

	this->prior = Distribution(prior.prob);
	this->channel.prior = Distribution(prior.prob);

	buildJoint();
	buildOuter();
	buildInners();
	reduceHyper();
}

void Hyper::rebuildHyper(Channel &channel){
	if(prior.num_el == 0 && prior.prob.size() == 0){
		fprintf(stderr, "The hyper-distribution can not be rebuilt if it was not been created!\n");
		exit(EXIT_FAILURE);
	}

	/* Check if the new channel is valid looking at the current prior */
	if(this->prior.num_el != channel.prior.num_el){
		fprintf(stderr, "The number of rows in the new channel differs from the number of elements in the current prior!\n");
		exit(EXIT_FAILURE);
	}

	this->prior = Distribution(channel.prior.prob);
	this->channel = Channel(this->prior, channel.matrix);

	buildJoint();
	buildOuter();
	buildInners();
	reduceHyper();
}

std::string Hyper::toString(std::string type, int precision){
	std::ostringstream out;

	/* Precision to print probabilities */
	out << std::fixed << std::setprecision(precision);

	if(type == "joint"){
		unsigned int n_secrets = joint.size();
		unsigned int n_outputs = joint[0].size();

		for(unsigned int i = 0; i < n_secrets; i++){
			for(unsigned int j = 0; j < n_outputs-1; j++){
				out << joint[i][j] << " ";
			}
			out << joint[i][n_outputs-1] << "\n";
		}
	}else if(type == "outer"){
		unsigned int n_elements = outer.prob.size();

		for(unsigned int i = 0; i < n_elements-1; i++){
			out << outer.prob[i] << " ";
		}

		out << outer.prob[n_elements-1] << "\n";
	}else if(type == "inners"){
		unsigned int n_secrets = inners.size();
		unsigned int n_num_post = inners[0].size();

		for(unsigned int i = 0; i < n_secrets; i++){
			for(unsigned int j = 0; j < n_num_post-1; j++){
				out << inners[i][j] << " ";
			}
			out << inners[i][n_num_post-1] << "\n";
		}
	}else if(type == "labels"){
		for(std::map<int, std::set<int> >::iterator new_label = labels.begin(); new_label != labels.end(); new_label++){
			out << new_label->first << ":";

			for(std::set<int>::iterator old_label = new_label->second.begin(); old_label != new_label->second.end(); old_label++){
				out << " " << *old_label;
			}
			out << "\n";
		}
	}else{
		fprintf(stderr, "Invalid parameter type! It must be ""joint"", ""outer"", ""inners"", ""labels""\n");
		exit(EXIT_FAILURE);
	}

	return out.str();
}

void Hyper::printToFile(std::string type, std::string file, int precision){
	std::ofstream F(file);

	if(F.is_open() == false){
		fprintf(stderr, "Error opening the file ""%s""!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	F << std::fixed << std::setprecision(precision);

	if(type == "joint"){
		F << joint.size() << " " << joint[0].size() << "\n";
	}else if(type == "outer"){
		F << outer.prob.size() << "\n";
	}else if(type == "inners"){
		F << inners.size() << " " << inners[0].size() << "\n";
	}else if(type != "labels"){
		fprintf(stderr, "Invalid parameter type! It must be ""joint"", ""outer"", ""inners"" or ""labels""\n");
		exit(EXIT_FAILURE);
	}

	F << toString(type, precision);		

	F.close();
}
