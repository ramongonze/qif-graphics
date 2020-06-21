.. highlight:: cpp

.. role:: bash(code)
   :language: bash

Getting started
===============

Here is a tutorial to explain how to create and use distributions, channels, 
functions and so on.

.. admonition:: Content

    * `Distributions`_
      
      * `Creating distributions`_
      * `Using distributions`_
    
    * `Channels`_

      * `Creating channels`_
      * `Using channels`_

    * `Hyper-distributions`_
      
      * `Creating hyper-distributions`_
      * `Using hyper-distributions`_

    * `Gain functions`_
      
      * `Creating gain functions`_
      * `Using gain functions`_
    
    * `Functions to measure vulnerabilities, entropies and leakages`_

*****************************

Distributions
*************

Creating distributions
----------------------

The vector of probabilities is structed as a :code:`std::vector<long double>`.

There are 3 options available to instantiate a :cpp:class:`Distribution` object:

* **Read the vector of probabilities from a file**

    The vector of probabilities can be read from a file in the following format: ::

        n
        p1
        p2
        ...
        pn

    where :code:`n` is the number of elements in the distribution and :code:`pi` is 
    the probability of the ith element occurs.

    Suppose you have a file called :bash:`prior_distribution.txt` containing the vector 
    of probabilities. To create a distribution, you have to type: ::

        Distribution dist("prior_distribution.txt");

* **Use an existing vector of probabilities**

    A distribution can be created using a vector stored in a variable.
    The type of the vector must be :code:`std::vector<long double>`.

    To create a distribution from a vector of probabilities :code:`v` you have to 
    type: ::

        std::vector<long double> v({0.25, 0.25, 0.5});
        Distribution dist(v);

* **Create a random or uniform distribution**

    To create an uniform or a random distribution, you have to give the number of
    elements as the first parameter and :code:`"uniform"` or :code:`"random"` as
    the second parameter.

    For example, to create two uniform distributions with 5 and 10 elements 
    respectively, you have to type: ::

        Distribution dist(5, "uniform");
        Distribution dist(10, "random");

.. seealso:: For more details, see 
    :ref:`Distribution constructors <distribution-constructors>`.

**************************************************


Using distributions
-------------------

Suppose you have created a new distribution using one of the constructors 
above and stored it in the variable :code:`dist`.

Accessing the elements
######################

To get the probability of each element, you have to access the attribute 
:code:`prob`. The elements in :code:`prob` are indexed starting at position 0.
For example, to get the fourth element of the distribution, you use the index 3: ::

    long double prob_fourth_element = dist.prob[3];

Getting the number of elements
##############################

The number of elements is stored in the attribute :code:`num_el`: ::
    
    int number_elements = dist.num_el;

Printing
########

To print the distribution on the screen, you can use the method :code:`toString()`.
The method has one parameter: the precision of the probabilities, and it 
returns a :code:`std::string`.

For example, if you want to get a string with precision of 4, you have to type: ::
    
    std::string str_dist = dist.toString(4);
    std::cout << str_dist << std::endl;

The result is the following: ::

    0.2500 0.2500 0.5000

For more details, see the documentation of :cpp:func:`Distribution::toString()`.
You can also print the distribution right into a file using 
:cpp:func:`Distribution::printToFile()`.

For more details about the class, see the documentation 
:ref:`here <distribution-class>`.


**************************************************

Channels
********

Creating channels
-----------------

An informational channel can be represented as a matrix :math:`n \times y`, where 
:math:`n` is the number of secrets (the rows), :math:`y` is the number of possible 
outputs (the columns) and each value stored at :code:`matrix[x][y]` is the 
conditional probability :math:`p(y|x)`. Before creating a channel, you must have
already created an object :cpp:class:`Distribution` that contains the prior 
distribution on the set of secrets.

.. note:: Before creating a channel, you have to create a :cpp:class:`Distribution`
    that corresponds to the prior distribution on the set of secrets.

There are 3 options available to instantiate a :cpp:class:`Channel` object:

* **Read the channel from a file**

    You can read the channel from a file in the following format: ::

        n y
        p11 p12 ... p1y
        p21 p22 ... p2y
        ...
        pn1 pn2 ... pny

    where :code:`n` is the number of secrets, :code:`y` is the number of outputs 
    and each two values must be separated by a space.
    
    Suppose you have a file called 
    :code:`channel.txt`. To create a channel you have to type: ::

        Distribution dist("prior_distribution.txt");
        Channel channel(dist, "channel.txt");

