#include "_scene.h"

int loc;

auto lastTime = chrono::steady_clock::now();

_scene::_scene()
{
    //ctor

    mouse.x = 0;
    mouse.y = 0;
    mouse.z = -6;
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

    ply->plyInit(8, 4, "images/ply.png");

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

    ply->playerActions(deltaTime);
    ply->updateQuad();
    ply->scale.x = 0.5;
    ply->scale.y = 0.5;
    ply->scale.z = 0.5;
    //ply->pos.y = -1.4;
    ply->drawQuad();


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
}
