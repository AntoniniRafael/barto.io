#include "cachorro.h"
#include "game.h"

#include <GL/glut.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265f
#endif

static void desenharCirculo(float cx, float cy, float raio)
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i < 32; i++) {
        float angulo = (2.0f * PI * i) / 32.0f;
        glVertex2f(cx + raio * cosf(angulo),
                   cy + raio * sinf(angulo));
    }
    glEnd();
}

static void desenharOrelhas(float x, float y, float s)
{
    glColor3f(0.30f, 0.15f, 0.05f);

    // Orelha esquerda
    glBegin(GL_POLYGON);
    glVertex2f(x + s * -0.25f, y + s *  0.20f);
    glVertex2f(x + s * -0.10f, y + s *  0.28f);
    glVertex2f(x + s * -0.05f, y + s *  0.20f);
    glVertex2f(x + s * -0.10f, y + s *  0.14f);
    glEnd();

    // Orelha direita
    glBegin(GL_POLYGON);
    glVertex2f(x + s *  0.05f, y + s *  0.20f);
    glVertex2f(x + s *  0.10f, y + s *  0.28f);
    glVertex2f(x + s *  0.25f, y + s *  0.20f);
    glVertex2f(x + s *  0.10f, y + s *  0.14f);
    glEnd();
}

static void desenharCabeca(float x, float y, float s)
{
    glColor3f(0.72f, 0.45f, 0.18f);
    glBegin(GL_POLYGON);
    glVertex2f(x + s * -0.10f, y + s *  0.20f);
    glVertex2f(x + s *  0.10f, y + s *  0.20f);
    glVertex2f(x + s *  0.20f, y + s *  0.00f);
    glVertex2f(x + s *  0.00f, y + s * -0.20f);
    glVertex2f(x + s * -0.20f, y + s *  0.00f);
    glEnd();
}

static void desenharOlhos(float x, float y, float s)
{
    glColor3f(0.05f, 0.05f, 0.05f);
    desenharCirculo(x + s * -0.07f, y + s * 0.06f, s * 0.04f);
    desenharCirculo(x + s *  0.07f, y + s * 0.06f, s * 0.04f);

    // Brilho dos olhos
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharCirculo(x + s * -0.05f, y + s * 0.08f, s * 0.012f);
    desenharCirculo(x + s *  0.09f, y + s * 0.08f, s * 0.012f);
}

static void desenharFocinhoEBoca(float x, float y, float s)
{
    // Focinho
    glColor3f(0.55f, 0.32f, 0.12f);
    desenharCirculo(x, y + s * -0.06f, s * 0.08f);

    // Nariz
    glColor3f(0.10f, 0.05f, 0.05f);
    desenharCirculo(x, y + s * -0.03f, s * 0.03f);

    // Língua
    glColor3f(0.90f, 0.30f, 0.45f);
    glBegin(GL_POLYGON);
    glVertex2f(x + s * -0.04f, y + s * -0.10f);
    glVertex2f(x + s *  0.04f, y + s * -0.10f);
    glVertex2f(x + s *  0.04f, y + s * -0.18f);
    glVertex2f(x             , y + s * -0.22f);
    glVertex2f(x + s * -0.04f, y + s * -0.18f);
    glEnd();
}

static void desenharRabo(float x, float y, float s)
{
    // Rabo animado
    float oscilacao = cosf(timeSurvived * 8.0f) * s * 0.12f;

    glColor3f(0.55f, 0.30f, 0.08f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x + s * -0.18f, y + s *  0.00f);
    glVertex2f(x + s * -0.28f, y + s *  0.10f + oscilacao);
    glVertex2f(x + s * -0.38f, y + s *  0.20f + oscilacao * 1.5f);
    glEnd();
    glLineWidth(1.0f);
}

void desenhaCachorro(float cx, float cy, float escala)
{
    desenharRabo(cx, cy, escala);
    desenharOrelhas(cx, cy, escala);
    desenharCabeca(cx, cy, escala);
    desenharOlhos(cx, cy, escala);
    desenharFocinhoEBoca(cx, cy, escala);
}
