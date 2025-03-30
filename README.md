Benchmark-PC 🖥️📊⏱️

A system performance testing tool developed in C++ that evaluates various hardware components through customized benchmarks.

✨ What It Does
- Analyzes CPU specifications and frequency.
- Evaluates memory capacity and access speed.
- Tests arithmetic and logical performance using: Huffman coding, Monte Carlo Pi estimation, Bit manipulation, Taylor series for exponential calculation.
- Measures I/O performance and memory transfer speed.
- Aggregates all results into a final benchmark score.
- Exposes results to a Python-based GUI via Pybind11.
  

📌 Key Features
- Accurate timing via std::chrono for nanosecond precision.
- Cross-language functionality: C++ core with Python GUI via Pybind11
- Reference Scoring: Rates performance against baseline systems
- Modular design: independent test components for easy maintenance
