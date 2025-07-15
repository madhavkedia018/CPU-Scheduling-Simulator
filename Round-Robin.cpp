void roundRobin(vector<Process>& processes, int quantum) {
    int n = processes.size();
    vector<int> remaining(n), waiting(n, 0), turnaround(n);
    for (int i = 0; i < n; ++i)
        remaining[i] = processes[i].burst;

    int currentTime = 0;
    bool done;

    do {
        done = true;
        for (int i = 0; i < n; ++i) {
            if (remaining[i] > 0 && processes[i].arrival <= currentTime) {
                done = false;
                int timeSpent = min(quantum, remaining[i]);
                remaining[i] -= timeSpent;
                currentTime += timeSpent;

                if (remaining[i] == 0)
                    turnaround[i] = currentTime - processes[i].arrival;
            } else if (remaining[i] > 0) {
                currentTime++;
            }
        }
    } while (!done);

    for (int i = 0; i < n; ++i) {
        processes[i].turnaround = turnaround[i];
        processes[i].waiting = turnaround[i] - processes[i].burst;
    }
}
