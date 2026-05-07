#include "_inputs.h"

_inputs::_inputs()
{
    //ctor

    isRotate = false;
    isTranslate = false;
    nextDirectionOrder = 0;

    for (int i = 0; i < DIR_COUNT; i++)
    {
        directionHeld[i] = false;
        directionOrder[i] = 0;
    }
}

_inputs::~_inputs()
{
    //dtor
}
void _inputs::keyPressed(_model *mdl)
{
    //cout<< wParam << endl;
    switch(wParam){
    case VK_LEFT:
        mdl->rotation.y += 1.0;
        break;

    case VK_RIGHT:
        mdl->rotation.y -= 1.0;
        break;

    case VK_UP:
        mdl->rotation.x += 1.0;
        break;

    case VK_DOWN:
        mdl->rotation.x -= 1.0;
        break;
    }
}

void _inputs::keyUp()
{
    switch(wParam){
        default: break;
    }
}

int _inputs::directionIndexForKey(WPARAM key) const
{
    switch (key)
    {
        case VK_LEFT:
            return DIR_LEFT;
        case VK_RIGHT:
            return DIR_RIGHT;
        case VK_UP:
            return DIR_UP;
        case VK_DOWN:
            return DIR_DOWN;
        default:
            return -1;
    }
}

void _inputs::handleKeyDown(WPARAM key)
{
    wParam = key;

    const int directionIndex = directionIndexForKey(key);
    if (directionIndex >= 0)
    {
        if (!directionHeld[directionIndex])
        {
            directionOrder[directionIndex] = ++nextDirectionOrder;
        }

        directionHeld[directionIndex] = true;
    }
}

void _inputs::handleKeyUp(WPARAM key)
{
    wParam = key;

    const int directionIndex = directionIndexForKey(key);
    if (directionIndex >= 0)
    {
        directionHeld[directionIndex] = false;
        directionOrder[directionIndex] = 0;
    }
}

void _inputs::syncPlayerMovement(_player* qD)
{
    if (qD == NULL || qD->isAttackActive)
    {
        return;
    }

    int activeDirection = -1;
    unsigned long latestOrder = 0;

    for (int i = 0; i < DIR_COUNT; i++)
    {
        if (directionHeld[i] && directionOrder[i] >= latestOrder)
        {
            latestOrder = directionOrder[i];
            activeDirection = i;
        }
    }

    switch (activeDirection)
    {
        case DIR_LEFT:
            qD->facingDirection = _player::FACE_LEFT;
            qD->actionTrigger = qD->LEFTWALK;
            break;
        case DIR_RIGHT:
            qD->facingDirection = _player::FACE_RIGHT;
            qD->actionTrigger = qD->RIGHTWALK;
            break;
        case DIR_UP:
            qD->facingDirection = _player::FACE_UP;
            qD->actionTrigger = qD->WALKUP;
            break;
        case DIR_DOWN:
            qD->facingDirection = _player::FACE_DOWN;
            qD->actionTrigger = qD->WALKDOWN;
            break;
        default:
            qD->actionTrigger = qD->STAND;
            break;
    }
}

void _inputs::mouseEventDown(_model *mdl, double x, double y)
{
    prevMx = x;
    prevMy = y;

    switch(wParam){
    case MK_LBUTTON:
        isRotate = true;
        break;
    case MK_RBUTTON:
        isTranslate = true;
        break;
    case MK_MBUTTON: break;
    default: break;
    }
}

void _inputs::mouseEventUp()
{
    isRotate = false;
    isTranslate = false;
}

void _inputs::mouseMove(_model *mdl, double x, double y)
{
    if(isRotate){
        mdl->rotation.y += (x-prevMx)/3.0;
        mdl->rotation.x += (y-prevMy)/3.0;
    }
    if(isTranslate){
        mdl->pos.x += (x-prevMx)/100.0;
        mdl->pos.y += (y-prevMy)/100.0;
    }

    prevMx = x;
    prevMy = y;
}

void _inputs::mouseWheel(_model *mdl, double delta)
{
    mdl->pos.z += delta/100.0;
}
void _inputs::keyPressed(_modelVBO* mdlv)
{
     switch(wParam){

    case 65:    // KEY A
        mdlv->rot.y -= 1.0;
        break;

    case 68:    // KEY D
        mdlv->rot.y += 1.0;
        break;

    case 87:    // KEY W
        mdlv->rot.x -= 1.0;
        break;

    case 83:    // KEY S
        mdlv->rot.x += 1.0;
        break;
    }
}

void _inputs::mouseEventDown(_modelVBO* mdlv, double x, double y)
{
    prevMxVBO = x;
    prevMyVBO = y;
}

void _inputs::mouseMove(_modelVBO* mdlv, double x, double y)
{
    if(isRotate){
        mdlv->rot.y += (x-prevMxVBO)/3.0;
        mdlv->rot.x += (y-prevMyVBO)/3.0;
    }
    if(isTranslate){
        mdlv->pos.x += (x-prevMxVBO)/100.0;
        mdlv->pos.y += (y-prevMyVBO)/100.0;
    }

    prevMxVBO = x;
    prevMyVBO = y;
}

void _inputs::mouseWheel(_modelVBO* mdlv, double delta)
{
    mdlv->pos.z += delta/100.0;
}

void _inputs::keyPressed(_player* qD)
{
    if (wParam == VK_SPACE)
    {
        qD->startAttack();
        return;
    }

    if (qD->isAttackActive)
    {
        return;
    }

    syncPlayerMovement(qD);
}
