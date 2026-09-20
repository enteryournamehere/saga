#pragma once

#include "decomp.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "nu2api/numath/numtx.h"

#ifdef __cplusplus

struct SUIT_s;

typedef struct SIGNAL_s {
    enum {
        FLAG_VISIBLE = 1 << 0,
        FLAG_ACTIVE = 1 << 1,
        FLAG_IN_USE = 1 << 2,
    };

    char name[0x10];
    NUMTX matrix;
    NUVEC position;
    u16 yaw;
    u16 terrain_pitch;
    u16 terrain_roll;
    i16 animation_0;
    i16 animation_1;
    i16 animation_2;
    char character_letter;
    char suit_letter;
    union {
        u8 flags;
        struct {
            u8 visible : 1;
            u8 active : 1;
            u8 in_use : 1;
            u8 reserved_flags : 5;
        };
    };
    u8 pad_6b;
    f32 radius;
    f32 scale;
    NUVEC normal;
    SUIT_s *suit;
    NUVEC target_position;
    i16 platform_id;
    u16 pad_92;
} SIGNAL;

typedef struct SIGNALPROGRESS_s {
    char suit_letters[32];
    u32 active_mask;
    u32 visible_mask;
} SIGNALPROGRESS;

DECOMP_ASSERT(sizeof(SIGNAL) == 0x94, "SIGNAL size");
DECOMP_ASSERT(offsetof(SIGNAL, position) == 0x50, "SIGNAL position offset");
DECOMP_ASSERT(offsetof(SIGNAL, animation_0) == 0x62, "SIGNAL animation indices offset");
DECOMP_ASSERT(offsetof(SIGNAL, flags) == 0x6a, "SIGNAL flags offset");
DECOMP_ASSERT(offsetof(SIGNAL, suit) == 0x80, "SIGNAL suit offset");
DECOMP_ASSERT(offsetof(SIGNAL, target_position) == 0x84, "SIGNAL movement target offset");
DECOMP_ASSERT(offsetof(SIGNAL, platform_id) == 0x90, "SIGNAL platform offset");
DECOMP_ASSERT(sizeof(SIGNALPROGRESS) == 0x28, "SIGNALPROGRESS size");

struct WORLDINFO_s;
struct GameObject_s;

ADDGIZMOTYPE *Signals_RegisterGizmo(i32 type_id);
void Signals_InitTerrain(WORLDINFO_s *world);
SIGNAL *Signal_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, f32 *distance);
void Signal_MoveCode(WORLDINFO_s *world, GameObject_s *object);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
