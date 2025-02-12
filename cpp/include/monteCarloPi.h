#ifndef MONTE_CARLO_PI_H
#define MONTE_CARLO_PI_H

#include <cstddef>
#include <string>
#include <vector>


struct MonteCarloPi {
    std::string pi;
    std::string duration;
};

std::vector<MonteCarloPi> calculate_pi(int iterations);

#endif
