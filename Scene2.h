#ifndef SCENE2_H
#define SCENE2_H

#include <GL/glut.h>

class Scene2
{
public:
    Scene2();

    void initGL();
    void draw();
    void update();

    void keyDown(unsigned char key);
    void specialDown(int key);

private:
    // ------------------------------
    // state
    // ------------------------------
    float BoyX, BoyY;
    float GirlX, GirlY;

    float PeopleScale;
    float ScaleStep;

    int   MeetDone;
    float CloseStep;

    // umbrella
    int   UmbrellaOpen;      // 0/1 toggle
    float UmbrellaAnim;      // 0..1 smooth open
    float UmbrellaY;         // adjustable
    float UmbrellaX;
    float UmbrellaT;
    float umbrellaScale;        // auto follow midpoint

    // rain
    float RainT;
    float RainSpeed;
    float RainOffset;

    // cover timer + heart glow
    float CoverTimer;
    int   HeartGlow;
    int HeartActive;
    float HeartPulseT;

    // ------------------------------
    // helpers
    // ------------------------------
    float clamp01(float v);
    float absf(float v);
    void  setColor(float r, float g, float b);

    // ------------------------------
    // shapes
    // ------------------------------
    void drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b);
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b);
    void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b);
    void drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b);

    void drawScreenTint(float r, float g, float b, float a);

    // ------------------------------
    // objects
    // ------------------------------
    void drawCloud(float x, float y, float s);
    void drawRain();
    void drawStreetLamp(float x, float y, float s);
    void drawPuddle(float x, float y, float s);

    void drawUmbrella(float x, float y, float openT);

    void drawHeartGlow(float x, float y, float s);
    float roadHalfWidth(float y);

    // NOTE: COPY these from Scene1.cpp into Scene2.cpp
    void drawHumanBoy(float x, float y, float s);
    void drawHumanGirl(float x, float y, float s);
    void drawSky();
    void drawRoad();
    void drawLamp(float x, float y, float s);
    void drawPuddle(float x, float y);
    void checkUmbrellaCover();

    // ------------------------------
    // scene
    // ------------------------------
    void drawPanel2();
    void checkMeetLock(); // lock when they get close (for X key)
};


#endif // SCENE2_H

