#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include<bits/stdc++.h>
using namespace std;

struct Process {
    int pid, arrival, burst, waiting, turnaround, completion, response, start;
};

struct PriorityProcess : Process {
    int priority;
};

bool compareArrival(PriorityProcess a, PriorityProcess b) {
    return a.arrival < b.arrival;
}

void priorityScheduling(vector<PriorityProcess>& processes, vector<pair<int, int>>& ganttChart) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1, highestPriority = INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (!done[i] && processes[i].arrival <= currentTime &&
                processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        processes[idx].start = currentTime;
        processes[idx].response = currentTime - processes[idx].arrival;
        processes[idx].waiting = processes[idx].response;
        currentTime += processes[idx].burst;
        processes[idx].completion = currentTime;
        processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;

        ganttChart.push_back({processes[idx].start, processes[idx].pid});
        done[idx] = true;
        completed++;
    }
}

void printTable(const vector<PriorityProcess>& processes) {
    cout << "\nProcess Table:\n";
    cout << "PID\tArrival\tBurst\tPriority\tCT\tTAT\tWT\tRT\n";
    for (const auto& p : processes) {
        cout << "P" << p.pid << "\t"
             << p.arrival << "\t"
             << p.burst << "\t"
             << p.priority << "\t\t"
             << p.completion << "\t"
             << p.turnaround << "\t"
             << p.waiting << "\t"
             << p.response << "\n";
    }
}

void printGanttChart(const vector<PriorityProcess>& processes, const vector<pair<int, int>>& ganttChart) {
    cout << "\nGantt Chart:\n";

    // Top border
    cout << " ";
    for (auto [start, pid] : ganttChart) {
        int len = processes[pid - 1].burst;
        for (int i = 0; i < len; ++i) cout << "--";
        cout << " ";
    }
    cout << "\n|";

    // Process IDs
    for (auto [start, pid] : ganttChart) {
        int len = processes[pid - 1].burst;
        cout << "P" << pid;
        for (int i = 0; i < 2 * len - 2; ++i) cout << " ";
        cout << "|";
    }

    // Bottom border
    cout << "\n ";
    for (auto [start, pid] : ganttChart) {
        int len = processes[pid - 1].burst;
        for (int i = 0; i < len * 2; ++i) cout << "-";
        cout << " ";
    }

    // Time markers
    cout << "\n" << ganttChart[0].first;
    for (auto [start, pid] : ganttChart) {
        int end = start + processes[pid - 1].burst;
        cout << setw(2 * processes[pid - 1].burst) << end;
    }
    cout << "\n";
}


int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<PriorityProcess> processes(n);
    vector<pair<int, int>> ganttChart;

    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time, burst time, and priority for P" << i + 1 << ": ";
        cin >> processes[i].arrival >> processes[i].burst >> processes[i].priority;
    }


    priorityScheduling(processes, ganttChart);
    
    printTable(processes);
    printGanttChart(processes, ganttChart);

    return 0;
}
