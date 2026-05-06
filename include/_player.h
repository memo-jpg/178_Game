#ifndef _PLAYER_H
#define _PLAYER_H

#include<_common.h>
#include<_quad.h>

class _player:public _quad
{
    public:
        _player();
        virtual ~_player();

        float timer;
        int xFrames, yFrames;
        int actionTrigger;
        enum{STAND, LEFTWALK, RIGHTWALK, ATTACK, WALKUP, WALKDOWN, HIT, BLOCK};

        void plyInit(int, int, char*);
        void playerActions(float);

    protected:

    private:
};

#endif // _PLAYER_H
