#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../cpp/include/memoryInfo.h"
#include "../include/config.h"

namespace py = pybind11;

PYBIND11_MODULE(binding_memoryInfo, m) {
    m.doc() = "Python bindings for Memory Information module";

    py::class_<MemoryInfo>(m, "MemoryInfo")
        .def_readonly("total_memory_gb", &MemoryInfo::totalMemoryGB, "Total memory in GB")
        .def_readonly("available_memory_mb", &MemoryInfo::availableMemoryMB, "Available memory in MB")
        .def_readonly("block_size_mb", &MemoryInfo::blockSizeMB, "Block size used for access tests in MB")
        .def_readonly("total_access_time", &MemoryInfo::totalAccessTime, "Total access time in nanoseconds")
        .def_readonly("access_time_per_byte", &MemoryInfo::accessTimePerByte, "Access time per byte in nanoseconds");

    m.def(
        "get_memory_info_and_access_time",
        &get_memory_info_and_access_time,
        "Get memory information and access time for a dynamically calculated block size. Returns a list of MemoryInfo for each repetition."
    );
    m.def("get_available_memory_in_mb", &get_available_memory_in_mb);
}
