#include "leakage.h"

long double additiveLeakage(Hyper &hyper, Gain &gain){
	return gVulnerability(hyper, gain) - gVulnerability(gain);
}

long double multiplicativeLeakage(Hyper &hyper, Gain &gain){
	long double vg_hyper = gVulnerability(hyper, gain);
	long double vg_prior = gVulnerability(gain);

	if(vg_prior < EPS){ /* Comparing if vg_prior == 0 */
		if(vg_hyper < EPS){ /* Comparing if vg_hyper == 0 */
			return 1;
		}else{
			return std::numeric_limits<long double>::infinity();
		}
	}else{
		return vg_hyper / vg_prior;
	}
}