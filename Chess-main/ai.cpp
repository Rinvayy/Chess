#include "ai.h"
#include <algorithm>
#include <climits>
#include <cmath>

AI::AI(Board* b) : board(b) {
    srand(time(0));
}

int AI::getPieceValue(FigureType type) const {
    switch (type) {
        case FigureType::PAWN:   return 100;
        case FigureType::KNIGHT: return 320;
        case FigureType::BISHOP: return 330;
        case FigureType::ROOK:   return 500;
        case FigureType::QUEEN:  return 900;
        case FigureType::KING:   return 10000;
        default:                 return 0;
    }
}

std::vector<Move> AI::getAllMoves(Color color) const {
    std::vector<Move> moves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Figure fig = board->getFigure(r, c);
            if (fig.isEmpty() || fig.color != color) continue;
            for (int tr = 0; tr < 8; ++tr) {
                for (int tc = 0; tc < 8; ++tc) {
                    if (r == tr && c == tc) continue;
                    Figure targetFig = board->getFigure(tr, tc);
                    if (!targetFig.isEmpty() && targetFig.color == color) continue;
                    bool valid = false;
                    switch (fig.type) {
                        case FigureType::PAWN: {
                            int direction = (color == Color::WHITE) ? -1 : 1;
                            int startRow = (color == Color::WHITE) ? 6 : 1;
                            if (c == tc && tr == r + direction && targetFig.isEmpty()) {
                                valid = true;
                            }
                            if (c == tc && tr == r + 2 * direction && 
                                r == startRow && targetFig.isEmpty() && 
                                board->getFigure(r + direction, c).isEmpty()) {
                                valid = true;
                            }
                            if (abs(c - tc) == 1 && tr == r + direction && 
                                !targetFig.isEmpty()) {
                                valid = true;
                            }
                            break;
                        }
                        case FigureType::KNIGHT: {
                            int dr = abs(r - tr);
                            int dc = abs(c - tc);
                            if ((dr == 2 && dc == 1) || (dr == 1 && dc == 2)) {
                                valid = true;
                            }
                            break;
                        }
                        case FigureType::BISHOP: {
                            if (abs(r - tr) == abs(c - tc) && (r != tr || c != tc)) {
                                // проверяем путь
                                int rowStep = (tr > r) ? 1 : -1;
                                int colStep = (tc > c) ? 1 : -1;
                                int row = r + rowStep;
                                int col = c + colStep;
                                bool pathClear = true;
                                while (row != tr || col != tc) {
                                    if (!board->getFigure(row, col).isEmpty()) {
                                        pathClear = false;
                                        break;
                                    }
                                    row += rowStep;
                                    col += colStep;
                                }
                                if (pathClear) valid = true;
                            }
                            break;
                        }
                        case FigureType::ROOK: {
                            if ((r == tr || c == tc) && (r != tr || c != tc)) {
                                // проверяем путь
                                int rowStep = (tr > r) ? 1 : (tr < r) ? -1 : 0;
                                int colStep = (tc > c) ? 1 : (tc < c) ? -1 : 0;
                                int row = r + rowStep;
                                int col = c + colStep;
                                bool pathClear = true;
                                while (row != tr || col != tc) {
                                    if (!board->getFigure(row, col).isEmpty()) {
                                        pathClear = false;
                                        break;
                                    }
                                    row += rowStep;
                                    col += colStep;
                                }
                                if (pathClear) valid = true;
                            }
                            break;
                        }
                        case FigureType::QUEEN: {
                            if ((r == tr || c == tc || abs(r - tr) == abs(c - tc)) && 
                                (r != tr || c != tc)) {
                                // проверяем путь
                                int rowStep = (tr > r) ? 1 : (tr < r) ? -1 : 0;
                                int colStep = (tc > c) ? 1 : (tc < c) ? -1 : 0;
                                int row = r + rowStep;
                                int col = c + colStep;
                                bool pathClear = true;
                                while (row != tr || col != tc) {
                                    if (!board->getFigure(row, col).isEmpty()) {
                                        pathClear = false;
                                        break;
                                    }
                                    row += rowStep;
                                    col += colStep;
                                }
                                if (pathClear) valid = true;
                            }
                            break;
                        }
                        case FigureType::KING: {
                            if (abs(r - tr) <= 1 && abs(c - tc) <= 1 && 
                                (r != tr || c != tc)) {
                                valid = true;
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    if (valid) {
                        Move m;
                        m.fromRow = r;
                        m.fromCol = c;
                        m.toRow = tr;
                        m.toCol = tc;
                        m.score = 0;
                        moves.push_back(m);
                    }
                }
            }
        }
    }
    return moves;
}

int AI::evaluateBoard() const {
    int score = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Figure fig = board->getFigure(r, c);
            if (fig.isEmpty()) continue;
            int value = getPieceValue(fig.type);
            if (fig.color == Color::WHITE) {
                score += value;
            } else {
                score -= value;
            }
        }
    }
    return score;
}

Move AI::getBestMove(Color aiColor) {
    std::vector<Move> moves = getAllMoves(aiColor);
    if (moves.empty()) {
        Move empty = {-1, -1, -1, -1, 0};
        return empty;
    }
    // оцениваем каждый ход
    for (auto& move : moves) {
        Figure moving = board->getFigure(move.fromRow, move.fromCol);
        Figure captured = board->getFigure(move.toRow, move.toCol);
        move.score = 0;
        // бонус за взятие фигуры
        if (!captured.isEmpty()) {
            move.score += getPieceValue(captured.type) * 2;
        }
        // бонус за продвижение пешки
        if (moving.type == FigureType::PAWN) {
            if (aiColor == Color::WHITE && move.toRow == 0) {
                move.score += 500;
            }
            if (aiColor == Color::BLACK && move.toRow == 7) {
                move.score += 500;
            }
        }
        // бонус за центральные клетки
        if (move.toRow >= 2 && move.toRow <= 5 && move.toCol >= 2 && move.toCol <= 5) {
            move.score += 10;
        }
    }
    // сортируем по убыванию оценки
    std::sort(moves.begin(), moves.end(), 
        [](const Move& a, const Move& b) {
            return a.score > b.score;
        });
    // добавляем случайность
    int randomFactor = rand() % 3;
    int index = (randomFactor < 2 && moves.size() > 1) ? rand() % std::min(3, (int)moves.size()) : 0;
    return moves[index];
}