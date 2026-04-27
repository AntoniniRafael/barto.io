#include "game.h"
#include "input.h"
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
int gameOverUntil = 0; // até que timestamp os efetios de game over precisam ser desenhados
int paused = 0;
int screen = 0;
int timeStarted = 0;
float timeSurvived = 0;
int nextInputTime = 0;
int canMove = 1;
int moveLeft = KEY_RELEASED;
int moveRight = KEY_RELEASED;;
float obstacleSpeed;
int obstacleSpawnInterval = 1000;
int nextObstaclesTime = 0;

void initGame() {
    lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    dogPos = COLS / 2; // inicia no meio
    score = 0;
    gameOver = 0;
    gameOverUntil = 0;
    paused = 0;
    timeStarted = glutGet(GLUT_ELAPSED_TIME);
    timeSurvived = 0;
    
    nextInputTime = 0;
    canMove = 1;
    moveLeft = moveRight = KEY_RELEASED;
    
    obstacleSpeed = 0.005f;
    obstacleSpawnInterval = 1000; // começa com 1 obstacúlo sendo criado por segundo
    nextObstaclesTime = 0;

    for(int i=0;i<MAX_OBS;i++)
        obs[i].active = 0;

    for(int i=0;i<MAX_PARTICLES;i++)
        particles[i].life = 0;
}

void endGame() {
    saveScore();
    screen = 2; // tela de fim de jogo / score board
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
    
    if (screen != 1) {
        return; // se não está na tela de jogo
    }

    if(gameOver) {
        if (now < gameOverUntil) {
            // desenha partículas do gameover no cachorro
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

        return; // não faz mais nada enquanto em gameover
    }

    if(gameOver || paused) return;
    
    if (moveLeft == KEY_PRESSED || moveRight == KEY_PRESSED) {
        if (canMove) {
            if (moveLeft == KEY_PRESSED && dogPos > 0) {
                dogPos -= 1;
            } else if (moveRight == KEY_PRESSED && dogPos < COLS - 1) {
                dogPos += 1;
            }
            canMove = 0; // Trava o movimento até um tempo passar
            nextInputTime = now + 150; // Delay entre pulos de grade
        }
    }

    // Se o tempo de espera passou, libera o movimento novamente
    if (now > nextInputTime) {
        canMove = 1;
    }

    // Se o jogador soltou as teclas, reseta o timer para ele poder clicar rápido de novo
    if (moveLeft == KEY_RELEASED && moveRight == KEY_RELEASED) {
        canMove = 1;
        nextInputTime = 0;
    }

    // obstáculos
    for(int i=0;i<MAX_OBS;i++) {
        if(obs[i].active) {
            obs[i].y -= obstacleSpeed * delta;

            float dogCenter = dogPos + 0.5;
            float obsCenter = obs[i].x + 0.5;

            if(obs[i].y < 1.0f && fabs(dogCenter - obsCenter) < 0.5) {
                createExplosion((int)dogPos, 0);
                gameOver = 1;
                gameOverUntil = now + GAME_OVER_DURATION;
            }

            if(obs[i].y < 0) {
                obs[i].active = 0;
                score++;
                if (obstacleSpawnInterval > 333) {
                    obstacleSpawnInterval *= 0.99; // diminui 1% o intervalo entre cada spawn
                }
                if (obstacleSpeed <= 0.01f) {
                    obstacleSpeed *= 1.01f; // aumenta 1% a velocidade a cada score
                }
                printf("obstacleSpeed: %f\nobstacleSpawnInterval: %d\n", obstacleSpeed, obstacleSpawnInterval);
            }
        }
    }

    // verifica se já deu o intervalo para novo obstaculo
    if(now > nextObstaclesTime) {
        spawnObstacle();
        nextObstaclesTime = now + obstacleSpawnInterval; // ajusta o próximo
    }

    timeSurvived = (now - timeStarted)/1000.0f; // atualiza a quantos segundos o player está vivo
}

void saveScore() {
    int scores[6]; // 5 + 1 (o novo score)
    int count = 0;

    // Tentar ler os scores atuais
    FILE *f = fopen("score.txt", "r");
    if(f) {
        while(count < 5 && fscanf(f, "%d", &scores[count]) != EOF) {
            count++;
        }
        fclose(f);
    }

    // adicionar o score atual ao array
    scores[count] = score;
    count++;

    // Bubble Sort
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(scores[j] < scores[j+1]) {
                int temp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = temp;
            }
        }
    }

    // salvar apenas os 5 melhores
    f = fopen("score.txt", "w");
    if(f) {
        int maxToSave = (count < 5) ? count : 5;
        for(int i = 0; i < maxToSave; i++) {
            fprintf(f, "%d\n", scores[i]);
        }
        fclose(f);
    }
}