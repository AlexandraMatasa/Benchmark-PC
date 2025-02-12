#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../cpp/include/bitManipulation.h"
#include "../include/config.h"


namespace py = pybind11;

PYBIND11_MODULE(binding_bitManipulation, m) {
    m.doc() = "Python bindings for Bit Manipulation operations";

    py::class_<BitManipulationResults>(m, "BitManipulationResults")
        .def_readonly("shiftRightTime", &BitManipulationResults::shiftRightTime)
        .def_readonly("shiftLeftTime", &BitManipulationResults::shiftLeftTime)
        .def_readonly("andTime", &BitManipulationResults::andTime)
        .def_readonly("orTime", &BitManipulationResults::orTime)
        .def_readonly("xorTime", &BitManipulationResults::xorTime)
        .def_readonly("notTime", &BitManipulationResults::notTime);

    m.def("generate_random_data", &generate_random_data,
          py::arg("num_elements"),
          "Generate random data for bit manipulation testing");

    m.def("test_bit_manipulation", &test_bit_manipulation,
          py::arg("data"),
          "Test various bit manipulation operations on the given data and return a list of results for each repetition");
}
