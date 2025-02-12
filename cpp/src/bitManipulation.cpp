#include "../include/bitManipulation.h"
#include "../include/config.h"
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <sstream>
#include <vector>


using namespace std;

vector<int> generate_random_data(int num_elements) {
    vector<int> data(num_elements);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 1000000);

    for (int& value : data) {
        value = distrib(gen);
    }

    return data;
}

vector<BitManipulationResults> test_bit_manipulation(const vector<int>& data) {
    vector<BitManipulationResults> all_results(TEST_REPETITIONS);
    
    auto format_time = [](double time_ns) -> std::string {
        stringstream ss;
        ss << fixed << setprecision(5) << time_ns;
        return ss.str() + " ns";
    };

    for (int i = 0; i < TEST_REPETITIONS; i++) {
        BitManipulationResults results;
        volatile int result;

        auto startShiftRight = chrono::high_resolution_clock::now();
        for (int value : data) {
            result = value >> 5;
        }
        auto endShiftRight = chrono::high_resolution_clock::now();
        results.shiftRightTime = format_time(chrono::duration<double, nano>(endShiftRight - startShiftRight).count());

        auto startShiftLeft = chrono::high_resolution_clock::now();
        for (int value : data) {
            result = value << 5;
        }
        auto endShiftLeft = chrono::high_resolution_clock::now();
        results.shiftLeftTime = format_time(chrono::duration<double, nano>(endShiftLeft - startShiftLeft).count());

        auto startAnd = chrono::high_resolution_clock::now();
        for (int value : data) {
            result = value & 0x55555555;
        }
        auto endAnd = chrono::high_resolution_clock::now();
        results.andTime = format_time(chrono::duration<double, nano>(endAnd - startAnd).count());

        auto startOr = chrono::high_resolution_clock::now();
        for (int value : data) {
            result = value | 0xAAAAAA;
        }
        auto endOr = chrono::high_resolution_clock::now();
        results.orTime = format_time(chrono::duration<double, nano>(endOr - startOr).count());

        auto startXor = chrono::high_resolution_clock::now();
        for (int value : data) {
            result = value ^ 0xAAAAAA;
        }
        auto endXor = chrono::high_resolution_clock::now();
        results.xorTime = format_time(chrono::duration<double, nano>(endXor - startXor).count());

        auto startNot = chrono::high_resolution_clock::now();
        for (int value : data) {
            result = ~value;
        }
        auto endNot = chrono::high_resolution_clock::now();
        results.notTime = format_time(chrono::duration<double, nano>(endNot - startNot).count());

        all_results[i] = results;
    }

    return all_results;
}

int main() {
    int num_elements = 1000000;
    vector<int> data = generate_random_data(num_elements);
    vector<BitManipulationResults> all_results = test_bit_manipulation(data);

     for (int i = 0; i < TEST_REPETITIONS; i++) {
        cout << "\nResults for repetition " << i + 1 << " of " << TEST_REPETITIONS << ":\n";
        cout << "Time for right shift operations: " << all_results[i].shiftRightTime << "\n";
        cout << "Time for left shift operations: " << all_results[i].shiftLeftTime << "\n";
        cout << "Time for AND operations: " << all_results[i].andTime << "\n";
        cout << "Time for OR operations: " << all_results[i].orTime << "\n";
        cout << "Time for XOR operations: " << all_results[i].xorTime << "\n";
        cout << "Time for NOT operations: " << all_results[i].notTime << "\n";
    }

    return 0;
}
