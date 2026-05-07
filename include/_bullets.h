#ifndef _BULLETS_H
#define _BULLETS_H

#include<_common.h>
#include<_quad.h>
#include<_shader.h>

class _bullets:public _quad
{
    public:
        _bullets();
        virtual ~_bullets();

        bool isLive = false;    //check the bullet active or not
        int actionTrigger = 0;
        float timer;
        float t = 0; // fraction of distance in parametric equation

        enum {IDLE, ACTIVE, HIT};

        void initBlt(int, int, char*);
        void update(vec3);      // update bullet movements
        void shoot(vec3, vec3, float); // source and destination
        void bulletActions();   // Address the bullets actions
        void drawBlt();         // render bullets
        void launch(vec3, vec3, float, float);
        void updateTravel(float);
        void deactivate();
        rect2D collisionBounds() const;
        bool isActive() const;

        float xFrames = 1;
        float yFrames = 1;
        float speed;
        float flashTimer;
        float collisionScale;
        bool flashEnabled;

        vec3 dest;              // to keep track of first destination of the bullet
        vec3 direction;

    protected:

    private:
        _shader* flashShader;
        bool flashShaderInitialized;

        void ensureFlashShader();
};

#endif // _BULLETS_H
