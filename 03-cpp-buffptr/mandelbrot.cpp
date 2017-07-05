#include <iostream>
#include <math.h>
#include <time.h>

#define BUFSIZE 1250 * 800 * 4
static uint8_t data[BUFSIZE];
static uint8_t *buffer = (uint8_t *) &data;

extern "C" uint8_t *buffer_ref() 
{
    return buffer;
}

extern "C" inline double point(double x, double y, int iterations, int limit) 
{
    double real = x;
    double imaginary = y;    
    double nextReal, nextImaginary;
    for (int i = 0; i < iterations; i++) {
        nextReal = real*real - imaginary*imaginary + x;
        nextImaginary = 2 * real * imaginary + y;
        real = nextReal;
        imaginary = nextImaginary;
        if (real * imaginary > limit) {
            return (double)i / iterations;
        }
    }
    return 0;
} 

extern "C" void render(int width, int height, int iterations, int limit, double zoom, double panX, double panY) 
{
    int offset = 0;
    double my, mx, intensity;
    for (int y = 0; y < height; y++) {
        my = (double)y / zoom - panY;
        for (int x = 0; x < width; x++) {
            mx = (double)x / zoom - panX;
            intensity = point(mx, my, iterations, limit);     
            buffer[offset++] = (uint8_t) roundf(fminf(1, intensity * 2) * 255);
            buffer[offset++] = (uint8_t) roundf(fmaxf(0, intensity - 0.5) * 2 * 255);
            buffer[offset++] = 0;
            buffer[offset++] = 255;            
       } 
    }    
}


int main(int argc, char * argv []) 
{
    clock_t begin = clock();

    render(1250, 800, 120, 10, 2000.0, 1.4, 0.55);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC * 1000;
    printf("Elapsed: %f ms\n", time_spent);
    
    return 0;
}