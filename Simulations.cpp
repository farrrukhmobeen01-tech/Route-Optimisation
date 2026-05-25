#include "Simulations.h"
#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

void printRepresentations(StaticGraph& matGraph, DynamicGraph& listGraph) {
    cout << "\n\n=======================================================\n";
    cout << "             STATIC REPRESENTATION (MATRIX)            \n";
    cout << "=======================================================\n\n";

    cout << "    | ";
    for (int i = 0; i < matGraph.numVertices; i++) cout << left << setw(4) << setfill(' ') << i;
    cout << "\n-------------------------------------------------------\n";
    for (int i = 0; i < matGraph.numVertices; i++) {
        cout << setw(3) << i << " | ";
        for (int j = 0; j < matGraph.numVertices; j++) {
            cout << left << setw(4) << setfill(' ') << matGraph.g[i][j];
        }
        cout << "\n";
    }

    cout << "\n\n=======================================================\n";
    cout << "             DYNAMIC REPRESENTATION (LINKED LIST)      \n";
    cout << "=======================================================\n\n";

    for (int i = 0; i < listGraph.numVertices; i++) {
        cout << " [" << i << "] " << left << setw(10) << getCityName(i) << " -> ";
        NODEPTR ptr = listGraph.adjList[i];
        if (ptr == NULL) cout << "NULL";
        while (ptr != NULL) {
            cout << "[City " << ptr->adj << "|w:" << ptr->weight << "] -> ";
            ptr = ptr->next;
        }
        if (listGraph.adjList[i] != NULL) cout << "NULL";
        cout << "\n";
    }
    cout << "\n=======================================================\n\n";
}

void printTableRow(string algo, int cost, long long time, int size, string rep) {
    string acc = (cost >= INF) ? "No Path" : to_string(cost) + " km";
    string speedStr = to_string(time) + " us";
    string sizeStr = to_string(size) + " B";

    cout << left << setw(28) << setfill(' ') << algo
        << "| " << setw(15) << acc
        << "| " << setw(15) << speedStr
        << "| " << setw(15) << sizeStr
        << "| " << rep << "\n";
}

void printPerformanceAnalysis(RouteResult res, int bytes, string complexity) {
    string acc = (res.cost >= INF) ? "No Path Found" : to_string(res.cost) + " km";

    cout << "\n========================================================================\n";
    cout << "                     PERFORMANCE ANALYSIS REPORT                        \n";
    cout << "========================================================================\n";
    cout << left << setw(20) << "Metric" << "| " << "Description\n";
    cout << "--------------------|---------------------------------------------------\n";
    cout << left << setw(20) << "Path Cost" << "| Total distance: " << acc << "\n";
    cout << left << setw(20) << "Execution Time" << "| Time per algorithm: " << res.timeTaken << " us\n";
    cout << left << setw(20) << "Memory Usage" << "| Space used: " << bytes << " bytes\n";
    cout << left << setw(20) << "Complexity" << "| Growth with input size: " << complexity << "\n";
    cout << "========================================================================\n";
}

void printAlgorithmComparison(RouteResult bfm, RouteResult bfl, RouteResult grm, RouteResult grl, RouteResult csm, RouteResult csl, int matBytes, int listBytes) {
    cout << "\n===============================================================================================\n";
    cout << "                                ALGORITHM COMPARISON REPORT                                    \n";
    cout << "===============================================================================================\n";
    cout << left << setw(28) << "Algorithm"
        << "| " << setw(15) << "Accuracy"
        << "| " << setw(15) << "Speed"
        << "| " << setw(15) << "Size"
        << "| " << "Representation\n";
    cout << "-----------------------------------------------------------------------------------------------\n";
    printTableRow(bfm.algoName, bfm.cost, bfm.timeTaken, matBytes, "Static (Matrix)");
    printTableRow(bfl.algoName, bfl.cost, bfl.timeTaken, listBytes, "Dynamic (List)");
    cout << "-----------------------------------------------------------------------------------------------\n";
    printTableRow(grm.algoName, grm.cost, grm.timeTaken, matBytes, "Static (Matrix)");
    printTableRow(grl.algoName, grl.cost, grl.timeTaken, listBytes, "Dynamic (List)");
    cout << "-----------------------------------------------------------------------------------------------\n";
    printTableRow(csm.algoName, csm.cost, csm.timeTaken, matBytes, "Static (Matrix)");
    printTableRow(csl.algoName, csl.cost, csl.timeTaken, listBytes, "Dynamic (List)");
    cout << "===============================================================================================\n";
}

