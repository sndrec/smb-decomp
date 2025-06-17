#include "types.h"
#include "global.h"
#include "stobj.h"

// Initialize stage object system
void stobj_sim_init(void)
{
    ev_stobj_init();
}

// Update stage objects for one frame
void stobj_sim_step(void)
{
    ev_stobj_main();
}

// Destroy stage object system
void stobj_sim_destroy(void)
{
    ev_stobj_dest();
}
