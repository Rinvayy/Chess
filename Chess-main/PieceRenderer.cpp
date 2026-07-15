#include "PieceRenderer.h"
#include <iostream>
#include <vector>

PieceRenderer::PieceRenderer() {
    loaded = false;
    for (int i = 0; i < 12; ++i) {
        sprites[i] = nullptr;  
    }
}

PieceRenderer::~PieceRenderer() {
    for (int i = 0; i < 12; ++i) {
        delete sprites[i];  
    }
}

bool PieceRenderer::loadTextures() {
    struct PieceInfo {
        FigureType type;
        Color color;
        std::string path;
    };
    
    std::vector<PieceInfo> pieces = {
        {FigureType::KING,   Color::WHITE, "assets/white_king.png"},
        {FigureType::QUEEN,  Color::WHITE, "assets/white_queen.png"},
        {FigureType::ROOK,   Color::WHITE, "assets/white_rook.png"},
        {FigureType::BISHOP, Color::WHITE, "assets/white_bishop.png"},
        {FigureType::KNIGHT, Color::WHITE, "assets/white_knight.png"},
        {FigureType::PAWN,   Color::WHITE, "assets/white_pawn.png"},
        {FigureType::KING,   Color::BLACK, "assets/black_king.png"},
        {FigureType::QUEEN,  Color::BLACK, "assets/black_queen.png"},
        {FigureType::ROOK,   Color::BLACK, "assets/black_rook.png"},
        {FigureType::BISHOP, Color::BLACK, "assets/black_bishop.png"},
        {FigureType::KNIGHT, Color::BLACK, "assets/black_knight.png"},
        {FigureType::PAWN,   Color::BLACK, "assets/black_pawn.png"}
    };
    
    bool allLoaded = true;
    for (size_t i = 0; i < pieces.size(); ++i) {
        if (!textures[i].loadFromFile(pieces[i].path)) {
            std::cout << "Ошибка: не удалось загрузить " << pieces[i].path << "\n";
            allLoaded = false;
        }
        //создаем спрайт для каждой текстуры
        sprites[i] = new sf::Sprite(textures[i]);
    }
    
    loaded = allLoaded;
    return loaded;
}

void PieceRenderer::drawPiece(sf::RenderWindow& window, const Figure& fig, float x, float y, float cellSize) {
    if (!loaded || fig.isEmpty()) return;
    
    int index = -1;
    if (fig.color == Color::WHITE) {
        switch (fig.type) {
            case FigureType::KING:   index = 0; break;
            case FigureType::QUEEN:  index = 1; break;
            case FigureType::ROOK:   index = 2; break;
            case FigureType::BISHOP: index = 3; break;
            case FigureType::KNIGHT: index = 4; break;
            case FigureType::PAWN:   index = 5; break;
            default: return;
        }
    } else {
        switch (fig.type) {
            case FigureType::KING:   index = 6; break;
            case FigureType::QUEEN:  index = 7; break;
            case FigureType::ROOK:   index = 8; break;
            case FigureType::BISHOP: index = 9; break;
            case FigureType::KNIGHT: index = 10; break;
            case FigureType::PAWN:   index = 11; break;
            default: return;
        }
    }
    
    if (sprites[index] == nullptr) return;
    
    sf::Vector2u texSize = textures[index].getSize();
    float scaleX = cellSize / texSize.x;
    float scaleY = cellSize / texSize.y;
    
    sprites[index]->setScale({scaleX, scaleY});
    sprites[index]->setPosition({x, y});
    
    window.draw(*sprites[index]);
}