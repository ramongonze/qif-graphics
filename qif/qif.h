#ifndef _qif
#define _qif

#include "src/distribution.h"
#include "src/channel.h"
#include "src/gain.h"
#include "src/hyper.h"
#include "src/vulnerability.h"
#include "src/entropy.h"
#include "src/leakage.h"

#endif

/** 
 * \mainpage
 *
 * \section description Description
 *
 * This is a C++ library and it is meant to model QIF - Quantitative Information Flow.
 *
 * It can be used to model systems and measure their entropies, vulnerabilities and information leakages.
 *
 * This library models the basics of QIF such as probability distributions, informational channels, gain functions, hyper-distributions,
 * and some functions to measure information flow.
 *
 * \line l1
 *
 * \section req System Requirements
 *
 * - GNU **GCC** project C and C++ compiler - Available on most UNIX disrtibutions. For more details access [GCC documentation](https://gcc.gnu.org/onlinedocs/).
 *
 * - GNU **ar**. Available on most UNIX disrtibutions. For more details acess [ar documentation](http://man7.org/linux/man-pages/man1/ar.1.html).
 *
 * - GNU **make**. Available on most UNIX distributions. For more details access [make documentation](https://www.gnu.org/software/make/).
 *
 * \line l9
 *
 * \section usage Usage
 *
 * To use the library follow these steps:
 * 
 * 1. Copy the folder ```qif``` to your project's directory;
 * 
 * 2. Open the folder ```qif``` in the terminal and type ```make``` to compile all the modules. 
 * A binary file named ```qif.a``` will be generated;
 * 
 * 3. Include the library in your C++ project using ```#include "qif/qif.hpp"```;
 * 
 * 4. Compile your project adding ```qif/qif.a``` in the command line used to compile your C++ project. 
 * 
 * Example:
 * ```bash
 * $ g++ main.cpp qif/qif.a
 * ```
 *
 * \line l2
 *
 * \section References
 * [[1]] (https://www.springer.com/us/book/9783319961293) Alvim M.S; Chatzikokolakis K; McIver A; Morgan C; Palamidessi C; Smith G.S.
 * **The Science of Quantitative Information Flow**. Springer, 1 edition, 2019.
 *
 * \line l3
 *
 * \author Gonze, R.
 * \version 1.0
 * \date 2019-04
 *
 * \section license License
 *
 * [MIT](https://choosealicense.com/licenses/mit/)
 */