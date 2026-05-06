#include "_dungeon.h"

namespace
{
    bool isTransitionMatch(float tileCoord, int minTile, int maxTile)
    {
        return tileCoord >= (float)minTile - 0.35f && tileCoord <= (float)maxTile + 0.35f;
    }
}

_dungeon::_dungeon()
{
    sourceTileSize = 16;
    maxRoomWorldWidth = 8.8f;
    maxRoomWorldHeight = 5.6f;
    minTileWorldSize = 0.18f;
    maxTileWorldSize = 0.42f;
    loaded = false;

    spawnWorld.x = 0.0;
    spawnWorld.y = 0.0;
    spawnWorld.z = -8.0;

    roomQuad.pos.x = 0.0f;
    roomQuad.pos.y = 0.0f;
    roomQuad.pos.z = -8.0f;
}

_dungeon::~_dungeon()
{
    //dtor
}

bool _dungeon::loadDungeon(const char* mapImagePath, const char* collisionMapPath)
{
    loaded = false;
    rooms.clear();
    currentRoomId.clear();

    roomQuad.initQuad((char*)mapImagePath);

    if (!parseCollisionMap(collisionMapPath))
    {
        return false;
    }

    buildDungeon1Transitions();
    loaded = !rooms.empty();
    return loaded;
}

bool _dungeon::parseCollisionMap(const char* collisionMapPath)
{
    ifstream file(collisionMapPath);
    if (!file.is_open())
    {
        cout << "ERROR: Could not open dungeon collision file: " << collisionMapPath << endl;
        return false;
    }

    DungeonRoom room;
    bool inRoom = false;
    bool inMap = false;
    string line;

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (line.empty())
        {
            continue;
        }

        if (inMap && line != "END")
        {
            room.collisionRows.push_back(line);
            continue;
        }

        istringstream stream(line);
        string token;
        stream >> token;

        if (token == "TILE_SIZE")
        {
            stream >> sourceTileSize;
            continue;
        }

        if (token == "ROOM")
        {
            room = DungeonRoom();
            stream >> room.id;
            room.defaultSpawnX = 0.0f;
            room.defaultSpawnY = 0.0f;
            inRoom = true;
            inMap = false;
            continue;
        }

        if (!inRoom)
        {
            continue;
        }

        if (token == "BOUNDS")
        {
            stream >> room.sourceTileX >> room.sourceTileY >> room.widthTiles >> room.heightTiles;
        }
        else if (token == "SPAWN")
        {
            stream >> room.defaultSpawnX >> room.defaultSpawnY;
        }
        else if (token == "MAP")
        {
            room.collisionRows.clear();
            inMap = true;
        }
        else if (token == "END")
        {
            inMap = false;
            inRoom = false;

            if ((int)room.collisionRows.size() != room.heightTiles)
            {
                cout << "ERROR: Room " << room.id << " has " << room.collisionRows.size()
                     << " collision rows, expected " << room.heightTiles << endl;
                return false;
            }

            for (size_t i = 0; i < room.collisionRows.size(); i++)
            {
                if ((int)room.collisionRows[i].size() != room.widthTiles)
                {
                    cout << "ERROR: Room " << room.id << " has invalid row width at row " << i << endl;
                    return false;
                }
            }

            rooms.push_back(room);
        }
    }

    return !rooms.empty();
}