void delaySimulation() {
    auto start_time = high_resolution_clock::now();
    while (duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count() < 400);
}

void printCityOptions(int limit) {
    cout << "\n--- Available Hubs & Locations ---\n";
    for (int i = 0; i < limit; i++) {
        cout << " [" << right << setw(2) << setfill('0') << i << "] " << left << setw(15) << setfill(' ') << getCityName(i);
        if ((i + 1) % 4 == 0) cout << "\n";
    }
    cout << "\n";
}

void printResult(RouteResult res) {
    cout << "\n=======================================================\n";
    cout << " [LOGISTICS ROUTE MANIFEST] \n";
    cout << "=======================================================\n";

    if (res.cost >= INF || res.pathLen == 0) {
        cout << " [SYSTEM STATUS] : FAILED (DISCONNECTED GRAPH)\n";
        cout << " [ERROR]         : No valid closed-loop path exists.\n";
        cout << "=======================================================\n";
        return;
    }

    cout << left << setw(20) << " ALGORITHM APPLIED" << ": " << res.algoName << "\n";
    cout << left << setw(20) << " ORIGIN HUB" << ": " << getCityName(res.path[0]) << "\n";
    cout << left << setw(20) << " DESTINATION" << ": " << getCityName(res.path[res.pathLen - 1]) << " (Return)\n";
    cout << left << setw(20) << " TOTAL WAYPOINTS" << ": " << (res.pathLen - 2) << " Stops\n";
    cout << left << setw(20) << " COMPUTATION TIME" << ": " << res.timeTaken << " microseconds\n";
    cout << "-------------------------------------------------------\n";
    cout << " [METRICS]\n";
    cout << " -> Total Distance   : " << res.cost << " km\n";
    cout << " -> Est. Fuel Req.   : " << fixed << setprecision(1) << (res.cost / 10.0) << " Liters (Based on 10 km/L)\n";
    cout << "-------------------------------------------------------\n";
    cout << " [APPROVED ITINERARY]\n";

    for (int i = 0; i < res.pathLen; i++) {
        if (i == 0) {
            cout << "   " << right << setw(2) << setfill('0') << i + 1 << ". " << setfill(' ') << left << setw(15) << getCityName(res.path[i]) << " [DEPARTURE]\n";
        }
        else if (i == res.pathLen - 1) {
            cout << "   " << right << setw(2) << setfill('0') << i + 1 << ". " << setfill(' ') << left << setw(15) << getCityName(res.path[i]) << " [RETURN COMPLETE]\n";
        }
        else {
            cout << "   " << right << setw(2) << setfill('0') << i + 1 << ". " << setfill(' ') << left << setw(15) << getCityName(res.path[i]) << " [WAYPOINT]\n";
        }
    }
    cout << "=======================================================\n";
}

void simulateDeliveryRoute(StaticGraph& graph, int startCity) {
    cout << "\n[INITIALIZING LOGISTICS DELIVERY MODULE...]\n";
    RouteResult result = Greedy_Matrix(graph, startCity);

    if (result.cost >= INF) {
        cout << "[SYSTEM ERROR: Graph disconnect. Delivery Cancelled.]\n";
        return;
    }

    cout << "\n=======================================================\n";
    cout << "       SCENARIO 1: DELIVERY ROUTE OPTIMIZATION         \n";
    cout << "=======================================================\n";
    cout << "Driver Starting Hub : " << getCityName(result.path[0]) << "\n";
    cout << "Total Route Distance: " << result.cost << " km\n";
    cout << "Est. Fuel Required  : " << fixed << setprecision(1) << (result.cost / 10.0) << " Liters (10 km/L)\n\n";

    for (int i = 0; i < result.pathLen - 1; i++) {
        cout << " [Stop " << i + 1 << "] Deliver to " << getCityName(result.path[i + 1]) << "\n";
    }
    cout << "=======================================================\n";
}

