#include "Scene1.h"
#include <cmath>
#include <cstdlib>

Scene1::Scene1()
{
    BoyX = -0.90f;
    GirlX =  0.90f;

    BoyY = -0.55f;
    GirlY = -0.55f;

    MoveStep = 0.030f;

    MeetDone = 0;
    SunGoingDown = 0;

    SunX = 0.70f;
    SunY = 0.72f;

    SkyT = 0.0f;

    HeartActive = 0;
    HeartScale = 0.0f;
    HeartT = 0.0f;

    Bird1X = -0.75f;
    Bird1Y = 0.82f;
    Bird2X = -0.45f;
    Bird2Y = 0.88f;
    Bird3X = -0.15f;
    Bird3Y = 0.84f;
    Bird4X =  0.15f;
    Bird4Y = 0.88f;
    Bird5X =  0.40f;
    Bird5Y = 0.82f;

    BirdSpeed = 0.0028f;
    BirdWaveT = 0.0f;
    Timer = 0.0f;
}

void Scene1::initGL()
{
    glClearColor(0.95f, 0.95f, 1.00f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluLookAt(0, 0, 1,
              0, 0, 0,
              0, 1, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ------------------------------
// helpers
// ------------------------------
float Scene1::clamp01(float v)
{
    if (v < 0.0f)
    {
        return 0.0f;
    }
    if (v > 1.0f)
    {
        return 1.0f;
    }
    return v;
}

float Scene1::absf(float v)
{
    if (v < 0.0f)
    {
        return -v;
    }
    return v;
}

void Scene1::setColor(float r, float g, float b)
{
    glColor3f(clamp01(r), clamp01(g), clamp01(b));
}

// ------------------------------
// shapes
// ------------------------------
void Scene1::drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b)
{
    setColor(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void Scene1::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b)
{
    setColor(r, g, b);
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

void Scene1::drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b)
{
    setColor(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glVertex2f(x4, y4);
    glEnd();
}

void Scene1::drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b)
{
    setColor(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);

        int seg = 60;
        for (int i = 0; i <= seg; i++)
        {
            float a = (2.0f * 3.1415926f * i) / (float)seg;
            float x = cx + std::cos(a) * rx;
            float y = cy + std::sin(a) * ry;
            glVertex2f(x, y);
        }
    glEnd();
}

void Scene1::drawScreenTint(float r, float g, float b, float a)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(clamp01(r), clamp01(g), clamp01(b), clamp01(a));
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    glDisable(GL_BLEND);
}

// ------------------------------
// objects
// ------------------------------
void Scene1::drawBird(float x, float y, float s)
{
    glLineWidth(2.0f);
    setColor(0.12f, 0.12f, 0.12f);
    glBegin(GL_LINES);
        glVertex2f(x - 0.03f * s, y);
        glVertex2f(x, y + 0.02f * s);

        glVertex2f(x, y + 0.02f * s);
        glVertex2f(x + 0.03f * s, y);
    glEnd();
}

void Scene1::drawBench(float x, float y, float s)
{
    drawRectangle(x - 0.18f*s, y + 0.06f*s, x + 0.18f*s, y + 0.10f*s, 0.45f, 0.28f, 0.12f);
    drawRectangle(x - 0.18f*s, y + 0.10f*s, x + 0.18f*s, y + 0.17f*s, 0.42f, 0.25f, 0.11f);

    drawRectangle(x - 0.16f*s, y + 0.00f*s, x - 0.13f*s, y + 0.06f*s, 0.25f, 0.18f, 0.10f);
    drawRectangle(x + 0.13f*s, y + 0.00f*s, x + 0.16f*s, y + 0.06f*s, 0.25f, 0.18f, 0.10f);

    drawRectangle(x - 0.16f*s, y + 0.03f*s, x + 0.16f*s, y + 0.05f*s, 0.30f, 0.20f, 0.10f);
}

void Scene1::drawTree(float x, float y, float s)
{
    drawRectangle(x - 0.04f*s, y - 0.25f*s, x + 0.04f*s, y + 0.05f*s, 0.40f, 0.25f, 0.12f);

    drawEllipse(x, y + 0.14f*s, 0.16f*s, 0.11f*s, 0.20f, 0.55f, 0.22f);
    drawEllipse(x - 0.10f*s,y + 0.10f*s, 0.14f*s, 0.10f*s, 0.18f, 0.50f, 0.20f);
    drawEllipse(x + 0.10f*s,y + 0.10f*s, 0.14f*s, 0.10f*s, 0.18f, 0.50f, 0.20f);
    drawEllipse(x,y + 0.05f*s, 0.18f*s, 0.12f*s, 0.17f, 0.48f, 0.19f);

    drawEllipse(x - 0.05f*s, y + 0.14f*s, 0.06f*s, 0.04f*s, 0.30f, 0.70f, 0.30f);
}

void Scene1::drawHumanBoy(float x, float y, float s, bool walking)
{

    float swing = walking ? std::sin(Timer*10.0f) * 0.020f*s : 0.0f;

    // head + hair
    drawEllipse(x, y + 0.22f*s, 0.06f*s, 0.08f*s, 0.93f, 0.78f, 0.65f);
    drawEllipse(x, y + 0.26f*s, 0.065f*s, 0.045f*s, 0.10f, 0.10f, 0.10f);

    // shirt
    float r,g,b =0.93f;
    drawRectangle(x - 0.07f*s, y + 0.06f*s, x + 0.07f*s, y + 0.18f*s, r, g, b);

    // arms (swing little)
    drawRectangle(x - 0.10f*s, y + 0.10f*s + swing, x - 0.07f*s, y + 0.16f*s + swing, r, g, b);
    drawRectangle(x + 0.07f*s, y + 0.10f*s - swing, x + 0.10f*s, y + 0.16f*s - swing, r, g, b);

    drawRectangle(x - 0.10f*s, y + 0.08f*s + swing, x - 0.06f*s, y + 0.12f*s + swing, 0.93f, 0.78f, 0.65f);
    drawRectangle(x + 0.06f*s, y + 0.08f*s - swing, x + 0.10f*s, y + 0.12f*s - swing, 0.93f, 0.78f, 0.65f);

    // pants
    drawRectangle(x - 0.06f*s, y - 0.08f*s, x + 0.06f*s, y + 0.06f*s, 0.12f, 0.12f, 0.16f);

    // legs (swing opposite)
    drawRectangle(x - 0.055f*s, y - 0.22f*s - swing, x - 0.015f*s, y - 0.08f*s, 0.12f, 0.12f, 0.16f);
    drawRectangle(x + 0.015f*s, y - 0.22f*s + swing, x + 0.055f*s, y - 0.08f*s, 0.12f, 0.12f, 0.16f);

    // shoes
    drawRectangle(x - 0.060f*s, y - 0.24f*s - swing, x - 0.005f*s, y - 0.22f*s - swing, 0.05f, 0.05f, 0.05f);
    drawRectangle(x + 0.005f*s, y - 0.24f*s + swing, x + 0.060f*s, y - 0.22f*s + swing, 0.05f, 0.05f, 0.05f);
}

void Scene1::drawHumanGirl(float x, float y, float s,bool walking)
{
     float swing = walking ? std::sin(FTimer*10.0f) * 0.020f*s : 0.0f;

    // head
    drawEllipse(x, y + 0.22f*s, 0.06f*s, 0.08f*s, 0.93f, 0.78f, 0.65f);

    // hair + side
    drawEllipse(x, y + 0.24f*s, 0.070f*s, 0.055f*s, 0.10f, 0.07f, 0.06f);
    drawRectangle(x - 0.070f*s, y + 0.12f*s, x - 0.030f*s, y + 0.24f*s, 0.10f, 0.07f, 0.06f);
    drawRectangle(x + 0.030f*s, y + 0.12f*s, x + 0.070f*s, y + 0.24f*s, 0.10f, 0.07f, 0.06f);

    // top
    drawRectangle(x - 0.065f*s, y + 0.10f*s, x + 0.065f*s, y + 0.18f*s, 0.72f, 0.18f, 0.35f);

    // skirt
    drawQuad(x - 0.09f*s, y + 0.10f*s,
             x + 0.09f*s, y + 0.10f*s,
             x + 0.06f*s, y - 0.10f*s,
             x - 0.06f*s, y - 0.10f*s,
             0.70f, 0.16f, 0.33f);

    // arms swing
    drawRectangle(x - 0.10f*s, y + 0.10f*s + swing, x - 0.06f*s, y + 0.14f*s + swing, 0.93f, 0.78f, 0.65f);
    drawRectangle(x + 0.06f*s, y + 0.10f*s - swing, x + 0.10f*s, y + 0.14f*s - swing, 0.93f, 0.78f, 0.65f);

    // legs
    drawRectangle(x - 0.030f*s, y - 0.22f*s - swing, x - 0.005f*s, y - 0.10f*s, 0.93f, 0.78f, 0.65f);
    drawRectangle(x + 0.005f*s, y - 0.22f*s + swing, x + 0.030f*s, y - 0.10f*s, 0.93f, 0.78f, 0.65f);

    // shoes
    drawRectangle(x - 0.035f*s, y - 0.24f*s - swing, x - 0.000f*s, y - 0.22f*s - swing, 0.08f, 0.05f, 0.05f);
    drawRectangle(x + 0.000f*s, y - 0.24f*s + swing, x + 0.035f*s, y - 0.22f*s + swing, 0.08f, 0.05f, 0.05f);
}

void Scene1::drawHeart(float x, float y, float s)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(s, s, 1.0f);

        glColor4f(0.92f, 0.12f, 0.28f, 0.95f);

        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(-0.05f, 0.03f);
            int seg = 40;
            for (int i = 0; i <= seg; i++)
            {
                float a = (2.0f * 3.1415926f * i) / (float)seg;
                glVertex2f(-0.05f + std::cos(a)*0.05f, 0.03f + std::sin(a)*0.05f);
            }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.05f, 0.03f);
            for (int i = 0; i <= seg; i++)
            {
                float a = (2.0f * 3.1415926f * i) / (float)seg;
                glVertex2f(0.05f + std::cos(a)*0.05f, 0.03f + std::sin(a)*0.05f);
            }
        glEnd();

        glBegin(GL_TRIANGLES);
            glVertex2f(-0.10f, 0.03f);
            glVertex2f( 0.10f, 0.03f);
            glVertex2f( 0.00f,-0.12f);
        glEnd();

        glColor4f(1.0f, 0.85f, 0.90f, 0.25f);
        drawEllipse(-0.04f, 0.06f, 0.025f, 0.018f, 1,1,1);

    glPopMatrix();

    glDisable(GL_BLEND);
}

