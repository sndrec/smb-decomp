cmake_minimum_required(VERSION 3.13)
project(libmkb C)

set(LIBMKB_SOURCES
    src/lib/stage_loader.c
    src/lib/ball_sim.c
    src/lib/camera_sim.c
    src/lib/world_sim.c
    src/lib/stage_anim.c
    src/lib/stobj_sim.c
    src/lib/item_sim.c
    src/world.c
    src/stage.c
    src/stobj.c
    src/stobj_goal.c
    src/item.c
    src/item_coin.c
    src/pool.c
    src/background.c
    src/obj_collision.c
    src/lzs_decompress.c
    src/ball.c
    src/camera.c
    src/event.c
    src/info.c
    src/recplay.c
    src/recplay_cmpr.c
    src/adv.c
    src/window.c
    src/effect.c
    src/sound.c
    src/lib/host_load.c
    src/lib/host_os.c
    src/lib/globals.c
)

add_library(libmkb STATIC ${LIBMKB_SOURCES})
set_target_properties(libmkb PROPERTIES OUTPUT_NAME mkb)

target_include_directories(libmkb PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(libmkb PRIVATE
    LIBMKB_HOST
    NONMATCHING
    C_ONLY
    __GNUC__
)

set_target_properties(libmkb PROPERTIES
    C_STANDARD 99
)

target_compile_options(libmkb PRIVATE -Wall -O3 -s)
