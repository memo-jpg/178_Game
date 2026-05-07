#include "_scene.h"
#include "_enemyMoblin.h"
#include "_enemyOck.h"

int loc;

auto lastTime = chrono::steady_clock::now();

namespace
{
    rect2D makeRect(float left, float right, float bottom, float top)
    {
        rect2D rect;
        rect.left = left;
        rect.right = right;
        rect.bottom = bottom;
        rect.top = top;
        return rect;
    }

    std::mt19937& dungeonEnemyRng()
    {
        static std::mt19937 generator((unsigned int)std::chrono::steady_clock::now().time_since_epoch().count());
        return generator;
    }
}

_scene::_scene()
{
    //ctor

    mouse.x = 0;
    mouse.y = 0;
    mouse.z = -6;

    playerSpawn.x = 0;
    playerSpawn.y = -0.8f;
    playerSpawn.z = -8.0f;

    overworldSpawn = playerSpawn;
    dungeon1EntranceZone = makeRect(-0.50f, 0.50f, 0.35f, 0.95f);
    dungeon2EntranceZone = makeRect(-2.50f, -1.50f, 0.35f, 0.95f);
    dungeon3EntranceZone = makeRect(1.50f, 2.50f, 0.35f, 0.95f);
    overworldEnemyCount = 0;
    dungeonRoomEnemyCount = 0;

    for (int i = 0; i < MAX_OVERWORLD_ENEMIES; i++)
    {
        overworldEnemies[i] = NULL;
        dungeonRoomEnemies[i] = NULL;
    }
}

_scene::~_scene()
{
    //dtor
    delete myLight;
    delete myKbMs;
    delete Mymodel;
    delete myVBO;
    delete myPrlx;
    delete myQuad;
    delete ply;
    delete snds;
    delete hit;
    delete dungeon1;
    delete dungeon2;
    delete dungeon3;
    delete swordTex;

    clearEnemyGroup(overworldEnemies, overworldEnemyCount);
    clearEnemyGroup(dungeonRoomEnemies, dungeonRoomEnemyCount);
}

GLint _scene::initGL()
{
    glewInit();
    glClearColor(0.0,0.0,0.0,1.0);  // background color
    glClearDepth(1.0);              // depth test value
    glEnable(GL_DEPTH_TEST);        // enable depth test
    glDepthFunc(GL_LEQUAL);         // true for less or equal

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // glEnable(GL_COLOR_MATERIAL);

    myLight->setLight(GL_LIGHT0);
    //Mymodel->initModel("images/crate.png");
    //myVBO->modelInit("images/crate.png");

    myPrlx->initPrlx("images/prlx2.jpg");

    //myQuad->initQuad("images/crate.png");

    ply->plyInit(4, 4, "images/BlueLink.png");
    ply->scale.x = 0.25f;
    ply->scale.y = 0.25f;
    ply->scale.z = 0.25f;
    swordTex->loadTexture("images/Sword.png");
    respawnPlayer();
    setupCollisionMap();
    setupDungeon();
    initOverworldEnemies();
    stateManager->init();   //init game state


    //snds->initSound();
    //sds->playMusic("sounds/BackOnTrack.mp3");

    return true;
}

void _scene::resize(GLint width, GLint height)
{
    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;   // keep track of window resize
    glViewport(0,0,width,height);   // setting the viewport

    glMatrixMode(GL_PROJECTION);    // initiate the projection
    glLoadIdentity();               // initialize the matrix with identity matrix
    gluPerspective(45.0,aspectRatio,0.1,100.0); // setup perspective projection

    glMatrixMode(GL_MODELVIEW);     // initiate model & view matrix
    glLoadIdentity();

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

}

float _scene::deltaTime = 0;        // initializing static variable

void _scene::setupCollisionMap()
{
    wallZones.clear();
    pitZones.clear();

    // World bounds for the current room
    wallZones.push_back(makeRect(-4.25f, -3.60f, -2.20f, 2.40f));   // left
    wallZones.push_back(makeRect(2.60f, 3.25f, -2.20f, 2.40f));     // right
    wallZones.push_back(makeRect(-4.25f, 3.25f, 1.70f, 2.40f));     // top
    wallZones.push_back(makeRect(-4.25f, 3.25f, -2.20f, -1.95f));   // down

    // Sample solid objects
    // Adjust for final map art
    wallZones.push_back(makeRect(-4.90f, -3.55f, -1.55f, -0.70f));
    wallZones.push_back(makeRect(-3.15f, -1.85f, -1.55f, -0.70f));
    wallZones.push_back(makeRect(2.80f, 4.10f, -1.55f, -0.70f));
    wallZones.push_back(makeRect(6.10f, 7.05f, -1.55f, -0.70f));

    // Pits/hazards
    pitZones.push_back(makeRect(-0.75f, 0.75f, -1.60f, -1.10f));
}

