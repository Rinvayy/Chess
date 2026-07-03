#include "board.h"
#include <iostream>
#include <string>

int main() {
    Board board;

    std::cout << "Шахматная доска\n";
    board.printBoard();
    
    std::cout << "\nТекущий игрок: " 
              << (board.getCurrentPlayer() == Color::WHITE ? "Белые" : "Черные")
              << "\n";
    
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