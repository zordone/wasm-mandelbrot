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
