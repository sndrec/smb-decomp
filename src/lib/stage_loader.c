#include "dolphin.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "ppcintrinsic.h"

#include "global.h"
#include "load.h"
#include "stage_loader.h"
#include "functions.h"

u8 lbl_8020AE00[0x20] __attribute__((aligned(32)));
FORCE_BSS_ORDER(lbl_8020AE00)

struct Stage *decodedStageLzPtr;

#undef OFFSET_TO_PTR
#define OFFSET_TO_PTR(base, offset) (void *)((u32)(offset) + (u32)(base))

void load_stage_collision_file(const char *path)
{
    struct File file;
    u8 unused[8];
    u32 compSize;
    u32 uncompSize;
    void *compData;
    void *uncompData;
    struct StageAnimGroup *coll;
    int i;

    if (!file_open(path, &file))
        OSPanic("stage_loader.c", __LINE__, "cannot Open");

    if (file_read(&file, lbl_8020AE00, 32, 0) < 0)
        OSPanic("stage_loader.c", __LINE__, "cannot Read");
    // Stage data provided with the web demo stores its header in little-endian
    // order. Convert the values before allocating buffers.
    compSize = OSRoundUp32B(read_u32_le(lbl_8020AE00, 0));
    uncompSize = OSRoundUp32B(read_u32_le(lbl_8020AE00, 4));

    uncompData = malloc(uncompSize);
    if (uncompData == NULL)
        OSPanic("stage_loader.c", __LINE__, "cannot malloc");
    compData = malloc(compSize);
    if (compData == NULL)
        OSPanic("stage_loader.c", __LINE__, "cannot malloc");

    if (file_read(&file, compData, compSize, 0) < 0)
        OSPanic("stage_loader.c", __LINE__, "cannot Read");
    if (file_close(&file) != 1)
        OSPanic("stage_loader.c", __LINE__, "cannot Close");

    lzs_decompress(compData, uncompData);
    free(compData);

    decodedStageLzPtr = uncompData;
    if (uncompData == NULL)
        OSPanic("stage_loader.c", __LINE__, "cannot open stcoli\n");
    decodedStageLzPtr->animGroups = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->animGroups);

    coll = decodedStageLzPtr->animGroups;
    for (i = 0; i < decodedStageLzPtr->animGroupCount; i++, coll++)
    {
        if (coll->anim != NULL)
            adjust_stage_anim_ptrs(&coll->anim, decodedStageLzPtr);
        if (coll->modelNames != NULL)
        {
            char **namep;
            coll->modelNames = OFFSET_TO_PTR(decodedStageLzPtr, coll->modelNames);
            namep = coll->modelNames;
            while (*namep != NULL)
            {
                *namep = OFFSET_TO_PTR(decodedStageLzPtr, *namep);
                namep++;
            }
        }
        if (coll->triangles != NULL)
            coll->triangles = OFFSET_TO_PTR(decodedStageLzPtr, coll->triangles);
        if (coll->gridCellTris != NULL)
        {
            int j;
            s16 **r5;

            coll->gridCellTris = OFFSET_TO_PTR(decodedStageLzPtr, coll->gridCellTris);
            for (j = 0, r5 = coll->gridCellTris; j < coll->gridCellCountX * coll->gridCellCountZ;
                 j++, r5++)
            {
                if (*r5 != NULL)
                    *r5 = OFFSET_TO_PTR(decodedStageLzPtr, *r5);
            }
        }
        if (coll->goals != NULL)
            coll->goals = OFFSET_TO_PTR(decodedStageLzPtr, coll->goals);
        if (coll->unk48 != NULL)
            coll->unk48 = OFFSET_TO_PTR(decodedStageLzPtr, coll->unk48);
        if (coll->bumpers != NULL)
            coll->bumpers = OFFSET_TO_PTR(decodedStageLzPtr, coll->bumpers);
        if (coll->jamabars != NULL)
            coll->jamabars = OFFSET_TO_PTR(decodedStageLzPtr, coll->jamabars);
        if (coll->bananas != NULL)
            coll->bananas = OFFSET_TO_PTR(decodedStageLzPtr, coll->bananas);
        if (coll->coliCones != NULL)
            coll->coliCones = OFFSET_TO_PTR(decodedStageLzPtr, coll->coliCones);
        if (coll->coliSpheres != NULL)
            coll->coliSpheres = OFFSET_TO_PTR(decodedStageLzPtr, coll->coliSpheres);
        if (coll->coliCylinders != NULL)
            coll->coliCylinders = OFFSET_TO_PTR(decodedStageLzPtr, coll->coliCylinders);
        if (coll->animGroupModels != NULL)
        {
            struct AnimGroupModel *animGroupModel;
            int j;

            coll->animGroupModels = OFFSET_TO_PTR(decodedStageLzPtr, coll->animGroupModels);
            for (j = 0, animGroupModel = coll->animGroupModels; j < coll->animGroupModelCount; j++, animGroupModel++)
                animGroupModel->name = OFFSET_TO_PTR(decodedStageLzPtr, animGroupModel->name);
        }
        if (coll->unk88 != NULL)
            coll->unk88 = OFFSET_TO_PTR(decodedStageLzPtr, coll->unk88);
        if (coll->unk90 != NULL)
        {
            struct DecodedStageLzPtr_child_child4 *r4;
            int j;

            coll->unk90 = OFFSET_TO_PTR(decodedStageLzPtr, coll->unk90);
            for (j = 0, r4 = coll->unk90; j < coll->unk8C; j++, r4++)
                r4->unk0 = OFFSET_TO_PTR(decodedStageLzPtr, r4->unk0);
        }
    }

    if (decodedStageLzPtr->startPos != NULL)
        decodedStageLzPtr->startPos = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->startPos);
    if (decodedStageLzPtr->pFallOutY != NULL)
        decodedStageLzPtr->pFallOutY = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->pFallOutY);
    if (decodedStageLzPtr->goals != NULL)
        decodedStageLzPtr->goals = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->goals);
    if (decodedStageLzPtr->unk24 != NULL)
        decodedStageLzPtr->unk24 = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->unk24);
    if (decodedStageLzPtr->bumpers != NULL)
        decodedStageLzPtr->bumpers = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->bumpers);
    if (decodedStageLzPtr->jamabars != NULL)
        decodedStageLzPtr->jamabars = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->jamabars);
    if (decodedStageLzPtr->bananas != NULL)
        decodedStageLzPtr->bananas = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->bananas);
    if (decodedStageLzPtr->coliCones != NULL)
        decodedStageLzPtr->coliCones = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->coliCones);
    if (decodedStageLzPtr->coliCylinders != NULL)
        decodedStageLzPtr->coliCylinders = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->coliCylinders);
    if (decodedStageLzPtr->animGroupModels != NULL)
        decodedStageLzPtr->animGroupModels = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->animGroupModels);
    if (decodedStageLzPtr->unk64 != NULL)
        decodedStageLzPtr->unk64 = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->unk64);
    if (decodedStageLzPtr->mirrors != NULL)
        decodedStageLzPtr->mirrors = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->mirrors);

    if (decodedStageLzPtr->bgObjects != NULL)
    {
        struct StageBgObject *bgObj;

        decodedStageLzPtr->bgObjects = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->bgObjects);
        for (i = 0, bgObj = decodedStageLzPtr->bgObjects; i < decodedStageLzPtr->bgObjectCount;
             i++, bgObj++)
        {
            u32 r3 = bgObj->flags;

            if (r3 & (1 << 15))
            {
                bgObj->flags &= 0xF;
                bgObj->flags |= (r3 >> 12) & 0xFFFF0;
            }
            bgObj->name = OFFSET_TO_PTR(decodedStageLzPtr, bgObj->name);
            if (bgObj->anim != NULL)
                func_800473C0(&bgObj->anim, decodedStageLzPtr);
            if (bgObj->flipbooks != NULL)
                adjust_stage_flipbook_anims_ptrs(&bgObj->flipbooks, decodedStageLzPtr);
        }
    }

    if (decodedStageLzPtr->fgObjects != NULL)
    {
        struct StageBgObject *bgObj;

        decodedStageLzPtr->fgObjects = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->fgObjects);
        for (i = 0, bgObj = decodedStageLzPtr->fgObjects; i < decodedStageLzPtr->fgObjectCount; i++, bgObj++)
        {
            u32 r3 = bgObj->flags;

            if (r3 & (1 << 15))
            {
                bgObj->flags = r3 & 0xF;
                bgObj->flags |= (r3 >> 12) & 0xFFFF0;
            }
            bgObj->name = OFFSET_TO_PTR(decodedStageLzPtr, bgObj->name);
            if (bgObj->anim != NULL)
                func_800473C0(&bgObj->anim, decodedStageLzPtr);
            if (bgObj->flipbooks != NULL)
                adjust_stage_flipbook_anims_ptrs(&bgObj->flipbooks, decodedStageLzPtr);
        }
    }

    if (decodedStageLzPtr->unk78 != NULL)
    {
        int j;
        struct DecodedStageLzPtr_child5 *r3;
        struct DecodedStageLzPtr_child5_child *r6;

        r3 = decodedStageLzPtr->unk78 = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->unk78);
        if (r3->unk4 != NULL)
            r3->unk4 = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk4);
        if (r3->unkC != NULL)
            r3->unkC = OFFSET_TO_PTR(decodedStageLzPtr, r3->unkC);
        if (r3->unk14 != NULL)
            r3->unk14 = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk14);
        if (r3->unk1C != NULL)
            r3->unk1C = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk1C);
        r6 = r3->unk1C;
        for (j = 0; j < r3->unk18; j++, r6++)
        {
            if (r6->unk4 != NULL)
                r6->unk4 = OFFSET_TO_PTR(decodedStageLzPtr, r6->unk4);
            if (r6->unkC != NULL)
                r6->unkC = OFFSET_TO_PTR(decodedStageLzPtr, r6->unkC);
            if (r6->unk14 != NULL)
                r6->unk14 = OFFSET_TO_PTR(decodedStageLzPtr, r6->unk14);
        }
        if (r3->unk24 != NULL)
            r3->unk24 = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk24);
        if (r3->unk2C != NULL)
            r3->unk2C = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk2C);
        if (r3->unk34 != NULL)
            r3->unk34 = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk34);
        if (r3->unk3C != NULL)
            r3->unk3C = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk3C);
        if (r3->unk44 != NULL)
            r3->unk44 = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk44);
        if (r3->unk4C != NULL)
            r3->unk4C = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk4C);
        if (r3->unk54 != NULL)
            r3->unk54 = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk54);
        if (r3->unk5C != NULL)
            r3->unk5C = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk5C);
        if (r3->unk64 != NULL)
            r3->unk64 = OFFSET_TO_PTR(decodedStageLzPtr, r3->unk64);
    }

    if (decodedStageLzPtr->unk88 != NULL)
    {
        decodedStageLzPtr->unk88 = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->unk88);
        if (decodedStageLzPtr->unk88->unkC != NULL)
            adjust_stage_anim_ptrs(&decodedStageLzPtr->unk88->unkC, decodedStageLzPtr);
        if (decodedStageLzPtr->unk88->unk10 != NULL)
            adjust_stage_anim_ptrs(&decodedStageLzPtr->unk88->unk10, decodedStageLzPtr);
    }

    if (decodedStageLzPtr->unk90 != NULL)
        decodedStageLzPtr->unk90 = OFFSET_TO_PTR(decodedStageLzPtr, decodedStageLzPtr->unk90);
    if (decodedStageLzPtr->unk7C < 1)
        decodedStageLzPtr->unk7C = 1;
}

void load_stage_collision(int stageId)
{
    char filename[32];
    sprintf(filename, "STAGE%03d.lz", stageId);
    load_stage_collision_file(filename);
}

void free_stage_collision(void)
{
    if (decodedStageLzPtr != NULL)
    {
        free(decodedStageLzPtr);
        decodedStageLzPtr = NULL;
    }
}
