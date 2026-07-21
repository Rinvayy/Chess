#include "TaskManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

TaskManager::TaskManager() {
    loadTasks();
    if (tasks.empty()) {
        initializeDefaultTasks();
        saveTasks();
    }
}

TaskManager::~TaskManager() {
    saveTasks();
}

void TaskManager::initializeDefaultTasks() {
    tasks.clear();
    
    // задача 1: Мат в 1 ход
    ChessTask t1;
    t1.name = "Мат в 1 ход #1";
    t1.fen = "rnb1kbnr/pppp1ppp/8/4p3/5P2/8/PPPPP1PP/RNBQKBNR w KQkq - 0 1";
    t1.solution = "f4e5";
    t1.difficulty = 1;
    t1.description = "Поставьте мат в 1 ход";
    t1.isUserCreated = false;
    tasks.push_back(t1);
    
    // задача 2: Мат в 1 ход
    ChessTask t2;
    t2.name = "Мат в 1 ход #2";
    t2.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1";
    t2.solution = "h5f7";
    t2.difficulty = 1;
    t2.description = "Поставьте мат в 1 ход (ферзем)";
    t2.isUserCreated = false;
    tasks.push_back(t2);
    
    // задача 3: Мат в 1 ход
    ChessTask t3;
    t3.name = "Мат в 1 ход #3";
    t3.fen = "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/8/PPPP1PPP/RNBQK1NR w KQkq - 0 1";
    t3.solution = "c4f7";
    t3.difficulty = 1;
    t3.description = "Поставьте мат в 1 ход (слоном)";
    t3.isUserCreated = false;
    tasks.push_back(t3);
    
    // задача 4: Мат в 2 хода
    ChessTask t4;
    t4.name = "Мат в 2 хода #1";
    t4.fen = "r1bqk2r/pppp1ppp/2n2n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1";
    t4.solution = "h5f7";
    t4.difficulty = 2;
    t4.description = "Поставьте мат в 2 хода";
    t4.isUserCreated = false;
    tasks.push_back(t4);
    
    // задача 5: Мат в 2 хода
    ChessTask t5;
    t5.name = "Мат в 2 хода #2";
    t5.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2NP4/PPP2PPP/R1BQKBNR w KQkq - 0 1";
    t5.solution = "d3e5";
    t5.difficulty = 2;
    t5.description = "Найдите мат в 2 хода";
    t5.isUserCreated = false;
    tasks.push_back(t5);
    
    // задача 6: Выигрыш фигуры
    ChessTask t6;
    t6.name = "Выигрыш ферзя";
    t6.fen = "rnbqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1";
    t6.solution = "d4d5";
    t6.difficulty = 3;
    t6.description = "Выиграйте ферзя в 1 ход";
    t6.isUserCreated = false;
    tasks.push_back(t6);
    
    // задача 7: Мат в 3 хода
    ChessTask t7;
    t7.name = "Мат в 3 хода #1";
    t7.fen = "r1bqk2r/pppp1ppp/2n2n2/4p3/2B1P3/3P4/PPP2PPP/RNBQK1NR w KQkq - 0 1";
    t7.solution = "c4f7";
    t7.difficulty = 3;
    t7.description = "Найдите мат в 3 хода";
    t7.isUserCreated = false;
    tasks.push_back(t7);
    
    // задача 8: Вилка
    ChessTask t8;
    t8.name = "Вилка конем";
    t8.fen = "rnbqkbnr/pppp1ppp/8/4p3/3P4/2N5/PPP1PPPP/R1BQKBNR w KQkq - 0 1";
    t8.solution = "c3e4";
    t8.difficulty = 3;
    t8.description = "Сделайте вилку конем";
    t8.isUserCreated = false;
    tasks.push_back(t8);
    
    // задача 9: Связка
    ChessTask t9;
    t9.name = "Связка";
    t9.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p3/3P4/2N5/PPP1PPPP/R1BQKBNR w KQkq - 0 1";
    t9.solution = "c1g5";
    t9.difficulty = 4;
    t9.description = "Создайте связку";
    t9.isUserCreated = false;
    tasks.push_back(t9);
    
    // задача 10: Эндшпиль
    ChessTask t10;
    t10.name = "Эндшпиль";
    t10.fen = "8/8/8/8/4K3/8/8/4k3 w - - 0 1";
    t10.solution = "e4e3";
    t10.difficulty = 4;
    t10.description = "Выиграйте эндшпиль";
    t10.isUserCreated = false;
    tasks.push_back(t10);
}

void TaskManager::loadTasks() {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return;
    
    tasks.clear();
    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    for (int i = 0; i < count; ++i) {
        ChessTask task;
        int nameLen, fenLen, solLen, descLen;
        
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        task.name.resize(nameLen);
        file.read(&task.name[0], nameLen);
        
        file.read(reinterpret_cast<char*>(&fenLen), sizeof(fenLen));
        task.fen.resize(fenLen);
        file.read(&task.fen[0], fenLen);
        
        file.read(reinterpret_cast<char*>(&solLen), sizeof(solLen));
        task.solution.resize(solLen);
        file.read(&task.solution[0], solLen);
        
        file.read(reinterpret_cast<char*>(&task.difficulty), sizeof(task.difficulty));
        
        file.read(reinterpret_cast<char*>(&descLen), sizeof(descLen));
        task.description.resize(descLen);
        file.read(&task.description[0], descLen);
        
        file.read(reinterpret_cast<char*>(&task.isUserCreated), sizeof(task.isUserCreated));
        
        tasks.push_back(task);
    }
    file.close();
}

void TaskManager::saveTasks() {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return;
    
    int count = tasks.size();
    file.write(reinterpret_cast<char*>(&count), sizeof(count));
    
    for (const auto& task : tasks) {
        int nameLen = task.name.size();
        file.write(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        file.write(task.name.c_str(), nameLen);
        
        int fenLen = task.fen.size();
        file.write(reinterpret_cast<char*>(&fenLen), sizeof(fenLen));
        file.write(task.fen.c_str(), fenLen);
        
        int solLen = task.solution.size();
        file.write(reinterpret_cast<char*>(&solLen), sizeof(solLen));
        file.write(task.solution.c_str(), solLen);
        
        file.write(reinterpret_cast<const char*>(&task.difficulty), sizeof(task.difficulty));
        
        int descLen = task.description.size();
        file.write(reinterpret_cast<char*>(&descLen), sizeof(descLen));
        file.write(task.description.c_str(), descLen);
        
        file.write(reinterpret_cast<const char*>(&task.isUserCreated), sizeof(task.isUserCreated));
    }
    file.close();
}

void TaskManager::addTask(const ChessTask& task) {
    tasks.push_back(task);
    saveTasks();
}

void TaskManager::removeTask(int index) {
    if (index >= 0 && index < (int)tasks.size()) {
        tasks.erase(tasks.begin() + index);
        saveTasks();
    }
}

void TaskManager::editTask(int index, const ChessTask& task) {
    if (index >= 0 && index < (int)tasks.size()) {
        tasks[index] = task;
        saveTasks();
    }
}