#include "board.h"
#include <iostream>
#include <string>

int main() {
    Board board;

    std::cout << "Пустая доска\n";
    board.printBoard();
    
    std::cout << "\nТекущий игрок: " 
              << (board.getCurrentPlayer() == Color::WHITE ? "Белые" : "Черные")
              << "\n";
    
    board.switchPlayer();
    std::cout << "После переключения: " 
              << (board.getCurrentPlayer() == Color::WHITE ? "Белые" : "Черные") 
              << "\n";
    
    board.switchPlayer();

    std::cout << "\nПроверка клеток:\n";
    bool allEmpty = true;  
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            //getFigure() возвращает фигуру на клетке
            //isEmpty() проверяет, пустая ли клетка
            if (!board.getFigure(r, c).isEmpty()) {
                // Если нашли непустую клетку - запоминаем
                allEmpty = false;
                std::cout << "  Ошибка! Клетка [" << r << "][" << c << "] не пустая\n";
            }
        }
    }
    
    if (allEmpty) {
        std::cout << "  Все клетки пустые\n";
    }
    
    // Все эти методы пока возвращают false (заглушки)
    std::cout << "\nПроверка методов (все должны вернуть false):\n";
    std::cout << "  makeMove(): " << board.makeMove(0,0,0,0) << " (ожидается 0)\n";
    std::cout << "  isInCheck(): " << board.isInCheck(Color::WHITE) << " (ожидается 0)\n";
    std::cout << "  isCheckmate(): " << board.isCheckmate(Color::WHITE) << " (ожидается 0)\n";
    std::cout << "  isStalemate(): " << board.isStalemate(Color::WHITE) << " (ожидается 0)\n";
    
    std::cout << "\nИнтерактивный режим:\n";
    std::cout << "Команды: print - показать доску, exit - выход\n";
    
    std::string cmd;
    
    while (true) {
        std::cout << "> ";
        std::cin >> cmd;
        
        if (cmd == "exit") {
            std::cout << "До свидания!\n";
            break;
        } 

        else if (cmd == "print") {
            board.printBoard();
        } 

        else {
            std::cout << "Неизвестная команда. Доступно: print, exit\n";
        }
    }

    return 0;
}