#include "Graph.h"

using namespace std;

string getCityName(int index) {
    string cityNames[10] = {
        "Islamabad", "Rawalpindi", "Lahore", "Karachi", "Peshawar",
        "Quetta", "Multan", "Faisalabad", "Sialkot", "Gujranwala"
    };
    if (index < 10) return cityNames[index];
    return "City_" + to_string(index);
}

// DynamicGraph Implementation
DynamicGraph::DynamicGraph(int v) {
    numVertices = v;
    edgeCount = 0;
    for (int i = 0; i < v; i++) {
        adjList[i] = NULL;
    }
}

DynamicGraph::~DynamicGraph() {
    for (int i = 0; i < numVertices; i++) {
        NODEPTR ptr = adjList[i];
        while (ptr != NULL) {
            NODEPTR temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }
}

void DynamicGraph::addEdge(int u, int v, int w) {
    if (w > 0) {
        NODEPTR p = new Node;
        p->adj = v;
        p->weight = w;
        p->next = adjList[u];
        adjList[u] = p;
        edgeCount++;
    }
}

int DynamicGraph::getWeight(int u, int v) {
    NODEPTR ptr = adjList[u];
    while (ptr != NULL) {
        if (ptr->adj == v) return ptr->weight;
        ptr = ptr->next;
    }
    return 0;
}

// StaticGraph Implementation
StaticGraph::StaticGraph(int v) {
    numVertices = v;
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            g[i][j] = 0;
        }
    }
}

void StaticGraph::addEdge(int u, int v, int w) {
    g[u][v] = w;
}