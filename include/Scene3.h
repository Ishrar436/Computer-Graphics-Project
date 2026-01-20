#include <cmath>
#include <GL/glut.h>



class Scene3 {
public:
    float clamp01(float v);
    float lerp(float a, float b, float t);
    void initGL();
    void reset();
    void update(float dt);
    void draw();
    void keyDown(unsigned char k);
    void drawRect(float x1, float y1, float x2, float y2);
    void drawCircle(float cx, float cy, float radius, int segments = 48);
    void drawLine(float x1, float y1, float x2, float y2, float width = 2.0f);
    void drawStar(float cx, float cy, float s);



private:
    // ----------- helpers -----------
    void setColor(float r, float g, float b) { glColor3f(r, g, b); }


    void drawBackground();
    void drawStation();
    void drawTracks();
    void drawClock(float cx, float cy, float scale);
    void drawSun();
    void drawTrain();
    void drawLetter();
    void drawBoy();
    void drawBrokenHeart(float x, float y, float scale);
    void drawCloud(float x, float y, float scale);
    void drawAllClouds();
    bool boyChasingTrain = false;
bool boyMissedTrain  = false;
float trainDoorWorldX = 0.0f;
void drawWomanInTrain();

    // ----------- state -----------
    // Train
    float trainX = 0.0f;
    bool trainMoving = false;
    bool trainGone = false;
    float cloud1X, cloud2X, cloud3X;

    // Letter
    bool letterVisible = false;
    bool letterArriving = false;
    bool letterArrived = false;
    float letterX = 1.2f;
    float letterY = -0.08f;     // on platform area
    float letterFloatT = 0.0f;  // small float wave

    // Boy
    float boyX = 0.30f;
    float boyY = -0.25f;
    bool boyMovingToLetter = false;
    bool boyPickedLetter = false;
    float runPhase = 0.0f;

    // Broken heart + time-lapse
    bool heartVisible = false;
    bool timeLapse = false;
    float dayToNight = 0.0f; // 0 day -> 1 night

    // Clock angles (radians)
    float minuteAng = 0.0f;
    float hourAng = 0.0f;

    // Sun position
    float sunY = 0.82f; // starts high

    // cached letter stop position
    float letterStopX = 0.55f;
};
