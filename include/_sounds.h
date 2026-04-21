#ifndef _SOUNDS_H
#define _SOUNDS_H

#include<_common.h>
#include<SNDS/irrKlang.h>

using namespace irrklang;

class _sounds
{
    public:
        _sounds();
        virtual ~_sounds();

        void playMusic(char*);
        void playSounds(char*);
        void pauseSound(char*);
        void initSound();

        ISoundEngine *sndEng = createIrrKlangDevice();

    protected:

    private:
};

#endif // _SOUNDS_H
