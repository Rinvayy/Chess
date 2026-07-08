#ifndef AI_H
#define AI_H

#include "board.h"
#include <vector>
#include <cstdlib>
#include <ctime>

struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    int score;
};

class AI {
private:
    Board* board;
    int getPieceValue(FigureType type) const;
    std::vector<Move> getAllMoves(Color color) const;
    int evaluateBoard() const;
    
public:
    AI(Board* b);
    Move getBestMove(Color aiColor);
};

#endif