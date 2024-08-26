#include "../header/Pathfinding.h"

Pathfinding::Pathfinding(int width, int height) : mapWidth(width), mapHeight(height) {
    inittext(); 
    nodes.reserve(mapWidth * mapHeight);
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            nodes.emplace_back(x, y);
        }
    }

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            Node& node = nodes[y * mapWidth + x];
            if (y > 0) node.neighbors.push_back(&nodes[(y - 1) * mapWidth + x]);
            if (y < mapHeight - 1) node.neighbors.push_back(&nodes[(y + 1) * mapWidth + x]);
            if (x > 0) node.neighbors.push_back(&nodes[y * mapWidth + (x - 1)]);
            if (x < mapWidth - 1) node.neighbors.push_back(&nodes[y * mapWidth + (x + 1)]);
        }
    }

    startNode = &nodes[(mapHeight / 2) * mapWidth + 1];
    endNode = &nodes[(mapHeight / 2) * mapWidth + mapWidth - 2];
}

void Pathfinding::solveAStar() {
    for (auto& node : nodes) {
        node.isVisited = false;
        node.globalGoal = INFINITY;
        node.localGoal = INFINITY;
        node.parent = nullptr;
    }

    Node* currentNode = startNode;
    startNode->localGoal = 0.0f;
    startNode->globalGoal = startNode->heuristicTo(endNode);

    std::list<Node*> notTestedNodes;
    notTestedNodes.push_back(startNode);

    while (!notTestedNodes.empty() && currentNode != endNode) {
        notTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->globalGoal < rhs->globalGoal; });

        while (!notTestedNodes.empty() && notTestedNodes.front()->isVisited) {
            notTestedNodes.pop_front();
        }

        if (notTestedNodes.empty()) break;

        currentNode = notTestedNodes.front();
        currentNode->isVisited = true;

        for (auto neighbor : currentNode->neighbors) {
            if (!neighbor->isVisited && !neighbor->isObstacle) {
                notTestedNodes.push_back(neighbor);
            }

            float possiblyLowerGoal = currentNode->localGoal + currentNode->distanceTo(neighbor);

            if (possiblyLowerGoal < neighbor->localGoal) {
                neighbor->parent = currentNode;
                neighbor->localGoal = possiblyLowerGoal;
                neighbor->globalGoal = neighbor->localGoal + neighbor->heuristicTo(endNode);
            }
        }
    }
}

void Pathfinding::handleEvent(sf::Event event) {
    if (event.type == sf::Event::MouseButtonReleased) {
        int x = event.mouseButton.x / nodeSize;
        int y = event.mouseButton.y / nodeSize;

        if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                    startNode = &nodes[y * mapWidth + x];
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    endNode = &nodes[y * mapWidth + x];
                }
                else {
                    nodes[y * mapWidth + x].isObstacle = !nodes[y * mapWidth + x].isObstacle;
                }

                solveAStar();
            }
        }
    }
}

void Pathfinding::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape rect(sf::Vector2f(nodeSize, nodeSize));
    rect.setFillColor(sf::Color::Black); // Set background color to black
    target.clear(sf::Color(300, 300, 300)); // Clear the window with a black background

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            const Node& node = nodes[y * mapWidth + x];
            rect.setPosition(x * nodeSize, y * nodeSize);
            if (node.isObstacle) {
                rect.setFillColor(sf::Color::Cyan);
            }
            else if (&node == startNode) {
                rect.setFillColor(sf::Color::Green);
            }
            else if (&node == endNode) {
                rect.setFillColor(sf::Color::Red);
            }
            else if (node.isVisited) {
                rect.setFillColor(sf::Color(20, 20, 20));
            }
            else {
                rect.setFillColor(sf::Color::Black);
            }
            target.draw(rect, states);
        }
    }

    sf::VertexArray path(sf::LinesStrip);
    if (endNode != nullptr) {
        Node* p = endNode;
        while (p->parent != nullptr) {
            path.append(sf::Vertex(sf::Vector2f(p->x * nodeSize + nodeSize / 2, p->y * nodeSize + nodeSize / 2), sf::Color::Yellow));
            p = p->parent;
        }
        path.append(sf::Vertex(sf::Vector2f(startNode->x * nodeSize + nodeSize / 2, startNode->y * nodeSize + nodeSize / 2), sf::Color::Yellow));
    }
    target.draw(path, states);
    target.draw(text, states); 
    target.draw(text2, states);
    target.draw(textGreen, states);
    target.draw(textRed, states); 
}


void Pathfinding::inittext()
{
    if (!font.loadFromFile("Assets/Font/Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    if (!font2.loadFromFile("Assets/Font/Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    text.setFont(font);
    text.setString("To Block &\nUnblock the path:\n-Mouse leftclick");
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setPosition(405.f, 150.f);


    textGreen.setFont(font);
    textGreen.setString("Green: Starting Node");
    textGreen.setCharacterSize(18);
    textGreen.setFillColor(sf::Color::Green);
    textGreen.setPosition(405.f, 50.f);


    textRed.setFont(font);
    textRed.setString("Red: Ending Node");
    textRed.setCharacterSize(18);
    textRed.setFillColor(sf::Color::Red);
    textRed.setPosition(405.f, 80.f);

    text2.setFont(font2);
    text2.setString("Gray: Total path visited");
    text2.setCharacterSize(18);
    text2.setFillColor(sf::Color::White);
    text2.setPosition(405.f, 110.f);

}
