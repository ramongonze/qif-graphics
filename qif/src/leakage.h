#ifndef _leakage
#define _leakage

#include "hyper.h"
#include "gain.h"
#include "vulnerability.h"

#include <limits>

/* Leakage measures */

/**
 * \brief The absolute difference between the posterior g-vulnerability and the prior g-vulnerability.
 *
 * \param hyper A hyper-distribution.
 * \param gain A gain function for a set of secrets.
 *
 * \return Vg([π>C]) - Vg(π)
 */
long double additiveLeakage(Hyper &hyper, Gain &gain);

/**
 * \brief The relative difference between the posterior g-vulnerability and the prior g-vulnerability.
 *
 * \param hyper A hyper-distribution.
 * \param gain A gain function for a set of secrets.
 *
 * \return
 * - If Vg(π) != 0 and Vg([π>C]) != 0, 
 * it returns Vg([π>C]) / Vg(π).
 *
 * - If Vg(π) = 0 and Vg([π>C]) = 0, it returns 1.
 *
 * - If Vg(π) = 0 and Vg([π>C]) != 0, it returns +∞.
 *
 * See the section 5.2 in the reference [[1]](https://www.springer.com/us/book/9783319961293) 
 * for more details about the return value of multiplicative leakage.
 */
long double multiplicativeLeakage(Hyper &hyper, Gain &gain);

#endif
