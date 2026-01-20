#include <GL/glut.h>

#include "Scene1.h"
   // NEW

static Scene1 scene1;


static int activeScene = 1;

static void setScene(int newScene)
{
    if (newScene == activeScene) return;
    activeScene = newScene;

    // init GL state per scene
    if      (activeScene == 1) scene1.initGL();

    // window titles
    if      (activeScene == 1) glutSetWindowTitle("Love Story - Scene 1: The Meeting");

    glutPostRedisplay();
}

static void display()
{
    // IMPORTANT:
    // Your scenes already call glutSwapBuffers() inside their draw().
    // So we DO NOT call glutSwapBuffers() here (otherwise white screen / flicker).
    if      (activeScene == 1) scene1.draw();  // NEW
    glutSwapBuffers();
}

static void keyboard(unsigned char key, int x, int y)
{
    // scene switching
    if(key == '1') { setScene(1); return; }// NEW

    // forward key to active scene
    if(activeScene == 1) scene1.keyDown(key); // NEW
}

static void specialKeys(int key, int x, int y)
{
    if(activeScene == 1) scene1.specialDown(key);// NEW
}

static void timer(int value)
{
    if(activeScene == 1) scene1.update();

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
