#ifndef PROCESS_H
#define PROCESS_H

#include <string>

struct Process {
    std::string pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int completion_time = 0;
    int turnaround_time = 0;
    int waiting_time = 0;
    int response_time = -1;
};

#endif
