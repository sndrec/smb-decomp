#include "ball_sim.h"
#include "ball.h"
#include "world.h"

// Local copy of the state dispatch table used by the main game
static void (*const ball_sim_funcs[])(struct Ball *) = {
    ball_func_0,              // BALL_STATE_0
    ball_func_1,              // BALL_STATE_1
    ball_func_ready_main,     // BALL_STATE_READY_MAIN
    ball_func_3,              // BALL_STATE_3
    ball_func_4,              // BALL_STATE_4
    ball_func_goal_init,      // BALL_STATE_GOAL_INIT
    ball_func_goal_main,      // BALL_STATE_GOAL_MAIN
    ball_func_replay_init,    // BALL_STATE_REPLAY_INIT_1
    ball_func_replay_main,    // BALL_STATE_REPLAY_MAIN_1
    ball_func_replay_init,    // BALL_STATE_REPLAY_INIT_2
    ball_func_replay_main,    // BALL_STATE_REPLAY_MAIN_2
    ball_func_11,
    ball_func_12,
    ball_func_13,
    ball_func_14,
    ball_func_15,
    ball_func_16,
    ball_func_17,
    ball_func_18,
    ball_func_19,
    ball_func_20,
    ball_func_demo_init,
    ball_func_mini,
    ball_func_mini,
    ball_func_mini,
    ball_func_mini,
    ball_func_mini,
    ball_func_27,
    ball_func_28,
};

static void clear_some_ball_flags_inline(struct Ball *ball)
{
    ball->flags &= ~(BALL_FLAG_27|BALL_FLAG_28|BALL_FLAG_29|BALL_FLAG_30|BALL_FLAG_31);
}

// Initialize ball for main gamemode
void ball_sim_init(struct Ball *ball)
{
    // Use existing initialization routines
    // Equivalent to calling ball_func_ready_main after basic setup
    ball_func_1(ball);
}

// Step ball simulation for one frame in the main gamemode
void ball_sim_step(struct Ball *ball)
{
    // The normal event loop updates all balls in ev_ball_main.
    // Here we simulate only a single ball for external library use.
    currentBall = ball;
    ball->unk120 = ball->flags;
    ball->flags &= ~(BALL_FLAG_00|BALL_FLAG_02);
    clear_some_ball_flags_inline(ball);
    set_ball_look_point(4, &ball->pos, 0.75f);
    ball_sim_funcs[ball->state](ball);
    func_80038528(ball);
    ball->u_opacity[0] = ball->u_opacity[1] = ball->u_opacity[2] = ball->u_opacity[3] = 1.0f;
    ball_ape_yang(ball);
    ball_effect();
    animate_ball_size_change(ball);
    ball_sound(ball);
    if (ball->unk14E > 0)
        ball->unk14E--;
}
