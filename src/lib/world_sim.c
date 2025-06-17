#include "world.h"

// Initialize world state for all players
void world_sim_init(void)
{
    ev_world_init();
}

// Update world tilt and gravity for one frame
void world_sim_step(void)
{
    ev_world_main();
}

// Clean up world state
void world_sim_destroy(void)
{
    ev_world_dest();
}
