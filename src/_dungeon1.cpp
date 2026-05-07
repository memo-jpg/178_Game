#include "_dungeon1.h"

_dungeon1::_dungeon1()
{
}

_dungeon1::~_dungeon1()
{
}

bool _dungeon1::load()
{
    return loadDungeon("images/Dungeons/level1.png", "images/Dungeons/level1_rooms.txt");
}

void _dungeon1::buildTransitions()
{
    clearTransitions();
    addOverworldExit("top_room", 4, 5, 2, 3);

    addTransition("top_room", SOUTH, 4, 5, "hub_center_left", 4, 5);

    addTransition("hub_left", EAST, 5, 6, "hub_center_left", 5, 6);

    addTransition("hub_center_left", NORTH, 4, 5, "top_room", 4, 5);
    addTransition("hub_center_left", EAST, 5, 6, "hub_center_right", 5, 6);
    addTransition("hub_center_left", WEST, 5, 6, "hub_left", 5, 6);
    addTransition("hub_center_left", SOUTH, 4, 5, "lower_center_room", 4, 5);

    addTransition("hub_center_right", EAST, 5, 6, "hub_right", 5, 6);
    addTransition("hub_center_right", WEST, 5, 6, "hub_center_left", 5, 6);
    addTransition("hub_center_right", SOUTH, 4, 5, "lower_center_room", 17, 18);

    addTransition("hub_right", WEST, 5, 6, "hub_center_right", 5, 6);
    addTransition("hub_right", SOUTH, 4, 5, "lower_right_room", 4, 5);

    addTransition("lower_center_room", NORTH, 4, 5, "hub_center_left", 4, 5);
    addTransition("lower_center_room", NORTH, 17, 18, "hub_center_right", 4, 5);

    addTransition("lower_right_room", NORTH, 4, 5, "hub_right", 4, 5);
    addTransition("lower_right_room", SOUTH, 0, 9, "bottom_room", 9, 10);

    addTransition("bottom_room", NORTH, 9, 10, "lower_right_room", 4, 5);
}

const char* _dungeon1::defaultRoomId() const
{
    return "top_room";
}
