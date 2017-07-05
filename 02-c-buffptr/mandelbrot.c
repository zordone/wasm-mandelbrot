#include <stdint.h>
#include <math.h>
// for the main only
#include <time.h>
#include <stdio.h>

#define BUFSIZE 1250 * 800 * 4
static uint8_t data[BUFSIZE];
static uint8_t *buffer = (uint8_t *) &data;

uint8_t *buffer_ref() {
    return buffer;
}

float point(float x, float y, int iterations, int limit) {
    float real = x;
    float imaginary = y;    
    for (int i = 0; i < iterations; i++) {
        float nextReal = real*real - imaginary*imaginary + x;
        float nextImaginary = 2 * real * imaginary + y;
        real = nextReal;
        imaginary = nextImaginary;
        if (real * imaginary > limit) {
            return (float)i / iterations;
        }
    }
    return 0;
}   

void render(int width, int height, int iterations, int limit, float zoom, float panX, float panY) {        
    int offset = 0;
    for (int y = 0; y < height; y++) {
        float my = (float)y / zoom - panY;
        for (int x = 0; x < width; x++) {
            float mx = (float)x / zoom - panX;
            float intensity = point(mx, my, iterations, limit);     
            buffer[offset++] = (uint8_t) roundf(fminf(1, intensity * 2) * 255);
            buffer[offset++] = (uint8_t) roundf(fmaxf(0, intensity - 0.5) * 2 * 255);
            buffer[offset++] = 0;
            buffer[offset++] = 255;            
       } 
    }
}

// this is used only measuring the C code directly
int main(int argc, char * argv []) {
    clock_t begin = clock();

    render(1250, 800, 120, 10, 2000.0, 1.4, 0.55);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC * 1000;
    printf("Elapsed: %f ms\n", time_spent);
    
    return 0;
}