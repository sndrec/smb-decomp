#ifndef BALL_SIM_H
#define BALL_SIM_H

#include "global.h"
#include "ball.h"

#ifdef __cplusplus
extern "C" {
#endif

void ball_sim_init(struct Ball *ball);
void ball_sim_step(struct Ball *ball);

#ifdef __cplusplus
}
#endif

#endif // BALL_SIM_H
