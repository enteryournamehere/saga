#pragma once

#include "legoapi/gizmo/base/gizmo.h"
#include "decomp.h"

#ifdef __cplusplus

typedef struct GUIDELINE_s {
    char name[8];
    NUVEC start_position;
    NUVEC end_position;
    union {
        u8 flags;
        struct {
            u8 active : 1;
            u8 visible : 1;
            u8 reserved_flags : 6;
        };
    };
    u8 reserved_21[3];
} GUIDELINE;

DECOMP_ASSERT(sizeof(GUIDELINE) == 0x24, "GUIDELINE ABI");
DECOMP_ASSERT(offsetof(GUIDELINE, flags) == 0x20, "GUIDELINE flags offset");

GUIDELINE *GuideLine_FindNearest(nuvec_s *position, WORLDINFO_s *world, i32 *endpoint, float *distance);
ADDGIZMOTYPE *GuideLines_RegisterGizmo(i32 type_id);

#endif
