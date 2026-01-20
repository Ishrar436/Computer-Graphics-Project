#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "newfile.cpp"

float left =-.3f, right =.3f, top=0.3f, bottom=-.3f;
bool changeColor= false;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (changeColor){
        glColor3f(1,0,0);
    }
    else{
        glColor3f(1,1,1);
    }
    glBegin(GL_POLYGON);
    glVertex2f(left,top);
    glVertex2f(right,top);
    glVertex2f(right,bottom);
    glVertex2f(left,bottom);

    glEnd();
    glFlush();



}

void handleMouseClick(int button, int state, int x , int y){

if (state == GLUT_DOWN){

        float px= (float)x/800*2.0-1.0;
        float py= 1.0- (float)y/600*2.0;

 std::cout<<"X="<<px<<" "<<"Y="<<py<<std::endl;
 if (px>=left && px<=right && py>=bottom && py<=top){
    changeColor=!changeColor;
    glutPostRedisplay();
 }}

}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
   // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL 64bit");

   // glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    //glutKeyboardFunc(keyboardInput);
//    glutReshapeFunc(reshape);
glutMouseFunc(handleMouseClick);
    //addmy(1, 200);

    glutMainLoop();
    return 0;
}