* **Use an existing matrix of numbers**

    The type of the matrix must be :code:`std::vector<std::vector<long double>>`.
    To create a channel from a matrix :code:`M` you have to type: ::

        Distribution prior("prior_distribution.txt");
        std::vector<std::vector<long double>> M({{0.25, 0.25, 0.50},
                                                 {0.30, 0.30, 0.40},
                                                 {1.00, 0.00, 0.00}});
        Channel channel(prior, M);

* **Generate a random channel**

    To create a random channel you have to give the prior distribution as the
    first parameter and the number of outputs as the second parameter.

    For example, to create a channel with 3 outputs, you have to type: ::

        Distribution prior("prior_distribution.txt");
        Channel channel(prior, 3);


.. seealso:: For more details, see :ref:`Channel constructors <channel-constructors>`.

**************************************************

Using channels
--------------

Suppose you have created a channel using one of the constructors above and stored 
it in the variable :code:`channel`.

Acessing the prior distribution
###############################

You can use the attribute :cpp:member:`Channel::prior`. You can get, for example, 
the number of secrets: ::

    int number_secrets = channel.prior.num_el;

Acessing the channel matrix
###########################

To get a conditional probability from the channel matrix, you have to access the 
attribute :cpp:member:`Channel::matrix`. Both rows and columns are indexed 
starting at the position 0.

For example, to get the probability :math:`p(y_3|x_2)`, you have to type: ::

    long double p = channel.matrix[2][1];

Getting the number of outputs from the channel
##############################################

The number of outputs is stored in the attribute :cpp:member:`Channel::num_out`: ::

    int number_outputs = channel.num_out;

Printing
########

To print the channel on the screen, you can use :cpp:func:`Channel::toString()`.
The method has one parameter: the precision of the probabilities, and it 
returns a :code:`std::string`.

For example, if you want to get a string with precision of 4, you have to type: ::

    std::string str_channel = channel.toString(4);
    std::cout << str_channel << std::endl;

The result is the following: ::

    0.2500 0.2500 0.5000
    1.0000 0.0000 0.0000
    0.9000 0.0100 0.0900

For more details, see the documentation of :cpp:func:`Channel::toString()`.
You can also print the channel right into a file using 
:cpp:func:`Channel::printToFile()`.

For more details about the class, see the documentation 
:ref:`here <channel-class>`.

**************************************************

Hyper-distributions
*******************

A hyper-distribution is a distribution on distributions on a set of secrets.
Each output of an informational channel is a possible "world", and each possible 
world is a new distribution on the set of secrets. We call the possible worlds 
the *inner distributions*. Each possible world has a probability to occur, and we 
call the distribution on the possible worlds as the *outer distribution*.

Creating hyper-distributions
----------------------------

There are 2 options available to instantiate a :cpp:class:`Hyper` object:

* **Read the prior distribution and the channel from files**
    
    You can read the prior distribution on a set of secrets and the channel from
    files.

    The prior distribution must be in the following format: ::
        
        n
        p1
        p2
        ...
        pn

    where :code:`n` is the number of secrets and :code:`pi` is the probability of the 
    ith secret occuring.

    The channel must be in the following format: ::
        
        n y
        p11 p12 ... p1y
        p21 p22 ... p2y
        ...
        pn1 pn2 ... pny

    where :code:`n` is the number of secrets (the rows), :code:`y` is the number 
    of possible outputs (the columns) and each value :code:`pij` is the conditional 
    probability :math:`p(j|i)`. Each two values must be separated by a space.

    Suppose you have the prior distribution in a file called 
    :code:`prior_distribution.txt` and a channel in a file called 
    :code:`channel.txt`. To create the hyper-distribution you have to type: ::
        
        Hyper hyper("prior_distribution.txt", "channel.txt");

* **Use existing Distribution and Channel objects**
    
    As a Channel has the attribute :cpp:member:`Channel::prior`, it is not 
    necessary to give a prior distribution as a parameter. So to create a 
    hyper-distribution, you can give only the channel: ::

        Distribution prior("prior_distribution.txt");
        Channel channel(prior, "channel.txt");
        Hyper hyper(channel);

