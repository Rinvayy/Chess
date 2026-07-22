#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() 
    : soundLoaded(false), musicPlaying(false) {
    clickSound = nullptr;
    moveSound = nullptr;
    captureSound = nullptr;
    gameoverSound = nullptr;
    backgroundMusic = nullptr;
    
    loadSounds();
}

SoundManager::~SoundManager() {
    delete clickSound;
    delete moveSound;
    delete captureSound;
    delete gameoverSound;
    delete backgroundMusic;
}

void SoundManager::loadSounds() {
    bool allLoaded = true;
    //загрузка звуковых файлов
    if (!clickBuffer.loadFromFile("sounds/click.wav")) {
        std::cout << "Warning: sounds/click.wav not found\n";
        allLoaded = false;
    }
    
    if (!moveBuffer.loadFromFile("sounds/move.wav")) {
        std::cout << "Warning: sounds/move.wav not found\n";
        allLoaded = false;
    }
    
    if (!captureBuffer.loadFromFile("sounds/capture.wav")) {
        std::cout << "Warning: sounds/capture.wav not found\n";
        allLoaded = false;
    }
    
    if (!gameoverBuffer.loadFromFile("sounds/gameover.wav")) {
        std::cout << "Warning: sounds/gameover.wav not found\n";
        allLoaded = false;
    }
    
    if (!backgroundBuffer.loadFromFile("sounds/background.wav")) {
        std::cout << "Warning: sounds/background.wav not found\n";
        allLoaded = false;
    }
    
    if (allLoaded) {
        clickSound = new sf::Sound(clickBuffer);
        clickSound->setVolume(30);
        
        moveSound = new sf::Sound(moveBuffer);
        moveSound->setVolume(30);
        
        captureSound = new sf::Sound(captureBuffer);
        captureSound->setVolume(40);
        
        gameoverSound = new sf::Sound(gameoverBuffer);
        gameoverSound->setVolume(60);
        
        backgroundMusic = new sf::Sound(backgroundBuffer);
        backgroundMusic->setVolume(15);
        
        soundLoaded = true;
    }
}

void SoundManager::playClick() {
    if (soundLoaded && clickSound) {
        clickSound->play();
    }
}

void SoundManager::playMove(bool isCapture) {
    if (soundLoaded) {
        if (isCapture && captureSound) {
            captureSound->play();
        } else if (moveSound) {
            moveSound->play();
        }
    }
}

void SoundManager::playGameOver() {
    if (soundLoaded && gameoverSound) {
        gameoverSound->play();
    }
}

void SoundManager::startBackgroundMusic() {
    if (soundLoaded && backgroundMusic) {
        backgroundMusic->play();
        musicPlaying = true;
    }
}

void SoundManager::stopBackgroundMusic() {
    if (soundLoaded && backgroundMusic) {
        backgroundMusic->stop();
        musicPlaying = false;
    }
}

void SoundManager::toggleMusic() {
    if (musicPlaying) {
        stopBackgroundMusic();
        std::cout << "Music: OFF\n";
    } else {
        startBackgroundMusic();
        std::cout << "Music: ON\n";
    }
}