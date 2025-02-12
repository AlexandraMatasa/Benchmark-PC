#include "../include/cpuInfo.h"
#include <windows.h>
#include <intrin.h>
#include <iostream>
#include <vector>

using namespace std;

std::string getCPUFrequency() {
    HKEY hKey;
    DWORD mhz;
    DWORD bufferSize = sizeof(mhz);
    std::string result;

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                      L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return "Failed to access registry for CPU frequency.";
    }

    if (RegQueryValueExW(hKey, L"~MHz", NULL, NULL, reinterpret_cast<LPBYTE>(&mhz), &bufferSize) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return "Failed to read CPU frequency from registry.";
    }

    RegCloseKey(hKey);
    result = "CPU Frequency: " + to_string(mhz) + " MHz";
    return result;
}

std::string getCPUType() {
    int cpuInfo[4] = {-1};
    char vendor[13];
    __cpuid(cpuInfo, 0);
    memcpy(vendor, &cpuInfo[1], 4);
    memcpy(vendor + 4, &cpuInfo[3], 4);
    memcpy(vendor + 8, &cpuInfo[2], 4);
    vendor[12] = '\0';

    char cpuModel[49];
    __cpuid((int*)cpuModel, 0x80000002);
    __cpuid((int*)(cpuModel + 16), 0x80000003);
    __cpuid((int*)(cpuModel + 32), 0x80000004);
    cpuModel[48] = '\0';

    return "Vendor: " + string(vendor) + "\n  Model: " + string(cpuModel);
}

int getCpuCoreInfo(int& numberOfCores, int& numberOfThreads) {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    numberOfThreads = sysInfo.dwNumberOfProcessors;

    DWORD returnedLength = 0;
    GetLogicalProcessorInformation(NULL, &returnedLength);
    vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(returnedLength / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));

    if (GetLogicalProcessorInformation(buffer.data(), &returnedLength)) {
        numberOfCores = 0;
        for (const auto& info : buffer) {
            if (info.Relationship == RelationProcessorCore) {
                if (info.ProcessorCore.Flags) {
                    numberOfCores++;
                } else {
                    numberOfCores++;
                }
            }
        }
    } else {
        return -1;
    }
    return 0;
}

CPUInfo get_cpu_info() {
    CPUInfo info;
    info.frequency = getCPUFrequency();
    info.type = getCPUType();

    if (getCpuCoreInfo(info.cores, info.threads) != 0) {
        throw std::runtime_error("Failed to retrieve core information.");
    }

    return info;
}


int main() {
    try {
        cout << "--- CPU Information ---\n";
        
        CPUInfo cpuInfo = get_cpu_info();
        
        cout << cpuInfo.frequency << endl;
        cout << cpuInfo.type << endl;
        
        cout << "Number of cores: " << cpuInfo.cores << endl;
        cout << "Threads: " << cpuInfo.threads << endl;
        if (cpuInfo.cores > 0) {
            int threadsPerCore = cpuInfo.threads / cpuInfo.cores;
            cout << "Threads per core: " << threadsPerCore << endl;
        } else {
            cout << "Unable to calculate threads per core." << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    return 0;
}