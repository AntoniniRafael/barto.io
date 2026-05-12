#include <GL/glut.h>
#include "game.h"
#include "input.h"
#include "utils.h"
void saveScreenshot();

void keyDown(unsigned char key,int x,int y){
    if(screen != TELA_JOGO) return;

    if(key=='a' || key=='A'){
        moveLeft = KEY_PRESSED;
        tocarSom("moveA.wav");
    }
    if(key=='d' || key=='D'){
        moveRight = KEY_PRESSED;
        tocarSom("moveD.wav");
    }
    if(key=='p' || key=='P') paused = !paused;
    if(key=='s' || key=='S') saveScreenshot();
}

void keyUp(unsigned char key,int x,int y){
    if(key=='a' || key=='A') moveLeft = KEY_RELEASED;
    if(key=='d' || key=='D') moveRight = KEY_RELEASED;
}

void mouse(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
        if(screen==TELA_MENU) {
            screen = TELA_JOGO;
            initGame();
        } else if(screen==TELA_FIM) {
            screen = TELA_MENU;
        } else if(screen==TELA_VITORIA) {
            screen = TELA_MENU;
        }
    }
}