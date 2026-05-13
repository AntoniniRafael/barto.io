#include <GL/glut.h>
#include "game.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SOIL/SOIL.h>
#include "cachorro.h"
GLuint texCabeca;

GLuint texturaDog;
int texturaCarregada = 0;
int menuImageLargura = 0;
int menuImageAltura = 0;

void carregarTextura(const char* arquivo, GLuint* idTextura) {
    int largura, altura;

    unsigned char* dados = SOIL_load_image(arquivo, &largura, &altura, NULL, SOIL_LOAD_RGBA);
    if (!dados) {
        printf("Erro SOIL em '%s': %s\n", arquivo, SOIL_last_result());
        return;
    }

    GLuint tex = SOIL_create_OGL_texture(dados, largura, altura, 4, 0, SOIL_FLAG_INVERT_Y);
    if (tex == 0) {
        printf("Erro SOIL em '%s': %s\n", arquivo, SOIL_last_result());
        SOIL_free_image_data(dados);
        return;
    }

    *idTextura = tex;
    glBindTexture(GL_TEXTURE_2D, *idTextura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SOIL_free_image_data(dados);
    printf("Sucesso: %s carregado! (%dx%d)\n", arquivo, largura, altura);
    texturaCarregada = 1;
    if (idTextura == &texturaDog) {
        menuImageLargura = largura;
        menuImageAltura = altura;
    }
}

void drawText(float x, float y, char *text) {
    glRasterPos2f(x, y);
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

static void drawColumnLines() {
    glColor3f(0.3f, 0.3f, 0.5f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for(int i = 1; i < COLS; i++) {
        glVertex2f((float)i, 0.0f);
        glVertex2f((float)i, (float)ROWS);
    }
    glEnd();
}

void drawDog() {
    glColor3f(0.8, 0.5, 0.2);
    glBegin(GL_QUADS);
    glVertex2f(dogPos, 0);
    glVertex2f(dogPos+1, 0);
    glVertex2f(dogPos+1, 0.5);
    glVertex2f(dogPos, 0.5);
    glEnd();

    glColor3f(0.9, 0.6, 0.3);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(dogPos+0.5, 0.8);
    for(int i = 0; i <= 20; i++) {
        float ang = i * 3.14 / 10;
        glVertex2f(dogPos+0.5 + 0.3*cos(ang), 0.8 + 0.3*sin(ang));
    }
    glEnd();

    glColor3f(0.5, 0.3, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex2f(dogPos+0.3, 1.0);
    glVertex2f(dogPos+0.5, 0.8);
    glVertex2f(dogPos+0.7, 1.0);
    glEnd();

    glColor3f(0, 0, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(dogPos+0.55, 0.85);
    glEnd();
}

void drawObs() {
    for(int i = 0; i < MAX_OBS; i++) {
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

void drawParticles() {
    glPointSize(6);
    glBegin(GL_POINTS);
    for(int i = 0; i < MAX_PARTICLES; i++) {
        if(particles[i].life > 0) {
            float lifeRatio = particles[i].life / 20.0;
            glColor3f(1, lifeRatio, 0);
            glVertex2f(particles[i].x, particles[i].y);
        }
    }
    glEnd();
}

static void desenhaBarraProgresso(float atual, float alvo) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(10.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(1.0f, 9.0f);
    glVertex2f(4.0f, 9.0f);
    glEnd();

    glColor3f(0.2f, 1.0f, 0.2f);
    glLineWidth(10.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(1.0f, 9.0f);
    glVertex2f(1.0f + ((atual/alvo) * 3.0f), 9.0f);
    glEnd();
}

void drawHUD() {
    char txt[50];
    glColor3f(1, 1, 1);
    sprintf(txt, "Score: %d", score);
    drawText(0, 9.5, txt);
    sprintf(txt, "Tempo: %.1fs", timeSurvived);
    drawText(3, 9.5, txt);
    desenhaBarraProgresso(score, PONTUACAO_VITORIA);
}

void drawRanking() {
    FILE *f = fopen("score.txt", "r");
    char txt[32];
    int sc = 0;
    float y = 3;
    int rank = 1;

    drawText(1, 4, "TOP 5:");

    if(f) {
        while(fscanf(f, "%d", &sc) != EOF) {
            sprintf(txt, "%do -> %4d", rank, sc);
            drawText(1, y, txt);
            y -= 0.5f;
            rank++;
        }
        fclose(f);
    }
}

void drawMenuImage() {
    if(!texturaCarregada) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaDog);
    glColor3f(1, 1, 1);

    float maxWidth = COLS * 0.9f;
    float maxHeight = ROWS * 0.35f;
    float imgW = maxWidth;
    float imgH = maxHeight;

    if (menuImageLargura > 0 && menuImageAltura > 0) {
        float aspect = (float)menuImageLargura / (float)menuImageAltura;
        if (aspect >= 1.0f) {
            imgW = maxWidth;
            imgH = imgW / aspect;
            if (imgH > maxHeight) { imgH = maxHeight; imgW = imgH * aspect; }
        } else {
            imgH = maxHeight;
            imgW = imgH * aspect;
            if (imgW > maxWidth) { imgW = maxWidth; imgH = imgW / aspect; }
        }
    }

    float centerX = COLS / 2.0f;
    float centerY = 8.0f;
    float left   = centerX - imgW / 2.0f;
    float right  = centerX + imgW / 2.0f;
    float bottom = centerY - imgH / 2.0f;
    float top    = centerY + imgH / 2.0f;

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(left,  bottom);
        glTexCoord2f(1, 0); glVertex2f(right, bottom);
        glTexCoord2f(1, 1); glVertex2f(right, top);
        glTexCoord2f(0, 1); glVertex2f(left,  top);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if(screen == TELA_MENU) {
        drawBackground();           
        drawMenuImage();
        glColor3f(1, 1, 1);
        drawText(1.2, 6, "BARTO.IO");
        drawText(1.5, 5, "Clique para jogar");
        drawText(1.3, 4, "A/D para mover");
        drawText(1.3, 3, "P para pausar");
        drawText(1.3, 2, "S para screenshot");
    }
    else if(screen == TELA_JOGO) {
        drawBackground();
        drawColumnLines();          
        desenhaCachorro(dogPos+0.5, 0.5f, 2.0f);
        drawObs();
        drawParticles();
        drawHUD();
    }
    else if(screen == TELA_FIM) {
        drawBackground();           
        glColor3f(1, 1, 1);
        drawText(1, 6, "GAME OVER");
        char txt[50];
        sprintf(txt, "Score: %d", score);
        drawText(1, 5, txt);
        drawRanking();
        glColor3f(0.6f, 0.6f, 0.6f);
        drawText(1.0, 0.5, "Clique para voltar ao menu");
    }
    else if(screen == TELA_VITORIA) {
        drawBackground();           
        float tempo = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        float pulse = (sin(tempo * 5.0f) + 1.0f) / 2.0f;

        glColor3f(1.0f, 0.8f + 0.2f * pulse, 0.0f);
        drawText(1.2, 8.0, "VOCE VENCEU!");

        char txt[50];
        glColor3f(1.0f, 1.0f, 1.0f);
        sprintf(txt, "Score final: %d", score);
        drawText(1.3, 7.0, txt);
        sprintf(txt, "Tempo: %.1fs", timeSurvived);
        drawText(1.3, 6.5, txt);

        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(10.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(1.0f, 6.0f);
        glVertex2f(4.0f, 6.0f);
        glEnd();

        glColor3f(1.0f, 0.8f + 0.2f * pulse, 0.0f);
        glLineWidth(10.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(1.0f, 6.0f);
        glVertex2f(4.0f, 6.0f);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(1.8, 5.6, "50 / 50");

        drawRanking();

        glColor3f(0.6f, 0.6f, 0.6f);
        drawText(0.8, 0.5, "Clique para voltar ao menu");
    }

    glutSwapBuffers();
}
