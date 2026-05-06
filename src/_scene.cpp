#include "_scene.h"

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
    dungeonEntranceZone = makeRect(-0.50f, 0.50f, 0.35f, 0.95f);
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

    ply->plyInit(3, 8, "images/SpriteSheet.png");
    ply->scale.x = 0.25f;
    ply->scale.y = 0.25f;
    ply->scale.z = 0.25f;
    respawnPlayer();
    setupCollisionMap();
    setupDungeon();
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
    dungeon1->loadDungeon("images/Dungeons/level1.png", "images/Dungeons/level1_rooms.txt");
}

void _scene::syncPlayerToDungeon()
{
    if (!inDungeon || !dungeon1->isLoaded())
    {
        return;
    }

    const float scale = dungeon1->currentPlayerScale();
    ply->scale.x = scale;
    ply->scale.y = scale;
    ply->scale.z = scale;
}

void _scene::enterDungeon1()
{
    if (!dungeon1->isLoaded())
    {
        return;
    }

    inDungeon = true;
    dungeon1->enterRoom("top_room");
    syncPlayerToDungeon();
    ply->pos = dungeon1->currentSpawnWorld();
    ply->finishAttack();
}

bool _scene::collidesWithWall(vec3 position) const
{
    rect2D playerBox = ply->collisionBoundsAt(position);

    if (inDungeon)
    {
        return dungeon1->collidesWithWall(playerBox);
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
    if (inDungeon)
    {
        return false;
    }

    rect2D playerBox = ply->collisionBoundsAt(position);

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
    if (!showCollisionDebug && !ply->isAttackActive)
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
            dungeon1->drawCollisionDebug();
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
                glVertex3f(dungeonEntranceZone.left, dungeonEntranceZone.bottom, debugZ);
                glVertex3f(dungeonEntranceZone.right, dungeonEntranceZone.bottom, debugZ);
                glVertex3f(dungeonEntranceZone.right, dungeonEntranceZone.top, debugZ);
                glVertex3f(dungeonEntranceZone.left, dungeonEntranceZone.top, debugZ);
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
    }

    if (ply->isAttackActive)
    {
        rect2D attackBox = ply->attackBounds();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.95f, 0.95f, 0.20f, 0.25f);
        glBegin(GL_QUADS);
            glVertex3f(attackBox.left, attackBox.bottom, debugZ);
            glVertex3f(attackBox.right, attackBox.bottom, debugZ);
            glVertex3f(attackBox.right, attackBox.top, debugZ);
            glVertex3f(attackBox.left, attackBox.top, debugZ);
        glEnd();

        glColor3f(1.0f, 0.95f, 0.25f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(attackBox.left, attackBox.bottom, debugZ);
            glVertex3f(attackBox.right, attackBox.bottom, debugZ);
            glVertex3f(attackBox.right, attackBox.top, debugZ);
            glVertex3f(attackBox.left, attackBox.top, debugZ);
        glEnd();

        glDisable(GL_BLEND);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
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
            dungeon1->drawCurrentRoom();
        }

        vec3 previousPlayerPos = ply->pos;
        ply->playerActions(deltaTime);

        if (inDungeon)
        {
            rect2D playerBox = ply->collisionBounds();
            if (dungeon1->updateRoomTransition(ply->pos, playerBox))
            {
                syncPlayerToDungeon();
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

        if (!inDungeon && hit->isRectCollide(ply->collisionBounds(), dungeonEntranceZone))
        {
            enterDungeon1();
        }

        ply->updateQuad();
        //ply->pos.y = -1.4;
        ply->drawPlayer();
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

        myKbMs->wParam = wParam;
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

        if (!ply->isAttackActive)
        {
            switch (wParam)
            {
                case VK_LEFT:
                case VK_RIGHT:
                case VK_UP:
                case VK_DOWN:
                    ply->actionTrigger = ply->STAND;
                    break;
                default:
                    break;
            }
        }
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
