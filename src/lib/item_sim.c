#include "types.h"
#include "global.h"
#include "item.h"

// Initialize item system
void item_sim_init(void)
{
    ev_item_init();
}

// Advance item behavior for one frame
void item_sim_step(void)
{
    ev_item_main();
}

// Clean up item resources
void item_sim_destroy(void)
{
    ev_item_dest();
}
