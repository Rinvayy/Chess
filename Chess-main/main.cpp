#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Menu.h"
#include "ChessBoard.h"

int main() {
    std::cout << "Launching the program\n";
    
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Chess - Menu");
    Menu menu(window);
    
    bool inMenu = true;
    std::unique_ptr<ChessBoard> game;
    
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (inMenu) {
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    auto mousePos = event->getIf<sf::Event::MouseButtonPressed>();
                    if (mousePos && mousePos->button == sf::Mouse::Button::Left) {
                        menu.handleClick(mousePos->position.x, mousePos->position.y);
                        int selected = menu.getSelected();
                        
                        if (selected == 0) {
                            //играть
                            inMenu = false;
                            menu.resetSelection();
                            window.close();
                            
                            game = std::make_unique<ChessBoard>();
                            
                            //запуск игры
                            game->run();
                            
                            window.create(sf::VideoMode({800, 600}), "Chess - Menu");
                            inMenu = true;
                        } else if (selected == 1) {
                            std::cout << "Use the switch below\n";
                            menu.resetSelection();
                        } else if (selected == 2) {
                            //выход
                            window.close();
                        }
                    }
                }
                
                if (event->is<sf::Event::MouseMoved>()) {
                    auto mousePos = event->getIf<sf::Event::MouseMoved>();
                    if (mousePos) {
                        menu.handleMouseMove(mousePos->position.x, mousePos->position.y);
                    }
                }
            }
        }
        
        if (inMenu) {
            menu.draw();
        }
    }
    return 0;
}