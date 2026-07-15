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
    Color humanColor;
    Color aiColor;
    
    int selectedRow, selectedCol;
    bool hasSelected;
    
public:
    ChessBoard();
    void run();
    
private:
    void drawBoard();
    void drawCoordinate(int row, int col);
    void drawPieces();
    void drawInfoPanel();
    void handleClick(int x, int y);
    void checkGameEnd();
};

#endif