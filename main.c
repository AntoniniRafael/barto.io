#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "input.h"

void display();

void timer(int v){
    int delay = 0;
    updateGame();
    glutPostRedisplay();
    
    glutTimerFunc(delay, timer, 0);
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
    
    int width = 500;
    int height = 700;
    int screenW = glutGet(GLUT_SCREEN_WIDTH);
    int screenH = glutGet(GLUT_SCREEN_HEIGHT);
    int posX = (screenW - width) / 2;
    int posY = (screenH - height) / 2;

    glutInitWindowSize(width, height);
    glutInitWindowPosition(posX, posY);
    
    glutCreateWindow("Dog Dodge Deluxe");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutMouseFunc(mouse);
    glutTimerFunc(0,timer,0);

    glutMainLoop();
}