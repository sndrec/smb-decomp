#include "mode.h"
#include "window.h"
#include "info.h"
#include "recplay.h"
#include "global.h"
#include "ball.h"
#include "camera.h"
#include "adv.h"
#include "variables.h"
#include <stddef.h>

// Stub global state for WebAssembly build
struct ModeControl modeCtrl __attribute__((weak)) = {0};
s16 gameMode __attribute__((weak)) = 0;
s16 gameModeRequest __attribute__((weak)) = 0;
s16 gameSubmode __attribute__((weak)) = 0;
s16 gameSubmodeRequest __attribute__((weak)) = 0;
void *modeStringPtr __attribute__((weak)) = NULL;
void *submodeStringPtr __attribute__((weak)) = NULL;
void (*lbl_802F1B70)(void) __attribute__((weak)) = NULL;
void (*lbl_802F1B74)(void) __attribute__((weak)) = NULL;
s32 lbl_802F1B78 __attribute__((weak)) = 0;

u32 debugFlags __attribute__((weak)) = 0;

struct Struct801F3A58 infoWork __attribute__((weak)) = {0};

struct ReplayInfo g_recplayInfo __attribute__((weak)) = { {0}, 0.0f, 0 };

struct Ball ballInfo[8] __attribute__((weak)) = {0};
struct Ball *currentBall __attribute__((weak)) = NULL;

struct Camera cameraInfo[5] __attribute__((weak)) = {0};
struct Camera *currentCamera __attribute__((weak)) = NULL;

struct AdvDemoInfo advDemoInfo __attribute__((weak)) = {0};

struct GMA *commonGma __attribute__((weak)) = NULL;
