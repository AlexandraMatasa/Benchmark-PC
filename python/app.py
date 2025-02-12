import os
import sys
import tkinter as tk
from tkinter import ttk, messagebox
import matplotlib.pyplot as plt
import numpy as np
import re

os.environ["TCL_LIBRARY"] = r"C:\Program Files\Python313\tcl\tcl8.6"
os.environ["TK_LIBRARY"] = r"C:\Program Files\Python313\tcl\tk8.6"

sys.path.append("../bindings/build")

import binding_bitManipulation
import binding_cpuInfo
import binding_huffmanCoding
import binding_ioPerformance
import binding_memoryInfo
import binding_memoryTransferSpeed
import binding_monteCarloPi
import binding_taylorSeriesExponential


class BenchmarkApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Benchmark PC")
        self.root.geometry("600x600")
        
        self.reference = {
            "cpu_frequency": 1992,  # MHz
            "cores": 4,
            "threads": 8,
            "bit_manipulation": {
                "shift_right": 356495,  # ns
                "shift_left": 566445,  # ns
                "and_time": 351190,  # ns
                "or_time": 511455,  # ns
                "xor_time": 355625,  # ns
                "not_time": 777030,  # ns
            },
            "huffman": {
                "compression_duration": 4942640,  # ns
            },
            "io_performance": {
                "write_speed": 129.61,  # MB/s
                "read_speed": 1104.89,  # MB/s
            },
            "memory_info": {
                "access_time_per_byte": 0.31971,  # ns/byte
                "total_memory": 7.8853,  # GB
            },
            "memory_transfer_speed": 6342.81,  # MB/s
            "monte_carlo_pi": 17431965,  # ns
            "taylor_series": 1720.0,  # ns
        }

        self.weights = {
            "cpu_frequency": 0.15,
            "bit_manipulation": 0.15,
            "huffman": 0.10,
            "io_performance": 0.15,
            "memory_info": 0.15,
            "memory_transfer_speed": 0.20,
            "monte_carlo_pi": 0.05,
            "taylor_series": 0.05,
        }

        self.test_frame = ttk.LabelFrame(self.root, text="Select Tests")
        self.test_frame.pack(fill="x", padx=10, pady=10)

        # Checkbox for CPU Info Test
        self.cpu_test_var = tk.BooleanVar()
        ttk.Checkbutton(
            self.test_frame, text="CPU Info Test", variable=self.cpu_test_var
        ).pack(anchor="w")

        # Checkbox for Bit Manipulation Test
        self.bit_manipulation_test_var = tk.BooleanVar()
        ttk.Checkbutton(
            self.test_frame, text="Bit Manipulation Test", variable=self.bit_manipulation_test_var
        ).pack(anchor="w")

        # Checkbox for Huffman Coding Test
        self.huffman_coding_test_var = tk.BooleanVar()
        ttk.Checkbutton(
            self.test_frame, text="Huffman Coding Test", variable=self.huffman_coding_test_var
        ).pack(anchor="w")

        # Checkbox for I/O Performance Test
        self.io_performance_test_var = tk.BooleanVar()
        ttk.Checkbutton(
            self.test_frame, text="I/O Performance Test", variable=self.io_performance_test_var
        ).pack(anchor="w")

        # Checkbox for Memory Info Test
        self.memory_info_test_var = tk.BooleanVar()
        ttk.Checkbutton(
            self.test_frame, text="Memory Info Test", variable=self.memory_info_test_var
        ).pack(anchor="w")

        # Checkbox for Memory Transfer Speed Test
        self.memory_transfer_test_var = tk.BooleanVar()
        ttk.Checkbutton(
            self.test_frame, text="Memory Transfer Speed Test", variable=self.memory_transfer_test_var
        ).pack(anchor="w")

        # Checkbox for Monte Carlo Pi Test
        self.monte_carlo_pi_test_var = tk.BooleanVar()
        ttk.Checkbutton(
            self.test_frame, text="Monte Carlo Pi Test", variable=self.monte_carlo_pi_test_var
        ).pack(anchor="w")

        # Checkbox for Taylor Series Exponential Test
        self.taylor_test_var = tk.BooleanVar()
        ttk.Checkbutton(
            self.test_frame, text="Taylor Series Exponential Test", variable=self.taylor_test_var
        ).pack(anchor="w")


        self.run_button = ttk.Button(self.root, text="Run Test(s)", command=self.run_tests)
        self.run_button.pack(pady=10)

        self.result_frame = ttk.LabelFrame(self.root, text="Results")
        self.result_frame.pack(fill="both", expand=True, padx=10, pady=10)

        self.result_text = tk.Text(self.result_frame, wrap="word", state="disabled", height=10)
        self.result_text.pack(fill="both", expand=True, padx=10, pady=10)

    def calculate_score(self, test_results):
        total_score = 0
        for key, weight in self.weights.items():
            if key == "bit_manipulation":
                score = sum(
                    float(self.reference[key][metric]) / float(test_results[key][metric])
                    for metric in self.reference[key]
                ) / len(self.reference[key])

            elif key == "memory_info":
                memory_score = float(test_results[key]["total_memory"]) / float(self.reference[key]["total_memory"])
                access_time_score = float(self.reference[key]["access_time_per_byte"]) / float(test_results[key]["access_time_per_byte"])
                score = (memory_score * 0.4 + access_time_score * 0.6)

            elif key == "huffman":
                score = float(self.reference[key]["compression_duration"]) / float(test_results[key]["compression_duration"])            
            elif key == "io_performance":
                write_score = float(test_results[key]["write_speed"]) / float(self.reference[key]["write_speed"])
                read_score = float(test_results[key]["read_speed"]) / float(self.reference[key]["read_speed"])
                score = (write_score + read_score) / 2.0

            else:
                score = float(self.reference[key]) / float(test_results[key])

            total_score += score * weight
        return total_score * 100
    

    def run_tests(self):
        """Runs the selected tests and displays the results."""
        if not any([
            self.cpu_test_var.get(), 
            self.bit_manipulation_test_var.get(), 
            self.huffman_coding_test_var.get(), 
            self.io_performance_test_var.get(), 
            self.memory_info_test_var.get(), 
            self.memory_transfer_test_var.get(), 
            self.monte_carlo_pi_test_var.get(), 
            self.taylor_test_var.get()
        ]):
            messagebox.showwarning("Warning", "Please select at least one test.")
            return

        test_results = {}

        test_results["cpu_frequency"] = 0
        test_results["cores"] = 0
        test_results["threads"] = 0

        results = ""

        # Run CPU Info Test
        if self.cpu_test_var.get():
            try:
                cpu_info = binding_cpuInfo.get_cpu_info()
                results += "--- CPU Info Test ---\n"
                results += f"  Frequency: {cpu_info.frequency}\n"
                results += f"  {cpu_info.type}\n"
                results += f"  Cores: {cpu_info.cores}\n"
                results += f"  Threads: {cpu_info.threads}\n\n"

                cpu_frequency = float(re.search(r'\d+', cpu_info.frequency).group())
                test_results["cpu_frequency"] = cpu_frequency
                cores = float(cpu_info.cores)
                threads = float(cpu_info.threads)
                test_results["cores"] = cores
                test_results["threads"] = threads

            except Exception as e:
                results += f"An error occurred in CPU Info Test: {e}\n\n"

        # Run Bit Manipulation Test
        if self.bit_manipulation_test_var.get():
            try:
                num_elements = 1000000
                data = binding_bitManipulation.generate_random_data(num_elements)
                all_bit_results = binding_bitManipulation.test_bit_manipulation(data)

                avg_shift_right = np.mean([float(result.shiftRightTime.split()[0]) for result in all_bit_results])
                avg_shift_left = np.mean([float(result.shiftLeftTime.split()[0]) for result in all_bit_results])
                avg_and_time = np.mean([float(result.andTime.split()[0]) for result in all_bit_results])
                avg_or_time = np.mean([float(result.orTime.split()[0]) for result in all_bit_results])
                avg_xor_time = np.mean([float(result.xorTime.split()[0]) for result in all_bit_results])
                avg_not_time = np.mean([float(result.notTime.split()[0]) for result in all_bit_results])

                results += "--- Bit Manipulation Test (Averages) ---\n"
                results += f"  Average Shift Right Time: {avg_shift_right:.5f} ns\n"
                results += f"  Average Shift Left Time: {avg_shift_left:.5f} ns\n"
                results += f"  Average AND Time: {avg_and_time:.5f} ns\n"
                results += f"  Average OR Time: {avg_or_time:.5f} ns\n"
                results += f"  Average XOR Time: {avg_xor_time:.5f} ns\n"
                results += f"  Average NOT Time: {avg_not_time:.5f} ns\n\n"

                test_results["bit_manipulation"] = {
                "shift_right": avg_shift_right,
                "shift_left": avg_shift_left,
                "and_time": avg_and_time,
                "or_time": avg_or_time,
                "xor_time": avg_xor_time,
                "not_time": avg_not_time,
    }
                self.plot_bit_results(all_bit_results)

            except Exception as e:
                results += f"An error occurred in Bit Manipulation Test: {e}\n\n"

        # Run Huffman Coding Test
        if self.huffman_coding_test_var.get():
            try:
                filename = "huffman.txt"
                all_huffman_results = binding_huffmanCoding.test_huffman_compression(filename)

                avg_duration = np.mean([result.duration for result in all_huffman_results])
                avg_original_size = np.mean([result.originalSize for result in all_huffman_results])
                avg_compressed_size = np.mean([result.compressedSize for result in all_huffman_results])

                results += "--- Huffman Coding Test (Averages) ---\n"
                results += f"  Average Original Size: {avg_original_size} characters\n"
                results += f"  Average Compressed Size: {avg_compressed_size} bytes\n"
                results += f"  Average Compression Duration: {avg_duration:.5f} ns\n\n"

                test_results["huffman"] = {
                    "compression_duration": avg_duration,
                }

                self.plot_huffman_results(all_huffman_results)

            except Exception as e:
                results += f"An error occurred in Huffman Coding Test: {e}\n\n"

        # Run I/O Performance Test
        if self.io_performance_test_var.get():
            try:
                file_size = 100 * 1024 * 1024  # 100MB in bytes
                filename = "testFile_100MB.bin"
                all_io_results = binding_ioPerformance.testIOPerformance(file_size, filename)

                write_durations = [float(result.write_duration.split()[0]) for result in all_io_results]
                read_durations = [float(result.read_duration.split()[0]) for result in all_io_results]
                write_speeds = [float(result.write_speed.split()[0]) for result in all_io_results]
                read_speeds = [float(result.read_speed.split()[0]) for result in all_io_results]
                avg_write_time_per_byte = [float(result.avg_write_time_per_byte.split()[0]) for result in all_io_results]
                avg_read_time_per_byte = [float(result.avg_read_time_per_byte.split()[0]) for result in all_io_results]

                avg_write_duration = np.mean(write_durations)
                avg_read_duration = np.mean(read_durations)
                avg_write_speed = np.mean(write_speeds)
                avg_read_speed = np.mean(read_speeds)
                avg_write_time_byte = np.mean(avg_write_time_per_byte)
                avg_read_time_byte = np.mean(avg_read_time_per_byte)

                results += "--- I/O Performance Test (Averages) ---\n"
                results += f"  Average Write Duration: {avg_write_duration:.5f} ns\n"
                results += f"  Average Read Duration: {avg_read_duration:.5f} ns\n"
                results += f"  Average Write Speed: {avg_write_speed:.2f} MB/s\n"
                results += f"  Average Read Speed: {avg_read_speed:.2f} MB/s\n"
                results += f"  Average Write Time/Byte: {avg_write_time_byte:.5f} ns/byte\n"
                results += f"  Average Read Time/Byte: {avg_read_time_byte:.5f} ns/byte\n\n"
                
                test_results["io_performance"] = {
                    "write_speed": avg_write_speed,
                    "read_speed": avg_read_speed,
                }

                self.plot_io_results(all_io_results)
            except Exception as e:
                results += f"An error occurred in I/O Performance Test: {e}\n\n"

       # Run Memory Info Test
        if self.memory_info_test_var.get():
            try:
                all_memory_results = binding_memoryInfo.get_memory_info_and_access_time()

                total_access_times = [float(result.total_access_time.split()[0]) for result in all_memory_results]
                access_times_per_byte = [float(result.access_time_per_byte.split()[0]) for result in all_memory_results]

                avg_total_access_time = np.mean(total_access_times)
                avg_access_time_per_byte = np.mean(access_times_per_byte)

                total_memory = all_memory_results[0].total_memory_gb
                available_memory = all_memory_results[0].available_memory_mb
                block_size = all_memory_results[0].block_size_mb 

                results += "--- Memory Information Test (Averages) ---\n"
                results += f"  Total Memory: {total_memory:.5f} GB\n"
                results += f"  Available Memory: {available_memory:.5f} MB\n"
                results += f"  Block Size Used: {block_size} MB\n"
                results += f"  Average Total Access Time: {avg_total_access_time:.5f} ns\n"
                results += f"  Average Access Time per Byte: {avg_access_time_per_byte:.5f} ns/byte\n\n"

                test_results["memory_info"] = {
                    "total_memory": total_memory,
                    "access_time_per_byte": avg_access_time_per_byte,
                }

                self.plot_memory_results(all_memory_results)

            except Exception as e:
                results += f"An error occurred in Memory Info Test: {e}\n\n"
       
        # Run Memory Transfer Speed Test
        if self.memory_transfer_test_var.get():
            try:
                all_results = binding_memoryTransferSpeed.test_memory_transfer_speed()

                transfer_speeds = [float(result.speed.split()[0]) for result in all_results]
                total_durations = [float(result.duration.split()[0]) for result in all_results]
                avg_times_per_byte = [float(result.avg_time_per_byte.split()[0]) for result in all_results]

                avg_transfer_speed = np.mean(transfer_speeds)
                avg_total_duration = np.mean(total_durations)
                avg_time_per_byte = np.mean(avg_times_per_byte)

                results += "--- Memory Transfer Speed Test (Averages) ---\n"
                results += f"  Block Size: {all_results[0].block_size}\n"
                results += f"  Average Transfer Speed: {avg_transfer_speed:.2f} MB/s\n"
                results += f"  Average Total Duration: {avg_total_duration:.5f} ns\n"
                results += f"  Average Time per Byte: {avg_time_per_byte:.5f} ns/byte\n\n"

                test_results["memory_transfer_speed"] = avg_transfer_speed

                self.plot_memory_transfer_speed(transfer_speeds, total_durations, avg_times_per_byte)

            except Exception as e:
                results += f"An error occurred in Memory Transfer Speed Test: {e}\n\n"

        # Run Monte Carlo Pi Test
        if self.monte_carlo_pi_test_var.get():
            try:
                iterations = 1000000
                all_results = binding_monteCarloPi.calculate_pi(iterations)

                pi_values = [float(result.pi) for result in all_results]
                durations = [float(result.duration.split()[0]) for result in all_results]

                avg_pi = sum(pi_values) / len(pi_values)
                avg_duration = sum(durations) / len(durations)

                results += "--- Monte Carlo Pi Test (Averages) ---\n"
                results += f"  Average Pi: {avg_pi:.6f}\n"
                results += f"  Average Duration: {avg_duration:.5f} ns\n\n"

                test_results["monte_carlo_pi"] = avg_duration

                self.plot_monte_carlo_results(pi_values, durations)
            except Exception as e:
                results += f"An error occurred in Monte Carlo Pi Test: {e}\n\n"

        # Run Taylor Series Exponential Test
        if self.taylor_test_var.get():
            try:
                x = 20.0  # The x value for e^x
                n = 1000  # The number of terms for the Taylor series
                all_results = binding_taylorSeriesExponential.compute_exponential_taylor(x, n)

                estimated_values = [float(result.estimated_value) for result in all_results]
                durations = [float(result.duration.split()[0]) for result in all_results]

                avg_value = np.mean(estimated_values)
                avg_duration = np.mean(durations)

                results += "--- Taylor Series Exponential Test (Averages) ---\n"
                results += f"  Average Estimated e^{x}: {avg_value:.10f}\n"
                results += f"  Average Duration: {avg_duration:.5f} ns\n\n"
                
                test_results["taylor_series"] = avg_duration

                self.plot_taylor_series_results(estimated_values, durations, x)

            except Exception as e:
                results += f"An error occurred in Taylor Series Exponential Test: {e}\n\n"
        
        try:
            all_tests_run = True
            for required_key in self.weights.keys():
                if required_key not in test_results:
                    all_tests_run = False
                    break

            if all_tests_run:
                final_score = self.calculate_score(test_results)
                results += f"--- Final Score ---\nYour System Score: {final_score:.2f} / 100\n\n"
            else:
                results += "You did not run all tests. To see the final score, please run all tests.\n\n"
        except Exception as e:
            results += f"An error occurred while calculating the final score: {e}\n\n"


        self.result_text.config(state="normal")
        self.result_text.delete("1.0", tk.END)
        self.result_text.insert(tk.END, results)
        self.result_text.config(state="disabled")



    def plot_bit_results(self, all_bit_results):
        """Plot the results of the bit manipulation test."""
        repetitions = len(all_bit_results)

        shift_right_times = [float(result.shiftRightTime.split()[0]) for result in all_bit_results]
        shift_left_times = [float(result.shiftLeftTime.split()[0]) for result in all_bit_results]
        and_times = [float(result.andTime.split()[0]) for result in all_bit_results]
        or_times = [float(result.orTime.split()[0]) for result in all_bit_results]
        xor_times = [float(result.xorTime.split()[0]) for result in all_bit_results]
        not_times = [float(result.notTime.split()[0]) for result in all_bit_results]

        x = np.arange(1, repetitions + 1)

        plt.figure(figsize=(10, 6))
        plt.plot(x, shift_right_times, label="Shift Right", marker="o")
        plt.plot(x, shift_left_times, label="Shift Left", marker="o")
        plt.plot(x, and_times, label="AND", marker="o")
        plt.plot(x, or_times, label="OR", marker="o")
        plt.plot(x, xor_times, label="XOR", marker="o")
        plt.plot(x, not_times, label="NOT", marker="o")

        plt.xlabel("Repetition")
        plt.ylabel("Time (nanoseconds)")
        plt.title("Bit Manipulation Test Results")
        plt.legend()
        plt.grid(True, linestyle="--", alpha=0.6)
        plt.show(block=False)

    def plot_huffman_results(self, all_huffman_results):
        durations = [result.duration for result in all_huffman_results]
        x = np.arange(1, len(all_huffman_results) + 1)

        plt.figure(figsize=(10, 6))
        plt.plot(x, durations, label="Compression Duration", marker="o")
        plt.xlabel("Repetition")
        plt.ylabel("Time (nanoseconds)")
        plt.title("Huffman Coding Test Results")
        plt.legend()
        plt.grid(True, linestyle="--", alpha=0.6)
        plt.show(block=False)

    def plot_io_results(self, all_io_results):
        """Plot the results of the I/O performance test."""
        repetitions = len(all_io_results)
        x = np.arange(1, repetitions + 1)

        write_durations = [float(result.write_duration.split()[0]) for result in all_io_results]
        read_durations = [float(result.read_duration.split()[0]) for result in all_io_results]

        plt.figure(figsize=(10, 6))
        plt.plot(x, write_durations, label="Write Duration", marker="o")
        plt.plot(x, read_durations, label="Read Duration", marker="o")
        plt.xlabel("Repetition")
        plt.ylabel("Time (nanoseconds)")
        plt.title("I/O Performance Test Results")
        plt.legend()
        plt.grid(True, linestyle="--", alpha=0.6)
        plt.show(block=False)
    
    def plot_memory_results(self, all_memory_results):
        access_times = [float(result.access_time_per_byte.split()[0]) for result in all_memory_results]
        x = np.arange(1, len(all_memory_results) + 1)
        plt.figure(figsize=(10, 6))
        plt.plot(x, access_times, label="Access Time per Byte", marker="o")
        plt.xlabel("Repetition")
        plt.ylabel("Time (nanoseconds)")
        plt.title("Memory Info Test Results")
        plt.legend()
        plt.grid(True, linestyle="--", alpha=0.6)
        plt.show(block=False)

    def plot_memory_transfer_speed(self, transfer_speeds, total_durations, avg_times_per_byte):
        """Plot the results of the memory transfer speed test."""
        repetitions = np.arange(1, len(transfer_speeds) + 1)

        fig, axs = plt.subplots(1, 2, figsize=(12, 6))

        axs[0].plot(repetitions, transfer_speeds, label="Transfer Speed (MB/s)", marker="o", color="blue")
        axs[0].set_xlabel("Repetition")
        axs[0].set_ylabel("Speed (MB/s)")
        axs[0].set_title("Transfer Speed per Repetition")
        axs[0].grid(True, linestyle="--", alpha=0.6)
        axs[0].legend()

        axs[1].plot(repetitions, avg_times_per_byte, label="Time Per Byte (ns/byte)", marker="o", color="green")
        axs[1].set_xlabel("Repetition")
        axs[1].set_ylabel("Time Per Byte (ns/byte)")
        axs[1].set_title("Time Per Byte per Repetition")
        axs[1].grid(True, linestyle="--", alpha=0.6)
        axs[1].legend()

        plt.tight_layout()
        plt.show(block=False)

    
    def plot_monte_carlo_results(self, pi_values):
        """Plot the Monte Carlo Pi estimates for each repetition."""
        repetitions = range(1, len(pi_values) + 1)
        plt.figure(figsize=(10, 6))
        plt.plot(repetitions, pi_values, label="Estimated Pi", marker="o")
        plt.axhline(y=3.14159, color='r', linestyle='-', label="Actual Pi")
        plt.xlabel("Repetition")
        plt.ylabel("Estimated Pi")
        plt.title("Monte Carlo Pi Calculation (Repetitions)")
        plt.legend()
        plt.grid(True, linestyle="--", alpha=0.6)
        plt.show(block=False)


    def plot_monte_carlo_results(self, pi_values, durations):
        """Plot the Monte Carlo Pi estimates and computation times for each repetition."""
        repetitions = range(1, len(pi_values) + 1)
        
        fig, axs = plt.subplots(1, 2, figsize=(12, 6))

        axs[0].plot(repetitions, pi_values, label="Estimated Pi", marker="o", color="blue")
        axs[0].axhline(y=3.14159, color='r', linestyle='--', label="Actual Pi")
        axs[0].set_xlabel("Repetition")
        axs[0].set_ylabel("Estimated Pi")
        axs[0].set_title("Monte Carlo Pi - Estimated Pi per Repetition")
        axs[0].grid(True, linestyle="--", alpha=0.6)
        axs[0].legend()

        axs[1].plot(repetitions, durations, label="Duration (ns)", marker="o", color="green")
        axs[1].set_xlabel("Repetition")
        axs[1].set_ylabel("Time (nanoseconds)")
        axs[1].set_title("Monte Carlo Pi - Duration per Repetition")
        axs[1].grid(True, linestyle="--", alpha=0.6)
        axs[1].legend()

        plt.tight_layout()
        plt.show(block=False)

    def plot_taylor_series_results(self, estimated_values, durations, x):
        """Plot the Taylor Series estimated e^x, computation durations, and actual e^x for each repetition."""
        repetitions = np.arange(1, len(estimated_values) + 1)

        plt.figure(figsize=(10, 6))
        plt.plot(repetitions, estimated_values, label="Estimated e^x", marker="o", color="blue")
        plt.axhline(y=np.exp(x), color='r', linestyle='--', label=f"Actual e^{x}")
        plt.plot(repetitions, durations, label="Duration (ns)", marker="o", color="green")

        plt.xlabel("Repetition")
        plt.ylabel("Value / Duration (nanoseconds)")
        plt.title(f"Taylor Series Exponential Test for e^{x}")
        plt.grid(True, linestyle="--", alpha=0.6)
        plt.legend()
        
        plt.show(block=False)


if __name__ == "__main__":
    root = tk.Tk()
    app = BenchmarkApp(root)
    root.mainloop()
