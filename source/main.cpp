#include <SFML/Graphics.hpp>
#include "../header/Pathfinding.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 400), "AStar Pathfinding");
    Pathfinding pathfinding(20, 20);
    pathfinding.updateNeigboursNode(); 


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            pathfinding.handleEvent(event);
        }

        window.clear(sf::Color::Black);
        window.draw(pathfinding);
        pathfinding.drawnode(window);
        window.display();
    }

    return 0;
}
