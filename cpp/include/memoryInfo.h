#ifndef MEMORY_INFO_H
#define MEMORY_INFO_H

#include <string>
#include <vector>

struct MemoryInfo {
    double totalMemoryGB;        
    double availableMemoryMB;   
    size_t blockSizeMB;
    std::string totalAccessTime;
    std::string accessTimePerByte;
};

double get_available_memory_in_mb();
std::vector<MemoryInfo> get_memory_info_and_access_time();

#endif