void _scene::setupDungeon()
{
    dungeon1->load();
    dungeon2->load();
    dungeon3->load();
}

void _scene::initOverworldEnemies()
{
    clearEnemyGroup(overworldEnemies, overworldEnemyCount);

    if (overworldEnemyCount >= MAX_OVERWORLD_ENEMIES)
    {
        return;
    }

    _enemyOck* testEnemy = new _enemyOck();
    testEnemy->initOck();

    overworldEnemies[overworldEnemyCount] = testEnemy;
    overworldEnemyCount++;

    if (overworldEnemyCount >= MAX_OVERWORLD_ENEMIES)
    {
        return;
    }

    _enemyMoblin* testMoblin = new _enemyMoblin();
    testMoblin->initMoblin();

    overworldEnemies[overworldEnemyCount] = testMoblin;
    overworldEnemyCount++;
}

void _scene::clearEnemyGroup(_enemies* enemies[], int& enemyCount)
{
    for (int i = 0; i < MAX_OVERWORLD_ENEMIES; i++)
    {
        delete enemies[i];
        enemies[i] = NULL;
    }

    enemyCount = 0;
}

void _scene::spawnDungeonRoomEnemies()
{
    clearEnemyGroup(dungeonRoomEnemies, dungeonRoomEnemyCount);

    if (!inDungeon || activeDungeon == NULL)
    {
        return;
    }

    const std::string roomName = activeDungeon->currentRoomName();
    const bool isDungeon1Room = activeDungeon == dungeon1;
    const bool isDungeon2Room = activeDungeon == dungeon2;

    if (!isDungeon1Room && !isDungeon2Room)
    {
        return;
    }

    if ((isDungeon1Room && (roomName == "top_room" || roomName == "bottom_room")) ||
        (isDungeon2Room && (roomName == "left_room" || roomName == "boss_room")))
    {
        return;
    }

    std::vector<vec3> candidates = activeDungeon->currentWalkableTileCenters();
    if (candidates.empty())
    {
        return;
    }

    const float tileSize = activeDungeon->currentTileWorldSizeValue();
    const vec3 playerRoomSpawn = activeDungeon->currentSpawnWorld();
    std::vector<vec3> filteredCandidates;

    for (size_t i = 0; i < candidates.size(); i++)
    {
        const vec3& candidate = candidates[i];
        const float deltaX = (float)candidate.x - (float)playerRoomSpawn.x;
        const float deltaY = (float)candidate.y - (float)playerRoomSpawn.y;
        const float distanceSquared = (deltaX * deltaX) + (deltaY * deltaY);
        const float minimumDistance = tileSize * 3.0f;

        if (distanceSquared < minimumDistance * minimumDistance)
        {
            continue;
        }

        filteredCandidates.push_back(candidate);
    }

    if (!filteredCandidates.empty())
    {
        candidates = filteredCandidates;
    }

    std::shuffle(candidates.begin(), candidates.end(), dungeonEnemyRng());

    std::uniform_int_distribution<int> spawnCountDistribution(1, 4);
    const int desiredSpawnCount = spawnCountDistribution(dungeonEnemyRng());
    const int spawnCount = std::min(desiredSpawnCount, (int)candidates.size());

    for (int i = 0; i < spawnCount && dungeonRoomEnemyCount < MAX_OVERWORLD_ENEMIES; i++)
    {
        _enemies* roomEnemy = NULL;

        if (isDungeon1Room)
        {
            _enemyOck* ock = new _enemyOck();
            ock->initOck();
            roomEnemy = ock;
        }
        else if (isDungeon2Room)
        {
            _enemyMoblin* moblin = new _enemyMoblin();
            moblin->initMoblin();
            roomEnemy = moblin;
        }

        if (roomEnemy == NULL)
        {
            continue;
        }

        roomEnemy->pos = candidates[i];
        roomEnemy->moveStartPos = roomEnemy->pos;
        roomEnemy->moveTargetPos = roomEnemy->pos;

        dungeonRoomEnemies[dungeonRoomEnemyCount] = roomEnemy;
        dungeonRoomEnemyCount++;
    }
}

