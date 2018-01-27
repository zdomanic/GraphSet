/**
 * @file structs.hpp
 * @author The CS2 TA Team
 * @version 1.0
 * @date 2013-2015
 * @copyright This code is in the public domain.
 *
 * @brief The Tuple struct for representing lattice point in the Euclidean
 *        2-D plane.
 */
#pragma once
#include <cmath>
#include <iostream>

using namespace std;


/**
 * @brief Struct representing a lattice point in the (Euclidean) 2-D plane.
 *
 * The Tuple struct is a representation of a lattice point in the (Euclidean)
 * 2-D plane. The stuct provides functionalities such as printing and angle
 * computations.
 */
struct Node
{
    /**
     * @brief Represents the x-coordinate of the point
     */
    int x;

    /**
     * @brief Represents the y-coordinate of the point
     */
    int y;

    /* 
     * @brief node id
     */
    int id;

    /**
     * @brief Initializes the struct
     *
     * @param x: first argument representing the x-coordinate
     * @param y: second argument representing the y-coordinate
     */
    Node(int id_, int x_, int y_) 
        : x(x_)
        , y(y_)
        , id(id_)
    {}

    double distance (Node a) {
        return sqrt((x - a.x)*(x - a.x) + (y - a.y)*(y - a.y));
    }
    
        
    std::vector<Node *> edges;	// adjacency list
    
    // extra variables for algorithms
    Node * previous;
    double distance_to_start;
};

struct Edge
{
    Node *a;
    Node *b;
    int id;

    double weight;

    Edge(int id_, Node *a_, Node *b_, double weight_)
        : a(a_)
        , b(b_)
        , id(id_)
        , weight(weight_) 
    {}

};

struct Graph
{
    vector<Node *> nodes;
    vector<Edge *> edges;
    
    Graph(vector<Node *> nodes_, vector<Edge *> edges_)
        : nodes(nodes_)
        , edges(edges_)
    {}
};
