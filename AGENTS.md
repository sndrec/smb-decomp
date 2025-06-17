# Codex Agent Instructions

This repository is dedicated to building **libmkb**, a standalone C/C++ library that simulates the physics, camera, and control logic of **Super Monkey Ball**. We no longer attempt to compile the original GameCube executable or preserve its exact structure. All code should move toward a clean, portable library focused solely on gameplay simulation. These are the guiding principles for all future contributions:

## Goals
- **Readability**: produce clear, well-documented code that faithfully reflects the original game's logic so that it can be easily understood.
- **Accuracy**: the resulting library should behave identically to the original game aside from unavoidable floating-point rounding differences.
- **Embeddability**: the core logic should compile as a reusable C++ library for use in custom frontends. Compiling libmkb for the browser via WebAssembly with three.js for graphics is the endgoal for an initial frontend.

## Non-Goals
- **Perfect matching** of the original binary is not required and we no longer maintain the ability to produce the original `*.dol` file.
- **Full completeness** (e.g. menus, graphics, character animation) is not a concern. Focus on physics, camera, and controls.

Code should be structured into self-contained modules with minimal dependencies so that `libmkb` can be embedded easily. Codex agents should follow these principles when decompiling, refactoring, or adding functionality.

## Testing

Always run `npm test` before submitting a pull request. The test suite consists
of several Node-based smoke tests:

* `tests/webdemo.js` executes `webdemo/main.js` using mocked browser globals to
  ensure the script loads without errors.
* `tests/build_wasm.js` invokes `tools/build_wasm.sh` when `emcc` is available
  and verifies that `libmkb.wasm` is produced.
* `tests/webdemo_sim.js` runs the real WebAssembly build. It warms up the
  simulation for 500 frames before applying random controller input and then
  checks that the ball and camera positions change.
