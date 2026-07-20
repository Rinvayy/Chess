#include "ChessBoard.h"
#include <iostream>
#include <SFML/Graphics.hpp>

ChessBoard::ChessBoard(GameMode mode, bool aiMode, Color playerColor) 
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Chess"),
      ai(&board),
      gameMode(mode),
      isAIMode(aiMode),
      playerColor(playerColor),
      hasSelected(false) {
    
    if (aiMode) {
        aiColor = (playerColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
        humanColor = playerColor;
    } else {
        humanColor = Color::WHITE;
        aiColor = Color::BLACK;
    }
    
    if (!font.openFromFile("font.ttf")) {
        std::cout << "Font not loaded!\n";
    }
    
    if (!pieceRenderer.loadTextures()) {
        std::cout << "Failed to load piece textures\n";
    }
    
    //кнопка переключения цвета
    switchColorButton.setSize(sf::Vector2f({140, 30}));
    switchColorButton.setFillColor(sf::Color(80, 40, 20));
    switchColorButton.setOutlineColor(sf::Color(60, 30, 10));
    switchColorButton.setOutlineThickness(2);
    switchColorButton.setPosition({(float)(BOARD_SIZE + 30), 310});
    
    switchColorText = new sf::Text(font);
    std::string btnText = "Сменить цвет";
    switchColorText->setString(sf::String::fromUtf8(btnText.begin(), btnText.end()));
    switchColorText->setCharacterSize(14);
    switchColorText->setFillColor(sf::Color::White);
    switchColorText->setStyle(sf::Text::Bold);
    
    float textX = BOARD_SIZE + 30 + (140 - switchColorText->getLocalBounds().size.x) / 2;
    float textY = 310 + (30 - switchColorText->getLocalBounds().size.y) / 2 - 2;
    switchColorText->setPosition({(float)textX, (float)textY});
    
    //устанавливаем начальную позицию
    board.resetBoard(Color::WHITE);
    
    std::cout << "You are playing as: " << (playerColor == Color::WHITE ? "White" : "Black") << "\n";
    std::cout << "Mode: " << (aiMode ? "vs AI" : "Two players") << "\n";
    
    std::ifstream saveFile("save.fen");
    if (saveFile.good()) {
        saveFile.close();
        std::cout << "Saved game found. Loading...\n";
        FEN::loadGame("save.fen", board);
    }
}

ChessBoard::~ChessBoard() {
    delete switchColorText;
}

void ChessBoard::run() {
    while (window.isOpen()) {
        if (isAIMode && board.getCurrentPlayer() == aiColor) {
            sf::sleep(sf::milliseconds(300));
            Move bestMove = ai.getBestMove(aiColor);
            if (bestMove.fromRow != -1) {
                board.makeMove(bestMove.fromRow, bestMove.fromCol, 
                             bestMove.toRow, bestMove.toCol);
                checkGameEnd();
            }
        }
        
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                FEN::saveGame(board, "save.fen");
                window.close();
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mousePos = event->getIf<sf::Event::MouseButtonPressed>();
                if (mousePos && mousePos->button == sf::Mouse::Button::Left) {
                    if (switchColorButton.getGlobalBounds().contains(
                            {(float)mousePos->position.x, (float)mousePos->position.y})) {
                        handleSwitchColorClick();
                    } else {
                        handleClick(mousePos->position.x, mousePos->position.y);
                    }
                }
            }
            
            if (event->is<sf::Event::MouseMoved>()) {
                auto mousePos = event->getIf<sf::Event::MouseMoved>();
                if (mousePos) {
                    isSwitchColorHovered = switchColorButton.getGlobalBounds().contains(
                        {(float)mousePos->position.x, (float)mousePos->position.y});
                }
            }
            
            if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();
                
                if (key && key->scancode == sf::Keyboard::Scancode::S && key->control) {
                    FEN::saveGame(board, "save.fen");
                    std::cout << "Game saved!\n";
                }
                
                if (key && key->scancode == sf::Keyboard::Scancode::L && key->control) {
                    if (FEN::loadGame("save.fen", board)) {
                        std::cout << "Game loaded!\n";
                        hasSelected = false;
                    }
                }
                
                if (key && key->scancode == sf::Keyboard::Scancode::A) {
                    isAIMode = !isAIMode;
                    std::cout << "AI mode: " << (isAIMode ? "ON" : "OFF") << "\n";
                }
                
                if (key && key->scancode == sf::Keyboard::Scancode::R) {
                    board = Board();
                    hasSelected = false;
                    std::cout << "New game!\n";
                }
            }
        }
        
        window.clear(sf::Color(30, 30, 30));
        drawBoard();
        drawPieces();
        drawInfoPanel();
        drawSwitchColorButton();
        window.display();
    }
}

