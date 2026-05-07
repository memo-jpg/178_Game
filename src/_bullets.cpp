#include "_bullets.h"

_bullets::_bullets()
{
    //ctor
    pos.x = 0;
    pos.y = -0.4;
    pos.z = -7;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;

    scale.x = 0.2;
    scale.y = 0.2;
    scale.z = 1.0;

    speed = 0.0f;
    flashTimer = 0.0f;
    collisionScale = 0.55f;
    flashEnabled = false;
    direction.x = 0.0;
    direction.y = 0.0;
    direction.z = 0.0;
    flashShader = new _shader();
    flashShaderInitialized = false;
}

_bullets::~_bullets()
{
    //dtor
    delete flashShader;
}

void _bullets::ensureFlashShader()
{
    if (flashShaderInitialized || flashShader == NULL)
    {
        return;
    }

    flashShader->initShader("shaders/enemy_hit.vs", "shaders/enemy_hit.fs");
    flashShaderInitialized = true;
}

void _bullets::initBlt(int x, int y, char *fileName)
{
    initQuad(fileName);
    xFrames = x;
    yFrames = y;

    xMin = 0;
    xMax = 1.0/(float)xFrames;
    yMax = 1.0/(float)yFrames;
    yMin = 0;
}

void _bullets::update(vec3)
{
    // TO DO | depends on use of bullets
}

void _bullets::launch(vec3 startPos, vec3 directionVector, float travelSpeed, float rotationOffset)
{
    const float magnitude = sqrt(
        (float)(directionVector.x * directionVector.x) +
        (float)(directionVector.y * directionVector.y) +
        (float)(directionVector.z * directionVector.z)
    );

    if (magnitude <= 0.0001f)
    {
        deactivate();
        return;
    }

    pos = startPos;
    dest = startPos;
    direction.x = directionVector.x / magnitude;
    direction.y = directionVector.y / magnitude;
    direction.z = directionVector.z / magnitude;
    speed = travelSpeed;
    flashTimer = 0.0f;
    timer = 0.0f;
    t = 0.0f;
    rot.z = (atan2((float)direction.y, (float)direction.x) * 180.0f / PI) + rotationOffset;
    actionTrigger = ACTIVE;
    isLive = true;
}

void _bullets::updateTravel(float deltaT)
{
    if (!isActive())
    {
        return;
    }

    flashTimer += deltaT;
    pos.x += direction.x * speed * deltaT;
    pos.y += direction.y * speed * deltaT;
    pos.z += direction.z * speed * deltaT;
}

void _bullets::deactivate()
{
    actionTrigger = IDLE;
    isLive = false;
    timer = 0.0f;
    t = 0.0f;
    speed = 0.0f;
    flashTimer = 0.0f;
}

rect2D _bullets::collisionBounds() const
{
    const float halfWidth = (float)scale.x * collisionScale;
    const float halfHeight = (float)scale.y * collisionScale;

    rect2D bounds;
    bounds.left = (float)pos.x - halfWidth;
    bounds.right = (float)pos.x + halfWidth;
    bounds.bottom = (float)pos.y - halfHeight;
    bounds.top = (float)pos.y + halfHeight;
    return bounds;
}

bool _bullets::isActive() const
{
    return isLive && actionTrigger == ACTIVE;
}

void _bullets::shoot(vec3 spos, vec3 dpos, float deltaT)
{
    if(actionTrigger == IDLE){
        dest = dpos;

        // angle between two points
        // arccos dot product of the points
        /*
        float X = spos.x * dest.x;
        float Y = spos.y * dest.y;
        float Z = spos.z * dest.z;
        float MS = sqrt(pow(spos.x, 2) + pow(spos.y, 2) + pow(spos.z, 2));  // magnitude of source value
        float MD = sqrt(pow(dest.x, 2) + pow(dest.y, 2) + pow(dest.z, 2));  // magnitude of destination

        rot.z = acos((X + Y + Z)/(MS*MD));      // X+Y+Z is dot product
        */

        float X = dest.x - spos.x;      // translate to the center
        float Y = dest.y - spos.y;      // translate to the center

        rot.z = (atan2(Y, X) * 180 / PI) + 140;

    }

    if(isLive){
        timer += deltaT;
        if(timer>0.06){
            pos.x = spos.x + t*(dest.x-spos.x);
            pos.y = spos.y + t*(dest.y-spos.y);
            pos.z = spos.z + t*(dest.z-spos.z);

            if (actionTrigger == ACTIVE){
                if(t > 1){
                    t = 0;
                    actionTrigger = IDLE;       // reset the bullet
                } else {
                    t += (deltaT + 0.1);       // let bullet move on the path
                }
            }

            timer = 0;
        }
    }
}

void _bullets::bulletActions()
{
    switch(actionTrigger){
        case IDLE:
            isLive = false; // if bullet is in the storage
            break;
        case ACTIVE:
            isLive = true;  // bullet is on the move
            break;
        case HIT:
            isLive = false; // when bullet hit the target or move out of screen
            break;
        default:
            break;
    }
}

void _bullets::drawBlt()
{
    if(isLive){
        updateQuad();

        if (flashEnabled)
        {
            ensureFlashShader();
        }

        if (flashEnabled && flashShaderInitialized && flashShader != NULL)
        {
            const bool useRedPalette = ((int)floor(flashTimer * 18.0f) % 2) == 0;
            const GLint textureLoc = glGetUniformLocation(flashShader->program, "u_texture");
            const GLint shadowLoc = glGetUniformLocation(flashShader->program, "u_shadowColor");
            const GLint highlightLoc = glGetUniformLocation(flashShader->program, "u_highlightColor");

            glUseProgram(flashShader->program);
            glUniform1i(textureLoc, 0);

            if (useRedPalette)
            {
                glUniform3f(shadowLoc, 0.45f, 0.02f, 0.02f);
                glUniform3f(highlightLoc, 1.0f, 0.92f, 0.92f);
            }
            else
            {
                glUniform3f(shadowLoc, 0.68f, 0.68f, 0.68f);
                glUniform3f(highlightLoc, 1.0f, 1.0f, 1.0f);
            }
        }

        drawQuad();

        if (flashEnabled && flashShaderInitialized && flashShader != NULL)
        {
            glUseProgram(0);
        }
    }
}
