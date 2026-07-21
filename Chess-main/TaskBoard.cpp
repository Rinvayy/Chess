#include "TaskBoard.h"
#include <iostream>
#include <sstream>
#include <cstdio>

TaskBoard::TaskBoard() 
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Chess Tasks"),
      ai(&board),
      isBoardFlipped(false),
      isSolved(false),
      isTaskCompleted(false) {
    
    if (!font.openFromFile("font.ttf")) {
        std::cout << "font.ttf не загружен, пробуем системный...\n";
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cout << "Не удалось загрузить шрифт!\n";
        } else {
            std::cout << "Загружен Arial\n";
        }
    } else {
        std::cout << "Загружен font.ttf\n";
    }
    
    if (!pieceRenderer.loadTextures()) {
        std::cout << "Не удалось загрузить текстуры фигур\n";
    }
    
    backText = std::make_unique<sf::Text>(font);
    nextText = std::make_unique<sf::Text>(font);
    prevText = std::make_unique<sf::Text>(font);
    resetText = std::make_unique<sf::Text>(font);
    
    backButton.setSize(sf::Vector2f({140, 45}));
    backButton.setFillColor(sf::Color(139, 69, 19));
    backButton.setOutlineColor(sf::Color(100, 50, 20));
    backButton.setOutlineThickness(2);
    backButton.setPosition({15, 15});
    
    std::string backStr = "Назад";
    backText->setString(sf::String::fromUtf8(backStr.begin(), backStr.end()));
    backText->setCharacterSize(20);
    backText->setFillColor(sf::Color::White);
    backText->setStyle(sf::Text::Bold);
    
    float backTextX = 15 + (140 - backText->getLocalBounds().size.x) / 2;
    float backTextY = 15 + (45 - backText->getLocalBounds().size.y) / 2 - 3;
    backText->setPosition({backTextX, backTextY});
    
    prevButton.setSize(sf::Vector2f({90, 40}));
    prevButton.setFillColor(sf::Color(80, 40, 20));
    prevButton.setOutlineColor(sf::Color(60, 30, 10));
    prevButton.setOutlineThickness(2);
    prevButton.setPosition({(float)BOARD_SIZE + 45, (float)BOARD_SIZE + 15});
    
    std::string prevStr = "Пред";
    prevText->setString(sf::String::fromUtf8(prevStr.begin(), prevStr.end()));
    prevText->setCharacterSize(18);
    prevText->setFillColor(sf::Color::White);
    prevText->setStyle(sf::Text::Bold);
    
    float prevTextX = BOARD_SIZE + 30 + (110 - prevText->getLocalBounds().size.x) / 2;
    float prevTextY = BOARD_SIZE + 15 + (40 - prevText->getLocalBounds().size.y) / 2 - 3;
    prevText->setPosition({(float)prevTextX, (float)prevTextY});
    
    nextButton.setSize(sf::Vector2f({90, 40}));
    nextButton.setFillColor(sf::Color(80, 40, 20));
    nextButton.setOutlineColor(sf::Color(60, 30, 10));
    nextButton.setOutlineThickness(2);
    nextButton.setPosition({(float)BOARD_SIZE + 165, (float)BOARD_SIZE + 15});
    
    std::string nextStr = "След";
    nextText->setString(sf::String::fromUtf8(nextStr.begin(), nextStr.end()));
    nextText->setCharacterSize(18);
    nextText->setFillColor(sf::Color::White);
    nextText->setStyle(sf::Text::Bold);
    
    float nextTextX = BOARD_SIZE + 155 + (110 - nextText->getLocalBounds().size.x) / 2;
    float nextTextY = BOARD_SIZE + 15 + (40 - nextText->getLocalBounds().size.y) / 2 - 3;
    nextText->setPosition({(float)nextTextX, (float)nextTextY});
    
    resetButton.setSize(sf::Vector2f({90, 40}));
    resetButton.setFillColor(sf::Color(139, 30, 20));
    resetButton.setOutlineColor(sf::Color(100, 20, 10));
    resetButton.setOutlineThickness(2);
    resetButton.setPosition({(float)BOARD_SIZE + 285, (float)BOARD_SIZE + 15});
    
    std::string resetStr = "Сброс";
    resetText->setString(sf::String::fromUtf8(resetStr.begin(), resetStr.end()));
    resetText->setCharacterSize(18);
    resetText->setFillColor(sf::Color::White);
    resetText->setStyle(sf::Text::Bold);
    
    float resetTextX = BOARD_SIZE + 280 + (110 - resetText->getLocalBounds().size.x) / 2;
    float resetTextY = BOARD_SIZE + 15 + (40 - resetText->getLocalBounds().size.y) / 2 - 3;
    resetText->setPosition({(float)resetTextX, (float)resetTextY});
    
    loadCurrentTask();
}

