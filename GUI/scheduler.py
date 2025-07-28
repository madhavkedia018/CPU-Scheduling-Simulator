# scheduler.py
from collections import deque

class Process:
    def __init__(self, pid, arrival, burst, priority=0):
        self.pid = pid
        self.arrival = arrival
        self.burst = burst
        self.remaining = burst
        self.priority = priority
        self.start_time = None
        self.completion = 0
        self.turnaround = 0
        self.waiting = 0

    def __repr__(self):
        return f"P{self.pid}(AT={self.arrival}, BT={self.burst})"

def fcfs(processes):
    processes.sort(key=lambda x: x.arrival)
    time = 0
    for p in processes:
        if time < p.arrival:
            time = p.arrival
        p.start_time = time
        time += p.burst
        p.completion = time
        p.turnaround = p.completion - p.arrival
        p.waiting = p.turnaround - p.burst
    return processes

def sjf_np(processes):
    processes = sorted(processes, key=lambda x: (x.arrival, x.burst))
    time = 0
    completed = []
    ready = []
    i = 0
    while len(completed) < len(processes):
        while i < len(processes) and processes[i].arrival <= time:
            ready.append(processes[i])
            i += 1
        if ready:
            ready.sort(key=lambda x: x.burst)
            p = ready.pop(0)
            if time < p.arrival:
                time = p.arrival
            p.start_time = time
            time += p.burst
            p.completion = time
            p.turnaround = p.completion - p.arrival
            p.waiting = p.turnaround - p.burst
            completed.append(p)
        else:
            time += 1
    return completed

def sjf_p(processes):
    n = len(processes)
    time = 0
    completed = 0
    ready = []
    last_proc = None
    res = []
    while completed < n:
        for p in processes:
            if p.arrival == time:
                ready.append(p)
        if ready:
            ready.sort(key=lambda x: (x.remaining, x.arrival))
            curr = ready[0]
            if curr.start_time is None:
                curr.start_time = time
            curr.remaining -= 1
            if curr.remaining == 0:
                curr.completion = time + 1
                curr.turnaround = curr.completion - curr.arrival
                curr.waiting = curr.turnaround - curr.burst
                ready.remove(curr)
                completed += 1
        time += 1
    return sorted(processes, key=lambda x: x.pid)

def round_robin(processes, quantum):
    queue = deque()
    time = 0
    i = 0
    completed = []
    processes.sort(key=lambda x: x.arrival)
    while i < len(processes) or queue:
        while i < len(processes) and processes[i].arrival <= time:
            queue.append(processes[i])
            i += 1
        if queue:
            p = queue.popleft()
            if p.start_time is None:
                p.start_time = time
            run_time = min(quantum, p.remaining)
            p.remaining -= run_time
            time += run_time
            while i < len(processes) and processes[i].arrival <= time:
                queue.append(processes[i])
                i += 1
            if p.remaining > 0:
                queue.append(p)
            else:
                p.completion = time
                p.turnaround = p.completion - p.arrival
                p.waiting = p.turnaround - p.burst
                completed.append(p)
        else:
            time += 1
    return sorted(processes, key=lambda x: x.pid)

def priority_np(processes):
    time = 0
    i = 0
    ready = []
    completed = []
    while len(completed) < len(processes):
        while i < len(processes) and processes[i].arrival <= time:
            ready.append(processes[i])
            i += 1
        if ready:
            ready.sort(key=lambda x: (x.priority, x.arrival))
            p = ready.pop(0)
            if time < p.arrival:
                time = p.arrival
            p.start_time = time
            time += p.burst
            p.completion = time
            p.turnaround = p.completion - p.arrival
            p.waiting = p.turnaround - p.burst
            completed.append(p)
        else:
            time += 1
    return completed

def priority_p(processes):
    time = 0
    completed = 0
    ready = []
    while completed < len(processes):
        for p in processes:
            if p.arrival == time:
                ready.append(p)
        if ready:
            ready.sort(key=lambda x: (x.priority, x.remaining, x.arrival))
            p = ready[0]
            if p.start_time is None:
                p.start_time = time
            p.remaining -= 1
            if p.remaining == 0:
                p.completion = time + 1
                p.turnaround = p.completion - p.arrival
                p.waiting = p.turnaround - p.burst
                ready.remove(p)
                completed += 1
        time += 1
    return sorted(processes, key=lambda x: x.pid)
