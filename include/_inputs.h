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

        void keyPressed(_model*);
        void keyPressed(_modelVBO*);
        //void keyPressed(_quad*);
        void keyPressed(_player*);
        void keyUp();

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
};

#endif // _INPUTS_H
