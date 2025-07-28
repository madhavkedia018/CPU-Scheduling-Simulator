# main_gui.py
import sys
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QPushButton,
    QTableWidget, QTableWidgetItem, QSpinBox, QHeaderView, QMessageBox, QLineEdit
)
from scheduler import *

class SchedulerGUI(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("CPU Scheduling Simulator")
        self.setGeometry(100, 100, 800, 500)
        self.process_count = 0
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()

        self.table = QTableWidget(0, 3)
        self.table.setHorizontalHeaderLabels(["Arrival Time", "Burst Time", "Priority"])
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)

        layout.addWidget(QLabel("Enter Process Details:"))
        layout.addWidget(self.table)

        btn_layout = QHBoxLayout()
        self.add_btn = QPushButton("Add Process")
        self.run_btn = QPushButton("Run Simulation")
        self.clear_btn = QPushButton("Clear All")
        self.exit_btn = QPushButton("Exit")

        btn_layout.addWidget(self.add_btn)
        btn_layout.addWidget(self.run_btn)
        btn_layout.addWidget(self.clear_btn)
        btn_layout.addWidget(self.exit_btn)

        layout.addLayout(btn_layout)

        q_layout = QHBoxLayout()
        q_layout.addWidget(QLabel("Time Quantum (for RR):"))
        self.quantum_input = QLineEdit()
        self.quantum_input.setText("2")
        q_layout.addWidget(self.quantum_input)
        layout.addLayout(q_layout)

        self.result_table = QTableWidget(0, 3)
        self.result_table.setHorizontalHeaderLabels(["Algorithm", "Avg Waiting Time", "Avg Turnaround Time"])
        self.result_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        layout.addWidget(QLabel("Results:"))
        layout.addWidget(self.result_table)

        self.best_algo_label = QLabel("Best Algorithm: ")
        layout.addWidget(self.best_algo_label)

        self.setLayout(layout)

        self.add_btn.clicked.connect(self.add_process)
        self.run_btn.clicked.connect(self.run_simulation)
        self.clear_btn.clicked.connect(self.clear_all)
        self.exit_btn.clicked.connect(self.close)

    def add_process(self):
        self.table.insertRow(self.table.rowCount())
        self.table.setItem(self.table.rowCount()-1, 0, QTableWidgetItem("0"))
        self.table.setItem(self.table.rowCount()-1, 1, QTableWidgetItem("0"))
        self.table.setItem(self.table.rowCount()-1, 2, QTableWidgetItem("0"))

    def clear_all(self):
        self.table.setRowCount(0)
        self.result_table.setRowCount(0)
        self.best_algo_label.setText("Best Algorithm: ")

    def run_simulation(self):
        row_count = self.table.rowCount()
        if row_count == 0:
            QMessageBox.warning(self, "Error", "No processes added!")
            return

        try:
            quantum = int(self.quantum_input.text())
            if quantum <= 0:
                raise ValueError
        except ValueError:
            QMessageBox.warning(self, "Error", "Time quantum must be a positive integer!")
            return

        processes = []
        for i in range(row_count):
            arrival = int(self.table.item(i, 0).text())
            burst = int(self.table.item(i, 1).text())
            priority = int(self.table.item(i, 2).text())
            processes.append(Process(i+1, arrival, burst, priority))

        algorithms = [
            ("FCFS", fcfs),
            ("SJF (NP)", sjf_np),
            ("SJF (P)", sjf_p),
            ("RR", lambda ps: round_robin(ps, quantum)),
            ("Priority (NP)", priority_np),
            ("Priority (P)", priority_p)
        ]

        results = []
        for name, algo in algorithms:
            copied = [Process(p.pid, p.arrival, p.burst, p.priority) for p in processes]
            completed = algo(copied)
            avg_wait = sum(p.waiting for p in completed) / len(completed)
            avg_turn = sum(p.turnaround for p in completed) / len(completed)
            results.append((name, avg_wait, avg_turn))

        self.result_table.setRowCount(0)
        for r in results:
            row = self.result_table.rowCount()
            self.result_table.insertRow(row)
            self.result_table.setItem(row, 0, QTableWidgetItem(r[0]))
            self.result_table.setItem(row, 1, QTableWidgetItem(f"{r[1]:.2f}"))
            self.result_table.setItem(row, 2, QTableWidgetItem(f"{r[2]:.2f}"))

        best = min(results, key=lambda x: x[1])  # Based on avg waiting time
        self.best_algo_label.setText(f"Best Algorithm: {best[0]} (Avg WT: {best[1]:.2f})")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = SchedulerGUI()
    window.show()
    sys.exit(app.exec_())