void ChessBoard::switchPlayerColor() {
    playerColor = (playerColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    if (isAIMode) {
        aiColor = (playerColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }
    
    board.resetBoard(Color::WHITE);
    hasSelected = false;
    
    std::cout << "Color changed! You are now playing as: " 
              << (playerColor == Color::WHITE ? "White" : "Black") << "\n";
}

void ChessBoard::handleSwitchColorClick() {
    switchPlayerColor();
}

void ChessBoard::drawSwitchColorButton() {
    if (isSwitchColorHovered) {
        switchColorButton.setFillColor(sf::Color(120, 60, 30));
    } else {
        switchColorButton.setFillColor(sf::Color(80, 40, 20));
    }
    window.draw(switchColorButton);
    window.draw(*switchColorText);
}

void ChessBoard::drawBoard() {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            sf::RectangleShape cell(sf::Vector2f({(float)CELL_SIZE, (float)CELL_SIZE}));
            cell.setPosition({(float)(col * CELL_SIZE + 10), (float)(row * CELL_SIZE + 10)});
            
            sf::Color color;
            if ((row + col) % 2 == 0) {
                color = sf::Color(240, 217, 181);
            } else {
                color = sf::Color(181, 136, 99);
            }
            
            if (hasSelected && selectedRow == row && selectedCol == col) {
                color = sf::Color(255, 255, 100);
            }
            
            cell.setFillColor(color);
            cell.setOutlineColor(sf::Color(100, 100, 100));
            cell.setOutlineThickness(1);
            window.draw(cell);
            drawCoordinate(row, col);
        }
    }
}

void ChessBoard::drawCoordinate(int row, int col) {
    sf::Text text(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color(100, 100, 100));
    
    if (row == 7) {
        char letter = 'a' + col;
        text.setString(std::string(1, letter));
        float x = col * CELL_SIZE + CELL_SIZE / 2 - 8 + 10;
        float y = row * CELL_SIZE + CELL_SIZE - 20 + 10;
        text.setPosition({x, y});
        window.draw(text);
    }
    
    if (col == 0) {
        text.setString(std::to_string(8 - row));
        float x = col * CELL_SIZE + 2 + 10;
        float y = row * CELL_SIZE + CELL_SIZE / 2 - 12 + 10;
        text.setPosition({x, y});
        window.draw(text);
    }
}

void ChessBoard::drawPieces() {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            Figure fig = board.getFigure(row, col);
            if (!fig.isEmpty()) {
                float x = col * CELL_SIZE + 10;
                float y = row * CELL_SIZE + 10;
                
                pieceRenderer.drawPiece(window, fig, x, y, CELL_SIZE);
            }
        }
    }
}

