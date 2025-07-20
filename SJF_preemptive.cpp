#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
using namespace std;

struct Process {
    int pid, arrival, burst;
    int remaining, waiting = 0, turnaround = 0, completion = 0, response = -1;
};

// Gantt chart representation
struct GanttBlock {
    int start, end;
    int pid; // -1 for IDLE
};

void preemptiveSJF(vector<Process>& processes, int& contextSwitches, vector<GanttBlock>& gantt) {
    int n = processes.size();
    int completed = 0, currentTime = 0, prevPid = -2;

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
            if (processes[idx].response == -1)
                processes[idx].response = currentTime - processes[idx].arrival;

            if (prevPid != processes[idx].pid) {
                contextSwitches++;
                gantt.push_back({currentTime, currentTime + 1, processes[idx].pid});
            } else {
                gantt.back().end++;  // Extend current block
            }

            processes[idx].remaining--;
            currentTime++;
            if (processes[idx].remaining == 0) {
                completed++;
                processes[idx].completion = currentTime;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            }

            prevPid = processes[idx].pid;
        } else {
            // CPU is idle
            if (prevPid != -1) {
                contextSwitches++;
                gantt.push_back({currentTime, currentTime + 1, -1});
            } else {
                gantt.back().end++;
            }
            currentTime++;
            prevPid = -1;
        }
    }

    // Subtract the first context switch which is not a real switch
    if (contextSwitches > 0) contextSwitches--;
}

void printResults(const vector<Process>& processes) {
    cout << "\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\tResponse\n";
    for (const auto& p : processes)
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" 
             << p.completion << "\t\t" << p.turnaround << "\t\t"
             << p.waiting << "\t" << p.response << "\n";
}

void printGanttChart(const vector<GanttBlock>& gantt) {
    cout << "\nGantt Chart:\n";

    // First line: top border
    cout << " ";
    for (const auto& block : gantt) {
        int len = block.end - block.start;
        for (int i = 0; i < len; ++i)
            cout << "--";
        cout << " ";
    }

    // Second line: process labels
    cout << "\n|";
    for (const auto& block : gantt) {
        int len = block.end - block.start;
        string label = (block.pid == -1) ? "IDLE" : "P" + to_string(block.pid);
        int padding = len * 2 - label.size();
        int left = padding / 2;
        int right = padding - left;
        cout << string(left, ' ') << label << string(right, ' ') << "|";
    }

    // Third line: bottom border
    cout << "\n ";
    for (const auto& block : gantt) {
        int len = block.end - block.start;
        for (int i = 0; i < len; ++i)
            cout << "--";
        cout << " ";
    }

    // Fourth line: timestamps
    cout << "\n";
    cout << gantt[0].start;
    for (const auto& block : gantt) {
        int len = block.end - block.start;
        cout << setw(len * 2) << block.end;
    }
    cout << "\n";
}


int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time and burst time for P" << i + 1 << ": ";
        cin >> processes[i].arrival >> processes[i].burst;
    }

    int contextSwitches = 0;
    vector<GanttBlock> gantt;
    preemptiveSJF(processes, contextSwitches, gantt);

    printResults(processes);
    printGanttChart(gantt);
    cout << "\nTotal context switches: " << contextSwitches << "\n";

    return 0;
}
