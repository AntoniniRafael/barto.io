#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h>

void tocarSom(const char *filename) {
    PlaySoundA(filename, NULL, SND_FILENAME | SND_ASYNC);
}

void saveScreenshot() {
    const char filename[] = "screenshot.bmp";
    int w = 500, h = 800;

    unsigned char* pixels = malloc(sizeof(unsigned char) * 3 * w * h);
    
    glReadPixels(0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    unsigned char fileHeader[14] = {
        'B','M',            
        0,0,0,0,            
        0,0,0,0,            
        54,0,0,0            
    };
    
    unsigned char infoHeader[40] = {
        40,0,0,0,         
        0,0,0,0,           
        0,0,0,0,            
        1,0,                
        24,0,              
        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 
    };

    
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


    FILE* f = fopen(filename, "wb");
    fwrite(fileHeader, 1, 14, f);
    fwrite(infoHeader, 1, 40, f);
    fwrite(pixels, 1, 3 * w * h, f);
    fclose(f);

    free(pixels);
}