#include <emscripten/bind.h>

uint8_t *buffer = nullptr;
size_t bufferSize = 0;

inline double point(double x, double y, int iterations, int limit) 
{
    double real = x;
    double imaginary = y;    
    double nextReal, nextImaginary;
    for (int i = 0; i < iterations; i++) 
    {
        nextReal = real*real - imaginary*imaginary + x;
        nextImaginary = 2 * real * imaginary + y;
        real = nextReal;
        imaginary = nextImaginary;
        if (real * imaginary > limit) 
        {
            return (double)i / iterations;
        }
    }
    return 0;
} 

inline double min(double a, double b) {
    return a < b ? a : b;
}

inline double max(double a, double b) {
    return a > b ? a : b;
}

inline uint8_t round(double a) {
    uint8_t t = (uint8_t) a;
    return a - t < 0.5 ? a : a + 1;    
}

emscripten::val mandelbrot(int width, int height, int iterations, int limit, double zoom, double panX, double panY) 
{
    // allocate (only first time)
    if (buffer == nullptr) 
    {
        bufferSize = width * height * 4;
        buffer = (uint8_t *)malloc(bufferSize);
    } 
    
    // render
    int offset = 0;
    double mx, my, intensity;
    for (int y = 0; y < height; y++) 
    {
        my = (double)y / zoom - panY;
        for (int x = 0; x < width; x++) 
        {
            mx = (double)x / zoom - panX;
            intensity = point(mx, my, iterations, limit);     
            buffer[offset++] = round(min(1.0, intensity * 2) * 255);
            buffer[offset++] = round(max(0.0, intensity - 0.5) * 2 * 255);
            buffer[offset++] = 0;
            buffer[offset++] = 255;            
       } 
    } 

    // return as typed array
    return emscripten::val(emscripten::typed_memory_view(bufferSize, buffer));
}

EMSCRIPTEN_BINDINGS(hello)
{
    emscripten::function("mandelbrot", &mandelbrot);
}
