#ifndef SCENE5_H
#define SCENE5_H

#include <GL/glut.h>

class Scene5
{
public:
    Scene5();
    float clamp01(float v);

    void initGL();
    void draw();
    void update(float dt);
    void reset();

    void keyDown(unsigned char key);
    void specialDown(int key);

float ourBoyX, ourBoyY;
float girlX, girlY;
float otherX, otherY;

// anim/time
float walkT;

// flags
bool started;
bool crossed;
bool boyStop;
bool boyLookBack;
bool boyKneel;

// timer after cross
float afterCrossT;

// helpers used
void drawCircle(float cx, float cy, float r, int seg);

// new person functions
void drawOurBoy(float x, float groundY, float s);
void drawGirl(float x, float groundY, float s);
void drawOtherBoy(float x, float groundY, float s); // small wave (optional)

private:
    // ---- helpers ----

    void  setColor(float r, float g, float b);

    // ---- primitives ----
    void drawRect(float x1, float y1, float x2, float y2);
    void drawQuad(float x1, float y1, float x2, float y2,
                  float x3, float y3, float x4, float y4, float r, float g, float b);
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
    void drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b);
    void drawLine(float x1,float y1,float x2,float y2,float w);


    // ---- scene parts ----
    void drawSky();
    void drawRiver();
    void drawCityline();

    void drawBridge();     // <- your  similar bridge like image
    void drawBridgeTower(float x, float y, float s);
    void drawCables(float x1, float y1, float x2, float y2);
    void drawWalkway();

    void drawManWalking(float x, float y, float s, float t);
};

#endif

