#ifndef _BOSS_H
#define _BOSS_H

#include<_enemies.h>

class _boss: public _enemies
{
    public:
        _boss();
        virtual ~_boss();

    protected:
        void initBossSprite(int, int, char*, int, int, int, int, int, int);
};

#endif // _BOSS_H
