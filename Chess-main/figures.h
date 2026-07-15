#ifndef FIGURES_H
#define FIGURES_H
#include <string>
#include <cctype>

enum class FigureType {
    NONE,    //пустая клетка
    KING,    //король
    QUEEN,   //ферзь
    ROOK,    //ладья
    BISHOP,  //слон
    KNIGHT,  //конь
    PAWN     //пешка
};

enum class Color {
    NONE,
    WHITE,
    BLACK
};

class Figure {
public:
    FigureType type;
    Color color;
    
    Figure() : type(FigureType::NONE), color(Color::NONE) {}
    
    Figure(FigureType t, Color c) : type(t), color(c) {}
    
    bool isEmpty() const {
        return type == FigureType::NONE;
    }
    
    char getSymbol() const {
        if (isEmpty()) return '.';
        
        char symbol;
        switch (type) {
            case FigureType::KING:   symbol = 'K'; break;
            case FigureType::QUEEN:  symbol = 'Q'; break;
            case FigureType::ROOK:   symbol = 'R'; break;
            case FigureType::BISHOP: symbol = 'B'; break;
            case FigureType::KNIGHT: symbol = 'N'; break;
            case FigureType::PAWN:   symbol = 'P'; break;
            default: return '.';
        }
        return (color == Color::WHITE) ? symbol : std::tolower(symbol);
    }
    
    std::string getUnicodeSymbol() const {
        if (isEmpty()) return "· ";
        
        if (color == Color::WHITE) {
            switch (type) {
                case FigureType::KING:   return "♔ ";
                case FigureType::QUEEN:  return "♕ ";
                case FigureType::ROOK:   return "♖ ";
                case FigureType::BISHOP: return "♗ ";
                case FigureType::KNIGHT: return "♘ ";
                case FigureType::PAWN:   return "♙ ";
                default: return "  ";
            }
        } else {
            switch (type) {
                case FigureType::KING:   return "♚ ";
                case FigureType::QUEEN:  return "♛ ";
                case FigureType::ROOK:   return "♜ ";
                case FigureType::BISHOP: return "♝ ";
                case FigureType::KNIGHT: return "♞ ";
                case FigureType::PAWN:   return "♟ ";
                default: return "  ";
            }
        }
    }
};

#endif