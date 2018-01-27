/**
 * @file
 * @author The CS2 TA Team
 * @version 1.0
 * @date 2013-2015
 * @copyright This code is in the public domain.
 *
 * @brief The gift wrapping and Graham scan convex hull algorithms
 * (header file).
 *
 */
#pragma once 

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include "GraphApp.hpp"
#include "structs.hpp"

using namespace std;

void buildMSTPrim(Graph g, GraphApp *app);

void buildMSTKruskal(Graph g, GraphApp *app);

void findShortestPath(int start, int end, Graph g, GraphApp * app);

void drawEdge(Node *pt1, Node *pt2, vector<Edge *> edges, GraphApp *app, bool mst);

vector<Node *> onMST;
vector<Node *> notOnMST;


struct KruskalEdge
{
	double weight;
	Node *u;
	Node *v;
};
