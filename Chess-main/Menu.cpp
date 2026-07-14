#include "Menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& win) : window(win) {
    if (!font.openFromFile("font.ttf")) {
        std::cout << "ERROR: font.ttf not loaded!\n";
        fontLoaded = false;
        return;
    }
    fontLoaded = true;
    
    //кнопка играть
    std::vector<std::string> mainNames = {"Играть"};
    float y = 250;
    
    for (size_t i = 0; i < mainNames.size(); ++i) {
        sf::Text text(font);
        std::string playStr = mainNames[i];
        text.setString(sf::String::fromUtf8(playStr.begin(), playStr.end()));
        text.setCharacterSize(48);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        
        float textWidth = text.getLocalBounds().size.x;
        float textHeight = text.getLocalBounds().size.y;
        
        sf::RectangleShape rect(sf::Vector2f({300, 70}));
        rect.setFillColor(sf::Color(139, 69, 19));
        rect.setOutlineColor(sf::Color(100, 50, 20));
        rect.setOutlineThickness(2);
        
        float rectX = (800 - 300) / 2;
        float rectY = y;
        rect.setPosition({rectX, rectY});
        
        float textX = rectX + (300 - textWidth) / 2;
        float textY = rectY + (70 - textHeight) / 2 - 10;
        text.setPosition({textX, textY});
        
        mainButtons.push_back(text);
        mainRects.push_back(rect);
        y += 100;
    }
    
    //кнопка выхода
    exitText = new sf::Text(font);
    std::string exitStr = "Выход";
    exitText->setString(sf::String::fromUtf8(exitStr.begin(), exitStr.end()));
    exitText->setCharacterSize(20);
    exitText->setFillColor(sf::Color::White);
    exitText->setStyle(sf::Text::Bold);
    
    float exitTextWidth = exitText->getLocalBounds().size.x;
    float exitTextHeight = exitText->getLocalBounds().size.y;
    
    float exitRectWidth = exitTextWidth + 30;
    float exitRectHeight = exitTextHeight + 20;
    float exitRectX = 800 - exitRectWidth - 20;
    float exitRectY = 600 - exitRectHeight - 20;
    
    exitRect.setSize(sf::Vector2f({exitRectWidth, exitRectHeight}));
    exitRect.setFillColor(sf::Color(139, 30, 20)); 
    exitRect.setOutlineColor(sf::Color(100, 20, 10));
    exitRect.setOutlineThickness(2);
    exitRect.setPosition({exitRectX, exitRectY});
    
    float exitX = exitRectX + (exitRectWidth - exitTextWidth) / 2;
    float exitY = exitRectY + (exitRectHeight - exitTextHeight) / 2 - 2;
    exitText->setPosition({exitX, exitY});
    
    //переключатель режима игры
    sf::RectangleShape modeFrame;
    modeFrame.setSize(sf::Vector2f({460, 90}));
    modeFrame.setFillColor(sf::Color(180, 160, 120));
    modeFrame.setOutlineColor(sf::Color(80, 40, 20));
    modeFrame.setOutlineThickness(2);
    modeFrame.setPosition({175, 378});
    
    toggleBg.setSize(sf::Vector2f({120, 50}));
    toggleBg.setFillColor(sf::Color(100, 70, 40));
    toggleBg.setOutlineColor(sf::Color(80, 50, 30));
    toggleBg.setOutlineThickness(2);
    toggleBg.setPosition({340, 400});
    
    toggleKnob.setSize(sf::Vector2f({40, 40}));
    toggleKnob.setFillColor(sf::Color(200, 150, 100));
    toggleKnob.setPosition({345, 405});
    
    toggleText1 = new sf::Text(font);
    std::string text1 = "На двоих";
    toggleText1->setString(sf::String::fromUtf8(text1.begin(), text1.end()));
    toggleText1->setCharacterSize(18);
    toggleText1->setFillColor(sf::Color(150, 100, 50));
    toggleText1->setStyle(sf::Text::Bold);
    toggleText1->setPosition({220, 410});
    
    toggleText2 = new sf::Text(font);
    std::string text2 = "С компьютером";
    toggleText2->setString(sf::String::fromUtf8(text2.begin(), text2.end()));
    toggleText2->setCharacterSize(18);
    toggleText2->setFillColor(sf::Color(120, 70, 30));
    toggleText2->setStyle(sf::Text::Bold);
    toggleText2->setPosition({470, 410});
}

