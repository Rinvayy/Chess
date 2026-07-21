#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "board.h"
#include "ai.h"
#include "fen.h"
#include "PieceRenderer.h"

enum class GameMode {
    CLASSIC,
    FISCHER,
    THREE_CHECKS
};

class ChessBoard {
private:
    static const int SIZE = 8;
    static const int CELL_SIZE = 90;
    static const int BOARD_SIZE = SIZE * CELL_SIZE;
    static const int WINDOW_WIDTH = BOARD_SIZE + 200;
    static const int WINDOW_HEIGHT = BOARD_SIZE + 100;
    
    sf::RenderWindow window;
    sf::Font font;
    
    PieceRenderer pieceRenderer;
    Board board;
    AI ai;
    
    bool isAIMode;
    Color playerColor;
    Color aiColor;
    Color humanColor;
    
    int selectedRow, selectedCol;
    bool hasSelected;
    
    GameMode gameMode;
    
    sf::RectangleShape switchColorButton;
    sf::Text* switchColorText = nullptr;
    bool isSwitchColorHovered = false;
    
public:
    ChessBoard(GameMode mode, bool aiMode, Color playerColor = Color::WHITE);
    ~ChessBoard();
    
    void run();
    void switchPlayerColor();
    void handleSwitchColorClick();
    void drawSwitchColorButton();
    
private:
    void drawBoard();
    void drawCoordinate(int row, int col);
    void drawPieces();
    void drawInfoPanel();
    void handleClick(int x, int y);
    void checkGameEnd();

private:
    bool isBoardFlipped = false;
};

#endif