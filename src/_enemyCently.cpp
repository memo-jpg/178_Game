#include "_enemyCently.h"

_enemyCently::_enemyCently()
{
    pos.x = 0.0f;
    pos.y = -0.85f;
    pos.z = -8.0f;

    scale.x = 0.245f;
    scale.y = 0.245f;
    scale.z = 1.0f;

    maxHealth = 2;
    health = 2;
    actionTrigger = STAND;
    moveInterval = 1.0f;
    moveDuration = 0.5f;
    chargeShader = new _shader();
    chargeShaderInitialized = false;
    isChargingAttack = false;
    isProjectileAttackActive = false;
    chargeTimer = 0.0f;
    chargeDuration = 1.0f;
    attackCooldownTimer = 0.0f;
    attackCooldownDuration = 5.0f;
    projectileSpeedTilesPerSecond = 6.0f;
    detectionLengthTiles = 5.0f;
    detectionWidthScale = 1.15f;
}

_enemyCently::~_enemyCently()
{
    //dtor
    delete chargeShader;
}

void _enemyCently::ensureChargeShader()
{
    if (chargeShaderInitialized || chargeShader == NULL)
    {
        return;
    }

    chargeShader->initShader("shaders/enemy_hit.vs", "shaders/enemy_hit.fs");
    chargeShaderInitialized = true;
}

rect2D _enemyCently::attackTriggerBounds() const
{
    const float tileLength = tileStepDistance * detectionLengthTiles;
    const float halfWidth = (float)scale.x * collisionWidthScale * detectionWidthScale;
    const float halfHeight = (float)scale.y * collisionHeightScale * detectionWidthScale;
    const float centerY = (float)pos.y + collisionOffsetY;
    rect2D bounds;

    bounds.left = (float)pos.x - halfWidth;
    bounds.right = (float)pos.x + halfWidth;
    bounds.bottom = centerY - halfHeight;
    bounds.top = centerY + halfHeight;

    switch (facingDirection)
    {
        case FACE_LEFT:
            bounds.left = (float)pos.x - tileLength;
            bounds.right = (float)pos.x;
            break;

        case FACE_RIGHT:
            bounds.left = (float)pos.x;
            bounds.right = (float)pos.x + tileLength;
            break;

        case FACE_UP:
            bounds.bottom = centerY;
            bounds.top = centerY + tileLength;
            break;

        case FACE_DOWN:
        default:
            bounds.bottom = centerY - tileLength;
            bounds.top = centerY;
            break;
    }

    return bounds;
}

void _enemyCently::startChargeAttack()
{
    haltMove();
    isChargingAttack = true;
    isProjectileAttackActive = false;
    chargeTimer = 0.0f;
    actionTrigger = ATTACK;
}

void _enemyCently::finishAttackCycle()
{
    projectile.deactivate();
    isChargingAttack = false;
    isProjectileAttackActive = false;
    attackCooldownTimer = attackCooldownDuration;
    actionTrigger = STAND;
    timer = 0.0f;
}

void _enemyCently::fireProjectile()
{
    vec2 direction = facingVector();
    vec3 projectileStart = pos;
    vec3 projectileDirection;
    const float launchOffset = tileStepDistance * 0.7f;
    const float projectileScale = tileStepDistance * 0.42f;

    projectileStart.x += direction.x * launchOffset;
    projectileStart.y += direction.y * launchOffset;
    projectileStart.z = pos.z;
    projectileDirection.x = direction.x;
    projectileDirection.y = direction.y;
    projectileDirection.z = 0.0;

    projectile.scale.x = projectileScale;
    projectile.scale.y = projectileScale;
    projectile.scale.z = 1.0f;
    projectile.launch(
        projectileStart,
        projectileDirection,
        tileStepDistance * projectileSpeedTilesPerSecond,
        135.0f
    );
}

void _enemyCently::enmsActions(float deltaT, const vec3* playerPos, const _enemyNavigation* navigation)
{
    startedMoveThisFrame = false;
    animationTimer += deltaT;
    if (damageFlashTimer > 0.0f)
    {
        damageFlashTimer = std::max(0.0f, damageFlashTimer - deltaT);
    }

    if (attackCooldownTimer > 0.0f)
    {
        attackCooldownTimer = std::max(0.0f, attackCooldownTimer - deltaT);
    }

    const float animationCycle = animationInterval * 2.0f;
    if (animationCycle > 0.0f && animationTimer >= animationCycle)
    {
        animationTimer = fmod(animationTimer, animationCycle);
    }

    if (isChargingAttack)
    {
        haltMove();
        chargeTimer += deltaT;

        if (chargeTimer >= chargeDuration)
        {
            isChargingAttack = false;
            isProjectileAttackActive = true;
            chargeTimer = 0.0f;
            fireProjectile();
        }

        setIdleFrame();
        return;
    }

    if (isProjectileAttackActive)
    {
        haltMove();
        projectile.updateTravel(deltaT);

        if (navigation != NULL &&
            (navigation->doesRectHitWall(projectile.collisionBounds()) ||
             navigation->doesRectHitPlayer(projectile.collisionBounds())))
        {
            finishAttackCycle();
        }
        else if (!projectile.isActive())
        {
            finishAttackCycle();
        }

        setIdleFrame();
        return;
    }

    _enemies::enmsActions(deltaT, playerPos, navigation);

    if (navigation != NULL &&
        attackCooldownTimer <= 0.0f &&
        navigation->doesRectHitPlayer(attackTriggerBounds()))
    {
        startChargeAttack();
    }
}

void _enemyCently::drawEnms()
{
    if (isChargingAttack)
    {
        ensureChargeShader();

        if (chargeShaderInitialized && chargeShader != NULL)
        {
            const bool useBrightWhite = ((int)floor(chargeTimer * 10.0f) % 2) == 0;
            vec3 shadowColor;
            vec3 highlightColor;

            if (useBrightWhite)
            {
                shadowColor.x = 0.72f;
                shadowColor.y = 0.72f;
                shadowColor.z = 0.72f;
                highlightColor.x = 1.0f;
                highlightColor.y = 1.0f;
                highlightColor.z = 1.0f;
            }
            else
            {
                shadowColor.x = 0.48f;
                shadowColor.y = 0.48f;
                shadowColor.z = 0.48f;
                highlightColor.x = 0.90f;
                highlightColor.y = 0.90f;
                highlightColor.z = 0.90f;
            }

            drawSelfWithPalette(chargeShader, shadowColor, highlightColor);
        }
        else
        {
            _enemies::drawEnms();
        }
    }
    else
    {
        _enemies::drawEnms();
    }

    projectile.drawBlt();
}

void _enemyCently::initCently()
{
    const int enemySheetWidth = 437;
    const int enemySheetHeight = 346;
    const int centlyRegionWidth = 120;
    const int centlyRegionHeight = 60;

    enmsInit(4, 2, "images/enemies_sheet.png");
    projectile.initBlt(1, 1, "images/bs.png");
    projectile.flashEnabled = true;
    projectile.deactivate();
    setSpriteRegionPixels(
        enemySheetWidth,
        enemySheetHeight,
        0,
        240,
        centlyRegionWidth,
        centlyRegionHeight
    );
}
