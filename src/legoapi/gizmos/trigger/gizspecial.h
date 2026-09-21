#pragma once

#include "legoapi/gizmo/base/gizmo.h"

extern i32 gizspecial_gizmotype_id;

#ifdef __cplusplus

typedef struct GIZSPECIAL_s GIZSPECIAL;

char *GizSpecial_GetName(GIZSPECIAL *special);
GIZSPECIAL *GizSpecial_FindByName(char *name, WORLDINFO_s *world);
GIZMO *createGizSpecial(void *, char *name);
ADDGIZMOTYPE *GizSpecial_RegisterGizmo(i32 type_id);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