void ChessBoard::drawInfoPanel() {
    sf::RectangleShape panel(sf::Vector2f({180, WINDOW_HEIGHT - 20}));
    panel.setPosition({(float)BOARD_SIZE + 20, 10});
    panel.setFillColor(sf::Color(50, 50, 50, 200));
    panel.setOutlineColor(sf::Color(100, 100, 100));
    panel.setOutlineThickness(1);
    window.draw(panel);
    
    int yPos = 30;
    
    sf::Text title(font);
    title.setString("INFO");
    title.setCharacterSize(20);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition({(float)BOARD_SIZE + 55, (float)yPos});
    window.draw(title);
    yPos += 40;
    
    sf::Text turnText(font);
    turnText.setString("Turn: " + std::string(board.getCurrentPlayer() == Color::WHITE ? "White" : "Black"));
    turnText.setCharacterSize(16);
    turnText.setFillColor(sf::Color(200, 200, 200));
    turnText.setPosition({(float)BOARD_SIZE + 30, (float)yPos});
    window.draw(turnText);
    yPos += 30;
    
    sf::Text aiText(font);
    aiText.setString("AI: " + std::string(isAIMode ? "ON" : "OFF"));
    aiText.setCharacterSize(16);
    aiText.setFillColor(isAIMode ? sf::Color::Green : sf::Color(200, 200, 200));
    aiText.setPosition({(float)BOARD_SIZE + 30, (float)yPos});
    window.draw(aiText);
    yPos += 40;
    
    sf::Text modeText(font);
    std::string modeStr;
    if (gameMode == GameMode::FISCHER) modeStr = "Fischer";
    else if (gameMode == GameMode::THREE_CHECKS) modeStr = "3 Checks";
    else modeStr = "Classic";
    modeText.setString("Mode: " + modeStr);
    modeText.setCharacterSize(14);
    modeText.setFillColor(sf::Color(200, 200, 200));
    modeText.setPosition({(float)BOARD_SIZE + 30, (float)yPos});
    window.draw(modeText);
    yPos += 30;
    
    if (board.isInCheck(board.getCurrentPlayer())) {
        sf::Text checkText(font);
        checkText.setString("CHECK!");
        checkText.setCharacterSize(18);
        checkText.setFillColor(sf::Color::Red);
        checkText.setStyle(sf::Text::Bold);
        checkText.setPosition({(float)BOARD_SIZE + 45, (float)yPos});
        window.draw(checkText);
        yPos += 30;
    }
    
    sf::Text controls(font);
    controls.setString("CONTROLS:");
    controls.setCharacterSize(16);
    controls.setFillColor(sf::Color(200, 200, 200));
    controls.setStyle(sf::Text::Bold);
    controls.setPosition({(float)BOARD_SIZE + 30, (float)yPos});
    window.draw(controls);
    yPos += 25;
    
    std::vector<std::string> shortcuts = {
        "A - AI mode",
        "R - New game",
        "Ctrl+S - Save",
        "Ctrl+L - Load"
    };
    
    for (const auto& shortcut : shortcuts) {
        sf::Text text(font);
        text.setString(shortcut);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color(180, 180, 180));
        text.setPosition({(float)BOARD_SIZE + 35, (float)yPos});
        window.draw(text);
        yPos += 22;
    }
}

void ChessBoard::handleClick(int x, int y) {
    if (x < 10 || x > BOARD_SIZE + 10 || y < 10 || y > BOARD_SIZE + 10) return;
    
    int col = (x - 10) / CELL_SIZE;
    int row = (y - 10) / CELL_SIZE;
    
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) return;
    
    if (isAIMode && board.getCurrentPlayer() == aiColor) return;
    
    Figure fig = board.getFigure(row, col);
    
    if (!hasSelected) {
        if (!fig.isEmpty() && fig.color == board.getCurrentPlayer()) {
            selectedRow = row;
            selectedCol = col;
            hasSelected = true;
            std::cout << "Selected: " << (char)('a' + col) << (8 - row) << "\n";
        }
    } else {
        if (board.makeMove(selectedRow, selectedCol, row, col)) {
            hasSelected = false;
            checkGameEnd();
        } else {
            if (!fig.isEmpty() && fig.color == board.getCurrentPlayer()) {
                selectedRow = row;
                selectedCol = col;
            } else {
                hasSelected = false;
            }
        }
    }
}

void ChessBoard::checkGameEnd() {
    Color current = board.getCurrentPlayer();
    if (board.isCheckmate(current)) {
        std::cout << "CHECKMATE! " << (current == Color::WHITE ? "White" : "Black") << " loses!\n";
    } else if (board.isStalemate(current)) {
        std::cout << "STALEMATE! Draw!\n";
    } else if (board.isInCheck(current)) {
        std::cout << "CHECK! " << (current == Color::WHITE ? "White" : "Black") << " king is in check!\n";
    }
}