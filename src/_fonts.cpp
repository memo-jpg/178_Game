#include "_fonts.h"

#include <cctype>
#include <cstring>

namespace
{
    const int FONT_GRID_COLUMNS = 10;
    const int FONT_GRID_ROWS = 8;
    const int MAX_FONT_GLYPHS = 500;
    const char* FONT_ATLAS_ROWS[FONT_GRID_ROWS] =
    {
        "*+,-./0123",
        "456789:;<=",
        ">?@ABCDEFG",
        "HIJKLMNOPQ",
        "RSTUVWXYZ[",
        "\\]^_`abcde",
        "fghijklmno",
        "pqrstuvwxy"
    };

    bool map8BitFontGlyph(char glyph, int& column, int& row)
    {
        if (glyph == ' ')
        {
            column = 0;
            row = 0;
            return true;
        }

        for (int atlasRow = 0; atlasRow < FONT_GRID_ROWS; atlasRow++)
        {
            for (int atlasColumn = 0; atlasColumn < FONT_GRID_COLUMNS; atlasColumn++)
            {
                if (FONT_ATLAS_ROWS[atlasRow][atlasColumn] == glyph)
                {
                    column = atlasColumn;
                    row = atlasRow;
                    return true;
                }
            }
        }

        if (glyph == 'z')
        {
            return map8BitFontGlyph('Z', column, row);
        }

        if (std::islower((unsigned char)glyph))
        {
            return map8BitFontGlyph((char)std::toupper((unsigned char)glyph), column, row);
        }

        return false;
    }
}

_fonts::_fonts()
{
    mystr = "";
    counter = 0;
}

_fonts::~_fonts()
{
    //dtor
}

void _fonts::initFonts(char* fileName)
{
    f[0].initQuad(fileName);

    for (int i = 0; i < MAX_FONT_GLYPHS; i++)
    {
        f[i].myTex->tex = f[0].myTex->tex;
        f[i].initQuad(NULL);
        glyphVisible[i] = false;
    }
}

void _fonts::buildFonts(const char* str)
{
    mystr = str != NULL ? str : "";
    counter = (int)std::min(std::strlen(mystr), (size_t)MAX_FONT_GLYPHS);

    for (int i = 0; i < MAX_FONT_GLYPHS; i++)
    {
        glyphVisible[i] = false;
    }

    for (int i = 0; i < counter; i++)
    {
        int column = 0;
        int row = 0;
        glyphVisible[i] = map8BitFontGlyph(mystr[i], column, row);

        if (!glyphVisible[i])
        {
            continue;
        }

        f[i].xMin = (float)column / (float)FONT_GRID_COLUMNS;
        f[i].xMax = (float)(column + 1) / (float)FONT_GRID_COLUMNS;
        f[i].yMin = (float)row / (float)FONT_GRID_ROWS;
        f[i].yMax = (float)(row + 1) / (float)FONT_GRID_ROWS;
        f[i].updateQuad();
    }
}

void _fonts::setPosition(float x, float y, float z)
{
    textPos.x = x;
    textPos.y = y;
    textPos.z = z;
}

void _fonts::drawFonts()
{
    for (int i = 0; i < counter; i++)
    {
        if (!glyphVisible[i] || mystr[i] == ' ')
        {
            continue;
        }

        f[i].pos.x = textPos.x + (i * offset);
        f[i].pos.y = textPos.y;
        f[i].pos.z = textPos.z;
        f[i].scale = textScale;
        f[i].drawQuad();
    }
}
