#pragma once

#include "decomp.h"
#include "nu2api/nu3d/nuhspecial.h"

struct GameObject_s;

struct EQUIVALENTOBJECTGROUP_s {
    i16 object_count;
    i16 byte_size;
    nuhspecial_s objects[];
};
DECOMP_ASSERT(offsetof(EQUIVALENTOBJECTGROUP_s, objects) == 4, "Equivalent-object entries offset");
DECOMP_ASSERT(sizeof(EQUIVALENTOBJECTGROUP_s) == 4, "Equivalent-object group header size");

void Buck_Start(GameObject_s *object, GameObject_s *rider);
void Buck_StartRiderJump(GameObject_s *rider, GameObject_s *mount);
