#ifndef _channel
#define _channel

#include "distribution.h"

/**
 * \brief Informational channels.
 *
 * A channel is a system that takes as input a secret _x_, whose possible
 * values come from a finite set **X** , and whose only observable behavior is to produce an
 * output _y_, whose possible values come from a finite set **Y**.
 *
 * A @ref Channel object has three attributes:
 * 
 * - @ref matrix : A channel matrix from **X** to **Y** whose rows give the 
 * distribution on outputs corresponding to each possible input. That is, entry 
 * matrix[_x_][_y_] denotes _p(y|x)_, the conditional probability of getting output
 * _y_ given input _x_. Note that all entries in a channel matrix are between 0 and 1,
 * and each row sums to 1.
 *
 * - @ref prior : A @ref Distribution object, which represents
 * the prior probability distribution on the set of secrets.
 *
 * - @ref num_out : The number of outputs in the channel. It is exactly the number of columns in @ref matrix.
 *
 * \warning To create a @ref Channel object you need first to create a @ref Distribution object, to keep the prior
 * distribution on the set of secrets.
 */
class Channel{
	public:
		/**
		 * \brief Default constructor
		 *
		 * This constructor creates a @ref Channel object with @ref num_out = 0, @ref prior = NULL and an empty @ref matrix.
		 */
		Channel();
		
		/**
		 * \brief Construtor used when the channel matrix is in a file.
		 * 
		 * The file format must be the following:
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
		 * \param prior Prior distribution on the set of secrets.
		 * \param file File name that contains a channel matrix.
		 *
		 * \warning The number of rows in the channel matrix must be equals to @ref prior -> num_el.
		 *
		 */
		Channel(Distribution &prior, std::string file);

		/**
		 * \brief Constructor used when there is already a channel matrix in a variable.
		 *
		 * \param prior: Prior distribution on the set of secrets.
		 * \param matrix: A channel matrix.
		 * 
		 * \warning The number of rows in the channel matrix must be equals to @ref prior -> num_el.
		 */
		Channel(Distribution &prior, std::vector<std::vector<long double> > &matrix);

		/**
		 * \brief Constructor used to generate random channels.
		 *
		 * Given a set of secrets in the @ref prior, the constructor builds a random channel matrix.
		 * The number of the channel outputs is passed as an argument.
		 *
		 * \param prior: Prior distribution on the set of secrets.
		 * \param num_out: Number of outputs.
		 */
		Channel(Distribution &prior, int num_out);

		/**
		 * \brief Number of outputs in the channel.
		 *
		 * This attribute is exactly the number of columns in @ref matrix.
		 */
		int num_out;
		
		/**
		 * \brief A probability distribution on a set of secrets.
		 *
		 * See the documentation of @ref Distribution for more details. 
		 */
		Distribution prior;
		
		/**
		 * \brief Channel matrix.
		 *
		 * A channel matrix of a set of secrets **X** and a set of possible outputs **Y**,
		 * whose rows give the distribution on outputs corresponding to each 
		 * possible input. That is, entry _matrix[x][y]_ denotes _p(y|x)_, the conditional
		 * probability of getting output _y_ given input _x_. Note that all entries in a 
		 * channel matrix are between 0 and 1, and each row sums to 1.
		 *
		 * The rows and columns are indexed from the position 0 (1st element) to the
		 * position @ref prior ->num_el-1 (for rows) or @ref num_out-1 (for columns).
		 */
		std::vector<std::vector<long double> > matrix;

		/**
		 * \brief Verifies if a matrix is a valid channel matrix.
		 * 
		 * A channel matrix is valid if each row is a probability distribution.
		 *
		 * \param matrix A channel matrix
		 */
		static bool isChannel(std::vector<std::vector<long double> > &matrix);
		
		/**
		 * \brief Generates a string with the channel matrix.
		 *
		 * \return A string in the following format:
		 *
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets and \c y is the number of outputs.
		 * Each \c pij denotes _p(j|i)_, the conditional probability of getting
		 * output _j_ given input _i_. Each two numbers must be separated by a space (" ").
		 *
		 * \param precision Decimal precision for long double numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		std::string toString(int precision);

		/**
		 * \brief Prints the channel matrix in a file.
		 *
		 * The channel matrix is printed in the following format:
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
		 * \param file File name to print the channel matrix.
		 * \param precision Decimal precision for long double numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string, int precision);
};

#endif