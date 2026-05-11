#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_model.h>
#include<_modelVBO.h>
//#include<_quad.h>
#include<_player.h>

class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        enum DirectionKey{DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_COUNT};

        void keyPressed(_model*);
        void keyPressed(_modelVBO*);
        //void keyPressed(_quad*);
        void keyPressed(_player*);
        void keyUp();
        void handleKeyDown(WPARAM);
        void handleKeyUp(WPARAM);
        void syncPlayerMovement(_player*);
        void resetPlayerInput();

        void mouseEventDown(_model*, double, double);
        void mouseEventUp();
        void mouseMove(_model*, double, double);
        void mouseWheel(_model*, double);

        void mouseEventDown(_modelVBO*, double, double);
        void mouseMove(_modelVBO*, double, double);
        void mouseWheel(_modelVBO*, double);

        double prevMx;
        double prevMy;

        double prevMxVBO;
        double prevMyVBO;

        bool isRotate;
        bool isTranslate;

        WPARAM wParam;

    protected:

    private:
        int directionIndexForKey(WPARAM) const;
        bool directionHeld[DIR_COUNT];
        unsigned long directionOrder[DIR_COUNT];
        unsigned long nextDirectionOrder;
};

#endif // _INPUTS_H
