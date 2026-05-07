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
        float attackTimer;
        float attackDuration;
        float collisionWidthScale;
        float collisionHeightScale;
        float collisionOffsetY;
        int xFrames, yFrames;
        int currentFrame;
        int attackId;
        int actionTrigger;
        enum{STAND, LEFTWALK, RIGHTWALK, RUN, JUMP, ATTACK, WALKUP, WALKDOWN};
        enum FacingDirection{FACE_DOWN, FACE_LEFT, FACE_RIGHT, FACE_UP};

        FacingDirection facingDirection;
        bool isAttackActive;

        void plyInit(int, int, char*);
        void playerActions(float);
        void drawPlayer();
        void startAttack();
        void finishAttack();
        bool canMove() const;
        float getAttackProgress() const;
        float getFacingAngleDegrees() const;
        int currentAttackId() const;
        rect2D collisionBoundsAt(vec3 position) const;
        rect2D collisionBounds() const;
        quad2D attackQuad() const;
        rect2D attackBounds() const;

    protected:

    private:
        vec3 visualOffset() const;
        void setIdleFrame();
        void setAnimationFrame(int rowIndex, int frameIndex);
};

#endif // _PLAYER_H