Menu::~Menu() {
    delete exitText;
    delete toggleText1;
    delete toggleText2;
}

void Menu::draw() {
    window.clear(sf::Color(220, 200, 160));
    
    if (!fontLoaded) {
        for (size_t i = 0; i < mainRects.size(); ++i) {
            if (selectedButton == (int)i) {
                mainRects[i].setFillColor(sf::Color(160, 82, 45));
            } else {
                mainRects[i].setFillColor(sf::Color(139, 69, 19));
            }
            window.draw(mainRects[i]);
        }
        window.display();
        return;
    }
    
    sf::Text title(font);
    std::string titleStr = "Шахматы";
    title.setString(sf::String::fromUtf8(titleStr.begin(), titleStr.end()));
    title.setCharacterSize(72);
    title.setFillColor(sf::Color(100, 50, 20));
    title.setStyle(sf::Text::Bold);
    float titleX = (800 - title.getLocalBounds().size.x) / 2;
    title.setPosition({titleX, 80});
    window.draw(title);
    
    for (size_t i = 0; i < mainButtons.size(); ++i) {
        if (selectedButton == (int)i) {
            mainRects[i].setFillColor(sf::Color(160, 82, 45));
        } else {
            mainRects[i].setFillColor(sf::Color(139, 69, 19));
        }
        window.draw(mainRects[i]);
        window.draw(mainButtons[i]);
    }
    
    if (exitHovered) {
        exitRect.setFillColor(sf::Color(180, 50, 30));  //кнопка выхода при наведении мыши
    } else {
        exitRect.setFillColor(sf::Color(139, 30, 20));
    }
    window.draw(exitRect);
    window.draw(*exitText);
    
    //рамка для переключателя режима игры
    sf::RectangleShape modeFrame;
    modeFrame.setSize(sf::Vector2f({460, 90}));
    modeFrame.setFillColor(sf::Color(180, 160, 120));
    modeFrame.setOutlineColor(sf::Color(80, 40, 20));
    modeFrame.setOutlineThickness(2);
    modeFrame.setPosition({175, 378});
    window.draw(modeFrame);
    
    window.draw(toggleBg);
    
    if (isPvE) {
        toggleText1->setFillColor(sf::Color(150, 100, 50));
        toggleText2->setFillColor(sf::Color::Black);
        toggleKnob.setPosition({345 + 70, 405});
    } else {
        toggleText1->setFillColor(sf::Color::Black);
        toggleText2->setFillColor(sf::Color(120, 70, 30));
        toggleKnob.setPosition({345, 405});
    }
    
    window.draw(*toggleText1);
    window.draw(*toggleText2);
    window.draw(toggleKnob);
    
    window.display();
}

void Menu::handleClick(int x, int y) {
    for (size_t i = 0; i < mainRects.size(); ++i) {
        sf::FloatRect rect = mainRects[i].getGlobalBounds();
        if (rect.contains({(float)x, (float)y})) {
            selectedButton = i;
            return;
        }
    }
    
    if (exitRect.getGlobalBounds().contains({(float)x, (float)y})) {
        selectedButton = 2;
        return;
    }
    
    if (toggleBg.getGlobalBounds().contains({(float)x, (float)y})) {
        isPvE = !isPvE;
        std::cout << (isPvE ? "Mode: Against the computer" : "Mode: Two Players") << std::endl;
        selectedButton = -1;
        return;
    }
    
    selectedButton = -1;
}

void Menu::handleMouseMove(int x, int y) {
    bool found = false;
    for (size_t i = 0; i < mainRects.size(); ++i) {
        sf::FloatRect rect = mainRects[i].getGlobalBounds();
        if (rect.contains({(float)x, (float)y})) {
            selectedButton = i;
            found = true;
            break;
        }
    }
    if (found) return;
    
    if (exitRect.getGlobalBounds().contains({(float)x, (float)y})) {
        exitHovered = true;
        selectedButton = -1;
        return;
    } else {
        exitHovered = false;
    }
    
    if (toggleBg.getGlobalBounds().contains({(float)x, (float)y})) {
        toggleHovered = true;
        selectedButton = -1;
        return;
    } else {
        toggleHovered = false;
    }
    
    selectedButton = -1;
}

int Menu::getSelected() const {
    return selectedButton;
}

void Menu::resetSelection() {
    selectedButton = -1;
    exitHovered = false;
    toggleHovered = false;
}