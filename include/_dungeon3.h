#ifndef _DUNGEON3_H
#define _DUNGEON3_H

#include<_dungeon.h>

class _dungeon3 : public _dungeon
{
    public:
        _dungeon3();
        virtual ~_dungeon3();

        bool load();

    protected:
        void buildTransitions();
        const char* defaultRoomId() const;
};

#endif // _DUNGEON3_H
