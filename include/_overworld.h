#ifndef _OVERWORLD_H
#define _OVERWORLD_H

#include<_dungeon.h>

class _overworld : public _dungeon
{
    public:
        _overworld();
        virtual ~_overworld();

        bool load();

    protected:
        void buildTransitions();
        const char* defaultRoomId() const;
};

#endif // _OVERWORLD_H