// ------------------------------
// sky/sun
// ------------------------------
void Scene1::drawGradientSky(float t)
{
    t = clamp01(t);

    float aTopR = 0.98f, aTopG = 0.70f, aTopB = 0.35f;
    float aLowR = 0.95f, aLowG = 0.86f, aLowB = 0.65f;

    float eTopR = 0.58f, eTopG = 0.42f, eTopB = 0.72f;
    float eLowR = 0.95f, eLowG = 0.62f, eLowB = 0.45f;

    float topR = aTopR + (eTopR - aTopR) * t;
    float topG = aTopG + (eTopG - aTopG) * t;
    float topB = aTopB + (eTopB - aTopB) * t;

    float lowR = aLowR + (eLowR - aLowR) * t;
    float lowG = aLowG + (eLowG - aLowG) * t;
    float lowB = aLowB + (eLowB - aLowB) * t;

    glBegin(GL_QUADS);
        glColor3f(lowR, lowG, lowB);
        glVertex2f(-1.0f, -0.10f);
        glVertex2f( 1.0f, -0.10f);

        glColor3f(topR, topG, topB);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    if (t < 0.65f)
    {
        float haze = (0.65f - t) * 0.18f;
        drawScreenTint(1.0f, 0.75f, 0.45f, haze);
    }
}

