#include "ChessBoard.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

ChessBoard::ChessBoard(GameMode mode, bool pve) 
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Шахматы"),
      ai(&board), isAIMode(pve), 
      humanColor(Color::WHITE), aiColor(Color::BLACK),
      hasSelected(false), gameMode(mode), checkCount(0) {
    
    if (!font.openFromFile("font.ttf")) {
        std::cout << "Шрифт не загружен!\n";
    }
    
    if (gameMode == GameMode::FISCHER) {
        setupFischerBoard();
    } else {
        board = Board();
    }
    
    std::ifstream saveFile("save.fen");
    if (saveFile.good()) {
        saveFile.close();
        std::cout << "Найдена сохраненная игра. Загрузка...\n";
        FEN::loadGame("save.fen", board);
    }
}

void ChessBoard::setupFischerBoard() {
    srand(time(0));
    board = Board();
    
    std::vector<int> positions(8);
    for (int i = 0; i < 8; ++i) positions[i] = i;
    
    for (int i = 7; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(positions[i], positions[j]);
    }
    
    std::vector<FigureType> pieces = {
        FigureType::ROOK, FigureType::KNIGHT, FigureType::BISHOP, 
        FigureType::QUEEN, FigureType::KING, FigureType::BISHOP,
        FigureType::KNIGHT, FigureType::ROOK
    };
    
    for (int i = 7; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(pieces[i], pieces[j]);
    }
    
    bool bishopsOnDifferentColors = false;
    int attempts = 0;
    while (!bishopsOnDifferentColors && attempts < 100) {
        int bishop1 = -1, bishop2 = -1;
        for (int i = 0; i < 8; ++i) {
            if (pieces[i] == FigureType::BISHOP) {
                if (bishop1 == -1) bishop1 = i;
                else bishop2 = i;
            }
        }
        if (bishop1 != -1 && bishop2 != -1) {
            if ((bishop1 % 2) != (bishop2 % 2)) {
                bishopsOnDifferentColors = true;
            } else {
                for (int i = 0; i < 8; ++i) {
                    if (i != bishop1 && i != bishop2 && pieces[i] != FigureType::KING) {
                        std::swap(pieces[i], pieces[bishop1]);
                        break;
                    }
                }
            }
        }
        attempts++;
    }
    
    int kingPos = -1;
    int rook1 = -1, rook2 = -1;
    for (int i = 0; i < 8; ++i) {
        if (pieces[i] == FigureType::KING) kingPos = i;
        if (pieces[i] == FigureType::ROOK) {
            if (rook1 == -1) rook1 = i;
            else rook2 = i;
        }
    }
    
    if (kingPos != -1 && rook1 != -1 && rook2 != -1) {
        if (kingPos < rook1 || kingPos > rook2) {
            for (int i = rook1 + 1; i < rook2; ++i) {
                if (pieces[i] != FigureType::KING) {
                    std::swap(pieces[i], pieces[kingPos]);
                    break;
                }
            }
        }
    }
    
    for (int i = 0; i < 8; ++i) {
        board.setFigure(0, i, Figure(pieces[i], Color::BLACK));
        board.setFigure(7, i, Figure(pieces[i], Color::WHITE));
    }
    
    for (int i = 0; i < 8; ++i) {
        board.setFigure(1, i, Figure(FigureType::PAWN, Color::BLACK));
        board.setFigure(6, i, Figure(FigureType::PAWN, Color::WHITE));
    }
    
    board.setCurrentPlayer(Color::WHITE);
}

void ChessBoard::run() {
    while (window.isOpen()) {
        if (isAIMode && board.getCurrentPlayer() == aiColor) {
            sf::sleep(sf::milliseconds(300));
            Move bestMove = ai.getBestMove(aiColor);
            if (bestMove.fromRow != -1) {
                board.makeMove(bestMove.fromRow, bestMove.fromCol, 
                             bestMove.toRow, bestMove.toCol);
                board.switchPlayer();
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
                    handleClick(mousePos->position.x, mousePos->position.y);
                }
            }
            
            if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();
                
                if (key && key->scancode == sf::Keyboard::Scancode::S && key->control) {
                    FEN::saveGame(board, "save.fen");
                    std::cout << "Игра сохранена!\n";
                }
                
                if (key && key->scancode == sf::Keyboard::Scancode::L && key->control) {
                    if (FEN::loadGame("save.fen", board)) {
                        std::cout << "Игра загружена!\n";
                        hasSelected = false;
                    }
                }
                
                if (key && key->scancode == sf::Keyboard::Scancode::A) {
                    isAIMode = !isAIMode;
                    std::cout << "Режим AI: " << (isAIMode ? "ВКЛ" : "ВЫКЛ") << "\n";
                }
                
                if (key && key->scancode == sf::Keyboard::Scancode::R) {
                    board = Board();
                    if (gameMode == GameMode::FISCHER) {
                        setupFischerBoard();
                    }
                    hasSelected = false;
                    checkCount = 0;
                    std::cout << "Новая игра!\n";
                }
            }
        }
        
        window.clear(sf::Color(30, 30, 30));
        drawBoard();
        drawPieces();
        drawInfoPanel();
        window.display();
    }
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
                sf::Text text(font);
                text.setString(getPieceSymbol(fig));
                text.setCharacterSize(60);
                text.setStyle(sf::Text::Bold);
                
                if (fig.color == Color::WHITE) {
                    text.setFillColor(sf::Color::White);
                    text.setOutlineColor(sf::Color::Black);
                    text.setOutlineThickness(2);
                } else {
                    text.setFillColor(sf::Color::Black);
                }
                
                float x = col * CELL_SIZE + CELL_SIZE / 2 - 25 + 10;
                float y = row * CELL_SIZE + CELL_SIZE / 2 - 35 + 10;
                text.setPosition({x, y});
                window.draw(text);
            }
        }
    }
}

