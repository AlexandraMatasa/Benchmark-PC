#include "../include/monteCarloPi.h"
#include "../include/config.h"
#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

std::vector<MonteCarloPi> calculate_pi(int iterations) {
    std::vector<MonteCarloPi> all_results(TEST_REPETITIONS);
    
    for (int i = 0; i < TEST_REPETITIONS; ++i) {
        int points_inside_circle = 0;
        random_device rd;
        mt19937 generator(rd());
        uniform_real_distribution<double> distribution(0.0, 1.0);
        
        auto start = chrono::high_resolution_clock::now();
        
        for (int j = 0; j < iterations; ++j) {
            double x = distribution(generator);
            double y = distribution(generator);
            if (x * x + y * y <= 1.0) {
                points_inside_circle++;
            }
        }
        
        auto end = chrono::high_resolution_clock::now();
        
        double pi_estimate = 4.0 * static_cast<double>(points_inside_circle) / iterations;
        double total_time_ns = chrono::duration<double, nano>(end - start).count();
        
        stringstream pi_stream, duration_stream;
        pi_stream << fixed << setprecision(6) << pi_estimate;
        duration_stream << fixed << setprecision(5) << total_time_ns << " ns";

        all_results[i] = {pi_stream.str(), duration_stream.str()};
    }

    return all_results;
}

int main() {
    int iterations = 1000000; // Number of points for Monte Carlo simulation
    std::vector<MonteCarloPi> results = calculate_pi(iterations);

    double total_pi = 0.0;
    double total_duration = 0.0;

    for (int i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        double pi_value = stod(result.pi);
        double duration_value = stod(result.duration.substr(0, result.duration.find(' ')));

        total_pi += pi_value;
        total_duration += duration_value;

        cout << "Repetition " << (i + 1) << ":\n";
        cout << "  Pi: " << result.pi << "\n";
        cout << "  Duration: " << result.duration << "\n\n";
    }

    double avg_pi = total_pi / TEST_REPETITIONS;
    double avg_duration = total_duration / TEST_REPETITIONS;

    cout << "--- Average Results ---\n";
    cout << "  Average Pi: " << fixed << setprecision(6) << avg_pi << "\n";
    cout << "  Average Duration: " << fixed << setprecision(5) << avg_duration << " ns\n";

    return 0;
}
