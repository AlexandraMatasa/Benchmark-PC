#include "../include/memoryTransferSpeed.h"
#include "../include/config.h"
#include <iostream>
#include <chrono>
#include <cstring>
#include <random>
#include <sstream>
#include <iomanip>

using namespace std;

std::vector<char> generateRandomData(size_t size) {
    std::vector<char> data(size);
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, 255);

    for (auto &val : data) {
        val = static_cast<char>(distribution(generator));
    }

    return data;
}

MemoryTransferResult testDataTransferSpeed(size_t blockSize) {
    auto source = generateRandomData(blockSize);
    std::vector<char> destination(blockSize);

    auto start = chrono::high_resolution_clock::now();
    memcpy(destination.data(), source.data(), blockSize);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, nano> duration = end - start;

    double totalTimeNanoseconds = duration.count();
    double seconds = totalTimeNanoseconds / 1e9;
    double speed = (blockSize / 1024.0 / 1024.0) / seconds; // MB/s
    double avgTimePerByte = totalTimeNanoseconds / static_cast<double>(blockSize); // ns/byte

    stringstream blockSizeStream, speedStream, durationStream, avgTimeStream;
    blockSizeStream << fixed << setprecision(2) << blockSize / 1024.0 / 1024.0 << " MB";
    speedStream << fixed << setprecision(2) << speed << " MB/s";
    durationStream << fixed << setprecision(5) << totalTimeNanoseconds << " nanoseconds";
    avgTimeStream << fixed << setprecision(5) << avgTimePerByte << " nanoseconds/byte";

    return {blockSizeStream.str(), speedStream.str(), durationStream.str(), avgTimeStream.str()};
}

std::vector<MemoryTransferResult> test_memory_transfer_speed() {
    vector<MemoryTransferResult> all_results(TEST_REPETITIONS);

    size_t blockSizeMB = 128; // 128 MB block size
    size_t blockSizeBytes = blockSizeMB * 1024 * 1024; // Convert MB to bytes

    for (int i = 0; i < TEST_REPETITIONS; ++i) {
        all_results[i] = testDataTransferSpeed(blockSizeBytes);
    }

    return all_results;
}

int main() {
    std::vector<MemoryTransferResult> results = test_memory_transfer_speed();

    cout << "--- Memory Transfer Speed Test ---\n";
    for (size_t i = 0; i < results.size(); ++i) {
        const auto &result = results[i];
        cout << "--- Test Run " << i + 1 << " ---\n";
        cout << "Block Size: " << result.blockSize << "\n";
        cout << "Transfer Speed: " << result.speed << "\n";
        cout << "Total Duration: " << result.duration << "\n";
        cout << "Average Time per Byte: " << result.avgTimePerByte << "\n\n";
    }

    return 0;
}
