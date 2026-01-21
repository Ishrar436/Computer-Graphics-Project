#include "Scene5.h"
#include <cmath>
#include <algorithm>

// ---------------- small utils ----------------
float Scene5::clamp01(float v) { return std::max(0.0f, std::min(1.0f, v)); }
//float Scene5::lerp(float a, float b, float t){ return a + (b - a) * t; }

void Scene5::setColor(float r, float g, float b){ glColor3f(r,g,b); }

void Scene5::drawRect(float x1, float y1, float x2, float y2){
    glBegin(GL_QUADS);
    glVertex2f(x1,y1); glVertex2f(x2,y1);
    glVertex2f(x2,y2); glVertex2f(x1,y2);
    glEnd();
}

void Scene5::drawLine(float x1,float y1,float x2,float y2,float w){
    glLineWidth(w);
    glBegin(GL_LINES);
    glVertex2f(x1,y1); glVertex2f(x2,y2);
    glEnd();
    glLineWidth(1.0f);
}

void Scene5::drawCircle(float cx,float cy,float r,int seg){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx,cy);
    for(int i=0;i<=seg;i++){
        float a = (float)i/(float)seg * 2.0f * 3.1415926f;
        glVertex2f(cx + std::cos(a)*r, cy + std::sin(a)*r);
    }
    glEnd();
}

void Scene5::drawEllipse(float cx,float cy,float rx,float ry,float r,float g,float b){
    setColor(r,g,b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx,cy);
    for(int i=0;i<=60;i++){
        float a = (float)i/60.0f * 2.0f * 3.1415926f;
        glVertex2f(cx + std::cos(a)*rx, cy + std::sin(a)*ry);
    }
    glEnd();
}

void Scene5::drawQuad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,float r,float g,float b){
    setColor(r,g,b);
    glBegin(GL_QUADS);
    glVertex2f(x1,y1); glVertex2f(x2,y2);
    glVertex2f(x3,y3); glVertex2f(x4,y4);
    glEnd();
}

