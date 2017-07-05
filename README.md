# wasm-mandelbrot
WebAssembly Mandelbrot POCs


## Install
First, you need to install the dependencies for dealing with WebAssembly.

http://webassembly.org/getting-started/developers-guide/

Notes:
- max open file issue
  - https://superuser.com/a/303058  
  - `sudo launchctl limit maxfiles 1000000 1000000`
- missing folder
  - emsdk/clang/fastcomp/src - no such file or directory  
  - make sure you installed cmake and it's available on the PATH
- missing privileges for folders
  - `sudo chown -R $(whoami) /usr/local/opt`
  
## Run
1. Activate Emscripten
  - `source <EMSDK-FOLDER>/emsdk_env.sh`
2. Host the main project folder
  - Do this in a different Terminal session, since you'll need the activated Emscripten, to compile wasm.
  - `cd <PROJECT-FOLDER>`
  - `http-server` (but you can use Apache or anything)
3. Open the `fractal.html` from one of the subfolders
  - http://localhost:8080/01-javascript/fractal.html
4. Check the console for the measurement or potential errors


## Subfoldres
There are multiple way of doing this. Each subfolder contains a different solution. 


### 01-javascript
This is the JavaScript version without using WebAssembly. 

Time spent: 135 ms


### 02-c-buffptr
This is the C version, allocating its own buffer and making it accessible from JavaScript.
You can recompile `mandelbrot.c` to generate `mandelbrot.wasm` and `mandelbrot.js` using:

`emcc mandelbrot.c -o mandelbrot.js -s WASM=1 -s TOTAL_MEMORY=67108864 -s EXPORTED_FUNCTIONS="['_render','_buffer_ref']"`

You can try different compiler optimization settings, by appending standard compiler flags, like `-O3` (that's a capital o letter there).

Time spent: 147 ms (with -O3)

#### I've included a main method in the C code, so you can run and measure it natively:

`gcc mandelbrot.c -o mandelbrot -O3`

`./mandelbrot`

Time spent: 127 ms (with -O3)

So, the native code is not much faster either. That makes me belive that WebAssembly isn't unexpectedly slow, rather JavaScript is unexpectedly fast.


### 03-cpp-buffptr
This is the same thing, just written in C++.
You can recompile `mandelbrot.cpp` to generate `mandelbrot.wasm` and `mandelbrot.js` using:

`em++ mandelbrot.cpp -o mandelbrot.js -s WASM=1 -s TOTAL_MEMORY=67108864 -s EXPORTED_FUNCTIONS="['_render','_buffer_ref']"`

Time spent: 130 ms (with -O3)

#### I've included a main method in this C++ code, so you can run and measure it natively:

`g++ -std=c++0x mandelbrot.cpp -o mandelbrot -O3`

`./mandelbrot`

Time spent: 125 ms (with -O3)


### 04-cpp-bind
This is also in C++, but using Emscripten's bind macro and types to `return` the results to JavaScript.

This one also has some optimizations I tried out: 

- inline-ing functions, 
- declaring the variables outside the heavy loops,
- added some compiler flags to encourage more optimization.

Sadly, that doesn't make much of a difference.

You can recompile `mandelbrot.cpp` to generate `mandelbrot.wasm` and `mandelbrot.js` using:

`em++ --bind mandelbrot.cpp -o mandelbrot.js -s WASM=1 -O2 --llvm-opts 3`

Time spent: 237 ms (first run), 134 ms (subsequent calls)

#### No main method here. 
Emscripten's macros and types are not available, so it can't be compiled to a standalone executable.
