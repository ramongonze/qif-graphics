#include "distribution.h"

Distribution::Distribution(){
	num_el = 0;
	prob.resize(0);
}

Distribution::Distribution(std::string file){
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file \"%s\"!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	if(!fscanf(F, "%d", &num_el)){
		exit(EXIT_FAILURE);
	}

	prob.resize(num_el);
	for(int i = 0; i < num_el; i++){
		if(!fscanf(F, "%Lf", &prob[i])){
			exit(EXIT_FAILURE);
		}
	}

	fclose(F);

	if(!isDistribution(prob)){
		fprintf(stderr, "The numbers in the file \"%s\" are not a probability distribution!\n", file.c_str());
		exit(EXIT_FAILURE);
	}
}	

Distribution::Distribution(std::vector<long double> &prob){
	if(!isDistribution(prob)){
		fprintf(stderr, "The numbers in the vector does not make up a probability distribution!\n");
		exit(EXIT_FAILURE);
	}

	num_el = prob.size();
	this->prob.resize(0);

	for(int i = 0; i < num_el; i++){
		this->prob.push_back(prob[i]);		
	}
}

Distribution::Distribution(int num_el, std::string type){
	this->num_el = num_el;
	
	if(type == "uniform"){
		prob.resize(this->num_el, 1/(long double)this->num_el);
	}else if(type == "random"){
		do{
			prob.resize(0);
			int threshold = RAND_MAX;
			int x;

			for(int i = 0; i < num_el-1; i++){
				x = rand() % threshold;
				prob.push_back((long double)x/RAND_MAX);
				threshold -= x;
			}
			prob.push_back((long double)threshold/RAND_MAX);	

		}while(!isDistribution(prob));
	}else{
		fprintf(stderr, "Invalid argument to create a distribution! \n");
		exit(EXIT_FAILURE);
	}
}

bool Distribution::isDistribution(std::vector<long double> &prob){
	long double S = 0;

	for(unsigned int i = 0; i < prob.size(); i++){
		if(prob[i] < 0 || prob[i] > 1+EPS){
			return false;
		}

		S += prob[i];
	}

	return 1-EPS <= S && S <= 1+EPS;
}

std::string Distribution::toString(int precision){
	std::ostringstream out;

	out << std::fixed << std::setprecision(precision);
	
	for(int i = 0; i < num_el-1; i++){
		out << prob[i] << " ";
	}
	out << prob[num_el-1];

	return out.str();
}

void Distribution::printToFile(std::string file, int precision){
	std::ofstream F(file);

	if(F.is_open() == false){
		fprintf(stderr, "Error opening the file ""%s""!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	F << num_el << "\n";
	F << std::fixed << std::setprecision(precision);
	for(int i = 0; i < num_el; i++){
		F << prob[i] << "\n";
	}

	F.close();
}