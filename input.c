#include <GL/glut.h>
#include "game.h"
#include "input.h"

void saveScreenshot();

void keyDown(unsigned char key,int x,int y){
    if(screen != 1) return;

    if(key=='a') moveLeft = KEY_PRESSED;
    if(key=='d') moveRight = KEY_PRESSED;
    if(key=='p') paused = !paused;
    if(key=='s') saveScreenshot();
}

void keyUp(unsigned char key,int x,int y){
    if(key=='a') moveLeft = KEY_RELEASED;
    if(key=='d') moveRight = KEY_RELEASED;
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