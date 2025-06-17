#ifndef STAGE_LOADER_H
#define STAGE_LOADER_H

#include "stage.h"

extern struct Stage *decodedStageLzPtr;

void load_stage_collision(int stageId);
void load_stage_collision_file(const char *path);
void free_stage_collision(void);

#endif // STAGE_LOADER_H
