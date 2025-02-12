import sys
sys.path.append("../bindings/build")
import binding_huffmanCoding

filename = "huffman.txt"
results = binding_huffmanCoding.test_huffman_compression(filename)

for i, result in enumerate(results):
    print(f"\nResults for repetition {i + 1}:")
    print(f"Original Size: {result.originalSize} characters")
    print(f"Compressed Size: {result.compressedSize} bytes")
    print(f"Compression Duration: {result.duration} nanoseconds")