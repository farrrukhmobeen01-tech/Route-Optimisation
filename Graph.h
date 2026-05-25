#pragma once
#include <string>

#define MAX_CITIES 20
const int INF = 99999999;

std::string getCityName(int index);

struct Node {
    int adj;
    int weight;
    Node* next;
};
typedef Node* NODEPTR;

class DynamicGraph {
public:
    int numVertices;
    int edgeCount;
    NODEPTR adjList[MAX_CITIES];

    DynamicGraph(int v);
    ~DynamicGraph();
    void addEdge(int u, int v, int w);
    int getWeight(int u, int v);
};

class StaticGraph {
public:
    int numVertices;
    int g[MAX_CITIES][MAX_CITIES];

    StaticGraph(int v);
    void addEdge(int u, int v, int w);
};

struct RouteResult {
    int path[MAX_CITIES + 1] = { 0 };
    int pathLen = 0;
    int cost = INF;
    std::string algoName = "";
    long long timeTaken = 0;
};