QIF Library
===========

This is a C++ library and it is meant to model QIF - Quantitative Information Flow.

It can be used to model systems and measure their entropies, vulnerabilities and information leakages.

This library models the basics of QIF such as probability distributions, informational channels, gain functions, hyper-distributions, and some functions to measure information flow.


.. toctree::
   :hidden:
   :maxdepth: 2

   getting-started
   classes
   functions
   license

.. highlight:: cpp

.. role:: bash(code)
   :language: bash

**********************************************

System requirements
===================
* GNU **gcc** project C and C++11 compiler - Available on most UNIX distributions. For more details access `gcc documentation <https://gcc.gnu.org/projects/cxx-status.html#cxx11>`_.
* GNU **ar**. Available on most UNIX distributions. For more details acess `ar documentation <http://man7.org/linux/man-pages/man1/ar.1.html>`_.
* GNU **make**. Available on most UNIX distributions. For more details access `make documentation <https://www.gnu.org/software/make/>`_.

Usage
=====
To use the library follow these steps:

1. Copy the folder :bash:`qif` to your project's directory;
2. Using the terminal, go to the folder :bash:`qif` type :bash:`make` to compile 
   all the modules. A binary file named :bash:`qif.a` will be generated;
3. Include the library in your C++ project using :code:`#include "qif/qif.hpp"`;
4. Compile your project adding :code:`qif/qif.a` in the command line used to compile your 
   C++ project.

Example: :bash:`$ g++ main.cpp qif/qif.a`.

**********************************************

References
==========
`[1] <https://www.springer.com/us/book/9783319961293>`_ Alvim M.S; Chatzikokolakis K; McIver A; Morgan C; Palamidessi C; Smith G.S. **The Science of Quantitative Information Flow**. Springer, 1 edition, 2019.

.. Indices and tables
.. ==================

.. * :ref:`genindex`
.. * :ref:`modindex`
.. * :ref:`search`
