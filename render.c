#include <GL/glut.h>
#include "game.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cachorro.h"
GLuint texCabeca;

//eu marrie
GLuint texturaDog;
int texturaCarregada = 0;
int menuImageLargura = 0;
int menuImageAltura = 0;

void carregarTextura(const char* arquivo, GLuint* idTextura) {
    int largura, altura, canais;
    stbi_set_flip_vertically_on_load(true); 

    // Carrega a imagem forçando 4 canais (RGBA) para evitar conflitos
    unsigned char* dados = stbi_load(arquivo, &largura, &altura, &canais, STBI_rgb_alpha);

    if (dados) {
        glGenTextures(1, idTextura);
        glBindTexture(GL_TEXTURE_2D, *idTextura);

        // Configurações de repetição e filtro
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Envia os dados para a GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);

        stbi_image_free(dados);
        printf("Sucesso: %s carregado! (%dx%d)\n", arquivo, largura, altura);
        texturaCarregada = 1;
        if (idTextura == &texturaDog) {
            menuImageLargura = largura;
            menuImageAltura = altura;
        }
    } else {
        // Se der erro, ele vai imprimir o motivo real aqui
        printf("Erro STB em '%s': %s\n", arquivo, stbi_failure_reason());
    }
}

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

void drawMenuImage() {
    if(!texturaCarregada)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaDog);
    glColor3f(1,1,1);

    float maxWidth = COLS * 0.9f;
    float maxHeight = ROWS * 0.35f;
    float imgW = maxWidth;
    float imgH = maxHeight;

    if (menuImageLargura > 0 && menuImageAltura > 0) {
        float aspect = (float)menuImageLargura / (float)menuImageAltura;
        if (aspect >= 1.0f) {
            imgW = maxWidth;
            imgH = imgW / aspect;
            if (imgH > maxHeight) {
                imgH = maxHeight;
                imgW = imgH * aspect;
            }
        } else {
            imgH = maxHeight;
            imgW = imgH * aspect;
            if (imgW > maxWidth) {
                imgW = maxWidth;
                imgH = imgW / aspect;
            }
        }
    }

    float centerX = COLS / 2.0f;
    float centerY = 8.0f;
    float left = centerX - imgW / 2.0f;
    float right = centerX + imgW / 2.0f;
    float bottom = centerY - imgH / 2.0f;
    float top = centerY + imgH / 2.0f;

    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(left, bottom);

        glTexCoord2f(1,0);
        glVertex2f(right, bottom);

        glTexCoord2f(1,1);
        glVertex2f(right, top);

        glTexCoord2f(0,1);
        glVertex2f(left, top);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    if(screen == 0) {
        drawMenuImage();
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