#include "Scene5.h"
#include <cmath>
#include <algorithm>

// --------------------- small helpers ---------------------
static float lerp(float a, float b, float t) { return a + (b - a) * t; }

float Scene5::clamp01(float v) { return std::max(0.0f, std::min(1.0f, v)); }

void Scene5::setColor(float r, float g, float b) { glColor3f(r, g, b); }

void Scene5::drawTriangle(float x1,float y1,float x2,float y2,float x3,float y3){
    glBegin(GL_TRIANGLES);
    glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3);
    glEnd();
}

void Scene5::drawRect(float x1,float y1,float x2,float y2){
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
void Scene5::drawQuad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,
                      float r,float g,float b)
{
    setColor(r,g,b);
    glBegin(GL_QUADS);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glVertex2f(x3,y3);
    glVertex2f(x4,y4);
    glEnd();
}
void Scene5::drawCircle(float cx, float cy, float r, int seg){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i=0;i<=seg;i++){
        float a = (float)i/(float)seg * 2.0f * 3.1415926f;
        glVertex2f(cx + std::cos(a)*r, cy + std::sin(a)*r);
    }
    glEnd();
}
void Scene5::reset(){
    // start positions (outside screen)
    ourBoyX = -1.25f;
    ourBoyY = -0.55f;     // ground = top of boardwalk
    girlX   =  1.25f;
    girlY   = -0.55f;
    otherX  =  1.38f;     // a bit behind girl
    otherY  = -0.55f;

    walkT = 0.0f;

    // simple flags (NO ENUM)
    started     = true;   // auto-run
    crossed     = false;
    boyStop     = false;
    boyLookBack = false;
    boyKneel    = false;

    afterCrossT = 0.0f;
}

// --------------------- Scene5 state ---------------------
Scene5::Scene5() {
    reset();
}

