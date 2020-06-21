#include "channel.h"

Channel::Channel(){
	this->num_out = 0;
	this->prior = Distribution();
	this->matrix = std::vector<std::vector<long double> >(0, std::vector<long double>(0));
}

Channel::Channel(Distribution &prior, std::string file){
	int k;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	if(!fscanf(F, "%d %d", &k, &(this->num_out))){
		exit(EXIT_FAILURE);
	}

	if(k != prior.num_el || this->num_out <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		exit(EXIT_FAILURE);
	}

	this->prior = Distribution(prior.prob);
	this->matrix = std::vector<std::vector<long double> >(this->prior.num_el, std::vector<long double>(this->num_out));

	for(int i = 0; i < this->prior.num_el; i++){
		for(int j = 0; j < this->num_out; j++){
			if(!fscanf(F, "%Lf", &(this->matrix[i][j]))){
				exit(EXIT_FAILURE);
			}
		}
	}
	fclose(F);

	if(!Channel::isChannel(this->matrix)){
		fprintf(stderr, "Error reading a channel. One of the rows is not a probability distribution!\n");
		exit(EXIT_FAILURE);
	}
}

Channel::Channel(Distribution &prior, std::vector<std::vector<long double> > &matrix){
	this->prior = Distribution(prior.prob);
	this->num_out = matrix[0].size();

	if((unsigned int)prior.num_el != matrix.size()){
		fprintf(stderr, "The number of secrets in the prior distribution is different of the number of rows in the channel matrix!\n");
		exit(EXIT_FAILURE);
	}

	this->matrix = std::vector<std::vector<long double> >(this->prior.num_el, std::vector<long double>(this->num_out));

	for(int i = 0; i < this->prior.num_el; i++){
		for(int j = 0; j < this->num_out; j++){
			this->matrix[i][j] = matrix[i][j];
		}
	}

	if(!isChannel(this->matrix)){
		fprintf(stderr, "The channel matrix is not valid!\n");
		exit(EXIT_FAILURE);
	}
}

Channel::Channel(Distribution &prior, int num_out){
	this->prior = Distribution(prior.prob);
	this->num_out = num_out;

	this->matrix = std::vector<std::vector<long double> >(this->prior.num_el, std::vector<long double>(this->num_out));

	for(int i = 0; i < this->prior.num_el; i++){
		int threshold = RAND_MAX;
		int x;

		for(int j = 0; j < this->num_out-1; j++){
			x = rand() % threshold;

			matrix[i][j] = (long double)x/RAND_MAX;
			threshold -= x;
		}

		matrix[i][this->num_out-1] = (long double)threshold/RAND_MAX;
	}
}

bool Channel::isChannel(std::vector<std::vector<long double> > &matrix){
	for(unsigned int i = 0; i < matrix.size(); i++){
		if(!Distribution::isDistribution(matrix[i])){
			return false;
		}
	}

	return true;
}

std::string Channel::toString(int precision){
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision);

	for(int i = 0; i < this->prior.num_el; i++){
		for(int j = 0; j < this->num_out-1; j++){
			out << this->matrix[i][j] << " ";
		}
		out << this->matrix[i][this->num_out-1] << "\n";
	}

	return out.str();
}

void Channel::printToFile(std::string file, int precision){
	std::ofstream F(file);

	if(F.is_open() == false){
		fprintf(stderr, "Error opening the file ""%s""!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	F << this->prior.num_el << " " << this->num_out << "\n";
	F << std::fixed << std::setprecision(precision);
	for(int i = 0; i < this->prior.num_el; i++){
		for(int j = 0; j < this->num_out-1; j++){
			F << this->matrix[i][j] << " ";
		}
		F << this->matrix[i][this->num_out-1] << "\n";
	}

	F.close();
}
