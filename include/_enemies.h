#ifndef _ENEMIES_H
#define _ENEMIES_H

#include<_common.h>
#include<_quad.h>
#include<_shader.h>

class _enemies;

class _enemyNavigation
{
    public:
        virtual ~_enemyNavigation() {}
        virtual bool isEnemyPositionWalkable(const _enemies*, vec3) const = 0;
        virtual bool doesRectHitPlayer(rect2D) const = 0;
        virtual bool tryDamagePlayer(rect2D) const = 0;
        virtual bool doesRectHitWall(rect2D) const = 0;
};

class _enemies:public _quad
{
    public:
        _enemies();
        virtual ~_enemies();


        float timer = 0;
        float animationTimer;
        float animationInterval;
        float moveInterval;
        float moveDuration;
        float moveProgress;
        float activeMoveDuration;
        float frameSampleScale;
        float tileStepDistance;
        float collisionWidthScale;
        float collisionHeightScale;
        float collisionOffsetY;
        float damageFlashTimer;
        float damageFlashDuration;
        float knockbackDuration;
        int currentFrame;
        int maxHealth;
        int health;
        int lastHitAttackId;
        int xFrames, yFrames;
        int actionTrigger;
        enum{STAND, LEFTWALK, RIGHTWALK, RUN, JUMP, ATTACK, ROLL_LEFT, ROLL_RIGHT, WALKUP, WALKDOWN};
        enum FacingDirection{FACE_DOWN, FACE_LEFT, FACE_RIGHT, FACE_UP};

        FacingDirection facingDirection;
        bool isMoving;
        bool isKnockbackActive;
        bool startedMoveThisFrame;
        vec3 moveStartPos;
        vec3 moveTargetPos;

        void enmsInit(int, int, char*);
        virtual void enmsActions(float, const vec3* = NULL, const _enemyNavigation* = NULL);
        void placeEnms(vec3, float);
        virtual void drawEnms();
        void setSpriteRegionPixels(int, int, int, int, int, int);
        rect2D collisionBoundsAt(vec3) const;
        rect2D collisionBounds() const;
        void cancelMove();
        void haltMove();
        void takeDamage(int, int, FacingDirection);
        bool canBeHitByAttack(int) const;

        bool isEnmsLive = true;

        float vel =0;
        float t = 0;
        float theta = 0;



    protected:
        void drawSelfWithPalette(_shader*, vec3, vec3);
        void setIdleFrame();
        float facingAngleDegrees() const;
        vec2 facingVector() const;

    private:
        _shader* hitShader;
        bool hitShaderInitialized;
        float regionXMin;
        float regionXMax;
        float regionYMin;
        float regionYMax;

        void beginKnockback(FacingDirection);
        void beginHitFlash();
        void ensureHitShader();
        bool isUsingHitShader() const;
        vec3 visualOffset() const;
        void setAnimationFrame(int, int);
        void beginMove(FacingDirection);
        int directionColumn() const;
        FacingDirection randomDirection() const;
        virtual bool chooseNextDirection(FacingDirection&, const vec3*, const _enemyNavigation*) const;
};

#endif // _ENEMIES_H
