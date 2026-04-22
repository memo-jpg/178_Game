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
    respawnPlayer();
    setupCollisionMap();

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

rect2D _scene::playerBoundsAt(vec3 position) const
{
    return makeRect(position.x - playerHalfWidth,
                    position.x + playerHalfWidth,
                    position.y - playerHalfHeight,
                    position.y + playerHalfHeight);
}

bool _scene::collidesWithWall(vec3 position) const
{
    rect2D playerBox = playerBoundsAt(position);

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
    rect2D playerBox = playerBoundsAt(position);

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
    ply->actionTrigger = ply->STAND;
}

void _scene::drawCollisionDebug() const
{
    if (!showCollisionDebug)
    {
        return;
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);

    for (size_t i = 0; i < wallZones.size(); i++)
    {
        const rect2D& zone = wallZones[i];
        glColor3f(1.0f, 0.2f, 0.2f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(zone.left, zone.bottom, -7.6f);
            glVertex3f(zone.right, zone.bottom, -7.6f);
            glVertex3f(zone.right, zone.top, -7.6f);
            glVertex3f(zone.left, zone.top, -7.6f);
        glEnd();
    }

    for (size_t i = 0; i < pitZones.size(); i++)
    {
        const rect2D& zone = pitZones[i];
        glColor3f(1.0f, 0.9f, 0.1f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(zone.left, zone.bottom, -7.6f);
            glVertex3f(zone.right, zone.bottom, -7.6f);
            glVertex3f(zone.right, zone.top, -7.6f);
            glVertex3f(zone.left, zone.top, -7.6f);
        glEnd();
    }

    rect2D playerBox = playerBoundsAt(ply->pos);
    glColor3f(0.2f, 1.0f, 0.2f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(playerBox.left, playerBox.bottom, -7.5f);
        glVertex3f(playerBox.right, playerBox.bottom, -7.5f);
        glVertex3f(playerBox.right, playerBox.top, -7.5f);
        glVertex3f(playerBox.left, playerBox.top, -7.5f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void _scene::drawScene()
{

    auto currenTime = chrono::steady_clock::now();

    chrono::duration<float> elapsed = currenTime - lastTime;
    _scene::deltaTime = elapsed.count();

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

    glPushMatrix();
    glScalef(13.3, 13.3, 1);
    myPrlx->drawBackground(dim.x, dim.y);
    //myPrlx->scroll(true, myPrlx->UP, 0.00005);
    //myPrlx->scroll(true, myPrlx->RIGHT, 0.05 * deltaTime);
    glPopMatrix();

    vec3 previousPlayerPos = ply->pos;
    ply->playerActions(deltaTime);

    if (collidesWithWall(ply->pos))
    {
        ply->pos = previousPlayerPos;
    }

    if (fallsIntoPit(ply->pos))
    {
        respawnPlayer();
    }

    ply->updateQuad();
    ply->scale.x = 0.25;
    ply->scale.y = 0.25;
    ply->scale.z = 0.25;
    //ply->pos.y = -1.4;
    ply->drawQuad();
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
    switch(uMsg){
    case WM_KEYDOWN:
        if (wParam == VK_F2)
        {
            showCollisionDebug = !showCollisionDebug;
            return 0;
        }

        myKbMs->wParam = wParam;
        myKbMs->keyPressed(Mymodel);
        //cout << "came here "<< endl;
        myKbMs->keyPressed(myVBO);
        //myKbMs->keyPressed(myQuad);
        myKbMs->keyPressed(ply);
        break;

    case WM_KEYUP:
        ply->actionTrigger = ply->STAND;
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

    return 0;
}
