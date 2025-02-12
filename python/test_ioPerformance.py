import sys
sys.path.append("../bindings/build")
import binding_ioPerformance

file_size = 100 * 1024 * 1024  # 100MB in bytes
filename = "testFile_1MB.bin"

all_results = binding_ioPerformance.testIOPerformance(file_size, filename)

for i, result in enumerate(all_results):
    print(f"Repetition {i + 1} for File Size: {result.file_size}")
    print(f"  Write Duration: {result.write_duration}")
    print(f"  Read Duration: {result.read_duration}")
    print(f"  Write Speed: {result.write_speed}")
    print(f"  Read Speed: {result.read_speed}")
    print(f"  Avg Write Time/Byte: {result.avg_write_time_per_byte}")
    print(f"  Avg Read Time/Byte: {result.avg_read_time_per_byte}\n")
