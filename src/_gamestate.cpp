#include "_gamestate.h"

_gameState::_gameState()
{
    //ctor
}

_gameState::~_gameState()
{
    delete landingPage;
    delete mainMenu;
    delete helpMenu;
    delete popupMenu;

    delete btnStart;
    delete btnHelp;
    delete btnExitMainMenu;
    delete btnResume;
    delete btnExitPopup;
    //dtor
}
void _gameState::init() {
    //load all images
    landingPage->initQuad("images/LandingPage.jpg");
    mainMenu->initQuad("images/MainMenu.jpg");
    helpMenu->initQuad("images/HelpMenu.jpg");
    popupMenu->initQuad("images/PopupMenu.jpg");

    btnStart->initQuad("images/StartButton.jpg");
    btnHelp->initQuad("images/HelpButton.jpg");
    btnExitMainMenu->initQuad("images/ExitButton.jpg");
    btnResume->initQuad("images/ResumeGame.jpg");
    btnExitPopup->initQuad("images/ExitButton.jpg");

    //set variable z depth to prevent flicker
    float zLanding = -5.4f;
    float zMainMenu = -5.2f;
    float zHelpMenu = -5.0f;
    float zPopup = -4.8f;

    //position and scale all elements
    landingPage->pos = {0,0,zLanding};
    landingPage->scale = {1,1,1};
    mainMenu->pos = {0,0,zMainMenu};
    mainMenu->scale = {1,1,1};
    helpMenu->pos = {0,0,zHelpMenu};
    helpMenu->scale = {1,1,1};

    popupMenu->pos = {0,0,zPopup};
    popupMenu->scale = {0.6f, 0.6f, 1.0f};

    btnStart->pos = {-0.3, -0.3f, zMainMenu + 0.1f};
    btnStart->scale = {0.1f, 0.1f, 1.0f};
    btnHelp->pos = {0.0, -0.3f, zMainMenu + 0.1f};
    btnHelp->scale = {0.1f, 0.1f, 1.0f};
    btnExitMainMenu->pos = {0.3, -0.3f, zMainMenu + 0.1f};
    btnExitMainMenu->scale = {0.1f, 0.1f, 1.0f};

    btnResume->pos = {-0.2, 0.0f, zPopup + 0.2f};
    btnResume->scale = {0.1f, 0.1f, 1.0f};
    btnExitPopup->pos = {0.2, 0.0f, zPopup + 0.2f};
    btnExitPopup->scale = {0.1f, 0.1f, 1.0f};
}

void _gameState::draw() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1.0, 1.0, 1.0);

    //On launch, state is autoset to LANDING
    switch(currentState) {
        case LANDING:
            landingPage->drawQuad();
            break;
        case MAIN_MENU:
            mainMenu->drawQuad();
            btnStart->drawQuad();
            btnHelp->drawQuad();
            btnExitMainMenu->drawQuad();
            break;
        case HELP_MENU:
            helpMenu->drawQuad();
            break;
        case PLAYING:
            //Game layer pulled from paralax project, needs to be re-implelemted

            break;
        case POPUP_MENU:
            popupMenu->drawQuad();
            btnResume->drawQuad();
            btnExitPopup->drawQuad();
            break;
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void _gameState::processInput(UINT uMsg, WPARAM wParam, float mouseX, float mouseY, float dimX, float dimY) {
    if (uMsg == WM_KEYDOWN) {
        if (currentState == LANDING) {
            currentState = MAIN_MENU;
        } else if (currentState == HELP_MENU) {
            currentState = MAIN_MENU;
        } else if (currentState == PLAYING && wParam == VK_ESCAPE) {
            currentState = POPUP_MENU;
        } else if (currentState == POPUP_MENU && wParam == VK_ESCAPE) {
            currentState = PLAYING; //Escape autoresumes
        } else if (currentState == MAIN_MENU) {
            if (wParam == 'S') {
                currentState = PLAYING;
            } else if (wParam == 'H') {
                currentState = HELP_MENU;
            } else if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
        }
    } else if (uMsg == WM_LBUTTONDOWN) {
        float uiX = (mouseX / (dimX / 2.0f)) - 1.0f;
        float uiY = 1.0f - (mouseY / (dimY / 2.0f));

        auto isClicked = [&](_quad* btn) {
            float minX = btn->pos.x - btn->scale.x;
            float maxX = btn->pos.x + btn->scale.x;
            float minY = btn->pos.y - btn->scale.y;
            float maxY = btn->pos.y + btn->scale.y;
            return (uiX >= minX && uiX <= maxX && uiY >= minY && uiY <= maxY);
        };

        if (currentState == LANDING) {
            currentState = MAIN_MENU;
        } else if (currentState == HELP_MENU) {
            currentState = MAIN_MENU;
        } else if (currentState == MAIN_MENU) {
            if (isClicked(btnStart)) {
                currentState = PLAYING;
            } else if (isClicked(btnHelp)) {
                currentState = HELP_MENU;
            } else if (isClicked(btnExitMainMenu)) {
                PostQuitMessage(0);
            }
        } else if (currentState == POPUP_MENU) {
            if (isClicked(btnResume)) {
                currentState = PLAYING;
            } else if (isClicked(btnExitPopup)) {
                PostQuitMessage(0);
            }
        }
    } else if (uMsg == WM_RBUTTONDOWN) {
        if (currentState == LANDING || currentState == HELP_MENU) {
            currentState = MAIN_MENU;
        }
    }
}
