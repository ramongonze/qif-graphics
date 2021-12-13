#include "random-response.h"

// Class random_response constructor.
random_response::random_response(int size, long double epsilon, long double delta)
{
    std::vector<std::vector<long double>> channel(size, std::vector<long double>(size, 0));

    random_response::create_channel(channel, size, epsilon, delta);

    try
    {
        check_channel check_channel(size);
        check_channel.check(channel, size);
    }
    catch (const char* err)
    {
        std::cerr << err << std::endl;
    }
}

// Channel updater.
std::vector<std::vector<long double>> random_response::get_channel(int size, long double epsilon, long double delta)
{
    try
    {
        random_response::check_parameters(size, epsilon, delta);
    }
    catch (const char* err)
    {
        std::cerr << err << std::endl;
    }

    std::vector<std::vector<long double>> channel(size, std::vector<long double>(size, 0));

    random_response::create_channel(channel, size, epsilon, delta);

    try
    {
        check_channel check_channel(size);
        check_channel.check(channel, size);
    }
    catch (const char* err)
    {
        std::cerr << err << std::endl;
    }

    return channel;
}

// Create channel.
void random_response::create_channel(std::vector<std::vector<long double>> &channel, int size, long double epsilon, long double delta)
{
    long double other = 1 / (exp(epsilon) + delta + size - 1);
    long double truthful = (exp(epsilon) + delta) * other;

    // Create channel for given parameters.
    int i = 0, j = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (i == j)
            {
                channel[i][j] = truthful;
            }
            else
            {
                channel[i][j] = other;
            }
        }
    }
}

// Check channel and differential privacy parameters.
void random_response::check_parameters(int size, long double epsilon, long double delta)
{
    // Secret domain size must be an integer.
    if (typeid(size) != typeid(int))
    {
        throw "Secret's domain size must be an integer.";
    }

    // Secret domain size must be greater than one.
    else if (size < 2)
    {
        throw "Secret's domain must have at least two values.";
    }

    // Both epsilon and delta must be real.
    else if (typeid(epsilon) != typeid(long double))
    {
        throw "Epsilon must be numeric.";
    }
    else if (typeid(delta) != typeid(long double))
    {
        throw "Delta must be numeric.";
    }

    // Epsilon must be non-negative.
    else if (epsilon < 0)
    {
        throw "Epsilon must be non-negative.";
    }

    // Delta must be in range [0,1].
    else if (delta < 0 || delta > 1)
    {
        throw "Delta must be in range [0,1].";
    }

    // Both epsilon and delta cannot be zero.
    else if (epsilon == 0 && delta == 0)
    {
        throw "Either epsilon or delta must be greater than zero.";
    }
}