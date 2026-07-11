#ifndef FEN_H
#define FEN_H

#include "board.h"
#include <string>

class FEN {
public:
    // сохраняет текущую позицию в формате FEN
    static std::string toFEN(const Board& board);
    // загружает позицию из FEN строки
    static bool fromFEN(const std::string& fen, Board& board);
    // сохраняет игру в файл
    static bool saveGame(const Board& board, const std::string& filename);
    // загружает игру из файла
    static bool loadGame(const std::string& filename, Board& board);
};

#endif