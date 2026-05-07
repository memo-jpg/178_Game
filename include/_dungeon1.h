#ifndef _DUNGEON1_H
#define _DUNGEON1_H

#include<_dungeon.h>

class _dungeon1 : public _dungeon
{
    public:
        _dungeon1();
        virtual ~_dungeon1();

        bool load();

    protected:
        void buildTransitions();
        const char* defaultRoomId() const;
};

#endif // _DUNGEON1_H
