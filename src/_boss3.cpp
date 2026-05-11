#include "_boss3.h"

_boss3::_boss3()
{
    pos.x = 0.0f;
    pos.y = -0.85f;
    pos.z = -8.0f;

    scale.x = 0.32f;
    scale.y = 0.32f;
    scale.z = 1.0f;

    maxHealth = 9;
    health = 9;
    actionTrigger = STAND;
    moveInterval = 0.75f;
    moveDuration = 0.3f;
    chargeShader = new _shader();
    chargeShaderInitialized = false;
    isChargingAttack = false;
    isProjectileAttackActive = false;
    chargeTimer = 0.0f;
    chargeDuration = 0.75f;
    attackCooldownTimer = 0.0f;
    attackCooldownDuration = 2.8f;
    projectileSpeedTilesPerSecond = 9.0f;
    detectionLengthTiles = 7.0f;
    detectionWidthScale = 1.4f;
}

_boss3::~_boss3()
{
    delete chargeShader;
}

void _boss3::ensureChargeShader()
{
    if (chargeShaderInitialized || chargeShader == NULL)
    {
        return;
    }

    chargeShader->initShader("shaders/enemy_hit.vs", "shaders/enemy_hit.fs");
    chargeShaderInitialized = true;
}

rect2D _boss3::attackTriggerBounds() const
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

void _boss3::startChargeAttack()
{
    haltMove();
    isChargingAttack = true;
    isProjectileAttackActive = false;
    chargeTimer = 0.0f;
    actionTrigger = ATTACK;
}

void _boss3::finishAttackCycle()
{
    projectile.deactivate();
    isChargingAttack = false;
    isProjectileAttackActive = false;
    attackCooldownTimer = attackCooldownDuration;
    actionTrigger = STAND;
    timer = 0.0f;
}

void _boss3::fireProjectile()
{
    vec2 direction = facingVector();
    vec3 projectileStart = pos;
    vec3 projectileDirection;
    const float launchOffset = tileStepDistance * 0.8f;
    const float projectileScale = tileStepDistance * 0.6f;

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

void _boss3::enmsActions(float deltaT, const vec3* playerPos, const _enemyNavigation* navigation)
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

        const rect2D projectileBounds = projectile.collisionBounds();
        const bool projectileHitWall = navigation != NULL && navigation->doesRectHitWall(projectileBounds);
        const bool projectileHitPlayer = navigation != NULL && navigation->tryDamagePlayer(projectileBounds);

        if (projectileHitWall || projectileHitPlayer)
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

void _boss3::drawEnms()
{
    if (isChargingAttack)
    {
        ensureChargeShader();

        if (chargeShaderInitialized && chargeShader != NULL)
        {
            const bool useBrightWhite = ((int)floor(chargeTimer * 12.0f) % 2) == 0;
            vec3 shadowColor;
            vec3 highlightColor;

            if (useBrightWhite)
            {
                shadowColor.x = 0.70f;
                shadowColor.y = 0.70f;
                shadowColor.z = 0.70f;
                highlightColor.x = 1.0f;
                highlightColor.y = 1.0f;
                highlightColor.z = 1.0f;
            }
            else
            {
                shadowColor.x = 0.18f;
                shadowColor.y = 0.18f;
                shadowColor.z = 0.18f;
                highlightColor.x = 0.78f;
                highlightColor.y = 0.92f;
                highlightColor.z = 1.0f;
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

void _boss3::initBoss3()
{
    const int enemySheetWidth = 437;
    const int enemySheetHeight = 346;
    const int centlyRegionWidth = 120;
    const int centlyRegionHeight = 60;

    initBossSprite(
        4,
        2,
        "images/enemies_sheet.png",
        enemySheetWidth,
        enemySheetHeight,
        0,
        240,
        centlyRegionWidth,
        centlyRegionHeight
    );

    projectile.initBlt(1, 1, "images/bs.png");
    projectile.flashEnabled = true;
    projectile.deactivate();
}
