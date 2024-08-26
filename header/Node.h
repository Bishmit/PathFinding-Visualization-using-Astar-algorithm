#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <cmath>
#include<iostream>

class Node {
public:
    Node(int x, int y);

    bool isObstacle = false;
    bool isVisited = false;
    float globalGoal = INFINITY;
    float localGoal = INFINITY;
    int x, y;
    std::vector<Node*> neighbors;
    Node* parent = nullptr;

    // For internal calculations
    float distanceTo(const Node* other) const;
    float heuristicTo(const Node* other) const;
};

#endif 