void _dungeon::buildDungeon1Transitions()
{
    for (size_t i = 0; i < rooms.size(); i++)
    {
        rooms[i].transitions.clear();
    }

    addTransition("top_room", SOUTH, 4, 5, "hub_center_left", 4.0f, 3.0f);

    addTransition("hub_left", EAST, 5, 6, "hub_center_left", 1.0f, 5.0f);

    addTransition("hub_center_left", NORTH, 4, 5, "top_room", 4.0f, 6.0f);
    addTransition("hub_center_left", EAST, 5, 6, "hub_center_right", 1.0f, 5.0f);
    addTransition("hub_center_left", WEST, 5, 6, "hub_left", 8.0f, 5.0f);
    addTransition("hub_center_left", SOUTH, 4, 5, "lower_center_room", 4.0f, 3.0f);

    addTransition("hub_center_right", EAST, 5, 6, "hub_right", 1.0f, 5.0f);
    addTransition("hub_center_right", WEST, 5, 6, "hub_center_left", 8.0f, 5.0f);
    addTransition("hub_center_right", SOUTH, 4, 5, "lower_center_room", 17.0f, 3.0f);

    addTransition("hub_right", WEST, 5, 6, "hub_center_right", 8.0f, 5.0f);
    addTransition("hub_right", SOUTH, 4, 5, "lower_right_room", 5.0f, 3.0f);

    addTransition("lower_center_room", NORTH, 4, 5, "hub_center_left", 4.0f, 8.0f);
    addTransition("lower_center_room", NORTH, 17, 18, "hub_center_right", 4.0f, 8.0f);

    addTransition("lower_right_room", NORTH, 4, 5, "hub_right", 5.0f, 8.0f);
    addTransition("lower_right_room", SOUTH, 0, 9, "bottom_room", 9.0f, 3.0f);

    addTransition("bottom_room", NORTH, 9, 10, "lower_right_room", 4.0f, 14.0f);
}

void _dungeon::addTransition(const std::string& roomId,
                             TransitionSide side,
                             int minTile,
                             int maxTile,
                             const std::string& targetRoomId,
                             float targetTileX,
                             float targetTileY)
{
    DungeonRoom* room = findRoom(roomId);
    if (!room)
    {
        return;
    }

    RoomTransition transition;
    transition.side = side;
    transition.minTile = minTile;
    transition.maxTile = maxTile;
    transition.targetRoomId = targetRoomId;
    transition.targetTileX = targetTileX;
    transition.targetTileY = targetTileY;
    room->transitions.push_back(transition);
}

const _dungeon::DungeonRoom* _dungeon::findRoom(const std::string& roomId) const
{
    for (size_t i = 0; i < rooms.size(); i++)
    {
        if (rooms[i].id == roomId)
        {
            return &rooms[i];
        }
    }

    return NULL;
}

_dungeon::DungeonRoom* _dungeon::findRoom(const std::string& roomId)
{
    for (size_t i = 0; i < rooms.size(); i++)
    {
        if (rooms[i].id == roomId)
        {
            return &rooms[i];
        }
    }

    return NULL;
}

bool _dungeon::enterRoom(const std::string& roomId, float spawnTileX, float spawnTileY)
{
    const DungeonRoom* room = findRoom(roomId);
    if (!room)
    {
        return false;
    }

    currentRoomId = roomId;
    syncCurrentRoomTexture();

    if (spawnTileX < 0.0f)
    {
        spawnTileX = room->defaultSpawnX;
    }

    if (spawnTileY < 0.0f)
    {
        spawnTileY = room->defaultSpawnY;
    }

    spawnWorld = worldPositionForTile(spawnTileX, spawnTileY);
    return true;
}

void _dungeon::syncCurrentRoomTexture()
{
    const DungeonRoom* room = findRoom(currentRoomId);
    if (!room)
    {
        return;
    }

    const float imageWidth = (float)roomQuad.myTex->width;
    const float imageHeight = (float)roomQuad.myTex->height;
    const float sourceLeft = (float)(room->sourceTileX * sourceTileSize);
    const float sourceRight = (float)((room->sourceTileX + room->widthTiles) * sourceTileSize);
    const float sourceTop = (float)(room->sourceTileY * sourceTileSize);
    const float sourceBottom = (float)((room->sourceTileY + room->heightTiles) * sourceTileSize);

    roomQuad.xMin = sourceLeft / imageWidth;
    roomQuad.xMax = sourceRight / imageWidth;
    roomQuad.yMin = sourceTop / imageHeight;
    roomQuad.yMax = sourceBottom / imageHeight;
    roomQuad.scale.x = roomWorldWidth() * 0.5f;
    roomQuad.scale.y = roomWorldHeight() * 0.5f;
    roomQuad.scale.z = 1.0f;
    roomQuad.updateQuad();
}