void _scene::syncPlayerToDungeon()
{
    if (!inDungeon || activeDungeon == NULL || !activeDungeon->isLoaded())
    {
        return;
    }

    const float scale = activeDungeon->currentPlayerScale();
    ply->scale.x = scale;
    ply->scale.y = scale;
    ply->scale.z = scale;
}

void _scene::enterDungeon1()
{
    enterDungeon(dungeon1);
}

void _scene::enterDungeon2()
{
    enterDungeon(dungeon2);
}

void _scene::enterDungeon3()
{
    enterDungeon(dungeon3);
}

void _scene::enterDungeon(_dungeon* dungeon)
{
    if (!dungeon || !dungeon->isLoaded())
    {
        return;
    }

    if (!dungeon->enterDefaultRoom())
    {
        return;
    }

    activeDungeon = dungeon;
    inDungeon = true;
    syncPlayerToDungeon();
    ply->pos = activeDungeon->currentSpawnWorld();
    ply->finishAttack();
    spawnDungeonRoomEnemies();
}

vec3 _scene::overworldSpawnForDungeon(const _dungeon* dungeon) const
{
    vec3 spawn = overworldSpawn;
    spawn.z = -8.0f;

    if (dungeon == dungeon1)
    {
        spawn.x = 0.0f;
        spawn.y = 0.10f;
    }
    else if (dungeon == dungeon2)
    {
        spawn.x = -2.0f;
        spawn.y = 0.10f;
    }
    else if (dungeon == dungeon3)
    {
        spawn.x = 2.0f;
        spawn.y = 0.10f;
    }

    return spawn;
}

bool _scene::isEnemyPositionWalkable(const _enemies* enemy, vec3 position) const
{
    if (enemy == NULL)
    {
        return false;
    }

    const rect2D enemyBox = enemy->collisionBoundsAt(position);
    return !collidesWithWall(enemyBox) && !fallsIntoPit(enemyBox);
}

void _scene::exitDungeon()
{
    if (activeDungeon == NULL)
    {
        return;
    }

    vec3 spawn = overworldSpawnForDungeon(activeDungeon);
    inDungeon = false;
    activeDungeon = NULL;
    ply->pos = spawn;
    ply->finishAttack();
    ply->scale.x = 0.25f;
    ply->scale.y = 0.25f;
    ply->scale.z = 0.25f;
    clearEnemyGroup(dungeonRoomEnemies, dungeonRoomEnemyCount);
}

bool _scene::collidesWithWall(vec3 position) const
{
    rect2D playerBox = ply->collisionBoundsAt(position);
    return collidesWithWall(playerBox);
}

bool _scene::collidesWithWall(rect2D playerBox) const
{
    if (inDungeon)
    {
        return activeDungeon != NULL ? activeDungeon->collidesWithWall(playerBox) : false;
    }

    for (size_t i = 0; i < wallZones.size(); i++)
    {
        if (hit->isRectCollide(playerBox, wallZones[i]))
        {
            return true;
        }
    }

    return false;
}

bool _scene::fallsIntoPit(vec3 position) const
{
    rect2D playerBox = ply->collisionBoundsAt(position);
    return fallsIntoPit(playerBox);
}

bool _scene::fallsIntoPit(rect2D playerBox) const
{
    if (inDungeon)
    {
        return false;
    }

    for (size_t i = 0; i < pitZones.size(); i++)
    {
        if (hit->isRectCollide(playerBox, pitZones[i]))
        {
            return true;
        }
    }

    return false;
}

void _scene::respawnPlayer()
{
    ply->pos = playerSpawn;
    ply->finishAttack();

    if (!inDungeon)
    {
        ply->scale.x = 0.25f;
        ply->scale.y = 0.25f;
        ply->scale.z = 0.25f;
    }
}