void Scene1::drawSun(float x, float y, float t)
{
    float glow = 0.22f - 0.10f * clamp01(t);
    if (glow < 0.05f) glow = 0.05f;

    drawEllipse(x, y, 0.18f, 0.18f, 1.00f, 0.65f, 0.25f);
    drawEllipse(x, y, 0.12f, 0.12f, 1.00f, 0.45f, 0.12f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.75f, 0.35f, glow);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        int seg = 60;
        for (int i = 0; i <= seg; i++)
        {
            float a = (2.0f * 3.1415926f * i) / (float)seg;
            glVertex2f(x + std::cos(a)*0.26f, y + std::sin(a)*0.26f);
        }
    glEnd();
    glDisable(GL_BLEND);
}

// ------------------------------
// scene parts
// ------------------------------
void Scene1::drawGround()
{
    drawRectangle(-1.0f, -1.0f, 1.0f, -0.10f, 0.28f, 0.60f, 0.30f);

    drawEllipse(-0.65f, -0.18f, 0.55f, 0.20f, 0.26f, 0.56f, 0.28f);
    drawEllipse( 0.55f, -0.22f, 0.60f, 0.22f, 0.26f, 0.56f, 0.28f);

    // path
    drawEllipse(0.0f, -0.85f, 0.85f, 0.25f, 0.75f, 0.68f, 0.55f);
    drawEllipse(0.0f, -0.88f, 0.70f, 0.20f, 0.72f, 0.65f, 0.52f);
}

