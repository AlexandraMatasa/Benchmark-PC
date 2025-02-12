#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../cpp/include/memoryTransferSpeed.h"

namespace py = pybind11;

PYBIND11_MODULE(binding_memoryTransferSpeed, m) {
    m.doc() = "Python bindings for Memory Transfer Speed Module";

    py::class_<MemoryTransferResult>(m, "MemoryTransferResult")
        .def_readonly("block_size", &MemoryTransferResult::blockSize)
        .def_readonly("speed", &MemoryTransferResult::speed)
        .def_readonly("duration", &MemoryTransferResult::duration)
        .def_readonly("avg_time_per_byte", &MemoryTransferResult::avgTimePerByte);

    m.def("test_memory_transfer_speed", &test_memory_transfer_speed,
          "Test the memory transfer speed for a fixed block size with multiple repetitions");
}
