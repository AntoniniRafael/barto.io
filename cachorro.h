#ifndef CACHORRO_H
#define CACHORRO_H

#ifndef PI
#define PI 3.14159
#endif
#include <GL/glut.h>
#include <math.h>

static void desenhaCirculo(float cx, float cy, float raio){
    glBegin(GL_POLYGON);
    for(int i=0; i<100; i++){
        float angulo = (2 * PI * i)/100;
        float x = cx + raio * cos(angulo);
        float y = cy + raio * sin(angulo);
        glVertex2f(x, y);
    }
    glEnd();
}

static void orelhas(float x, float y, float scale){
    glColor3f(0.3, 0.15, 0.05);

    glBegin(GL_POLYGON);
    glVertex2f(x + scale*(-0.25), y + scale*(0.20)); 
    glVertex2f(x + scale*(-0.10), y + scale*(0.25)); 
    glVertex2f(x + scale*(-0.05), y + scale*(0.20)); 
    glVertex2f(x + scale*(-0.10), y + scale*(0.15)); 
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + scale*(0.05), y + scale*(0.20)); 
    glVertex2f(x + scale*(0.10), y + scale*(0.25));  
    glVertex2f(x + scale*(0.25), y + scale*(0.20)); 
    glVertex2f(x + scale*(0.10), y + scale*(0.15));
    glEnd();
}

static void cabeca(float x, float y, float scale){
    glColor3f(0.3, 0.15, 0.05);

    glBegin(GL_POLYGON);
    glVertex2f(x + scale*(-0.10), y + scale*(0.20));
    glVertex2f(x + scale*(0.10), y + scale*(0.20));
    glVertex2f(x + scale*(0.20), y + scale*(0.00));
    glVertex2f(x + scale*(0.00), y + scale*(-0.20));
    glVertex2f(x + scale*(-0.20), y + scale*(0.00));
    glEnd();
}

static void olhos(float x, float y, float scale){
    glColor3f(0.0, 0.0, 0.0);
    desenhaCirculo(x + scale*(-0.05), y + scale*(0.05), scale*0.03);
    desenhaCirculo(x + scale*(0.05), y + scale*(0.05), scale*0.03);
}

static void focinho_boca(float x, float y, float scale){
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(x + scale*(-0.03), y + scale*(-0.05));
    glVertex2f(x + scale*(0.03), y + scale*(-0.05));
    glVertex2f(x + scale*(0.03), y + scale*(-0.10));
    glVertex2f(x + scale*(-0.03), y + scale*(-0.10));
    glEnd();

    glColor3f(0.9, 0.3, 0.5);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + scale*(-0.03), y + scale*(-0.10));
    glVertex2f(x + scale*(0.03), y + scale*(-0.10));
    glVertex2f(x + scale*(0.00), y + scale*(-0.15));
    glEnd();
}

void desenhaCachorro(float x, float y, float scale){
    orelhas(x, y, scale);
    cabeca(x, y, scale);
    olhos(x, y, scale);
    focinho_boca(x, y, scale);
}

#endif