TaskBoard::~TaskBoard() {}

void TaskBoard::setTask(int index) {
    if (index >= 0 && index < taskManager.getTaskCount()) {
        currentTaskIndex = index;
        loadCurrentTask();
        resetTask();
    }
}

void TaskBoard::loadCurrentTask() {
    const auto& tasks = taskManager.getTasks();
    if (currentTaskIndex < 0 || currentTaskIndex >= (int)tasks.size()) {
        currentTaskIndex = 0;
    }
    
    const ChessTask& task = tasks[currentTaskIndex];
    FEN::fromFEN(task.fen, board);
    solutionMoves = task.getSolutionMoves();
    currentMoveIndex = 0;
    isSolved = false;
    isTaskCompleted = false;
    
    isBoardFlipped = (board.getCurrentPlayer() == Color::BLACK);
    hasSelected = false;
}

void TaskBoard::resetTask() {
    loadCurrentTask();
    showMessage("Задача загружена");
}

void TaskBoard::run() {
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mousePos = event->getIf<sf::Event::MouseButtonPressed>();
                if (mousePos && mousePos->button == sf::Mouse::Button::Left) {
                    handleButtonClick(mousePos->position.x, mousePos->position.y);
                    handleClick(mousePos->position.x, mousePos->position.y);
                }
            }
            
            if (event->is<sf::Event::MouseMoved>()) {
                auto mousePos = event->getIf<sf::Event::MouseMoved>();
                if (mousePos) {
                    isBackHovered = backButton.getGlobalBounds().contains(
                        {(float)mousePos->position.x, (float)mousePos->position.y});
                    isNextHovered = nextButton.getGlobalBounds().contains(
                        {(float)mousePos->position.x, (float)mousePos->position.y});
                    isPrevHovered = prevButton.getGlobalBounds().contains(
                        {(float)mousePos->position.x, (float)mousePos->position.y});
                    isResetHovered = resetButton.getGlobalBounds().contains(
                        {(float)mousePos->position.x, (float)mousePos->position.y});
                }
            }
        }
        
        window.clear(sf::Color(30, 30, 30));
        drawBoard();
        drawPieces();
        drawInfoPanel();
        drawButtons();
        window.display();
    }
}

void TaskBoard::drawBoard() {
    float offsetX = 30;
    float offsetY = 70;
    
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            int boardRow, boardCol;
            if (isBoardFlipped) {
                boardRow = SIZE - 1 - row;
                boardCol = SIZE - 1 - col;
            } else {
                boardRow = row;
                boardCol = col;
            }
            
            sf::RectangleShape cell(sf::Vector2f({(float)CELL_SIZE, (float)CELL_SIZE}));
            cell.setPosition({(float)(col * CELL_SIZE + offsetX), (float)(row * CELL_SIZE + offsetY)});
            
            sf::Color color;
            if ((boardRow + boardCol) % 2 == 0) {
                color = sf::Color(240, 217, 181);
            } else {
                color = sf::Color(181, 136, 99);
            }
            
            if (hasSelected && selectedRow == boardRow && selectedCol == boardCol) {
                color = sf::Color(255, 255, 100);
            }
            
            cell.setFillColor(color);
            cell.setOutlineColor(sf::Color(100, 100, 100));
            cell.setOutlineThickness(1);
            window.draw(cell);
            drawCoordinate(row, col);
        }
    }
}

