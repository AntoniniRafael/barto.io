#include <stdio.h>
#include <GL/glut.h>

void saveScreenshot() {
    int width = 500, height = 800;

    unsigned char *pixels = (unsigned char*) malloc(3 * width * height);

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    FILE *f = fopen("screenshot.ppm", "wb");

    fprintf(f, "P6\n%d %d\n255\n", width, height);

    for(int y = height-1; y >= 0; y--) {
        fwrite(pixels + (y*width*3), 3, width, f);
    }

    fclose(f);
    free(pixels);
}