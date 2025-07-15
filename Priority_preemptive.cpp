#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct PriorityProcess {
    int pid, arrival, burst, remaining, priority, waiting, turnaround, completion;
};

void preemptivePriority(vector<PriorityProcess>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;

    for (auto& p : processes)
        p.remaining = p.burst;

    while (completed < n) {
        int idx = -1;
        int highestPriority = INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            processes[idx].remaining--;
            if (processes[idx].remaining == 0) {
                completed++;
                processes[idx].completion = currentTime + 1;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            }
        }
        currentTime++;
    }
}

void printPriorityResults(const vector<PriorityProcess>& processes) {
    cout << "PID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n";
    for (const auto& p : processes)
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" << p.priority << "\t\t"
             << p.waiting << "\t" << p.turnaround << endl;
}
