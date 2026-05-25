#pragma once
#include "Graph.h"
#include "Algorithms.h"
#include <string>

void printRepresentations(StaticGraph& matGraph, DynamicGraph& listGraph);
void printTableRow(std::string algo, int cost, long long time, int size, std::string rep);


void printPerformanceAnalysis(RouteResult res, int bytes, std::string complexity);
void printAlgorithmComparison(RouteResult bfm, RouteResult bfl, RouteResult grm, RouteResult grl, RouteResult csm, RouteResult csl, int matBytes, int listBytes);

void delaySimulation();
void printCityOptions(int limit);
void printResult(RouteResult res);
void simulateDeliveryRoute(StaticGraph& graph, int startCity);
void simulateGPSNavigation(StaticGraph& graph);
void simulateRoboticsPathPlanning(StaticGraph& graph, int startCity);
void analyzeSpace(StaticGraph& g_mat, DynamicGraph& g_list);