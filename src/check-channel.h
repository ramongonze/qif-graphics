#ifndef CHECK_CHANNEL // include guard
#define CHECK_CHANNEL

#include <cfloat>
#include <cmath>
#include <iostream>
#include <vector>

class check_channel
{
    public:
        // Class check_channel constructor.
        check_channel(int size = 3);

        // Check resulting channel.
        void check(std::vector<std::vector<long double>> channel, int size);

    private:
        // Secret domain size.
        int size;

        // Create channel.
        void create(std::vector<std::vector<long double>> &channel, int size);

        // Check absolute and relative equality between long double numbers.
        // max is the maximum possible input for either a or b.
        bool check_equality(long double a, long double b, long double max);

        // Print channel.
        void print_channel(std::vector<std::vector<long double>> channel);
};

#endif /* CHECK_CHANNEL */