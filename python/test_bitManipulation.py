import sys
sys.path.append("../bindings/build")
import binding_bitManipulation

data = binding_bitManipulation.generate_random_data(1000000)

results = binding_bitManipulation.test_bit_manipulation(data)

for i, result in enumerate(results):
    print(f"\nResults for repetition {i + 1}:")
    print(f"Time for right shift operations: {result.shiftRightTime}")
    print(f"Time for left shift operations: {result.shiftLeftTime}")
    print(f"Time for AND operations: {result.andTime}")
    print(f"Time for OR operations: {result.orTime}")
    print(f"Time for XOR operations: {result.xorTime}")
    print(f"Time for NOT operations: {result.notTime}")
