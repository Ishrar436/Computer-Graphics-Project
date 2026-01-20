#include "Scene4.h"
#include <cmath>

Scene4::Scene4()
{
    StarTimer = 0.0f;
    ManBreatheT = 0.0f;
    LetterLook = 0;
    LetterOffset = 0.0f;

    ShootingStarActive = 0;
    ShootingStarX = -1.2f;
    ShootingStarY = 0.8f;

    MoonX = -0.7f;
    MoonY = 0.6f;
    SkyT = 0.0f;
    MoonSetting = 0;
}

void Scene4::initGL()
{
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Scene4::setColor(float r, float g, float b) { glColor3f(r, g, b); }

void Scene4::drawRectangle(float x1,float y1,float x2,float y2,
                           float r,float g,float b)
{
    setColor(r,g,b);
    glBegin(GL_QUADS);
        glVertex2f(x1,y1); glVertex2f(x2,y1);
        glVertex2f(x2,y2); glVertex2f(x1,y2);
    glEnd();
}

void Scene4::drawEllipse(float cx,float cy,float rx,float ry,
                         float r,float g,float b)
{
    setColor(r,g,b);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx,cy);
        for(int i=0;i<=40;i++){
            float a=2.0f*3.14159f*i/40.0f;
            glVertex2f(cx+std::cos(a)*rx,
                       cy+std::sin(a)*ry);
        }
    glEnd();
}

void Scene4::drawSky()
{
    // OLD VALUES: (0.59f*SkyT) made it light blue.
    // NEW VALUES: Kept dark even as SkyT changes.
    float r1 = 0.01f;
    float g1 = 0.01f;
    float b1 = 0.05f + (0.15f * SkyT); // Only allow a tiny bit of deep blue

    float r2 = 0.02f;
    float g2 = 0.02f;
    float b2 = 0.10f + (0.10f * SkyT); // Slightly brightens the top, but stays night

    glBegin(GL_QUADS);
        glColor3f(r1, g1, b1); glVertex2f(-1, -1);
        glColor3f(r1, g1, b1); glVertex2f( 1, -1);
        glColor3f(r2, g2, b2); glVertex2f( 1, 1);
        glColor3f(r2, g2, b2); glVertex2f(-1, 1);
    glEnd();

    // KEEP STARS ACTIVE: Change starAlpha so they don't fade away
    float starAlpha = 1.0f - SkyT;     // stars fade as SkyT increases
    if(starAlpha < 0.0f) starAlpha = 0.0f;

    for(int i = 0; i < 15; i++){
        float flicker = 0.002f * std::sin(StarTimer + i);
        drawEllipse(-0.8f + i * 0.13f, 0.5f + std::sin(i) * 0.3f,
                    0.004f + flicker, 0.004f + flicker,
                    0.9f * starAlpha, 0.9f * starAlpha, 1.0f * starAlpha);
    }

    if(MoonY > -0.6f){
        drawEllipse(MoonX, MoonY, 0.12f, 0.12f, 0.95f, 0.95f, 0.8f);
        drawEllipse(MoonX - 0.05f, MoonY + 0.03f, 0.11f, 0.11f, r2, g2, b2);
    }
}

void Scene4::drawSkyline()
{
    float bColor=0.02f+(0.1f*SkyT);

    float dayR=0.15f*SkyT;
    float dayG=0.18f*SkyT;
    float dayB=0.22f*SkyT;

    drawRectangle(-1.0f,-0.4f,-0.7f,0.2f,
        bColor+dayR,bColor+dayG,bColor+dayB);

    drawRectangle(-0.7f,-0.4f,-0.4f,0.4f,
        bColor+dayR+0.02f,bColor+dayG,bColor+dayB);

    drawRectangle(0.2f,-0.4f,0.5f,0.3f,
        bColor+dayR,bColor+dayG+0.02f,bColor+dayB+0.03f);

    // -------- WINDOWS (YELLOW AT NIGHT → BLUE AT DAY) --------
    float blueR = 0.3f * SkyT;
    float blueG = 0.5f * SkyT;
    float blueB = 1.0f * SkyT;

    float nightR = 0.9f * (1.0f - SkyT);
    float nightG = 0.8f * (1.0f - SkyT);
    float nightB = 0.3f * (1.0f - SkyT);

    float winR = nightR + blueR;
    float winG = nightG + blueG;
    float winB = nightB + blueB;

    for(float x=-0.95f;x<-0.75f;x+=0.07f)
        for(float y=0.15f;y>-0.3f;y-=0.12f)
            drawRectangle(x,y,x+0.03f,y+0.04f,winR,winG,winB);

    for(float x=-0.65f;x<-0.45f;x+=0.07f)
        for(float y=0.35f;y>-0.3f;y-=0.12f)
            drawRectangle(x,y,x+0.03f,y+0.04f,winR,winG,winB);

    for(float x=0.25f;x<0.45f;x+=0.07f)
        for(float y=0.25f;y>-0.3f;y-=0.12f)
            drawRectangle(x,y,x+0.03f,y+0.04f,winR,winG,winB);
}

