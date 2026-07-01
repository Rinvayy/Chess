#include <ctype.h>
#include <string>
#ifndef PIECE_H
#define PIECE_H

enum class FigureType {
    NONE,    // 0 - пустая клетка
    PAWN,    // 1 - пешка
    KNIGHT,  // 2 - конь
    BISHOP,  // 3 - слон
    ROOK,    // 4 - ладья
    QUEEN,   // 5 - ферзь
    KING     // 6 - король
};

enum class Color {
    NONE,   // 0 - нет цвета (для пустых клеток)
    WHITE,  // 1 - белый цвет
    BLACK   // 2 - черный цвет
};

class Figure {       //класс фигуры
public:
    FigureType type;
    Color color;
    
    Figure() : type(FigureType::NONE), color(Color::NONE) {
        //конструктор по умолчанию создает пустую клетку (нет фигуры)
    }

    Figure(FigureType t, Color c) : type(t), color(c) {
        //конструктор с параметрами создает фигуру указанного типа и цвета
    }

    bool isEmpty() const {
        //проверяет, является ли клетка пустой
        return type == FigureType::NONE;
    }
    
    char getSymbol() const {
        //возвращает символ фигуры для текстового вывода
        if (isEmpty()) {
            return '.';
        }
        
        char symbol;        //определяем символ в зависимости от типа фигуры
        switch (type) {
            case FigureType::PAWN:   symbol = 'P'; break;   //пешка
            case FigureType::KNIGHT: symbol = 'N'; break;   //конь (K уже занят королем)
            case FigureType::BISHOP: symbol = 'B'; break;   //слон
            case FigureType::ROOK:   symbol = 'R'; break;   //ладья
            case FigureType::QUEEN:  symbol = 'Q'; break;   //ферзь
            case FigureType::KING:   symbol = 'K'; break;   //король
            default:                symbol = '?'; break;   //неизвестная фигура
        }

        return (color == Color::WHITE) ? symbol : tolower(symbol);  //возвращаем символ в нижнем регистре для черных фигур
    }

    std::string getUnicodeSymbol() const {  //возвращает юникод-символ фигуры для графического вывода
        if (isEmpty()) {
            return "· ";
        }
        
        //для белых фигур - белые юникod-символы
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
        } 
        //для черных фигур - черные юникod-символы
        else {
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