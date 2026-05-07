#ifndef _FONTS_H
#define _FONTS_H

#include<_common.h>
#include<_quad.h>


class _fonts
{
    public:
        _fonts();
        virtual ~_fonts();

        void initFonts(char*);
        void buildFonts(const char*);
        void setPosition(float, float,float);
        void drawFonts();

        vec2 frames;
        int counter;// for character offset
        const char *mystr;
         _quad f[500];

    float offset =1.5;
    bool valid =true; // to ignore invalid chars

    protected:

    private:
};

#endif // _FONTS_H
