// ------------------ INIT / RESET / INPUT ------------------
#include"Scene3.h"

float Scene3:: clamp01(float v) { return std::max(0.0f, std::min(1.0f, v)); }
 float Scene3::lerp(float a, float b, float t) { return a + (b - a) * t; }

void Scene3::initGL() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    reset();
}

void Scene3::reset() {
    trainX = 0.25f;
    trainMoving = false;
    trainGone = false;

    letterVisible = false;
    letterArriving = false;
    letterArrived = false;
    letterX = 1.25f;
    letterY = -0.50f;
    letterFloatT = 0.0f;

    cloud1X = -0.5f;
cloud2X = 0.2f;
cloud3X = 0.8f;

    boyX = -0.9f;
    boyY = -0.55f;
    boyMovingToLetter = false;
    boyPickedLetter = false;
    boyChasingTrain = false;
    boyMissedTrain  = false;
    runPhase = 0.0f;

    heartVisible = false;
    timeLapse = false;
    dayToNight = 0.0f;

    minuteAng = 0.0f;
    hourAng = 0.0f;

    sunY = 0.82f;

    trainDoorWorldX = 0.0f;
}
void Scene3::drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void Scene3::drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; ++i) {
        float a = (float)i / (float)segments * 2.0f * 3.1415926f;
        glVertex2f(cx + std::cos(a) * radius, cy + std::sin(a) * radius);
    }
    glEnd();
}

void Scene3::drawLine(float x1, float y1, float x2, float y2, float width) {
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1.0f);
}
void Scene3::keyDown(unsigned char k) {
    if (k == 'r' || k == 'R') {
        reset();
        return;
    }

    if (k == 't' || k == 'T') {
        // one-shot start
        if (!trainMoving && !trainGone) {
            trainMoving = true;

            // start chase immediately
            boyChasingTrain = true;
            boyMissedTrain  = false;
        }
        return;
    }

    if (k == 'l' || k == 'L') {
        // Only after letter arrives, boy goes to letter and picks it
        if (letterArrived && !boyPickedLetter && !boyMovingToLetter) {
            boyMovingToLetter = true;
        }
        return;
    }
}

// ------------------ UPDATE ------------------

void Scene3::update(float dt) {
    letterFloatT += dt;

    // compute door X in world coords (door center is local -0.125)
    trainDoorWorldX = trainX + (-0.125f);

    // Train depart
    if (trainMoving) {
        trainX += 0.62f * dt; // a bit faster

        // When train fully exits right side
        if (trainX > 2.2f) {
            trainMoving = false;
            trainGone = true;

            // stop chase
            boyChasingTrain = false;
            boyMissedTrain  = true;

            // spawn letter AFTER train gone
            letterVisible = true;
            letterArriving = true;
            letterArrived = false;
            letterX = 1.30f; // start off screen right
        }
    }

    // Boy chases train while it's moving (but slower -> misses)
    if (boyChasingTrain && !trainGone) {
        runPhase += dt * 14.0f;

        // run right
        boyX += 0.36f * dt;

        // keep a bit behind the door
        float target = trainDoorWorldX - 0.22f;
        if (boyX > target) boyX = target;

        // when train is far enough, give up
        if (trainX > 1.25f) {
            boyChasingTrain = false;
            boyMissedTrain  = true;
        }
    }

    // Letter arrives from right after train gone
    if (letterArriving) {
        letterX -= 0.52f * dt;
        if (letterX <= letterStopX) {
            letterX = letterStopX;
            letterArriving = false;
            letterArrived = true;
        }
    }

    // Boy runs to letter if L pressed
    if (boyMovingToLetter) {
        runPhase += dt * 12.0f;

        float targetX = letterX - 0.10f;

        // if boy is left of letter -> run right, else run left (safe)
        if (boyX < targetX) boyX += 0.40f * dt;
        else                boyX -= 0.40f * dt;

        if (std::fabs(boyX - targetX) < 0.01f) {
            boyX = targetX;
            boyMovingToLetter = false;
            boyPickedLetter = true;

            heartVisible = true;
            timeLapse = true;
        }
    }

    // Clock + day/night transition AFTER picking letter
    if (timeLapse) {
        minuteAng -= dt * 12.0f;
        hourAng   -= dt * 2.0f;

        dayToNight = clamp01(dayToNight + dt * 0.22f);
        sunY = lerp(0.82f, 0.30f, dayToNight);

        if (dayToNight >= 1.0f) {
            timeLapse = false;
        }
    } else {
        minuteAng -= dt * 0.15f;
        hourAng   -= dt * 0.02f;
    }
    // Move clouds slowly to the right
cloud1X += 0.05f * dt;
cloud2X += 0.03f * dt;
cloud3X += 0.07f * dt;

// Wrap clouds back to the left side when they go off-screen
if (cloud1X > 1.2f) cloud1X = -1.2f;
if (cloud2X > 1.2f) cloud2X = -1.2f;
if (cloud3X > 1.2f) cloud3X = -1.2f;
}

