#ifndef _ENEMIES_H
#define _ENEMIES_H

#include<_common.h>
#include<_quad.h>

class _enemies:public _quad
{
    public:
        _enemies();
        virtual ~_enemies();


        float timer = 0;
        int xFrames, yFrames;
        int actionTrigger;
        enum{STAND, LEFTWALK, RIGHTWALK, RUN, JUMP, ATTACK, ROLL_LEFT, ROLL_RIGHT};

        void enmsInit(int, int, char*);
        void enmsActions(float);
        void placeEnms(vec3, float);
        void drawEnms();

        bool isEnmsLive = true;

        float vel =0;
        float t = 0;
        float theta = 0;



    protected:

    private:
};

#endif // _ENEMIES_H
