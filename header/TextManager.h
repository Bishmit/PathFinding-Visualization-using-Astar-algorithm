#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>

class TextManager {
public:
    TextManager();
    void loadFonts();
    void setTexts();
    void drawTexts(sf::RenderTarget& target) const;

private:
    sf::Font font;
    sf::Font font2;
    sf::Text text;
    sf::Text textGreen;
    sf::Text textRed;
    sf::Text text2;
};

#endif