void Scene5::initGL() {
    glClearColor(1,1,1,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    reset();
}


void Scene5::keyDown(unsigned char k){
    if(k=='r' || k=='R') reset();
    // if you want manual start:
    // if(k=='s' || k=='S') started = true;
}

void Scene5::specialDown(int key){
    (void)key;
}

// --------------------- Update ---------------------
void Scene5::update(float dt){
    if(dt <= 0) return;
    walkT += dt;

    if(!started) return;

    float boySpeed   = 0.35f;
    float girlSpeed  = 0.30f;

    // move until cross
    if(!crossed){
        ourBoyX += boySpeed * dt;     // boy goes right
        girlX   -= girlSpeed * dt;    // girl goes left
        otherX  -= girlSpeed * dt;    // other boy goes left

        // detect crossing (girl passes boy)
        if(girlX <= ourBoyX){
            crossed = true;
            boyStop = true;
            boyLookBack = true;
            afterCrossT = 0.0f;
        }
    }
    else{
        // girl + other continue leaving left side
        girlX  -= girlSpeed * dt;
        otherX -= girlSpeed * dt;

        // boy stops, then kneels after short delay
        if(boyStop){
            afterCrossT += dt;

            // keep boy fixed while stopping
            // (no movement)

            if(afterCrossT > 0.70f){
                boyKneel = true;   // final pose
                // end state: keep everything
            }
        }
    }
}

// --------------------- Draw persons ---------------------

// OUR BOY
void Scene5::drawOurBoy(float x, float groundY, float s){
    // groundY is the floor top line (-0.55)

    // walking leg swing (only if not kneel)
    float swing = 0.0f;
    if(!boyKneel && !boyStop) swing = std::sin(walkT*10.0f) * 0.04f*s;

    // if kneeling: lower body + bend legs
    float bodyDrop = boyKneel ? -0.06f*s : 0.0f;

    // head
    setColor(0.93f, 0.78f, 0.65f);
    drawCircle(x, groundY + (0.26f*s) + bodyDrop, 0.06f*s, 40);

    // hair
    setColor(0.10f, 0.10f, 0.10f);
    drawCircle(x, groundY + (0.285f*s) + bodyDrop, 0.065f*s, 40);

    // shirt
    setColor(0.18f, 0.35f, 0.70f);
    drawRect(x - 0.07f*s, groundY + (0.12f*s) + bodyDrop, x + 0.07f*s, groundY + (0.22f*s) + bodyDrop);

    // arms (simple)
    setColor(0.18f, 0.35f, 0.70f);
    drawRect(x - 0.10f*s, groundY + (0.15f*s) + bodyDrop, x - 0.07f*s, groundY + (0.20f*s) + bodyDrop);
    drawRect(x + 0.07f*s, groundY + (0.15f*s) + bodyDrop, x + 0.10f*s, groundY + (0.20f*s) + bodyDrop);

    setColor(0.93f, 0.78f, 0.65f);
    drawRect(x - 0.10f*s, groundY + (0.13f*s) + bodyDrop, x - 0.06f*s, groundY + (0.16f*s) + bodyDrop);
    drawRect(x + 0.06f*s, groundY + (0.13f*s) + bodyDrop, x + 0.10f*s, groundY + (0.16f*s) + bodyDrop);

    // pants
    setColor(0.12f, 0.12f, 0.16f);
    drawRect(x - 0.06f*s, groundY + (0.02f*s) + bodyDrop, x + 0.06f*s, groundY + (0.12f*s) + bodyDrop);

    if(!boyKneel){
        // legs (walking)
        setColor(0.12f, 0.12f, 0.16f);
        drawRect(x - 0.055f*s, groundY - (0.12f*s) + swing, x - 0.015f*s, groundY + (0.02f*s) + bodyDrop);
        drawRect(x + 0.015f*s, groundY - (0.12f*s) - swing, x + 0.055f*s, groundY + (0.02f*s) + bodyDrop);

        // shoes
        setColor(0.05f,0.05f,0.05f);
        drawRect(x - 0.060f*s, groundY - (0.14f*s) + swing, x - 0.005f*s, groundY - (0.12f*s) + swing);
        drawRect(x + 0.005f*s, groundY - (0.14f*s) - swing, x + 0.060f*s, groundY - (0.12f*s) - swing);
    }else{
        // kneeling pose: one leg folded, one knee down
        setColor(0.12f, 0.12f, 0.16f);
        // standing leg
        drawRect(x - 0.055f*s, groundY - (0.10f*s), x - 0.015f*s, groundY + (0.02f*s) + bodyDrop);
        // folded leg
        drawRect(x + 0.00f*s, groundY - (0.06f*s), x + 0.08f*s, groundY - (0.02f*s));

        setColor(0.05f,0.05f,0.05f);
        drawRect(x - 0.060f*s, groundY - (0.12f*s), x - 0.005f*s, groundY - (0.10f*s));
        drawRect(x + 0.04f*s, groundY - (0.08f*s), x + 0.10f*s, groundY - (0.06f*s));
    }

    // look back: small nose line to the LEFT
    if(boyLookBack){
        setColor(0.10f,0.10f,0.10f);
        drawLine(x, groundY + (0.26f*s) + bodyDrop, x - 0.03f*s, groundY + (0.25f*s) + bodyDrop, 2.0f);
    }
}

void Scene5::drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b)
{
    setColor(r,g,b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i=0;i<=60;i++){
        float a = (float)i/60.0f * 2.0f * 3.1415926f;
        glVertex2f(cx + std::cos(a)*rx, cy + std::sin(a)*ry);
    }
    glEnd();
}
// GIRL
// SAME PARAMETERS: (x, y, s)
// Makes it look like your "second" version: nicer skirt + center crease line
void Scene5::drawGirl(float x, float groundY, float s)
{
    float baseY = groundY + 0.24f*s;

    drawEllipse(x, baseY + 0.22f*s, 0.06f*s, 0.08f*s, 0.93f, 0.78f, 0.65f);

    drawEllipse(x, baseY + 0.24f*s, 0.070f*s, 0.055f*s, 0.10f, 0.07f, 0.06f);

    setColor(0.10f, 0.07f, 0.06f);
    drawRect(x - 0.070f*s, baseY + 0.12f*s, x - 0.030f*s, baseY + 0.24f*s);
    drawRect(x + 0.030f*s, baseY + 0.12f*s, x + 0.070f*s, baseY + 0.24f*s);

    // top
    setColor(0.72f, 0.18f, 0.35f);
    drawRect(x - 0.065f*s, baseY + 0.10f*s, x + 0.065f*s, baseY + 0.18f*s);

    // skirt
    drawQuad(x - 0.09f*s, baseY + 0.10f*s,
             x + 0.09f*s, baseY + 0.10f*s,
             x + 0.06f*s, baseY - 0.10f*s,
             x - 0.06f*s, baseY - 0.10f*s,
             0.70f, 0.16f, 0.33f);

    // center fold line (nice look)
    glLineWidth(2.0f);
    setColor(0.55f, 0.10f, 0.25f);
    glBegin(GL_LINES);
        glVertex2f(x, baseY + 0.10f*s);
        glVertex2f(x, baseY - 0.10f*s);
    glEnd();
    glLineWidth(1.0f);

    // arms
    setColor(0.93f, 0.78f, 0.65f);
    drawRect(x - 0.10f*s, baseY + 0.10f*s, x - 0.06f*s, baseY + 0.14f*s);
    drawRect(x + 0.06f*s, baseY + 0.10f*s, x + 0.10f*s, baseY + 0.14f*s);

    // legs
    setColor(0.93f, 0.78f, 0.65f);
    drawRect(x - 0.030f*s, baseY - 0.22f*s, x - 0.005f*s, baseY - 0.10f*s);
    drawRect(x + 0.005f*s, baseY - 0.22f*s, x + 0.030f*s, baseY - 0.10f*s);

    // shoes
    setColor(0.08f, 0.05f, 0.05f);
    drawRect(x - 0.035f*s, baseY - 0.24f*s, x - 0.000f*s, baseY - 0.22f*s);
    drawRect(x + 0.000f*s, baseY - 0.24f*s, x + 0.035f*s, baseY - 0.22f*s);
}



