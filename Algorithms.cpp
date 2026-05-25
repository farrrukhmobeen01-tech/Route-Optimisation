#include "../../Documents/LogisticsTSP/Algorithms.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <QGraphicsTextItem>

using namespace std;
using namespace std::chrono;

RouteResult BruteForce_Matrix(StaticGraph& graph, int start) {
    auto startTime = high_resolution_clock::now();
    int vertices[MAX_CITIES], vCount = 0;

    for (int i = 0; i < graph.numVertices; i++) {
        if (i != start) vertices[vCount++] = i;
    }

    int minCost = INF;
    int bestPath[MAX_CITIES] = { 0 };

    do {
        int currCost = 0, currCity = start;
        bool valid = true;

        for (int i = 0; i < vCount; i++) {
            int w = graph.g[currCity][vertices[i]];
            if (w == 0) { valid = false; break; }
            currCost += w;
            currCity = vertices[i];
        }

        if (valid && graph.g[currCity][start] != 0) {
            currCost += graph.g[currCity][start];
            if (currCost < minCost) {
                minCost = currCost;
                for (int i = 0; i < vCount; i++) bestPath[i] = vertices[i];
            }
        }
    } while (next_permutation(vertices, vertices + vCount));

    auto stopTime = high_resolution_clock::now();
    RouteResult res;
    res.algoName = "Brute Force";
    res.timeTaken = duration_cast<microseconds>(stopTime - startTime).count();

    if (minCost == INF) return res;

    res.path[0] = start;
    for (int i = 0; i < vCount; i++) res.path[i + 1] = bestPath[i];
    res.path[vCount + 1] = start;
    res.pathLen = vCount + 2;
    res.cost = minCost;
    return res;
}

RouteResult BruteForce_List(DynamicGraph& graph, int start) {
    auto startTime = high_resolution_clock::now();
    int vertices[MAX_CITIES], vCount = 0;

    for (int i = 0; i < graph.numVertices; i++) {
        if (i != start) vertices[vCount++] = i;
    }

    int minCost = INF;
    int bestPath[MAX_CITIES] = { 0 };

    do {
        int currCost = 0, currCity = start;
        bool valid = true;

        for (int i = 0; i < vCount; i++) {
            int w = graph.getWeight(currCity, vertices[i]);
            if (w == 0) { valid = false; break; }
            currCost += w;
            currCity = vertices[i];
        }

        if (valid && graph.getWeight(currCity, start) != 0) {
            currCost += graph.getWeight(currCity, start);
            if (currCost < minCost) {
                minCost = currCost;
                for (int i = 0; i < vCount; i++) bestPath[i] = vertices[i];
            }
        }
    } while (next_permutation(vertices, vertices + vCount));

    auto stopTime = high_resolution_clock::now();
    RouteResult res;
    res.algoName = "Brute Force";
    res.timeTaken = duration_cast<microseconds>(stopTime - startTime).count();

    if (minCost == INF) return res;

    res.path[0] = start;
    for (int i = 0; i < vCount; i++) res.path[i + 1] = bestPath[i];
    res.path[vCount + 1] = start;
    res.pathLen = vCount + 2;
    res.cost = minCost;
    return res;
}

RouteResult Greedy_Matrix(StaticGraph& graph, int start) {
    auto startTime = high_resolution_clock::now();
    int n = graph.numVertices;
    bool visited[MAX_CITIES] = { false };

    RouteResult res;
    res.algoName = "Greedy";
    res.path[0] = start;
    res.pathLen = 1;

    int totalCost = 0, curr = start;
    visited[curr] = true;

    for (int step = 0; step < n - 1; step++) {
        int nextCity = -1, minDist = INF;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && graph.g[curr][j] > 0 && graph.g[curr][j] < minDist) {
                minDist = graph.g[curr][j];
                nextCity = j;
            }
        }
        if (nextCity == -1) break;

        visited[nextCity] = true;
        res.path[res.pathLen++] = nextCity;
        totalCost += minDist;
        curr = nextCity;
    }

    bool allVisited = true;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) { allVisited = false; break; }
    }

    if (allVisited && graph.g[curr][start] > 0) {
        totalCost += graph.g[curr][start];
        res.path[res.pathLen++] = start;
        res.cost = totalCost;
    }
    else {
        res.cost = INF;
        res.pathLen = 0;
    }

    res.timeTaken = duration_cast<microseconds>(high_resolution_clock::now() - startTime).count();
    return res;
}

