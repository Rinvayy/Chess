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
void Board::setupBoard() {     //метод расстановки фигур (пока пустой)
    // ЧЕРНЫЕ ФИГУРЫ (ряд 0 - верх доски)
    board[0][0] = Figure(FigureType::ROOK, Color::BLACK);
    board[0][1] = Figure(FigureType::KNIGHT, Color::BLACK);
    board[0][2] = Figure(FigureType::BISHOP, Color::BLACK);
    board[0][3] = Figure(FigureType::QUEEN, Color::BLACK);
    board[0][4] = Figure(FigureType::KING, Color::BLACK);
    board[0][5] = Figure(FigureType::BISHOP, Color::BLACK);
    board[0][6] = Figure(FigureType::KNIGHT, Color::BLACK);
    board[0][7] = Figure(FigureType::ROOK, Color::BLACK);

    // ЧЕРНЫЕ ПЕШКИ (ряд 1)
    for (int c = 0; c < 8; ++c) {
        board[1][c] = Figure(FigureType::PAWN, Color::BLACK);
    }

    // БЕЛЫЕ ПЕШКИ (ряд 6)
    for (int c = 0; c < 8; ++c) {
        board[6][c] = Figure(FigureType::PAWN, Color::WHITE);
    }

    // БЕЛЫЕ ФИГУРЫ (ряд 7 - низ доски)
    board[7][0] = Figure(FigureType::ROOK, Color::WHITE);
    board[7][1] = Figure(FigureType::KNIGHT, Color::WHITE);
    board[7][2] = Figure(FigureType::BISHOP, Color::WHITE);
    board[7][3] = Figure(FigureType::QUEEN, Color::WHITE);
    board[7][4] = Figure(FigureType::KING, Color::WHITE);
    board[7][5] = Figure(FigureType::BISHOP, Color::WHITE);
    board[7][6] = Figure(FigureType::KNIGHT, Color::WHITE);
    board[7][7] = Figure(FigureType::ROOK, Color::WHITE);
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
            } else {                //если на клетке есть фигура, печатаем её юникoд-символ 
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

bool Board::makeMove(int fromRow, int fromCol, int toRow, int toCol) {    //метод для совершения хода
    if (!isValidMove(fromRow, fromCol, toRow, toCol)) {
        return false;
    }
    
    //сохраняем информацию о взятии
    Figure captured = board[toRow][toCol];
    //делаем ход
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = Figure();
    
    //проверяем, не оставили ли мы себя под шахом
    if (isInCheck(currentPlayer)) {
        //если оставляем, отменяем ход и возвращаем false
        board[fromRow][fromCol] = board[toRow][toCol];
        board[toRow][toCol] = captured;
        return false;
    }
    
    //проверяем, не достигла ли пешка последнего ряда и превращаем её в ферзя
    if (board[toRow][toCol].type == FigureType::PAWN) {
        if ((toRow == 0 && board[toRow][toCol].color == Color::BLACK) ||
            (toRow == 7 && board[toRow][toCol].color == Color::WHITE)) {
            board[toRow][toCol] = Figure(FigureType::QUEEN, board[toRow][toCol].color);
        }
    }
    
    //меняем текущего игрока на противоположного
    switchPlayer();
    return true;
}

bool Board::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const {    //метод для проверки допустимости хода
    //проверка границ
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
        return false;
    }
    //проверка, что начальная клетка не пуста
    if (board[fromRow][fromCol].isEmpty()) {
        return false;
    }
    //проверка, что ходим своей фигурой
    if (board[fromRow][fromCol].color != currentPlayer) {
        return false;
    }
    
    //проверка, что конечная клетка не занята своей фигурой
    if (!board[toRow][toCol].isEmpty() && board[toRow][toCol].color == currentPlayer) {
        return false;
    }
    
    //проверка правил хода для конкретной фигуры
    FigureType type = board[fromRow][fromCol].type;
    switch (type) {
        case FigureType::PAWN:   return isValidPawnMove(fromRow, fromCol, toRow, toCol);
        case FigureType::KNIGHT: return isValidKnightMove(fromRow, fromCol, toRow, toCol);
        case FigureType::BISHOP: return isValidBishopMove(fromRow, fromCol, toRow, toCol);
        case FigureType::ROOK:   return isValidRookMove(fromRow, fromCol, toRow, toCol);
        case FigureType::QUEEN:  return isValidQueenMove(fromRow, fromCol, toRow, toCol);
        case FigureType::KING:   return isValidKingMove(fromRow, fromCol, toRow, toCol);
        default: return false;
    }
}

bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol) const {    //метод для проверки свободен ли путь между клетками (для фигур, которые ходят по прямой линии)
    int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
    int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
    int row = fromRow + rowStep;
    int col = fromCol + colStep;
    
    while (row != toRow || col != toCol) {
        if (!board[row][col].isEmpty()) {
            return false; //если на пути есть фигура, путь не свободен
        }
        row += rowStep;
        col += colStep;
    }
    
    return true;
}