void Scene4::drawStreet()
{
    drawRectangle(-1,-1,1,-0.4f,0.05f,0.05f,0.07f);

    float bx=-0.3f,by=-0.65f;
    drawRectangle(bx,by,bx+0.6f,by+0.04f,0.25f,0.15f,0.1f);
    drawRectangle(bx,by+0.04f,bx+0.6f,by+0.2f,0.2f,0.12f,0.08f);
    drawRectangle(bx+0.05f,by-0.1f,bx+0.08f,by,0.1f,0.1f,0.1f);
    drawRectangle(bx+0.52f,by-0.1f,bx+0.55f,by,0.1f,0.1f,0.1f);

    drawRectangle(0.5f,-0.4f,0.53f,0.4f,0.15f,0.15f,0.18f);
    drawRectangle(0.45f,0.4f,0.65f,0.45f,0.2f,0.2f,0.2f);
    drawEllipse(0.55f,0.38f,0.08f,0.04f,1,0.9f,0.5f);
}

void Scene4::drawManSitting()
{
    float x=0.0f,y=-0.55f;
    float breathe=std::sin(ManBreatheT)*0.005f;

    drawEllipse(x,y+0.3f+breathe,0.05f,0.06f,0.93f,0.78f,0.65f);
    drawEllipse(x+0.01f,y+0.32f+breathe,0.055f,0.04f,0.1f,0.1f,0.1f);

    drawRectangle(x-0.07f,y+0.1f,x+0.07f,y+0.25f+breathe,0.15f,0.28f,0.62f);
    drawRectangle(x-0.07f,y+0.05f,x+0.15f,y+0.12f,0.12f,0.12f,0.16f);

    float lx=x+0.08f+LetterOffset;
    float ly=y+0.18f+breathe+(LetterOffset*0.5f);
    drawRectangle(lx-0.04f,ly-0.02f,lx+0.04f,ly+0.02f,0.98f,0.98f,0.95f);
}

void Scene4::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky();
    drawSkyline();
    drawStreet();
    drawManSitting();

    if(ShootingStarActive){
        drawEllipse(ShootingStarX,ShootingStarY,0.02f,0.002f,1,1,1);
        drawEllipse(ShootingStarX-0.03f,ShootingStarY+0.015f,0.01f,0.001f,0.8f,0.8f,1);
    }
}

void Scene4::update()
{
    StarTimer+=0.05f;
    ManBreatheT+=0.03f;
    LetterOffset=LetterLook?-0.05f:0.0f;

    if(ShootingStarActive){
        ShootingStarX+=0.04f;
        ShootingStarY-=0.02f;
        if(ShootingStarX>1.2f) ShootingStarActive=0;
    }

    if(MoonSetting){
        MoonX+=0.005f;
        MoonY-=0.003f;

        if(MoonX>0.0f){
            SkyT+=0.003f;
            //if(SkyT>1.0f) SkyT=1.0f;
        }
        if(MoonX>1.5f) MoonSetting=0;
    }

    glutPostRedisplay();
}

void Scene4::keyDown(unsigned char key)
{
    if(key=='l'||key=='L') LetterLook=!LetterLook;
    if(key=='s'||key=='S'){
        ShootingStarActive=1;
        ShootingStarX=-1.1f;
        ShootingStarY=0.8f;
    }
    if(key=='m'||key=='M') MoonSetting=1;
    if(key=='r'||key=='R') LetterLook=0;
}

void Scene4::specialDown(int) {}
