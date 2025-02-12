import sys
sys.path.append("../bindings/build")
import binding_memoryTransferSpeed

results = binding_memoryTransferSpeed.test_memory_transfer_speed()

for i, result in enumerate(results):
    print(f"Repetition {i+1}:")
    print(f"  Block size: {result.block_size}")
    print(f"  Transfer Speed: {result.speed}")
    print(f"  Total Duration: {result.duration}")
    print(f"  Average Time per Byte: {result.avg_time_per_byte}")
    print()