// DRAW COLLISION BOXES
void _scene::drawCollisionDebug() const
{
    if (!showCollisionDebug)
    {
        return;
    }

    const float debugZ = (float)ply->pos.z + 0.01f;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);

    if (showCollisionDebug)
    {
        if (inDungeon)
        {
            if (activeDungeon != NULL)
            {
                activeDungeon->drawCollisionDebug();
            }
        }
        else
        {
            for (size_t i = 0; i < wallZones.size(); i++)
            {
                const rect2D& zone = wallZones[i];
                glColor3f(1.0f, 0.2f, 0.2f);
                glBegin(GL_LINE_LOOP);
                    glVertex3f(zone.left, zone.bottom, debugZ);
                    glVertex3f(zone.right, zone.bottom, debugZ);
                    glVertex3f(zone.right, zone.top, debugZ);
                    glVertex3f(zone.left, zone.top, debugZ);
                glEnd();
            }

            for (size_t i = 0; i < pitZones.size(); i++)
            {
                const rect2D& zone = pitZones[i];
                glColor3f(1.0f, 0.9f, 0.1f);
                glBegin(GL_LINE_LOOP);
                    glVertex3f(zone.left, zone.bottom, debugZ);
                    glVertex3f(zone.right, zone.bottom, debugZ);
                    glVertex3f(zone.right, zone.top, debugZ);
                    glVertex3f(zone.left, zone.top, debugZ);
                glEnd();
            }

            glColor3f(0.2f, 0.8f, 1.0f);
            glBegin(GL_LINE_LOOP);
                glVertex3f(dungeon1EntranceZone.left, dungeon1EntranceZone.bottom, debugZ);
                glVertex3f(dungeon1EntranceZone.right, dungeon1EntranceZone.bottom, debugZ);
                glVertex3f(dungeon1EntranceZone.right, dungeon1EntranceZone.top, debugZ);
                glVertex3f(dungeon1EntranceZone.left, dungeon1EntranceZone.top, debugZ);
            glEnd();

            glColor3f(0.3f, 1.0f, 0.4f);
            glBegin(GL_LINE_LOOP);
                glVertex3f(dungeon2EntranceZone.left, dungeon2EntranceZone.bottom, debugZ);
                glVertex3f(dungeon2EntranceZone.right, dungeon2EntranceZone.bottom, debugZ);
                glVertex3f(dungeon2EntranceZone.right, dungeon2EntranceZone.top, debugZ);
                glVertex3f(dungeon2EntranceZone.left, dungeon2EntranceZone.top, debugZ);
            glEnd();

            glColor3f(1.0f, 0.6f, 0.2f);
            glBegin(GL_LINE_LOOP);
                glVertex3f(dungeon3EntranceZone.left, dungeon3EntranceZone.bottom, debugZ);
                glVertex3f(dungeon3EntranceZone.right, dungeon3EntranceZone.bottom, debugZ);
                glVertex3f(dungeon3EntranceZone.right, dungeon3EntranceZone.top, debugZ);
                glVertex3f(dungeon3EntranceZone.left, dungeon3EntranceZone.top, debugZ);
            glEnd();
        }

        rect2D playerBox = ply->collisionBounds();
        glColor3f(0.2f, 1.0f, 0.2f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(playerBox.left, playerBox.bottom, debugZ);
            glVertex3f(playerBox.right, playerBox.bottom, debugZ);
            glVertex3f(playerBox.right, playerBox.top, debugZ);
            glVertex3f(playerBox.left, playerBox.top, debugZ);
        glEnd();

        if (!inDungeon)
        {
            for (int i = 0; i < overworldEnemyCount; i++)
            {
                if (overworldEnemies[i] == NULL || !overworldEnemies[i]->isEnmsLive)
                {
                    continue;
                }

                rect2D enemyBox = overworldEnemies[i]->collisionBounds();
                glColor3f(1.0f, 0.15f, 0.85f);
                glBegin(GL_LINE_LOOP);
                    glVertex3f(enemyBox.left, enemyBox.bottom, debugZ);
                    glVertex3f(enemyBox.right, enemyBox.bottom, debugZ);
                    glVertex3f(enemyBox.right, enemyBox.top, debugZ);
                    glVertex3f(enemyBox.left, enemyBox.top, debugZ);
                glEnd();
            }
        }
    }

    if (ply->isAttackActive)
    {
        quad2D attackQuad = ply->attackQuad();
        rect2D attackBox = ply->attackBounds();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.95f, 0.95f, 0.20f, 0.25f);
        glBegin(GL_QUADS);
            glVertex3f(attackQuad.points[0].x, attackQuad.points[0].y, debugZ);
            glVertex3f(attackQuad.points[1].x, attackQuad.points[1].y, debugZ);
            glVertex3f(attackQuad.points[2].x, attackQuad.points[2].y, debugZ);
            glVertex3f(attackQuad.points[3].x, attackQuad.points[3].y, debugZ);
        glEnd();

        glColor3f(1.0f, 0.95f, 0.25f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(attackQuad.points[0].x, attackQuad.points[0].y, debugZ);
            glVertex3f(attackQuad.points[1].x, attackQuad.points[1].y, debugZ);
            glVertex3f(attackQuad.points[2].x, attackQuad.points[2].y, debugZ);
            glVertex3f(attackQuad.points[3].x, attackQuad.points[3].y, debugZ);
        glEnd();

        if (showCollisionDebug)
        {
        glColor3f(1.0f, 0.45f, 0.10f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(attackBox.left, attackBox.bottom, debugZ);
            glVertex3f(attackBox.right, attackBox.bottom, debugZ);
            glVertex3f(attackBox.right, attackBox.top, debugZ);
            glVertex3f(attackBox.left, attackBox.top, debugZ);
        glEnd();
    }

        glDisable(GL_BLEND);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void _scene::updateAndDrawEnemyGroup(_enemies* enemies[], int& enemyCount)
{
    const bool playerAttackActive = ply->isAttackActive;
    const int playerAttackId = ply->currentAttackId();
    const rect2D playerAttackBox = playerAttackActive ? ply->attackBounds() : rect2D();

    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i] == NULL || !enemies[i]->isEnmsLive)
        {
            continue;
        }

        vec3 previousEnemyPos = enemies[i]->pos;
        enemies[i]->enmsActions(deltaTime, &ply->pos, this);

        if (enemies[i]->startedMoveThisFrame &&
            (collidesWithWall(enemies[i]->collisionBoundsAt(enemies[i]->moveTargetPos)) ||
             fallsIntoPit(enemies[i]->collisionBoundsAt(enemies[i]->moveTargetPos))))
        {
            enemies[i]->cancelMove();
        }
        else if (collidesWithWall(enemies[i]->collisionBounds()) ||
                 fallsIntoPit(enemies[i]->collisionBounds()))
        {
            enemies[i]->pos = previousEnemyPos;
            enemies[i]->cancelMove();
        }

        if (playerAttackActive &&
            hit->isRectCollide(playerAttackBox, enemies[i]->collisionBounds()))
        {
            enemies[i]->takeDamage(1, playerAttackId, (_enemies::FacingDirection)ply->facingDirection);
        }

        if (!enemies[i]->isEnmsLive)
        {
            continue;
        }

        enemies[i]->drawEnms();
    }
}

