/**
 * @file
 * @author The CS2 TA Team
 * @version 1.0
 * @date 2013-2015
 * @copyright This code is in the public domain.
 *
 * @brief The MST and Shortest Path algorithms
 * (implementation).
 *
 */
#include "GraphAlgorithms.hpp"

/**
 * TO STUDENTS: In all of the following functions, feel free to change the
 * function arguments and/or write helper functions as you see fit. Remember to
 * add the function header to GraphAlgorithms.hpp if you write a helper function!
 *
 */

/**
 * TODO: Implement Prim's Algorithm to build the MST.
 * 
 * @brief Builds the MST of the given graph with Prim's algorithm
 * 
 * @param g 	Graph object containing vector of nodes and edges
 * 				Definition for struct Graph in structs.hpp
 * @param app	Graphics application class
 * 				Class definition in GraphApp.hpp
 * 				You should not need to use this class other than passing app
 * 				as a parameter to drawEdge
 * 
 * @attention 	Some hints for implementation and how to interact with our code
 * 				onMST and notOnMST are two vectors defined in GraphAlgorithms.hpp
 * 					that you can use to store which nodes are both in/not in the
 * 					MST. These are cleared at the start of the MST computation for you
 * 				To access the nodes that a specific node connects to you, you
 * 					can use the vector<Node *> edges which is part of the Node struct
 * 					in structs.hpp
 * 				You can compare two nodes to see if they are the same by
 * 					comparing the id attribute of a Node
 *				You can calculate distance from one star to another by calling
 * 					the double distance(Node a) function of the Node struct.
 * 				Whenever you decide to add an edge between two nodes to the MST,
 * 					you can use the provided drawEdge function in GraphAlgorithms.cpp
 * 
 * You can assume that the graph is completely connected. Also, we only use
 * existing edges for the MST.
 * 
 * Add your outline here
 * 
 * 
 */
void buildMSTPrim(Graph g, GraphApp *app) {
	onMST.erase(onMST.begin(), onMST.end());
	notOnMST.erase(notOnMST.begin(), notOnMST.end());
	
	/** REMOVE BELOW THIS COMMENT FOR PUBLIC REPO **/
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
	/** REMOVE ABOVE THIS COMMENT FOR PUBLIC REPO **/
}

/**
 * TODO: Implement Kruskal's Algorithm to build the MST.
 * 
 * @brief Builds the MST of the given graph with Kruskal's algorithm
 * 
 * @param g 	Graph object containing vector of nodes and edges
 * 				Definition for struct Graph in structs.hpp
 * @param app	Graphics application class
 * 				Class definition in GraphApp.hpp
 * 				You should not need to use this class other than passing app
 * 				as a parameter to drawEdge
 * 
 * @attention 	Some hints for implementation and how to interact with our code
 * 				You'll want to use a priority queue to determine which edges
 * 					to add first. We've created the priority queue for you
 * 					along with the compare function it uses. All you need to do
 * 					is call edge_queue.top(), edge_queue.pop(), edge_queue.push()
 * 				The data type that this priority queue stores, KruskalEdge is defined
 * 					in GraphAlgorithms.hpp and is an edge between any two trees
 *				Each Node has a kruskal_edges attribute to store all the nodes it
 * 					connects to in the MST. Make sure to use this to be able
 * 					to join trees later!
 * 				To know which tree a node is in, use the which_tree attribute.
 * 				You can still use the edges, distance, and id attributes of
 * 					a Node.
 * 				When connecting trees, you can call the kruskalFloodFill function
 * 					defined in structs.hpp on a node to convert it and its
 * 					MST connected nodes to a different tree number recursively.
 *				As in Prim's algorith, call drawEdge to add a connection between
 * 					two nodes to the MST
 * 				
 * You can assume that the graph is completely connected. Also, we only use
 * existing edges for the MST.
 * 
 * Add your outline here
 * 
 * 
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
	/** REMOVE ABOVE THIS COMMENT FOR PUBLIC REPO **/
}


/**
 * TODO: Implement Djikstra's shortest path algorithm.
 * 
 * @brief Find the shortest path between start and end nodes with Djikstra's
 * 		  shortest path algorithm
 * 
 * @param start	Index of start node of path to find
 * 					Can access the Node * element by using g.nodes[start]
 * @param end	Index of end node of path to find
 * 					Can access the Node * element by using g.nodes[end]
 * @param g 	Graph object containing vector of nodes and edges
 * 				Definition for struct Graph in structs.hpp
 * @param app	Graphics application class
 * 				Class definition in GraphApp.hpp
 * 				You should not need to use this class other than passing app
 * 				as a parameter to drawEdge
 * 
 * @attention 	Some hints for implementation and how to interact with our code
 * 				You can use the distance_to_start attribute of the Node struct
 * 					in structs.hpp to keep track of what the distance from each
 * 					Node to the start node during computation
 * 				You can use the previous attribute of the Node struct in structs.hpp
 * 					to keep track of the path you are taking to later backtrack
 *				To access the nodes that a specific node connects to you, you
 * 					can use the vector<Node *> edges which is part of the Node struct
 * 					in structs.hpp
 *				Whenever you decide to add an edge between two nodes to the MST,
 * 					you can use the provided drawEdge function in GraphAlgorithms.cpp
 * 
 * Add your outline here
 * 
 * 
 */
void findShortestPath(int start, int end, Graph g, GraphApp * app) {
	/** REMOVE BELOW THIS COMMENT FOR PUBLIC REPO **/
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
	
	Node *s = dest;
	while (s != src && s->previous != NULL) {
		drawEdge(s, s->previous, g.edges, app, false);
		s = s->previous;
	}
	/** REMOVE ABOVE THIS COMMENT FOR PUBLIC REPO **/
}

/**
 * @brief Adds an edge to either the MST or the shortest path based on the
 * 			nodes to connect given. This is done by iterating through the edges
 * 			to find the correct edge given the nodes.
 * 
 * @param pt1	One side of edge to draw
 * @param pt2	Other side of edge to draw
 * @param edges	Vector of edges in the graph
 * @param app	Graphics application class
 * @param mst	True if we are adding edge to MST, False if we are adding edge
 * 					to shortest path
 **/

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
