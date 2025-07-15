void sjfScheduling(vector<Process>& processes) {
    int n = processes.size();
    vector<bool> done(n, false);
    int completed = 0, currentTime = 0;

    while (completed < n) {
        int idx = -1, minBurst = 1e9;
        for (int i = 0; i < n; ++i) {
            if (!done[i] && processes[i].arrival <= currentTime && processes[i].burst < minBurst) {
                minBurst = processes[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        processes[idx].waiting = currentTime - processes[idx].arrival;
        currentTime += processes[idx].burst;
        processes[idx].turnaround = processes[idx].waiting + processes[idx].burst;
        done[idx] = true;
        completed++;
    }
}
