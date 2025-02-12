#ifndef TAYLOR_SERIES_EXPONENTIAL_H
#define TAYLOR_SERIES_EXPONENTIAL_H

#include <string>
#include <vector>


struct TaylorSeriesResult {
    std::string estimatedValue;
    std::string duration;
};

std::vector<TaylorSeriesResult> compute_exponential_taylor(double x, int n);

#endif
