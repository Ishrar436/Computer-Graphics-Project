#include "Scene2.h"
#include <cmath>
#include <cstdlib>
Scene2::Scene2()
{
    BoyX = -0.22f;
    GirlX =  0.22f;

    BoyY = -0.62f;
    GirlY = -0.62f;

    PeopleScale = 1.20f;
    ScaleStep   = 0.06f;

    CloseStep = 0.020f;
    MeetDone  = 0;

    UmbrellaOpen = 0;
    UmbrellaT    = 0.0f;
    UmbrellaX    = 0.0f;
    UmbrellaY    = -0.40f;


    RainOffset = 0.0f;
    RainSpeed  = 0.055f;

    CoverTimer   = 0.0f;
    HeartActive  = 0;
    HeartPulseT  = 0.0f;
}

void Scene2::initGL()
{
    glClearColor(0.90f, 0.92f, 0.95f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluLookAt(0, 0, 1,
              0, 0, 0,
              0, 1, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float Scene2::clamp01(float v)
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

float Scene2::absf(float v)
{
    if (v < 0.0f)
    {
        return -v;
    }
    return v;
}

void Scene2::setColor(float r, float g, float b)
{
    glColor3f(clamp01(r), clamp01(g), clamp01(b));
}

void Scene2::drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b)
{
    setColor(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void Scene2::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b)
{
    setColor(r, g, b);
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

void Scene2::drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b)
{
    setColor(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glVertex2f(x4, y4);
    glEnd();
}

void Scene2::drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b)
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

void Scene2::drawScreenTint(float r, float g, float b, float a)
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

float Scene2::roadHalfWidth(float y)
{
    float k = clamp01( (y + 1.0f) / 1.05f );

    float nearW = 0.65f;
    float farW  = 0.08f;

    return nearW + (farW - nearW) * k;
}


void Scene2::drawSky()
{
    glBegin(GL_QUADS);
       glColor3f(0.22f, 0.28f, 0.42f);
        glVertex2f(-1.0f, -0.10f);
        glVertex2f( 1.0f, -0.10f);

        glColor3f(0.12f, 0.18f, 0.32f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    drawScreenTint(0.70f, 0.76f, 0.88f, 0.12f);
}

void Scene2::drawCloud(float x, float y, float s)
{
    drawEllipse(x, y, 0.26f*s, 0.12f*s, 0.78f, 0.78f, 0.82f);
    drawEllipse(x - 0.20f*s, y, 0.22f*s, 0.10f*s, 0.74f, 0.74f, 0.78f);
    drawEllipse(x + 0.20f*s, y, 0.22f*s, 0.10f*s, 0.74f, 0.74f, 0.78f);
    drawEllipse(x, y + 0.06f*s, 0.18f*s, 0.09f*s, 0.82f, 0.82f, 0.86f);
}

void Scene2::drawRoad()
{
    drawRectangle(-1.0f, -1.0f, 1.0f, -0.10f, 0.22f, 0.50f, 0.26f);

    for(float y = -1.00f; y < -0.095f; y += 0.025f)
    {
        float w1 = roadHalfWidth(y);
        float w2 = roadHalfWidth(y+0.025f);

        setColor(0.32f, 0.32f, 0.34f);
        glBegin(GL_QUADS);
            glVertex2f(-w1, y);    glVertex2f(w1, y);
            glVertex2f(w2, y+0.025f); glVertex2f(-w2, y+0.025f);
        glEnd();
    }

    glLineWidth(4.0f);
    setColor(0.45f, 0.45f, 0.48f);
    glBegin(GL_LINES);
        glVertex2f(-0.65f, -1.0f); glVertex2f(-roadHalfWidth(-0.10f), -0.10f);
        glVertex2f( 0.65f, -1.0f); glVertex2f( roadHalfWidth(-0.10f), -0.10f);
    glEnd();

    for(float y = -0.95f; y < -0.12f; y += 0.18f)
    {
        float hw = roadHalfWidth(y);
        float dashW = 0.030f * (hw / 0.65f);
        drawRectangle(-dashW, y, dashW, y + 0.08f, 0.82f, 0.82f, 0.84f);
    }
}
void Scene2::drawLamp(float x, float y, float s)
{
    drawRectangle(x - 0.02f*s, y - 0.60f*s, x + 0.02f*s, y + 0.25f*s,
                  0.18f, 0.18f, 0.20f);

    drawRectangle(x - 0.06f*s, y + 0.23f*s, x + 0.06f*s, y + 0.28f*s,
                  0.22f, 0.22f, 0.24f);
    drawEllipse(x, y + 0.20f*s, 0.05f*s, 0.06f*s,
                1.00f, 0.96f, 0.78f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
        glTranslatef(x, y + 0.20f*s, 0.0f);

        glColor4f(1.00f, 0.96f, 0.80f, 0.45f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f);
            int seg = 50;
            for (int i = 0; i <= seg; ++i)
            {
                float a = (2.0f * 3.1415926f * i) / (float)seg;
                glVertex2f(std::cos(a) * 0.18f*s,
                           std::sin(a) * 0.14f*s);
            }
        glEnd();

        glColor4f(1.00f, 0.90f, 0.60f, 0.22f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, -0.02f*s);
            for (int i = 0; i <= seg; ++i)
            {
                float a = (2.0f * 3.1415926f * i) / (float)seg;
                glVertex2f(std::cos(a) * 0.30f*s,
                           std::sin(a) * 0.22f*s - 0.02f*s);
            }
        glEnd();

        glColor4f(1.00f, 0.95f, 0.70f, 0.30f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, -0.01f*s);
            glVertex2f(-0.30f*s, -0.10f*s);
            glVertex2f(-0.45f*s, -0.40f*s);
            glVertex2f( 0.00f*s, -0.55f*s);
            glVertex2f( 0.45f*s, -0.40f*s);
            glVertex2f( 0.30f*s, -0.10f*s);
        glEnd();

    glPopMatrix();

    glDisable(GL_BLEND);
}


void Scene2::drawPuddle(float x, float y)
{
    drawEllipse(x, y, 0.22f, 0.07f, 0.20f, 0.45f, 0.75f);
    drawEllipse(x + 0.03f, y - 0.01f, 0.16f, 0.05f, 0.25f, 0.50f, 0.80f);
}

void Scene2::drawRain()
{
    glLineWidth(1.8f);

    glBegin(GL_LINES);
    setColor(0.22f, 0.55f, 0.95f);

    int count = 180;
    for (int i = 0; i < count; i++)
    {
        float baseX = -1.4f + (float)i * 0.016f;
        float baseY = 1.30f - std::fmod((float)i * 0.13f + RainOffset, 2.40f);

        float len = 0.16f + 0.04f * std::sin(RainOffset * 2.0f + i);

        glVertex2f(baseX, baseY);
        glVertex2f(baseX + 0.08f, baseY - len);
    }
    glEnd();
    glLineWidth(1.2f);
    setColor(0.45f, 0.65f, 0.95f);
    glBegin(GL_LINES);
    for (int i = 0; i < 80; i++)
    {
        float x = -1.2f + (float)i * 0.030f;
        float y = -0.95f - std::fmod((float)i * 0.25f + RainOffset * 0.7f, 0.35f);
        glVertex2f(x - 0.015f, y);
        glVertex2f(x + 0.015f, y);
    }
    glEnd();
}

void Scene2::drawHumanBoy(float x, float y, float s)
{

    drawEllipse(x, y + 0.22f*s, 0.06f*s, 0.08f*s, 0.93f, 0.78f, 0.65f);
    drawEllipse(x, y + 0.26f*s, 0.065f*s, 0.045f*s, 0.10f, 0.10f, 0.10f);


    drawRectangle(x - 0.07f*s, y + 0.06f*s, x + 0.07f*s, y + 0.18f*s, 0.18f, 0.35f, 0.70f);


    drawRectangle(x - 0.10f*s, y + 0.10f*s, x - 0.07f*s, y + 0.16f*s, 0.18f, 0.35f, 0.70f);
    drawRectangle(x + 0.07f*s, y + 0.10f*s, x + 0.10f*s, y + 0.16f*s, 0.18f, 0.35f, 0.70f);

    drawRectangle(x - 0.10f*s, y + 0.08f*s, x - 0.06f*s, y + 0.12f*s, 0.93f, 0.78f, 0.65f);
    drawRectangle(x + 0.06f*s, y + 0.08f*s, x + 0.10f*s, y + 0.12f*s, 0.93f, 0.78f, 0.65f);


    drawRectangle(x - 0.06f*s, y - 0.08f*s, x + 0.06f*s, y + 0.06f*s, 0.12f, 0.12f, 0.16f);

    drawRectangle(x - 0.055f*s, y - 0.22f*s, x - 0.015f*s, y - 0.08f*s, 0.12f, 0.12f, 0.16f);
    drawRectangle(x + 0.015f*s, y - 0.22f*s, x + 0.055f*s, y - 0.08f*s, 0.12f, 0.12f, 0.16f);


    drawRectangle(x - 0.060f*s, y - 0.24f*s, x - 0.005f*s, y - 0.22f*s, 0.05f, 0.05f, 0.05f);
    drawRectangle(x + 0.005f*s, y - 0.24f*s, x + 0.060f*s, y - 0.22f*s, 0.05f, 0.05f, 0.05f);
}

void Scene2::drawHumanGirl(float x, float y, float s)
{

    drawEllipse(x, y + 0.22f*s, 0.06f*s, 0.08f*s, 0.93f, 0.78f, 0.65f);

    drawEllipse(x, y + 0.24f*s, 0.070f*s, 0.055f*s, 0.10f, 0.07f, 0.06f);
    drawRectangle(x - 0.070f*s, y + 0.12f*s, x - 0.030f*s, y + 0.24f*s, 0.10f, 0.07f, 0.06f);
    drawRectangle(x + 0.030f*s, y + 0.12f*s, x + 0.070f*s, y + 0.24f*s, 0.10f, 0.07f, 0.06f);


    drawRectangle(x - 0.065f*s, y + 0.10f*s, x + 0.065f*s, y + 0.18f*s, 0.72f, 0.18f, 0.35f);


    drawQuad(x - 0.09f*s, y + 0.10f*s,
             x + 0.09f*s, y + 0.10f*s,
             x + 0.06f*s, y - 0.10f*s,
             x - 0.06f*s, y - 0.10f*s,
             0.70f, 0.16f, 0.33f);


    drawRectangle(x - 0.10f*s, y + 0.10f*s, x - 0.06f*s, y + 0.14f*s, 0.93f, 0.78f, 0.65f);
    drawRectangle(x + 0.06f*s, y + 0.10f*s, x + 0.10f*s, y + 0.14f*s, 0.93f, 0.78f, 0.65f);


    drawRectangle(x - 0.030f*s, y - 0.22f*s, x - 0.005f*s, y - 0.10f*s, 0.93f, 0.78f, 0.65f);
    drawRectangle(x + 0.005f*s, y - 0.22f*s, x + 0.030f*s, y - 0.10f*s, 0.93f, 0.78f, 0.65f);


    drawRectangle(x - 0.035f*s, y - 0.24f*s, x - 0.000f*s, y - 0.22f*s, 0.08f, 0.05f, 0.05f);
    drawRectangle(x + 0.000f*s, y - 0.24f*s, x + 0.035f*s, y - 0.22f*s, 0.08f, 0.05f, 0.05f);
}

void Scene2::drawUmbrella(float x, float y, float s)
{
    drawRectangle(x - 0.010f*s, y - 0.38f*s, x + 0.010f*s, y + 0.16f*s,
                  0.12f, 0.12f, 0.14f);

    float t = clamp01(UmbrellaT);

    if (t < 0.20f)
    {
        drawRectangle(x - 0.030f*s, y + 0.10f*s, x + 0.030f*s, y + 0.18f*s,
                      0.86f, 0.12f, 0.20f);
        drawTriangle(x - 0.030f*s, y + 0.10f*s,
                     x + 0.030f*s, y + 0.10f*s,
                     x,            y + 0.03f*s,
                     0.86f, 0.12f, 0.20f);
        return;
    }

    float open = t; // 0..1
    float canopyRX = 0.32f * s * open;
    float canopyRY = 0.20f * s * open;

    glPushMatrix();
        glTranslatef(x, y + 0.22f*s, 0.0f);
        glRotatef(-10.0f + 10.0f*open, 0.0f, 0.0f, 1.0f);

        drawEllipse(0.0f, 0.0f, canopyRX, canopyRY,
                    0.86f, 0.12f, 0.20f);

        drawEllipse(0.0f, -0.03f*s, canopyRX*0.85f, canopyRY*0.55f,
                    0.78f, 0.08f, 0.16f);

        float scallopY = -0.08f * s;
        drawEllipse(-0.12f*s, scallopY, 0.055f*s, 0.030f*s,
                    0.70f, 0.06f, 0.14f);
        drawEllipse( 0.00f*s, scallopY, 0.055f*s, 0.030f*s,
                    0.70f, 0.06f, 0.14f);
        drawEllipse( 0.12f*s, scallopY, 0.055f*s, 0.030f*s,
                    0.70f, 0.06f, 0.14f);
    glPopMatrix();
}

void Scene2::drawHeartGlow(float x, float y, float s)
{
    float pulse = 0.0f;
    if (HeartActive == 1)
    {
        pulse = 0.50f + 0.50f * std::sin(HeartPulseT);
    }

    float alpha = 0.35f + 0.35f * pulse;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(s, s, 1.0f);

        glColor4f(1.0f, 0.35f, 0.55f, 0.14f + 0.10f*pulse);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f);
            int seg = 60;
            for (int i = 0; i <= seg; i++)
            {
                float a = (2.0f * 3.1415926f * i) / (float)seg;
                glVertex2f(std::cos(a)*0.22f, std::sin(a)*0.18f);
            }
        glEnd();

        glColor4f(0.92f, 0.12f, 0.28f, alpha);

        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(-0.05f, 0.03f);
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

    glPopMatrix();

    glDisable(GL_BLEND);
}

void Scene2::checkUmbrellaCover()
{
    if (UmbrellaT < 0.85f)
    {
        CoverTimer  = 0.0f;
        HeartActive = 0;
        return;
    }

    float midX = (BoyX + GirlX) * 0.5f + UmbrellaX;
    float umbBaseY = UmbrellaY;

    float umbCanopyY = umbBaseY + 0.20f * 1.20f;

    float headBoyX = BoyX;
    float headBoyY = BoyY + 0.22f * 1.15f;

    float headGirlX = GirlX;
    float headGirlY = GirlY + 0.22f * 1.15f;

    int coverBoy  = (absf(headBoyX  - midX) < 0.30f) && (headBoyY  < umbCanopyY + 0.06f);
    int coverGirl = (absf(headGirlX - midX) < 0.30f) && (headGirlY < umbCanopyY + 0.06f);

    if (coverBoy && coverGirl)
    {
        CoverTimer += 0.016f;
        if (CoverTimer >= 2.0f)
        {
            HeartActive = 1;
        }
    }
    else
    {
        CoverTimer  = 0.0f;
        HeartActive = 0;
    }
}

void Scene2::drawPanel2()
{
    drawSky();

    drawCloud(-0.62f, 0.82f, 1.25f);
    drawCloud( 0.30f, 0.86f, 1.05f);

    drawRoad();

    drawLamp(-0.82f, -0.10f, 1.10f);
    drawLamp( 0.82f, -0.10f, 1.10f);

    drawPuddle(0.16f, -0.83f);

    drawRain();

    float anchorY = -0.62f;
    glPushMatrix();
        glTranslatef(0.0f, anchorY, 0.0f);
        glScalef(PeopleScale, PeopleScale, 1.0f);
        glTranslatef(0.0f, -anchorY, 0.0f);


        drawHumanBoy(BoyX,  BoyY,  1.15f);
        drawHumanGirl(GirlX, GirlY, 1.15f);


        float midX = (BoyX + GirlX) * 0.5f + UmbrellaX;
        drawUmbrella(midX, UmbrellaY, 1.20f);


        if (HeartActive == 1)
        {
            drawHeartGlow(midX, UmbrellaY + 0.40f, 1.10f);
        }

    glPopMatrix();
}

void Scene2::draw()
{
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, w, h);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawPanel2();
}

void Scene2::update()
{

    RainOffset += RainSpeed;
    if (RainOffset > 2.30f)
    {
        RainOffset = 0.0f;
    }


    float target = (UmbrellaOpen == 1) ? 1.0f : 0.0f;
    UmbrellaT = UmbrellaT + (target - UmbrellaT) * 0.14f;


    checkUmbrellaCover();

    if (HeartActive == 1)
    {
        HeartPulseT += 0.12f;
    }
    else
    {
        HeartPulseT = 0.0f;
    }

    glutPostRedisplay();
}

void Scene2::keyDown(unsigned char key)
{
    if (key == 27 || key == 'q' || key == 'Q')
    {
        std::exit(0);
    }

    if (key == 'u' || key == 'U')
    {
        UmbrellaOpen = (UmbrellaOpen == 1) ? 0 : 1;
    }

    if (key == 'w' || key == 'W')
    {
        PeopleScale -= ScaleStep;
        if (PeopleScale < 0.70f)
        {
            PeopleScale = 0.70f;
        }
    }

    if (key == 'x' || key == 'X')
    {
        if (MeetDone == 0)
        {
            BoyX  += CloseStep;
            GirlX -= CloseStep;

            float gap = absf(GirlX - BoyX);
            float meetGap = 0.20f;

            if (gap <= meetGap)
            {
                float mid = (BoyX + GirlX) * 0.5f;
                BoyX  = mid - meetGap * 0.5f;
                GirlX = mid + meetGap * 0.5f;
                MeetDone = 1;
            }
        }
    }

    if (key == 'r' || key == 'R')
    {
        BoyX = -0.22f;
        GirlX = 0.22f;
        BoyY = -0.62f;
        GirlY = -0.62f;

        PeopleScale = 1.20f;
        MeetDone = 0;

        UmbrellaOpen = 0;
        UmbrellaT = 0.0f;
        UmbrellaX = 0.0f;
        UmbrellaY = -0.40f;

        CoverTimer = 0.0f;
        HeartActive = 0;
        HeartPulseT = 0.0f;

        RainOffset = 0.0f;
    }
}

void Scene2::specialDown(int key)
{
    if (key == GLUT_KEY_UP)
    {
        UmbrellaY += 0.03f;
        if (UmbrellaY > -0.20f)
        {
            UmbrellaY = -0.20f;
        }
    }

    if (key == GLUT_KEY_DOWN)
    {
        UmbrellaY -= 0.03f;
        if (UmbrellaY < -0.55f)
        {
            UmbrellaY = -0.55f;
        }
    }
}

