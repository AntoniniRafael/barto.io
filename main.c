#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

void display();
void keyDown(unsigned char,int,int);
void keyUp(unsigned char,int,int);
void mouse(int,int,int,int);

void timer(int v){
    updateGame();
    glutPostRedisplay();

    int speed = 200 - (timeSurvived / 10);
    if(speed < 90) speed = 90;

    glutTimerFunc(speed, timer, 0);
}

void init(){
    glClearColor(0,0,0,1);
    gluOrtho2D(0,COLS,0,ROWS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc,char** argv){
    srand(time(NULL));

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(500,800);
    glutCreateWindow("Dog Dodge Deluxe");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutMouseFunc(mouse);
    glutTimerFunc(0,timer,0);

    glutMainLoop();
}