.. seealso:: For more details, see :ref:`Hyper constructors <hyper-constructors>`.


Using hyper-distributions
-------------------------

Suppose you have created a Hyper object and stored it in a variable :code:`hyper`.

* **Accessing the prior distribution and the channel**

    The access to the prior distribution can be made by :cpp:member:`Hyper::prior`
    and the access to the channel by :cpp:member:`Hyper::channel`.

    For example, you can access the number of secrets and the number of channel 
    outputs: ::

        int number_secrets = hyper.prior.num_el;
        int number_outputs = hyper.channel.num_out;

* **Accessing the joint distribution**

    The attribute :cpp:member:`Hyper::joint` represents the joint distribution. 
    It is a matrix (:code:`std::vector<std::vector<long double>>`) 
    :math:`n \times y` where :math:`n` is the number of secrets and :math:`y` is 
    the number of outputs of the channel. Both rows and columns are indexed 
    starting at position 0.

    For example, to get the joint probability :math:`p(x_2,y_1)`, you have to 
    type: ::

        long double joint_x2_y1 = hyper.joint[1][0]


* **Accessing the outer distributon**

    The attribute :cpp:member:`Hyper::outer` represents the outer distribution, 
    and it is a :cpp:class:`Distribution` object.

    For example, to access the probability :math:`p(y2)` of the 2nd posterior 
    occurring, you have to type: ::
    
        long double prob_second_posterior = hyper.outer.prob[1];

* **Accessing the inner distributions**

    The attribute :cpp:member:`Hyper::inners` keeps the inner distributions. 
    It is a matrix (:code:`std::vector<std::vector<long double>>`) 
    :math:`n \times y` where :math:`n` is the number of secrets and :math:`y` is 
    the number of posterior distributions. Both rows and columns are indexed 
    starting at position 0.

    For example, to access the conditional probability :math:`p(x2|y1)` you have 
    to type: ::

        long double prob_x2_y1 = hyper.inners[1][0];

* **Changing the prior distribution or the channel**

    If you have already created a hyper-distribution and need to change the prior 
    distribution or the channel, it is possible to do it using the function 
    :cpp:func:`Hyper::rebuildHyper`.

    To change the prior distribution or the channel, you have to type: ::

        // Original hyper-distribution
        Hyper hyper("prior.txt", "channel.txt");

        // Creating new prior and channel
        Distribution new_prior("another_prior.txt");
        Channel new_channel("another_channel.txt");

        // Rebuilding the hyper-distribution using the new prior
        hyper.rebuildHyper(new_prior);

        // Or rebuild using a new channel
        hyper.rebuildHyper(new_channel);

    .. seealso:: For more details, see :cpp:func:`Hyper::rebuildHyper`.

* **Printing**

    To print the joint, the outer, the inners or the labels, you can use 
    :cpp:func:`Hyper::toString()`. The first parameter must be :code:`"joint"`,
    :code:`"outer"`, :code:`"inners"` or :code:`"labels"`, and the second 
    parameter the precision of probabilities. This method returns a 
    :code:`std::string`.

    For example, if you want to get a string of the joint matrix with precision 
    of 3, you have to type: ::

        std::string joint_matrix = hyper.toString("joint", 3);
        std::cout << joint_matrix << std::endl;

    The output will be the following: ::

        0.062 0.062 0.125
        0.250 0.000 0.000
        0.000 0.500 0.000

    .. note:: The :cpp:member:`Hyper::labels` is a map from posterior distribution
        indexes (in the matrix) to a set of channel output indexes. It is used
        to store the relationship between posterior distributions and channel 
        outputs.

    You can also print all elements from right into a file using 
    :cpp:func:`Hyper::printToFile()`.

For more details about the class, see the documentation 
:ref:`here <hyper-class>`.
    
**************************************************

Gain functions
**************

A gain function can be represented as a matrix :math:`w \times n`, where 
:math:`w` is the number of actions that the adversary can take and :math:`n` is 
the number of secrets. Each value :math:`gain(w_i,x_j)` corresponds to the gain 
that the adversary achieves when taking the action :math:`w_i` and the real value 
of the secret is :math:`x_j`.

Creating gain functions
-----------------------

