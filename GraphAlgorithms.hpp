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

#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include "GraphApp.hpp"
#include "structs.hpp"

using namespace std;

void buildMSTPrim(Graph g, GraphApp *app);

void buildMSTKruskal(Graph g, GraphApp *app);

void findShortestPath(int start, int end, Graph g, GraphApp * app);