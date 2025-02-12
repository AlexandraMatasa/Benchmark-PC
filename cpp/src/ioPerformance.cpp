#include "../include/ioPerformance.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include "../include/config.h"

using namespace std;

std::vector<char> generateData(size_t size) {
    std::vector<char> data(size);
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0, 255);

    for (auto &val : data) {
        val = static_cast<char>(distribution(generator));
    }

    return data;
}

double writeTest(const std::vector<char> &data, const std::string &filename) {
    auto start = chrono::high_resolution_clock::now();
    ofstream file(filename, ios::binary);

    if (!file) {
        cerr << "Failed to open file for writing: " << filename << endl;
        return 0.0;
    }

    file.write(data.data(), data.size());
    file.close();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, nano> duration = end - start;
    return duration.count();
}

double readTest(const std::string &filename) {
    ifstream file(filename, ios::binary | ios::ate);

    if (!file) {
        cerr << "Failed to open file for reading: " << filename << endl;
        return 0.0;
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<char> buffer(size);

    auto start = chrono::high_resolution_clock::now();
    file.read(buffer.data(), size);
    auto end = chrono::high_resolution_clock::now();

    file.close();
    chrono::duration<double, nano> duration = end - start;
    return duration.count();
}

std::vector<IOResult> testIOPerformance(size_t fileSize, const std::string &filename) {
    vector<IOResult> all_results(TEST_REPETITIONS);

for (int i = 0; i < TEST_REPETITIONS; i++) {
        vector<char> data = generateData(fileSize);
        double writeDurationNs = writeTest(data, filename);
        double readDurationNs = readTest(filename);
        double writeSpeedMBps = (fileSize / 1024.0 / 1024.0) / (writeDurationNs / 1e9); 
        double readSpeedMBps = (fileSize / 1024.0 / 1024.0) / (readDurationNs / 1e9); 
        double avgWriteTimePerByteNs = writeDurationNs / static_cast<double>(fileSize);
        double avgReadTimePerByteNs = readDurationNs / static_cast<double>(fileSize);

        stringstream fileSizeStream, writeDurationStream, readDurationStream, writeSpeedStream, readSpeedStream, avgWriteStream, avgReadStream;

        fileSizeStream << fixed << setprecision(2) << fileSize / 1024.0 / 1024.0 << " MB";
        writeDurationStream << fixed << setprecision(5) << writeDurationNs << " ns";
        readDurationStream << fixed << setprecision(5) << readDurationNs << " ns";
        writeSpeedStream << fixed << setprecision(2) << writeSpeedMBps << " MB/s";
        readSpeedStream << fixed << setprecision(2) << readSpeedMBps << " MB/s";
        avgWriteStream << fixed << setprecision(5) << avgWriteTimePerByteNs << " ns/byte";
        avgReadStream << fixed << setprecision(5) << avgReadTimePerByteNs << " ns/byte";

        all_results[i] = {fileSizeStream.str(), writeDurationStream.str(), readDurationStream.str(), writeSpeedStream.str(), readSpeedStream.str(), avgWriteStream.str(), avgReadStream.str()};
    }

    return all_results;
}


int main() {
    // File sizes to test: 1MB, 10MB, 100MB
    std::vector<size_t> sizes = {1024 * 1024, 10 * 1024 * 1024, 100 * 1024 * 1024};  
    std::string filenameBase = "testFile_";

    for (size_t size : sizes) {
        std::string filename = filenameBase + std::to_string(size) + ".bin";
        
        std::vector<IOResult> results = testIOPerformance(size, filename);

        if (results.empty()) {
            std::cerr << "No results returned for size: " << size << std::endl;
            continue;
        }

        double totalWriteDuration = 0.0;
        double totalReadDuration = 0.0;
        double totalWriteSpeed = 0.0;
        double totalReadSpeed = 0.0;
        double totalAvgWriteTimePerByte = 0.0;
        double totalAvgReadTimePerByte = 0.0;

        for (const auto& res : results) {
            auto parseValue = [](const std::string& s) -> double {
                auto pos = s.find(' ');
                std::string numberStr = (pos == std::string::npos) ? s : s.substr(0, pos);
                return std::stod(numberStr);
            };

            double fileSizeMB = parseValue(res.fileSize); // in MB
            double writeNs = parseValue(res.writeDuration); // in ns
            double readNs = parseValue(res.readDuration); // in ns
            double writeMBs = parseValue(res.writeSpeed); // in MB/s
            double readMBs = parseValue(res.readSpeed);   // in MB/s
            double avgWriteNsPerByte = parseValue(res.avgWriteTimePerByte);
            double avgReadNsPerByte = parseValue(res.avgReadTimePerByte);

            totalWriteDuration += writeNs;
            totalReadDuration += readNs;
            totalWriteSpeed += writeMBs;
            totalReadSpeed += readMBs;
            totalAvgWriteTimePerByte += avgWriteNsPerByte;
            totalAvgReadTimePerByte += avgReadNsPerByte;
        }

        int count = static_cast<int>(results.size());
        double avgWriteDuration = totalWriteDuration / count;
        double avgReadDuration = totalReadDuration / count;
        double avgWriteSpeed = totalWriteSpeed / count;
        double avgReadSpeed = totalReadSpeed / count;
        double avgAvgWriteTimePerByte = totalAvgWriteTimePerByte / count;
        double avgAvgReadTimePerByte = totalAvgReadTimePerByte / count;

        std::cout << "File size: " << (double)size / (1024.0 * 1024.0) << " MB\n"
                  << "Average write duration: " << avgWriteDuration << " ns\n"
                  << "Average read duration: " << avgReadDuration << " ns\n"
                  << "Average write speed: " << avgWriteSpeed << " MB/s\n"
                  << "Average read speed: " << avgReadSpeed << " MB/s\n"
                  << "Average write time per byte: " << avgAvgWriteTimePerByte << " ns/byte\n"
                  << "Average read time per byte: " << avgAvgReadTimePerByte << " ns/byte\n\n";
    }

    return 0;
}