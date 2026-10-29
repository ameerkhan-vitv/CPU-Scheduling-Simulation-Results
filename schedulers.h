#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include <vector>
#include <string>
#include "process.h"

std::vector<Process> generate_workload(int count);
void draw_timeline(const std::string& algo_name, const std::vector<std::pair<int, std::string>>& timeline);
void print_metrics(const std::string& algo_name, const std::vector<Process>& processes);

void simulate_fcfs(std::vector<Process>& processes, bool visual);
void simulate_sjf(std::vector<Process>& processes, bool visual);
void simulate_priority(std::vector<Process>& processes, bool visual);
void simulate_rr(std::vector<Process>& processes, int quantum, int& switches, bool visual);
void simulate_mlfq(std::vector<Process>& processes, bool visual);

#endif
