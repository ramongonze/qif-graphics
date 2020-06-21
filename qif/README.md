# QIF Library

This is a C++ library and it is meant to model QIF - Quantitative Information Flow.

It can be used to model systems and measure their entropies, vulnerabilities and information leakages.

This library models the basics of QIF such as probability distributions, informational channels, gain functions, hyper-distributions, and some functions to measure information flow.

## Library Documentation

The documentation is available in HTML format. To access the documentation just open ```docs/build/html/index.html``` in a browser.

## System Requirements

- GNU **GCC** project C and C++11 compiler - Available on most UNIX distributions. For more details access [GCC documentation](https://gcc.gnu.org/projects/cxx-status.html#cxx11).
 
- GNU **ar**. Available on most UNIX distributions. For more details access [ar documentation](http://man7.org/linux/man-pages/man1/ar.1.html).

- GNU **make**. Available on most UNIX distributions. For more details access [make documentation](https://www.gnu.org/software/make/).

## Usage

To use the library follow these steps:

1. Clone this repository;

2. Copy the folder ```qif``` to your project's directory;

3. Open the folder ```qif``` in the terminal and type ```make``` to compile all the modules. 
A binary file named ```qif.a``` will be generated;

4. Include the library in your C++ project using ```#include "qif/qif.hpp"```;

5. Compile your project adding ```qif/qif.a``` in the command line used to compile your C++ project. 

Example:
```bash
$ g++ main.cpp qif/qif.a
```

Following the steps above you will be able to use all the classes and functions from this library.

## References

[[1]](https://www.springer.com/us/book/9783319961293) Alvim M.S; Chatzikokolakis K; McIver A; Morgan C; Palamidessi C; Smith G.S. **The Science of Quantitative Information Flow**. Springer, 1 edition, 2019.

## License
[MIT](https://choosealicense.com/licenses/mit/)
