#include <GL/glut.h>
#include "game.h"
#include <stdio.h>
#include <math.h>
#include "cachorro.h"

void drawText(float x, float y, char *text) {
    glRasterPos2f(x,y);
    while(*text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
}

void drawBackground() {
    glBegin(GL_QUADS);

    glColor3f(0.1, 0.1, 0.3);
    glVertex2f(0,0);

    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(COLS,0);

    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(COLS,ROWS);

    glColor3f(0.1, 0.1, 0.3);
    glVertex2f(0,ROWS);

    glEnd();
}

void drawDog() {
    glColor3f(0.8,0.5,0.2);
    glBegin(GL_QUADS);
    glVertex2f(dogPos,0);
    glVertex2f(dogPos+1,0);
    glVertex2f(dogPos+1,0.5);
    glVertex2f(dogPos,0.5);
    glEnd();

    glColor3f(0.9,0.6,0.3);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(dogPos+0.5,0.8);
    for(int i=0;i<=20;i++){
        float ang = i*3.14/10;
        glVertex2f(dogPos+0.5 + 0.3*cos(ang), 0.8 + 0.3*sin(ang));
    }
    glEnd();

    glColor3f(0.5,0.3,0.1);
    glBegin(GL_TRIANGLES);
    glVertex2f(dogPos+0.3,1.0);
    glVertex2f(dogPos+0.5,0.8);
    glVertex2f(dogPos+0.7,1.0);
    glEnd();

    glColor3f(0,0,0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(dogPos+0.55,0.85);
    glEnd();
}

void drawObs() {
    for(int i=0;i<MAX_OBS;i++) {
        if(obs[i].active) {

            float pulse = (sin(timeSurvived * 0.1) + 1) / 2;
            glColor4f(1, pulse, 0, 0.8);

            glBegin(GL_QUADS);
            glVertex2f(obs[i].x, obs[i].y);
            glVertex2f(obs[i].x+1, obs[i].y);
            glVertex2f(obs[i].x+1, obs[i].y+1);
            glVertex2f(obs[i].x, obs[i].y+1);
            glEnd();
        }
    }
}

void drawParticles(){
    glPointSize(6);
    glBegin(GL_POINTS);

    for(int i=0;i<MAX_PARTICLES;i++){
        if(particles[i].life > 0){
            float lifeRatio = particles[i].life / 20.0;
            glColor3f(1, lifeRatio, 0);
            glVertex2f(particles[i].x, particles[i].y);
        }
    }

    glEnd();
}

void drawHUD() {
    char txt[50];

    glColor3f(1,1,1);

    sprintf(txt,"Score: %d",score);
    drawText(0,9.5,txt);

    sprintf(txt,"Tempo: %.1fs",timeSurvived);
    drawText(3,9.5,txt);
}

void drawRanking(){
    FILE *f = fopen("score.txt","r");
    char txt[32];
    int score = 0;
    float y = 3;
    int rank = 1;

    drawText(1,4,"TOP 5:");

    if(f){
        while(fscanf(f,"%d",&score)!=EOF){
            sprintf(txt,"%do -> %4d",rank,score);
            drawText(1,y,txt);
            y -= 0.5f;
            rank++;
        }
        fclose(f);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    if(screen == 0) {
        glColor3f(1,1,1);
        drawText(1.2,6,"BARTO.IO");
        drawText(1.5,5,"Clique para jogar");
        drawText(1.3,4,"A/D para mover");
        drawText(1.3,3,"P para pausar");
        drawText(1.3,2,"S para screenshot");
    }
    else if(screen == 1) {
        //drawDog();
        desenhaCachorro(dogPos+0.5, 0.5f, 2.0f);
        drawObs();
        drawParticles();
        drawHUD();
    }
    else if(screen == 2) {
        glColor3f(1,1,1);

        drawText(1,6,"GAME OVER");

        char txt[50];
        sprintf(txt,"Score: %d", score);
        drawText(1,5,txt);

        drawRanking();
    }

    glutSwapBuffers();
}