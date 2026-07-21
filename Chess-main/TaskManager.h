#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>
#include <string>

class TaskManager {
private:
    std::vector<ChessTask> tasks;
    std::string filename = "tasks.dat";
    
public:
    TaskManager();
    ~TaskManager();
    
    void loadTasks();
    void saveTasks();
    void addTask(const ChessTask& task);
    void removeTask(int index);
    void editTask(int index, const ChessTask& task);
    const std::vector<ChessTask>& getTasks() const { return tasks; }
    int getTaskCount() const { return tasks.size(); }
    
    void initializeDefaultTasks();
};

#endif