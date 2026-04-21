#include "_quad.h"

_quad::_quad()
{
    //ctor
    rot.x = rot.y = rot.z = 0;
    pos.x = pos.y = 0;
    pos.z = -8;
    scale.x = scale.y = scale.z = 1;
}

_quad::~_quad()
{
    //dtor
}
void _quad::initQuad(char* fileName)
{
    glGenBuffers(1, &vboPos);
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &vboTex);
    glBindBuffer(GL_ARRAY_BUFFER, vboTex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_DYNAMIC_DRAW);

    if(fileName) myTex->loadTexture(fileName);
}

void _quad::drawQuad()
{
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);               // set base color

    myTex->bindTexture();

    glTranslatef(pos.x, pos.y, pos.z);      // place the object
    glRotatef(rot.x, 1, 0, 0);         // rotation around x axis
    glRotatef(rot.y, 0, 1, 0);         // rotation around y axis
    glRotatef(rot.z, 0, 0, 1);         // rotation around z axis
    glScalef(scale.x, scale.y, scale.z);    // resize object

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, vboTex);
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
}

void _quad::updateQuad()
{
    float texCoord[8] = {   xMax, yMin,
                            xMax, yMax,
                            xMin, yMax,
                            xMin, yMin
    };

    glBindBuffer(GL_ARRAY_BUFFER, vboTex);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoord), texCoord);
}
