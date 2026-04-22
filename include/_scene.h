#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>
#include<_lightSettings.h>
#include<_model.h>
#include<_inputs.h>
#include<_modelVBO.h>
#include<_parallax.h>
#include<_quad.h>
#include<_player.h>
#include<_sounds.h>
#include<_enemies.h>
#include<_collisionCheck.h>
#include<_bullets.h>
#include<_particles.h>
#include<_shader.h>

class _scene
{
    public:
        _scene();
        virtual ~_scene();

        GLint initGL(); // initialize the openGL contents
        void resize(GLint, GLint); // to handle resize window
        void drawScene(); // render the final scene

        int winMsg(HWND	hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        void mouseMapping(int, int);
        void setupCollisionMap();
        rect2D playerBoundsAt(vec3) const;
        bool collidesWithWall(vec3) const;
        bool fallsIntoPit(vec3) const;
        void respawnPlayer();
        void drawCollisionDebug() const;
        static float deltaTime;
        //auto lastTime = chrono::steady_clock::now();

        _lightSettings *myLight = new _lightSettings();
        _inputs *myKbMs = new _inputs();
        _model *Mymodel = new _model();
        _modelVBO *myVBO = new _modelVBO();
        _parallax *myPrlx = new _parallax();
        _quad *myQuad = new _quad();
        _player *ply = new _player();
        _sounds *snds = new _sounds();
        _collisionCheck *hit = new _collisionCheck();


        vec3 mouse; // to keep track of the mouse location
        vec2 dim;   // window size

        int clickCount = 0; // temp variable to count mouse clicks used in bullets
        std::vector<rect2D> wallZones;
        std::vector<rect2D> pitZones;
        vec3 playerSpawn;
        float playerHalfWidth = 0.20f;
        float playerHalfHeight = 0.18f;
        bool showCollisionDebug = true;

    protected:

    private:
};

#endif // _SCENE_H
