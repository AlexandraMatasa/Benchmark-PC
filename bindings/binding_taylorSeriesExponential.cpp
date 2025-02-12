#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../cpp/include/taylorSeriesExponential.h"

namespace py = pybind11;

PYBIND11_MODULE(binding_taylorSeriesExponential, m) {
    m.doc() = "Python bindings for computing exponential using Taylor series";

    py::class_<TaylorSeriesResult>(m, "TaylorSeriesResult")
        .def_readonly("estimated_value", &TaylorSeriesResult::estimatedValue)
        .def_readonly("duration", &TaylorSeriesResult::duration);

      m.def("compute_exponential_taylor", &compute_exponential_taylor, 
          "Compute the Taylor series exponential for x and n terms",
          py::arg("x"), py::arg("n"));
}
