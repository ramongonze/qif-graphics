#ifndef _hyper
#define _hyper

#include "distribution.h"
#include "channel.h"
#include <math.h>
#include <map>
#include <set>

/**
 * \brief Hyper-distributions.
 *
 * Definition of hyper-distribution:
 *
 * If **X** is a finite set (of possible secret values), π is the prior distribution on **X**,
 * and C is a channel, a hyper-distribution _[π>C]_ resulted from C on π is a 
 * distribution on distributions on **X**. Each output in an informational channel is a possible "world",
 * and each possible world is a new distribution on the set of secrets. We call the possible worlds
 * the *inner distributions*. Each possible world has a probability of occurring, and we call the 
 * distribution on the possible worlds as the *outer distribution*.
 *
 * A @ref Hyper object has 7 attributes:
 *
 * - @ref prior : A @ref Distribution object, which represents
 * the prior probability distribution on the set of secrets.
 *
 * - @ref channel : A @ref Channel object.
 *
 * - @ref joint : Joint dstribution matrix, where each position _joint[x][y]_ is the joint probability _p(x,y)_.
 *
 * - @ref inners : The inner distributions matrix. It is the set of possible "worlds" of a hyper-distribution.
 * Each column of this matrix is a posterior distribution on the set of secrets.
 *
 * - @ref outer : The probability distribution on the inners.
 *
 * - @ref num_post : Number of posterior distributons.
 *
 * - @ref labels : A map from an integer to a set. The number of posterior distributions can differ from the
 * number of channel outputs, because when we have two or more posteriors that are equals, we simplify them merging
 * into a single posterior, and sum their outer probabilities. So _labels_ maps from posterior distributions
 * to a subset of channel outputs.
 */
class Hyper{
	private:
		void buildJoint();
		void buildOuter();
		void buildInners();
		void buildHyper();
		void reduceHyper();

	public:
		/**
		 * \brief Default constructor.
		 *
		 * This constructor creates a @ref Hyper object with @ref prior and @ref channel equals to NULL,
		 * @ref num_post = 0 and @ref joint matrix, @ref inners matrix, @ref outer and @ref labels empty.
		 */
		Hyper();

		/**
		 * \brief Constructor used when the prior distribution on the set of secrets and the channel are in a file.
		 *
		 * The file format of @ref prior must be the following:
		 *
		 * 	n
		 * 	p1
		 * 	p2
		 * 	...
		 * 	pn
		 *
		 * where \c n is the number of elements in the distribution and \c pi is the probability of the ith element occurs.
		 *
		 * The file format of @ref channel must be the following:
		 *
		 * 	n y
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets and \c y is the number of outputs.
		 * Each \c pij denotes _p(j|i)_, the conditional probability of getting
		 * output _j_ given input _i_. Each two numbers must be separated by a space (" ").
		 *
		 * \param prior_file File name that contains a probability distribution.
		 * \param channel_file File name that contains a channel matrix.
		 *
		 * \warning The number of rows in the channel matrix must be equals to the number of
		 * elements in the prior distribution.
		 */
		Hyper(std::string prior_file, std::string channel_file);

		/**
		 * \brief Constructor used when there is already a @ref Channel object in a variable.
		 *
		 * As a channel has a distribution object (corresponding to the prior distribution on the set of secrets),
		 * it is not necessary to give a @ref Distribution as a parameter.
		 *
		 * \param channel A @ref Channel object.
		 */
		Hyper(Channel &channel);

		/**
		 * \brief A probability distribution on a set of secrets.
		 *
		 * It is a distribution on the set of secrets. See @ref Distribution for more details.
		 */
		Distribution prior;

		/**
		 * \brief An informational channel.
		 *
		 * It is an informational channel. See @ref Channel for more details.
		 */
		Channel channel;

