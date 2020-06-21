.. highlight:: cpp

.. _gain-class:

**Gain** class
**************

Definition of a gain function:

Given a finite, non-empty set :math:`\mathcal{X}` (of possible secret values) and a 
non-empty set :math:`W` (of possible actions), a gain function is a function 
:math:`g: W \times \mathcal{X} \rightarrow R`. The idea is that :math:`g(w, x)` specifies the 
gain that the adversary achieves by taking action :math:`w` when the actual value 
of the secret is :math:`x`.

.. cpp:class:: Gain

    .. _gain-variables:

    **Variables**

    .. cpp:var:: int num_act

        Number of distinct actions that an adversary can take. This attribute is 
        exactly the number of rows in :cpp:var:`matrix`.

    .. cpp:var:: Distribution prior

        A probability distribution on a set of secrets. It is a distribution on 
        the set of secrets. See :cpp:class:`Distribution` for more details.

    .. cpp:var:: std::vector<std::vector<long double>> matrix

        Gain function matrix. It is a matrix :math:`w \times n` where :math:`w` 
        is the number of actions that the adversary can take and :math:`n` is the 
        number of secrets. Each value :code:`matrix[i][j]` is the gain that the 
        adversary achieves by taking action :code:`i` when the actual value of the 
        secret is :code:`j`.

        Both rows and columns are indexed starting at position 0.

    .. _gain-constructors:
    
    **Constructors**

    .. cpp:function:: Gain()

        Default constructor.
    
    .. cpp:function:: Gain(Distribution& prior, std::string file)

        Construtor used when the gain function matrix is in a file.

        :param prior: Prior distribution on the set of secrets.

        :param file: File name which contains a gain function matrix.
            The file format must be the following: ::

                w n
                g11 g12 ... g1n
                g21 g22 ... g2n
                ...
                gw1 gw2 ... gwn

            where :code:`w` is the number of actions (:cpp:var:`num_act`) and 
            :code:`n` is the number of secrets. Each value :code:`gij` is the gain
            of taking action :code:`i` when the secret value is :code:`j`. Each two
            numbers must be separated by a space.

        .. warning:: The number of columns in the gain function matrix must be the
            same as :cpp:var:`prior.num_el <Distribution::num_el>`, i.e. the number
            of secrets in the distribution given in the first parameter.

    .. cpp:function:: Gain(Distribution& prior, std::vector<std::vector<long double>>& matrix)

        Constructor used when there is already a gain function matrix in a variable.

        :param prior: Prior distribution on the set of secrets.

        :param matrix: A gain function matrix.

        .. warning:: The number of columns in the gain function matrix must be the
            same as :cpp:var:`prior.num_el <Distribution::num_el>`, i.e. the number
            of secrets in the distribution given in the first parameter.

    .. cpp:function:: Gain::Gain(Distribution& prior, int num_act, int MIN, int MAX)

        Constructor used to generate a random gain function.

        :param prior: Prior distribution on the set of secrets.

        :param num_act: Number of actions.

        :param MIN: Lower bound for the gain of an action.

        :param MAX: Upper bound for the gain of an action.

        The parameters :code:`MIN` and :code:`MAX` must make up an interval. All
        the gains will be generated randomly in the interval :code:`[MIN,MAX]`.

    .. _gain-functions:

    **Public functions**

    .. cpp:function:: std::string toString(int precision)

        Generates a string with the gain function matrix.

        :param precision: Decimal precision for long double numbers. For example, 
            use the value 3 to print 0.322, use 5 to print 0.32197, and so on.

        :return: The gain function :cpp:var:`matrix <Gain::matrix>` will be 
            returned in the following format: ::

                g11 g12 ... g1n
                g21 g22 ... g2n
                ...
                gw1 gw2 ... gwn

            where each gain :code:`gij` is the gain of taking action :code:`i` when
            the secret value is :code:`j`. Each two numbers are separated by a 
            space.

    .. cpp:function:: void printToFile(std::string file, int precision)

        Prints the gain function matrix in a file.

        :param file: File name.

        :param precision: Decimal precision for long double numbers. For example, 
            use the value 3 to print 0.322, use 5 to print 0.32197, and so on.

        The gain function matrix will be printed in the following format: ::

            w n
            g11 g12 ... g1n
            g21 g22 ... g2n
            ...
            gw1 gw2 ... gwn

        where :code:`w` is the number of actions (:cpp:var:`num_act`) and :code:`n`
        is the number of secrets. Each gain :code:`gij` is the gain of taking 
        action :code:`i` when the secret value is :code:`j`. Each two numbers are
        sparated by a space.

        




