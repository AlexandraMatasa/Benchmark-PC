import sys
sys.path.append("../bindings/build")
import binding_cpuInfo

cpu_info = binding_cpuInfo.get_cpu_info()

print("--- CPU Information ---")
print(f"{cpu_info.frequency}")
print(f"{cpu_info.type}")
print(f"Number of cores: {cpu_info.cores}")
print(f"Threads: {cpu_info.threads}")

if cpu_info.cores > 0:
    threads_per_core = cpu_info.threads // cpu_info.cores
    print(f"Threads per core: {threads_per_core}")
else:
    print("Unable to calculate threads per core.")
