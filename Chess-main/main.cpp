#include "board.h"
#include "ai.h"
#include "fen.h"
#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include <fstream>

// функция вывода справки по командам
void printHelp() {
    std::cout << "\nКоманды:\n";
    std::cout << "  move <from> <to>  - сделать ход (например: move e2 e4)\n";
    std::cout << "  print             - показать доску\n";
    std::cout << "  ai                - сделать ход компьютера\n";
    std::cout << "  switch            - включить/выключить режим игры с компьютером\n";
    std::cout << "  save              - сохранить игру\n";
    std::cout << "  load              - загрузить игру\n";
    std::cout << "  help              - показать эту справку\n";
    std::cout << "  exit              - выйти из игры\n\n";
}

int main() {
    Board board;
    AI ai(&board);
    bool aiMode = false;
    Color humanColor = Color::WHITE;
    Color aiColor = Color::BLACK;
    
    // проверяем наличие сохраненной игры
    std::ifstream saveFile("save.fen");
    if (saveFile.good()) {
        saveFile.close();
        std::cout << "Найдена сохраненная партия. Загрузить? (y/n): ";
        std::string answer;
        std::cin >> answer;
        if (answer == "y" || answer == "Y") {
            if (FEN::loadGame("save.fen", board)) {
                std::cout << "Игра загружена!\n";
            } else {
                std::cout << "Ошибка загрузки. Начинаем новую игру.\n";
            }
        } else {
            std::cout << "Начинаем новую игру.\n";
        }
    }
    
    std::cout << "Шахматы с компьютером\n";
    board.printBoard();
    printHelp();
    
    std::string cmd;
    
    while (true) {
        if (aiMode && board.getCurrentPlayer() == aiColor) {
            std::cout << "\nКомпьютер думает...\n";
            Move bestMove = ai.getBestMove(aiColor);
            
            if (bestMove.fromRow == -1) {
                std::cout << "У компьютера нет доступных ходов!\n";
                if (board.isInCheck(aiColor)) {
                    std::cout << "ШАХ И МАТ! Вы победили!\n";
                } else {
                    std::cout << "ПАТ! Ничья!\n";
                }
                break;
            }
            
            char fromCol = 'a' + bestMove.fromCol;
            int fromRow = 8 - bestMove.fromRow;
            char toCol = 'a' + bestMove.toCol;
            int toRow = 8 - bestMove.toRow;
            
            std::cout << "Компьютер делает ход: " << fromCol << fromRow << " " << toCol << toRow << "\n";
            
            // выполняем ход
            if (board.makeMove(bestMove.fromRow, bestMove.fromCol, bestMove.toRow, bestMove.toCol)) {
                board.printBoard();
                
                // проверяем окончание игры после хода компьютера
                Color current = board.getCurrentPlayer();
                if (board.isCheckmate(current)) {
                    std::cout << "\nШАХ И МАТ! " 
                              << (current == Color::WHITE ? "Белые" : "Черные") 
                              << " проиграли!\n";
                    if (current == aiColor) {
                        std::cout << "Поздравляем! Вы победили!\n";
                    } else {
                        std::cout << "Компьютер победил! Попробуйте еще раз.\n";
                    }
                    break;
                } else if (board.isStalemate(current)) {
                    std::cout << "\nПАТ! Ничья!\n";
                    break;
                } else if (board.isInCheck(current)) {
                    std::cout << "\nШАХ! Король " 
                              << (current == Color::WHITE ? "белых" : "черных") 
                              << " под шахом!\n";
                }
                // передаем ход другому игроку
                board.switchPlayer();
            }
            continue;
        }
        
        std::cout << "\n> ";
        std::cin >> cmd;
        
        // обработка команд
        if (cmd == "exit") {
            // сохраняем игру перед выходом
            FEN::saveGame(board, "save.fen");
            std::cout << "Игра сохранена. До свидания!\n";
            break;
        } 
        else if (cmd == "help") {
            printHelp();
        }
        else if (cmd == "print") {
            board.printBoard();
        }
        else if (cmd == "switch") {
            // переключение режима игры с компьютером
            aiMode = !aiMode;
            if (aiMode) {
                std::cout << "Режим игры с компьютером ВКЛЮЧЕН. Вы играете за " 
                          << (humanColor == Color::WHITE ? "белых" : "черных") 
                          << ", компьютер за " << (aiColor == Color::WHITE ? "белых" : "черных") << ".\n";
                if (board.getCurrentPlayer() == aiColor) {
                    std::cout << "Ход компьютера...\n";
                }
            } else {
                std::cout << "Режим игры с компьютером ВЫКЛЮЧЕН. Играйте вдвоем.\n";
            }
        }
        else if (cmd == "ai") {
            // принудительный ход компьютера
            if (!aiMode) {
                std::cout << "Сначала включите режим игры с компьютером командой 'switch'\n";
                continue;
            }
            
            if (board.getCurrentPlayer() != aiColor) {
                std::cout << "Сейчас не ход компьютера!\n";
                continue;
            }
            
            Move bestMove = ai.getBestMove(aiColor);
            if (bestMove.fromRow == -1) {
                std::cout << "У компьютера нет доступных ходов!\n";
                if (board.isInCheck(aiColor)) {
                    std::cout << "ШАХ И МАТ! Вы победили!\n";
                } else {
                    std::cout << "ПАТ! Ничья!\n";
                }
                break;
            }
            
            char fromCol = 'a' + bestMove.fromCol;
            int fromRow = 8 - bestMove.fromRow;
            char toCol = 'a' + bestMove.toCol;
            int toRow = 8 - bestMove.toRow;
            
            std::cout << "Компьютер делает ход: " << fromCol << fromRow << " " << toCol << toRow << "\n";
            
            if (board.makeMove(bestMove.fromRow, bestMove.fromCol, bestMove.toRow, bestMove.toCol)) {
                board.printBoard();
                
                Color current = board.getCurrentPlayer();
                if (board.isCheckmate(current)) {
                    std::cout << "\nШАХ И МАТ! " 
                              << (current == Color::WHITE ? "Белые" : "Черные") 
                              << " проиграли!\n";
                    if (current == aiColor) {
                        std::cout << "Поздравляем! Вы победили!\n";
                    } else {
                        std::cout << "Компьютер победил! Попробуйте еще раз.\n";
                    }
                    break;
                } else if (board.isStalemate(current)) {
                    std::cout << "\nПАТ! Ничья!\n";
                    break;
                } else if (board.isInCheck(current)) {
                    std::cout << "\nШАХ! Король " 
                              << (current == Color::WHITE ? "белых" : "черных") 
                              << " под шахом!\n";
                }
                
                board.switchPlayer();
            }
        }
        else if (cmd == "move") {
            // обработка хода игрока
            std::string from, to;
            std::cin >> from >> to;
            
            if (from.length() != 2 || to.length() != 2) {
                std::cout << "Неверный формат! Используйте: move e2 e4\n";
                continue;
            }
            
            int fromCol = tolower(from[0]) - 'a';
            int fromRow = 8 - (from[1] - '0');
            int toCol = tolower(to[0]) - 'a';
            int toRow = 8 - (to[1] - '0');
            
            if (fromCol < 0 || fromCol > 7 || fromRow < 0 || fromRow > 7 ||
                toCol < 0 || toCol > 7 || toRow < 0 || toRow > 7) {
                std::cout << "Неверные координаты! Используйте a1-h8\n";
                continue;
            }
            
            Figure fig = board.getFigure(fromRow, fromCol);
            if (fig.isEmpty()) {
                std::cout << "На этой клетке нет фигуры!\n";
                continue;
            }
            
            if (aiMode && fig.color != humanColor) {
                std::cout << "Сейчас ход " 
                          << (board.getCurrentPlayer() == Color::WHITE ? "белых" : "черных") 
                          << "!\n";
                continue;
            }
            
            if (board.makeMove(fromRow, fromCol, toRow, toCol)) {
                board.printBoard();
                
                Color current = board.getCurrentPlayer();
                if (board.isCheckmate(current)) {
                    std::cout << "\nШАХ И МАТ! " 
                              << (current == Color::WHITE ? "Белые" : "Черные") 
                              << " проиграли!\n";
                    if (aiMode && current == aiColor) {
                        std::cout << "Поздравляем! Вы победили!\n";
                    } else if (aiMode && current == humanColor) {
                        std::cout << "Компьютер победил! Попробуйте еще раз.\n";
                    }
                    break;
                } else if (board.isStalemate(current)) {
                    std::cout << "\nПАТ! Ничья!\n";
                    break;
                } else if (board.isInCheck(current)) {
                    std::cout << "\nШАХ! Король " 
                              << (current == Color::WHITE ? "белых" : "черных") 
                              << " под шахом!\n";
                }
                
                board.switchPlayer();
            } else {
                std::cout << "Неверный ход! Попробуйте снова.\n";
            }
        }
        else if (cmd == "save") {
            // сохранение игры
            if (FEN::saveGame(board, "save.fen")) {
                std::cout << "Игра сохранена!\n";
            } else {
                std::cout << "Ошибка сохранения!\n";
            }
        }
        else if (cmd == "load") {
            // загрузка игры
            if (FEN::loadGame("save.fen", board)) {
                std::cout << "Игра загружена!\n";
                board.printBoard();
            } else {
                std::cout << "Ошибка загрузки!\n";
            }
        }
        else {
            std::cout << "Неизвестная команда. Введите 'help' для справки.\n";
        }
    }
    std::cout << "\nИгра завершена. Спасибо за игру!\n";
    return 0;
}