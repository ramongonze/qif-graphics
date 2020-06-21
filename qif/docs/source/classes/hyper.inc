.. highlight:: cpp

.. _hyper-class:

**Hyper** Class
***************

Definition of hyper-distribution:

If :math:`\mathcal{X}` is a finite set (of possible secret values), :math:`\pi` 
is the prior distribution on :math:`\mathcal{X}`, and :math:`C` is a channel, 
a hyper-distribution :math:`[\pi \triangleright C]` resulted from :math:`C` on 
:math:`\pi` is a distribution on distributions on :math:`\mathcal{X}`. Each output in 
an informational channel is a possible "world", and each possible world is a new 
distribution on the set of secrets. We call the possible worlds the *inner 
distributions*. Each possible world has a probability of occurring, and we call 
the distribution on the possible worlds as the *outer distribution*.

.. cpp:class:: Hyper

    .. _hyper-variables:

    **Variables**

    .. cpp:var:: Distribution prior

        A probability distribution on a set of secrets.

    .. cpp:var:: Channel channel

        An informational channel.

    .. cpp:var:: int num_post

        The number of posterior distributions yielded by :cpp:var:`channel` and 
        :cpp:var:`prior`.

        .. note:: The number of posterior distributions can be different from the 
            number of outputs in channel. See :cpp:var:`labels` for more details.

    .. cpp:var:: std::vector<std::vector<long double>> joint

        Joint distribution matrix.

        It is a matrix :math:`n` x :math:`y` where :math:`n` is the number of secrets in 
        the channel and :math:`y` is the number of outputs in the channel. Each value 
        :code:`joint[i][j]` is the joint probability :math:`p(i,j)`. The joint 
        distribution is build by multiplying the prior distribution by each row of the 
        channel matrix.

    .. cpp:var:: Distribution outer

        A distribution on posterior distributions.

        Each value :code:`outer.prob[i]` is the probability :math:`p(i)` of the ith 
        posterior distribution occuring, with :math:`0 \leq i <` 
        :cpp:var:`num_post-1 <num_post>`.

    .. cpp:var:: std::vector<std::vector<long double>> inners

        Inner distributions matrix. It is a matrix :math:`n` x :math:`y` 
        where :math:`n` is the number of secrets (:cpp:var:`prior.num_el <prior>`) 
        and :math:`y` is the number of posterior distributions 
        (:cpp:var:`num_post`). Each value :code:`inners[i][j]` corresponds to the 
        conditional probability :math:`p(i|j)`, that is, the probability of the 
        secret being :code:`i` when the output is :code:`j`. Each column is a 
        posterior distribution.

        Both rows and columns are indexed starting at position 0.

    .. cpp:var:: std::map<int, std::set<int>> labels

        A map from posterior dsitributions indexes to a set of channel outputs 
        indexes. When we look at the hyper-distrubtion, we have the outer 
        distribution and the inner distributions. Depending on the prior 
        distribution and the channel, the hyper-distribution may have two different 
        outputs which yield the same posterior distribution on the set of secrets, 
        and in this case, there is no difference for the adversary observing both.

        Indeed we can simplify the representation of a hyper-distribution matrix 
        mergin all equal columns in a single column, and sum their outer 
        probabilities. We also delete a posterior column when its outer 
        probability of ocurring is 0 (which means that this "world" has 0 
        probability of occuring).

        - The map *key* is the index of a posterior distribution in the reduced 
          hyper-distribution matrix.
        - The map *value* of a key :code:`i` is the set of channel outputs indexes 
          which gave rise this single posterior distribution :code:`i`.

        Example: If the outputs 2 and 4 from the channel yield the same posterior 
        distribution, they are merged in, for example, the 3rd posterior 
        distribution. Then, :code:`labels[2]` (the 3rd posterior has index 
        2), you will have the set :math:`\{2,4\}`.


    .. _hyper-constructors:

    **Constructors**

    .. cpp:function:: Hyper()

        Default constructor.

    .. cpp:function:: Hyper(std::string prior_file, std::string channel_file)

        Constructor used when the prior distribution on the set of secrets and the 
        channel are in a file.

        :param prior_file: 	File name that contains a probability distribution.
        
            The file format must be the following: ::

                n
                p1
                p2
                ...
                pn

            where :code:`n` is the number of elements in the distribution and 
            :code:`pi` is the probability of the ith element occuring.


        :param channel_file: File name that contains a channel matrix.

            The file format must be the following: ::

                n y
                p11 p12 ... p1y
                p21 p22 ... p2y
                ...
                pn1 pn2 ... pny

            where :code:`n` is the number of secrets and :code:`y` is the number of 
            outputs. Each :code:`pij` value denotes :math:`p(j|i)`, the conditional 
            probability of getting output :math:`j` given input :math:`i`. Each two 
            numbers must be separated by a space.

        .. warning:: The number of rows in the channel matrix must be equal to 
            the number of elements in the prior distribution.

    .. cpp:function:: Hyper(Channel& channel)

        Constructor used when there is already a Channel object in a variable.

        .. note:: As a channel has the attribute 
            :cpp:member:`prior <Channel::prior>` (that corresponds to the prior 
            distribution on the set of secrets), it is not necessary to give a 
            :cpp:class:`Distribution` parameter in this constructor.

        :param channel: A :cpp:class:`Channel` object.

    
    .. _hyper-functions:

    **Public functions**
    
    .. cpp:function:: std::string toString(std::string type, int precision)

        Generates a string with the :cpp:var:`joint` matrix, the :cpp:var:`outer` 
        distribution, the :cpp:var:`inners` or the :cpp:var:`labels`.

        :param type: The name of what will be returned. It must be 
            :code:`"labels"`, :code:`"joint"`, :code:`"outer"` or :code:`"inners"`.

        :param precision: Decimal precision for float numbers. For example, use the 
            value 3 to print 0.322, use 5 to print 0.32197, and so on.

        - The joint matrix will be returned in the following format: ::

            p11 p12 ... p1y
            p21 p22 ... p2y
            ...
            pn1 pn2 ... pny

          where :code:`n` is the number of secrets, :code:`y` is the number of 
          channel outputs and :code:`pij` is the joint probability :math:`p(i,j)`.

        - The outer distribution will be returned in the following format: ::

            p1 p2 ... py
            
          where :code:`y` is the number of outputs.

        - The inners matrix will be returned in the following format: ::

            p11 p12 ... p1y
            p21 p22 ... p2y
            ...
            pn1 pn2 ... pny
            
          where :code:`n` is the number of secrets, :code:`y` is the number of 
          outputs and :code:`pij` is the conditional probability :math:`p(i|j)`.

        - The labels will be returned in the following format: ::

            1: x11 x12 ...
            2: x21 x22 ...
            ...
            3: xn1 xn2 ...
          
          where :code:`n` is the number of posteriors (:cpp:var:`num_post`) and 
          :code:`xij` is an integer between 0 and 
          :cpp:var:`channel.num_out <Channel::num_out>`.

        Each two numbers in any choice are separated by a space.

    .. cpp:function:: void printToFile(std::string type, std::string file, int precision)

        Prints the joint matrix, outer distribution, inners or the labels in a file.

        :param type: The name of what will be returned. It must be 
            :code:`"labels"`, :code:`"joint"`, :code:`"outer"` or :code:`"inners"`.

        :param file: File name.

        :param precision: Decimal precision for float numbers. For example, use the 
            value 3 to print 0.322, use 5 to print 0.32197, and so on.

        - The joint matrix will be printed in the following format: ::

            n y
            p11 p12 ... p1y
            p21 p22 ... p2y
            ...
            pn1 pn2 ... pny

        where :code:`n` is the number of secrets, :code:`y` is the number of 
        channel outputs and :code:`pij` is the joint probability :math:`p(i,j)`.

        - The outer distribution will be printed in the following format: ::

            y
            p1
            p2
            ...
            py

        where :code:`y` is the number of posterior distributions 
        (:cpp:var:`num_post`) and :code:`pi` is the probability :math:`p(i)` 
        of the ith posterior distribution ocurring.

        - The inners matrix will be printed in the following format: ::

            n y
            p11 p12 ... p1y
            p21 p22 ... p2y
            ...
            pn1 pn2 ... pny

        where :code:`n` is the number of secrets, :code:`y` is the number of 
        posterior distributions (:cpp:var:`num_post`) and :code:`pij` is the 
        conditional probability :math:`p(i|j)`.

        - The labels will be printed in the following format: ::

            y
            1: x11 x12 ...
            2: x21 x22 ...
            ...
            y: xy1 xy2 ...

        where :code:`y` is the number of posterior distributions 
        (:cpp:var:`num_post`) and :code:`xij` is an integer between 0 and 
        :cpp:var:`channel.num_out <Channel::num_out>`.

        Each two numbers in any choice are are separated by a space.

    .. cpp:function:: void rebuildHyper(Distribution& prior)

        Given a new prior distribution, rebuild the joint, outer and 
        inner distributions.

        :param prior: Prior distribution on a set of secrets.

        .. warning:: The number of rows in the current channel matrix must be 
            equal to the number of elements in the new prior distribution.

    .. cpp:function:: void rebuildHyper(Channel& channel)

        Given a new channel, rebuild the joint, outer and inner distributions.

        :param channel: Informational channel.

        .. warning:: The number of rows in the new channel matrix must be equal 
            to the number of elements in the current prior distribution.
