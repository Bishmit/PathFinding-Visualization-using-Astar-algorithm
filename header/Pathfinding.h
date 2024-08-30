#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "../header/Node.h"
#include "../header/TextManager.h"

class Pathfinding : public sf::Drawable {
public:
    Pathfinding(int width, int height);
    void handleEvent(sf::Event event);
    void updateNeigboursNode(); 
    void drawnode(sf::RenderWindow& window);
   
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::vector<Node> nodes;
    Node* startNode;
    Node* endNode;
    TextManager textManager; 
    int temp_x, temp_y;
    size_t mapWidth; 
    size_t mapHeight;
    static const int nodeSize = 20;
    std::vector<std::vector<sf::RectangleShape>> rectangles;

    sf::Text text, text2;
    sf::Text textGreen, textRed, textGray;
    sf::Font font, font2;
    sf::RectangleShape rectangle1;
    sf::RectangleShape rectangle2;
    bool includeDiagonalsFlag = true, Dpressed = false;

    // Private function 
    void solveAStar();
};

#endif 