void TaskBoard::drawCoordinate(int row, int col) {
    sf::Text text(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color(100, 100, 100));
    
    int boardRow, boardCol;
    if (isBoardFlipped) {
        boardRow = SIZE - 1 - row;
        boardCol = SIZE - 1 - col;
    } else {
        boardRow = row;
        boardCol = col;
    }
    
    float offsetX = 30;
    float offsetY = 70;
    
    if (row == 7) {
        char letter = 'a' + boardCol;
        text.setString(std::string(1, letter));
        float x = col * CELL_SIZE + CELL_SIZE / 2 - 8 + offsetX;
        float y = row * CELL_SIZE + CELL_SIZE - 5 + offsetY;
        text.setPosition({x, y});
        window.draw(text);
    }
    
    if (col == 0) {
        text.setString(std::to_string(8 - boardRow));
        float x = col * CELL_SIZE - 25 + offsetX;
        float y = row * CELL_SIZE + CELL_SIZE / 2 - 12 + offsetY;
        text.setPosition({x, y});
        window.draw(text);
    }
}

void TaskBoard::drawPieces() {
    float offsetX = 30;
    float offsetY = 70;
    
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            int boardRow, boardCol;
            if (isBoardFlipped) {
                boardRow = SIZE - 1 - row;
                boardCol = SIZE - 1 - col;
            } else {
                boardRow = row;
                boardCol = col;
            }
            
            Figure fig = board.getFigure(boardRow, boardCol);
            if (!fig.isEmpty()) {
                float x = col * CELL_SIZE + offsetX;
                float y = row * CELL_SIZE + offsetY;
                pieceRenderer.drawPiece(window, fig, x, y, CELL_SIZE);
            }
        }
    }
}

void TaskBoard::drawInfoPanel() {
    float panelX = 30 + BOARD_SIZE + 10;
    
    sf::RectangleShape panel(sf::Vector2f({340, WINDOW_HEIGHT - 100}));
    panel.setPosition({panelX, 40});
    panel.setFillColor(sf::Color(50, 50, 50, 220));
    panel.setOutlineColor(sf::Color(100, 100, 100));
    panel.setOutlineThickness(2);
    window.draw(panel);
    
    int yPos = 60;
    
    const auto& tasks = taskManager.getTasks();
    if (currentTaskIndex < 0 || currentTaskIndex >= (int)tasks.size()) return;
    const ChessTask& task = tasks[currentTaskIndex];
    
    sf::Text title(font);
    title.setString(sf::String::fromUtf8(task.name.begin(), task.name.end()));
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition({panelX + 20, (float)yPos});
    window.draw(title);
    yPos += 45;
    
    sf::Text desc(font);
    desc.setString(sf::String::fromUtf8(task.description.begin(), task.description.end()));
    desc.setCharacterSize(18);
    desc.setFillColor(sf::Color(200, 200, 200));
    desc.setPosition({panelX + 20, (float)yPos});
    window.draw(desc);
    yPos += 40;
    
    sf::Text diffText(font);
    std::string stars;
    for (int i = 0; i < task.difficulty; ++i) stars += "*";
    for (int i = task.difficulty; i < 5; ++i) stars += " ";
    std::string diffStr = "Сложность: " + stars;
    diffText.setString(sf::String::fromUtf8(diffStr.begin(), diffStr.end()));
    diffText.setCharacterSize(18);
    diffText.setFillColor(sf::Color(255, 215, 0));
    diffText.setPosition({panelX + 20, (float)yPos});
    window.draw(diffText);
    yPos += 40;
    
    std::string statusStr;
    sf::Color statusColor;
    if (isTaskCompleted) {
        statusStr = "ЗАДАЧА РЕШЕНА!";
        statusColor = sf::Color::Green;
    } else if (isSolved) {
        statusStr = "Правильно!";
        statusColor = sf::Color::Green;
    } else if (currentMoveIndex > 0) {
        statusStr = "Ход " + std::to_string(currentMoveIndex) + "/" + std::to_string(solutionMoves.size());
        statusColor = sf::Color(200, 200, 200);
    } else {
        statusStr = "Начните решать задачу";
        statusColor = sf::Color(200, 200, 200);
    }
    
    sf::Text status(font);
    status.setString(sf::String::fromUtf8(statusStr.begin(), statusStr.end()));
    status.setCharacterSize(18);
    status.setFillColor(statusColor);
    status.setStyle(sf::Text::Bold);
    status.setPosition({panelX + 20, (float)yPos});
    window.draw(status);
    yPos += 45;
    
    if (messageClock.getElapsedTime().asSeconds() < 3.0f) {
        sf::Text msgText(font);
        msgText.setString(sf::String::fromUtf8(message.begin(), message.end()));
        msgText.setCharacterSize(18);
        msgText.setFillColor(sf::Color(255, 200, 100));
        msgText.setPosition({panelX + 20, (float)yPos});
        window.draw(msgText);
    }
}

