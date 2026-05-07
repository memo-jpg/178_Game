#include "_player.h"

_player::_player()
{
    //ctor
    //xMin = 0;
    //xMax = 1.0/9.0;
    //yMax = 1.0/4.0;
    //yMin = 0;

    pos.y = -0.8;

    timer = 0;
    attackTimer = 0;
    attackDuration = 0.18f;
    collisionWidthScale = 0.576f;
    collisionHeightScale = 0.576f;
    collisionOffsetY = 0.0f;
    currentFrame = 0;
    attackId = 0;
    actionTrigger = STAND;
    facingDirection = FACE_DOWN;
    isAttackActive = false;
}

_player::~_player()
{
    //dtor
}

void _player::plyInit(int x, int y, char *fileName)
{
    initQuad(fileName);
    xFrames = x;
    yFrames = y;

    xMin = 0;
    xMax = 1.0/(float)xFrames;
    //xMax = 1.0/9.0;
    yMax = 1.0/(float)yFrames;
    yMin = 0;

}

vec3 _player::visualOffset() const
{
    vec3 offset;
    offset.x = 0.0;
    offset.y = 0.0;
    offset.z = 0.0;

    switch (facingDirection)
    {
        case FACE_LEFT:
            offset.x = -0.028;
            break;
        case FACE_RIGHT:
            offset.x = 0.028;
            break;
        case FACE_UP:
            offset.y = -0.012;
            break;
        case FACE_DOWN:
        default:
            break;
    }

    return offset;
}

void _player::drawPlayer()
{
    const vec3 basePos = pos;
    const vec3 offset = visualOffset();

    pos.x = basePos.x + offset.x;
    pos.y = basePos.y + offset.y;
    pos.z = basePos.z + offset.z;

    drawQuad();

    pos = basePos;
}

void _player::setAnimationFrame(int rowIndex, int frameIndex)
{
    const float frameWidth = 1.0f / (float)xFrames;
    const float frameHeight = 1.0f / (float)yFrames;
    const int wrappedFrame = ((frameIndex % xFrames) + xFrames) % xFrames;

    currentFrame = wrappedFrame;
    xMin = frameWidth * (float)wrappedFrame;
    xMax = xMin + frameWidth;
    yMin = frameHeight * (float)rowIndex;
    yMax = yMin + frameHeight;
}

void _player::setIdleFrame()
{
    switch (facingDirection)
    {
        case FACE_LEFT:
            setAnimationFrame(1, 0);
            break;
        case FACE_RIGHT:
            setAnimationFrame(2, 0);
            break;
        case FACE_UP:
            setAnimationFrame(3, 0);
            break;
        case FACE_DOWN:
        default:
            setAnimationFrame(0, 0);
            break;
    }
}

void _player::startAttack()
{
    if (isAttackActive)
    {
        return;
    }

    isAttackActive = true;
    attackId++;
    timer = 0.0f;
    attackTimer = 0.0f;
    actionTrigger = ATTACK;
    setIdleFrame();
}

void _player::finishAttack()
{
    isAttackActive = false;
    timer = 0.0f;
    attackTimer = 0.0f;
    actionTrigger = STAND;
    setIdleFrame();
}

bool _player::canMove() const
{
    return !isAttackActive;
}

float _player::getAttackProgress() const
{
    if (!isAttackActive || attackDuration <= 0.0f)
    {
        return 0.0f;
    }

    float progress = attackTimer / attackDuration;
    if (progress < 0.0f)
    {
        return 0.0f;
    }
    if (progress > 1.0f)
    {
        return 1.0f;
    }

    return progress;
}

float _player::getFacingAngleDegrees() const
{
    switch (facingDirection)
    {
        case FACE_LEFT: return 180.0f;
        case FACE_RIGHT: return 0.0f;
        case FACE_UP: return 90.0f;
        case FACE_DOWN:
        default:
            return -90.0f;
    }
}

int _player::currentAttackId() const
{
    return attackId;
}

