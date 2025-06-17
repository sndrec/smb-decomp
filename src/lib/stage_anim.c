#include "stage.h"

// Initialize stage animation state
void stage_anim_init(void)
{
    ev_stage_init();
}

// Advance stage animation logic for one frame
void stage_anim_step(void)
{
    ev_stage_main();
}

// Clean up stage animation resources
void stage_anim_destroy(void)
{
    ev_stage_dest();
}
