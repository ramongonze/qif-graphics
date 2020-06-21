#ifndef _gain
#define _gain

#include "distribution.h"

/**
 * \brief Gain functions.
 *
 * Definition of a gain function:
 * 
 * Given a finite, non-empty set **X** (of possible secret values) and a non-empty set
 * **W** (of possible actions), a gain function is a function _g_: **W** x **X** → R.
 * The idea is that _g(w, x)_ specifies the gain that the adversary achieves by taking
 * action _w_ when the actual value of the secret is _x_.
 * 
 * A @ref Gain object has 3 attributes:
 *
 * - @ref prior : A @ref Distribution object, which represents
 * the prior probability distribution on the set of secrets.
 *
 * - @ref matrix : A matrix \c w x \c n where \c w is the number of actions that 
 * the adversary can take and \c n is the number of secrets.
 *
 * - @ref num_act : Number of distinct actions that an adversary can take.
 *
 */
class Gain{
	
	public:
		/**
		 * \brief Default constructor
		 *
		 * This constructor creates a @ref Gain object with @ref num_act = 0, @ref prior = NULL
		 * and empty @ref matrix.
		 */

		Gain();

		/**
		 * \brief Construtor used when the gain function matrix is in a file.
		 *
		 * The file format must be the following:
		 *
		 * 	w n
		 * 	g11 g12 ... g1n
		 * 	g21 g22 ... g2n
		 * 	...
		 * 	gw1 gw2 ... gwn
		 *
		 * where \c w is @ref num_act, and \c n is the number of secrets.
		 * Each gain \c gij is the gain of taking action \c i when the secret value is \c j.
		 * Each two numbers must be separated by a space (" ").
		 * 
		 * \param prior: Prior distribution on the set of secrets
		 * \param file: File name which contains a gain function matrix.
		 *
		 * \warning The number of columns in the gain function matrix must be the same as 
		 * @ref prior -> num_el.
		 */
		Gain(Distribution &prior, std::string file);

		/**
		 * \brief Constructor used when there is already a gain function matrix in a variable. 
		 *
		 * \param prior Prior distribution on the set of secrets
		 * \param matrix A gain function matrix
		 * 
		 * \warning The number of columns in the gain function matrix must be the same as 
		 * @ref prior -> num_el.
		 */
		Gain(Distribution &prior, std::vector<std::vector<long double> > &matrix);

		/**
		 * \brief Constructor used to generate a random gain function.
		 *
		 * \param prior: Prior distribution on the set of secrets.
		 * \param num_act: Number of actions.
		 * \param MIN: Lower bound for the gain of an action.
		 * \param MAX: Upper bound for the gain of an action.
		 *
		 * The parameters MIN and MAX must form an interval. All the gains will be generated
		 * randomly in the interval [MIN,MAX].
		 */
		Gain(Distribution &prior, int num_act, int MIN, int MAX);

		/**
		 * \brief Number of distinct actions that an adversary can take.
		 *
		 * This attribute is exactly the number of rows in @ref matrix.
		 */
		int num_act;

		/**
		 * \brief A probability distribution on a set of secrets.
		 *
		 * It is a distribution on the set of secrets. See @ref Distribution for more details.
		 */
		Distribution prior;
		
		/**
		 * \brief Gain function matrix.
		 *
		 * A matrix \c w x \c n where \c w is the number of actions that 
 		 * the adversary can take and \c n is the number of secrets.
 		 * Each value _matrix[i][j]_ is the gain that the adversary achieves by taking action _i_
 		 * when the actual value of the secret is _j_.
		 *
		 * The rows and columns are indexed from the position 0 (1st element) to the
		 * position @ref num_act-1 (for rows) or @ref prior ->num_el-1 (for columns).
		 */
		std::vector<std::vector<long double> > matrix;

		/**
		 * \brief Generates a string with the gain function matrix.
		 * 
		 * \return The gain function @ref matrix is returned in the following format:
		 *
		 * 	g11 g12 ... g1n
		 * 	g21 g22 ... g2n
		 * 	...
		 * 	gw1 gw2 ... gwn
		 *
		 * where each gain \c gij is the gain of taking action \c i when the secret value is \c j.
		 * Each two numbers are separated by a space (" ").
		 *
		 * \param precision Decimal precision for long double numbers.
		 * For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		std::string toString(int precision);

		/**
		 * \brief Prints the gain function matrix in a file.
		 *
		 * The gain function @ref matrix is printed in the following format:
		 *
		 * 	w n
		 * 	g11 g12 ... g1n
		 * 	g21 g22 ... g2n
		 * 	...
		 * 	gw1 gw2 ... gwn
		 *
		 * where \c w is @ref num_act, and \c n is the number of secrets.
		 * Each gain \c gij is the gain of taking action \c i when the secret value is \c j.
		 * Each two numbers are separated by a space (" ").
		 *
		 * \param file File name to print the gain function matrix.
		 * \param precision Decimal precision for long double numbers.
		 * For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string file, int precision);
};

#endif