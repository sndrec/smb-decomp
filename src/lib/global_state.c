#include "mode.h"
#include "variables.h"
#include <stddef.h>

// Global game state now lives inside libmkb so it can be modified directly
struct ModeControl modeCtrl = {0};
s16 gameMode = 0;
s16 gameModeRequest = 0;
s16 gameSubmode = 0;
s16 gameSubmodeRequest = 0;
void *modeStringPtr = NULL;
void *submodeStringPtr = NULL;
void (*lbl_802F1B70)(void) = NULL;
void (*lbl_802F1B74)(void) = NULL;
s32 lbl_802F1B78 = 0;
void (*unusedCallback)(void) = NULL;
void (*submodeFinishFunc)(void) = NULL;

// Graphics assets used by several systems
struct GMA *commonGma = NULL;
