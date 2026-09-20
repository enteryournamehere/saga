#pragma once

#include "decomp.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nuprim.h"

// Immediate-mode vertex; UV storage is either two floats or packed half UVs.
struct PrimVertexRaw {
    f32 x, y, z;
    u32 color;
    union {
        u32 uv[2];
        f32 float_uv[2];
        u16 half_uv[4];
    };
};
DECOMP_ASSERT(sizeof(PrimVertexRaw) == 0x18, "PrimVertexRaw size");
DECOMP_ASSERT(offsetof(PrimVertexRaw, float_uv) == 0x10, "PrimVertexRaw float UV offset");
DECOMP_ASSERT(offsetof(PrimVertexRaw, half_uv) == 0x10, "PrimVertexRaw half UV offset");

static inline u16 NuRndrFloatToHalf(f32 value) {
    union {
        f32 value;
        u32 bits;
    } conversion = {value};
    i32 mantissa = conversion.bits & 0x7fffff;
    i32 sign = conversion.bits >> 31;
    i32 exponent = static_cast<i32>((conversion.bits >> 23) & 0xff) - 0x70;
    u16 half_exponent = 0;
    if (exponent >= 0) {
        half_exponent = 0x7c00;
        if (exponent < 0x20) {
            half_exponent = static_cast<u16>(exponent * 0x400);
        }
    }
    return static_cast<u16>(mantissa >> 13) | static_cast<u16>(sign << 15) | half_exponent;
}

static inline void NuRndrPrimSetColour(i32 colour) {
    if (g_NuPrim_NeedsOverbrightening)
        ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color = colour;
    else
        ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color =
            ((colour >> 1) & 0x007f7f7f) | (colour & 0xff000000);
}
