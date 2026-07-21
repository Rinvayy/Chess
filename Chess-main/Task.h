#ifndef TASK_H
#define TASK_H

#include "board.h"
#include <string>
#include <vector>

struct ChessTask {
    std::string name;
    std::string fen;            // начальная позиция
    std::string solution;       // правильное решение
    int difficulty;
    std::string description;    // лписание
    bool isUserCreated = false; // создана пользователем
    
    // для проверки хода
    std::vector<std::string> getSolutionMoves() const {
        std::vector<std::string> moves;
        std::string move;
        for (char c : solution) {
            if (c == ' ') {
                if (!move.empty()) {
                    moves.push_back(move);
                    move.clear();
                }
            } else {
                move += c;
            }
        }
        if (!move.empty()) {
            moves.push_back(move);
        }
        return moves;
    }
};

#endif