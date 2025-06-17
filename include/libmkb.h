#ifndef LIBMKB_H
#define LIBMKB_H

#include "stage.h"
#include "ball.h"
#include "camera.h"
#include "world.h"

#ifdef __cplusplus
extern "C" {
#endif

void load_stage_collision(int stageId);
void free_stage_collision(void);

void ball_sim_init(struct Ball *ball);
void ball_sim_step(struct Ball *ball);

void camera_sim_init(struct Camera *camera, struct Ball *ball);
void camera_sim_step(struct Camera *camera, struct Ball *ball);

void world_sim_init(void);
void world_sim_step(void);
void world_sim_destroy(void);

#ifdef __cplusplus
}
#endif

#endif // LIBMKB_H
