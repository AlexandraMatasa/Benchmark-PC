#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <string>

struct CPUInfo {
    std::string frequency;
    std::string type;
    int cores;
    int threads;
};

std::string getCPUFrequency();
std::string getCPUType();
int getCpuCoreInfo(int& numberOfCores, int& numberOfThreads);

CPUInfo get_cpu_info();

#endif
