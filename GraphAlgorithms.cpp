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
 * 
 * This assumes that the graph is connected
 * We also only use existing edges on the MST
 * 
 * 
 */
void buildMSTPrim(Graph g, GraphApp *app) {
	onMST.erase(onMST.begin(), onMST.end());
	notOnMST.erase(notOnMST.begin(), notOnMST.end());
	
	
    vector<Node *> nodes = g.nodes;
    
    for (unsigned int i = 0; i < nodes.size(); i++) {
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
		
		drawEdge(notOnTree, onTree, g.edges, app, true);
		
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
 * Do not ignore existing edges
 */
void buildMSTKruskal(Graph g, GraphApp *app) {
    auto compare_func = [](KruskalEdge *a, KruskalEdge *b) {
		return (a->weight > b->weight);
	};
	std::priority_queue<KruskalEdge*, 
						std::vector<KruskalEdge*>, 
						decltype(compare_func)>
						edge_queue(compare_func);
						
	/** REMOVE BELOW FOR STUDENT IMPLEMENTATION **/
	vector<Node *> nodes = g.nodes;
	int j = 0;
	for (unsigned int i = 0; i < nodes.size(); i++) {
		nodes[i]->which_tree = j++;
		nodes[i]->kruskal_edges.clear();
		for (unsigned int k = 0; k < nodes[i]->edges.size(); k++) {
			Node *q = nodes[i]->edges[k];
			if (nodes[i]->id < q->id) {
				KruskalEdge *e = new KruskalEdge();
				e->u = nodes[i];
				e->v = q;
				e->weight = nodes[i]->distance(*q);
				edge_queue.push(e);
			}
		}
	}
	
	while (j > 1 && edge_queue.size() > 0) {
		KruskalEdge *e = edge_queue.top();
		edge_queue.pop();
		
		Node *u = e->u;
		Node *v = e->v;
		if (u->which_tree != v->which_tree) {
			v->kruskalFloodFill(u->which_tree);
			v->kruskal_edges.push_back(u);
			u->kruskal_edges.push_back(v);
			drawEdge(u, v, g.edges, app, true);
			j--;
		}
		delete e;
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
		drawEdge(s, s->previous, g.edges, app, false);
		s = s->previous;
		ret.push_front(s);
	}
	/*
    for (uint i = 0; i < g.edges.size(); ++i) {
        app->add_to_path(g.edges[i]);
    }*/
}

// this is so ugly :(
void drawEdge(Node *pt1, Node *pt2, vector<Edge *> edges, GraphApp * app, bool mst) {
	for (unsigned int i = 0; i < edges.size(); i++) {
		if ((edges[i]->a == pt1 && edges[i]->b == pt2) || (edges[i]->a == pt2 && edges[i]->b == pt1)) {
			if (mst)
				app->add_to_mst(edges[i]);
			else
				app->add_to_path(edges[i]);
			break;
		}
	}
	return;
}
