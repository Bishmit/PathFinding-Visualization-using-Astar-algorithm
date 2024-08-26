#include "../header/Node.h"

Node::Node(int x, int y) : x(x), y(y) {}

float Node::distanceTo(const Node* other) const {
    return std::sqrtf((x - other->x) * (x - other->x) + (y - other->y) * (y - other->y));
}

float Node::heuristicTo(const Node* other) const {
    return distanceTo(other);
}
