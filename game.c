#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>

float dogPos = 2;

int score = 0;
int gameOver = 0;
int paused = 0;
int screen = 0;
int timeSurvived = 0;

int moveLeft = 0;
int moveRight = 0;

Obstacle obs[MAX_OBS];
Particle particles[MAX_PARTICLES];

void initGame() {
    dogPos = 2;
    score = 0;
    gameOver = 0;
    paused = 0;
    timeSurvived = 0;

    moveLeft = moveRight = 0;

    for(int i=0;i<MAX_OBS;i++)
        obs[i].active = 0;

    for(int i=0;i<MAX_PARTICLES;i++)
        particles[i].life = 0;
}

void spawnObstacle() {
    for(int i=0;i<MAX_OBS;i++) {
        if(!obs[i].active) {
            obs[i].x = rand() % COLS;
            obs[i].y = ROWS-1;
            obs[i].active = 1;
            break;
        }
    }
}

void createExplosion(int x, int y){
    for(int i=0;i<MAX_PARTICLES;i++){
        particles[i].x = x + 0.5;
        particles[i].y = y + 0.5;
        particles[i].dx = (rand()%10 - 5) / 10.0;
        particles[i].dy = (rand()%10 - 5) / 10.0;
        particles[i].life = rand()%20 + 10;
    }
}

void updateGame() {
    if(gameOver || paused || screen != 1) return;

    // movimentação suave
    float speed = 0.55;

    if(moveLeft && dogPos > 0)
        dogPos -= speed;

    if(moveRight && dogPos < COLS-1)
        dogPos += speed;

    // obstáculos
    for(int i=0;i<MAX_OBS;i++) {
        if(obs[i].active) {
            obs[i].y--;

            float dogCenter = dogPos + 0.5;
            float obsCenter = obs[i].x + 0.5;

            if(obs[i].y == 0 && fabs(dogCenter - obsCenter) < 0.5) {
                createExplosion((int)dogPos, 0);
                Beep(750, 300);

                gameOver = 1;
                screen = 2;
                saveScore();
            }

            if(obs[i].y < 0) {
                obs[i].active = 0;
                score++;
            }
        }
    }

    // dificuldade progressiva
    float difficulty = timeSurvived / 200.0;
    if(rand()%10 < (3 + difficulty))
        spawnObstacle();

    // partículas
    for(int i=0;i<MAX_PARTICLES;i++){
        if(particles[i].life > 0){
            particles[i].x += particles[i].dx;
            particles[i].y += particles[i].dy;
            particles[i].life--;
        }
    }

    timeSurvived++;
}

void saveScore() {
    int scores[100], count = 0;

    FILE *f = fopen("score.txt", "r");

    if(f){
        while(fscanf(f,"%d",&scores[count])!=EOF && count<100)
            count++;
        fclose(f);
    }

    scores[count++] = score;

    for(int i=0;i<count-1;i++){
        for(int j=0;j<count-i-1;j++){
            if(scores[j] < scores[j+1]){
                int tmp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = tmp;
            }
        }
    }

    f = fopen("score.txt","w");

    for(int i=0;i<count && i<5;i++){
        fprintf(f,"%d\n",scores[i]);
    }

    fclose(f);
}