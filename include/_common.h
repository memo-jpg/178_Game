#ifndef _COMMON_H
#define _COMMON_H

#include<GL/glew.h>
#include<windows.h>
#include<iostream>
#include<GL/gl.h>
#include<GL/glut.h>
#include<time.h>
#include<cmath>
#include <stdlib.h>
#include<chrono>
#include<random>
#include<fstream>
#include<vector>

#define PI 3.14159
#define GLEW_STATIC
#define GRAVITY 9.81
#define MAX_DROPS 5000

using namespace std;

typedef struct{

    double x;
    double y;
    double z;

}vec3;

typedef struct{

    float x;
    float y;
}vec2;

typedef struct{

    float left;
    float right;
    float bottom;
    float top;

}rect2D;


#endif // _COMMON_H
