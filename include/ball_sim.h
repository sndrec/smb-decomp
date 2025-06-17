#ifndef BALL_SIM_H
#define BALL_SIM_H

#include "global.h"
#include "ball.h"

void ball_sim_init(struct Ball *ball);
void ball_sim_step(struct Ball *ball);

#endif // BALL_SIM_H
