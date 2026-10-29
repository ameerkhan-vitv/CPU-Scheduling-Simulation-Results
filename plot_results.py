import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('simulation_results.csv')
df.columns = df.columns.str.strip().str.lower()

size_col = [c for c in df.columns if 'size' in c or 'load' in c][0]
algo_col = [c for c in df.columns if 'algo' in c][0]
wait_col = [c for c in df.columns if 'wait' in c][0]
tat_col = [c for c in df.columns if 'turnaround' in c or 'tat' in c][0]
switch_col = [c for c in df.columns if 'switch' in c][0]

# Graph 1: Scale Test
plt.figure(figsize=(9, 5))
for algo in df[algo_col].unique():
    algo_data = df[df[algo_col] == algo].sort_values(by=size_col)
    plt.plot(algo_data[size_col], algo_data[wait_col], marker='o', linewidth=1.5, label=algo)
plt.xlabel('Number of Processes')
plt.ylabel('Avg Waiting Time (ms)')
plt.title('Average Waiting Time vs Workload Size')
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig('scalability_benchmark.png', dpi=300)
plt.close()

# Filter for 1000 processes
df_1000 = df[df[size_col] == 1000]

if not df_1000.empty:
    # Graph 2: Bar Chart Comparison
    plt.figure(figsize=(9, 5))
    x = range(len(df_1000[algo_col]))
    width = 0.35
    plt.bar([i - width/2 for i in x], df_1000[wait_col], width, label='Waiting Time', color='#4a90e2')
    plt.bar([i + width/2 for i in x], df_1000[tat_col], width, label='Turnaround Time', color='#e056fd')
    plt.xlabel('Scheduling Algorithm')
    plt.ylabel('Time (ms)')
    plt.title('Performance Comparison for 1000 Processes')
    plt.xticks(x, df_1000[algo_col], rotation=10)
    plt.legend()
    plt.grid(axis='y', linestyle=':', alpha=0.6)
    plt.tight_layout()
    plt.savefig('performance_comparison.png', dpi=300)
    plt.close()

    # Graph 3: Round Robin Quanta Curve
    rr_data = df_1000[df_1000[algo_col].str.contains('round robin', case=False)].copy()
    if not rr_data.empty:
        rr_data['quantum'] = rr_data[algo_col].str.extract(r'=(\d+)').astype(int)
        rr_summary = rr_data.sort_values('quantum')
        
        plt.figure(figsize=(8, 4.5))
        plt.plot(rr_summary['quantum'], rr_summary[switch_col], marker='o', linewidth=1.5, color='#2c3e50', label='Context Switches')
        plt.xlabel('Time Quantum')
        plt.ylabel('Total Context Switches')
        plt.title('Round Robin Context Switches vs Quantum Size')
        plt.grid(True, linestyle=':', alpha=0.6)
        plt.legend()
        plt.tight_layout()
        plt.savefig('context_switching_overhead.png', dpi=300)
        plt.close()

print("Graphs Generated Successfully.")
