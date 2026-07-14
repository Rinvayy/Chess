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
    
    //главные кнопки
    std::vector<sf::Text> mainButtons;
    std::vector<sf::RectangleShape> mainRects;
    int selectedButton = -1;
    
    //кнопка выхода
    sf::Text* exitText = nullptr;
    sf::RectangleShape exitRect;
    bool exitHovered = false;
    
    //переключатель
    sf::Text* toggleText1 = nullptr;
    sf::Text* toggleText2 = nullptr;
    sf::RectangleShape toggleBg;
    sf::RectangleShape toggleKnob;
    bool isPvE = false;
    bool toggleHovered = false;
    
public:
    Menu(sf::RenderWindow& win);
    ~Menu();
    
    void draw();
    void handleClick(int x, int y);
    void handleMouseMove(int x, int y);
    int getSelected() const;
    void resetSelection();
    bool getIsPvE() const { return isPvE; }
};

#endif