#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../cpp/include/huffmanCoding.h"
#include "../include/config.h"

namespace py = pybind11;

PYBIND11_MODULE(binding_huffmanCoding, m) {
    m.doc() = "Python bindings for Huffman Coding module";

   py::class_<HuffmanResult>(m, "HuffmanResult")
        .def_readonly("duration", &HuffmanResult::duration)
        .def_readonly("originalSize", &HuffmanResult::originalSize)
        .def_readonly("compressedSize", &HuffmanResult::compressedSize);

    m.def("compress_huffman", &compressHuffman, "Compress a string using Huffman coding",
          py::arg("data"));

    m.def("test_huffman_compression", &testHuffmanCompression, "Test Huffman compression using a file",
          py::arg("filename"));
}
