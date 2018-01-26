/**
 * @file
 * @author The CS2 TA Team
 * @version 1.0
 * @date 2013-2015
 * @copyright This code is in the public domain.
 *
 * @brief The gift wrapping and Graham scan convex hull algorithms
 * (implementation).
 *
 */
#include "GraphAlgorithms.hpp"

/**
 * TO STUDENTS: In all of the following functions, feel free to change the
 * function arguments and/or write helper functions as you see fit. Remember to
 * add the function header to GraohAlgorithms.hpp if you write a helper function!
 *
 */

/**
 * TODO: Implement this function.
 */
void buildMSTPrim(Graph g, GraphApp *app) {
    
    for (uint i = 0; i < g.edges.size(); ++i) {
        app->add_to_mst(g.edges[i]);
    }

}

/**
 * TODO: Implement this function.
 */
void buildMSTKruskal(Graph g, GraphApp *app) {
    
    for (uint i = 0; i < g.edges.size(); ++i) {
        app->add_to_mst(g.edges[i]);
    }

}

/**
 * TODO: Implement this function.
 */
void findShortestPath(int start, int end, Graph g, GraphApp * app) {
    // start is the id of the starting node
    // end is the id of the ending node

    for (uint i = 0; i < g.edges.size(); ++i) {
        app->add_to_path(g.edges[i]);
    }
}

