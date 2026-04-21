#ifndef _SHADER_H
#define _SHADER_H

#include<_common.h>

class _shader
{
    public:
        _shader();
        virtual ~_shader();

        void loadFile(char*, string&);                      // setup basics
        unsigned int loadShader(string&, unsigned int);     // compile and link
        void initShader(char*, char*);                      // read shader files
        void cleanUp();                                     // clean shaders

        unsigned vs, fs, program;                           // handlers

    protected:

    private:
};

#endif // _SHADER_H
