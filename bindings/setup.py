from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

import os

VENV_PATH = "D:/AN3-SEM1/SSC-PROIECT/BenchmarkPC/.venv"

extensions = [
    Extension(
        "binding_cpuInfo",
        sources=["binding_cpuInfo.cpp", "../cpp/src/cpuInfo.cpp"],
        include_dirs=[
            f"{VENV_PATH}/Lib/site-packages/pybind11/include",
            "../cpp/include"
        ],
        library_dirs=[
            f"{VENV_PATH}/libs"
        ],
        libraries=[
            "python313",
            "Advapi32"
        ],
        language="c++",
    ),
    Extension(
        "binding_memoryInfo",
        sources=["binding_memoryInfo.cpp", "../cpp/src/memoryInfo.cpp"],
        include_dirs=[
            f"{VENV_PATH}/Lib/site-packages/pybind11/include",
            "../cpp/include"
        ],
        library_dirs=[
            f"{VENV_PATH}/libs"
        ],
        libraries=[
            "python313"
        ],
        language="c++",
    ),
    Extension(
        "binding_huffmanCoding",
        sources=["binding_huffmanCoding.cpp", "../cpp/src/huffmanCoding.cpp"],
        include_dirs=[
            f"{VENV_PATH}/Lib/site-packages/pybind11/include",
            "../cpp/include"
        ],
        library_dirs=[
            f"{VENV_PATH}/libs"
        ],
        libraries=[
            "python313"
        ],
        language="c++",
    ),
    Extension(
        "binding_monteCarloPi",
        sources=["binding_monteCarloPi.cpp", "../cpp/src/monteCarloPi.cpp"],
        include_dirs=[
            f"{VENV_PATH}/Lib/site-packages/pybind11/include",
            "../cpp/include"
        ],
        library_dirs=[
            f"{VENV_PATH}/libs"
        ],
        libraries=[
            "python313"
        ],
        language="c++",
    ),
    Extension(
        "binding_bitManipulation",
        sources=["binding_bitManipulation.cpp", "../cpp/src/bitManipulation.cpp"],
        include_dirs=[
            f"{VENV_PATH}/Lib/site-packages/pybind11/include",
            "../cpp/include"
        ],
        library_dirs=[
            f"{VENV_PATH}/libs"
        ],
        libraries=[
            "python313"
        ],
        language="c++",
    ),
    Extension(
        "binding_taylorSeriesExponential",
        sources=["binding_taylorSeriesExponential.cpp", "../cpp/src/taylorSeriesExponential.cpp"],
        include_dirs=[
            f"{VENV_PATH}/Lib/site-packages/pybind11/include",
            "../cpp/include"
        ],
        library_dirs=[
            f"{VENV_PATH}/libs"
        ],
        libraries=[
            "python313"
        ],
        language="c++",
    ),
    Extension(
        "binding_memoryTransferSpeed",
        sources=["binding_memoryTransferSpeed.cpp", "../cpp/src/memoryTransferSpeed.cpp"],
        include_dirs=[
            f"{VENV_PATH}/Lib/site-packages/pybind11/include",
            "../cpp/include"
        ],
        library_dirs=[
            f"{VENV_PATH}/libs"
        ],
        libraries=[
            "python313"
        ],
        language="c++",
    ),
    Extension(
        "binding_ioPerformance",
        sources=["binding_ioPerformance.cpp", "../cpp/src/ioPerformance.cpp"],
        include_dirs=[
            f"{VENV_PATH}/Lib/site-packages/pybind11/include",
            "../cpp/include"
        ],
        library_dirs=[
            f"{VENV_PATH}/libs"
        ],
        libraries=[
            "python313"
        ],
        language="c++",
    )
]

class CustomBuildExtCommand(build_ext):
    """Custom build_ext command to place .pyd files in a specific directory."""
    def get_ext_fullpath(self, ext_name):
        path = super().get_ext_fullpath(ext_name)
        return os.path.join("build", os.path.basename(path))

setup(
    name="BenchmarkPCBindings",
    version="0.1",
    author="Matasa Alexandra",
    description="Pybind11 bindings for BenchmarkPC C++ modules",
    ext_modules=extensions,
    cmdclass={"build_ext": CustomBuildExtCommand},
)
