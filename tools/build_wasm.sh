#!/bin/bash
set -e

# Build libmkb with Emscripten and export all functions required by the
# simulation. The resulting libmkb.js and libmkb.wasm are placed in the
# webdemo directory.

BUILD_DIR=build-wasm

emcmake cmake -B "$BUILD_DIR" .
cmake --build "$BUILD_DIR" --target libmkb

emcc "$BUILD_DIR/libmkb.a" \
  -s MODULARIZE=1 \
  -s EXPORT_NAME=createMKBModule \
  -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
  -s EXPORTED_FUNCTIONS="['_load_stage_collision','_free_stage_collision','_ball_sim_init','_ball_sim_step','_camera_sim_init','_camera_sim_step','_world_sim_init','_world_sim_step','_world_sim_destroy','_stobj_sim_init','_stobj_sim_step','_stobj_sim_destroy','_item_sim_init','_item_sim_step','_item_sim_destroy','_stage_anim_init','_stage_anim_step','_stage_anim_destroy','_malloc']" \
  -s EXPORTED_RUNTIME_METHODS="['cwrap','getValue','FS_createDataFile']" \
  -o webdemo/libmkb.js
