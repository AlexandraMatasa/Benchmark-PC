import sys
sys.path.append("../bindings/build")
import binding_taylorSeriesExponential

x = 20.0
n = 1000

results = binding_taylorSeriesExponential.compute_exponential_taylor(x, n)

for i, result in enumerate(results):
    print(f"\nResults for repetition {i + 1}:")
    print(f"Estimated e^{x}: {result.estimated_value}")
    print(f"Computation Duration: {result.duration}")