#ifndef TASKBOARD_H
#define TASKBOARD_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <memory>
#include "board.h"
#include "PieceRenderer.h"
#include "TaskManager.h"
#include "ai.h"
#include "fen.h"

class TaskBoard {
private:
    static const int SIZE = 8;
    static const int CELL_SIZE = 80;
    static const int BOARD_SIZE = SIZE * CELL_SIZE;
    static const int WINDOW_WIDTH = BOARD_SIZE + 410;
    static const int WINDOW_HEIGHT = BOARD_SIZE + 150;
    
    sf::RenderWindow window;
    sf::Font font;
    PieceRenderer pieceRenderer;
    
    Board board;
    AI ai;
    
    TaskManager taskManager;
    int currentTaskIndex = 0;
    
    int selectedRow, selectedCol;
    bool hasSelected = false;
    bool isBoardFlipped = false;
    
    std::vector<std::string> solutionMoves;
    int currentMoveIndex = 0;
    bool isSolved = false;
    bool isTaskCompleted = false;
    std::string message;
    sf::Clock messageClock;
    
    sf::RectangleShape backButton;
    std::unique_ptr<sf::Text> backText;
    
    sf::RectangleShape nextButton;
    std::unique_ptr<sf::Text> nextText;
    
    sf::RectangleShape prevButton;
    std::unique_ptr<sf::Text> prevText;
    
    sf::RectangleShape resetButton;
    std::unique_ptr<sf::Text> resetText;
    
    bool isBackHovered = false;
    bool isNextHovered = false;
    bool isPrevHovered = false;
    bool isResetHovered = false;
    
public:
    TaskBoard();
    ~TaskBoard();
    void run();
    void setTask(int index);
    
private:
    void drawBoard();
    void drawCoordinate(int row, int col);
    void drawPieces();
    void drawInfoPanel();
    void drawButtons();
    void handleClick(int x, int y);
    void handleButtonClick(int x, int y);
    void loadCurrentTask();
    void checkSolution(const std::string& move);
    void resetTask();
    void showMessage(const std::string& msg);
};

#endif