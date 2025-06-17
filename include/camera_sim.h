#ifndef CAMERA_SIM_H
#define CAMERA_SIM_H

#include "global.h"
#include "camera.h"
#include "ball.h"

#ifdef __cplusplus
extern "C" {
#endif

void camera_sim_init(struct Camera *camera, struct Ball *ball);
void camera_sim_step(struct Camera *camera, struct Ball *ball);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_SIM_H
