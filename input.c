#include <GL/glut.h>
#include "game.h"

void saveScreenshot();

void keyDown(unsigned char key,int x,int y){
    if(screen != 1) return;

    if(key=='a') moveLeft = 1;
    if(key=='d') moveRight = 1;
    if(key=='p') paused = !paused;
    if(key=='s') saveScreenshot();
}

void keyUp(unsigned char key,int x,int y){
    if(key=='a') moveLeft = 0;
    if(key=='d') moveRight = 0;
}

void mouse(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
        if(screen==0) {
            screen = 1;
            initGame();
        } else if(screen==2) {
            screen = 0;
        }
    }
}