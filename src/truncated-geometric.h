#ifndef TRUNCATED_GEOMETRIC // include guard
#define TRUNCATED_GEOMETRIC

#include <iostream>
#include <typeinfo>
#include <vector>
#include <cfloat>
#include <cmath>

namespace TG
{
    class truncated_geometric
    {
        public:
            // Class truncated_geometric constructor.
            truncated_geometric(int size = 3, long double alpha = 0.5);

            // Channel updater.
            std::vector<std::vector<long double>> get_channel(int size, long double alpha);

        private:
            // Secret domain size.
            int size;

            // Differential privacy parameters.
            long double alpha;

            // Create channel.
            void create_channel(std::vector<std::vector<long double>> &channel, int size, long double alpha);

            // Check channel and differential privacy parameters.
            void check_parameters(int size, long double alpha);

            // Check resulting channel.
            void check_channel(std::vector<std::vector<long double>> channel, int size, long double alpha);
    };
}

#endif /* TRUNCATED_GEOMETRIC */