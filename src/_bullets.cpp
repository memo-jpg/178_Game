#include "_bullets.h"

_bullets::_bullets()
{
    //ctor
    pos.x = 0;
    pos.y = -0.4;
    pos.z = -7;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;

    scale.x = 0.2;
    scale.y = 0.2;
    scale.z = 1.0;
}

_bullets::~_bullets()
{
    //dtor
}
void _bullets::initBlt(int x, int y, char *fileName)
{
    initQuad(fileName);
    xFrames = x;
    yFrames = y;

    xMin = 0;
    xMax = 1.0/(float)xFrames;
    yMax = 1.0/(float)yFrames;
    yMin = 0;
}

void _bullets::update(vec3)
{
    // TO DO | depends on use of bullets
}

void _bullets::shoot(vec3 spos, vec3 dpos, float deltaT)
{
    if(actionTrigger == IDLE){
        dest = dpos;

        // angle between two points
        // arccos dot product of the points
        /*
        float X = spos.x * dest.x;
        float Y = spos.y * dest.y;
        float Z = spos.z * dest.z;
        float MS = sqrt(pow(spos.x, 2) + pow(spos.y, 2) + pow(spos.z, 2));  // magnitude of source value
        float MD = sqrt(pow(dest.x, 2) + pow(dest.y, 2) + pow(dest.z, 2));  // magnitude of destination

        rot.z = acos((X + Y + Z)/(MS*MD));      // X+Y+Z is dot product
        */

        float X = dest.x - spos.x;      // translate to the center
        float Y = dest.y - spos.y;      // translate to the center

        rot.z = (atan2(Y, X) * 180 / PI) + 140;

    }

    if(isLive){
        timer += deltaT;
        if(timer>0.06){
            pos.x = spos.x + t*(dest.x-spos.x);
            pos.y = spos.y + t*(dest.y-spos.y);
            pos.z = spos.z + t*(dest.z-spos.z);

            if (actionTrigger == ACTIVE){
                if(t > 1){
                    t = 0;
                    actionTrigger = IDLE;       // reset the bullet
                } else {
                    t += (deltaT + 0.1);       // let bullet move on the path
                }
            }

            timer = 0;
        }
    }
}

void _bullets::bulletActions()
{
    switch(actionTrigger){
        case IDLE:
            isLive = false; // if bullet is in the storage
            break;
        case ACTIVE:
            isLive = true;  // bullet is on the move
            break;
        case HIT:
            isLive = false; // when bullet hit the target or move out of screen
            break;
        default:
            break;
    }
}

void _bullets::drawBlt()
{
    if(isLive){
        updateQuad();
        drawQuad();
    }
}