bool Board::isPositionUnderAttack(int row, int col, Color attackerColor) const {   //метод для проверки, находится ли клетка (row, col) под атакой фигуры указанного цвета (attackerColor)
    //проверяем все клетки доски
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r][c].isEmpty()) continue;
            if (board[r][c].color != attackerColor) continue;
            //временно меняем текущего игрока для проверки хода
            Color savedPlayer = currentPlayer;
            const_cast<Board*>(this)->currentPlayer = attackerColor;
            bool canAttack = isValidMove(r, c, row, col);
            const_cast<Board*>(this)->currentPlayer = savedPlayer;
            
            if (canAttack) return true;
        }
    }
    return false;
}

bool Board::isInCheck(Color color) const {      //метод для проверки шаха
    //находим короля данного цвета
    int kingRow = -1, kingCol = -1;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r][c].type == FigureType::KING && board[r][c].color == color) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
        if (kingRow != -1) break;
    }
    
    if (kingRow == -1) return false;
    
    //проверяем, атакуют ли вражеские фигуры позицию короля
    Color enemyColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isPositionUnderAttack(kingRow, kingCol, enemyColor);
}

bool Board::isCheckmate(Color color) const {    //метод для проверки мата
    if (!isInCheck(color)) return false;
    //проверяем все возможные ходы
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r][c].isEmpty()) continue;
            if (board[r][c].color != color) continue;
            
            for (int tr = 0; tr < 8; ++tr) {
                for (int tc = 0; tc < 8; ++tc) {
                    //пробуем сделать ход
                    Board tempBoard = *this;
                    if (tempBoard.makeMove(r, c, tr, tc)) {
                        if (!tempBoard.isInCheck(color)) {
                            return false; //есть спасительный ход
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Board::isStalemate(Color color) const {    //метод для проверки пата
    if (isInCheck(color)) return false;
    //проверяем все возможные ходы
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r][c].isEmpty()) continue;
            if (board[r][c].color != color) continue;
            
            for (int tr = 0; tr < 8; ++tr) {
                for (int tc = 0; tc < 8; ++tc) {
                    Board tempBoard = *this;
                    if (tempBoard.makeMove(r, c, tr, tc)) {
                        if (!tempBoard.isInCheck(color)) {
                            return false; //есть ход
                        }
                    }
                }
            }
        }
    }
    return true;
}

//методы проверки допустимости ходов для каждой фигуры 
bool Board::isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol) const {
    int direction = (board[fromRow][fromCol].color == Color::WHITE) ? 1 : -1;
    int startRow = (board[fromRow][fromCol].color == Color::WHITE) ? 6 : 1;
    //ход вперед на 1 клетку
    if (fromCol == toCol && toRow == fromRow + direction && board[toRow][toCol].isEmpty()) {
        return true;
    }
    //ход вперед на 2 клетки (только с начальной позиции)
    if (fromCol == toCol && toRow == fromRow + 2 * direction && 
        fromRow == startRow && board[toRow][toCol].isEmpty() && 
        board[fromRow + direction][fromCol].isEmpty()) {
        return true;
    }
    //взятие по диагонали
    if (std::abs(toCol - fromCol) == 1 && toRow == fromRow + direction) {
        if (!board[toRow][toCol].isEmpty() && board[toRow][toCol].color != board[fromRow][fromCol].color) {
            return true;
        }
    }
    return false;
}

bool Board::isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol) const {
    int rowDiff = std::abs(toRow - fromRow);
    int colDiff = std::abs(toCol - fromCol);
    
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

bool Board::isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol) const {
    int rowDiff = std::abs(toRow - fromRow);
    int colDiff = std::abs(toCol - fromCol);
    
    if (rowDiff != colDiff || rowDiff == 0) {
        return false;
    }
    
    return isPathClear(fromRow, fromCol, toRow, toCol);
}

bool Board::isValidRookMove(int fromRow, int fromCol, int toRow, int toCol) const {
    if (fromRow != toRow && fromCol != toCol) {
        return false;
    }
    
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    return isPathClear(fromRow, fromCol, toRow, toCol);
}

bool Board::isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol) const {
    //ферзь ходит как слон или ладья
    return isValidBishopMove(fromRow, fromCol, toRow, toCol) ||
           isValidRookMove(fromRow, fromCol, toRow, toCol);
}

bool Board::isValidKingMove(int fromRow, int fromCol, int toRow, int toCol) const {
    int rowDiff = std::abs(toRow - fromRow);
    int colDiff = std::abs(toCol - fromCol);
    
    return rowDiff <= 1 && colDiff <= 1 && (rowDiff + colDiff > 0);
}
