#include <iostream>
#include <vector>
#include <fstream>
#include "schedulers.h"

void append_csv(std::ofstream& file, const std::string& algo, const std::vector<Process>& processes, int switches = 0) {
    float total_wait = 0, total_tat = 0;
    for (const auto& p : processes) {
        total_wait += p.waiting_time;
        total_tat += p.turnaround_time;
    }
    file << algo << "," << total_wait / processes.size() << "," << total_tat / processes.size() << "," << switches << "\n";
}
int main() {
    std::cout << "\n=============================================\n";
    std::cout << "  Simulation Dashboard Demo\n";
    std::cout << "=============================================\n";
    
    std::vector<Process> visual_workload = generate_workload(5);
    auto workload_copy = visual_workload;

    simulate_fcfs(workload_copy, true);
    print_metrics("FCFS", workload_copy);

    workload_copy = visual_workload;
    simulate_sjf(workload_copy, true);
    print_metrics("SJF", workload_copy);

    workload_copy = visual_workload;
    simulate_priority(workload_copy, true);
    print_metrics("Priority", workload_copy);

    int switches = 0;
    workload_copy = visual_workload;
    simulate_rr(workload_copy, 4, switches, true);
    print_metrics("Round Robin (Q=4)", workload_copy);

    workload_copy = visual_workload;
    simulate_mlfq(workload_copy, true);
    print_metrics("MLFQ", workload_copy);

    std::cout << "\n=============================================\n";
    std::cout << "  Executing Multi-Load Scaling\n";
    std::cout << "=============================================\n";
    
    std::ofstream csv_file("simulation_results.csv");
    csv_file << "WorkloadSize,Algorithm,Average Waiting Time,Average Turnaround Time,Total Context Switches\n";

    std::vector<int> sizes = {100, 500, 1000, 1500, 2000};

    for (int size : sizes) {
        std::cout << "Simulating stress test for " << size << " concurrent processes...\n";
        std::vector<Process> stress_workload = generate_workload(size);

        workload_copy = stress_workload;
        simulate_fcfs(workload_copy, false);
        csv_file << size << ","; append_csv(csv_file, "FCFS", workload_copy);

        workload_copy = stress_workload;
        simulate_sjf(workload_copy, false);
        csv_file << size << ","; append_csv(csv_file, "SJF", workload_copy);

        workload_copy = stress_workload;
        simulate_priority(workload_copy, false);
        csv_file << size << ","; append_csv(csv_file, "Priority", workload_copy);

        // Test multiple Round Robin boundaries to generate full line datasets
        int sw = 0;
        workload_copy = stress_workload;
        simulate_rr(workload_copy, 2, sw, false);
        csv_file << size << ","; append_csv(csv_file, "Round Robin (Q=2)", workload_copy, sw);

        workload_copy = stress_workload;
        simulate_rr(workload_copy, 5, sw, false);
        csv_file << size << ","; append_csv(csv_file, "Round Robin (Q=5)", workload_copy, sw);

        workload_copy = stress_workload;
        simulate_rr(workload_copy, 10, sw, false);
        csv_file << size << ","; append_csv(csv_file, "Round Robin (Q=10)", workload_copy, sw);

        workload_copy = stress_workload;
        simulate_mlfq(workload_copy, false);
        csv_file << size << ","; append_csv(csv_file, "MLFQ", workload_copy);
    }

    csv_file.close();
    std::cout << "\nData extraction complete! Multi-load metrics exported.\n\n";

    return 0;
}
