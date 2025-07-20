#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid, arrival, burst;
    int start, completion, waiting, turnaround, response;
};

bool compareArrival(const Process &a, const Process &b) {
    return a.arrival < b.arrival;
}

void fcfsScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival);
    int currentTime = 0;

    for (auto& p : processes) {
        if (currentTime < p.arrival) {
            currentTime = p.arrival; // CPU idle until this process arrives
        }
        p.start = currentTime;
        p.response = p.start - p.arrival;
        p.waiting = p.response;
        p.completion = p.start + p.burst;
        p.turnaround = p.completion - p.arrival;
        currentTime = p.completion;
    }
}

void printResults(const vector<Process>& processes) {
    cout << "\nPID\tArrival\tBurst\tStart\tCompletion\tWaiting\tTurnaround\tResponse\n";
    for (const auto& p : processes) {
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" << p.start << "\t"
             << p.completion << "\t\t" << p.waiting << "\t" 
             << p.turnaround << "\t\t" << p.response << "\n";
    }
}

void printGanttChart(const vector<Process>& processes) {
    cout << "\nGantt Chart:\n";

    // Top border
    cout << " ";
    int prevEnd = 0;
    for (const auto& p : processes) {
        if (p.start > prevEnd) {
            for (int i = 0; i < p.start - prevEnd; ++i) cout << "--";
            cout << " ";
        }
        for (int i = 0; i < p.burst; ++i) cout << "--";
        cout << " ";
        prevEnd = p.completion;
    }

    // Middle with process/idle labels
    cout << "\n|";
    prevEnd = 0;
    for (const auto& p : processes) {
        if (p.start > prevEnd) {
            int idleLength = p.start - prevEnd;
            for (int i = 0; i < idleLength - 1; ++i) cout << " ";
            cout << "IDLE";
            for (int i = 0; i < idleLength - 1; ++i) cout << " ";
            cout << "|";
        }
        for (int i = 0; i < p.burst - 1; ++i) cout << " ";
        cout << "P" << p.pid;
        for (int i = 0; i < p.burst - 1; ++i) cout << " ";
        cout << "|";
        prevEnd = p.completion;
    }

    // Bottom border
    cout << "\n ";
    prevEnd = 0;
    for (const auto& p : processes) {
        if (p.start > prevEnd) {
            for (int i = 0; i < p.start - prevEnd; ++i) cout << "--";
            cout << " ";
        }
        for (int i = 0; i < p.burst; ++i) cout << "--";
        cout << " ";
        prevEnd = p.completion;
    }

    // Timeline
    cout << "\n";
    prevEnd = 0;
    cout << prevEnd;
    for (const auto& p : processes) {
        if (p.start > prevEnd) {
            cout << setw((p.start - prevEnd) * 2) << p.start;
        }
        cout << setw(p.burst * 2) << p.completion;
        prevEnd = p.completion;
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
        cout << "\nProcess " << i + 1 << ":\n";
        cout << "Arrival Time: ";
        cin >> processes[i].arrival;
        cout << "Burst Time: ";
        cin >> processes[i].burst;
    }

    fcfsScheduling(processes);
    printResults(processes);
    printGanttChart(processes);

    return 0;
}