float _dungeon::currentTileWorldSize() const
{
    const DungeonRoom* room = findRoom(currentRoomId);
    if (!room)
    {
        return minTileWorldSize;
    }

    const float widthSize = maxRoomWorldWidth / (float)room->widthTiles;
    const float heightSize = maxRoomWorldHeight / (float)room->heightTiles;
    const float unclamped = std::min(widthSize, heightSize);
    return std::max(minTileWorldSize, std::min(maxTileWorldSize, unclamped));
}

float _dungeon::roomWorldWidth() const
{
    const DungeonRoom* room = findRoom(currentRoomId);
    return room ? currentTileWorldSize() * (float)room->widthTiles : 0.0f;
}

float _dungeon::roomWorldHeight() const
{
    const DungeonRoom* room = findRoom(currentRoomId);
    return room ? currentTileWorldSize() * (float)room->heightTiles : 0.0f;
}

float _dungeon::roomLeft() const
{
    return -roomWorldWidth() * 0.5f;
}

float _dungeon::roomRight() const
{
    return roomWorldWidth() * 0.5f;
}

float _dungeon::roomTop() const
{
    return roomWorldHeight() * 0.5f;
}

float _dungeon::roomBottom() const
{
    return -roomWorldHeight() * 0.5f;
}

float _dungeon::localTileXForWorld(float worldX) const
{
    return ((worldX - roomLeft()) / currentTileWorldSize()) - 0.5f;
}

float _dungeon::localTileYForWorld(float worldY) const
{
    return ((roomTop() - worldY) / currentTileWorldSize()) - 0.5f;
}

vec3 _dungeon::worldPositionForTile(float tileX, float tileY) const
{
    vec3 world;
    const float tileWorldSize = currentTileWorldSize();
    world.x = roomLeft() + ((tileX + 0.5f) * tileWorldSize);
    world.y = roomTop() - ((tileY + 0.5f) * tileWorldSize);
    world.z = -8.0f;
    return world;
}

bool _dungeon::updateRoomTransition(vec3& playerPosition, const rect2D& playerBox)
{
    const DungeonRoom* room = findRoom(currentRoomId);
    if (!room)
    {
        return false;
    }

    const float tileX = localTileXForWorld((float)playerPosition.x);
    const float tileY = localTileYForWorld((float)playerPosition.y);

    for (size_t i = 0; i < room->transitions.size(); i++)
    {
        const RoomTransition& transition = room->transitions[i];
        bool crossedBoundary = false;
        bool rangeMatch = false;

        switch (transition.side)
        {
            case NORTH:
                crossedBoundary = playerBox.top > roomTop();
                rangeMatch = isTransitionMatch(tileX, transition.minTile, transition.maxTile);
                break;

            case SOUTH:
                crossedBoundary = playerBox.bottom < roomBottom();
                rangeMatch = isTransitionMatch(tileX, transition.minTile, transition.maxTile);
                break;

            case EAST:
                crossedBoundary = playerBox.right > roomRight();
                rangeMatch = isTransitionMatch(tileY, transition.minTile, transition.maxTile);
                break;

            case WEST:
                crossedBoundary = playerBox.left < roomLeft();
                rangeMatch = isTransitionMatch(tileY, transition.minTile, transition.maxTile);
                break;
        }

        if (!crossedBoundary || !rangeMatch)
        {
            continue;
        }

        if (!enterRoom(transition.targetRoomId, transition.targetTileX, transition.targetTileY))
        {
            return false;
        }

        playerPosition = spawnWorld;
        return true;
    }

    return false;
}

