#include "mode.h"
#include "window.h"
#include "info.h"
#include "recplay.h"
#include "ball.h"
#include "camera.h"
#include "adv.h"
#include "variables.h"

// Stub global state for WebAssembly build
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

u32 debugFlags = 0;

struct Struct801F3A58 infoWork = {0};

struct ReplayInfo g_recplayInfo = { {0}, 0.0f, 0 };

struct Ball ballInfo[8] = {0};
struct Ball *currentBall = NULL;

struct Camera cameraInfo[5] = {0};
struct Camera *currentCamera = NULL;

struct AdvDemoInfo advDemoInfo = {0};

struct GMA *commonGma = NULL;
