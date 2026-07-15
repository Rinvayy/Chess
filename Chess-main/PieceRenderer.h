#ifndef PIECE_RENDERER_H
#define PIECE_RENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "figures.h"

class PieceRenderer {
private:
    sf::Texture textures[12];
    sf::Sprite* sprites[12];
    bool loaded = false;
    
public:
    PieceRenderer();
    ~PieceRenderer();
    
    bool loadTextures();
    void drawPiece(sf::RenderWindow& window, const Figure& fig, float x, float y, float cellSize);
    bool isLoaded() const { return loaded; }
};

#endif