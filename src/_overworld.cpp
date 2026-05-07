#include "_overworld.h"

_overworld::_overworld()
{
    //ctor
}

_overworld::~_overworld()
{
    //dtor
}

bool _overworld::load()
{
    return loadDungeon("images/overworld.png", "images/overworld_rooms.txt");
}

void _overworld::buildTransitions()
{
    clearTransitions();

    addTransition("hub_room", NORTH, 0, 3, "left_room", 4, 5);
    addTransition("hub_room", NORTH, 11, 12, "center_room", 4, 5);
    addTransition("hub_room", NORTH, 19, 22, "right_room", 4, 5);

    addTransition("left_room", SOUTH, 4, 5, "hub_room", 0, 3);
    addTransition("center_room", SOUTH, 4, 5, "hub_room", 11, 12);
    addTransition("right_room", SOUTH, 4, 5, "hub_room", 19, 22);
}

const char* _overworld::defaultRoomId() const
{
    return "hub_room";
}
