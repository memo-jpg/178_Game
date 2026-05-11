#ifndef _BOSS2_H
#define _BOSS2_H

#include<_boss.h>

class _boss2: public _boss
{
    public:
        _boss2();
        virtual ~_boss2();

        void initBoss2();

    protected:
        bool chooseNextDirection(FacingDirection&, const vec3*, const _enemyNavigation*) const;
};

#endif // _BOSS2_H
