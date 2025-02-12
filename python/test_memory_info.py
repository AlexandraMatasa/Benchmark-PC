import sys
sys.path.append("../bindings/build")
import binding_memoryInfo

results = binding_memoryInfo.get_memory_info_and_access_time()

for i, result in enumerate(results):
    print(f"--- Memory Information (Repetition {i + 1}) ---")
    print(f"Total Memory: {result.total_memory_gb:.5f} GB")
    print(f"Available Memory: {result.available_memory_mb:.5f} MB")
    print(f"Block Size: {result.block_size_mb} MB")
    print(f"Total Access Time: {result.total_access_time}")
    print(f"Access Time per Byte: {result.access_time_per_byte}\n")