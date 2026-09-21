#ifndef GAMELIB_UTIL_NETOUTPUTSTREAM_H
#define GAMELIB_UTIL_NETOUTPUTSTREAM_H
#pragma once

#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/gameapi_edtools_types.h"

struct NetOutputStream : EdOutputStream {
    NetMessage *message;

    NetOutputStream() {
        mode = 2;
    }

    virtual ~NetOutputStream() {}

    virtual i32 Eat(i32, i32) {
        return 0;
    }

    virtual i32 SerialiseBuffer(void *buffer, i32 element_size, i32 count) {
        NetMessage &output = *message;
        i32 size = element_size * count;
        if (output.data != NULL) {
            memmove(output.data->bytes + output.write_offset, buffer, size);
            if (output.swap_endianness && element_size > 1) {
                u8 *destination = output.data->bytes + output.write_offset;
                for (i32 i = 0; i < count; ++i) {
                    if (element_size == 2) {
                        EdFileSwapEndianess16(destination);
                        destination += 2;
                    } else if (element_size == 4) {
                        EdFileSwapEndianess32(destination);
                        destination += 4;
                    }
                }
            }
            output.write_offset += size;
        }
        return size;
    }

    virtual char const *BeginBlock(char const *name) {
        return name;
    }

    virtual void EndBlock() {}
};

DECOMP_ASSERT(sizeof(NetOutputStream) == 0x24, "NetOutputStream size");
DECOMP_ASSERT(offsetof(NetOutputStream, message) == 0x20, "NetOutputStream message offset");

#endif
