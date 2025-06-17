# libmkb - Super Monkey Ball Simulation Library

This project extracts and refines the gameplay logic of **Super Monkey Ball** into a portable C/C++ library. It no longer attempts to build the original GameCube executable; all effort is directed at maintaining `libmkb` as a clean simulation library.

## Building

### Requirements

* gcc or clang
* make
* [Emscripten](https://emscripten.org/) (optional for WebAssembly builds)
### Building libmkb

Running `make libmkb.a` will compile the standalone simulation library `libmkb.a` using the host compiler. This library contains the stage loading, ball, camera, world, stage object, item and animation simulation code and can be linked into custom frontends. The full list of available functions is documented in [include/libmkb.h](include/libmkb.h).

Example usage:

```c
#include <libmkb.h>

int main(void) {
    struct Ball ball;
    struct Camera camera;

    load_stage_collision(1);       // Load STAGE001
    world_sim_init();
    stobj_sim_init();
    item_sim_init();
    stage_anim_init();
    ball_sim_init(&ball);
    camera_sim_init(&camera, &ball);

    while (1) {
        world_sim_step();
        stobj_sim_step();
        item_sim_step();
        stage_anim_step();
        ball_sim_step(&ball);
        camera_sim_step(&camera, &ball);
        // render here
    }

    stage_anim_destroy();
    item_sim_destroy();
    stobj_sim_destroy();
    world_sim_destroy();
    free_stage_collision();
    return 0;
}
```

### Building libmkb for WebAssembly

The library can also be compiled with [Emscripten](https://emscripten.org/) to produce a WebAssembly build suitable for the browser. A helper script is provided to build the WebAssembly version and export all functions used by the simulation:

```bash
tools/build_wasm.sh
```

This script configures the project with `emcmake`, builds the static library and then links it with `emcc`, exporting all of the simulation entry points and placing `libmkb.js` and `libmkb.wasm` in the `webdemo` directory.

Alternatively you can invoke `emcc` directly on the source files if you prefer a custom build system.

### Web Frontend Demo

A simple three.js based demo is provided in the `webdemo` directory. After
building `libmkb.js` and `libmkb.wasm` with Emscripten copy both files into the
`webdemo` folder. The demo script will automatically instantiate the
WebAssembly module when the page loads. You can then serve the directory with
any static file server:

```bash
cd webdemo
python3 -m http.server
```

Navigate to <http://localhost:8000> to see the simulation running in your
browser.
