#include "_dungeon3.h"

_dungeon3::_dungeon3()
{
}

_dungeon3::~_dungeon3()
{
}

bool _dungeon3::load()
{
    return loadDungeon("images/Dungeons/level3.png", "images/Dungeons/level3_rooms.txt");
}

void _dungeon3::buildTransitions()
{
    clearTransitions();

    addOverworldExit("left_room", 2, 3, 4, 5);

    addTransition("left_room", EAST, 4, 5, "center_left", 4, 5);

    addTransition("upper_left", EAST, 4, 5, "upper_right", 4, 5);
    addTransition("upper_left", SOUTH, 4, 5, "mid_left", 4, 5);

    addTransition("mid_left", NORTH, 4, 5, "upper_left", 4, 5);
    addTransition("mid_left", SOUTH, 4, 5, "center_left", 4, 5);

    addTransition("center_left", NORTH, 4, 5, "mid_left", 4, 5);
    addTransition("center_left", SOUTH, 4, 5, "bottom_left", 4, 5);
    addTransition("center_left", WEST, 4, 5, "left_room", 4, 5);

    addTransition("bottom_left", NORTH, 4, 5, "center_left", 4, 5);
    addTransition("bottom_left", EAST, 4, 5, "bottom_center", 4, 5);

    addTransition("bottom_center", NORTH, 17, 18, "center_right", 4, 5);
    addTransition("bottom_center", WEST, 4, 5, "bottom_left", 4, 5);

    addTransition("upper_right", SOUTH, 4, 5, "mid_right", 4, 5);
    addTransition("upper_right", WEST, 4, 5, "upper_left", 4, 5);

    addTransition("mid_right", NORTH, 4, 5, "upper_right", 4, 5);
    addTransition("mid_right", SOUTH, 4, 5, "center_right", 4, 5);

    addTransition("center_right", NORTH, 4, 5, "mid_right", 4, 5);
    addTransition("center_right", EAST, 4, 5, "right_room", 4, 5);
    addTransition("center_right", SOUTH, 4, 5, "bottom_center", 17, 18);

    addTransition("right_room", NORTH, 4, 5, "boss_room", 3, 4);
    addTransition("right_room", WEST, 4, 5, "center_right", 4, 5);

    addTransition("boss_room", SOUTH, 3, 4, "right_room", 4, 5);
}

const char* _dungeon3::defaultRoomId() const
{
    return "left_room";
}