void _scene::drawScene()
{

    auto currenTime = chrono::steady_clock::now();

    chrono::duration<float> elapsed = currenTime - lastTime;
    if (stateManager->currentState == PLAYING) {
        _scene::deltaTime = elapsed.count();
    } else {
        _scene::deltaTime = 0.0f;
    }   //Modified timer to enable pause feature
    lastTime = currenTime;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);   // Clear buffers
    glLoadIdentity();
    glColor3f(0.232,0.193,0.0); // set color for my model

    //glTranslatef(0.0,0.0,-8.0); // move model to the center and back
    //glutSolidTeapot(1.5);   // draw teapot

    //myQuad->updateQuad();
    //myQuad->drawQuad();

    //Mymodel->drawModel();
    //myVBO->drawmodel();


    if (stateManager->currentState == PLAYING || stateManager->currentState == POPUP_MENU) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glColor3f(1.0,0,1.0);

        //Game objects now go here
        if (!inDungeon)
        {
            glPushMatrix();
            glScalef(13.3, 13.3, 1);
            myPrlx->drawBackground(dim.x, dim.y);
            glPopMatrix();
        }
        else
        {
            if (activeDungeon != NULL)
            {
                activeDungeon->drawCurrentRoom();
            }
        }

        if (stateManager->currentState == PLAYING)
        {
            myKbMs->syncPlayerMovement(ply);
        }

        vec3 previousPlayerPos = ply->pos;
        ply->playerActions(deltaTime);

        if (inDungeon)
        {
            rect2D playerBox = ply->collisionBounds();
            if (activeDungeon != NULL && activeDungeon->updateRoomTransition(ply->pos, playerBox))
            {
                if (activeDungeon->consumePendingOverworldExit())
                {
                    exitDungeon();
                }
                else
                {
                    syncPlayerToDungeon();
                    spawnDungeonRoomEnemies();
                }
            }
            else if (collidesWithWall(ply->pos))
            {
                ply->pos = previousPlayerPos;
            }
        }
        else if (collidesWithWall(ply->pos))
        {
            ply->pos = previousPlayerPos;
        }

        if (!inDungeon && fallsIntoPit(ply->pos))
        {
            respawnPlayer();
        }

        if (!inDungeon && hit->isRectCollide(ply->collisionBounds(), dungeon1EntranceZone))
        {
            enterDungeon1();
        }
        else if (!inDungeon && hit->isRectCollide(ply->collisionBounds(), dungeon2EntranceZone))
        {
            enterDungeon2();
        }
        else if (!inDungeon && hit->isRectCollide(ply->collisionBounds(), dungeon3EntranceZone))
        {
            enterDungeon3();
        }

        if (!inDungeon)
        {
            updateAndDrawEnemyGroup(overworldEnemies, overworldEnemyCount);
        }
        else if (activeDungeon == dungeon1 || activeDungeon == dungeon2)
        {
            updateAndDrawEnemyGroup(dungeonRoomEnemies, dungeonRoomEnemyCount);
        }

        ply->updateQuad();
        //ply->pos.y = -1.4;
        ply->drawPlayer();

        if (ply->isAttackActive)
        {
            const float swordZ = (float)ply->pos.z + 0.005f;
            quad2D attackQuad = ply->attackQuad();

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_TEXTURE_2D);
            swordTex->bindTexture();
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(attackQuad.points[0].x, attackQuad.points[0].y, swordZ);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(attackQuad.points[1].x, attackQuad.points[1].y, swordZ);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(attackQuad.points[2].x, attackQuad.points[2].y, swordZ);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(attackQuad.points[3].x, attackQuad.points[3].y, swordZ);
            glEnd();

            glDisable(GL_BLEND);
        }

        drawCollisionDebug();


        glPushMatrix();

        glDisable(GL_TEXTURE_2D);
        //glTranslatef(0, 0, -6);
        glTranslatef(mouse.x, mouse.y, mouse.z);
        //glutSolidTeapot(0.2);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);

        glPopMatrix();

    }

    stateManager->draw();







}

