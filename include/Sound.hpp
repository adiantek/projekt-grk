#pragma once

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alext.h>
#include <stdbool.h>

class Sound {
   private:
    bool loaded = false;

   public:
    Sound(char ***argv, int *argc);
    ~Sound();
    void setPosition(float x, float y, float z);
    void setOrientation(float *orientation);
    ALuint loadSound(const char *name, ALenum format, ALsizei freq);
    void start();
    void openChest(float x, float y, float z);
    void closeChest(float x, float y, float z);
};

extern Sound *sound;