# CPU-Scheduling-Simulation-Results
# Multi-Algorithm CPU Scheduling Simulator

A native, modular C++ discrete-event simulation engine designed to benchmark core operating system scheduling architectures under heavy, variable traffic workloads. This simulator evaluates processing efficiency, queue latencies, and system preemption overhead across five distinct paradigms, providing full dataset extraction for performance analysis.

## Key Features

* **Modular C++ Architecture:** Decoupled layout utilizing header and implementation file separation for a clean, maintainable software design.
* **Five Implemented Schedulers:** Evaluates First-Come First-Served (FCFS), Non-Preemptive Shortest Job First (SJF), Non-Preemptive Priority, Preemptive Round Robin (RR), and an adaptive multi-tier Multi-Level Feedback Queue (MLFQ).
* **Interactive Terminal Visualizer:** Renders a real-time ASCII execution timeline chart in the console to monitor clock ticks and thread allocations dynamically.
* **Large-Scale Metric Sweeps:** Simulates massive stress tests scaling from 100 up to 2,000 randomized concurrent processes.
* **Automated Data Plotting:** Automatically exports performance data straight to a `.csv` file, with a built-in Python script to plot research-grade scalability and overhead charts.

## File Structure

```text
├── process.h                # Process Control Block (PCB) attributes struct
├── schedulers.h             # Function declarations and scheduler interfaces
├── schedulers.cpp           # Core algorithm implementation logic loops
├── main.cpp                 # Main control routine, interactive demo, and scaling sweep
├── plot_results.py          # Python utility script for data plotting and graph generation
└── simulation_results.csv   # Automatically exported benchmark metric dataset
```

## Setup & Execution on Ubuntu

### 1. Prerequisites
Ensure you have the essential C++ compiler build tools and Python plotting libraries installed:

```bash
sudo apt update
sudo apt install build-essential python3-pandas python3-matplotlib -y
```

### 2. Compile and Run the Simulator
Compile the multi-file project with `g++` and execute the executable binary to trigger the interactive dashboard and stress sweep:

```bash
g++ main.cpp schedulers.cpp -o simulator
./simulator
```

### 3. Generate Analysis Graphs
Once the C++ program outputs `simulation_results.csv`, run the plotter utility to update your high-resolution PNG charts:

```bash
python3 plot_results.py
```

## Generated Visualizations

Running the plotting script generates three comparative benchmark files directly inside your directory:
1. `scalability_benchmark.png` — Plots average waiting latencies as the system scales up to 2,000 tasks.
2. `performance_comparison.png` — Provides a bar chart comparison of execution times for a fixed load of 1,000 processes.
3. `context_switching_overhead.png` — Maps out the system preemption curve showing the friction cost of a low Round Robin quantum.
