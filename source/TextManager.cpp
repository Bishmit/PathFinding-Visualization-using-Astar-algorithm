#include "../header/TextManager.h"

TextManager::TextManager() {
    loadFonts();
    setTexts();
}

void TextManager::loadFonts() {
    if (!font.loadFromFile("Assets/Font/Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    if (!font2.loadFromFile("Assets/Font/Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
}

void TextManager::setTexts() {
    text.setFont(font);
    text.setString("To Block &\nUnblock the path:\n-Mouse leftclick");
    text.setCharacterSize(18);  
    text.setFillColor(sf::Color::White);
    text.setOutlineThickness(1);  // thin outline
    text.setOutlineColor(sf::Color::Black);  // Black outline for contrast for clarity
    text.setPosition(405.f, 325.f);

    textGreen.setFont(font);
    textGreen.setString("Green: Starting Node");
    textGreen.setCharacterSize(18);
    textGreen.setFillColor(sf::Color::Green);
    textGreen.setOutlineThickness(1);
    textGreen.setOutlineColor(sf::Color::Black);
    textGreen.setPosition(405.f, 20.f);

    textRed.setFont(font);
    textRed.setString("Red: Ending Node");
    textRed.setCharacterSize(18);
    textRed.setFillColor(sf::Color::Red);
    textRed.setOutlineThickness(1);
    textRed.setOutlineColor(sf::Color::Black);
    textRed.setPosition(405.f, 60.f);

    text2.setFont(font2);
    text2.setString("Gray: Total path visited\n\nLshift: Relocate the star\nting node\n\nLctrl: Relocate the end\ning node\n\nTo include/exclude \ndiagonal path: D");
    text2.setCharacterSize(18);
    text2.setFillColor(sf::Color::White);
    text2.setOutlineThickness(1);
    text2.setOutlineColor(sf::Color::Black);
    text2.setPosition(405.f, 100.f);
}


void TextManager::drawTexts(sf::RenderTarget& target) const {
    target.draw(text);
    target.draw(textGreen);
    target.draw(textRed);
    target.draw(text2);
}