.. note:: Before creating a gain function, you have to create a 
    :cpp:class:`Distribution` that corresponds to the prior distribution on the 
    set of secrets.

There are 3 options available to instantiate a :cpp:class:`Gain` object:

* **Read the gain function matrix from a file**

    You can read the gain function matrix from a file in the following format: ::

        w n
        p11 p12 ... p1n
        p21 p22 ... p2n
        ...
        pw1 pw2 ... pwn

    where :code:`w` is the number of actions, :code:`n` is the number of secrets 
    and each two values must be separated by a space.

    Suppose you have a file called :code:`gain.txt`. To create a gain function you 
    have to type: ::

        Distribution prior("prior_distribution.txt");
        Gain gain(prior, "gain.txt");

    .. warning:: The number of columns in the gain function matrix must be equal 
        to the number of elements in the prior distribution.

* **Use an existing matrix of numbers**

    The type of the matrix must be :code:`std::vector<std::<long double>>`.

    To crete a gain function from a matrix :code:`M` you have to type: ::

        Distribution prior("prior_distribution.txt");
        std::vector<std::vector<long double>> M({{10, 20, 30},
                                                 {-5, -6, 10},
                                                 { 0, 10,  0}});
        Gain gain(prior, M);

    .. warning:: The number of columns in matrix :code:`M` must be equal to the 
        number of elements in the prior distribution.

* **Generate a random gain function**

    To create a random gain function, there are 4 parameters in the constructor:

    - :code:`prior`: Prior distribution on the set of secrets;
    - :code:`num_act`: Number of actions that the adversary can take;
    - :code:`MIN`: Lower bound for the gain of an action;
    - :code:`MAX`: Upper bound for the gain of an action.

    For example, to create a gain function with 7 actions which give a gain in the 
    interval :math:`[-5,5]`, you have to type: ::

        Distribution prior("prior_distribution.txt");
        Gain gain(prior, 7, -5, 5);

For more details see :ref:`Gain constructors <gain-constructors>`.

Using gain functions
--------------------

Suppose you have created a gain function using one of the constructors above and 
stored it in the variable :code:`gain`.

* **Accessing the prior distribution**

    The access to the prior distribution can be done by :cpp:member:`Gain::prior`. 
    You can get, for example, the number of secrets: ::

        int number_secrets = gain.prior.num_el;

* **Accesing the gain function matrix**

    To get a gain for an action and a secret, you can use 
    :cpp:member:`Gain::matrix`. Both rows and columns are indexed starting at 
    position 0.

    For example, to get the gain that the adversary achieves by taking action 
    :math:`w_4` when the actual value of the secret is :math:`n_2`, you have to 
    type: ::

        long double gain_w4_n2 = gain.matrix[3][1];

    
* **Getting the number of actions from the gain function**

    The number of actions is stored in :cpp:member:`Gain::num_act`: ::

        int number_actions = gain.num_act;

* **Printing**

    To print the gain function matrix on the screen, you can use 
    :cpp:func:`Gain::toString()`.
    The method has one parameter: the precision of the probabilities, and it 
    returns a :code:`std::string`.

    For example, if you want to get a string with precision of 2, you have to 
    type: ::

        std::string str_gain = gain.toString(2);
        std::cout << str_gain << std::endl;

    The output will be the following: ::

        50.00 -6.00 10.00
        -9.00 25.00 -9.00
        -3.00 12.00 33.00

    For more details, see :cpp:func:`Gain::toString()`.
    You can also print the gain function matrix right into a file using 
    :cpp:func:`Gain::printToFile()`.

For more details about the class, see the documentation 
:ref:`here <gain-class>`.

**************************************************

Functions to measure vulnerabilities, entropies and leakages
************************************************************

This library offers some functions to calculate vulnerabilities, leakages and 
entropies of systems. Here is a list of all available functions:

Vulnerability measures
----------------------

* :ref:`Bayes Vulnerability <bayes-vulnerability>`
* :ref:`g-vulnerability <g-vulnerability>`
* :ref:`k-tries <k-tries>`

Leakage measures
----------------

* :ref:`Additive leakage <additive-leakage>`
* :ref:`Multiplicative leakage <multiplicative-leakage>`

Entropy measures
----------------

* :ref:`Guessing entropy <guessing-entropy>`
* :ref:`Shannon entropy <shannon-entropy>`