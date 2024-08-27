#include "../header/Node.h"

Node::Node(int x, int y) : x(x), y(y) {}

// Computes the Euclidean distance between this node and another node
float Node::distanceTo(const Node* other) const {
    return std::sqrtf((x - other->x) * (x - other->x) + (y - other->y) * (y - other->y));
}

// Computes the heuristic cost from this node to another node
// In this case, the heuristic is the same as the Euclidean distance
float Node::heuristicTo(const Node* other) const {
    return distanceTo(other);
}
