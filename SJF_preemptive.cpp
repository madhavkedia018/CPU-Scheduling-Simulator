#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Process {
    int pid, arrival, burst, remaining, waiting, turnaround, completion;
};

void preemptiveSJF(vector<Process>& processes) {
    int n = processes.size();
    int completed = 0, currentTime = 0;
    int prev = -1;

    for (auto& p : processes)
        p.remaining = p.burst;

    while (completed < n) {
        int idx = -1;
        int minRem = INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                if (processes[i].remaining < minRem) {
                    minRem = processes[i].remaining;
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

void printResults(const vector<Process>& processes) {
    cout << "PID\tArrival\tBurst\tWaiting\tTurnaround\n";
    for (const auto& p : processes)
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" << p.waiting << "\t" << p.turnaround << endl;
}
