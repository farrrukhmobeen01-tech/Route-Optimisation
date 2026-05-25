# ✦ BuroBrain Logistics Network (TSP) ✦

> **An enterprise-grade, neo-cyberpunk Route Optimization System engineered in C++ to tackle the Travelling Salesperson Problem (TSP) with real-time visual tracing.**

The **BuroBrain Logistics Network** is a high-performance desktop application that merges complex backend data structures with a cinematic, highly interactive Qt graphics engine. It allows users to benchmark algorithmic routing efficiency, analyze memory tradeoffs, and simulate real-world logistics scenarios through a premium tactical dashboard.

---

## 🚀 Demo

Https://www.linkedin.com/posts/mfarrukhmubeen_cpp-qtframework-datastructures-ugcPost-7464413832468201472-xL0O/?utm_source=share&utm_medium=member_android&rcm=ACoAAFf4T8UB7mLQKywHzHc0IRGt2wrWbwnAmCM

---

## ✨ Key Features

### 🧠 Advanced Optimization Engines

Real-time execution and benchmarking of multiple pathfinding algorithms:

* **Brute Force (Exhaustive):** Computes the exact shortest path.
* **Greedy Heuristic:** Nearest-neighbor approximation for rapid, scalable routing.
* **Custom Stochastic Search:** Randomized heuristic exploration for complex node clusters.

### 💾 Dual Memory Architecture

Dynamically implements and compares two fundamental graph storage methods:

* **Static Adjacency Matrix [ O(V²) ]:** Optimized for dense topologies and O(1) edge lookups.
* **Dynamic Adjacency List [ O(V+E) ]:** Pointer-based architecture optimized for sparse networks and strict memory constraints.

### 🎨 Neo-Cyberpunk Graphics Engine

Custom-built using the `QGraphicsScene` API:

* **Tactical Radar Grid:** Deep space-blue UI with glowing geometric nodes.
* **Live Path Tracing:** Animated, neon laser-tracing to visualize the algorithm's decision-making process.
* **Glassmorphism Panels:** Translucent control decks with dynamic computational metric outputs.

### 🛰️ Real-World Field Scenarios

Directly applies the Travelling Salesperson Problem to modern logistics:

1. **[TRK] Ground Fleet Delivery:** Standard closed-loop TSP optimization.
2. **[SAT] Orbital GPS Navigation:** Point-to-point direct distance targeting.
3. **[UAV] Autonomous Patrol:** Complete sector-scanning routing protocol.

---

## 🛠️ Technology Stack

* **Core Language:** C++ (Standard Template Library)
* **GUI Framework:** Qt 6 (Qt Widgets, QGraphicsView, QPropertyAnimation)
* **Build System:** CMake / qmake (MinGW 64-bit)

---

## 📚 Applied Concepts

* **Graph Theory:** Nodes, Edges, Weights, and Cycles.
* **Data Structures & Algorithms (DSA):** Matrices, Linked Lists, Permutations.
* **Big-O Complexity:** Space-time tradeoff analysis and algorithm scalability limits.
* **Object-Oriented Programming (OOP):** Encapsulation, custom UI classes, and memory management.

---

## ⚙️ Installation & Build Guide

### Prerequisites

1. Install [Qt Creator](https://www.qt.io/download) with **Qt 6.x** and the **MinGW 64-bit** compiler.
2. Ensure **CMake** is installed and configured in your Qt environment.

### Steps to Run

1. Clone the repository:
```bash
git clone https://github.com/yourusername/LogisticsTSP.git

```


2. Open the `LogisticsTSP.pro` or `CMakeLists.txt` file in **Qt Creator**.
3. Select your kit (e.g., `Desktop Qt 6.x.x MinGW 64-bit`).
4. Build and run the project (`Ctrl+R`).
5. On the Splash Screen, click anywhere or press `Enter` to initialize the mainframe.
6. Click **"📂 Initialize Map Payload"** and select a `.txt` topology file to begin routing.

---

## 📂 Repository Structure

* `main.cpp` - Application entry point.
* `mainwindow.h` / `mainwindow.cpp` - Core UI logic, animations, Qt setup, and event handling.
* `Graph.h` / `Graph.cpp` - Implementation of the Adjacency Matrix and Adjacency List data structures.
* `Algorithms.h` / `Algorithms.cpp` - Mathematical routing logic (Brute Force, Greedy, Stochastic).
* `TopologyData.txt` - Sample payload files for loading node configurations.

---
👨‍💻 Author
M. Farrukh Mubeen Computer Science (BSCS-3B)
LinkedIn: Https://www.linkedin.com/posts/mfarrukhmubeen_cpp-qtframework-datastructures-ugcPost-7464413832468201472-xL0O/?utm_source=share&utm_medium=member_android&rcm=ACoAAFf4T8UB7mLQKywHzHc0IRGt2wrWbwnAmCM
