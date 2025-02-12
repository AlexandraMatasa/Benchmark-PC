#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

#include <memory>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>

struct HuffmanNode {
    char character;
    int frequency;
    std::unique_ptr<HuffmanNode> left, right;
    HuffmanNode(char ch, int freq);
};

struct HuffmanResult {
    double duration;
    size_t originalSize;
    size_t compressedSize;
};

struct Compare {
    bool operator()(const HuffmanNode* left, const HuffmanNode* right);
};

void generateCodes(const HuffmanNode* root, const std::string& code, std::unordered_map<char, std::string>& huffmanCode);
HuffmanResult compressHuffman(const std::string& data);
std::vector<HuffmanResult> testHuffmanCompression(const std::string& filename);

#endif