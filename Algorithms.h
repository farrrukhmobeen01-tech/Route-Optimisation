#pragma once
#include "Graph.h"

RouteResult BruteForce_Matrix(StaticGraph& graph, int start);
RouteResult BruteForce_List(DynamicGraph& graph, int start);
RouteResult Greedy_Matrix(StaticGraph& graph, int start);
RouteResult Greedy_List(DynamicGraph& graph, int start);
RouteResult RandomSearch_Matrix(StaticGraph& graph, int start);
RouteResult RandomSearch_List(DynamicGraph& graph, int start);