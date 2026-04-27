#include <stdio.h>
#include <GL/glut.h>

void saveScreenshot() {
    const char filename[] = "screenshot.bmp";
    int w = 500, h = 800;

    // 1. Alocar memória para os pixels (RGB)
    unsigned char* pixels = malloc(sizeof(unsigned char) * 3 * w * h);
    
    // 2. Ler os pixels do Back Buffer
    // Usamos GL_BGR porque o formato BMP armazena as cores como Blue-Green-Red
    glReadPixels(0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    // 3. Cabeçalhos do arquivo BMP (Total 54 bytes)
    unsigned char fileHeader[14] = {
        'B','M',            // Assinatura
        0,0,0,0,            // Tamanho do arquivo (preencheremos depois)
        0,0,0,0,            // Reservado
        54,0,0,0            // Offset onde começam os pixels
    };
    
    unsigned char infoHeader[40] = {
        40,0,0,0,           // Tamanho do cabeçalho de info
        0,0,0,0,            // Largura (preencheremos depois)
        0,0,0,0,            // Altura (preencheremos depois)
        1,0,                // Planos
        24,0,               // Bits por pixel (24 bits = RGB)
        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 // Outros campos zerados
    };

    // Preencher tamanhos no cabeçalho
    int fileSize = 54 + (3 * w * h);
    fileHeader[2] = (unsigned char)(fileSize);
    fileHeader[3] = (unsigned char)(fileSize >> 8);
    fileHeader[4] = (unsigned char)(fileSize >> 16);
    fileHeader[5] = (unsigned char)(fileSize >> 24);

    infoHeader[4] = (unsigned char)(w);
    infoHeader[5] = (unsigned char)(w >> 8);
    infoHeader[6] = (unsigned char)(w >> 16);
    infoHeader[7] = (unsigned char)(w >> 24);

    infoHeader[8] = (unsigned char)(h);
    infoHeader[9] = (unsigned char)(h >> 8);
    infoHeader[10] = (unsigned char)(h >> 16);
    infoHeader[11] = (unsigned char)(h >> 24);

    // 4. Escrever no arquivo
    FILE* f = fopen(filename, "wb");
    fwrite(fileHeader, 1, 14, f);
    fwrite(infoHeader, 1, 40, f);
    fwrite(pixels, 1, 3 * w * h, f);
    fclose(f);

    free(pixels);
}