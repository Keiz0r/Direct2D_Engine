#include "Sound.h"

void Sound::openMP3() {
    mciSendString(GAMESOUND(testmusic.mp3), NULL, 0, NULL);
}

void Sound::playOnRepeatMP3() {
    mciSendString(L"play mp3 repeat", NULL, 0, NULL);
}

void Sound::closeMP3() {
    mciSendString(L"close mp3", NULL, 0, NULL);
}
