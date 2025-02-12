import sys
sys.path.append("../bindings/build") 
import binding_monteCarloPi

iterations = 1000000
all_results = binding_monteCarloPi.calculate_pi(iterations)

pi_values = [float(result.pi) for result in all_results]
durations = [float(result.duration.split()[0]) for result in all_results]

avg_pi = sum(pi_values) / len(pi_values)
avg_duration = sum(durations) / len(durations)

print(f"--- Monte Carlo Pi Test ---")
for i, result in enumerate(all_results, 1):
    print(f"Repetition {i}: Pi = {result.pi}, Duration = {result.duration}")

print("\n--- Average Results ---")
print(f"Average Pi: {avg_pi:.6f}")
print(f"Average Duration: {avg_duration:.5f} ns")