// OTHER BOY (with her)
void Scene5::drawOtherBoy(float x, float groundY, float s){
    float swing = std::sin(walkT*10.0f) * 0.03f*s;

    // head
    setColor(0.93f, 0.78f, 0.65f);
    drawCircle(x, groundY + 0.26f*s, 0.055f*s, 40);

    // hair
    setColor(0.05f,0.05f,0.05f);
    drawCircle(x, groundY + 0.285f*s, 0.060f*s, 40);

    // shirt
    setColor(0.20f, 0.55f, 0.30f);
    drawRect(x - 0.06f*s, groundY + 0.12f*s, x + 0.06f*s, groundY + 0.22f*s);

    // pants
    setColor(0.12f,0.12f,0.16f);
    drawRect(x - 0.05f*s, groundY + 0.02f*s, x + 0.05f*s, groundY + 0.12f*s);

    // legs
    setColor(0.10f,0.10f,0.10f);
    drawLine(x - 0.02f*s, groundY + 0.02f*s, x - 0.03f*s, groundY - 0.10f*s + swing, 3.0f);
    drawLine(x + 0.02f*s, groundY + 0.02f*s, x + 0.03f*s, groundY - 0.10f*s - swing, 3.0f);
}

// --------------------- Bridge background (YOUR CODE) ---------------------
void Scene5::drawBridge()
{
    // SKY
    setColor(0.92f, 0.92f, 0.95f);
    drawRect(-1.0f, 0.15f, 1.0f, 1.0f);

    // RIVER (horizon line around y=0.12)
    setColor(0.75f, 0.80f, 0.85f);
    drawRect(-1.0f, -0.15f, 1.0f, 0.15f);

    // DISTANT CITY SKYLINE (simple blocks)
    setColor(0.70f, 0.72f, 0.76f);
    for(int i=0;i<12;i++){
        float bx = -0.20f + i*0.10f;
        float h  = 0.06f + (i%4)*0.03f;
        drawRect(bx, 0.10f, bx+0.06f, 0.10f+h);
    }
    setColor(0.62f, 0.64f, 0.68f);
    drawRect(0.35f, 0.10f, 0.46f, 0.22f);
    drawRect(0.52f, 0.10f, 0.60f, 0.18f);

    // BOARDWALK FLOOR (wood planks)
    setColor(0.70f, 0.63f, 0.52f);
    drawRect(-1.0f, -1.0f, 1.0f, -0.55f);

    // plank lines
    setColor(0.60f, 0.52f, 0.42f);
    for(int i=0;i<22;i++){
        float x = -1.0f + i*(2.0f/22.0f);
        drawRect(x, -1.0f, x+0.002f, -0.55f);
    }

    // RAILING POSTS + MESH (like photo)
    // top red rail
    setColor(0.70f, 0.10f, 0.12f);
    drawRect(-1.0f, -0.33f, 1.0f, -0.31f);

    // metal posts
    setColor(0.65f, 0.65f, 0.68f);
    for(int i=0;i<7;i++){
        float px = -0.90f + i*0.30f;
        drawRect(px, -0.55f, px+0.03f, -0.31f);
    }

    // mesh panels
    setColor(0.78f, 0.78f, 0.80f);
    for(int i=0;i<6;i++){
        float x1 = -0.87f + i*0.30f;
        float x2 = x1 + 0.24f;
        drawRect(x1, -0.52f, x2, -0.36f);

        // mesh lines
        setColor(0.70f, 0.70f, 0.72f);
        for(int k=0;k<7;k++){
            float yy = -0.52f + k*(0.16f/7.0f);
            drawRect(x1, yy, x2, yy+0.0015f);
        }
        setColor(0.78f, 0.78f, 0.80f);
    }

    // --------- BRIDGE: tower on left ----------
    setColor(0.62f, 0.58f, 0.55f);
    drawRect(-0.98f, -0.10f, -0.84f, 0.55f);

    setColor(0.70f, 0.66f, 0.62f);
    drawRect(-0.96f, 0.05f, -0.86f, 0.12f);
    drawRect(-0.96f, 0.28f, -0.86f, 0.35f);

    setColor(0.55f, 0.52f, 0.50f);
    drawRect(-0.985f, 0.55f, -0.835f, 0.60f);

    // --------- BRIDGE DECK ----------
    setColor(0.20f, 0.20f, 0.22f);
    glBegin(GL_QUADS);
    glVertex2f(-0.84f, 0.22f);
    glVertex2f(-0.10f, 0.07f);
    glVertex2f(-0.08f, 0.11f);
    glVertex2f(-0.84f, 0.26f);
    glEnd();

    setColor(0.30f, 0.30f, 0.32f);
    drawLine(-0.84f, 0.26f, -0.08f, 0.11f, 2.0f);

    // --------- MAIN CABLES ----------
    setColor(0.25f, 0.25f, 0.27f);
    drawLine(-0.90f, 0.56f, -0.05f, 0.20f, 2.5f);
    drawLine(-0.90f, 0.52f, -0.05f, 0.17f, 2.5f);

    // --------- HANGERS ----------
    setColor(0.35f, 0.35f, 0.38f);
    for(int i=0;i<10;i++){
        float t = i / 9.0f;
        float cx = -0.80f + t*(0.70f);
        float cy = 0.50f + (0.18f-0.50f)*t;
        float dy = 0.24f + (0.10f-0.24f)*t;
        drawLine(cx, cy, cx, dy, 1.5f);
    }

    // --------- PIER POST ----------
    setColor(0.45f, 0.42f, 0.40f);
    drawRect(0.68f, -0.15f, 0.74f, 0.06f);
    setColor(0.38f, 0.36f, 0.34f);
    drawRect(0.675f, 0.06f, 0.745f, 0.10f);

    setColor(0.60f, 0.65f, 0.70f);
    drawRect(-1.0f, 0.148f, 1.0f, 0.152f);
}

// --------------------- draw ---------------------
void Scene5::draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawBridge();

    // draw characters on ground
    // (ground = -0.55, bodies are drawn above this)
    drawOurBoy(ourBoyX, ourBoyY, 1.0f);
    drawGirl(girlX, girlY, 1.0f);
    drawOtherBoy(otherX, otherY, 1.0f);


}

