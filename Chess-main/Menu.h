#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    bool fontLoaded = false;
    
    std::vector<sf::Text> mainButtons;
    std::vector<sf::RectangleShape> mainRects;
    int selectedButton = -1;
    
    sf::Text* exitText = nullptr;
    sf::RectangleShape exitRect;
    bool exitHovered = false;
    
    sf::Text* toggleText1 = nullptr;
    sf::Text* toggleText2 = nullptr;
    sf::RectangleShape toggleBg;
    sf::RectangleShape toggleKnob;
    bool isPvE = false;
    bool toggleHovered = false;
    
    std::vector<sf::Text> modeButtons;
    std::vector<sf::RectangleShape> modeRects;
    int selectedMode = 0;
    bool modeHovered = false;
    
public:
    Menu(sf::RenderWindow& win);
    ~Menu();
    
    void draw();
    void handleClick(int x, int y);
    void handleMouseMove(int x, int y);
    int getSelected() const;
    void resetSelection();
    bool getIsPvE() const { return isPvE; }
    int getGameMode() const { return selectedMode; }
};

#endif