#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "../header/Node.h"

class Pathfinding : public sf::Drawable {
public:
    Pathfinding(int width, int height);

    void solveAStar();
    void handleEvent(sf::Event event);
    void updateNeigboursNode(); 
    void drawnode(sf::RenderWindow& window);
    //void includeDiagonals();
    bool includeDiagonalsFlag = true;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::vector<Node> nodes;
    Node* startNode;
    Node* endNode;
    int mapWidth;
    int mapHeight;
    static const int nodeSize = 20;
    std::vector<std::vector<sf::RectangleShape>> rectangles;

    sf::Text text, text2;
    sf::Text textGreen, textRed, textGray;
    sf::Font font, font2;
    sf::RectangleShape rectangle1;
    sf::RectangleShape rectangle2;

    // function 
    void inittext(); 

};

#endif 
