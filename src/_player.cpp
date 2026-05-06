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

rect2D _player::attackBounds() const
{
    const float sweepDegrees = 45.0f;
    const float angleDegrees = getFacingAngleDegrees() - (sweepDegrees * getAttackProgress());
    const float angleRadians = angleDegrees * PI / 180.0f;

    const float hitboxHalfWidth = 0.16f;
    const float hitboxHalfHeight = 0.10f;
    const float reach = 0.42f;

    const float centerX = (float)pos.x + cos(angleRadians) * reach;
    const float centerY = (float)pos.y + sin(angleRadians) * reach;

    rect2D bounds;
    bounds.left = centerX - hitboxHalfWidth;
    bounds.right = centerX + hitboxHalfWidth;
    bounds.bottom = centerY - hitboxHalfHeight;
    bounds.top = centerY + hitboxHalfHeight;
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

        case BLOCK:
            break;

        case HIT: //for when the PLAYER gets hit
            break;

        case ATTACK:
            setIdleFrame();
            break;

        default: break;

    }
}
