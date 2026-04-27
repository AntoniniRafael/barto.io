#ifndef GAME_H
#define GAME_H

#define ROWS 10
#define COLS 5
#define MAX_OBS 30
#define MAX_PARTICLES 50
#define GAME_OVER_DURATION 500

typedef struct {
    float x, y;
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
extern float timeSurvived;

extern int moveLeft;
extern int moveRight;
extern int canMove;

extern Obstacle obs[MAX_OBS];
extern Particle particles[MAX_PARTICLES];

extern int lastFrameTime;

extern int score;
extern int gameOver;
extern int gameOverUntil; // até que timestamp os efetios de game over precisam ser desenhados
extern int paused;
extern int screen;
extern int timeStarted;
extern float timeSurvived;

extern int nextInputTime;
extern int canMove;
extern int moveLeft;
extern int moveRight;

extern float obstacleSpeed;
extern int obstacleSpawnInterval;
extern int nextObstaclesTime;

void initGame();
void endGame();
void updateGame();
void spawnObstacle();
void saveScore();
void createExplosion(int x, int y);

#endif