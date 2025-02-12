#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../cpp/include/monteCarloPi.h"

PYBIND11_MODULE(binding_monteCarloPi, m) {
    m.doc() = "Python bindings for Monte Carlo Pi calculation";

    pybind11::class_<MonteCarloPi>(m, "MonteCarloPi")
        .def_readonly("pi", &MonteCarloPi::pi)
        .def_readonly("duration", &MonteCarloPi::duration);

    m.def("calculate_pi", &calculate_pi, "Calculate the value of Pi using Monte Carlo simulation",
          pybind11::arg("iterations"));
}
