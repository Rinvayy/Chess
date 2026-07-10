#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

// ============================================================================
// ШАХМАТНАЯ ДОСКА (ГРАФИЧЕСКАЯ ВЕРСИЯ)
// ============================================================================

class ChessBoard {
private:
    static const int SIZE = 8;
    static const int CELL_SIZE = 80;
    static const int BOARD_SIZE = SIZE * CELL_SIZE;
    
    sf::RenderWindow window;
    std::vector<std::vector<char>> board;
    bool isWhiteTurn;
    
public:
    ChessBoard() : window(sf::VideoMode({BOARD_SIZE, BOARD_SIZE + 100}), "Шахматы"), isWhiteTurn(true) {
        board = {
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
        };
    }
    
    void run() {
    while (window.isOpen()) {
        // Обработка событий
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mousePos = event->getIf<sf::Event::MouseButtonPressed>();
                if (mousePos && mousePos->button == sf::Mouse::Button::Left) {
                    // В SFML 3.0.0 координаты хранятся в структуре position
                    int x = mousePos->position.x;
                    int y = mousePos->position.y;
                    handleClick(x, y);
                }
            }
        }
        
        // Отрисовка
        window.clear(sf::Color::White);
        drawBoard();
        drawPieces();
        drawTurnInfo();
        window.display();
    }
}
    
private:
    void drawBoard() {
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                sf::RectangleShape cell(sf::Vector2f({(float)CELL_SIZE, (float)CELL_SIZE}));
                cell.setPosition({(float)(col * CELL_SIZE), (float)(row * CELL_SIZE)});
                
                if ((row + col) % 2 == 0) {
                    cell.setFillColor(sf::Color(240, 217, 181));
                } else {
                    cell.setFillColor(sf::Color(181, 136, 99));
                }
                window.draw(cell);
            }
        }
    }
    
    void drawPieces() {
        sf::Font font;
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            // Если шрифт не загрузился, пропускаем
            return;
        }
        
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                char piece = board[row][col];
                if (piece != ' ') {
                    sf::Text text(font);
                    text.setString(std::string(1, piece));
                    text.setCharacterSize(60);
                    text.setStyle(sf::Text::Bold);
                    
                    if (piece >= 'A' && piece <= 'Z') {
                        text.setFillColor(sf::Color::White);
                    } else {
                        text.setFillColor(sf::Color::Black);
                    }
                    
                    float x = col * CELL_SIZE + CELL_SIZE / 2 - 25;
                    float y = row * CELL_SIZE + CELL_SIZE / 2 - 35;
                    text.setPosition({x, y});
                    
                    window.draw(text);
                }
            }
        }
    }
    
    void drawTurnInfo() {
        sf::Font font;
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            return;
        }
        
        sf::Text text(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        text.setPosition({10, (float)(BOARD_SIZE + 10)});
        
        std::string turn = isWhiteTurn ? "Ход: Белые" : "Ход: Черные";
        text.setString(turn);
        window.draw(text);
    }
    
    void handleClick(int x, int y) {
        int col = x / CELL_SIZE;
        int row = y / CELL_SIZE;
        
        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
            std::cout << "Клик на клетку: " << (char)('a' + col) << (8 - row) << std::endl;
        }
    }
};

// ============================================================================
// ГЛАВНАЯ ФУНКЦИЯ
// ============================================================================

int main() {
    ChessBoard game;
    game.run();
    return 0;
}