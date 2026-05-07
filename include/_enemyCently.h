#ifndef _ENEMYCENTLY_H
#define _ENEMYCENTLY_H

#include<_enemies.h>
#include<_bullets.h>

class _enemyCently: public _enemies
{
    public:
        _enemyCently();
        virtual ~_enemyCently();

        void initCently();
        void enmsActions(float, const vec3* = NULL, const _enemyNavigation* = NULL);
        void drawEnms();

    private:
        _bullets projectile;
        _shader* chargeShader;
        bool chargeShaderInitialized;
        bool isChargingAttack;
        bool isProjectileAttackActive;
        float chargeTimer;
        float chargeDuration;
        float attackCooldownTimer;
        float attackCooldownDuration;
        float projectileSpeedTilesPerSecond;
        float detectionLengthTiles;
        float detectionWidthScale;

        void ensureChargeShader();
        rect2D attackTriggerBounds() const;
        void startChargeAttack();
        void finishAttackCycle();
        void fireProjectile();
};

#endif // _ENEMYCENTLY_H
