#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../cpp/include/cpuInfo.h"

namespace py = pybind11;

PYBIND11_MODULE(binding_cpuInfo, m) {
    m.doc() = "Python bindings for CPU information module";

    py::class_<CPUInfo>(m, "CPUInfo")
        .def_readonly("frequency", &CPUInfo::frequency)
        .def_readonly("type", &CPUInfo::type)
        .def_readonly("cores", &CPUInfo::cores)
        .def_readonly("threads", &CPUInfo::threads);

    m.def("get_cpu_info", &get_cpu_info, "Get CPU info as a single result");

    m.def("get_cpu_frequency", &getCPUFrequency, "Get the CPU frequency as a string");
    m.def("get_cpu_type", &getCPUType, "Get the CPU type (Vendor and Model) as a string");
    m.def("get_cpu_core_info", [](void) {
        int cores = 0, threads = 0;
        if (getCpuCoreInfo(cores, threads) == 0) {
            return std::make_pair(cores, threads);
        } else {
            throw std::runtime_error("Failed to retrieve core information");
        }
    }, "Get the number of cores and threads as a tuple (cores, threads)");
}
