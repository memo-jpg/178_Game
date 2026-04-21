#ifndef _PARALLAX_H
#define _PARALLAX_H

#include<_common.h>
#include<_textureLoader.h>

class _parallax
{
    public:
        _parallax();
        virtual ~_parallax();

        void initPrlx(char *);
        void scroll(bool, int, float);
        void drawBackground(float, float);

        _textureLoader *bckGrnd = new _textureLoader();
        enum{LEFT, RIGHT, UP, DOWN};    // can extend depending

        float xMax, xMin, yMax, yMin;   // texture coordinates
        vec3 scale;

    protected:

    private:
};

#endif // _PARALLAX_H
