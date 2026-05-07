#include "_enemies.h"

_enemies::_enemies()
{
    //ctor
    pos.x = 0;
    pos.y = -0.4;
    pos.z = -7;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;

    scale.x = 0.5;
    scale.y = 0.5;
    scale.z = 1;

    currentFrame = 0;
    actionTrigger = STAND;
    animationTimer = 0.0f;
    animationInterval = 0.35f;
    facingDirection = FACE_DOWN;
    moveInterval = 2.0f;
    moveDuration = 2.0f;
    moveProgress = 0.0f;
    activeMoveDuration = 2.0f;
    frameSampleScale = 0.7f;
    tileStepDistance = 0.5f;
    collisionWidthScale = 0.576f / 0.7f;
    collisionHeightScale = 0.576f / 0.7f;
    collisionOffsetY = 0.0f;
    damageFlashTimer = 0.0f;
    damageFlashDuration = 0.18f;
    knockbackDuration = 0.18f;
    maxHealth = 1;
    health = 1;
    lastHitAttackId = -1;
    isMoving = false;
    isKnockbackActive = false;
    startedMoveThisFrame = false;
    moveStartPos = pos;
    moveTargetPos = pos;
    vel = 30;
    t = 0.1;
    hitShader = new _shader();
    hitShaderInitialized = false;
    regionXMin = 0.0f;
    regionXMax = 1.0f;
    regionYMin = 0.0f;
    regionYMax = 1.0f;
}

_enemies::~_enemies()
{
    //dtor
    delete hitShader;
}
void _enemies::enmsInit(int x, int y, char *fileName)
{
    initQuad(fileName);
    xFrames = x;
    yFrames = y;
    currentFrame = 0;

    setAnimationFrame(0, 0);
}

void _enemies::setSpriteRegionPixels(int atlasWidth, int atlasHeight, int left, int top, int width, int height)
{
    regionXMin = (float)left / (float)atlasWidth;
    regionXMax = (float)(left + width) / (float)atlasWidth;
    regionYMin = (float)top / (float)atlasHeight;
    regionYMax = (float)(top + height) / (float)atlasHeight;

    setAnimationFrame(0, 0);
}

void _enemies::ensureHitShader()
{
    if (hitShaderInitialized || hitShader == NULL)
    {
        return;
    }

    hitShader->initShader("shaders/enemy_hit.vs", "shaders/enemy_hit.fs");
    hitShaderInitialized = true;
}

bool _enemies::isUsingHitShader() const
{
    return damageFlashTimer > 0.0f && hitShaderInitialized && hitShader != NULL;
}

void _enemies::setAnimationFrame(int rowIndex, int frameIndex)
{
    const float frameWidth = (regionXMax - regionXMin) / (float)xFrames;
    const float frameHeight = (regionYMax - regionYMin) / (float)yFrames;
    const float sampleWidth = frameWidth * frameSampleScale;
    const float sampleHeight = frameHeight * frameSampleScale;
    const int wrappedFrame = ((frameIndex % xFrames) + xFrames) % xFrames;
    const int wrappedRow = ((rowIndex % yFrames) + yFrames) % yFrames;

    currentFrame = wrappedFrame;
    xMin = regionXMin + frameWidth * (float)wrappedFrame;
    xMax = xMin + sampleWidth;
    yMin = regionYMin + frameHeight * (float)wrappedRow;
    yMax = yMin + sampleHeight;
}

vec3 _enemies::visualOffset() const
{
    vec3 offset;
    const float spriteCenterFromFrameOrigin = 0.25f;
    const float xOffsetFactor = 1.0f - (spriteCenterFromFrameOrigin / (frameSampleScale * 0.5f));
    const float yOffsetFactor = xOffsetFactor;

    offset.x = scale.x * xOffsetFactor;
    offset.y = -scale.y * yOffsetFactor;
    offset.z = 0.0f;
    return offset;
}

void _enemies::setIdleFrame()
{
    const int animationRow = animationTimer >= animationInterval ? 1 : 0;
    setAnimationFrame(animationRow, directionColumn());
}

int _enemies::directionColumn() const
{
    switch (facingDirection)
    {
        case FACE_LEFT:
            return 1;
        case FACE_RIGHT:
            return 3;
        case FACE_UP:
            return 2;
        case FACE_DOWN:
        default:
            return 0;
    }
}

