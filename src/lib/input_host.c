#include "input.h"
#include <string.h>

void mkb_set_controller_state(int port, const PADStatus *pad)
{
    if (port < 0 || port >= 4 || !pad)
        return;
    struct ControllerInfo *c = &controllerInfo[port];
    c->prevHeld = c->held;
    c->held = *pad;
    c->pressed.button  = pad->button & ~c->prevHeld.button;
    c->released.button = c->prevHeld.button & ~pad->button;
    c->repeat.button   = c->pressed.button;

    struct AnalogInput *a = &analogInputs[port];
    a->prevHeld = a->held;
    a->held = 0;
    if (pad->stickX < -40) a->held |= ANALOG_STICK_LEFT;
    if (pad->stickX > 40)  a->held |= ANALOG_STICK_RIGHT;
    if (pad->stickY < -40) a->held |= ANALOG_STICK_DOWN;
    if (pad->stickY > 40)  a->held |= ANALOG_STICK_UP;
    if (pad->substickX < -40) a->held |= ANALOG_CSTICK_LEFT;
    if (pad->substickX > 40)  a->held |= ANALOG_CSTICK_RIGHT;
    if (pad->substickY < -40) a->held |= ANALOG_CSTICK_DOWN;
    if (pad->substickY > 40)  a->held |= ANALOG_CSTICK_UP;
    if (pad->triggerLeft  > 0x10) a->held |= ANALOG_TRIGGER_LEFT;
    if (pad->triggerRight > 0x10) a->held |= ANALOG_TRIGGER_RIGHT;
    a->pressed  = a->held & ~a->prevHeld;
    a->released = a->prevHeld & ~a->held;
    a->repeat   = a->pressed;

    g_currPlayerButtons[port] = c->held.button;
}
