#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QMediaPlayer>
#include <QSoundEffect>
#include <QAudioOutput>
#include <QFile>
#include <QUrl>
#include <QGraphicsView>

enum class MusicState
{
    NONE,
    MENU,
    GAMEPLAY
};

class AudioManager
{
public:
    AudioManager();
    ~AudioManager();

    static AudioManager& getInstance();

    bool init(QGraphicsView* parent);
    bool loadAudio();

    void playMenuMusic();
    void playGameplayMusic();
    void stopMusic();
    void updateMusic(MusicState newState);

    void playGameOverSFX();
    void playCountdownSFX();
    void playJumpSFX();
    void playLoseLifeSFX();
    void playDeathSFX();


    void playButtonBackSFX();
    void playButtonSelectSFX();
    void clean();

private:
    QMediaPlayer* musicPlayer = nullptr;
    QAudioOutput* audioOutput = nullptr;


    QSoundEffect gameOverSFX;
    QSoundEffect countdownSFX;
    QSoundEffect jumpSFX;
    QSoundEffect loseLifeSFX;
    QSoundEffect deathSFX;

    QSoundEffect buttonBackSFX;
    QSoundEffect buttonSelectSFX;

    MusicState currentMusicState;
};

#endif