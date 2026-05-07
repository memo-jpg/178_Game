#ifndef _DUNGEON2_H
#define _DUNGEON2_H

#include<_dungeon.h>

class _dungeon2 : public _dungeon
{
    public:
        _dungeon2();
        virtual ~_dungeon2();

        bool load();

    protected:
        void buildTransitions();
        const char* defaultRoomId() const;
};

#endif // _DUNGEON2_H
