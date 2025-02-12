#ifndef IO_PERFORMANCE_H
#define IO_PERFORMANCE_H

#include <vector>
#include <string>

struct IOResult {
    std::string fileSize;
    std::string writeDuration;
    std::string readDuration;
    std::string writeSpeed;
    std::string readSpeed;
    std::string avgWriteTimePerByte;
    std::string avgReadTimePerByte;
};

std::vector<char> generateData(size_t size);
double writeTest(const std::vector<char>& data, const std::string& filename);
double readTest(const std::string& filename);
std::vector<IOResult> testIOPerformance(size_t fileSize, const std::string& filename);

#endif