// ------------------ DRAW STATION (Platform updated like 2nd screenshot) ------------------



// ------------------ WOMAN IN TRAIN ------------------

void Scene3::drawWomanInTrain() {
    // draw inside a window near right side (train local coords)
    // head
    setColor(0.95f, 0.84f, 0.72f);
    drawCircle(0.00f, -0.39f, 0.03f, 30);

    // hair
    setColor(0.10f, 0.05f, 0.05f);
    drawCircle(0.00f, -0.38f, 0.032f, 30);

    // body
    setColor(0.70f, 0.10f, 0.25f);
    drawRect(-0.03f, -0.48f, 0.03f, -0.41f);
}

// ------------------ TRAIN (with passenger) ------------------

void Scene3::drawTrain() {
    if (trainGone) return;

    glPushMatrix();
    glTranslatef(trainX, 0.0f, 0.0f);

    // body
    setColor(0.85f, 0.15f, 0.15f);
    drawRect(-1.05f, -0.60f, 0.10f, -0.25f);

    // roof line
    setColor(0.15f, 0.15f, 0.16f);
    drawRect(-1.05f, -0.25f, 0.10f, -0.20f);

    // windows
    for (int i = 0; i < 7; ++i) {
        float x = -0.98f + i * 0.14f;
        setColor(0.70f, 0.82f, 0.95f);
        drawRect(x, -0.48f, x + 0.10f, -0.35f);
    }

    // door
    setColor(0.93f, 0.93f, 0.93f);
    drawRect(-0.20f, -0.58f, -0.05f, -0.30f);

    // wheels
    setColor(0.08f, 0.08f, 0.08f);
    drawCircle(-0.90f, -0.62f, 0.035f, 30);
    drawCircle(-0.50f, -0.62f, 0.035f, 30);
    drawCircle(-0.15f, -0.62f, 0.035f, 30);

    // head light
    setColor(0.95f, 0.90f, 0.70f);
    drawCircle(0.06f, -0.40f, 0.025f, 30);

    // passenger (woman) in a right-side window
    drawWomanInTrain();

    glPopMatrix();
}

// ------------------ BOY (runs both for chase + letter) ------------------

