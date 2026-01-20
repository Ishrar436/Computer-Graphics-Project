#include <GL/glut.h>
#include "Scene1.h"

static Scene1 gScene1;

static void display()
{
    gScene1.draw();
    glutSwapBuffers();
}

static void keyboard(unsigned char key, int, int)
{
    gScene1.keyDown(key);
    glutPostRedisplay();
}

static void specialKey(int key, int, int)
{
    gScene1.specialDown(key);
    glutPostRedisplay();
}

static void timer(int)
{
    gScene1.update();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(1280, 720);
    glutCreateWindow("Love Story - Panel 1 (Sunny Park)");

    glutFullScreen();  // ✅ fullscreen

    gScene1.initGL();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