void Scene5::drawRectangle(float x1,float y1,float x2,float y2,float r,float g,float b){
    setColor(r,g,b);
    drawRect(x1,y1,x2,y2);
}
void Scene5::reset(){
    // positions
    ourBoyX = -1.25f;
    girlX   =  1.20f;
    otherX  =  1.38f;

    groundY = -0.62f;

    // walking anim time
    walkT = 0.0f;
    cloudT = 0.0f;

    // story flags (no enum)
    crossed = false;
    turningBack = false;
    kneeling = false;

    turnAngleDeg = 0.0f;

    // car
    carVisible = true;
    carMoving = false;
    carX = -0.05f;       // front of him from beginning
    carY = -0.78f;

    boyGoingToCar = false;
    boyHidden = false;
}
// ---------------- Scene5 core ----------------
void Scene5::initGL(){
    glClearColor(1,1,1,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    reset();
}



void Scene5::keyDown(unsigned char k){
    if(k=='r' || k=='R'){ reset(); return; }

    // press E after kneel -> get up + go to car
    if(k=='e' || k=='E'){
        if(kneeling && !boyHidden && !boyGoingToCar && !carMoving){
            kneeling = false;
            boyGoingToCar = true;

            // face right again
            turnAngleDeg = 0.0f;
        }
        return;
    }
}

//void Scene5::specialDown(int){}

void Scene5::update(float dt){
    if(dt <= 0) return;

    walkT  += dt;
    cloudT += dt;

    float boySpeed  = 0.35f;
    float girlSpeed = 0.30f;
    float carSpeed  = 0.75f;

    // 1) walking until cross
    if(!crossed){
        ourBoyX += boySpeed * dt;
        girlX   -= girlSpeed * dt;
        otherX  -= girlSpeed * dt;

        if(girlX <= ourBoyX){
            crossed = true;

            // boy stops + turning starts
            turningBack = true;
            kneeling = false;

            turnAngleDeg = 0.0f;
        }
        return;
    }

    // 2) girl + other keep walking away forever
    girlX  -= girlSpeed * dt;
    otherX -= girlSpeed * dt;

    // 3) turning back animation (0 -> 180)
    if(turningBack){
        turnAngleDeg += 260.0f * dt;
        if(turnAngleDeg >= 180.0f){
            turnAngleDeg = 180.0f;
            turningBack = false;
            kneeling = true;
        }
        return;
    }

    // 4) kneel until E
    if(kneeling){
        return;
    }

    // 5) E pressed: boy goes to car door -> disappears -> car moves
    if(boyGoingToCar && carVisible && !carMoving){
        float doorX = carX + 0.08f;
        ourBoyX += 0.45f * dt;

        if(ourBoyX >= doorX){
            boyGoingToCar = false;
            boyHidden = true;
            carMoving = true;
        }
    }

    // 6) car moves out
    if(carVisible && carMoving){
        carX += carSpeed * dt;
        if(carX > 1.80f){
            carVisible = false;
            carMoving  = false;
        }
    }
}

// ---------------- background + bridge ----------------
void Scene5::drawSky(){
    // Beautiful gradient sky
    glBegin(GL_QUADS);
    glColor3f(0.55f, 0.75f, 0.95f); glVertex2f(-1.0f, 1.0f);
    glColor3f(0.55f, 0.75f, 0.95f); glVertex2f( 1.0f, 1.0f);
    glColor3f(0.90f, 0.95f, 1.00f); glVertex2f( 1.0f, 0.10f);
    glColor3f(0.90f, 0.95f, 1.00f); glVertex2f(-1.0f, 0.10f);
    glEnd();

    // sun
    setColor(1.0f, 0.95f, 0.30f);
    drawCircle(0.0f, 0.78f, 0.08f, 70);

    // moving clouds
    float cx1 = -0.8f + std::fmod(cloudT * 0.10f, 2.2f);
    float cx2 = -0.2f + std::fmod(cloudT * 0.08f, 2.2f);
    float cx3 =  0.5f + std::fmod(cloudT * 0.06f, 2.2f);

    setColor(1,1,1);
    drawCircle(cx1, 0.72f, 0.06f, 40);
    drawCircle(cx1+0.06f, 0.72f, 0.05f, 40);
    drawCircle(cx1+0.12f, 0.72f, 0.06f, 40);

    drawCircle(cx2, 0.60f, 0.05f, 40);
    drawCircle(cx2+0.06f, 0.60f, 0.04f, 40);
    drawCircle(cx2+0.12f, 0.60f, 0.05f, 40);

    drawCircle(cx3, 0.67f, 0.06f, 40);
    drawCircle(cx3+0.06f, 0.67f, 0.05f, 40);
    drawCircle(cx3+0.12f, 0.67f, 0.06f, 40);
}

void Scene5::drawBridge(){
    drawSky();

    // river
    setColor(0.60f, 0.75f, 0.85f);
    drawRect(-1.0f, -0.20f, 1.0f, 0.10f);

    // boardwalk floor
    setColor(0.78f, 0.68f, 0.55f);
    drawRect(-1.0f, -1.0f, 1.0f, -0.55f);

    // plank lines
    setColor(0.65f, 0.56f, 0.46f);
    for(int i=0;i<26;i++){
        float x = -1.0f + i*(2.0f/26.0f);
        drawRect(x, -1.0f, x+0.002f, -0.55f);
    }

    // railing top
    setColor(0.75f, 0.12f, 0.15f);
    drawRect(-1.0f, -0.33f, 1.0f, -0.31f);

    // posts
    setColor(0.72f, 0.72f, 0.74f);
    for(int i=0;i<7;i++){
        float px = -0.95f + i*0.32f;
        drawRect(px, -0.55f, px+0.03f, -0.31f);
    }

    // mesh panels
    setColor(0.86f,0.86f,0.88f);
    for(int i=0;i<6;i++){
        float x1 = -0.92f + i*0.32f;
        float x2 = x1 + 0.26f;
        drawRect(x1, -0.52f, x2, -0.36f);

        setColor(0.78f,0.78f,0.80f);
        for(int k=0;k<7;k++){
            float yy = -0.52f + k*(0.16f/7.0f);
            drawRect(x1, yy, x2, yy+0.0015f);
        }
        setColor(0.86f,0.86f,0.88f);
    }

    // left tower
    setColor(0.62f, 0.58f, 0.55f);
    drawRect(-0.98f, -0.10f, -0.84f, 0.55f);

    setColor(0.55f, 0.52f, 0.50f);
    drawRect(-0.985f, 0.55f, -0.835f, 0.60f);

    // bridge deck (diagonal)
    setColor(0.20f, 0.20f, 0.22f);
    glBegin(GL_QUADS);
    glVertex2f(-0.84f, 0.22f);
    glVertex2f(-0.10f, 0.07f);
    glVertex2f(-0.08f, 0.11f);
    glVertex2f(-0.84f, 0.26f);
    glEnd();

    // cables
    setColor(0.25f, 0.25f, 0.27f);
    drawLine(-0.90f, 0.56f, -0.05f, 0.20f, 2.5f);
    drawLine(-0.90f, 0.52f, -0.05f, 0.17f, 2.5f);

    // hangers
    setColor(0.35f, 0.35f, 0.38f);
    for(int i=0;i<10;i++){
        float t = i / 9.0f;
        float cx = -0.80f + t*(0.70f);
        float cy = 0.50f + (0.18f-0.50f)*t;
        float dy = 0.24f + (0.10f-0.24f)*t;
        drawLine(cx, cy, cx, dy, 1.5f);
    }
}

// ---------------- PERSONS (Scene1 boy style) ----------------
void Scene5::drawHumanBoySwing(float x, float y, float s, float r, float g, float b, bool walking)
{
    float swing = walking ? std::sin(walkT*10.0f) * 0.020f*s : 0.0f;

    // head + hair
    drawEllipse(x, y + 0.22f*s, 0.06f*s, 0.08f*s, 0.93f, 0.78f, 0.65f);
    drawEllipse(x, y + 0.26f*s, 0.065f*s, 0.045f*s, 0.10f, 0.10f, 0.10f);

    // shirt
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

void Scene5::drawGirlSwing(float x, float y, float s, bool walking)
{
    float swing = walking ? std::sin(walkT*10.0f) * 0.020f*s : 0.0f;

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
    drawRectangle(x - 0.030f*s, y - 0.22f*s, x - 0.005f*s, y - 0.10f*s, 0.93f, 0.78f, 0.65f);
    drawRectangle(x + 0.005f*s, y - 0.22f*s, x + 0.030f*s, y - 0.10f*s, 0.93f, 0.78f, 0.65f);

    // shoes
    drawRectangle(x - 0.035f*s, y - 0.24f*s, x - 0.000f*s, y - 0.22f*s, 0.08f, 0.05f, 0.05f);
    drawRectangle(x + 0.000f*s, y - 0.24f*s, x + 0.035f*s, y - 0.22f*s, 0.08f, 0.05f, 0.05f);
}

// ---------------- CAR ----------------
void Scene5::drawCar(float x, float y){
    if(!carVisible) return;

    glPushMatrix();
    glTranslatef(x, y, 0);

    // shadow
    setColor(0.0f,0.0f,0.0f);
    drawEllipse(0.0f, -0.02f, 0.22f, 0.05f, 0.0f,0.0f,0.0f);

    // body
    setColor(0.12f, 0.55f, 0.95f);
    drawRect(-0.25f, 0.00f, 0.25f, 0.12f);

    // top
    setColor(0.10f, 0.45f, 0.85f);
    glBegin(GL_QUADS);
    glVertex2f(-0.12f, 0.12f);
    glVertex2f( 0.10f, 0.12f);
    glVertex2f( 0.05f, 0.20f);
    glVertex2f(-0.08f, 0.20f);
    glEnd();

    // windows
    setColor(0.75f, 0.90f, 1.0f);
    drawRect(-0.09f, 0.13f, 0.02f, 0.18f);
    drawRect(0.03f, 0.13f, 0.10f, 0.18f);

    // door line
    setColor(0.05f, 0.15f, 0.25f);
    drawLine(0.02f, 0.01f, 0.02f, 0.12f, 2.0f);

    // handle
    setColor(0.95f,0.95f,0.95f);
    drawRect(0.05f, 0.07f, 0.09f, 0.075f);

    // wheels
    setColor(0.05f,0.05f,0.05f);
    drawCircle(-0.16f, 0.00f, 0.05f, 30);
    drawCircle( 0.16f, 0.00f, 0.05f, 30);

    setColor(0.70f,0.70f,0.70f);
    drawCircle(-0.16f, 0.00f, 0.025f, 20);
    drawCircle( 0.16f, 0.00f, 0.025f, 20);

    // headlight
    setColor(1.0f, 0.95f, 0.60f);
    drawCircle(0.25f, 0.07f, 0.018f, 20);

    glPopMatrix();
}

// ---------------- draw scene ----------------
void Scene5::draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawBridge();

    // car always present
    drawCar(carX, carY);

    // draw people
    if(!boyHidden){
        // Our boy with rotate + kneel
        glPushMatrix();

        // pivot rotate around body center
        float pivotY = groundY + 0.12f;
        glTranslatef(ourBoyX, pivotY, 0);


        // kneel drop
        float kneelDrop = kneeling ? 0.09f : 0.0f;

        bool walkingBoy = (!crossed) || boyGoingToCar;
        drawHumanBoySwing(ourBoyX, 0.15f - kneelDrop, 0.70f, 0.18f, 0.35f, 0.70f, walkingBoy);

        glPopMatrix();
    }

    // girl + other boy
    bool girlWalking = true;
    drawGirlSwing(girlX, groundY, 0.70f, girlWalking);

    // other boy same style but GREEN shirt
    drawHumanBoySwing(otherX, groundY, 0.70f, 0.10f, 0.55f, 0.20f, true);
}
