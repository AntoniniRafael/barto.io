#include "game.h"
#include "input.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>

#include <GL/glut.h>

Obstacle obs[MAX_OBS];
Particle particles[MAX_PARTICLES];

int lastFrameTime = 0;
float dogPos = 0;
int score = 0;
int gameOver = 0;
int gameOverUntil = 0;
int paused = 0;
int screen = TELA_MENU;
int timeStarted = 0;
float timeSurvived = 0;
int canMove = 1;
int moveLeft = KEY_RELEASED;
int moveRight = KEY_RELEASED;;
float obstacleSpeed;
int obstacleSpawnInterval = 1000;
int nextObstaclesTime = 0;

void initGame() {
    lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    dogPos = COLS / 2;
    score = 0;
    gameOver = 0;
    gameOverUntil = 0;
    paused = 0;
    timeStarted = glutGet(GLUT_ELAPSED_TIME);
    timeSurvived = 0;
    
    canMove = 1;
    moveLeft = moveRight = KEY_RELEASED;
    
    obstacleSpeed = 0.005f;
    obstacleSpawnInterval = 1000;
    nextObstaclesTime = 0;

    for(int i=0;i<MAX_OBS;i++)
        obs[i].active = 0;

    for(int i=0;i<MAX_PARTICLES;i++)
        particles[i].life = 0;
}

void endGame() {
    saveScore();
    screen = TELA_FIM;
}

void winGame() {
    saveScore();
    screen = TELA_VITORIA;
}

void spawnObstacle() {
    for(int i=0;i<MAX_OBS;i++) {
        if(!obs[i].active) {
            obs[i].x = rand() % COLS;
            obs[i].y = ROWS+1;
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
    int now = glutGet(GLUT_ELAPSED_TIME);
    int delta = now - lastFrameTime;
    lastFrameTime = now;
    
    if (screen != TELA_JOGO) {
        return;
    }

    if(gameOver) {
        if (now < gameOverUntil) {
            for(int i=0;i<MAX_PARTICLES;i++){
                if(particles[i].life > 0){
                    particles[i].x += particles[i].dx;
                    particles[i].y += particles[i].dy;
                    particles[i].life--;
                }
            }
        } else {
            endGame();
        }

        return;
    }

    if(gameOver || paused) return;
    
    if (moveLeft == KEY_PRESSED || moveRight == KEY_PRESSED) {
        if (canMove) {
            if (moveLeft == KEY_PRESSED && dogPos > 0) {
                dogPos -= 1;
            } else if (moveRight == KEY_PRESSED && dogPos < COLS - 1) {
                dogPos += 1;
            }
            canMove = 0;
        }
    }

    if (moveLeft == KEY_RELEASED && moveRight == KEY_RELEASED) {
        canMove = 1;
    }

    for(int i=0;i<MAX_OBS;i++) {
        if(obs[i].active) {
            obs[i].y -= obstacleSpeed * delta;

            float dogCenter = dogPos + 0.5;
            float obsCenter = obs[i].x + 0.5;

            if(obs[i].y < 1.0f && fabs(dogCenter - obsCenter) < 0.5) {
                createExplosion((int)dogPos, 0);
                tocarSom("game_over.wav");
                gameOver = 1;
                gameOverUntil = now + GAME_OVER_DURATION;
            }

            if(obs[i].y < 0) {
                obs[i].active = 0;
                score++;

                
                if (score >= PONTUACAO_VITORIA) {
                    winGame();
                    return; 
                }

                if (obstacleSpawnInterval > 0) {
                    obstacleSpawnInterval *= 0.99;
                }
                
                obstacleSpeed *= 1.01f;
                printf("obstacleSpeed: %f\nobstacleSpawnInterval: %d\n", obstacleSpeed, obstacleSpawnInterval);
            }
        }
    }

    if(now > nextObstaclesTime) {
        spawnObstacle();
        nextObstaclesTime = now + obstacleSpawnInterval;
    }

    timeSurvived = (now - timeStarted)/1000.0f;
}

void saveScore() {
    int scores[6];
    int count = 0;

    FILE *f = fopen("score.txt", "r");
    if(f) {
        while(count < 5 && fscanf(f, "%d", &scores[count]) != EOF) {
            count++;
        }
        fclose(f);
    }

    scores[count] = score;
    count++;

    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(scores[j] < scores[j+1]) {
                int temp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = temp;
            }
        }
    }

    f = fopen("score.txt", "w");
    if(f) {
        int maxToSave = (count < 5) ? count : 5;
        for(int i = 0; i < maxToSave; i++) {
            fprintf(f, "%d\n", scores[i]);
        }
        fclose(f);
    }
}