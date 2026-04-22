#ifndef _COLLISIONCHECK_H
#define _COLLISIONCHECK_H

#include<_common.h>

class _collisionCheck
{
    public:
        _collisionCheck();
        virtual ~_collisionCheck();

        bool isLinearCol(vec3, vec3);   // check only x & y
        bool isRadialCol(vec3, vec3, float, float, float); //
        //positions, radius, threshold

        bool isPlanoCol(vec3, vec3);
        bool isPointInRect(vec3, rect2D);
        bool isRectCollide(rect2D, rect2D);

    protected:

    private:
};

#endif // _COLLISIONCHECK_H
