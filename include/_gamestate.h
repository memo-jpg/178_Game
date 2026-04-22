#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include<_common.h>
#include<_quad.h>

enum GameState {LANDING, MAIN_MENU, HELP_MENU, PLAYING, POPUP_MENU};

class _gameState
{
    public:
        _gameState();
        virtual ~_gameState();

        GameState currentState = LANDING;

        _quad *landingPage = new _quad;
        _quad *mainMenu = new _quad;
        _quad *helpMenu = new _quad;
        _quad *popupMenu = new _quad;

        _quad *btnStart = new _quad;
        _quad *btnHelp = new _quad;
        _quad *btnExitMainMenu = new _quad;
        _quad *btnResume = new _quad;
        _quad *btnExitPopup = new _quad;

        void init();
        void draw();
        void processInput(UINT uMsg, WPARAM wParam, float mouseX, float mouseY, float dimX, float dimY);

    protected:

    private:
};

#endif // _GAMESTATE_H