void Scene3::drawBoy() {
    // scale similar to Scene2's "s"
    float s = 1.0f;

    // running animation parameters
    float swing = 0.0f;
    float bounce = 0.0f;

    bool running = (boyMovingToLetter || boyChasingTrain);
    if (running) {
        swing  = std::sin(runPhase) * 0.05f;          // arms/legs
        bounce = std::fabs(std::sin(runPhase)) * 0.01f; // small up-down
    }

    float x = boyX;
    float y = boyY + bounce;

    // ---------------- HEAD + HAIR (no face) ----------------
    setColor(0.93f, 0.78f, 0.65f);
    drawCircle(x, y + 0.14f*s, 0.06f*s, 50);

    setColor(0.10f, 0.10f, 0.10f);
    drawCircle(x, y + 0.17f*s, 0.06f*s, 50);
    drawRect(x - 0.06f*s, y + 0.14f*s, x + 0.06f*s, y + 0.17f*s);

    // ---------------- SHIRT (torso) ----------------
    setColor(0.18f, 0.35f, 0.70f);
    drawRect(x - 0.07f*s, y + 0.02f*s, x + 0.07f*s, y + 0.14f*s);

    // sleeves
    drawRect(x - 0.10f*s, y + 0.06f*s, x - 0.07f*s, y + 0.12f*s);
    drawRect(x + 0.07f*s, y + 0.06f*s, x + 0.10f*s, y + 0.12f*s);

    // hands (move a bit with swing)
    setColor(0.93f, 0.78f, 0.65f);
    drawRect(x - 0.10f*s, (y + 0.04f*s) + swing*0.4f, x - 0.06f*s, (y + 0.08f*s) + swing*0.4f);
    drawRect(x + 0.06f*s, (y + 0.04f*s) - swing*0.4f, x + 0.10f*s, (y + 0.08f*s) - swing*0.4f);

    // ---------------- PANTS ----------------
    setColor(0.12f, 0.12f, 0.16f);
    drawRect(x - 0.06f*s, y - 0.10f*s, x + 0.06f*s, y + 0.02f*s);

    // ---------------- LEGS (alternate for running) ----------------
    // left leg forward when swing>0, right leg forward when swing<0
    float legShiftL =  swing * 0.8f;
    float legShiftR = -swing * 0.8f;

    // left leg
    drawRect((x - 0.055f*s) + legShiftL, y - 0.24f*s,
             (x - 0.015f*s) + legShiftL, y - 0.10f*s);

    // right leg
    drawRect((x + 0.015f*s) + legShiftR, y - 0.24f*s,
             (x + 0.055f*s) + legShiftR, y - 0.10f*s);

    // ---------------- SHOES ----------------
    setColor(0.05f, 0.05f, 0.05f);

    // left shoe
    drawRect((x - 0.060f*s) + legShiftL, y - 0.26f*s,
             (x - 0.005f*s) + legShiftL, y - 0.24f*s);

    // right shoe
    drawRect((x + 0.005f*s) + legShiftR, y - 0.26f*s,
             (x + 0.060f*s) + legShiftR, y - 0.24f*s);
}

void Scene3::drawStar(float cx, float cy, float s) {
    drawLine(cx - s, cy, cx + s, cy, 2.0f);
    drawLine(cx, cy - s, cx, cy + s, 2.0f);
}

void Scene3::drawBackground() {
    float t = dayToNight;

    float dayR = 0.75f, dayG = 0.86f, dayB = 0.98f;
    float nightR = 0.08f, nightG = 0.10f, nightB = 0.20f;

    setColor(lerp(dayR, nightR, t), lerp(dayG, nightG, t), lerp(dayB, nightB, t));
    drawRect(-1, 0.25f, 1, 1);

    setColor(0.55f, 0.55f, 0.55f);
    drawRect(-1, -1, 1, 0.25f);

    if (t > 0.45f) {
        float a = clamp01((t - 0.45f) / 0.55f);
        setColor(a, a, a);
        drawStar(-0.75f, 0.85f, 0.01f);
        drawStar(-0.40f, 0.92f, 0.008f);
        drawStar( 0.15f, 0.88f, 0.01f);
        drawStar( 0.55f, 0.93f, 0.008f);
        drawStar( 0.82f, 0.86f, 0.01f);
    }
}
void Scene3::drawCloud(float x, float y, float scale) {
    float t = dayToNight;
    // Clouds get slightly darker/greyer at night
    float c = lerp(1.0f, 0.6f, t);
    setColor(c, c, c);

    // Draw a cluster of circles to make it look fluffy
    drawCircle(x, y, 0.05f * scale, 30);
    drawCircle(x + 0.04f * scale, y + 0.02f * scale, 0.04f * scale, 30);
    drawCircle(x - 0.04f * scale, y + 0.01f * scale, 0.035f * scale, 30);
    drawCircle(x + 0.07f * scale, y - 0.01f * scale, 0.03f * scale, 30);
}

