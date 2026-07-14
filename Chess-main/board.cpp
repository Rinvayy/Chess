#include "board.h"
#include <iostream>
#include <cmath>

Board::Board() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            board[r][c] = Figure();
        }
    }

    setupBoard();
    currentPlayer = Color::WHITE;
}

void Board::setupBoard() {
    // ЧЕРНЫЕ ФИГУРЫ
    board[0][0] = Figure(FigureType::ROOK, Color::BLACK);
    board[0][1] = Figure(FigureType::KNIGHT, Color::BLACK);
    board[0][2] = Figure(FigureType::BISHOP, Color::BLACK);
    board[0][3] = Figure(FigureType::QUEEN, Color::BLACK);
    board[0][4] = Figure(FigureType::KING, Color::BLACK);
    board[0][5] = Figure(FigureType::BISHOP, Color::BLACK);
    board[0][6] = Figure(FigureType::KNIGHT, Color::BLACK);
    board[0][7] = Figure(FigureType::ROOK, Color::BLACK);
    // ЧЕРНЫЕ ПЕШКИ
    for (int c = 0; c < 8; ++c) {
        board[1][c] = Figure(FigureType::PAWN, Color::BLACK);
    }
    // БЕЛЫЕ ПЕШКИ
    for (int c = 0; c < 8; ++c) {
        board[6][c] = Figure(FigureType::PAWN, Color::WHITE);
    }
    // БЕЛЫЕ ФИГУРЫ
    board[7][0] = Figure(FigureType::ROOK, Color::WHITE);
    board[7][1] = Figure(FigureType::KNIGHT, Color::WHITE);
    board[7][2] = Figure(FigureType::BISHOP, Color::WHITE);
    board[7][3] = Figure(FigureType::QUEEN, Color::WHITE);
    board[7][4] = Figure(FigureType::KING, Color::WHITE);
    board[7][5] = Figure(FigureType::BISHOP, Color::WHITE);
    board[7][6] = Figure(FigureType::KNIGHT, Color::WHITE);
    board[7][7] = Figure(FigureType::ROOK, Color::WHITE);
}

bool Board::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (!isValidMove(fromRow, fromCol, toRow, toCol)) {
        return false;
    }
    // проверяем, что ходим своей фигурой
    Figure fromFig = board[fromRow][fromCol];
    if (fromFig.color != currentPlayer) {
        return false;
    }
    // сохраняем состояние для отката
    Figure captured = board[toRow][toCol];
    // выполняем ход
    board[toRow][toCol] = fromFig;
    board[fromRow][fromCol] = Figure();
    // проверяем, не оставляем ли мы своего короля под шахом
    if (isInCheck(currentPlayer)) {
        // откатываем ход
        board[fromRow][fromCol] = fromFig;
        board[toRow][toCol] = captured;
        return false;
    }
    // превращение пешки
    if (fromFig.type == FigureType::PAWN) {
        if (toRow == 0 || toRow == 7) {
            board[toRow][toCol] = Figure(FigureType::QUEEN, fromFig.color);
            std::cout << "Пешка превратилась в ферзя!\n";
        }
    }

    switchPlayer();
    return true;
}

bool Board::isInCheck(Color color) const {
    // находим короля указанного цвета
    int kingRow = -1, kingCol = -1;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Figure fig = board[r][c];
            if (fig.type == FigureType::KING && fig.color == color) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
        if (kingRow != -1) break;
    }
    if (kingRow == -1) return false;
    Color attackerColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isPositionUnderAttack(kingRow, kingCol, attackerColor);
}

