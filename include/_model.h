#ifndef _MODEL_H
#define _MODEL_H

#include<_common.h>
#include<_textureLoader.h>

class _model
{
    public:
        _model();
        virtual ~_model();

        vec3 rotation;
        vec3 pos;
        vec3 scale;

        void initModel(char *); // include the image for the texture
        void drawModel();

        _textureLoader *myTex = new _textureLoader();

    protected:

    private:
};

#endif // _MODEL_H
