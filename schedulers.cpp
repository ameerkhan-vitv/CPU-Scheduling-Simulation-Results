#include "schedulers.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <queue>
#include <iomanip>

std::vector<Process> generate_workload(int count) {
    std::vector<Process> processes;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> arrival_dist(0, 20); 
    std::uniform_int_distribution<> burst_dist(2, 12);   
    std::uniform_int_distribution<> priority_dist(1, 4); 

    for (int i = 0; i < count; ++i) {
        Process p;
        p.pid = "P" + std::to_string(i + 1);
        p.arrival_time = arrival_dist(gen);
        p.burst_time = burst_dist(gen);
        p.remaining_time = p.burst_time;
        p.priority = priority_dist(gen);
        processes.push_back(p);
    }
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });
    return processes;
}

void draw_timeline(const std::string& algo_name, const std::vector<std::pair<int, std::string>>& timeline) {
    std::cout << "\n=======================================================\n";
    std::cout << "  CPU Execution Timeline: " << algo_name << "\n";
    std::cout << "=======================================================\n\n";
    std::cout << "Time: ";
    for (const auto& slot : timeline) std::cout << std::setw(3) << slot.first << " ";
    std::cout << "\nCPU : ";
    for (const auto& slot : timeline) std::cout << " [" << std::setw(2) << slot.second << "]";
    std::cout << "\n\n";
}

void print_metrics(const std::string& algo_name, const std::vector<Process>& processes) {
    std::cout << "--- " << algo_name << " Performance Metrics ---\n";
    std::cout << "PID\tArrival\tBurst\tComplete\tTurnAround\tWaiting\n";
    float total_wait = 0, total_tat = 0;
    for (const auto& p : processes) {
        total_wait += p.waiting_time;
        total_tat += p.turnaround_time;
        std::cout << p.pid << "\t" << p.arrival_time << "\t" 
                  << p.burst_time << "\t" << p.completion_time << "\t\t" 
                  << p.turnaround_time << "\t\t" << p.waiting_time << "\n";
    }
    std::cout << "Average Waiting Time: " << total_wait / processes.size() << " ms\n";
    std::cout << "Average Turnaround Time: " << total_tat / processes.size() << " ms\n\n";
}

void simulate_fcfs(std::vector<Process>& processes, bool visual) {
    int current_time = 0;
    std::vector<std::pair<int, std::string>> timeline;
    for (auto& p : processes) {
        while (current_time < p.arrival_time) {
            if (visual) timeline.push_back({current_time, "--"});
            current_time++;
        }
        p.response_time = current_time - p.arrival_time;
        for (int i = 0; i < p.burst_time; ++i) {
            if (visual) timeline.push_back({current_time, p.pid});
            current_time++;
        }
        p.completion_time = current_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
    if (visual) draw_timeline("FCFS", timeline);
}

void simulate_sjf(std::vector<Process>& processes, bool visual) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    std::vector<bool> is_completed(n, false);
    std::vector<std::pair<int, std::string>> timeline;

    while (completed < n) {
        int idx = -1;
        int min_burst = 1e9;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival_time <= current_time && !is_completed[i]) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            processes[idx].response_time = current_time - processes[idx].arrival_time;
            for (int i = 0; i < processes[idx].burst_time; ++i) {
                if (visual) timeline.push_back({current_time, processes[idx].pid});
                current_time++;
            }
            processes[idx].completion_time = current_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            is_completed[idx] = true;
            completed++;
        } else {
            if (visual) timeline.push_back({current_time, "--"});
            current_time++;
        }
    }
    if (visual) draw_timeline("SJF", timeline);
}
void simulate_priority(std::vector<Process>& processes, bool visual) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    std::vector<bool> is_completed(n, false);
    std::vector<std::pair<int, std::string>> timeline;

    while (completed < n) {
        int idx = -1;
        int highest_priority = 1e9;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival_time <= current_time && !is_completed[i]) {
                if (processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            processes[idx].response_time = current_time - processes[idx].arrival_time;
            for (int i = 0; i < processes[idx].burst_time; ++i) {
                if (visual) timeline.push_back({current_time, processes[idx].pid});
                current_time++;
            }
            processes[idx].completion_time = current_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            is_completed[idx] = true;
            completed++;
        } else {
            if (visual) timeline.push_back({current_time, "--"});
            current_time++;
        }
    }
    if (visual) draw_timeline("Priority Scheduling", timeline);
}

void simulate_rr(std::vector<Process>& processes, int quantum, int& switches, bool visual) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    std::queue<int> q;
    std::vector<bool> in_queue(n, false);
    std::vector<std::pair<int, std::string>> timeline;
    switches = 0;
    int last_idx = -1;

    auto update_arrivals = [&]() {
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && !in_queue[i]) {
                q.push(i);
                in_queue[i] = true;
            }
        }
    };

    update_arrivals();

    while (completed < n) {
        if (q.empty()) {
            if (visual) timeline.push_back({current_time, "--"});
            current_time++;
            update_arrivals();
            continue;
        }

        int idx = q.front();
        q.pop();
        in_queue[idx] = false;

        if (last_idx != -1 && last_idx != idx) switches++;
        last_idx = idx;

        if (processes[idx].response_time == -1) {
            processes[idx].response_time = current_time - processes[idx].arrival_time;
        }

        int run = std::min(quantum, processes[idx].remaining_time);
        for (int i = 0; i < run; ++i) {
            if (visual) timeline.push_back({current_time, processes[idx].pid});
            current_time++;
            processes[idx].remaining_time--;
            update_arrivals();
        }

        if (processes[idx].remaining_time > 0) {
            q.push(idx);
            in_queue[idx] = true;
        } else {
            processes[idx].completion_time = current_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            completed++;
        }
    }
    if (visual) draw_timeline("Round Robin (Q=" + std::to_string(quantum) + ")", timeline);
}

void simulate_mlfq(std::vector<Process>& processes, bool visual) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    std::queue<int> q1; 
    std::queue<int> q2; 
    int q1_quantum = 3;
    std::vector<bool> visited(n, false);
    std::vector<std::pair<int, std::string>> timeline;

    auto check_arrivals = [&]() {
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival_time <= current_time && !visited[i]) {
                q1.push(i);
                visited[i] = true;
            }
        }
    };

    check_arrivals();

    while (completed < n) {
        if (q1.empty() && q2.empty()) {
            if (visual) timeline.push_back({current_time, "--"});
            current_time++;
            check_arrivals();
            continue;
        }

        if (!q1.empty()) {
            int idx = q1.front();
            q1.pop();

            if (processes[idx].response_time == -1) {
                processes[idx].response_time = current_time - processes[idx].arrival_time;
            }

            int run = std::min(q1_quantum, processes[idx].remaining_time);
            for (int t = 0; t < run; ++t) {
                if (visual) timeline.push_back({current_time, processes[idx].pid});
                current_time++;
                processes[idx].remaining_time--;
                check_arrivals();
            }

            if (processes[idx].remaining_time > 0) {
                q2.push(idx); 
            } else {
                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                completed++;
            }
        } 
        else if (!q2.empty()) {
            int idx = q2.front();
            q2.pop();

            if (processes[idx].response_time == -1) {
                processes[idx].response_time = current_time - processes[idx].arrival_time;
            }

            if (visual) timeline.push_back({current_time, processes[idx].pid});
            current_time++;
            processes[idx].remaining_time--;
            check_arrivals();

            if (processes[idx].remaining_time > 0) {
                q2.push(idx);
            } else {
                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                completed++;
            }
        }
    }
    if (visual) draw_timeline("Multi-Level Feedback Queue (MLFQ)", timeline);
}