bool _dungeon::collidesWithWall(const rect2D& playerBox) const
{
    const DungeonRoom* room = findRoom(currentRoomId);
    if (!room)
    {
        return false;
    }

    const float tileWorldSize = currentTileWorldSize();
    const int minCol = (int)floor((playerBox.left - roomLeft()) / tileWorldSize);
    const int maxCol = (int)floor(((playerBox.right - roomLeft()) - 0.0001f) / tileWorldSize);
    const int minRow = (int)floor((roomTop() - playerBox.top) / tileWorldSize);
    const int maxRow = (int)floor(((roomTop() - playerBox.bottom) - 0.0001f) / tileWorldSize);

    if (playerBox.left < roomLeft() || playerBox.right > roomRight() ||
        playerBox.bottom < roomBottom() || playerBox.top > roomTop())
    {
        return true;
    }

    for (int row = minRow; row <= maxRow; row++)
    {
        for (int col = minCol; col <= maxCol; col++)
        {
            if (row < 0 || col < 0 || row >= room->heightTiles || col >= room->widthTiles)
            {
                return true;
            }

            if (room->collisionRows[row][col] != '.')
            {
                return true;
            }
        }
    }

    return false;
}

void _dungeon::drawCurrentRoom()
{
    if (!loaded || currentRoomId.empty())
    {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    roomQuad.drawQuad();
    glEnable(GL_LIGHTING);
}

void _dungeon::drawCollisionDebug() const
{
    const DungeonRoom* room = findRoom(currentRoomId);
    if (!loaded || !room)
    {
        return;
    }

    const float tileWorldSize = currentTileWorldSize();
    const float debugZ = roomQuad.pos.z + 0.02f;

    glColor3f(1.0f, 0.2f, 0.2f);
    for (int row = 0; row < room->heightTiles; row++)
    {
        for (int col = 0; col < room->widthTiles; col++)
        {
            if (room->collisionRows[row][col] == '.')
            {
                continue;
            }

            const float left = roomLeft() + (float)col * tileWorldSize;
            const float right = left + tileWorldSize;
            const float top = roomTop() - (float)row * tileWorldSize;
            const float bottom = top - tileWorldSize;

            glBegin(GL_LINE_LOOP);
                glVertex3f(left, bottom, debugZ);
                glVertex3f(right, bottom, debugZ);
                glVertex3f(right, top, debugZ);
                glVertex3f(left, top, debugZ);
            glEnd();
        }
    }

    glColor3f(0.2f, 0.8f, 1.0f);
    for (size_t i = 0; i < room->transitions.size(); i++)
    {
        const RoomTransition& transition = room->transitions[i];

        switch (transition.side)
        {
            case NORTH:
            {
                const float left = roomLeft() + (float)transition.minTile * tileWorldSize;
                const float right = roomLeft() + (float)(transition.maxTile + 1) * tileWorldSize;
                const float y = roomTop();
                glBegin(GL_LINES);
                    glVertex3f(left, y, debugZ);
                    glVertex3f(right, y, debugZ);
                glEnd();
                break;
            }

            case SOUTH:
            {
                const float left = roomLeft() + (float)transition.minTile * tileWorldSize;
                const float right = roomLeft() + (float)(transition.maxTile + 1) * tileWorldSize;
                const float y = roomBottom();
                glBegin(GL_LINES);
                    glVertex3f(left, y, debugZ);
                    glVertex3f(right, y, debugZ);
                glEnd();
                break;
            }

            case EAST:
            {
                const float bottom = roomTop() - (float)(transition.maxTile + 1) * tileWorldSize;
                const float top = roomTop() - (float)transition.minTile * tileWorldSize;
                const float x = roomRight();
                glBegin(GL_LINES);
                    glVertex3f(x, bottom, debugZ);
                    glVertex3f(x, top, debugZ);
                glEnd();
                break;
            }

            case WEST:
            {
                const float bottom = roomTop() - (float)(transition.maxTile + 1) * tileWorldSize;
                const float top = roomTop() - (float)transition.minTile * tileWorldSize;
                const float x = roomLeft();
                glBegin(GL_LINES);
                    glVertex3f(x, bottom, debugZ);
                    glVertex3f(x, top, debugZ);
                glEnd();
                break;
            }
        }
    }
}

bool _dungeon::isLoaded() const
{
    return loaded;
}

vec3 _dungeon::currentSpawnWorld() const
{
    return spawnWorld;
}

float _dungeon::currentPlayerScale() const
{
    return currentTileWorldSize() * 0.67f;
}