		/**
		 * \brief Joint distribution matrix.
		 *
		 * It is a matrix \c n x \c y where \c n is the number of secrets in
		 * @ref channel and \c y is the number of outputs in @ref channel.
		 * Each value _joint[i][j]_ is the joint probability _p(i,j)_.
		 * The joint distribution is build multiplying @ref prior distribution
		 * by each row of the @ref channel matrix.
		 *
		 * The rows and columns are indexed from the position 0 (1st element) to the
		 * position @ref prior ->num_el-1 (for rows) or @ref num_out-1 (for columns).
		 */
		std::vector<std::vector<long double> > joint;

		/**
		 * \brief A distribution on posterior distributions.
		 *
		 * Each value _outer[i]_ is the probability _p(i)_ of the _ith_ posterior distribution occuring.
		 *
		 * The vector is indexed from the position 0 (1st element) to the
		 * position @ref num_post - 1 (last element). So the vector size = @ref num_post.
		 */
		Distribution outer;

		/**
		 * \brief Number of posterior distributions
		 *
		 * It is the number of posterior distributions yielded by @ref channel and @ref prior.
		 *
		 * \warning The number of posterior distributions can be different from the number of outputs in @ref channel.
		 * See @ref labels for more details.
		 */
		int num_post;

		/**
		 * \brief A matrix for the inner distributions.
		 *
		 * It is a matrix \c n x \c y where \c n is the number of secrets in @ref channel and \c y is the number of posterior distributions (@ref num_post).
		 * Each value _inners[i][j]_ is the conditional probability _p(i|j)_, that is, the probability
		 * of the secret being _i_ when the output is _j_. Each column is a posterior distribution.
		 *
		 * The rows and columns are indexed from the position 0 (1st element) to the
		 * position @ref prior ->num_el-1 (for rows) or @ref num_post-1 (for columns).
		 *
		 * \warning The number of columns in @ref inners (that is, the number of posterior distrbutions) can be different from the number of outputs in @ref channel.
		 * See @ref labels for more details.
		 */
		std::vector<std::vector<long double> > inners;

		/**
		 * \brief A map from posterior dsitributions indexes to a set of channel outputs indexes.
		 *
		 * When we look at the hyper-distrubtion, we have the outer distribution and the inner distributions.
		 * Depending on the prior distribution and the channel, the hyper-distribution may have two different
		 * outputs which yield the same posterior distribution on the set of secrets, and in this case, there is no
		 * difference for the adversary observing both.
		 *
		 * Indeed we can simplify the representation of a hyper-distribution matrix mergin all equal columns
		 * in a single column, and sum their outer probabilities. We also delete a posterior column when
		 * its outer probability of ocurring is 0 (which means that this "world" has 0 probability of occuring).
		 *
		 * The map key is the index of a posterior distribution in the reduced hyper-distribution matrix.
		 *
		 * The map value of a key \c i is the set of channel outputs indexes which gave rise this single posterior distribution \c i.
		 *
		 * Example: If the outputs 2 and 4 from the @ref channel yield the same posterior distribution, they are merged in, for example,
		 * the \c 3rd posterior distribution. Then, when you call \c labels\c[2\c] (the 3rd posterior has index 2), you will have the set {2,4}.
		 */
		std::map<int, std::set<int> > labels;