rect2D _player::collisionBoundsAt(vec3 position) const
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

rect2D _player::collisionBounds() const
{
    return collisionBoundsAt(pos);
}

quad2D _player::attackQuad() const
{
    const float sweepDegrees = 135.0f;
    const float startOffsetDegrees = 45.0f;
    const float angleDegrees = (getFacingAngleDegrees() + startOffsetDegrees) - (sweepDegrees * getAttackProgress());
    const float angleRadians = angleDegrees * PI / 180.0f;
    const float swordHalfWidth = (float)scale.x * 0.20f;
    const float swordLength = (float)scale.y * 1.95f;
    const float hiltOffset = (float)scale.y * 0.35f;

    const float originX = (float)pos.x;
    const float originY = (float)pos.y + collisionOffsetY;

    const float forwardX = cos(angleRadians);
    const float forwardY = sin(angleRadians);
    const float sideX = -forwardY;
    const float sideY = forwardX;

    const float nearX = originX + forwardX * hiltOffset;
    const float nearY = originY + forwardY * hiltOffset;
    const float farX = originX + forwardX * (hiltOffset + swordLength);
    const float farY = originY + forwardY * (hiltOffset + swordLength);

    quad2D quad;
    quad.points[0].x = nearX - sideX * swordHalfWidth;
    quad.points[0].y = nearY - sideY * swordHalfWidth;
    quad.points[1].x = nearX + sideX * swordHalfWidth;
    quad.points[1].y = nearY + sideY * swordHalfWidth;
    quad.points[2].x = farX + sideX * swordHalfWidth;
    quad.points[2].y = farY + sideY * swordHalfWidth;
    quad.points[3].x = farX - sideX * swordHalfWidth;
    quad.points[3].y = farY - sideY * swordHalfWidth;

    return quad;
}

rect2D _player::attackBounds() const
{
    quad2D swordQuad = attackQuad();
    rect2D bounds;
    bounds.left = swordQuad.points[0].x;
    bounds.right = swordQuad.points[0].x;
    bounds.bottom = swordQuad.points[0].y;
    bounds.top = swordQuad.points[0].y;

    for (int i = 1; i < 4; i++)
    {
        bounds.left = min(bounds.left, swordQuad.points[i].x);
        bounds.right = max(bounds.right, swordQuad.points[i].x);
        bounds.bottom = min(bounds.bottom, swordQuad.points[i].y);
        bounds.top = max(bounds.top, swordQuad.points[i].y);
    }

    return bounds;
}

void _player::playerActions(float deltaT)
{
    const float moveSpeed = 1.65f;

    timer += deltaT;

    if (isAttackActive)
    {
        attackTimer += deltaT;
        setIdleFrame();

        if (attackTimer >= attackDuration)
        {
            finishAttack();
        }

        return;
    }

    switch(actionTrigger){
        case STAND:
            setIdleFrame();
            break;
        case LEFTWALK:
            facingDirection = FACE_LEFT;
            pos.x -= moveSpeed * deltaT;
            if(timer>0.08){
                setAnimationFrame(1, currentFrame + 1);

                timer = 0;
            }
            break;
        case RIGHTWALK:
            facingDirection = FACE_RIGHT;
            pos.x += moveSpeed * deltaT;
            if(timer>0.08){
                setAnimationFrame(2, currentFrame + 1);

                timer = 0;
            }
            break;
        case WALKUP:
            facingDirection = FACE_UP;
            pos.y += moveSpeed * deltaT;
            if(timer>0.08){
                setAnimationFrame(3, currentFrame + 1);

                timer = 0;
            }
            break;
        case WALKDOWN:
            facingDirection = FACE_DOWN;
            pos.y -= moveSpeed * deltaT;
            if(timer>0.08){
                setAnimationFrame(0, currentFrame + 1);

                timer = 0;
            }
            break;
        case ATTACK:
            setIdleFrame();
            break;
        default: break;

    }
}
