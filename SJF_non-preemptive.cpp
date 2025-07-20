#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid, arrival, burst;
    int completion = 0, turnaround = 0, waiting = 0, response = -1;
};

bool compareArrival(const Process& a, const Process& b) {
    return a.arrival < b.arrival;
}

void sjfScheduling(vector<Process>& processes, vector<pair<int, string>>& gantt) {
    int n = processes.size();
    sort(processes.begin(), processes.end(), compareArrival);

    vector<bool> done(n, false);
    int completed = 0, currentTime = 0;

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; ++i) {
            if (!done[i] && processes[i].arrival <= currentTime) {
                if (processes[i].burst < minBurst) {
                    minBurst = processes[i].burst;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            gantt.push_back({currentTime, "Idle"});
            currentTime++;
            continue;
        }

        if (processes[idx].response == -1)
            processes[idx].response = currentTime - processes[idx].arrival;

        gantt.push_back({currentTime, "P" + to_string(processes[idx].pid)});

        currentTime += processes[idx].burst;
        processes[idx].completion = currentTime;
        processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
        processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;

        done[idx] = true;
        completed++;
    }
}

void printResults(const vector<Process>& processes) {
    cout << "\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\tResponse\n";
    for (const auto& p : processes) {
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.completion << "\t\t" << p.turnaround << "\t\t"
             << p.waiting << "\t" << p.response << "\n";
    }
}

void printGanttChart(const vector<pair<int, string>>& gantt) {
    cout << "\nGantt Chart:\n";

    // Print top bar
    cout << " ";
    for (const auto& slot : gantt)
        cout << "-------";
    cout << "\n|";

    // Print process names
    for (const auto& slot : gantt)
        cout << "  " << setw(3) << slot.second << " |";

    // Print bottom bar
    cout << "\n ";
    for (const auto& slot : gantt)
        cout << "-------";
    cout << "\n";

    // Print time markers
    for (size_t i = 0; i < gantt.size(); ++i)
        cout << setw(4) << gantt[i].first << "   ";
    cout << setw(4) << gantt.back().first + 1 << "\n"; // Final time marker
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival and burst time for P" << (i + 1) << ": ";
        cin >> processes[i].arrival >> processes[i].burst;
    }

    vector<pair<int, string>> gantt;
    sjfScheduling(processes, gantt);

    printResults(processes);
    printGanttChart(gantt);

    return 0;
}
