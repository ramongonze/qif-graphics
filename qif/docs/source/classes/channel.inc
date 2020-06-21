.. highlight:: cpp

.. _channel-class:

**Channel** class
*****************

A channel is a system that takes as input a secret :math:`x`, whose possible 
values come from a finite set :math:`\mathcal{X}` , and whose only observable 
behavior is to produce an output :math:`y`, whose possible values come from a 
finite set :math:`\mathcal{Y}`.

.. cpp:class:: Channel

	.. _channel-variables:

	**Variables**

	.. cpp:var:: Distribution prior
		
		A :cpp:class:`Distribution` object used to represent the prior 
		probability distribution on the set of secrets.
		
	.. cpp:var:: int num_out
		
		The number of outputs in the channel. It is exactly the number of 
		columns in :cpp:var:`matrix <Channel::matrix>`.

	.. cpp:var:: std::vector<std::vector<long double>> matrix
		
		A channel matrix from :math:`\mathcal{X}` to :math:`\mathcal{Y}` whose 
		rows give the distribution on the set of outputs. That is, entry 
		:code:`matrix[x][y]` denotes :math:`p(y|x)`, the conditional probability 
		of getting output :math:`y` given input :math:`x`. Note that all entries 
		in a channel matrix are between 0 and 1, and each row sums to 1.

		Both rows and columns are indexed starting at position 0.

	.. note:: To create a Channel object you need first to create a 
		:cpp:class:`Distribution` object, that keeps the prior distribution on the 
		set of secrets.

	.. _channel-constructors:

	**Constructors**

	.. cpp:function:: Channel()
	
		Default constructor.

	.. cpp:function:: Channel(Distribution& prior, std::string file)

		Construtor used when the channel matrix is in a file.

		:param prior: Prior distribution on the set of secrets.
		:param file: File name that contains a channel matrix.
			The file format must be the following: ::

				n y
				p11 p12 ... p1y
				p21 p22 ... p2y
				...
				pn1 pn2 ... pny

			where :code:`n` is the number of secrets and :code:`y` is the number 
			of outputs. Each :code:`pij` denotes :math:`p(j|i)` - the conditional 
			probability of getting output :math:`j` given input :math:`i`. Each 
			two numbers must be separated by a space.

	.. cpp:function:: Channel(Distribution& prior, std::vector<std::vector<long double>>& matrix)

		Constructor used when there is already a channel matrix in a variable.

		:param prior: Prior distribution on the set of secrets.
		:param matrix: 	A channel matrix.

		.. warning :: The number of rows in the channel matrix must be equal to 
			:cpp:var:`prior.num_el <Distribution::num_el>`.

	.. cpp:function:: Channel(Distribution& prior, int num_out)

		Constructor used to generate random channels. Given a set of secrets on 
		:cpp:var:`prior`, the constructor builds a random channel matrix.

		:param prior: Prior distribution on the set of secrets.
		:param num_out: Number of outputs.

	.. _channel-functions:

	**Public functions**

	.. cpp:function:: static bool isChannel(std::vector<std::vector<long double>>& matrix)

		Verifies if a matrix is a valid channel matrix. A channel matrix is valid 
		if each row corresponds to a probability distribution.

		:param matrix: A channel matrix.

		:return: :code:`true` if the matrix is valid or :code:`false` otherwise.

	.. cpp:function:: std::string toString(int precision)

		Generates a string with the channel matrix.

		:param precision: Decimal precision for long double numbers. For example,
			use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		:return: A string in the following format: ::

				p11 p12 ... p1y
				p21 p22 ... p2y
				...
				pn1 pn2 ... pny

			where :code:`n` is the number of secrets and :code:`y` is the number
			of outputs. Each :code:`pij` denotes :code:`p(j|i)`, the conditional
			probability of getting output :code:`j` given input :code:`i`. Each 
			two numbers must be separated by a space.

	.. cpp:function:: void printToFile(std::string file, int precision)

		Prints the channel matrix in a file.

		:param file: File name to print the distribution.
		:param precision: Decimal precision for long double numbers. For example, 
			use the value 3 to print 0.322, use 5 to print 0.32197, and so on.

		The channel matrix will be printed in the following format: ::

			n y
			p11 p12 ... p1y
			p21 p22 ... p2y
			...
			pn1 pn2 ... pny

		where :code:`n` is the number of secrets and :code:`y` is the number of 
		outputs. Each :code:`pij` denotes :code:`p(j|i)`, the conditional 
		probability of getting output :code:`j` given input :code:`i`. Each two 
		numbers must be separated by a space.