void TaskBoard::drawButtons() {
    // Кнопка Назад
    if (isBackHovered) {
        backButton.setFillColor(sf::Color(180, 100, 40));
    } else {
        backButton.setFillColor(sf::Color(139, 69, 19));
    }
    window.draw(backButton);
    window.draw(*backText);
    
    // Кнопка Предыдущая
    if (isPrevHovered) {
        prevButton.setFillColor(sf::Color(120, 60, 30));
    } else {
        prevButton.setFillColor(sf::Color(80, 40, 20));
    }
    window.draw(prevButton);
    window.draw(*prevText);
    
    // Кнопка Следующая
    if (isNextHovered) {
        nextButton.setFillColor(sf::Color(120, 60, 30));
    } else {
        nextButton.setFillColor(sf::Color(80, 40, 20));
    }
    window.draw(nextButton);
    window.draw(*nextText);
    
    // Кнопка Сброс
    if (isResetHovered) {
        resetButton.setFillColor(sf::Color(180, 50, 30));
    } else {
        resetButton.setFillColor(sf::Color(139, 30, 20));
    }
    window.draw(resetButton);
    window.draw(*resetText);
}

void TaskBoard::handleButtonClick(int x, int y) {
    if (backButton.getGlobalBounds().contains({(float)x, (float)y})) {
        window.close();
        return;
    }
    
    if (prevButton.getGlobalBounds().contains({(float)x, (float)y})) {
        if (currentTaskIndex > 0) {
            currentTaskIndex--;
            loadCurrentTask();
            resetTask();
        }
        return;
    }
    
    if (nextButton.getGlobalBounds().contains({(float)x, (float)y})) {
        if (currentTaskIndex < taskManager.getTaskCount() - 1) {
            currentTaskIndex++;
            loadCurrentTask();
            resetTask();
        }
        return;
    }
    
    if (resetButton.getGlobalBounds().contains({(float)x, (float)y})) {
        resetTask();
        return;
    }
}

void TaskBoard::handleClick(int x, int y) {
    float offsetX = 30;
    float offsetY = 70;
    
    if (x < offsetX || x > offsetX + BOARD_SIZE || y < offsetY || y > offsetY + BOARD_SIZE) return;
    
    int col = (x - offsetX) / CELL_SIZE;
    int row = (y - offsetY) / CELL_SIZE;
    
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) return;
    
    if (isTaskCompleted) return;
    
    int boardRow, boardCol;
    if (isBoardFlipped) {
        boardRow = SIZE - 1 - row;
        boardCol = SIZE - 1 - col;
    } else {
        boardRow = row;
        boardCol = col;
    }
    
    Figure fig = board.getFigure(boardRow, boardCol);
    
    if (!hasSelected) {
        if (!fig.isEmpty() && fig.color == board.getCurrentPlayer()) {
            selectedRow = boardRow;
            selectedCol = boardCol;
            hasSelected = true;
        }
    } else {
        std::string move = std::string(1, 'a' + selectedCol) + std::to_string(8 - selectedRow) +
                          std::string(1, 'a' + boardCol) + std::to_string(8 - boardRow);
        
        if (board.makeMove(selectedRow, selectedCol, boardRow, boardCol)) {
            hasSelected = false;
            checkSolution(move);
        } else {
            if (!fig.isEmpty() && fig.color == board.getCurrentPlayer()) {
                selectedRow = boardRow;
                selectedCol = boardCol;
            } else {
                hasSelected = false;
            }
        }
    }
}

void TaskBoard::checkSolution(const std::string& move) {
    if (currentMoveIndex >= (int)solutionMoves.size()) {
        showMessage("Лишний ход!");
        resetTask();
        return;
    }
    
    std::string expectedMove = solutionMoves[currentMoveIndex];
    
    if (move == expectedMove) {
        currentMoveIndex++;
        showMessage("Правильно!");
        
        if (currentMoveIndex >= (int)solutionMoves.size()) {
            isSolved = true;
            isTaskCompleted = true;
            showMessage("ЗАДАЧА РЕШЕНА!");
        }
    } else {
        showMessage("Неправильно! Попробуйте снова");
        resetTask();
    }
}

void TaskBoard::showMessage(const std::string& msg) {
    message = msg;
    messageClock.restart();
}