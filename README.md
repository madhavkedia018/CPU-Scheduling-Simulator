#  CPU Scheduling Simulator

A **C++ console-based simulator** implementing key CPU scheduling algorithms found in operating systems. The simulator handles both **preemptive and non-preemptive scheduling**, models **realistic process execution**, and computes a full range of process performance metrics, including **visual Gantt chart rendering** and **context switching behavior**.

---

##  Algorithms Implemented

- **FCFS** – First Come First Serve  
- **SJF** – Shortest Job First  
  - Non-Preemptive  
  - Preemptive (Shortest Remaining Time First)  
- **Round Robin** – With configurable time quantum  
- **Priority Scheduling**  
  - Non-Preemptive  
  - Preemptive (with user-specified priority order: higher or lower number = higher priority)

---

##  Features

-  **Time-driven simulation** of CPU execution and idle periods  
-  **Context switch tracking** with count of switches  
-  **Real-time preemption** and process selection based on scheduling policy  
-  **Computation of all key performance metrics**:
  - Waiting Time  
  - Turnaround Time  
  - Completion Time  
  - **Response Time**  
-  **Starvation & fairness scenarios** to help analyze scheduling trade-offs  
-  **Gantt Chart Visualization** for intuitive process timeline analysis  
-  **User-friendly input prompts** for dynamic testing  
-  Easily extendable framework for adding new scheduling algorithms or variations





