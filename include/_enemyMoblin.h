#ifndef _ENEMYMOBLIN_H
#define _ENEMYMOBLIN_H

#include<_enemies.h>

class _enemyMoblin: public _enemies
{
    public:
        _enemyMoblin();
        virtual ~_enemyMoblin();

        void initMoblin();

    protected:
        bool chooseNextDirection(FacingDirection&, const vec3*, const _enemyNavigation*) const;
};

#endif // _ENEMYMOBLIN_H
