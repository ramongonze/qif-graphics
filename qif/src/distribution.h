#ifndef _distribution
#define _distribution

#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>

/**
 * \brief Epsilon used to compare long double numbers.
 *
 * As computers have limit to store long double numbers, a comparision between two long double 
 * numbers is not well defined (precision erros can happen).
 *
 * For this library, we consider two numbers as equals if their difference is less than EPS,
 * which the default value is 1e-6.
 */
#define EPS 1e-6

/**
 * \brief Probability distributions on sets of elements.
 * 
 * A @ref Distribution object has two attributes:
 *
 * - @ref num_el : The number of elements in the set.
 * - @ref prob : A vector containing the probability of each element from the set.
 *
 * The vector @ref prob is valid when all the elements are in the interval [0,1] and
 * the sum of all numbers in the vector is 1, with margin of error (upper or lower) equals
 * to a constant _epsilon_, by default set as _10e-6_.
 *
 */
class Distribution{
	public:
		/**
		 * \brief Default constructor.
		 *
		 * This constructor creates a @ref Distribution object with @ref num_el = 0 and an empty vector @ref prob.
		 */
		Distribution();
		
		/** 
		 * \brief Constructor used when the probability distribution is in a file.
		 *
		 * The file format must be the following:
		 *
		 * 	n
		 * 	p1
		 * 	p2
		 * 	...
		 * 	pn
		 *
		 * where \c n is the number of elements in the distribution and \c pi is the probability of the ith element occurs.
		 *
		 * \param file File name that contains a probability distribution.
		 */
		Distribution(std::string file);
		
		/**
		 * \brief Constructor used when there is already a probability distribution in a vector.
		 *
		 * \param prob A vector containing a probability distribution.
		 */
		Distribution(std::vector<long double> &prob);

		/**
		 * \brief Constructor used to generate a uniform or a random probability distribution.
		 *
		 * \param num_el Number of elements in the set of elements.
		 * \param type Must be "uniform" for generating a uniform distribution or "random" for a random distribution.
		 */
		Distribution(int num_el, std::string type);
		
		/** \brief Number of elements in the distribution
		 *
		 * The number of elements in the distribution is equal to @ref prob size.
		 */
		int num_el;

		/**
		 * \brief A vector of elements' probabilities.
		 * 
		 * The vector contains the probabilities of all elements from a set.
		 * Each value _prob[i]_ is the probability of the ith element occurs.
		 * The vector is indexed from the position 0 (1st element) to the
		 * position @ref num_el - 1 (last element). So the vector size = @ref num_el.
		 */
		std::vector<long double> prob;

	    /** 
	     * \brief Check if a vector of numbers contains a probability distribution.
	     *
	     * \param prob A vector containing a probability distribution.
	     *
	     * A vector contains a probability distribution if:
	     *
		 * - For every probability _p_ in the vector, 0 <= _p_ <= 1;
		 * - The sum of all probabilities in the vector is 1 , with margin of error (upper or lower) equals 
		 *   to a constant _epsilon_, by default set as _10e-6_.
		 *
		 * \return It returns \c true if the vector contains a probability distribution or \c false otherwise.
		 */
		static bool isDistribution(std::vector<long double> &prob);

		/**
		 * \brief Generates a string with the probability distribution.
		 * 
		 * \param precision Decimal precision for long double numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 *
		 * \return A string in the following format:
		 * 
		 * 	p1 p2 p3 ... pn
		 *
		 * where \c pi is the probability of the ith element occurring. Each two values are separated by a space (" ").
		 * 
		 */  
		std::string toString(int precision);

		/**
		 * \brief Prints the probability distribution in a file.
		 *
		 * The probability distribution is printed in the following format:
		 *
		 * 	n
		 * 	p1
		 * 	p2
		 * 	...
		 * 	pn
		 *
		 * where \c n is the number of elements in the probability distribution and \c pi is the probability _p(i) of the ith element occuring.
		 *
		 * \param file File name to print the distribution.
		 * \param precision Decimal precision for long double numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string file, int precision);
};

#endif