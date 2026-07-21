#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    bool fontLoaded = false;
    
    std::vector<sf::Text> mainButtons;
    std::vector<sf::RectangleShape> mainRects;
    int selectedButton = -1;
    
    std::vector<sf::Text> modeButtons;
    std::vector<sf::RectangleShape> modeRects;
    int selectedMode = -1;
    
    std::unique_ptr<sf::Text> exitText;
    sf::RectangleShape exitRect;
    bool exitHovered = false;
    
    std::unique_ptr<sf::Text> toggleText1;
    std::unique_ptr<sf::Text> toggleText2;
    sf::RectangleShape toggleBg;
    sf::RectangleShape toggleKnob;
    bool isPvE = false;
    bool toggleHovered = false;
    
    sf::RectangleShape tasksButton;
    std::unique_ptr<sf::Text> tasksText;
    bool tasksHovered = false;
    
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