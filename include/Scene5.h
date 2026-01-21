#ifndef SCENE5_H
#define SCENE5_H

#include <GL/glut.h>

class Scene5
{
public:
    void initGL();
    void reset();
    void keyDown(unsigned char k);
    void specialDown(int k);
    void update(float dt);
    void draw();

private:

    float clamp01(float v);
    //float lerp(float a, float b, float t);


    void setColor(float r, float g, float b);
    void drawRect(float x1, float y1, float x2, float y2);
    void drawLine(float x1, float y1, float x2, float y2, float w);
    void drawCircle(float cx, float cy, float r, int seg);
    void drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b);
    void drawQuad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,
                  float r,float g,float b);
    void drawRectangle(float x1,float y1,float x2,float y2,float r,float g,float b);


    void drawSky();
    void drawBridge();


    void drawHumanBoySwing(float x, float y, float s,
                           float r, float g, float b,
                           bool walking);

    void drawGirlSwing(float x, float y, float s,
                       bool walking);


    void drawCar(float x, float y);

private:

    float ourBoyX;
    float girlX;
    float otherX;

    float groundY;
    float walkT;
    float cloudT;


    bool crossed;
    bool turningBack;
    bool kneeling;

    float turnAngleDeg;


    bool carVisible;
    bool carMoving;
    float carX, carY;

    bool boyGoingToCar;
    bool boyHidden;
};

#endif
