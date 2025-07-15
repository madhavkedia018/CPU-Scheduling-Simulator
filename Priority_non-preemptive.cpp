#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid, arrival, burst, waiting, turnaround;
};

struct PriorityProcess : Process {
    int priority;
};

bool comparePriority(PriorityProcess a, PriorityProcess b) {
    if (a.arrival == b.arrival)
        return a.priority < b.priority;
    return a.arrival < b.arrival;
}

void priorityScheduling(vector<PriorityProcess>& processes) {
    int n = processes.size(), currentTime = 0, completed = 0;
    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1, highest = 1e9;
        for (int i = 0; i < n; ++i) {
            if (!done[i] && processes[i].arrival <= currentTime && processes[i].priority < highest) {
                highest = processes[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        processes[idx].waiting = currentTime - processes[idx].arrival;
        currentTime += processes[idx].burst;
        processes[idx].turnaround = processes[idx].waiting + processes[idx].burst;
        done[idx] = true;
        completed++;
    }
}
