#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid, arrival, burst, remaining;
    int completion = 0, waiting = 0, turnaround = 0, response = -1;
};

struct GanttBlock {
    string label;
    int startTime;
    int endTime;
};

bool arrivalCompare(Process a, Process b) {
    return a.arrival < b.arrival;
}

int roundRobin(vector<Process>& processes, int quantum, vector<GanttBlock>& ganttChart) {
    sort(processes.begin(), processes.end(), arrivalCompare);
    int n = processes.size();
    int currentTime = 0;
    int contextSwitches = 0;
    queue<int> readyQueue;
    vector<bool> inQueue(n, false);
    int completed = 0;
    int prevProcess = -1;

    while (completed < n) {
        // Enqueue new arrivals
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0 && !inQueue[i]) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (readyQueue.empty()) {
            ganttChart.push_back({"IDLE", currentTime, currentTime + 1});
            currentTime++;
            prevProcess = -1;
            continue;
        }

        int i = readyQueue.front();
        readyQueue.pop();

        if (prevProcess != -1 && prevProcess != i)
            contextSwitches++;

        if (processes[i].response == -1)
            processes[i].response = currentTime - processes[i].arrival;

        int timeSlice = min(quantum, processes[i].remaining);
        ganttChart.push_back({"P" + to_string(processes[i].pid), currentTime, currentTime + timeSlice});
        processes[i].remaining -= timeSlice;
        currentTime += timeSlice;

        // Enqueue arrivals during execution
        for (int j = 0; j < n; ++j) {
            if (processes[j].arrival <= currentTime && processes[j].remaining > 0 && !inQueue[j]) {
                readyQueue.push(j);
                inQueue[j] = true;
            }
        }

        if (processes[i].remaining > 0) {
            readyQueue.push(i);  // requeue
        } else {
            processes[i].completion = currentTime;
            processes[i].turnaround = processes[i].completion - processes[i].arrival;
            processes[i].waiting = processes[i].turnaround - processes[i].burst;
            completed++;
        }

        prevProcess = i;
    }

    return contextSwitches;
}

void printGanttChart(const vector<GanttBlock>& chart) {
    cout << "\nGantt Chart:\n";
    for (const auto& block : chart) {
        cout << "| " << setw(block.endTime - block.startTime) << block.label << " ";
    }
    cout << "|\n";

    int current = 0;
    for (const auto& block : chart) {
        cout << current << setw(block.endTime - block.startTime + 3);
        current = block.endTime;
    }
    cout << current << "\n";
}

void printResults(const vector<Process>& processes) {
    cout << "\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\tResponse\n";
    for (const auto& p : processes) {
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.completion << "\t\t" << p.turnaround << "\t\t"
             << p.waiting << "\t" << p.response << "\n";
    }
    
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time and burst time for P" << (i + 1) << ": ";
        cin >> processes[i].arrival >> processes[i].burst;
        processes[i].remaining = processes[i].burst;
    }

    int quantum;
    cout << "Enter time quantum: ";
    cin >> quantum;

    vector<GanttBlock> ganttChart;
    int contextSwitches = roundRobin(processes, quantum, ganttChart);
    printResults(processes);
    printGanttChart(ganttChart);
    cout << "\nTotal Context Switches: " << contextSwitches << "\n";
    

    return 0;
}
