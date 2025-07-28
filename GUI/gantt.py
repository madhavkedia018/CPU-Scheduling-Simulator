# gantt.py
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

def draw_gantt(processes, title):
    processes = sorted(processes, key=lambda p: p.start_time or 0)
    fig, gnt = plt.subplots()
    gnt.set_title(f"Gantt Chart - {title}")
    gnt.set_xlabel("Time")
    gnt.set_ylabel("Processes")
    gnt.set_yticks([15 + 10 * i for i in range(len(processes))])
    gnt.set_yticklabels([f"P{p.pid}" for p in processes])
    gnt.set_ylim(0, 10 + 10 * len(processes))
    gnt.grid(True)

    colors = plt.cm.tab20.colors
    for i, p in enumerate(processes):
        start = p.start_time if p.start_time is not None else 0
        gnt.broken_barh([(start, p.burst)], (10 + i * 10, 9), facecolors=colors[i % len(colors)])
        gnt.text(start + p.burst / 2, 10 + i * 10 + 4.5, f"P{p.pid}", 
                 ha='center', va='center', color='white', fontsize=8, weight='bold')

    plt.tight_layout()
    plt.show()
