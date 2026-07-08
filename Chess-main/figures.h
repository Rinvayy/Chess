#ifndef FIGURES_H
#define FIGURES_H

#include <string>
#include <cctype>

enum class FigureType {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum class Color {
    NONE,
    WHITE,
    BLACK
};

class Figure {
public:
    FigureType type = FigureType::NONE;
    Color color = Color::NONE;
    
    Figure() = default;
    Figure(FigureType t, Color c) : type(t), color(c) {}

    bool isEmpty() const {
        return type == FigureType::NONE;
    }
    
    char getSymbol() const {
        if (isEmpty()) {
            return '.';
        }
        
        char symbol;
        switch (type) {
            case FigureType::PAWN:   symbol = 'P'; break;
            case FigureType::KNIGHT: symbol = 'N'; break;
            case FigureType::BISHOP: symbol = 'B'; break;
            case FigureType::ROOK:   symbol = 'R'; break;
            case FigureType::QUEEN:  symbol = 'Q'; break;
            case FigureType::KING:   symbol = 'K'; break;
            default:                symbol = '?'; break;
        }

        return (color == Color::WHITE) ? symbol : tolower(symbol);
    }

    std::string getUnicodeSymbol() const {
        if (isEmpty()) {
            return "· ";
        }
        
        if (color == Color::WHITE) {
            switch (type) {
                case FigureType::KING:   return "♔ ";
                case FigureType::QUEEN:  return "♕ ";
                case FigureType::ROOK:   return "♖ ";
                case FigureType::BISHOP: return "♗ ";
                case FigureType::KNIGHT: return "♘ ";
                case FigureType::PAWN:   return "♙ ";
                default:                return "  ";
            }
        } else {
            switch (type) {
                case FigureType::KING:   return "♚ ";
                case FigureType::QUEEN:  return "♛ ";
                case FigureType::ROOK:   return "♜ ";
                case FigureType::BISHOP: return "♝ ";
                case FigureType::KNIGHT: return "♞ ";
                case FigureType::PAWN:   return "♟ ";
                default:                return "  ";
            }
        }
    }
};

#endif