#include "mode.h"
#include "variables.h"
#include "input.h"
#include "types.h"
#include "mathutil.h"
#include <stddef.h>
#include <stdlib.h>

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
struct GMA *decodedBgGma = NULL;
struct TPL *decodedBgTpl = NULL;
struct GMA *minigameGma = NULL;
struct TPL *g_commonNlTpl = NULL;
struct TPL *g_stageNlTpl = NULL;
struct TPL *g_bgNlTpl = NULL;
struct TPL *g_minigameNlTpl = NULL;
struct TPL *lbl_802F1AE4 = NULL;
struct NlObj *g_commonNlObj = NULL;
struct NlObj *g_stageNlObj = NULL;
struct NlObj *g_bgNlObj = NULL;
struct NlObj *g_minigameNlObj = NULL;
struct BgLightInfo g_bgLightInfo = {0};
struct GFXBufferInfo *gfxBufferInfo = NULL;

// Rendering configuration (defaults for host build)
GXRenderModeObj libmkbRenderMode = {
    .fbWidth = 640,
    .efbHeight = 480,
    .xfbHeight = 480,
};
GXRenderModeObj *currRenderMode = &libmkbRenderMode;

// Mathutil working area
static struct MathutilData libmkbMathutilData;
struct MathutilData *mathutilData = &libmkbMathutilData;

// Order table workspace used by camera
static struct Struct802F1B3C libmkbUserWork;
struct Struct802F1B3C *userWork = &libmkbUserWork;

// Input state
struct ControllerInfo controllerInfo[4] = {0};
struct ControllerInfo lbl_801F3C60[4] = {0};
struct AnalogInput analogInputs[4] = {0};
u16 g_currPlayerButtons[6] = {0};
u16 g_currPlayerAnalogButtons[6] = {0};

// Timers used by some effects
u32 globalAnimTimer = 0;
u32 powerOnTimer = 0;

void mkb_init(void)
{
    // Initialize mathutil workspace
    mathutilData->constZeroF = 0.0f;
    mathutilData->constOneF = 1.0f;
    mathutilData->constOneHalfF = 0.5f;
    mathutilData->constThreeHalvesF = 1.5f;
    mathutilData->unk1A8 = -1;
    mathutilData->constZeroOne[0] = 0;
    mathutilData->constZeroOne[1] = 1;
    mathutilData->constOneZero[0] = 1;
    mathutilData->constOneZero[1] = 0;
    mathutil_mtx_from_identity(mathutilData->mtxA);
    mathutil_mtxA_to_mtxB();
    mathutilData->mtxStack = malloc(32 * sizeof(Mtx));
    mathutilData->mtxStackPtr = mathutilData->mtxStack + 32;
}