void simulateGPSNavigation(StaticGraph& graph) {
    cout << "\n=======================================================\n";
    cout << " [GPS POINT-TO-POINT NAVIGATION SYSTEM] \n";
    cout << "=======================================================\n";
    printCityOptions(graph.numVertices);

    int startCity, endCity;
    cout << "Enter Current Location ID: "; cin >> startCity;
    cout << "Enter Destination ID: "; cin >> endCity;

    if (startCity >= 0 && startCity < graph.numVertices && endCity >= 0 && endCity < graph.numVertices) {
        int dist = graph.g[startCity][endCity];
        cout << "\n-------------------------------------------------------\n";
        if (dist > 0) {
            cout << " [DIRECTIONS] : Drive straight from " << getCityName(startCity) << " to " << getCityName(endCity) << ".\n";
            cout << " [DISTANCE]   : " << dist << " km\n";
            cout << " [EST. TIME]  : " << (dist / 80) << " hours " << ((dist % 80) * 60 / 80) << " minutes (at 80 km/h)\n";
        }
        else {
            cout << " [ERROR]      : No direct highway found. Rerouting required.\n";
        }
        cout << "-------------------------------------------------------\n";
    }
    else {
        cout << "[ERROR] Invalid GPS coordinates entered.\n";
    }
}

void simulateRoboticsPathPlanning(StaticGraph& graph, int startCity) {
    cout << "\n=======================================================\n";
    cout << " [AUTONOMOUS ROBOTICS PATH PLANNING] \n";
    cout << "=======================================================\n";

    RouteResult result = Greedy_Matrix(graph, startCity);
    if (result.cost >= INF || result.pathLen == 0) {
        cout << "[SYSTEM ERROR: Graph disconnect. Cannot complete patrol loop.]\n";
        return;
    }

    cout << "[SYS] Uploading coordinate matrix to drone...\n";
    cout << "[SYS] Route locked. Base station: " << getCityName(startCity) << "\n";
    cout << "[SYS] Engaging rotors...\n\n";

    for (int i = 0; i < result.pathLen; i++) {
        if (i == 0) {
            cout << " -> [LAUNCH] Leaving " << getCityName(result.path[i]) << "\n";
        }
        else {
            cout << " -> [ARRIVED] Waypoint: " << getCityName(result.path[i]) << "\n";
        }

        if (i < result.pathLen - 1) {
            cout << "    |-- Scanning sector...\n";
            delaySimulation();
            cout << "    |-- Sector clear. Proceeding to next target.\n";
            delaySimulation();
        }
    }
    cout << "\n[SYS] Patrol complete. Battery optimal. Entering sleep mode.\n";
    cout << "=======================================================\n";
}

void analyzeSpace(StaticGraph& g_mat, DynamicGraph& g_list) {
    cout << "\n=======================================================\n";
    cout << " [MEMORY AND SPACE COMPLEXITY ANALYSIS] \n";
    cout << "=======================================================\n";

    int v = g_mat.numVertices;
    int e = g_list.edgeCount;

    int matBytes = v * v * sizeof(int);
    int listBytes = (v * sizeof(NODEPTR)) + (e * sizeof(Node));

    cout << left << setw(25) << " [STATIC] Adj. Matrix" << ": " << matBytes << " bytes (O(V^2))\n";
    cout << left << setw(25) << " [DYNAMIC] Adj. List" << ": " << listBytes << " bytes (O(V+E))\n";
    cout << "=======================================================\n";
}