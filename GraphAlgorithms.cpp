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
    vector<Node *> nodes = g.nodes;
    
    onMST.erase(onMST.begin(), onMST.end());
	notOnMST.erase(notOnMST.begin(), notOnMST.end());
	
    for (unsigned int i = 0; i < nodes.size(); i++) {
		if (!nodes[i]->mst_edges.empty()) {
			nodes[i]->mst_edges.erase(nodes[i]->mst_edges.begin(), nodes[i]->mst_edges.end());
		}
		if (i != 0) {
			notOnMST.push_back(nodes[i]);
		}
	}
	
	onMST.push_back(nodes[0]);
	
	Node *notOnTree;
	Node *onTree;
	while (!notOnMST.empty()) {
		double shortestDist = INFINITY;
		notOnTree = NULL;
		onTree = NULL;
		int added = -1;
		for (unsigned int i = 0; i < onMST.size(); i++) {
			for (unsigned int j = 0; j < onMST[i]->edges.size(); j++) {
				double dist = onMST[i]->distance(*(onMST[i]->edges[j]));
				bool alreadyIn = true;
				if (dist < shortestDist) {
					for (unsigned int k = 0; k < notOnMST.size(); k++) {
						if (onMST[i]->edges[j]->id == notOnMST[k]->id) {
							alreadyIn = false;
							added = k;
						}
					}
					if (alreadyIn) continue;
					shortestDist = dist;
					notOnTree = onMST[i]->edges[j];
					onTree = onMST[i];
				}
			}
		}
		onTree->mst_edges.push_back(notOnTree);
		notOnTree->mst_edges.push_back(onTree);
		
		app->add_to_mst(g.edges[getEdge(notOnTree, onTree, g.edges)]);
		
		onMST.push_back(notOnTree);
		notOnMST.erase(notOnMST.begin() + added);
	}
	/*
    for (uint i = 0; i < g.edges.size(); ++i) {
        app->add_to_mst(g.edges[i]);
    }*/

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
	// Using a vector rather than a priority queue for now
	
    // src is starting node
    // dest is the ending node
    vector<Node *> nodes = g.nodes;
    Node *src = nodes[start];
    Node *dest = nodes[end];
    // container to keep track of all unoptimized nodes
	std::vector<Node *> Q;
	unsigned int dQ;
	
	// infinitize all distance values	
	for (unsigned int f = 0; f < nodes.size(); f++) {
		nodes[f]->distance_to_start = INFINITY;
		nodes[f]->previous = NULL;
		Q.push_back(nodes[f]);
	}
	
	// distances to self is 0
	src->distance_to_start = 0;
	
	// algorithm main loop
	while (Q.size() != 0) {
		Node *u = NULL;
		
		// This isn't exactly efficient, but I wanted to just
		// keep it the same as the old tested solution code :)
		
		dQ = Q.size() + 1;
		for (unsigned int i = 0; i < Q.size(); i++) {
			if (u == NULL || Q[i]->distance_to_start < u->distance_to_start) {
				u = Q[i];
				dQ = i;
			}
		}
		if (dQ != Q.size() + 1) {
			Q.erase(Q.begin() + dQ);
		}
		// Remaining vertices unreachable or found our goal
		if (u->distance_to_start == INFINITY || u == dest) {
			break;
		}
		
		for (unsigned int p = 0; p < u->edges.size(); p++) {
			Node *v = u->edges[p];
			double alt = u->distance_to_start + u->distance(*v);
			if (alt < v->distance_to_start) {
				v->distance_to_start = alt;
				v->previous = u;
			}
		}
	}
	
	std::list<Node *> ret;
	ret.push_front(dest);
	Node *s = dest;
	while (s != src && s->previous != NULL) {
		int i = getEdge(s, s->previous, g.edges);
		if (i != -1) {
			app->add_to_path(g.edges[i]);
		}
		s = s->previous;
		ret.push_front(s);
	}
	/*
    for (uint i = 0; i < g.edges.size(); ++i) {
        app->add_to_path(g.edges[i]);
    }*/
}

// this is so ugly :(
int getEdge(Node *pt1, Node *pt2, vector<Edge *> edges) {
	for (unsigned int i = 0; i < edges.size(); i++) {
		if ((edges[i]->a == pt1 && edges[i]->b == pt2) || (edges[i]->a == pt2 && edges[i]->b == pt1)) {
			return i;
		}
	}
	return -1;
}
