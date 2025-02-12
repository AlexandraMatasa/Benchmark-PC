#include "../include/huffmanCoding.h"
#include "../include/config.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

HuffmanNode::HuffmanNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

bool Compare::operator()(const HuffmanNode* left, const HuffmanNode* right) {
    return left->frequency > right->frequency;
}

void generateCodes(const HuffmanNode* root, const string& code, unordered_map<char, string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->character] = code;
    }
    generateCodes(root->left.get(), code + "0", huffmanCode);
    generateCodes(root->right.get(), code + "1", huffmanCode);
}

HuffmanResult compressHuffman(const string& data) {
    unordered_map<char, int> frequency;
    for (char character : data) {
        frequency[character]++;
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> priorityQueue;
    for (auto& pair : frequency) {
        priorityQueue.emplace(new HuffmanNode(pair.first, pair.second));
    }

    while (priorityQueue.size() > 1) {
        auto left = unique_ptr<HuffmanNode>(priorityQueue.top());
        priorityQueue.pop();
        auto right = unique_ptr<HuffmanNode>(priorityQueue.top());
        priorityQueue.pop();

        int sum = left->frequency + right->frequency;
        auto node = make_unique<HuffmanNode>('\0', sum);
        node->left = move(left);
        node->right = move(right);

        priorityQueue.push(node.release());
    }

    auto root = unique_ptr<HuffmanNode>(priorityQueue.top());
    unordered_map<char, string> huffmanCode;
    generateCodes(root.get(), "", huffmanCode);

    string encodedString;
    for (char character : data) {
        encodedString += huffmanCode[character];
    }

    HuffmanResult result;
    result.originalSize = data.length();
    result.compressedSize = encodedString.length() / 8;  // convert bit length to byte
    return result;
}

std::vector<HuffmanResult> testHuffmanCompression(const string& filename) {
    std::string basePath = "D:/AN3-SEM1/SSC-PROIECT/BenchmarkPC/cpp/src/";
    std::ifstream file(basePath + filename);
    if (!file) {
        std::cerr << "Could not open file " << basePath + filename << "\n";
        return {};
    }

    string data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    std::vector<HuffmanResult> all_results(TEST_REPETITIONS);

    for (int i = 0; i < TEST_REPETITIONS; ++i) {
        auto start = chrono::high_resolution_clock::now();
        HuffmanResult result = compressHuffman(data);
        auto end = chrono::high_resolution_clock::now();

        result.duration = chrono::duration<double, nano>(end - start).count();
        all_results[i] = result;
    }

    return all_results;
}

int main() {
    string filename = "huffman.txt";
    auto results = testHuffmanCompression(filename);

     if (results.empty()) {
        std::cerr << "No results returned. Possibly file not found or empty.\n";
        return 1;
    }

    double totalDuration = 0.0;
    int originalSize = 0;
    int compressedSize = 0;

    for (const auto& res : results) {
        totalDuration += res.duration;
        originalSize = res.originalSize;
        compressedSize = res.compressedSize;
    }

    double avgDuration = totalDuration / results.size();

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Huffman Compression average time: " << avgDuration << " nanoseconds.\n";
    std::cout << "Initial size: " << originalSize << " characters, Compressed size: "
              << compressedSize << " bytes.\n";

    return 0;
}
