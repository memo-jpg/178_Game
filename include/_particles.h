#ifndef _PARTICLES_H
#define _PARTICLES_H

#include<_common.h>
#include<_quad.h>

class _particles:public _quad
{
    public:
        _particles();
        virtual ~_particles();

        int numDrops = 0;
        int newDrops = 0;
        float timer = 0;

        typedef struct{
            bool live;          // life time
            vec3 pos;           // position
            vec3 dir;           // direction of the move
            vec3 color;         // to setup colors
            vec3 vel;           // velocity
            float alpha;        // alpha value
            float t = 0;        // for parametric equations

            float mass;         // if you work against gravity
            float radius;       // circular expansion
            float expandRadius;
            float theta;        // angle
        } particle;

        particle drops[MAX_DROPS];

        void drawParticles();   // to render
        void lifeTime(float);   // pass the timer
        void generateParticles(float, float, float);
        void drawQuads(float, float, float);
        void initParticles(char *);

    protected:

    private:
};

#endif // _PARTICLES_H