_enemies::FacingDirection _enemies::randomDirection() const
{
    static std::mt19937 generator((unsigned int)std::chrono::steady_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<int> directionDistribution(0, 3);

    return (FacingDirection)directionDistribution(generator);
}

bool _enemies::chooseNextDirection(FacingDirection& nextDirection, const vec3* playerPos, const _enemyNavigation* navigation) const
{
    (void)playerPos;
    (void)navigation;
    nextDirection = randomDirection();
    return true;
}

rect2D _enemies::collisionBoundsAt(vec3 position) const
{
    const float halfWidth = (float)scale.x * collisionWidthScale;
    const float halfHeight = (float)scale.y * collisionHeightScale;
    const float centerY = (float)position.y + collisionOffsetY;

    rect2D bounds;
    bounds.left = (float)position.x - halfWidth;
    bounds.right = (float)position.x + halfWidth;
    bounds.bottom = centerY - halfHeight;
    bounds.top = centerY + halfHeight;
    return bounds;
}

rect2D _enemies::collisionBounds() const
{
    return collisionBoundsAt(pos);
}

void _enemies::beginMove(FacingDirection nextDirection)
{
    facingDirection = nextDirection;
    moveStartPos = pos;
    moveTargetPos = pos;
    moveProgress = 0.0f;
    activeMoveDuration = moveDuration;
    isMoving = true;
    isKnockbackActive = false;
    startedMoveThisFrame = true;

    switch (nextDirection)
    {
        case FACE_LEFT:
            actionTrigger = LEFTWALK;
            moveTargetPos.x -= tileStepDistance;
            break;

        case FACE_RIGHT:
            actionTrigger = RIGHTWALK;
            moveTargetPos.x += tileStepDistance;
            break;

        case FACE_UP:
            actionTrigger = WALKUP;
            moveTargetPos.y += tileStepDistance;
            break;

        case FACE_DOWN:
        default:
            actionTrigger = WALKDOWN;
            moveTargetPos.y -= tileStepDistance;
            break;
    }
}

void _enemies::beginKnockback(FacingDirection knockbackDirection)
{
    beginMove(knockbackDirection);
    moveProgress = 0.0f;
    activeMoveDuration = knockbackDuration;
    isKnockbackActive = true;
    timer = 0.0f;
}

void _enemies::beginHitFlash()
{
    ensureHitShader();
    damageFlashTimer = damageFlashDuration;
}

void _enemies::cancelMove()
{
    pos = moveStartPos;
    moveTargetPos = moveStartPos;
    moveProgress = 0.0f;
    timer = 0.0f;
    isMoving = false;
    isKnockbackActive = false;
    startedMoveThisFrame = false;
    actionTrigger = STAND;
}

bool _enemies::canBeHitByAttack(int attackId) const
{
    return isEnmsLive && attackId >= 0 && lastHitAttackId != attackId;
}

void _enemies::takeDamage(int damage, int attackId, FacingDirection knockbackDirection)
{
    if (!canBeHitByAttack(attackId))
    {
        return;
    }

    lastHitAttackId = attackId;
    health -= damage;
    beginHitFlash();

    if (health <= 0)
    {
        health = 0;
        isEnmsLive = false;
        cancelMove();
        return;
    }

    beginKnockback(knockbackDirection);
}

void _enemies::enmsActions(float deltaT, const vec3* playerPos, const _enemyNavigation* navigation)
{
    startedMoveThisFrame = false;
    animationTimer += deltaT;
    if (damageFlashTimer > 0.0f)
    {
        damageFlashTimer = std::max(0.0f, damageFlashTimer - deltaT);
    }

    const float animationCycle = animationInterval * 2.0f;
    if (animationCycle > 0.0f && animationTimer >= animationCycle)
    {
        animationTimer = fmod(animationTimer, animationCycle);
    }

    if (isMoving)
    {
        moveProgress += deltaT;
        const float progress = activeMoveDuration > 0.0f ? std::min(moveProgress / activeMoveDuration, 1.0f) : 1.0f;

        pos.x = moveStartPos.x + (moveTargetPos.x - moveStartPos.x) * progress;
        pos.y = moveStartPos.y + (moveTargetPos.y - moveStartPos.y) * progress;
        pos.z = moveStartPos.z + (moveTargetPos.z - moveStartPos.z) * progress;

        if (progress >= 1.0f)
        {
            pos = moveTargetPos;
            moveStartPos = pos;
            moveProgress = 0.0f;
            isMoving = false;
            isKnockbackActive = false;
            timer = 0.0f;
            actionTrigger = STAND;
        }
    }
    else
    {
        timer += deltaT;

        if (timer >= moveInterval)
        {
            FacingDirection nextDirection = facingDirection;

            if (chooseNextDirection(nextDirection, playerPos, navigation))
            {
                beginMove(nextDirection);
            }
            else
            {
                timer = 0.0f;
            }
        }
    }

    setIdleFrame();
}

void _enemies::placeEnms(vec3 Epos, float deltaT)
{
    pos.x = Epos.x;
    pos.y = Epos.y;
    pos.z = Epos.z;
}

void _enemies::drawEnms()
{
    updateQuad();
    const vec3 basePos = pos;
    const vec3 offset = visualOffset();

    if (isUsingHitShader())
    {
        glUseProgram(hitShader->program);

        const float flashProgress = damageFlashDuration > 0.0f ?
            1.0f - (damageFlashTimer / damageFlashDuration) : 1.0f;
        const bool useRedPalette = flashProgress < 0.5f;
        const GLint textureLoc = glGetUniformLocation(hitShader->program, "u_texture");
        const GLint shadowLoc = glGetUniformLocation(hitShader->program, "u_shadowColor");
        const GLint highlightLoc = glGetUniformLocation(hitShader->program, "u_highlightColor");

        glUniform1i(textureLoc, 0);

        if (useRedPalette)
        {
            glUniform3f(shadowLoc, 0.28f, 0.02f, 0.02f);
            glUniform3f(highlightLoc, 1.0f, 0.38f, 0.32f);
        }
        else
        {
            glUniform3f(shadowLoc, 0.03f, 0.06f, 0.24f);
            glUniform3f(highlightLoc, 0.42f, 0.68f, 1.0f);
        }
    }

    pos.x = basePos.x + offset.x;
    pos.y = basePos.y + offset.y;
    pos.z = basePos.z + offset.z;

    drawQuad();

    pos = basePos;

    if (isUsingHitShader())
    {
        glUseProgram(0);
    }
}
