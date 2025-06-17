#ifndef CAMERA_SIM_H
#define CAMERA_SIM_H

#include "camera.h"

void camera_sim_init(struct Camera *camera, struct Ball *ball);
void camera_sim_step(struct Camera *camera, struct Ball *ball);

#endif // CAMERA_SIM_H