void Scene3::drawAllClouds() {
    drawCloud(cloud1X, 0.85f, 1.2f);
    drawCloud(cloud2X, 0.75f, 0.8f);
    drawCloud(cloud3X, 0.90f, 1.0f);
}
void Scene3::drawSun() {
    float t = dayToNight;
    float alpha = 1.0f - t;
    if (alpha <= 0.02f) return;

    float cx = 0.0f;
    float cy = sunY;

    // --- 1. Draw the "Outer Glow" (Translucent layer) ---
    // We use a larger radius and lower opacity for the atmospheric glow
    setColor(1.0f * alpha, 0.8f * alpha, 0.2f * alpha);
    drawCircle(cx, cy, 0.12f, 60); // Large faint glow

    // --- 2. Draw the "Inner Glow" ---
    setColor(1.0f * alpha, 0.9f * alpha, 0.4f * alpha);
    drawCircle(cx, cy, 0.09f, 60); // Medium glow

    // --- 3. Draw the Sun Core (Brightest part) ---
    setColor(1.0f * alpha, 1.0f * alpha, 0.8f * alpha); // Near white core
    drawCircle(cx, cy, 0.06f, 60);

    // --- 4. Draw Shiny Sunbeams (The Rays) ---
    // We will draw 8 rays that rotate slightly using 'letterFloatT' for animation
    setColor(1.0f * alpha, 0.9f * alpha, 0.2f * alpha);
    float rayRotation = letterFloatT * 0.5f; // Slow rotation

    for (int i = 0; i < 8; i++) {
        float angle = (i * 45.0f * 3.14159f / 180.0f) + rayRotation;

        // Ray start (just outside the core)
        float xStart = cx + std::cos(angle) * 0.08f;
        float yStart = cy + std::sin(angle) * 0.08f;

        // Ray end (stretching out)
        float xEnd = cx + std::cos(angle) * 0.15f;
        float yEnd = cy + std::sin(angle) * 0.15f;

        // Draw the ray as a line with a bit of width
        drawLine(xStart, yStart, xEnd, yEnd, 3.0f);
    }
}

void Scene3::drawTracks() {
    setColor(0.35f, 0.35f, 0.35f);
    drawRect(-1, -0.80f, 1, -0.62f);

    setColor(0.10f, 0.10f, 0.10f);
    drawRect(-1, -0.74f, 1, -0.72f);
    drawRect(-1, -0.69f, 1, -0.67f);

    setColor(0.30f, 0.20f, 0.12f);
    for (int i = 0; i < 18; ++i) {
        float x = -0.95f + i * 0.11f;
        drawRect(x, -0.73f, x + 0.06f, -0.68f);
    }
}
void Scene3::drawClock(float cx, float cy, float scale) {
    setColor(1, 1, 1);
    drawCircle(cx, cy, 0.06f * scale, 60);

    setColor(0, 0, 0);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 60; ++i) {
        float a = (float)i / 60.0f * 2.0f * 3.1415926f;
        glVertex2f(cx + std::cos(a) * 0.06f * scale, cy + std::sin(a) * 0.06f * scale);
    }
    glEnd();
    glLineWidth(1.0f);

    // hands
    float mx = cx + std::cos(minuteAng) * 0.045f * scale;
    float my = cy + std::sin(minuteAng) * 0.045f * scale;
    float hx = cx + std::cos(hourAng) * 0.030f * scale;
    float hy = cy + std::sin(hourAng) * 0.030f * scale;

    setColor(0, 0, 0);
    drawLine(cx, cy, mx, my, 2.2f);
    drawLine(cx, cy, hx, hy, 3.0f);
    drawCircle(cx, cy, 0.005f * scale, 18);
}
void Scene3::drawStation() {
    // Roof
    setColor(0.20f, 0.20f, 0.22f);
    drawRect(-1, 0.23f, 1, 0.36f);

    // Main wall
    setColor(0.86f, 0.84f, 0.82f);
    drawRect(-1, -0.12f, 1, 0.23f);

    // Upper border line
    setColor(0.30f, 0.30f, 0.32f);
    drawRect(-1, 0.20f, 1, 0.23f);

    // Windows
    for (int i = 0; i < 9; ++i) {
        float x = -0.92f + i * 0.22f;
        setColor(0.68f, 0.78f, 0.92f);
        drawRect(x, -0.02f, x + 0.11f, 0.11f);

        setColor(0.55f, 0.65f, 0.80f);
        drawRect(x - 0.01f, -0.03f, x + 0.12f, -0.02f);
        drawRect(x - 0.01f, 0.11f, x + 0.12f, 0.12f);
        drawRect(x - 0.01f, -0.03f, x, 0.12f);
        drawRect(x + 0.11f, -0.03f, x + 0.12f, 0.12f);
    }

    // Pillars
    float px[] = {-0.82f, -0.42f, -0.02f, 0.38f, 0.78f};
    for (float p : px) {
        setColor(0.80f, 0.80f, 0.82f);
        drawRect(p - 0.03f, -0.12f, p + 0.03f, 0.23f);

        setColor(0.76f, 0.76f, 0.78f);
        drawCircle(p, 0.23f, 0.04f, 40);
        drawCircle(p, -0.12f, 0.04f, 40);
    }

    // ---------------- PLATFORM (like your 2nd screenshot) ----------------

    // top surface
    setColor(0.72f, 0.72f, 0.74f);
    drawRect(-1, -0.62f, 1, -0.12f);

    // depth band (dark strip under platform)
    setColor(0.40f, 0.40f, 0.42f);
    drawRect(-1, -0.66f, 1, -0.62f);

    // front edge highlight
    setColor(0.55f, 0.55f, 0.57f);
    drawRect(-1, -0.125f, 1, -0.12f);

    // vertical tile lines (stronger)
    setColor(0.60f, 0.60f, 0.62f);
    for (int i = 0; i < 26; ++i) {
        float x = -1.0f + i * (2.0f / 26.0f);
        drawRect(x, -0.62f, x + 0.0025f, -0.12f);
    }

    // subtle horizontal seams
    setColor(0.66f, 0.66f, 0.68f);
    drawRect(-1, -0.30f, 1, -0.298f);
    drawRect(-1, -0.45f, 1, -0.448f);

    // yellow safety line (thicker & bright)
    setColor(0.98f, 0.90f, 0.12f);
    drawRect(-1, -0.62f, 1, -0.595f);

    // Station board behind clock
    setColor(0.10f, 0.10f, 0.10f);
    drawRect(-0.22f, 0.12f, 0.22f, 0.18f);
    setColor(0.95f, 0.95f, 0.95f);
    drawRect(-0.20f, 0.13f, 0.20f, 0.17f);

    // Hanging light
    setColor(0.10f, 0.10f, 0.10f);
    drawRect(-0.62f, 0.18f, -0.61f, 0.23f);
    drawCircle(-0.615f, 0.17f, 0.025f, 30);

    // Clock
    drawClock(0.0f, 0.15f, 1.0f);
}
void Scene3::drawBrokenHeart(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    // left half
    setColor(0.85f, 0.05f, 0.10f);
    drawCircle(-0.03f, 0.02f, 0.035f, 40);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07f, 0.02f);
    glVertex2f( 0.00f, 0.02f);
    glVertex2f(-0.02f,-0.08f);
    glEnd();

    // right half
    drawCircle(0.03f, 0.02f, 0.035f, 40);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.07f, 0.02f);
    glVertex2f(0.00f, 0.02f);
    glVertex2f(0.02f,-0.08f);
    glEnd();

    // crack line
    setColor(1.0f, 0.92f, 0.92f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0.00f, 0.06f);
    glVertex2f(-0.01f, 0.03f);
    glVertex2f(0.01f, 0.01f);
    glVertex2f(-0.01f, -0.02f);
    glVertex2f(0.01f, -0.04f);
    glVertex2f(0.00f, -0.07f);
    glEnd();
    glLineWidth(1.0f);

    glPopMatrix();
}
// ------------------ LETTER ------------------

