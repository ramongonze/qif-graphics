.. highlight:: cpp

.. _leakage-measures:

Leakage measures
****************

.. note::
    * :math:`V_g(\pi)`: Prior g-vulnerability for a prior distribution :math:`\pi`.
    * :math:`V_g([\pi \triangleright C])`: Posterior g-vulnerability of a 
      hyper-distribution :math:`[\pi \triangleright C]` resulted from a channel 
      :math:`C` and a prior :math:`\pi`.

.. _additive-leakage:

.. cpp:function:: long double additiveLeakage(Hyper& hyper, Gain& gain)

    The absolute difference between the posterior g-vulnerability and the prior 
    g-vulnerability.

    :param hyper: A hyper-distribution.

    :param gain: A gain function for a set of secrets.

    :return: :math:`V_g([\pi \triangleright C]) - V_g(\pi)`

.. _multiplicative-leakage:

.. cpp:function:: long double multiplicativeLeakage(Hyper& hyper, Gain& gain)

    The relative difference between the posterior g-vulnerability and the
    prior g-vulnerability.

    :param hyper: A hyper-distribution.

    :param gain: A gain function for a set of secrets.

    :return:
        * If :math:`V_g(\pi) \neq 0` and :math:`V_g([\pi \triangleright C]) \neq 0`
          it returns :math:`\frac{V_g([\pi \triangleright C])}{V_g(\pi)}`.
        * If :math:`V_g(\pi) = 0` and :math:`V_g([\pi \triangleright C]) = 0`
          it returns 1.
        * If :math:`V_g(\pi) = 0` and :math:`V_g([\pi \triangleright C]) \neq 0`
          it returns :math:`+\infty`.


    See the Definition 5.11 `here <https://www.springer.com/us/book/9783319961293>`_ for more details about the return 
    value of multiplicative leakage.