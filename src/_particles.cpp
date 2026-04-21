#include "_particles.h"

_particles::_particles()
{
    //ctor
}

_particles::~_particles()
{
    //dtor
}
void _particles::drawParticles()
{
    for(int i=0; i<numDrops; i++){
        if(drops[i].live){
            glPushMatrix();
            drawQuads(drops[i].pos.x, drops[i].pos.y, drops[i].pos.z);
            glPopMatrix();
        }
    }
}

void _particles::lifeTime(float deltaT)
{
    timer += deltaT;

    if(timer > 0.06){
        for(int i=0; i<numDrops; i++){
            if(drops[i].live){
                drops[i].pos.x += drops[i].vel.x;
                drops[i].pos.y += drops[i].vel.y;

                drops[i].alpha -= 3;
                drops[i].t += deltaT;

                if(drops[i].alpha < 0){
                    drops[i].live = false;
                    drops[i].t = 0;
                }
                //drops[i].alpha<0 ? (drops[i].live = false, drops[i].t = 0) : NULL;
            }
        }

        timer = 0;
    }
}

void _particles::generateParticles(float mx, float my, float mz)
{
    newDrops = rand()%30 + 1;

    //(newDrops + numDrops) >= MAX_DROPS ? (newDrops = MAX_DROPS - newDrops) : NULL;
    if (newDrops + numDrops > MAX_DROPS){
        newDrops = MAX_DROPS - newDrops;
    }
    // to make sure number of particles are within range

    for(int i=numDrops; i<(numDrops+newDrops); i++){
        drops[i].live = true;
        drops[i].radius = 1.0;
        drops[i].pos.x = mx;
        drops[i].pos.y = my;
        drops[i].pos.z = mz;

        drops[i].expandRadius = 0.05;
        drops[i].theta = rand()%360;

        drops[i].vel.x = sin(drops[i].theta)*drops[i].expandRadius;
        drops[i].vel.y = cos(drops[i].theta)*drops[i].expandRadius;
        drops[i].vel.z = cos(drops[i].theta)*drops[i].expandRadius;

        drops[i].alpha = rand()%50;
        drops[i].t = 0;
    }

    numDrops += newDrops;
    numDrops >= MAX_DROPS ? numDrops = 0 : NULL;    // reset total number of drops
}

void _particles::drawQuads(float x, float y, float z)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    pos.x = x;
    pos.y = y;
    pos.z = z;

    scale.x = 0.05;
    scale.y = 0.05;
    scale.z = 1.0;

    drawQuad();

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}

void _particles::initParticles(char* fileName)
{
    initQuad(fileName);
}