void Scene3::drawLetter() {
    if (!letterVisible) return;

    float floatOffset = std::sin(letterFloatT * 2.0f) * 0.008f;
    float y = letterY + floatOffset;

    setColor(0.98f, 0.96f, 0.90f);
    drawRect(letterX - 0.05f, y - 0.02f, letterX + 0.05f, y + 0.02f);

    setColor(0.20f, 0.20f, 0.20f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(letterX - 0.05f, y - 0.02f);
    glVertex2f(letterX + 0.05f, y - 0.02f);
    glVertex2f(letterX + 0.05f, y + 0.02f);
    glVertex2f(letterX - 0.05f, y + 0.02f);
    glEnd();
    glLineWidth(1.0f);

    glBegin(GL_TRIANGLES);
    glVertex2f(letterX - 0.05f, y + 0.02f);
    glVertex2f(letterX + 0.05f, y + 0.02f);
    glVertex2f(letterX,        y - 0.005f);
    glEnd();
}

// ------------------ DRAW ------------------

void Scene3::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawBackground();
    drawAllClouds();
    drawSun();
    drawStation();
    drawTracks();

    drawTrain();
    drawLetter();
    drawBoy();

    if (heartVisible) {
        drawBrokenHeart(boyX + 0.02f, boyY + 0.40f, 1.3f);
    }
}
