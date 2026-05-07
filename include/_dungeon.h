#ifndef _DUNGEON_H
#define _DUNGEON_H

#include<_common.h>
#include<_quad.h>

class _dungeon
{
    public:
        _dungeon();
        virtual ~_dungeon();

        virtual bool load() = 0;
        bool enterRoom(const std::string& roomId, float spawnTileX = -1.0f, float spawnTileY = -1.0f);
        bool enterDefaultRoom();
        bool updateRoomTransition(vec3& playerPosition, const rect2D& playerBox);
        bool collidesWithWall(const rect2D& playerBox) const;
        void drawCurrentRoom();
        void drawCollisionDebug() const;
        bool consumePendingOverworldExit();

        bool isLoaded() const;
        vec3 currentSpawnWorld() const;
        float currentPlayerScale() const;
        std::string currentRoomName() const;
        float currentTileWorldSizeValue() const;
        std::vector<vec3> currentWalkableTileCenters() const;
        std::vector<rect2D> currentExitZones() const;
        vec3 currentWorldPositionForTile(float tileX, float tileY) const;
        rect2D currentWorldRectForTiles(int minTileX, int maxTileX, int minTileY, int maxTileY) const;

    protected:
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
            int targetMinTile;
            int targetMaxTile;
        };

        struct RoomExit
        {
            int minTileX;
            int maxTileX;
            int minTileY;
            int maxTileY;
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
            std::vector<RoomExit> exits;
        };

        bool loadDungeon(const char* mapImagePath, const char* collisionMapPath);
        virtual void buildTransitions() = 0;
        virtual const char* defaultRoomId() const = 0;
        void clearTransitions();
        void addOverworldExit(const std::string& roomId,
                              int minTileX,
                              int maxTileX,
                              int minTileY,
                              int maxTileY);
        const DungeonRoom* findRoom(const std::string& roomId) const;
        DungeonRoom* findRoom(const std::string& roomId);
        void addTransition(const std::string& roomId,
                           TransitionSide side,
                           int minTile,
                           int maxTile,
                           const std::string& targetRoomId,
                           int targetMinTile,
                           int targetMaxTile);

    private:
        bool parseCollisionMap(const char* collisionMapPath);
        void syncCurrentRoomTexture();
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
        bool pendingOverworldExit;
};

#endif // _DUNGEON_H
