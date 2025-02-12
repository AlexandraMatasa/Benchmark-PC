#ifndef BIT_MANIPULATION_H
#define BIT_MANIPULATION_H

#include <vector>
#include <string>

struct BitManipulationResults {
    std::string shiftRightTime;
    std::string shiftLeftTime;
    std::string andTime;
    std::string orTime;
    std::string xorTime;
    std::string notTime;
};

std::vector<int> generate_random_data(int num_elements);
std::vector<BitManipulationResults> test_bit_manipulation(const std::vector<int>& data);

#endif