void _scene::mouseMapping(int x, int y)
{
    GLint viewPort[4];         // for the window
    GLdouble modelViewM[16];    // model and camera
    GLdouble projectionM[16];   // for the projection
    GLfloat winX, winY, winZ;   // mouse clicks

    glGetDoublev(GL_MODELVIEW_MATRIX, modelViewM);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionM);
    glGetIntegerv(GL_VIEWPORT, viewPort);

    winX = (GLfloat)x;
    winY = (GLfloat)viewPort[3] - y;

    glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelViewM, projectionM, viewPort, &mouse.x, &mouse.y, &mouse.z);
}


int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    float mouseX = LOWORD(lParam);
    float mouseY = HIWORD(lParam);
    switch(uMsg){
    case WM_KEYDOWN:
        if (wParam == VK_F2)
        {
            showCollisionDebug = !showCollisionDebug;
            return 0;
        }

        if (stateManager->currentState != PLAYING)
        {
            break;
        }

        myKbMs->handleKeyDown(wParam);
        myKbMs->keyPressed(Mymodel);
        //cout << "came here "<< endl;
        myKbMs->keyPressed(myVBO);
        //myKbMs->keyPressed(myQuad);
        myKbMs->keyPressed(ply);
        break;

    case WM_KEYUP:
        if (stateManager->currentState != PLAYING)
        {
            break;
        }

        myKbMs->handleKeyUp(wParam);
        myKbMs->syncPlayerMovement(ply);
        break;

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        myKbMs->wParam = wParam;
        myKbMs->mouseEventDown(Mymodel, LOWORD(lParam), HIWORD(lParam));
        //myKbMs->mouseEventDown(myVBO, LOWORD(lParam), HIWORD(lParam));
        mouseMapping(LOWORD(lParam), HIWORD(lParam));

        clickCount++;
        clickCount = (clickCount)%20;
        break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:

        //snds->playSounds("sounds/Gunfire.mp3");

        myKbMs ->mouseEventUp();
        break;

    case WM_MBUTTONDOWN:
        break;

    case WM_MOUSEMOVE:
        myKbMs->mouseMove(Mymodel, LOWORD(lParam), HIWORD(lParam));
        myKbMs->mouseMove(myVBO, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_MOUSEWHEEL:
        myKbMs->mouseWheel(Mymodel, (double)GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    }
    stateManager->processInput(uMsg, wParam, mouseX, mouseY, dim.x, dim.y);    //State manager inputs


    return 0;
}
