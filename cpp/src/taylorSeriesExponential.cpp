#include "../include/taylorSeriesExponential.h"
#include "../include/config.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

std::vector<TaylorSeriesResult> compute_exponential_taylor(double x, int n) {
    vector<TaylorSeriesResult> all_results(TEST_REPETITIONS);

    for (int i = 0; i < TEST_REPETITIONS; ++i) {
        auto start = chrono::high_resolution_clock::now();
        
        double result = 1.0;
        double term = 1.0;

        for (int j = 1; j <= n; ++j) {
            term *= x / j;
            result += term;
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, nano> duration = end - start;

        stringstream resultStream, durationStream;
        resultStream << fixed << setprecision(10) << result;
        durationStream << fixed << setprecision(5) << duration.count() << " ns";

        all_results[i] = { resultStream.str(), durationStream.str() };
    }

    return all_results;
}

int main() {
    double x = 3.0;  // e^x to be computed
    int terms = 20;  // number of terms in the Taylor series

    vector<TaylorSeriesResult> results = compute_exponential_taylor(x, terms);

    cout << "--- Taylor Series Computation for e^" << x << " ---" << endl;
    cout << fixed << setprecision(10);
    for (size_t i = 0; i < results.size(); ++i) {
        const auto &result = results[i];
        cout << "--- Test Run " << i + 1 << " ---" << endl;
        cout << "Estimated e^" << x << " = " << result.estimatedValue 
             << " (Computation time: " << result.duration << ")" << endl;
    }

    double exactValue = exp(x);
    cout << "\nExact e^" << x << " = " << exactValue << endl;

    return 0;
}
