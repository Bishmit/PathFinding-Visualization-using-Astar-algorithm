#include "../header/Pathfinding.h"

Pathfinding::Pathfinding(int width, int height) : mapWidth(width), mapHeight(height), temp_x(0), temp_y(0), startNode(nullptr), endNode(nullptr) {
    nodes.reserve(mapWidth * mapHeight);

    // Initialize rectangles as a 2D vector
    rectangles.resize(mapHeight, std::vector<sf::RectangleShape>(mapWidth));

    float shiftX = 3.5f; // Shift right // lazy adjustment
    float shiftY = 3.5f; // Shift down

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            Node node(x, y);
            nodes.push_back(node);

            sf::RectangleShape& rectangle = rectangles[y][x];
            rectangle.setSize(sf::Vector2f(nodeSize, nodeSize));
            rectangle.setPosition(x * nodeSize + shiftX , y * nodeSize + shiftY);
            rectangle.setFillColor(sf::Color(255, 255, 0, 50)); 
            rectangle.setScale(sf::Vector2f(0.5f, 0.5f)); 
        }
    }
}


void Pathfinding::updateNeigboursNode()
{
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            Node& node = nodes[y * mapWidth + x]; // nodes[y* mapWidth + x] Calculates the index in the 1d array that corresponds to the (x.y) coordinates in a 2d grid
            node.neighbors.clear(); // remove all elemetns from the neigbours 

            // Up
            if (y > 0) node.neighbors.push_back(&nodes[(y - 1) * mapWidth + x]);

            // Down
            if (y < mapHeight - 1) node.neighbors.push_back(&nodes[(y + 1) * mapWidth + x]);

            // Left
            if (x > 0) node.neighbors.push_back(&nodes[y * mapWidth + (x - 1)]);

            // Right
            if (x < mapWidth - 1) node.neighbors.push_back(&nodes[y * mapWidth + (x + 1)]);

            // Upper condition add verticals and horizontal neigbours in grid and if we have to include Diagonals neigbours we can do follow things
            if (!includeDiagonalsFlag) {
                // Top-Left
                if (x > 0 && y > 0) node.neighbors.push_back(&nodes[(y - 1) * mapWidth + (x - 1)]);

                // Top-Right
                if (x < mapWidth - 1 && y > 0) node.neighbors.push_back(&nodes[(y - 1) * mapWidth + (x + 1)]);

                // Bottom-Left
                if (x > 0 && y < mapHeight - 1) node.neighbors.push_back(&nodes[(y + 1) * mapWidth + (x - 1)]);

                // Bottom-Right
                if (x < mapWidth - 1 && y < mapHeight - 1) node.neighbors.push_back(&nodes[(y + 1) * mapWidth + (x + 1)]);
            }
        }
    }

    // initiall startNode and endNode Position setup 
    if (!Dpressed) {
        startNode = &nodes[(mapHeight / 2) * mapWidth + 1];
        endNode = &nodes[(mapHeight / 2) * mapWidth + mapWidth - 2];
    }
    else {
        // Ensure startNode and endNode are different when D key is pressed is true
        if (temp_x >= 0 && temp_x < mapWidth && temp_y >= 0 && temp_y < mapHeight) {
            if (startNode == nullptr) { // check if start node has not been selected yet
                startNode = &nodes[temp_y * mapWidth + temp_x];  // if so assign the startnode to its current position 
            }
            else if (endNode == nullptr) { 
                endNode = &nodes[temp_y * mapWidth + temp_x];  // assign the endnode to its current position if it is changed
            }
        }
    }
}

void Pathfinding::solveAStar() 
{
    for (auto& node : nodes) {
        node.isVisited = false; // Marks all nodes as not visited intially 
        node.globalGoal = INFINITY; // initially, estimated cost from start node to end node is assigned infinty 
        node.localGoal = INFINITY; // if we want to traverse to any node initially it is set to infinity 
        node.parent = nullptr; // parent node is not associated with any node initially 
    }

    Node* currentNode = startNode; // set up the start node
    startNode->localGoal = 0.0f;   // cost to reach itself is zero 
    startNode->globalGoal = startNode->heuristicTo(endNode); // this will calculate the heuristic function using eucledian distance

    std::list<Node*> notTestedNodes; // making list of non tested nodes , list in stl is double linked list which keep tracks of previous and next node if a neighbour hasn't been visited yet and isn't an obstacle, it's added to nontestednodes for later evaluation
    notTestedNodes.push_back(startNode);  // it starts from begining 

    while (!notTestedNodes.empty() && currentNode != endNode) { // the loop run as long as there are nodes to test and the end node hasn't been reached
        notTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->globalGoal < rhs->globalGoal; }); // as A* algorithm lowest estimated cost is calculated and is proceeded first 

        while (!notTestedNodes.empty() && notTestedNodes.front()->isVisited) {
            notTestedNodes.pop_front(); // skip if the node are already visited so it becomes computationally inexpensive
        }

        if (notTestedNodes.empty()) break; // if there is no more unique node to visit break 

        currentNode = notTestedNodes.front(); //Since the list is sorted based on the node with globalGoal value with lowest cost, the node at the front of the list is the one that appears to be closest to the goal and should be evaluated next
        currentNode->isVisited = true; 

        for (auto neighbor : currentNode->neighbors) {
            if (!neighbor->isVisited && !neighbor->isObstacle) {
                notTestedNodes.push_back(neighbor);  // if neighbour node hasn't been visited and isn't an obstacle, it's added to notTestedNodes
            }

            float possiblyLowerGoal = currentNode->localGoal + currentNode->distanceTo(neighbor); // since A* needs to value one heuristic cost and actual cose i.e f(n) = g(n) + h(n)

            if (possiblyLowerGoal < neighbor->localGoal) {
                neighbor->parent = currentNode;  // Sets the neighbor's parent to the current node. This helps in reconstructing the path later. The algorithm may later find a better path, but this step establishes the current best known path to the neighbor
                neighbor->localGoal = possiblyLowerGoal;// if the new cost is less then neigbour current node actual cost i.e localgoal, the algorithm has found a better path to the neighbour and localgoal is updated to the new, lower value 
                neighbor->globalGoal = neighbor->localGoal + neighbor->heuristicTo(endNode);// the neihbour actual cost is updated by adding the heuristic estimate to the end node
            }
        }
    }
}

