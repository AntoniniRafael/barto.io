#ifndef GAME_H
#define GAME_H

#define ROWS 10
#define COLS 5
#define MAX_OBS 30
#define MAX_PARTICLES 50

typedef struct {
    int x, y;
    int active;
} Obstacle;

typedef struct {
    float x, y;
    float dx, dy;
    int life;
} Particle;

extern float dogPos;
extern int score;
extern int gameOver;
extern int paused;
extern int screen;
extern int timeSurvived;

extern int moveLeft;
extern int moveRight;

extern Obstacle obs[MAX_OBS];
extern Particle particles[MAX_PARTICLES];

void initGame();
void updateGame();
void spawnObstacle();
void saveScore();
void createExplosion(int x, int y);

#endif