bool Board::isCheckmate(Color color) {
    if (!isInCheck(color)) return false;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Figure fig = board[r][c];
            if (fig.isEmpty() || fig.color != color) continue;
            for (int tr = 0; tr < 8; ++tr) {
                for (int tc = 0; tc < 8; ++tc) {
                    Figure captured = board[tr][tc];
                    // временно делаем ход
                    board[tr][tc] = fig;
                    board[r][c] = Figure();
                    bool stillInCheck = isInCheck(color);
                    // откатываем ход
                    board[r][c] = fig;
                    board[tr][tc] = captured;
                    if (!stillInCheck) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::isStalemate(Color color) {
    if (isInCheck(color)) return false;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Figure fig = board[r][c];
            if (fig.isEmpty() || fig.color != color) continue;
            for (int tr = 0; tr < 8; ++tr) {
                for (int tc = 0; tc < 8; ++tc) {
                    if (!isValidMove(r, c, tr, tc)) continue;
                    Figure captured = board[tr][tc];
                    // временно делаем ход
                    board[tr][tc] = fig;
                    board[r][c] = Figure();
                    bool stillInCheck = isInCheck(color);
                    // откатываем ход
                    board[r][c] = fig;
                    board[tr][tc] = captured;
                    if (!stillInCheck) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const {
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
        return false;
    }
    
    Figure fromFig = board[fromRow][fromCol];
    Figure toFig = board[toRow][toCol];
    
    if (fromFig.isEmpty()) return false;
    if (!toFig.isEmpty() && toFig.color == fromFig.color) return false;
    
    switch (fromFig.type) {
        case FigureType::PAWN:   return isValidPawnMove(fromRow, fromCol, toRow, toCol);
        case FigureType::KNIGHT: return isValidKnightMove(fromRow, fromCol, toRow, toCol);
        case FigureType::BISHOP: return isValidBishopMove(fromRow, fromCol, toRow, toCol);
        case FigureType::ROOK:   return isValidRookMove(fromRow, fromCol, toRow, toCol);
        case FigureType::QUEEN:  return isValidQueenMove(fromRow, fromCol, toRow, toCol);
        case FigureType::KING:   return isValidKingMove(fromRow, fromCol, toRow, toCol);
        default: return false;
    }
}

bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol) const {
    int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
    int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
    
    int row = fromRow + rowStep;
    int col = fromCol + colStep;
    
    while (row != toRow || col != toCol) {
        if (!board[row][col].isEmpty()) {
            return false;
        }
        row += rowStep;
        col += colStep;
    }
    return true;
}

bool Board::isPositionUnderAttack(int row, int col, Color attackerColor) const {
    // проверка от пешек
    int pawnDirection = (attackerColor == Color::WHITE) ? -1 : 1;
    int pawnRow = row + pawnDirection;
    
    if (pawnRow >= 0 && pawnRow < 8) {
        if (col - 1 >= 0) {
            Figure fig = board[pawnRow][col - 1];
            if (fig.type == FigureType::PAWN && fig.color == attackerColor) {
                return true;
            }
        }
        if (col + 1 < 8) {
            Figure fig = board[pawnRow][col + 1];
            if (fig.type == FigureType::PAWN && fig.color == attackerColor) {
                return true;
            }
        }
    }
    
    // проверка от коней
    int knightMoves[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    for (int i = 0; i < 8; ++i) {
        int nr = row + knightMoves[i][0];
        int nc = col + knightMoves[i][1];
        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            Figure fig = board[nr][nc];
            if (fig.type == FigureType::KNIGHT && fig.color == attackerColor) {
                return true;
            }
        }
    }
    
    // проверка от слонов и ферзей (диагонали)
    int directions[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};
    for (int i = 0; i < 4; ++i) {
        int r = row + directions[i][0];
        int c = col + directions[i][1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Figure fig = board[r][c];
            if (!fig.isEmpty()) {
                if ((fig.type == FigureType::BISHOP || fig.type == FigureType::QUEEN) &&
                    fig.color == attackerColor) {
                    return true;
                }
                break;
            }
            r += directions[i][0];
            c += directions[i][1];
        }
    }
    
    // проверка от ладей и ферзей (прямые линии)
    int directions2[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    for (int i = 0; i < 4; ++i) {
        int r = row + directions2[i][0];
        int c = col + directions2[i][1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Figure fig = board[r][c];
            if (!fig.isEmpty()) {
                if ((fig.type == FigureType::ROOK || fig.type == FigureType::QUEEN) &&
                    fig.color == attackerColor) {
                    return true;
                }
                break;
            }
            r += directions2[i][0];
            c += directions2[i][1];
        }
    }
    
    // проверка от короля
    int kingMoves[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    for (int i = 0; i < 8; ++i) {
        int nr = row + kingMoves[i][0];
        int nc = col + kingMoves[i][1];
        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            Figure fig = board[nr][nc];
            if (fig.type == FigureType::KING && fig.color == attackerColor) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol) const {
    Figure fromFig = board[fromRow][fromCol];
    Figure toFig = board[toRow][toCol];
    
    int direction = (fromFig.color == Color::WHITE) ? -1 : 1;
    int startRow = (fromFig.color == Color::WHITE) ? 6 : 1;
    
    if (fromCol == toCol && toFig.isEmpty()) {
        if (toRow == fromRow + direction) {
            return true;
        }
        if (fromRow == startRow && toRow == fromRow + 2 * direction) {
            return board[fromRow + direction][fromCol].isEmpty();
        }
    }
    
    if (abs(fromCol - toCol) == 1 && toRow == fromRow + direction) {
        return !toFig.isEmpty();
    }
    
    return false;
}

bool Board::isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol) const {
    int dr = abs(fromRow - toRow);
    int dc = abs(fromCol - toCol);
    return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
}

bool Board::isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol) const {
    if (abs(fromRow - toRow) == abs(fromCol - toCol) && (fromRow != toRow || fromCol != toCol)) {
        return isPathClear(fromRow, fromCol, toRow, toCol);
    }
    return false;
}

bool Board::isValidRookMove(int fromRow, int fromCol, int toRow, int toCol) const {
    if ((fromRow == toRow || fromCol == toCol) && (fromRow != toRow || fromCol != toCol)) {
        return isPathClear(fromRow, fromCol, toRow, toCol);
    }
    return false;
}

bool Board::isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol) const {
    return isValidRookMove(fromRow, fromCol, toRow, toCol) || 
           isValidBishopMove(fromRow, fromCol, toRow, toCol);
}

bool Board::isValidKingMove(int fromRow, int fromCol, int toRow, int toCol) const {
    int dr = abs(fromRow - toRow);
    int dc = abs(fromCol - toCol);
    return dr <= 1 && dc <= 1 && (dr != 0 || dc != 0);
}

void Board::setCurrentPlayer(Color player) {
    currentPlayer = player;
}

void Board::setFigure(int row, int col, const Figure& fig) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        board[row][col] = fig;
    }
}