void Pathfinding::handleEvent(sf::Event event) 
{
    // this will toggle when D is pressed allowing path to visit diagonally in all 8 direction 
    if (event.type == sf::Event::MouseButtonReleased) {
        int x = event.mouseButton.x / nodeSize;
        int y = event.mouseButton.y / nodeSize;
        temp_x = x; temp_y = y; 

        if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // this will take starting node and ending node to the position of mouse co-ordinates
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                    startNode = &nodes[y * mapWidth + x];
                    std::cout << x << " " << y << std::endl; 
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    endNode = &nodes[y * mapWidth + x];
                }
                else {
                    nodes[y * mapWidth + x].isObstacle = !nodes[y * mapWidth + x].isObstacle; // to place and remove the obstacle 
                }
                solveAStar();
            }
        }
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::D) { 
            includeDiagonalsFlag = !includeDiagonalsFlag;  // Toggle the flag
            Dpressed = true;
            updateNeigboursNode();  // Update neighbors immediately after    
            solveAStar();
        }
    }
}

void Pathfinding::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    sf::RectangleShape rect(sf::Vector2f(nodeSize, nodeSize));
    rect.setFillColor(sf::Color::Black); // Set background color to black
    target.clear(sf::Color(300, 300, 300)); // Clear the window with a black background
    
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            const Node& node = nodes[y * mapWidth + x];
            rect.setPosition(x * nodeSize, y * nodeSize);
            if (node.isObstacle) {
                rect.setFillColor(sf::Color(255, 165, 0, 250));
            }
            else if (&node == startNode) {
                rect.setFillColor(sf::Color::Green);
            }
            else if (&node == endNode) {
                rect.setFillColor(sf::Color::Red);
            }
            else if (node.isVisited) {
                rect.setFillColor(sf::Color(0, 255, 0, 20));
            }
            else {
                rect.setFillColor(sf::Color::Black);
            }
            target.draw(rect, states);
            textManager.drawTexts(target); // drawing text 
        }
    }

    // Draw path with small squares
    sf::RectangleShape pathSquare(sf::Vector2f(nodeSize * 0.5f, nodeSize * 0.5f)); // Small square
    pathSquare.setFillColor(sf::Color(255, 255, 0, 128)); // Semi-transparent yellow

    if (endNode != nullptr) {
        Node* p = endNode;
        while (p->parent != nullptr) {
            pathSquare.setPosition(p->x * nodeSize + nodeSize * 0.25f, p->y * nodeSize + nodeSize * 0.25f); // this will trace the path from start to end with glowing yellow box
            target.draw(pathSquare, states);
            p = p->parent;
        }
        pathSquare.setPosition(startNode->x * nodeSize + nodeSize * 0.25f, startNode->y * nodeSize + nodeSize * 0.25f);
        target.draw(pathSquare, states);
    }

    sf::VertexArray path(sf::LinesStrip);
    if (endNode != nullptr) {
        Node* p = endNode;
        while (p->parent != nullptr) {
            path.append(sf::Vertex(sf::Vector2f(p->x * nodeSize + nodeSize / 2, p->y * nodeSize + nodeSize / 2), sf::Color::Yellow));
            p = p->parent; // this will calculate the end destination and draw the line from each node to node to end node
        }
        path.append(sf::Vertex(sf::Vector2f(startNode->x * nodeSize + nodeSize / 2, startNode->y * nodeSize + nodeSize / 2), sf::Color::Yellow));
    }
    target.draw(path, states); 
}

void Pathfinding::drawnode(sf::RenderWindow& window)
{
    // this will draw a low fade yellow color box in grid which is grid itself but the size id halfed 
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            window.draw(rectangles[y][x]); 
        }
    }
}

