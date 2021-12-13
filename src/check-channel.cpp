#include "check-channel.h"

// Class check_channel constructor.
check_channel::check_channel(int size)
{
    std::vector<std::vector<long double>> channel(size, std::vector<long double>(size, 0));

    check_channel::create(channel, size);

    try
    {
        check_channel::check(channel, size);
    }
    catch (const char* err)
    {
        std::cerr << err << std::endl;
    }
}

// Check resulting channel.
void check_channel::check(std::vector<std::vector<long double>> channel, int size)
{
    // Check channel properties (e.g. each row sums to 1).
    int i = 0, j = 0;
    for (i = 0; i < size; i++)
    {
        long double row = 0;

        for (j = 0; j < size; j++)
        {
            row = row + channel[i][j];
        }

        if (!check_channel::check_equality(row, 1, 1))
        {
            throw "Channel rows must sum to one.";
        }
    }
}

// Check absolute and relative equality between long double numbers.
// max is the maximum possible input for either a or b.
bool check_channel::check_equality(long double a, long double b, long double max)
{
    // Absolute equality. Useful for close to zero subtractions.
    long double difference = fabs(a - b);
    if (difference <= max * DBL_EPSILON / 2)
    {
        return true;
    }

    // Relative equality.
    a = fabs(a);
    b = fabs(b);
    long double largest = (b > a) ? b : a;
    if (difference <= largest * DBL_EPSILON / 2)
    {
        return true;
    } else {
        return false;
    }
}

// Create channel.
void check_channel::create(std::vector<std::vector<long double>> &channel, int size)
{
    long double value = 1.0 / size;

    // Create channel for given parameters.
    int i = 0, j = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            channel[i][j] = value;
        }
    }
}

void check_channel::print_channel(std::vector<std::vector<long double>> channel)
{
    for (auto i: channel)
    {
        for (auto j: i)
        {
            std::cout << j << "\t";
        }
        std::cout << std::endl;
    }
}