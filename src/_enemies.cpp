#include "_enemies.h"

_enemies::_enemies()
{
    //ctor
    pos.x = 0;
    pos.y = -0.4;
    pos.z = -7;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;

    scale.x = 0.5;
    scale.y = 0.5;
    scale.z = 1;

    actionTrigger = 2;
    vel = 30;
    t = 0.1;
}

_enemies::~_enemies()
{
    //dtor
}
void _enemies::enmsInit(int x, int y, char *fileName)
{
    initQuad(fileName);
    xFrames = x;
    yFrames = y;

    xMin = 0;
    xMax = 1.0/(float)xFrames;
    yMax = 1.0/(float)yFrames;
    yMin = 0;
}

void _enemies::enmsActions(float deltaT)
{
    timer += deltaT;
    switch(actionTrigger){
        case STAND:
            xMin = 0;
            xMax = 1.0/(float)xFrames;
            yMax = 1.0/(float)yFrames;
            yMin = yMax-(1.0/(float)yFrames);
            break;
        case RIGHTWALK:
            if(timer>0.08){

                xMax<xMin?(xMin = 0, xMax = 1.0/(float)xFrames):NULL;

                xMin += 1.0/(float)xFrames;
                xMax += 1.0/(float)xFrames;


                pos.x <= 4.5 ? pos.x+=2*deltaT+0.2:actionTrigger=LEFTWALK;
                pos.y = -1.4;

                timer = 0;
            }
            break;
        case LEFTWALK:
            if(timer>0.08){

                xMax>xMin?(xMax = 0, xMin = 1.0/(float)xFrames):NULL;

                xMin += 1.0/(float)xFrames;
                xMax += 1.0/(float)xFrames;

                pos.x >= -4.5 ? pos.x-=2*deltaT+0.2:actionTrigger=RIGHTWALK;
                pos.y = -1.4;

                timer = 0;
            }
            break;
        case ROLL_LEFT:
            if(timer>0.08){


                timer = 0;
            }
            break;
        case ROLL_RIGHT:
            if(timer>0.08){
                theta = 30*PI/180.0;

                rot.z += (float)rand()/(float)(RAND_MAX)*100;

                // x = v*t*cos
                // y = v*t*sin - (1/2)gravity*t^2
                pos.x += vel*t*cos(theta)/1500.0;
                pos.y += (vel*t*sin(theta)-0.5*GRAVITY*t*t)/300.0;

                pos.y > -1.5 ? t+=0.3 : (t=0.1, pos.y = -0.75);
                pos.x > 4.5 ? (actionTrigger = LEFTWALK, rot.z = 0) : NULL;

                timer = 0;
            }
            break;
        default: break;
    }
}

void _enemies::placeEnms(vec3 Epos, float deltaT)
{
    pos.x = Epos.x;
    pos.y = Epos.y;
    pos.z = Epos.z;
}

void _enemies::drawEnms()
{
    updateQuad();
    drawQuad();
}
