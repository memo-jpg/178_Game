#include "_textureLoader.h"

_textureLoader::_textureLoader()
{
    //ctor
}

_textureLoader::~_textureLoader()
{
    //dtor
}
void _textureLoader::loadTexture(char* fileName)
{
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

    if (!image) cout << "ERROR: Image File not Loaded" << endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glEnable(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void _textureLoader::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, tex);
}
