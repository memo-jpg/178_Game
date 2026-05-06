#ifndef _DUNGEON_H
#define _DUNGEON_H

#include<_common.h>
#include<_quad.h>

class _dungeon
{
    public:
        _dungeon();
        virtual ~_dungeon();

        bool loadDungeon(const char* mapImagePath, const char* collisionMapPath);
        bool enterRoom(const std::string& roomId, float spawnTileX = -1.0f, float spawnTileY = -1.0f);
        bool updateRoomTransition(vec3& playerPosition, const rect2D& playerBox);
        bool collidesWithWall(const rect2D& playerBox) const;
        void drawCurrentRoom();
        void drawCollisionDebug() const;

        bool isLoaded() const;
        vec3 currentSpawnWorld() const;
        float currentPlayerScale() const;

    private:
        enum TransitionSide
        {
            NORTH,
            EAST,
            SOUTH,
            WEST
        };

        struct RoomTransition
        {
            TransitionSide side;
            int minTile;
            int maxTile;
            std::string targetRoomId;
            float targetTileX;
            float targetTileY;
        };

        struct DungeonRoom
        {
            std::string id;
            int sourceTileX;
            int sourceTileY;
            int widthTiles;
            int heightTiles;
            float defaultSpawnX;
            float defaultSpawnY;
            std::vector<std::string> collisionRows;
            std::vector<RoomTransition> transitions;
        };

        bool parseCollisionMap(const char* collisionMapPath);
        void buildDungeon1Transitions();
        void syncCurrentRoomTexture();
        const DungeonRoom* findRoom(const std::string& roomId) const;
        DungeonRoom* findRoom(const std::string& roomId);
        void addTransition(const std::string& roomId,
                           TransitionSide side,
                           int minTile,
                           int maxTile,
                           const std::string& targetRoomId,
                           float targetTileX,
                           float targetTileY);
        float currentTileWorldSize() const;
        float roomWorldWidth() const;
        float roomWorldHeight() const;
        float roomLeft() const;
        float roomRight() const;
        float roomTop() const;
        float roomBottom() const;
        float localTileXForWorld(float worldX) const;
        float localTileYForWorld(float worldY) const;
        vec3 worldPositionForTile(float tileX, float tileY) const;

        _quad roomQuad;
        std::vector<DungeonRoom> rooms;
        std::string currentRoomId;
        vec3 spawnWorld;
        int sourceTileSize;
        float maxRoomWorldWidth;
        float maxRoomWorldHeight;
        float minTileWorldSize;
        float maxTileWorldSize;
        bool loaded;
};

#endif // _DUNGEON_H
