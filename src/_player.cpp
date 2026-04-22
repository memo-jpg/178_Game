#include "_player.h"

_player::_player()
{
    //ctor
    //xMin = 0;
    //xMax = 1.0/9.0;
    //yMax = 1.0/4.0;
    //yMin = 0;

    pos.y = -0.8;

    timer = 0;
}

_player::~_player()
{
    //dtor
}

void _player::plyInit(int x, int y, char *fileName)
{
    initQuad(fileName);
    xFrames = x;
    yFrames = y;

    xMin = 0;
    xMax = 1.0/(float)xFrames;
    //xMax = 1.0/9.0;
    yMax = 1.0/(float)yFrames;
    yMin = 0;

}

void _player::playerActions(float deltaT)
{
    timer += deltaT;
    switch(actionTrigger){
        case STAND:
            xMin = 0;
            xMax = 1.0/(float)xFrames;
            yMax = 1.0/(float)yFrames;
            yMin = yMax-(1.0/(float)yFrames);
            break;
        case LEFTWALK:
            if(timer>0.08){
                xMin += 1.0/(float)xFrames;
                xMax += 1.0/(float)xFrames;
                yMax = 2.0/(float)yFrames;
                yMin = 1.0/(float)yFrames;

                pos.x -= 2*deltaT + 0.1;

                timer = 0;
            }
            break;
        case RIGHTWALK:
            if(timer>0.08){
                xMin += 1.0/(float)xFrames;
                xMax += 1.0/(float)xFrames;
                yMax = 3.0/(float)yFrames;
                yMin = 2.0/(float)yFrames;

                pos.x += 2*deltaT + 0.1;

                timer = 0;
            }
            break;
        case WALKUP:
            if(timer>0.08){
                xMin += 1.0/(float)xFrames;
                xMax += 1.0/(float)xFrames;
                yMax = 4.0/(float)yFrames;
                yMin = 3.0/(float)yFrames;

                pos.y += 2*deltaT + 0.1;

                timer = 0;
            }
            break;
        case WALKDOWN:
            if(timer>0.08){
                xMin += 1.0/(float)xFrames;
                xMax += 1.0/(float)xFrames;
                yMax = 1.0/(float)yFrames;
                yMin = 0.0/(float)yFrames;

                pos.y -= 2*deltaT + 0.1;

                timer = 0;
            }
            break;
        default: break;

    }
}
