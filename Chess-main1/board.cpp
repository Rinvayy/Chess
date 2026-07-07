#include "board.h"
#include <iostream>
#include <cmath>

Board::Board() {        //конструктор доски
    for (int r = 0; r < 8; ++r) {          //ряды
        for (int c = 0; c < 8; ++c) {      //колонки
            //пока что все клетки пустые
            board[r][c] = Figure();
        }
    }

    setupBoard();                  //вызываем метод расстановки фигур (пока пустой)
    currentPlayer = Color::WHITE;  //устанавливаем текущего игрока на белых
}
void Board::setupBoard() {              //метод расстановки фигур (пока пустой)
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            board[r][c] = Figure();
        }
    }
}

void Board::printBoard() const {
    std::cout << "\n  a  b  c  d  e  f  g  h\n";        //печатаем буквы сверху (a-h)
    std::cout << " +------------------------+\n";

    for (int r = 0; r < 8; ++r) {       //внешний цикл по рядам
        std::cout << (8 - r) << "|";
        
        for (int c = 0; c < 8; ++c) {           //внутренний цикл по колонкам
            Figure figure = board[r][c];
            
            if (figure.isEmpty()) {              //если клетка пуста, печатаем точку
                std::cout << "· ";
            } else {                //если на клетке есть фигура, печатаем её юникod-символ 
                std::cout << figure.getUnicodeSymbol();
            }
        }
        std::cout << "|" << (8 - r) << "\n";        //печатаем номер ряда справа
    }
    
    //печатаем нижнюю границу доски и буквы снизу
    std::cout << " +------------------------+\n";
    std::cout << "  a  b  c  d  e  f  g  h\n\n";
    
    std::cout << "Ход: " << (currentPlayer == Color::WHITE ? "Белые" : "Черные") << "\n";       //печатаем текущего игрока
}

bool Board::makeMove(int fromRow, int fromCol, int toRow, int toCol) {      //метод для совершения хода
    std::cout << "Метод makeMove() пока не реализован\n";         
    return false;
}

bool Board::isInCheck(Color color) const {      //метод для проверки шаха
    return false;
}

bool Board::isCheckmate(Color color) const {        //метод для проверки мата
    return false;
}

bool Board::isStalemate(Color color) const {        ///метод для проверки пата
    return false;
}

bool Board::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const {    //метод для проверки допустимости хода
    return false;
}

bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol) const {     //метод для проверки свободен ли путь между клетками (для фигур, которые ходят по прямой линии)
    return true;
}

bool Board::isPositionUnderAttack(int row, int col, Color attackerColor) const {        //метод для проверки, находится ли клетка (row, col) под атакой фигуры указанного цвета (attackerColor)
    return false;
}

//методы проверки допустимости ходов для каждой фигруры 
bool Board::isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol) const {
    return false; 
}

bool Board::isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol) const {
    return false;
}

bool Board::isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol) const {
    return false; 
}

bool Board::isValidRookMove(int fromRow, int fromCol, int toRow, int toCol) const {
    return false; 
}

bool Board::isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol) const {
    return false;  
}

bool Board::isValidKingMove(int fromRow, int fromCol, int toRow, int toCol) const {
    return false;
}