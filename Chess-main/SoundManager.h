#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML/Audio.hpp>
#include <string>

class SoundManager {
private:
    sf::SoundBuffer clickBuffer;
    sf::SoundBuffer moveBuffer;
    sf::SoundBuffer captureBuffer;
    sf::SoundBuffer gameoverBuffer;
    sf::SoundBuffer backgroundBuffer;
    
    sf::Sound* clickSound;
    sf::Sound* moveSound;
    sf::Sound* captureSound;
    sf::Sound* gameoverSound;
    sf::Sound* backgroundMusic;
    
    bool soundLoaded;
    bool musicPlaying;
    
public:
    SoundManager();
    ~SoundManager();
    
    void loadSounds();
    void playClick();
    void playMove(bool isCapture);
    void playGameOver();
    void toggleMusic();
    void startBackgroundMusic();
    void stopBackgroundMusic();
    bool isMusicPlaying() const { return musicPlaying; }
    bool isSoundLoaded() const { return soundLoaded; }
};

#endif