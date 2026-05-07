#include "_fonts.h"

_fonts::_fonts()
{
    //ctor
}

_fonts::~_fonts()
{
    //dtor
}
void _fonts::initFonts(char* fileName)
{
    f[0].initQuad(fileName);

    for(int i =0; i<500; i++)
    {
         f[i].myTex->tex = f[0].myTex->tex;
         f[i].initQuad(NULL);
    }

  // todo: initialize all the quads
}

void _fonts::buildFonts(const char* str)
{
     mystr = str;


    for(int i =0; i<strlen(str); i++)
    {
     valid = true;

    switch(str[i])
    {
    case 'a':
             f[i].xMin =0;
             f[i].xMax =1.0/7.0;
             f[i].yMin =0;
             f[i].yMax = 1.0/6.0;
             f[i].updateQuad();
        break;
    case 'b':
             f[i].xMin =1.0/7.0;
             f[i].xMax =2.0/7.0;
             f[i].yMin =0;
             f[i].yMax = 1.0/6.0;
             f[i].updateQuad();

        break;

    case 'c':
             f[i].xMin =2.0/7.0;
             f[i].xMax =3.0/7.0;
             f[i].yMin =0;
             f[i].yMax = 1.0/6.0;
             f[i].updateQuad();

        break;

    case 'd':
             f[i].xMin =3.0/7.0;
             f[i].xMax =4.0/7.0;
             f[i].yMin =0;
             f[i].yMax = 1.0/6.0;
             f[i].updateQuad();

        break;

    case 'e':
             f[i].xMin =4.0/7.0;
             f[i].xMax =5.0/7.0;
             f[i].yMin =0;
             f[i].yMax = 1.0/6.0;
             f[i].updateQuad();

        break;
    case 'f':
             f[i].xMin =5.0/7.0;
             f[i].xMax =6.0/7.0;
             f[i].yMin =0;
             f[i].yMax = 1.0/6.0;
             f[i].updateQuad();

        break;
    case 'g':
             f[i].xMin =6.0/7.0;
             f[i].xMax =7.0/7.0;
             f[i].yMin =0;
             f[i].yMax = 1.0/6.0;
             f[i].updateQuad();

        break;
    case 'h':
             f[i].xMin =0.0/7.0;
             f[i].xMax =1.0/7.0;
             f[i].yMin =1.0/6.0;
             f[i].yMax = 2.0/6.0;
             f[i].updateQuad();
        break;

     case 'i':
             f[i].xMin =1.0/7.0;
             f[i].xMax =2.0/7.0;
             f[i].yMin =1.0/6.0;
             f[i].yMax = 2.0/6.0;
             f[i].updateQuad();
        break;
     case 'j':
             f[i].xMin =2.0/7.0;
             f[i].xMax =3.0/7.0;
             f[i].yMin =1.0/6.0;
             f[i].yMax = 2.0/6.0;
             f[i].updateQuad();
        break;

      case 'k':
             f[i].xMin =3.0/7.0;
             f[i].xMax =4.0/7.0;
             f[i].yMin =1.0/6.0;
             f[i].yMax = 2.0/6.0;
             f[i].updateQuad();
        break;

        case 'l':
             f[i].xMin =4.0/7.0;
             f[i].xMax =5.0/7.0;
             f[i].yMin =1.0/6.0;
             f[i].yMax = 2.0/6.0;
             f[i].updateQuad();
        break;

         case 'm':
             f[i].xMin =5.0/7.0;
             f[i].xMax =6.0/7.0;
             f[i].yMin =1.0/6.0;
             f[i].yMax = 2.0/6.0;
             f[i].updateQuad();
        break;

         case 'n':
             f[i].xMin = 6.0/7.0;
             f[i].xMax = 7.0/7.0;
             f[i].yMin = 1.0/6.0;
             f[i].yMax = 2.0/6.0;
             f[i].updateQuad();
        break;

        case 'o':
             f[i].xMin = 0;
             f[i].xMax = 1.0/7.0;
             f[i].yMin = 2.0/6.0;
             f[i].yMax = 3.0/6.0;
             f[i].updateQuad();
        break;
        case 'p':
             f[i].xMin =1.0/7.0;
             f[i].xMax =2.0/7.0;
             f[i].yMin = 2.0/6.0;
             f[i].yMax = 3.0/6.0;
             f[i].updateQuad();
        break;

        case 'q':
             f[i].xMin =2.0/7.0;
             f[i].xMax =3.0/7.0;
             f[i].yMin = 2.0/6.0;
             f[i].yMax = 3.0/6.0;
             f[i].updateQuad();
        break;

        case 'r':
             f[i].xMin =3.0/7.0;
             f[i].xMax =4.0/7.0;
             f[i].yMin = 2.0/6.0;
             f[i].yMax = 3.0/6.0;
             f[i].updateQuad();
        break;

        case 's':
             f[i].xMin =4.0/7.0;
             f[i].xMax =5.0/7.0;
             f[i].yMin = 2.0/6.0;
             f[i].yMax = 3.0/6.0;
             f[i].updateQuad();
        break;
        case 't':
             f[i].xMin =5.0/7.0;
             f[i].xMax =6.0/7.0;
             f[i].yMin = 2.0/6.0;
             f[i].yMax = 3.0/6.0;
             f[i].updateQuad();

        break;
        case 'u':
             f[i].xMin =6.0/7.0;
             f[i].xMax =7.0/7.0;
             f[i].yMin = 2.0/6.0;
             f[i].yMax = 3.0/6.0;
             f[i].updateQuad();

        break;
        case 'v':
             f[i].xMin =0.0/7.0;
             f[i].xMax =1.0/7.0;
             f[i].yMin =3.0/6.0;
             f[i].yMax = 4.0/6.0;
             f[i].updateQuad();
        break;

          case 'w':
             f[i].xMin =1.0/7.0;
             f[i].xMax =2.0/7.0;
             f[i].yMin =3.0/6.0;
             f[i].yMax = 4.0/6.0;
             f[i].updateQuad();
        break;

          case 'x':
             f[i].xMin =2.0/7.0;
             f[i].xMax =3.0/7.0;
             f[i].yMin =3.0/6.0;
             f[i].yMax = 4.0/6.0;
             f[i].updateQuad();
        break;

          case 'y':
             f[i].xMin =3.0/7.0;
             f[i].xMax =4.0/7.0;
             f[i].yMin =3.0/6.0;
             f[i].yMax = 4.0/6.0;
             f[i].updateQuad();
        break;

          case 'z':
             f[i].xMin =4.0/7.0;
             f[i].xMax =5.0/7.0;
             f[i].yMin =3.0/6.0;
             f[i].yMax = 4.0/6.0;
             f[i].updateQuad();
        break;

          case '!':
             f[i].xMin =5.0/7.0;
             f[i].xMax =6.0/7.0;
             f[i].yMin =3.0/6.0;
             f[i].yMax = 4.0/6.0;
             f[i].updateQuad();
        break;

          case '?':
             f[i].xMin =6.0/7.0;
             f[i].xMax =7.0/7.0;
             f[i].yMin =3.0/6.0;
             f[i].yMax = 4.0/6.0;
             f[i].updateQuad();
        break;

          case '0':
             f[i].xMin =0.0/7.0;
             f[i].xMax =1.0/7.0;
             f[i].yMin =4.0/6.0;
             f[i].yMax =5.0/6.0;
             f[i].updateQuad();
        break;

          case '1':
             f[i].xMin =1.0/7.0;
             f[i].xMax =2.0/7.0;
             f[i].yMin =4.0/6.0;
             f[i].yMax = 5.0/6.0;
             f[i].updateQuad();
        break;
          case '2':
             f[i].xMin =2.0/7.0;
             f[i].xMax =3.0/7.0;
             f[i].yMin =4.0/6.0;
             f[i].yMax = 5.0/6.0;
             f[i].updateQuad();
        break;

         case '3':
             f[i].xMin =3.0/7.0;
             f[i].xMax =4.0/7.0;
             f[i].yMin =4.0/6.0;
             f[i].yMax = 5.0/6.0;
             f[i].updateQuad();
        break;

        case '4':
             f[i].xMin =4.0/7.0;
             f[i].xMax =5.0/7.0;
             f[i].yMin =4.0/6.0;
             f[i].yMax = 5.0/6.0;
             f[i].updateQuad();
        break;
          case '5':
             f[i].xMin =5.0/7.0;
             f[i].xMax =6.0/7.0;
             f[i].yMin =4.0/6.0;
             f[i].yMax = 5.0/6.0;
             f[i].updateQuad();
        break;

         case '6':
             f[i].xMin =6.0/7.0;
             f[i].xMax =7.0/7.0;
             f[i].yMin =4.0/6.0;
             f[i].yMax =5.0/6.0;
             f[i].updateQuad();
        break;
          case '7':
             f[i].xMin =0.0/7.0;
             f[i].xMax =1.0/7.0;
             f[i].yMin =5.0/6.0;
             f[i].yMax =6.0/6.0;
             f[i].updateQuad();
        break;

         case '8':
             f[i].xMin =1.0/7.0;
             f[i].xMax =2.0/7.0;
             f[i].yMin =5.0/6.0;
             f[i].yMax =6.0/6.0;
             f[i].updateQuad();
        break;
          case '9':
             f[i].xMin =2.0/7.0;
             f[i].xMax =3.0/7.0;
             f[i].yMin =5.0/6.0;
             f[i].yMax =6.0/6.0;
             f[i].updateQuad();
        break;

          case ' ':
             f[i].xMin =3.0/7.0;
             f[i].xMax =4.0/7.0;
             f[i].yMin =5.0/6.0;
             f[i].yMax =6.0/6.0;
             f[i].updateQuad();
        break;

          default:
                valid = false;
            break;
     }
    }
}

void _fonts::setPosition(float x, float y, float z)
{

}

void _fonts::drawFonts()
{
       for(int i =0; i<strlen(mystr); i++){

        if(valid)
        {
        f[i].pos.x =-23+i*offset;
        f[i].pos.y = 10;
        f[i].pos.z =-7;
        f[i].drawQuad();
       }
       }
}
