#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager()
    : musicPlayer(nullptr),
      audioOutput(nullptr),
      currentMusicState(MusicState::NONE)
{
}

AudioManager::~AudioManager()
{
    if (musicPlayer != nullptr) {
        delete musicPlayer;
        musicPlayer = nullptr;
    }
    if (audioOutput != nullptr) {
        delete audioOutput;
        audioOutput = nullptr;
    }
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::init(QGraphicsView* parent)
{
    audioOutput = new QAudioOutput(parent);
    musicPlayer = new QMediaPlayer(parent);

    musicPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5f);

    return loadAudio();
}

bool AudioManager::loadAudio()
{
    if (!QFile::exists(":/sounds/sfx/btnback.wav")) return false;
    if (!QFile::exists(":/sounds/sfx/btnselect.wav")) return false;
    if (!QFile::exists(":/sounds/sfx/countdown_321go.wav")) return false;
    if (!QFile::exists(":/sounds/sfx/game_over.wav")) return false;
    if (!QFile::exists(":/sounds/sfx/jump.wav")) return false;
    if (!QFile::exists(":/sounds/sfx/lose_life.wav")) return false;

    gameOverSFX.setSource(QUrl("qrc:/sounds/sfx/game_over.wav"));
    countdownSFX.setSource(QUrl("qrc:/sounds/sfx/countdown_321go.wav"));
    jumpSFX.setSource(QUrl("qrc:/sounds/sfx/jump.wav"));
    loseLifeSFX.setSource(QUrl("qrc:/sounds/sfx/lose_life.wav"));

    deathSFX.setSource(QUrl("qrc:/sounds/sfx/death.wav"));
    buttonBackSFX.setSource(QUrl("qrc:/sounds/sfx/btnback.wav"));
    buttonSelectSFX.setSource(QUrl("qrc:/sounds/sfx/btnselect.wav"));

    gameOverSFX.setVolume(0.5f);
    countdownSFX.setVolume(0.5f);
    jumpSFX.setVolume(0.5f);
    loseLifeSFX.setVolume(0.5f);
    deathSFX.setVolume(0.5f);
    buttonBackSFX.setVolume(0.5f);
    buttonSelectSFX.setVolume(0.5f);
    return true;
}

void AudioManager::playMenuMusic()
{
    musicPlayer->setSource(QUrl::fromLocalFile("resources/sounds/music/ECOLE_Jeux.wav"));
    musicPlayer->setLoops(QMediaPlayer::Infinite);
    musicPlayer->play();
    
}

void AudioManager::playGameplayMusic()
{
    musicPlayer->setSource(QUrl::fromLocalFile("resources/sounds/music/InRunBeat.wav"));
    musicPlayer->setLoops(QMediaPlayer::Infinite);
    musicPlayer->play();
}

void AudioManager::stopMusic()
{
    musicPlayer->stop();
}

void AudioManager::updateMusic(MusicState newState)
{
    if (newState == currentMusicState)
    {
        return;
    }

    currentMusicState = newState;

    if (newState == MusicState::MENU)
    {
        playMenuMusic();
    }
    else if (newState == MusicState::GAMEPLAY)
    {
        playGameplayMusic();
    }
    else
    {
        stopMusic();
    }
}

void AudioManager::playGameOverSFX()
{
    gameOverSFX.play();
}

void AudioManager::playCountdownSFX()
{
    countdownSFX.play();
}

void AudioManager::playJumpSFX()
{
    jumpSFX.play();
}

void AudioManager::playLoseLifeSFX()
{
    loseLifeSFX.play();
}

void AudioManager::playDeathSFX()
{
    deathSFX.play();
}

void AudioManager::playButtonBackSFX()
{
    buttonBackSFX.play();
}

void AudioManager::playButtonSelectSFX()
{
    buttonSelectSFX.play();
}

void AudioManager::clean() {
    if (musicPlayer != nullptr) {
        delete musicPlayer;
        musicPlayer = nullptr;
    }
    if (audioOutput != nullptr) {
        delete audioOutput;
        audioOutput = nullptr;
    }
}

void AudioManager::setVolume(float volume) {
    audioOutput->setVolume(volume);

    gameOverSFX.setVolume(volume);
    countdownSFX.setVolume(volume);
    jumpSFX.setVolume(volume);
    loseLifeSFX.setVolume(volume);
    deathSFX.setVolume(volume);
    buttonBackSFX.setVolume(volume);
    buttonSelectSFX.setVolume(volume);
}

int AudioManager::getVolume() {
    return audioOutput->volume() * 100;
}
