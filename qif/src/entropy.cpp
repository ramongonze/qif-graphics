#include "entropy.h"

long double guessingEntropy(Distribution &prior){
	long double g_entropy = 0;

	std::vector<long double> ordered_prior = prior.prob;
	sort(ordered_prior.rbegin(), ordered_prior.rend());

	for(int i = 0; i < prior.num_el; i++){
		g_entropy += ((i+1) * ordered_prior[i]);
	}

	return g_entropy;
}

long double guessingEntropy(Hyper &hyper){
	long double g_entropy = 0;

	for(int i = 0; i < hyper.channel.num_out; i++){
		std::vector<long double> ordered_dist;

		for(int j = 0; j < hyper.prior.num_el; j++){
			ordered_dist.push_back(hyper.inners[j][i]);
		}

		sort(ordered_dist.rbegin(), ordered_dist.rend());

		long double post_g_entropy = 0;
		for(int j = 0; j < hyper.prior.num_el; j++){
			post_g_entropy += ((j+1) * ordered_dist[j]);
		}

		g_entropy += (post_g_entropy * hyper.outer.prob[i]);
	}


	return g_entropy;
}

long double shannonEntropy(Distribution &dist){
	long double entropy = 0;

	for(int i = 0; i < dist.num_el; i++){
		if(dist.prob[i] > 0){
			entropy += (dist.prob[i] * log2(dist.prob[i]));
		}
	}

	return -entropy;
}

long double shannonEntropy(Hyper &hyper){
	long double entropy = 0;

	for(int i = 0; i < hyper.channel.num_out; i++){
		/* For every posterior distribution */
		
		long double posteriorEntropy = 0;
		for(int j = 0; j < hyper.prior.num_el; j++){
			if(hyper.inners[j][i] > 0){
				posteriorEntropy += (hyper.inners[j][i] * log2(hyper.inners[j][i]));
			}
		}

		entropy -= (hyper.outer.prob[i] * posteriorEntropy);
	}

	return entropy;
}
