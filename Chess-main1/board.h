#ifndef BOARD_H
#define BOARD_H 
#include "figures.h"            //на данном этапе figures.h содержит только перечисления без логики
#include <string>

class Board {
private:
    Figure board[8][8];
    Color currentPlayer;
    
public:
    Board();
    void printBoard() const;
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool isInCheck(Color color) const;      //проверка шаха
    
    bool isCheckmate(Color color) const;        //проверка мата

    bool isStalemate(Color color) const;        ///проверка пата
    
    Color getCurrentPlayer() const { 
        return currentPlayer;           //геттер - возвращает текущего игрока (white или black)
    }
    
    void switchPlayer() { 
        currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;      //меняем текущего игрока на противоположного
    }
    
    Figure getFigure(int row, int col) const { 
        return board[row][col];             //возвращает фигуру на указанной клетке (row, col)
    }
    

private:
    
    void setupBoard();      //устанавливает начальную расстановку фигур на доске
 
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;     //проверяет, является ли ход допустимым
    
    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;     //проверяет, свободен ли путь между клетками (для фигур, которые ходят по прямой линии)
    
    bool isPositionUnderAttack(int row, int col, Color attackerColor) const;        //проверяет, находится ли клетка (row, col) под атакой фигуры указанного цвета (attackerColor)
    
    //порядок реализации (от простого к сложному):
    //  1. isValidRookMove   - прямые линии (ладья)
    //  2. isValidBishopMove - диагонали (слон)
    //  3. isValidQueenMove  - комбинация ладьи и слона (ферзь)
    //  4. isValidKnightMove - буквой Г (конь)
    //  5. isValidKingMove   - 1 клетка в любую сторону (король)
    //  6. isValidPawnMove   - сложные правила пешки (двойной ход, взятие на проходе, превращение)\

    bool isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidRookMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidKingMove(int fromRow, int fromCol, int toRow, int toCol) const;
};

#endif 