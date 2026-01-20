#include <GL/glut.h>

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"   // NEW

static Scene1 scene1;
static Scene2 scene2;
static Scene3 scene3;
static Scene4 scene4;
static Scene5 scene5; // NEW

static int activeScene = 1;

static void setScene(int newScene)
{
    if (newScene == activeScene) return;
    activeScene = newScene;

    // init GL state per scene
    if      (activeScene == 1) scene1.initGL();
    else if (activeScene == 2) scene2.initGL();
    else if (activeScene == 3) scene3.initGL();
    else if (activeScene == 4) scene4.initGL();
    else if (activeScene == 5) scene5.initGL(); // NEW

    // window titles
    if      (activeScene == 1) glutSetWindowTitle("Love Story - Scene 1: The Meeting");
    else if (activeScene == 2) glutSetWindowTitle("Love Story - Scene 2: The Rain");
    else if (activeScene == 3) glutSetWindowTitle("Love Story - Scene 3: The Farewell");
    else if (activeScene == 4) glutSetWindowTitle("Love Story - Scene 4: Lonely Night");
    else if (activeScene == 5) glutSetWindowTitle("Love Story - Scene 5: The Bridge Walk"); // NEW

    glutPostRedisplay();
}

static void display()
{
    // IMPORTANT:
    // Your scenes already call glutSwapBuffers() inside their draw().
    // So we DO NOT call glutSwapBuffers() here (otherwise white screen / flicker).
    if      (activeScene == 1) scene1.draw();
    else if (activeScene == 2) scene2.draw();
    else if (activeScene == 3) scene3.draw();
    else if (activeScene == 4) scene4.draw();
    else if (activeScene == 5) scene5.draw();  // NEW
    glutSwapBuffers();
}

static void keyboard(unsigned char key, int x, int y)
{
    // scene switching
    if (key == '1') { setScene(1); return; }
    if (key == '2') { setScene(2); return; }
    if (key == '3') { setScene(3); return; }
    if (key == '4') { setScene(4); return; }
    if (key == '5') { setScene(5); return; } // NEW

    // forward key to active scene
    if      (activeScene == 1) scene1.keyDown(key);
    else if (activeScene == 2) scene2.keyDown(key);
    else if (activeScene == 3) scene3.keyDown(key);
    else if (activeScene == 4) scene4.keyDown(key);
    else if (activeScene == 5) scene5.keyDown(key); // NEW
}

static void specialKeys(int key, int x, int y)
{
    if      (activeScene == 1) scene1.specialDown(key);
    else if (activeScene == 2) scene2.specialDown(key);
    //else if (activeScene == 3) scene3.specialDown(key);
    else if (activeScene == 4) scene4.specialDown(key);
    //else if (activeScene == 5) scene5.specialDown(key); // NEW
}

static void timer(int value)
{
    if      (activeScene == 1) scene1.update();
    else if (activeScene == 2) scene2.update();
    else if (activeScene == 3) scene3.update(0.016f);     // if yours is update(float), change to: scene3.update(0.016f);
    else if (activeScene == 4) scene4.update();
    else if (activeScene == 5) scene5.update(0.016f);     // NEW

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Love Story");

    //glutFullScreen();

    scene1.initGL(); // start at scene 1

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