		/** \brief Generates a string with the @ref joint matrix, the @ref outer distribution, the @ref inner distributions or 
		 * the @ref labels.
		 *
		 * \return Possible returns:
		 *
		 * - The @ref joint matrix is returned in the following format:
		 *
		 * 	  p11 p12 ... p1y
		 * 	  p21 p22 ... p2y
		 * 	  ...
		 * 	  pn1 pn2 ... pny
		 *
		 * 	 where \c n is the number of secrets, \c y is the number of channel outputs and \c pij is
		 * 	 the joint probability _p(i,j)_.
		 *
		 * - The @ref outer distribution is returned in the following format:
		 *
		 * 	  p1 p2 ... py
		 *
		 *   where \c y is the number of outputs.
		 *
		 * - The @ref inners matrix is returned in the following format:
		 *
		 * 	  p11 p12 ... p1y
		 * 	  p21 p22 ... p2y
		 * 	  ...
		 * 	  pn1 pn2 ... pny
		 *
		 *   where \c n is the number of secrets, \c y is the number of outputs and \c pij is
		 *   the conditional probability _p(i|j)_.
		 *
		 * - The @ref labels are returned in the following format:
		 *
		 * 	  1: x11 x12 ...
		 * 	  2: x21 x22 ...
		 * 	  ...
		 * 	  3: xn1 xn2 ...
		 *
		 *   where \c n is the number of posteriors (@ref num_post) and \c xij is an integer between 0 and the number of outputs in @ref channel.
		 *
		 * Each two numbers in any choice are separated by a space (" ").
		 *
		 * \param type The name of what will be returned. It must be "labels" to return the @ref labels, "joint",
		 * "outer" or "inners", to return the @ref joint matrix, @ref outer distribution
		 * or the @ref inners' matrix, respectively.
		 *
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		std::string toString(std::string type, int precision);

		/**
		 * \brief Prints the joint matrix, the outer distribution, the inners or the labels. in a file.
		 *
		 * - The @ref joint matrix is printed in the following format:
		 *
		 * 	  n y
		 * 	  p11 p12 ... p1y
		 * 	  p21 p22 ... p2y
		 * 	  ...
		 * 	  pn1 pn2 ... pny
		 *
		 * 	 where \c n is the number of secrets, \c y is the number of channel outputs and \c pij is
		 * 	 the joint probability _p(i,j)_.
		 *
		 * - The @ref outer distribution is printed in the following format:
		 *
		 * 	  y
		 * 	  p1
		 * 	  p2
		 * 	  ...
		 * 	  py
		 *
		 *   where \c y is the number of posterior distributions (@ref num_post) and \c pi is the probability _p(i)_
		 *	 of the ith posterior distribution ocurring.
		 *
		 * - The @ref inners matrix is printed in the following format:
		 *
		 * 	  n y
		 * 	  p11 p12 ... p1y
		 * 	  p21 p22 ... p2y
		 * 	  ...
		 * 	  pn1 pn2 ... pny
		 *
		 *   where \c n is the number of secrets, \c y is the number of posterior distributions (@ref num_post)
		 *	 and \c pij is the conditional probability _p(i|j)_.
		 *
		 * - The @ref labels are printed in the following format:
		 *
		 * 	  y
		 * 	  1: x11 x12 ...
		 * 	  2: x21 x22 ...
		 * 	  ...
		 * 	  y: xy1 xy2 ...
		 *
		 *   where \c y is the number of posterior distributions (@ref num_post) and \c xij is an integer between 0 and the number of outputs in @ref channel.
		 *
		 * Each two numbers in any choice are are separated by a space (" ").
		 *
		 * \param type The name of what will be returned. It must be "labels" to return the @ref labels, "joint",
		 * "outer" or "inners", to return the @ref joint matrix, @ref outer distribution
		 * or the @ref inners' matrix, respectively.
		 *
		 * \param file File name to print one of the four options.
		 *
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string type, std::string file, int precision);

		/**
		 * \brief Given a new prior distribution, rebuild the joint, outer and inner distributions.
		 *
		 * \param prior Prior distribution on a set of secrets.
		 *
		 * \warning The number of rows in the current channel matrix must be equals to the number of elements in the new prior distribution.
		 */
		void rebuildHyper(Distribution &prior);

		/**
		 * \brief Given a new channel, rebuild the joint, outer and inner distributions.
		 *
		 * \param channel Channel matrix.
		 *
		 * \warning The number of rows in the new channel matrix must be equals to the number of elements in the current prior distribution.
		 */
		void rebuildHyper(Channel &channel);
};

#endif
