#ifndef MEMORY_TRANSFER_SPEED_H
#define MEMORY_TRANSFER_SPEED_H

#include <vector>
#include <string>

struct MemoryTransferResult {
    std::string blockSize;
    std::string speed;
    std::string duration;
    std::string avgTimePerByte;
};

std::vector<char> generateRandomData(size_t size);
std::vector<MemoryTransferResult> test_memory_transfer_speed();

#endif
