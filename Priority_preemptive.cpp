#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include<bits/stdc++.h>
using namespace std;

struct PriorityProcess {
    int pid, arrival, burst, remaining, priority;
    int waiting = 0, turnaround = 0, completion = 0, response = -1;
};

void preemptivePriority(vector<PriorityProcess>& processes, int& contextSwitches, vector<int>& gantt, bool higherNumberHigherPriority) {
    int n = processes.size();
    int currentTime = 0, completed = 0, prev = -1;
    contextSwitches = 0;

    for (auto& p : processes)
        p.remaining = p.burst;

    while (completed < n) {
        int idx = -1;
        int bestPriority = higherNumberHigherPriority ? INT_MIN : INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                bool isBetter = higherNumberHigherPriority ? 
                    (processes[i].priority > bestPriority) : 
                    (processes[i].priority < bestPriority);
                if (isBetter) {
                    bestPriority = processes[i].priority;
                    idx = i;
                } else if (processes[i].priority == bestPriority && idx != -1) {
                    // Tie breaker: lower PID or earlier arrival
                    if (processes[i].arrival < processes[idx].arrival)
                        idx = i;
                }
            }
        }

        if (idx != -1) {
            if (prev != idx) {
                if (prev != -1) contextSwitches++;
                gantt.push_back(processes[idx].pid);
                prev = idx;
            }

            if (processes[idx].response == -1)
                processes[idx].response = currentTime - processes[idx].arrival;

            processes[idx].remaining--;

            if (processes[idx].remaining == 0) {
                completed++;
                processes[idx].completion = currentTime + 1;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            }
        } else {
            // CPU Idle
            if (prev != -1) contextSwitches++;
            prev = -1;
            gantt.push_back(-1); // -1 for idle
        }

        currentTime++;
    }
}

void printResults(const vector<PriorityProcess>& processes) {
    cout << "\nPID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\tResponse\tCompletion\n";
    for (const auto& p : processes)
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" << p.priority << "\t\t"
             << p.waiting << "\t" << p.turnaround << "\t\t" << p.response << "\t\t" << p.completion << "\n";
}

void printGanttChart(const vector<int>& gantt) {
    cout << "\nGantt Chart:\n";

    // Top bar
    for (int i = 0; i < gantt.size(); ++i)
        cout << "--";
    cout << "-\n|";

    // Process IDs
    for (int i = 0; i < gantt.size(); ++i) {
        if (gantt[i] == -1)
            cout << "  |";
        else
            cout << "P" << gantt[i] << "|";
    }

    // Bottom bar
    cout << "\n";
    for (int i = 0; i < gantt.size(); ++i)
        cout << "--";
    cout << "-\n";

    // Timeline
    cout << "0";
    for (int i = 1; i <= gantt.size(); ++i)
        cout << setw(3) << i;
    cout << "\n";
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<PriorityProcess> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter Arrival Time, Burst Time, and Priority for Process P" << i + 1 << ": ";
        cin >> processes[i].arrival >> processes[i].burst >> processes[i].priority;
    }

    cout << "Should higher number indicate higher priority? (1 for Yes, 0 for No): ";
    bool higherPriorityWins;
    cin >> higherPriorityWins;

    int contextSwitches = 0;
    vector<int> gantt;

    preemptivePriority(processes, contextSwitches, gantt, higherPriorityWins);
    printResults(processes);
    printGanttChart(gantt);
    cout << "\nTotal Context Switches: " << contextSwitches << "\n";

    return 0;
}