void Scene1::drawHorizonMask()
{
    // covers sun lower half when it drops
    // color matches far hill tone
    drawEllipse(0.78f, 0.18f, 0.70f, 0.26f, 0.26f, 0.56f, 0.28f);
}

void Scene1::drawPanel1()
{
    drawGradientSky(SkyT);

    // moving birds (hard-coded count, but positions update)
    drawBird(Bird1X, Bird1Y, 1.0f);
    drawBird(Bird2X, Bird2Y, 0.9f);
    drawBird(Bird3X, Bird3Y, 1.1f);
    drawBird(Bird4X, Bird4Y, 1.0f);
    drawBird(Bird5X, Bird5Y, 0.95f);

    // sun
    drawSun(SunX, SunY, SkyT);
    drawHorizonMask(); // makes sun half at sunset

    drawGround();

    drawTree(-0.82f, -0.10f, 1.2f);
    drawTree(-0.60f, -0.08f, 1.0f);
    drawTree( 0.85f, -0.10f, 1.25f);

    drawBench(0.02f, -0.50f, 1.0f);

    // larger humans (not chibi)
    drawHumanBoy(BoyX,  BoyY,  1.15f, true);
    drawHumanGirl(GirlX, GirlY, 1.15f, true);

    if (HeartActive == 1)
    {
        drawHeart(0.0f, -0.12f, HeartScale);
    }

    if (SkyT > 0.2f)
    {
        float a = (SkyT - 0.2f) * 0.12f;
        drawScreenTint(0.20f, 0.10f, 0.25f, a);
    }
}

// ------------------------------
// logic
// ------------------------------
void Scene1::checkMeetCollision()
{
    if (MeetDone == 1) return;

    float gap = 0.18f;

    float dx = BoyX - GirlX;
    if (dx < 0.0f) dx = -dx;

    if (dx <= gap)
    {
        float mid = (BoyX + GirlX) * 0.5f;

        BoyX  = mid - gap * 0.5f;
        GirlX = mid + gap * 0.5f;

        MeetDone = 1;
        SunGoingDown = 1;

        HeartActive = 1;
        HeartScale = 0.20f;
        HeartT = 0.0f;
    }
}





// ------------------------------
// public draw/update
// ------------------------------
void Scene1::draw()
{
    // keep fullscreen viewport correct without reshape
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, w, h);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawPanel1();
}

