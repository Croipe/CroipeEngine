#ifndef AUDIO_LAYER
#define AUDIO_LAYER
/* A Primitive Audio Layer With Functions To Play Sound. It Is Not Able To Play Two Sounds At Once */
#include <Windows.h>
#include <string>

namespace Croipe::AudioLayer {
    void PlaySoundFile(const std::string filename) {
        PlaySoundA(filename.c_str(), NULL, SND_FILENAME | SND_ASYNC);
    }

    void PlaySoundLoop(const std::string filename) {
        PlaySoundA(filename.c_str(), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC); 
    }
}


#endif