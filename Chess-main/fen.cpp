#include "fen.h"
#include <fstream>
#include <sstream>
#include <cctype>

std::string FEN::toFEN(const Board& board) {
    std::string fen;
    int emptyCount = 0;
    
    // расстановка фигур
    for (int r = 0; r < 8; ++r) {
        emptyCount = 0;
        for (int c = 0; c < 8; ++c) {
            Figure fig = board.getFigure(r, c);
            if (fig.isEmpty()) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += fig.getSymbol();
            }
        }
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        if (r < 7) fen += '/';
    }
    
    // очередь хода
    fen += ' ';
    fen += (board.getCurrentPlayer() == Color::WHITE) ? 'w' : 'b';
    
    // рокировка (пока не реализована)
    fen += " -";
    
    // битое поле (пока не реализовано)
    fen += " -";
    
    // количество полуходов и ходов (пока не реализовано)
    fen += " 0 1";
    
    return fen;
}

bool FEN::fromFEN(const std::string& fen, Board& board) {
    std::stringstream ss(fen);
    std::string position, turn, castling, enPassant, halfMove, fullMove;
    
    if (!(ss >> position >> turn >> castling >> enPassant >> halfMove >> fullMove)) {
        return false;
    }
    
    // создаем новую доску
    Board newBoard;
    
    // расставляем фигуры
    int row = 0, col = 0;
    for (char c : position) {
        if (c == '/') {
            row++;
            col = 0;
        } else if (isdigit(c)) {
            int empty = c - '0';
            for (int i = 0; i < empty; ++i) {
                if (col < 8) {
                    col++;
                }
            }
        } else {
            if (row < 8 && col < 8) {
                Color color = isupper(c) ? Color::WHITE : Color::BLACK;
                FigureType type;
                char upper = toupper(c);
                switch (upper) {
                    case 'P': type = FigureType::PAWN; break;
                    case 'N': type = FigureType::KNIGHT; break;
                    case 'B': type = FigureType::BISHOP; break;
                    case 'R': type = FigureType::ROOK; break;
                    case 'Q': type = FigureType::QUEEN; break;
                    case 'K': type = FigureType::KING; break;
                    default: return false;
                }
                newBoard.setFigure(row, col, Figure(type, color));
                col++;
            }
        }
    }
    
    // устанавливаем очередь хода
    Color player = (turn == "w") ? Color::WHITE : Color::BLACK;
    newBoard.setCurrentPlayer(player);
    
    // копируем новую доску в переданную
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            board.setFigure(r, c, newBoard.getFigure(r, c));
        }
    }
    board.setCurrentPlayer(newBoard.getCurrentPlayer());
    
    return true;
}

bool FEN::saveGame(const Board& board, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string fen = toFEN(board);
    file << fen;
    file.close();
    return true;
}

bool FEN::loadGame(const std::string& filename, Board& board) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string fen;
    std::getline(file, fen);
    file.close();
    
    return fromFEN(fen, board);
}