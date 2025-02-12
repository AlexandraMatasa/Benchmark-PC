#include "../include/memoryInfo.h"
#include <iostream>
#include <windows.h>
#include <intrin.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <chrono>
#include "../include/config.h"

using namespace std;

std::vector<MemoryInfo> get_memory_info_and_access_time() {
    vector<MemoryInfo> all_results(TEST_REPETITIONS);

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memStatus)) {
        cerr << "Failed to get memory status." << endl;
        return {};
    }

    double totalMemoryGB = static_cast<double>(memStatus.ullTotalPhys) / (1024.0 * 1024.0 * 1024.0);
    double availableMemoryMB = static_cast<double>(memStatus.ullAvailPhys) / (1024.0 * 1024.0);
    
    size_t blockSizeMB = static_cast<size_t>(availableMemoryMB * 0.25); // 25% of available memory
    if (blockSizeMB > 1024) {
        blockSizeMB = 1024;
    } else if (blockSizeMB < 64) {
        blockSizeMB = 64;
    }

    for (int i = 0; i < TEST_REPETITIONS; ++i) {
        vector<char> memoryBlock(blockSizeMB * 1024 * 1024, 0); 
        volatile char tmp;

        auto start = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < memoryBlock.size(); ++i) {
            tmp = memoryBlock[i];
        }
        auto end = chrono::high_resolution_clock::now();
        
        double totalAccessTimeNs = chrono::duration<double, nano>(end - start).count();
        double accessTimePerByteNs = totalAccessTimeNs / static_cast<double>(memoryBlock.size());

        stringstream totalAccessTimeStream, accessTimePerByteStream;
        totalAccessTimeStream << fixed << setprecision(5) << totalAccessTimeNs << " ns";
        accessTimePerByteStream << fixed << setprecision(5) << accessTimePerByteNs << " ns/byte";

        all_results[i] = {totalMemoryGB, availableMemoryMB, blockSizeMB, totalAccessTimeStream.str(), accessTimePerByteStream.str()};
    }

    return all_results;
}

double get_available_memory_in_mb() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memStatus)) {
        cerr << "Failed to get memory status." << endl;
        return 0.0;
    }
    return static_cast<double>(memStatus.ullAvailPhys) / (1024.0 * 1024.0);
}

int main() {
    double availableMemoryMB = get_available_memory_in_mb();
    if (availableMemoryMB == 0.0) {
        cerr << "Failed to get available memory." << endl;
        return 1;
    }

    size_t blockSizeMB = static_cast<size_t>(availableMemoryMB * 0.25); 
    if (blockSizeMB > 1024) {
        blockSizeMB = 1024;
    } else if (blockSizeMB < 64) {
        blockSizeMB = 64;
    }

    cout << "--- Memory Information ---\n";
    cout << fixed << setprecision(5);
    cout << "Available Memory: " << availableMemoryMB << " MB\n";
    cout << "Block Size Used: " << blockSizeMB << " MB\n\n";

    vector<MemoryInfo> results = get_memory_info_and_access_time();
    if (results.empty()) {
        cerr << "Failed to get memory info and access time." << endl;
        return 1;
    }

    double totalAccessTimeSum = 0.0;
    double accessTimePerByteSum = 0.0;

    for (int i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        double totalAccessTime = stod(result.totalAccessTime.substr(0, result.totalAccessTime.find(' ')));
        double accessTimePerByte = stod(result.accessTimePerByte.substr(0, result.accessTimePerByte.find(' ')));

        cout << "Repetition " << i + 1 << ":\n";
        cout << "  Total Memory: " << result.totalMemoryGB << " GB\n";
        cout << "  Available Memory: " << result.availableMemoryMB << " MB\n";
        cout << "  Block Size: " << result.blockSizeMB << " MB\n";
        cout << "  Total Access Time: " << result.totalAccessTime << "\n";
        cout << "  Access Time per Byte: " << result.accessTimePerByte << "\n\n";

        totalAccessTimeSum += totalAccessTime;
        accessTimePerByteSum += accessTimePerByte;
    }

    double averageTotalAccessTime = totalAccessTimeSum / TEST_REPETITIONS;
    double averageAccessTimePerByte = accessTimePerByteSum / TEST_REPETITIONS;

    cout << "--- Average Results ---\n";
    cout << "  Average Total Access Time: " << fixed << setprecision(5) << averageTotalAccessTime << " ns\n";
    cout << "  Average Access Time per Byte: " << fixed << setprecision(5) << averageAccessTimePerByte << " ns/byte\n";

    return 0;
}