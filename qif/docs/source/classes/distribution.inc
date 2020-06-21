.. highlight:: cpp

.. _distribution-class:

**Distribution** class
**********************

It represents a probability distribution on a set of elements.

.. cpp:class:: Distribution

   .. _distribution-variables:

   **Variables**

      .. cpp:var:: int num_el
      
         The number of elements in the set. It is also equal to 
         :cpp:var:`prob.size() <prob>`.
      
      .. cpp:var:: std::vector<long double> prob

         It contains the probability of ocurrence of each element from a set.
   
         Each value :cpp:var:`prob[i] <prob>` is the probability of ocurrence 
         of the ith element. The vector is indexed starting at position 0.

         .. note:: The vector prob is valid when all the elements are in the 
            interval :math:`[0,1]` and the sum of all numbers in the vector is 1
            with margin of error (upper or lower) equals to a constant *epsilon* 
            by default set as 10e-6.

   .. _distribution-constructors:

   **Constructors**

      .. cpp:function:: Distribution()

         Default constructor.

      .. cpp:function:: Distribution(std::string file)

         Constructor used when the probability distribution is in a file.
         
         :param file: File name that contains a probability distribution.
            The file format must be the following: ::

               n
               p1
               p2
               ...
               pn

            where :code:`n` is the number of elements in the set and
            :code:`pi` is the probability of the ith element ocurring.
            
      .. cpp:function:: Distribution(std::vector<long double>& prob)

         Constructor used when there is already a probability distribution in a 
         vector.

         :param prob: A vector containing a probability distribution.      

      .. cpp:function:: Distribution(int num_el, std::string type)

         Constructor used to generate a uniform or a random probability 
         distribution.

         :param num_el: Number of elements in the set.
         :param type: Must be :code:`"uniform"` for generating a uniform 
            distribution or :code:`"random"` for a random distribution.
      

   .. _distribution-functions:

   **Public functions**                                                              
                                                                                                             
      .. cpp:function:: std::string toString(int precision)
                                                                                    
         Generates a string with the probability distribution.
                                                                                    
         :param precision: Decimal precision for long double numbers. For example,
            use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
         :return: A string containing the probabilities. The number of digits after
            de decimal point for each value will depend on :cpp:var:`precision`.
            Each two values are separated by a space.
                                                                                    
      .. cpp:function:: void printToFile(std::string file, int precision)
                                                                                    
         Prints the probability distribution in a file.

         :param file: File name.
         :param precision: Decimal precision for long double numbers. For example,
            use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
                                                                                    
         The probability distribution will be printed in the following format: ::

            n
            p1
            p2
            ...
            pn

         where :code:`n` is the number of elements in the probability distribution 
         and :code:`pi` is the probability :math:`p(i)` of the ith element 
         occuring.
