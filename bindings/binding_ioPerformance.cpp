#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../cpp/include/ioPerformance.h"

namespace py = pybind11;

PYBIND11_MODULE(binding_ioPerformance, m) {
    m.doc() = "Python bindings for IO Performance module";

    py::class_<IOResult>(m, "IOResult")
        .def_readonly("file_size", &IOResult::fileSize)
        .def_readonly("write_duration", &IOResult::writeDuration)
        .def_readonly("read_duration", &IOResult::readDuration)
        .def_readonly("write_speed", &IOResult::writeSpeed)
        .def_readonly("read_speed", &IOResult::readSpeed)
        .def_readonly("avg_write_time_per_byte", &IOResult::avgWriteTimePerByte)
        .def_readonly("avg_read_time_per_byte", &IOResult::avgReadTimePerByte);

    m.def(
        "testIOPerformance",
        &testIOPerformance,
        "Test I/O performance with file size and filename. Returns a list of IOResult objects for each repetition.",
        py::arg("file_size"),
        py::arg("filename")
    );
}
