.. highlight:: cpp

.. _entropy:

Entropy
*******

.. _guessing-entropy:

.. cpp:function:: long double guessingEntropy(Distribution& prior)

    Guessing entropy of a prior probability distribution.

    :param prior: Prior distribution on the set of secrets.

    :return: The expected number of tries needed by the adversary to guess the 
        secret correctly.

.. cpp:function:: long double guessingEntropy(Hyper& hyper)

    Guessing entropy of a hyper-distribution.

    :param hyper: A hyper-distribution. 
    
    :return: The expected number of tries needed by the adversary to guess the 
        secret correctly, looking to all possible worlds (inner distributions).

.. _shannon-entropy:

.. cpp:function:: long double shannonEntropy(Distribution& dist)

    Shannon entropy of a probability distribution is a way to measure the secrecy 
    of a secret.

    :param dist: Prior distribution on a set of secrets.


    :return: Shannon entropy of a probability distribution. The returned value is 
        in the interval :math:`[0,\log_2 (n)]`, where :math:`n` is the number of 
        elements in the distribution.

.. cpp:function:: long double shannonEntropy(Hyper& hyper)

    The Shannon entropy of a hyper-distribution is a way to measure the secrecy 
    of a secret, looking to all possible worlds (inner distributions).

    :param hyper: A hyper-distribution.
    
    :return: Shannon entropy of a hyper-distribution. The returned value is in the
        interval :math:`[0,\log_2 (y)]`, where :math:`y` is the number of outputs of
        the channel pointed by :cpp:var:`hyper`.

    