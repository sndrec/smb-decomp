# Super Monkey Ball Decompilation

This repo contains a WIP disassembly/decompilation of the program code of Super Monkey Ball NTSC-U version (GMBE8P).

It builds the following file:

supermonkeyball.dol: `sha1: 424e8ce10135686de0709a147e6a3a5a3fda02f1`

## Build Instructions

### Required Tools

* [devkitPro](https://devkitpro.org/wiki/Getting_Started)
* CodeWarrior for GameCube compiler (version 1.0, 1.1, or 1.2.5)
* gcc
* make
* wine (only on non-Windows systems)

### Steps

* Create a directory called `mwcc_compiler/<VERSION>` where `<VERSION>` is the version of CodeWarrior for GameCube you have (1.0, 1.1, or 1.2.5).
* Place the following CodeWarrior compiler executables in the aforementioned directory:
  - mwcceppc.exe
  - mwldeppc.exe
  - lmgr326b.dll
* Run `make` from the repository root directory. If you are using a version of CodeWarrior besides 1.1, you must run `make COMPILER_VERSION=<VERSION>` (where `<VERSION>` is your CodeWarrior version).

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

The library can also be compiled with [Emscripten](https://emscripten.org/) to produce a WebAssembly build. Configure the project using `emcmake cmake` and then build the target:

```bash
emcmake cmake -B build .
cmake --build build --target libmkb
```

Alternatively you can invoke `emcc` directly on the source files if you prefer a custom build system.
