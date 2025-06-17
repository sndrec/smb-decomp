#include "camera_sim.h"

extern void (*cameraFuncs[])(struct Camera *, struct Ball *);

void camera_sim_init(struct Camera *camera, struct Ball *ball)
{
    // Set up camera using existing initialization logic
    camera_init();
    ev_camera_init();
    camera->state = CAMERA_STATE_READY_INIT;
    cameraFuncs[camera->state](camera, ball);
}

void camera_sim_step(struct Camera *camera, struct Ball *ball)
{
    currentCamera = camera;
    cameraFuncs[camera->state](camera, ball);
}