void Scene1::update()
{
    // birds move + little wave
    BirdWaveT += 0.05f;

    Bird1X += BirdSpeed;
    Bird2X += BirdSpeed * 0.9f;
    Bird3X += BirdSpeed * 1.1f;
    Bird4X += BirdSpeed * 0.8f;
    Bird5X += BirdSpeed * 1.0f;

    float wave1 = std::sin(BirdWaveT) * 0.0020f;
    float wave2 = std::sin(BirdWaveT + 1.0f) * 0.0015f;

    Bird1Y = 0.82f + wave1;
    Bird2Y = 0.88f + wave2;
    Bird3Y = 0.84f - wave1;
    Bird4Y = 0.88f - wave2;
    Bird5Y = 0.82f + wave2;

    if (Bird1X > 1.20f) Bird1X = -1.20f;
    if (Bird2X > 1.20f) Bird2X = -1.20f;
    if (Bird3X > 1.20f) Bird3X = -1.20f;
    if (Bird4X > 1.20f) Bird4X = -1.20f;
    if (Bird5X > 1.20f) Bird5X = -1.20f;

    // sun setting animation after meet
    if (SunGoingDown == 1)
    {
        SunY -= 0.0030f;   // stronger visible movement
        SkyT += 0.0020f;

        // sunset stop (evening, not night)
        if (SunY < 0.22f) SunY = 0.22f;
        if (SkyT > 1.0f)  SkyT = 1.0f;

        if (SunY <= 0.22f && SkyT >= 1.0f)
        {
            SunGoingDown = 0;
        }
    }

    // heart pop
    if (HeartActive == 1)
    {
        HeartT += 0.016f;

        float target = 1.0f;
        if (HeartT < 0.18f) target = 1.40f;
        if (HeartT >= 0.18f) target = 1.05f;

        HeartScale = HeartScale + (target - HeartScale) * 0.22f;
    }
    //Timer += 0.08f;
}

// ------------------------------
// input
// ------------------------------
void Scene1::keyDown(unsigned char key)
{
    if (key == 27 || key == 'q' || key == 'Q')
    {
        std::exit(0);
    }


    if (key == 'a' || key == 'A')
    {
        Timer += 0.2f;
    if (MeetDone == 1) return;



    float next = BoyX - MoveStep;
    if (next < -0.95f) next = -0.95f;

    BoyX = next;
    checkMeetCollision();
   }

if (key == 'd' || key == 'D')
{
    Timer += 0.2f;
    if (MeetDone == 1) return;

    float next = BoyX + MoveStep;
    if (next > 0.95f) next = 0.95f;

    float gap = 0.18f;
    if (next > GirlX - gap)
    {
        next = GirlX - gap;   // BLOCK right movement by collision rule
    }

    BoyX = next;
    checkMeetCollision();
}

    // reset (test)
    if (key == 'r' || key == 'R')
    {
        BoyX = -0.90f;
        GirlX = 0.90f;

        MeetDone = 0;
        SunGoingDown = 0;

        SunY = 0.72f;
        SkyT = 0.0f;

        HeartActive = 0;
        HeartScale = 0.0f;
        HeartT = 0.0f;
    }

}

void Scene1::specialDown(int key)
{
    if (key == GLUT_KEY_LEFT)
{
    FTimer += 0.2f;
    if (MeetDone == 1) return;

    float next = GirlX - MoveStep;
    if (next < -0.95f) next = -0.95f;

    float gap = 0.18f;
    if (next < BoyX + gap)
    {
        next = BoyX + gap;    // BLOCK left movement by collision rule
    }

    GirlX = next;
    checkMeetCollision();
}

if (key == GLUT_KEY_RIGHT)
{
    FTimer += 0.2f;
    if (MeetDone == 1) return;

    float next = GirlX + MoveStep;
    if (next > 0.95f) next = 0.95f;

    GirlX = next;
    checkMeetCollision();
}

}
