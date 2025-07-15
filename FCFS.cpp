#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid, arrival, burst, waiting, turnaround;
};

bool compareArrival(Process a, Process b) {
    return a.arrival < b.arrival;
}

void fcfsScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival);
    int currentTime = 0;

    for (auto& p : processes) {
        if (currentTime < p.arrival)
            currentTime = p.arrival;

        p.waiting = currentTime - p.arrival;
        currentTime += p.burst;
        p.turnaround = p.waiting + p.burst;
    }
}

void printResults(const vector<Process>& processes) {
    cout << "PID\tArrival\tBurst\tWaiting\tTurnaround\n";
    for (const auto& p : processes)
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" << p.waiting << "\t" << p.turnaround << endl;
}
