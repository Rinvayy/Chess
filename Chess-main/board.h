#ifndef BOARD_H
#define BOARD_H 
#include "PieceRenderer.h"
#include <string>

class Board {
private:
    Figure board[8][8];
    Color currentPlayer;
    
public:
    Board();
    void printBoard() const;
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool isInCheck(Color color) const;
    bool isCheckmate(Color color);
    bool isStalemate(Color color);
    
    Color getCurrentPlayer() const { 
        return currentPlayer;
    }
    
    void switchPlayer() { 
        currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }
    
    Figure getFigure(int row, int col) const { 
        return board[row][col];
    }

    void setCurrentPlayer(Color player);
    void setFigure(int row, int col, const Figure& fig);

private:
    void setupBoard();
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isPositionUnderAttack(int row, int col, Color attackerColor) const;
    
    bool isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidRookMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidKingMove(int fromRow, int fromCol, int toRow, int toCol) const;
};

#endif