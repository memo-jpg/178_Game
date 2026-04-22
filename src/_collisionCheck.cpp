#include "_collisionCheck.h"

_collisionCheck::_collisionCheck()
{
    //ctor
}

_collisionCheck::~_collisionCheck()
{
    //dtor
}
bool _collisionCheck::isLinearCol(vec3 p1, vec3 p2)
{
    return fabs(p1.x - p2.x) < 0.01f &&
           fabs(p1.y - p2.y) < 0.01f;
}

bool _collisionCheck::isRadialCol(vec3 p1, vec3 p2, float r1, float r2, float thrld)
{
    return (sqrt(pow(p1.x - p2.x, 2) +
                 pow(p1.y - p2.y, 2) +
                 pow(p1.z - p2.z, 2)) - (r1 + r2) < thrld);
}

bool _collisionCheck::isPlanoCol(vec3 p1, vec3 p2)
{
    return fabs(p1.x - p2.x) < 0.01f &&
           fabs(p1.z - p2.z) < 0.01f;
}

bool _collisionCheck::isPointInRect(vec3 point, rect2D rect)
{
    return point.x >= rect.left && point.x <= rect.right &&
           point.y >= rect.bottom && point.y <= rect.top;
}

bool _collisionCheck::isRectCollide(rect2D a, rect2D b)
{
    return !(a.right < b.left || a.left > b.right ||
             a.top < b.bottom || a.bottom > b.top);
}
