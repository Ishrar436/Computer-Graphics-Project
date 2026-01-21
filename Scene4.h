#ifndef SCENE4_H
#define SCENE4_H

#include <GL/glut.h>

class Scene4 {
public:
    Scene4();
    void initGL();
    void draw();
    void update();
    void keyDown(unsigned char key);
    void specialDown(int key);
    float MoonX, MoonY;
    float SkyT;          // 0.0 for night, 1.0 for bright day
    int   MoonSetting;

private:
    // Helpers
    void setColor(float r, float g, float b);
    void drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b);
    void drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b);

    // Scene Parts
    void drawSky();
    void drawSkyline();
    void drawStreet();
    void drawManSitting();

    // Variables
    float StarTimer;
    float ManBreatheT;
    int   LetterLook;      // Toggle 0 or 1
    float LetterOffset;

    int   ShootingStarActive;
    float ShootingStarX;
    float ShootingStarY;
};

#endif


