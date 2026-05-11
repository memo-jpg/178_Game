#ifndef _BOSS3_H
#define _BOSS3_H

#include<_boss.h>
#include<_bullets.h>

class _boss3: public _boss
{
    public:
        _boss3();
        virtual ~_boss3();

        void initBoss3();
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

#endif // _BOSS3_H