RouteResult Greedy_List(DynamicGraph& graph, int start) {
    auto startTime = high_resolution_clock::now();
    int n = graph.numVertices;
    bool visited[MAX_CITIES] = { false };

    RouteResult res;
    res.algoName = "Greedy";
    res.path[0] = start;
    res.pathLen = 1;

    int totalCost = 0, curr = start;
    visited[curr] = true;

    for (int step = 0; step < n - 1; step++) {
        int nextCity = -1, minDist = INF;
        NODEPTR ptr = graph.adjList[curr];

        while (ptr != NULL) {
            if (!visited[ptr->adj] && ptr->weight < minDist) {
                minDist = ptr->weight;
                nextCity = ptr->adj;
            }
            ptr = ptr->next;
        }

        if (nextCity == -1) break;

        visited[nextCity] = true;
        res.path[res.pathLen++] = nextCity;
        totalCost += minDist;
        curr = nextCity;
    }

    bool allVisited = true;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) { allVisited = false; break; }
    }

    int homeDist = graph.getWeight(curr, start);
    if (allVisited && homeDist > 0) {
        totalCost += homeDist;
        res.path[res.pathLen++] = start;
        res.cost = totalCost;
    }
    else {
        res.cost = INF;
        res.pathLen = 0;
    }

    res.timeTaken = duration_cast<microseconds>(high_resolution_clock::now() - startTime).count();
    return res;
}

RouteResult RandomSearch_Matrix(StaticGraph& graph, int start) {
    auto startTime = high_resolution_clock::now();
    int vertices[MAX_CITIES], vCount = 0;

    for (int i = 0; i < graph.numVertices; i++) {
        if (i != start) vertices[vCount++] = i;
    }

    int minCost = INF;
    int bestPath[MAX_CITIES] = { 0 };

    for (int attempt = 0; attempt < 5000; attempt++) {
        int tempVertices[MAX_CITIES];
        for (int i = 0; i < vCount; i++) tempVertices[i] = vertices[i];

        for (int i = vCount - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = tempVertices[i];
            tempVertices[i] = tempVertices[j];
            tempVertices[j] = temp;
        }

        int currCost = 0, currCity = start;
        bool valid = true;

        for (int i = 0; i < vCount; i++) {
            int w = graph.g[currCity][tempVertices[i]];
            if (w == 0) { valid = false; break; }
            currCost += w;
            currCity = tempVertices[i];
        }

        if (valid && graph.g[currCity][start] != 0) {
            currCost += graph.g[currCity][start];
            if (currCost < minCost) {
                minCost = currCost;
                for (int k = 0; k < vCount; k++) bestPath[k] = tempVertices[k];
            }
        }
    }

    auto stopTime = high_resolution_clock::now();
    RouteResult res;
    res.algoName = "My Algo (Random)";
    res.timeTaken = duration_cast<microseconds>(stopTime - startTime).count();

    if (minCost == INF) return res;

    res.path[0] = start;
    for (int i = 0; i < vCount; i++) res.path[i + 1] = bestPath[i];
    res.path[vCount + 1] = start;
    res.pathLen = vCount + 2;
    res.cost = minCost;
    return res;
}

RouteResult RandomSearch_List(DynamicGraph& graph, int start) {
    auto startTime = high_resolution_clock::now();
    int vertices[MAX_CITIES], vCount = 0;

    for (int i = 0; i < graph.numVertices; i++) {
        if (i != start) vertices[vCount++] = i;
    }

    int minCost = INF;
    int bestPath[MAX_CITIES] = { 0 };

    for (int attempt = 0; attempt < 5000; attempt++) {
        int tempVertices[MAX_CITIES];
        for (int i = 0; i < vCount; i++) tempVertices[i] = vertices[i];

        for (int i = vCount - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = tempVertices[i];
            tempVertices[i] = tempVertices[j];
            tempVertices[j] = temp;
        }

        int currCost = 0, currCity = start;
        bool valid = true;

        for (int i = 0; i < vCount; i++) {
            int w = graph.getWeight(currCity, tempVertices[i]);
            if (w == 0) { valid = false; break; }
            currCost += w;
            currCity = tempVertices[i];
        }

        if (valid && graph.getWeight(currCity, start) != 0) {
            currCost += graph.getWeight(currCity, start);
            if (currCost < minCost) {
                minCost = currCost;
                for (int k = 0; k < vCount; k++) bestPath[k] = tempVertices[k];
            }
        }
    }

    auto stopTime = high_resolution_clock::now();
    RouteResult res;
    res.algoName = "My Algo (Random)";
    res.timeTaken = duration_cast<microseconds>(stopTime - startTime).count();

    if (minCost == INF) return res;

    res.path[0] = start;
    for (int i = 0; i < vCount; i++) res.path[i + 1] = bestPath[i];
    res.path[vCount + 1] = start;
    res.pathLen = vCount + 2;
    res.cost = minCost;
    return res;
}