std::string ChessBoard::getPieceSymbol(const Figure& fig) {
    switch (fig.type) {
        case FigureType::KING:   return "K";
        case FigureType::QUEEN:  return "Q";
        case FigureType::ROOK:   return "R";
        case FigureType::BISHOP: return "B";
        case FigureType::KNIGHT: return "N";
        case FigureType::PAWN:   return "P";
        default: return "";
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
    title.setString("ИНФОРМАЦИЯ");
    title.setCharacterSize(18);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition({(float)BOARD_SIZE + 30, (float)yPos});
    window.draw(title);
    yPos += 35;
    
    sf::Text modeText(font);
    std::string modeStr;
    switch (gameMode) {
        case GameMode::CLASSIC: modeStr = "Классика"; break;
        case GameMode::FISCHER: modeStr = "Фишера"; break;
        case GameMode::THREE_CHECKS: modeStr = "3 шаха"; break;
    }
    modeText.setString("Режим: " + modeStr);
    modeText.setCharacterSize(14);
    modeText.setFillColor(sf::Color(200, 200, 200));
    modeText.setPosition({(float)BOARD_SIZE + 25, (float)yPos});
    window.draw(modeText);
    yPos += 25;
    
    sf::Text turnText(font);
    turnText.setString("Ход: " + std::string(board.getCurrentPlayer() == Color::WHITE ? "Белые" : "Черные"));
    turnText.setCharacterSize(15);
    turnText.setFillColor(sf::Color(200, 200, 200));
    turnText.setPosition({(float)BOARD_SIZE + 25, (float)yPos});
    window.draw(turnText);
    yPos += 28;
    
    if (gameMode == GameMode::THREE_CHECKS) {
        sf::Text checkText(font);
        checkText.setString("Шахов: " + std::to_string(checkCount) + "/3");
        checkText.setCharacterSize(15);
        checkText.setFillColor(sf::Color(255, 200, 0));
        checkText.setPosition({(float)BOARD_SIZE + 25, (float)yPos});
        window.draw(checkText);
        yPos += 28;
    }
    
    sf::Text aiText(font);
    aiText.setString("AI: " + std::string(isAIMode ? "ВКЛ" : "ВЫКЛ"));
    aiText.setCharacterSize(15);
    aiText.setFillColor(isAIMode ? sf::Color::Green : sf::Color(200, 200, 200));
    aiText.setPosition({(float)BOARD_SIZE + 25, (float)yPos});
    window.draw(aiText);
    yPos += 35;
    
    if (board.isInCheck(board.getCurrentPlayer())) {
        sf::Text checkText(font);
        checkText.setString("ШАХ!");
        checkText.setCharacterSize(18);
        checkText.setFillColor(sf::Color::Red);
        checkText.setStyle(sf::Text::Bold);
        checkText.setPosition({(float)BOARD_SIZE + 45, (float)yPos});
        window.draw(checkText);
        yPos += 30;
    }
    
    sf::Text controls(font);
    controls.setString("УПРАВЛЕНИЕ:");
    controls.setCharacterSize(15);
    controls.setFillColor(sf::Color(200, 200, 200));
    controls.setStyle(sf::Text::Bold);
    controls.setPosition({(float)BOARD_SIZE + 25, (float)yPos});
    window.draw(controls);
    yPos += 25;
    
    std::vector<std::string> shortcuts = {
        "A - AI вкл/выкл",
        "R - Новая игра",
        "Ctrl+S - Сохранить",
        "Ctrl+L - Загрузить"
    };
    
    for (const auto& shortcut : shortcuts) {
        sf::Text text(font);
        text.setString(shortcut);
        text.setCharacterSize(13);
        text.setFillColor(sf::Color(180, 180, 180));
        text.setPosition({(float)BOARD_SIZE + 28, (float)yPos});
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
            std::cout << "Выбрано: " << (char)('a' + col) << (8 - row) << "\n";
        }
    } else {
        if (board.makeMove(selectedRow, selectedCol, row, col)) {
            board.switchPlayer();
            hasSelected = false;
            
            if (gameMode == GameMode::THREE_CHECKS) {
                if (board.isInCheck(board.getCurrentPlayer())) {
                    checkCount++;
                    std::cout << "Шах! " << checkCount << "/3" << std::endl;
                }
            }
            
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

bool ChessBoard::isThreeChecksEnd() {
    if (gameMode != GameMode::THREE_CHECKS) return false;
    return checkCount >= 3;
}

void ChessBoard::checkGameEnd() {
    Color current = board.getCurrentPlayer();
    
    if (gameMode == GameMode::THREE_CHECKS) {
        if (isThreeChecksEnd()) {
            std::cout << "ТРИ ШАХА! " << (current == Color::WHITE ? "Белые" : "Черные") << " проиграли!\n";
            return;
        }
    }
    
    if (board.isCheckmate(current)) {
        std::cout << "МАТ! " << (current == Color::WHITE ? "Белые" : "Черные") << " проиграли!\n";
    } else if (board.isStalemate(current)) {
        std::cout << "ПАТ! Ничья!\n";
    } else if (board.isInCheck(current)) {
        std::cout << "ШАХ! " << (current == Color::WHITE ? "Белые" : "Черные") << " король под шахом!\n";
    }
}