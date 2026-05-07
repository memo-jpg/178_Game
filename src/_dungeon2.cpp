#include "_dungeon2.h"

_dungeon2::_dungeon2()
{
}

_dungeon2::~_dungeon2()
{
}

bool _dungeon2::load()
{
    return loadDungeon("images/Dungeons/level2.png", "images/Dungeons/level2_rooms.txt");
}

void _dungeon2::buildTransitions()
{
    clearTransitions();

    addOverworldExit("left_room", 2, 3, 4, 5);

    addTransition("left_room", EAST, 4, 5, "hub_left", 4, 5);

    addTransition("top_left_room", SOUTH, 4, 5, "hub_left", 4, 5);

    addTransition("hub_left", NORTH, 4, 5, "top_left_room", 4, 5);
    addTransition("hub_left", EAST, 4, 5, "hub_center", 4, 5);
    addTransition("hub_left", WEST, 4, 5, "left_room", 4, 5);

    addTransition("hub_center", EAST, 4, 5, "hub_right", 4, 5);
    addTransition("hub_center", WEST, 4, 5, "hub_left", 4, 5);
    addTransition("hub_center", SOUTH, 4, 5, "lower_center", 4, 5);

    addTransition("hub_right", EAST, 4, 5, "right_room", 4, 5);
    addTransition("hub_right", SOUTH, 4, 5, "lower_center", 17, 18);
    addTransition("hub_right", WEST, 4, 5, "hub_center", 4, 5);

    addTransition("top_right_room", SOUTH, 4, 5, "right_room", 4, 5);

    addTransition("right_room", NORTH, 4, 5, "top_right_room", 4, 5);
    addTransition("right_room", SOUTH, 4, 5, "boss_room", 9, 10);
    addTransition("right_room", WEST, 4, 5, "hub_right", 4, 5);

    addTransition("lower_center", NORTH, 4, 5, "hub_center", 4, 5);
    addTransition("lower_center", NORTH, 17, 18, "hub_right", 4, 5);
    addTransition("lower_center", SOUTH, 4, 5, "bottom_room", 4, 5);

    addTransition("bottom_room", NORTH, 4, 5, "lower_center", 4, 5);

    addTransition("boss_room", NORTH, 9, 10, "right_room", 4, 5);
}

const char* _dungeon2::defaultRoomId() const
{
    return "left_room";
}
