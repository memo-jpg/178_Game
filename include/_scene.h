#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>
#include<_lightSettings.h>
#include<_model.h>
#include<_inputs.h>
#include<_modelVBO.h>
#include<_parallax.h>
#include<_quad.h>
#include<_player.h>
#include<_sounds.h>
#include<_enemies.h>
#include<_collisionCheck.h>
#include<_bullets.h>
#include<_particles.h>
#include<_shader.h>
#include<_gamestate.h>
#include<_fonts.h>
#include<_overworld.h>
#include<_dungeon1.h>
#include<_dungeon2.h>
#include<_dungeon3.h>

class _scene: public _enemyNavigation
{
    public:
        _scene();
        virtual ~_scene();

        GLint initGL(); // initialize the openGL contents
        void resize(GLint, GLint); // to handle resize window
        void drawScene(); // render the final scene

        int winMsg(HWND	hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        void mouseMapping(int, int);
        void setupOverworld();
        void setupDungeon();
        void enterDungeon1();
        void enterDungeon2();
        void enterDungeon3();
        void enterDungeon(_dungeon* dungeon);
        void exitDungeon();
        void syncPlayerToDungeon();
        void syncPlayerToOverworld();
        bool collidesWithWall(vec3) const;
        bool fallsIntoPit(vec3) const;
        void respawnPlayer();
        void drawCollisionDebug() const;
        bool isEnemyPositionWalkable(const _enemies*, vec3) const;
        bool doesRectHitPlayer(rect2D) const;
        bool tryDamagePlayer(rect2D) const;
        bool doesRectHitWall(rect2D) const;
        static float deltaTime;
        static const int MAX_OVERWORLD_ENEMIES = 20;
        //auto lastTime = chrono::steady_clock::now();

        _lightSettings *myLight = new _lightSettings();
        _inputs *myKbMs = new _inputs();
        _model *Mymodel = new _model();
        _modelVBO *myVBO = new _modelVBO();
        _parallax *myPrlx = new _parallax();
        _quad *myQuad = new _quad();
        _player *ply = new _player();
        _sounds *snds = new _sounds();
        _sounds *sfx = new _sounds();
        _collisionCheck *hit = new _collisionCheck();
        _gameState *stateManager = new _gameState;
        _fonts *hudFont = new _fonts();
        _fonts *gameOverFont = new _fonts();
        _dungeon *overworld = new _overworld();
        _dungeon *dungeon1 = new _dungeon1();
        _dungeon *dungeon2 = new _dungeon2();
        _dungeon *dungeon3 = new _dungeon3();
        _dungeon *activeDungeon = NULL;

        _textureLoader* swordTex = new _textureLoader();
        _quad *dungeonExitTriggerSprite = new _quad();



        vec3 mouse; // to keep track of the mouse location
        vec2 dim;   // window size

        int clickCount = 0; // temp variable to count mouse clicks used in bullets
        std::vector<rect2D> wallZones;
        std::vector<rect2D> pitZones;
        vec3 playerSpawn;
        bool inDungeon = false;
        bool showCollisionDebug = false;
        bool isGameOver = false;
        float gameOverTimer = 0.0f;
        float gameOverRestartDelay = 2.0f;

    protected:

    private:
        struct HealthPickup
        {
            vec3 pos;
            float halfSize;
            bool active;
        };

        _enemies* overworldEnemies[MAX_OVERWORLD_ENEMIES];
        int overworldEnemyCount;
        _enemies* dungeonRoomEnemies[MAX_OVERWORLD_ENEMIES];
        int dungeonRoomEnemyCount;
        std::vector<HealthPickup> healthPickups;

        void initOverworldEnemies();
        void clearEnemyGroup(_enemies*[], int&);
        void clearHealthPickups();
        void maybeSpawnHealthPickup(vec3, float);
        void updateHealthPickups();
        void drawHealthPickups() const;
        void spawnDungeonRoomEnemies();
        void drawEnemyGroup(_enemies*[], int) const;
        void updateAndDrawEnemyGroup(_enemies*[], int&);
        void enterOverworldRoomForDungeon(const _dungeon*);
        void drawExitTriggerSprite(rect2D, float, float) const;
        void drawActiveDungeonReturnTriggers() const;
        bool isDungeonBossRoomActive() const;
        bool isDungeonBossRoomCleared() const;
        rect2D dungeonBossReturnTriggerBounds() const;
        void drawDungeonBossReturnTrigger() const;
        rect2D currentOverworldDungeonEntranceZone() const;
        bool collidesWithWall(rect2D) const;
        bool fallsIntoPit(rect2D) const;
        float currentPlayerTileSize() const;
        vec3 playerKnockbackTarget(rect2D) const;
        void drawHud() const;
        void drawGameOverScreen() const;
        void startGameOver();
        void resetGameplaySession();
};

#endif // _SCENE_H
