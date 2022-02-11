#include <Logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ResourceLoader.hpp>
#include <Sound.hpp>

int InitAL(char ***argv, int *argc) {
    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    /* Open and initialize a device */
    device = NULL;
    if (argc && argv && *argc > 1 && strcmp((*argv)[0], "-device") == 0) {
        device = alcOpenDevice((*argv)[1]);
        if (!device)
            LOGE("Failed to open \"%s\", trying default", (*argv)[1]);
        (*argv) += 2;
        (*argc) -= 2;
    }
    if (!device)
        device = alcOpenDevice(NULL);
    if (!device) {
        LOGE("Could not open a device!");
        return 1;
    }

    ctx = alcCreateContext(device, NULL);
    if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE) {
        if (ctx != NULL)
            alcDestroyContext(ctx);
        alcCloseDevice(device);
        LOGE("Could not set a context!");
        return 1;
    }

    name = NULL;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    if (!name || alcGetError(device) != AL_NO_ERROR)
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    LOGI("Opened \"%s\"", name);

    return 0;
}

static void printDeviceList(const char *list) {
    if (!list || *list == '\0')
        LOGE("    !!! none !!!");
    else
        do {
            LOGI("    %s", list);
            list += strlen(list) + 1;
        } while (*list != '\0');
}

Sound *sound = 0;

Sound::Sound(char ***argv, int *argc) {
    sound = this;
    LOGI("Available playback devices:");
    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") != AL_FALSE)
        printDeviceList(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));
    else
        printDeviceList(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

    if (InitAL(argv, argc) != 0) {
        LOGE("Failed!");
        this->loaded = false;
        return;
    }
    this->loaded = true;
}

Sound::~Sound() {
}

ALuint Sound::loadSound(const char *name, ALenum format, ALsizei freq) {
    size_t size;
    char *data = ResourceLoader::readFile(name, &size);
    if (!data) {
        return 0;
    }
    ALuint buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data, (ALsizei)size, freq);
    ALenum err = alGetError();
    if (err != AL_NO_ERROR) {
        LOGE("OpenAL Error: %s", alGetString(err));
        if (buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        free(data);
        return 0;
    }
    ALuint source = 0;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)buffer);
    free(data);
    return source;
}

void Sound::setPosition(float x, float y, float z) {
    if (!this->loaded) {
        return;
    }
    alListener3f(AL_POSITION, x, y, z);
}

void Sound::setOrientation(float *orientation) {
    if (!this->loaded) {
        return;
    }
    alListenerfv(AL_ORIENTATION, orientation);
}

void Sound::start() {
    if (!this->loaded) {
        return;
    }
    alSourcei(resourceLoaderExternal->sound_bg, AL_LOOPING, AL_TRUE);
    alSourcef(resourceLoaderExternal->sound_bg, AL_GAIN, 0.1f); // volume
    alSourcei(resourceLoaderExternal->sound_bg, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(resourceLoaderExternal->sound_bg, AL_POSITION, 0, 0, 0);
    alSourcePlay(resourceLoaderExternal->sound_bg);
}

void Sound::openChest(float x, float y, float z) {
    if (!this->loaded) {
        return;
    }
    alSourcei(resourceLoaderExternal->sound_chestopen, AL_SOURCE_RELATIVE, AL_FALSE);
    alSource3f(resourceLoaderExternal->sound_chestopen, AL_POSITION, x, y, z);
    alSourcePlay(resourceLoaderExternal->sound_chestopen);
}

void Sound::closeChest(float x, float y, float z) {
    if (!this->loaded) {
        return;
    }
    alSourcei(resourceLoaderExternal->sound_chestclosed, AL_SOURCE_RELATIVE, AL_FALSE);
    alSource3f(resourceLoaderExternal->sound_chestclosed, AL_POSITION, x, y, z);
    alSourcePlay(resourceLoaderExternal->sound_chestclosed);
}

void Sound::pickCoin(float x, float y, float z) {
    if (!this->loaded) {
        return;
    }
    alSourcei(resourceLoaderExternal->sound_coin, AL_SOURCE_RELATIVE, AL_FALSE);
    alSource3f(resourceLoaderExternal->sound_coin, AL_POSITION, x, y, z);
    alSourcePlay(resourceLoaderExternal->sound_coin);
}
