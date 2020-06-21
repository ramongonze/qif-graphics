#ifndef _entropy
#define _entropy

#include "hyper.h"
#include <cmath>

/**
 * \brief Guessing entropy of a prior probability distribution.
 *
 * The guessing entropy of a prior distribution on a set of secrets 
 * is the expected number of tries needed by the adversary to guess the secret correctly.
 *
 * \param prior Prior distribution on the set of secrets.
 *
 * \return Guessing entropy of a prior distribution.
 */
long double guessingEntropy(Distribution &prior);

/**
 * \brief Guessing entropy of a hyper-distribution.
 *
 * The guessing entropy of a hyper-distribution is the expected number of tries
 * needed by the adversary to guess the secret correctly, looking to all possible
 * worlds (inner distributions). 
 *
 * \param hyper A hyper-distribution.
 *
 * \return Guessing entropy of a hyper-distribution.
 */
long double guessingEntropy(Hyper &hyper);

/**
 * \brief Shannon entropy of a probability distribution.
 *
 * The Shannon entropy of a prior distribution is a way to measure the secrecy of a secret.
 *
 * \param dist Prior distribution on the set of secrets.
 *
 * \return Shannon entropy of a probability distribution.
 * The returned value is in the interval [0,log2(_n_)], where _n_ is the number of elements
 * in the distribution.
 */
long double shannonEntropy(Distribution &dist);

/**
 * \brief Shannon entropy of a hyper-distribution.
 *
 * The Shannon entropy of a hyper-distribution is a way to measure the secrecy of a secret,
 * looking to all possible worlds (inner distributions).
 *
 * \param hyper A hyper-distribution.
 *
 * \return Shannon entropy of a hyper-distribution.
 * The returned value is in the interval [0,log2(_y_)], where _y_ is the
 * number of outputs of a channel.
 */
long double shannonEntropy(Hyper &hyper);

#endif
