#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nufile/nufile.h"
#include <stdio.h>
#include <string.h>
#include "nu2api/numath/nurand.h"

EdRegistry theRegistry;
i32 pad_disabled;
eduimenu_s *edLevelPinnedMenu;

void eduiSetPinnedMenu(eduimenu_s *menu) {
    edLevelPinnedMenu = menu;
}

void edSetPadDisabled(i32 disabled) {
    pad_disabled = disabled;
}

i32 edGetPadDisabled() {
    return pad_disabled;
}

static NUGSPLINE *splineStore;
static i32 numSplinesLoaded;
char *EDSPLINE_FILECHECK = const_cast<char *>("EDSPLINE v. ");

extern "C" {
    char edgra_filter_string[16] = "GRASS";
    i32 edgra_mode = 1;
    extern NUGSCN *edbits_base_scene;
    i32 edgra_last_clump_in_buffer = -1;
    i32 edgra_copy_source = -1;
    f32 edgra_global_fadein = 15.0f, edgra_global_fadeout = 25.0f;
    void edgraInitAllClumps(void);
    void DebFreeInstantly(i32 *);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, i8, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetPriority(i32, u16, i8);
    void DebrisSetRoomID(i32, i32);
    void DebrisSetDetailLevels(i32, i32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i32 part_page_used[8];
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
    extern f32 edpp_offset;
    i32 edbits_particle_level_page;
}

i32 edpartLookupObjectInScene(char *, NUGSCN *);

void EdTerrInit(void *, void *) {
    STUBBED();
}

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char) {
    STUBBED();
}

void EdDrawBegin(i32) {
    STUBBED();
}

void edpartPlace(i32, nuvec_s *) {
    STUBBED();
}

void edppDoInput(nupad_s *) {
    STUBBED();
}

void EdTerrShadow(nuvec_s *, float, float, i32) {
    STUBBED();
}

void edbriDoInput(nupad_s *) {
    STUBBED();
}

void edgraDoInput(nupad_s *) {
    STUBBED();
}

void edpartCreate(nuvec_s *, i32) {
    STUBBED();
}

i32 edppPtlPlace(i32 index, NUVEC *position) {
    edpp_particle_s *particle = &edpp_ptls[index];
    DebrisEmitterPos(particle->instance_id, position->x, position->y, position->z);
    DebrisOrientation(particle->instance_id, edpp_rotz, edpp_roty);
    DebrisEmitterOrientation(particle->instance_id, edpp_emitrotz, edpp_emitroty, edpp_emitrotx);
    DebrisReflectionOrientation(particle->instance_id, edpp_refrotz, edpp_refroty,
                               particle->reflection_offset, particle->reflection_bounce);
    if (particle->facing_mode != 0) {
        particle->facing_rotation_x = edpp_facrotx;
        particle->facing_rotation_y = edpp_facroty;
        DebrisSetFacing(particle->instance_id, particle->facing_mode,
                        particle->facing_rotation_x, particle->facing_rotation_y);
    }
    edpp_ptls[index].position = *position;
    edpp_ptls[index].emitter_rotation_z = edpp_emitrotz;
    edpp_ptls[index].emitter_rotation_y = edpp_emitroty;
    edpp_ptls[index].emitter_rotation_x = edpp_emitrotx;
    edpp_ptls[index].rotation_z = edpp_rotz;
    edpp_ptls[index].rotation_y = edpp_roty;
    edpp_ptls[index].reflection_rotation_z = edpp_refrotz;
    edpp_ptls[index].reflection_rotation_y = edpp_refroty;
    return index;
}

void EdDrawPolyTri(VuVec const &, VuVec const &, VuVec const &, i32) {
    STUBBED();
}

void edanimDoInput(nupad_s *) {
    STUBBED();
}

void edbobsDrawBox(nuvec_s *, nuvec_s *, i32) {
    STUBBED();
}

void edbriFileSave(char *) {
    STUBBED();
}

void edgraFileSave(char *) {
    STUBBED();
}

void edpartDoInput(nupad_s *) {
    STUBBED();
}

i32 edppPtlCreate(NUVEC *position, i32 effect_index) {
    if (edpp_instances_used == 512)
        return -1;
    i32 index = 0;
    while (edpp_ptls[index].instance_id != -1)
        ++index;
    AddDebrisEffect(&edpp_ptls[index].instance_id, effect_index, position->x, position->y, position->z);
    edpp_particle_s *particle = &edpp_ptls[index];
    if (particle->instance_id == -1)
        return -1;
    debkeydata[particle->instance_id].field_2f9 = 0;
    particle->position = *position;
    particle->effect_index = effect_index;
    particle->rotation_z = edpp_rotz;
    particle->rotation_y = edpp_roty;
    particle->emitter_rotation_z = edpp_emitrotz;
    particle->emitter_rotation_y = edpp_emitroty;
    particle->emitter_rotation_x = edpp_emitrotx;
    particle->start_offset = edpp_offset;
    particle->switch_type = 0;
    particle->switch_id = -1;
    particle->switch_variable = 0.0f;
    particle->reflection_offset = 0.0f;
    particle->reflection_bounce = 0.9f;
    particle->render_group = 0;
    particle->page = edbits_particle_level_page;
    particle->detail_levels = 7;
    switch (debtab[effect_index]->particle_type) {
    case 0:
        particle->render_priority = 20000;
        break;
    case 2:
        particle->render_priority = static_cast<i16>(40000);
        break;
    case 3:
        particle->render_priority = 30000;
        break;
    case 7:
        particle->render_priority = 10000;
        break;
    }
    particle = &edpp_ptls[index];
    particle->dynamic_priority = 0;
    particle->facing_mode = 0;
    particle->facing_rotation_x = 0;
    particle->facing_rotation_y = 0;
    strcpy(particle->name, debtab[particle->effect_index]->name);
    DebrisOrientation(particle->instance_id, particle->rotation_z, particle->rotation_y);
    DebrisEmitterOrientation(particle->instance_id, particle->emitter_rotation_z,
                             particle->emitter_rotation_y, particle->emitter_rotation_x);
    DebrisStartOffset(particle->instance_id, particle->start_offset);
    DebrisReflectionOrientation(particle->instance_id, particle->reflection_rotation_z,
                                particle->reflection_rotation_y, particle->reflection_offset,
                                particle->reflection_bounce);
    DebrisSetFacing(particle->instance_id, particle->facing_mode,
                    particle->facing_rotation_x, particle->facing_rotation_y);
    DebrisSetGroupID(particle->instance_id, particle->render_group);
    DebrisSetPriority(particle->instance_id, particle->render_priority, particle->dynamic_priority);
    DebrisSetRoomID(particle->instance_id, 0);
    DebrisSetDetailLevels(particle->instance_id, particle->detail_levels);
    ++edpp_instances_used;
    edpp_page_used[edbits_particle_level_page] = 1;
    edpp_page_on[edbits_particle_level_page] = 1;
    if (edpp_page_scene[edbits_particle_level_page] == 0)
        edpp_page_scene[edbits_particle_level_page] = reinterpret_cast<usize>(edbits_base_scene);
    return index;
}

void edppPtlShelve(i32 index) {
    edpp_particle_s *particle = &edpp_ptls[index];
    if (particle->instance_id != -1 && particle->instance_id != 99999) {
        DebFreeInstantly(&particle->instance_id);
        particle->instance_id = 99999;
    }
}

void EdDrawLineCube(VuMtx const &, float, i32) {
    STUBBED();
}

void EdDrawPolyAxis(VuMtx const &, float, i32) {
    STUBBED();
}

void edanimFileSave(char *) {
    STUBBED();
}

void edpartInitType(i32) {
    STUBBED();
}

void edppDrawCursor() {
    STUBBED();
}

extern "C" {
    void DebFreeInstantly(i32 *);
}

void edppPtlDestroy(i32 index) {
    if (edpp_ptls[index].instance_id != -1) {
        if (edpp_ptls[index].instance_id != 99999)
            DebFreeInstantly(&edpp_ptls[index].instance_id);
        --edpp_instances_used;
        edpp_ptls[index].instance_id = -1;
    }
}

void EdDrawLineArrow(VuMtx const &, float, i32) {
    STUBBED();
}

void EdDrawLineCross(VuVec const &, float, i32) {
    STUBBED();
}

void EdDrawPolyArrow(VuVec const &, VuVec const &, i32, i32, float, float, float, float) {
    STUBBED();
}

void edbriDrawCursor() {
    STUBBED();
}

void edgraClumpPlace(i32 index, NUVEC *position) {
    edgra_clump_s *clump = &GrassClumps[index];
    clump->position = *position;
    clump->size = edgra_size;
    clump->rotation_z = edgra_rotz;
    clump->rotation_y = edgra_roty;
    if (edgra_mode != 3) {
        if (edgra_units_used + edgra_clump_size - clump->element_count <= 0x3000)
            clump->element_count = edgra_clump_size;
    }
    edgra_free_vecbuffer = static_cast<NUVEC *>(clump->vector_buffer) + clump->element_count;
    edgraInitAllClumps();
}

void edgraDrawCursor() {
    STUBBED();
}

void edpartPtlShelve(i32) {
    STUBBED();
}

void edpartScaleType(i32, float) {
    STUBBED();
}

void edppSaveEffects(char *, char) {
    STUBBED();
}

void EdDrawLineSphere(VuVec const &, float, float, i32) {
    STUBBED();
}

void EdDrawPolySector(VuVec const &, float, i32, i32, i32, i32, i32) {
    STUBBED();
}

void edanimDrawCursor() {
    STUBBED();
}

i32 edgraClumpCreate(NUVEC *position) {
    if (edgra_clumps_used == EDGRA_MAX_CLUMPS)
        return -1;
    if (edgra_copy_source == -1 && edgra_mode == 3) {
        if (edgra_units_used == 0x3000 || edgra_ind_clumps_used == EDGRA_MAX_INDIVIDUAL_CLUMPS)
            return -1;
    } else if (edgra_units_used + edgra_clump_size > 0x3000)
        return -1;
    i32 index = 0;
    while (GrassClumps[index].element_count != 0)
        ++index;
    edgra_clump_s *clump = &GrassClumps[index];
    if (edgra_copy_source != -1) {
        clump->field_18 = GrassClumps[edgra_copy_source].field_18;
        clump->special_index = GrassClumps[edgra_copy_source].special_index;
        clump->field_20 = GrassClumps[edgra_copy_source].field_20;
        clump->element_count = edgra_clump_size;
        clump->flags = GrassClumps[edgra_copy_source].flags;
        clump->page = edgra_pageid;
        clump->seed = NuRand(NULL);
        GrassClumps[index].unknown_25 = GrassClumps[edgra_copy_source].unknown_25;
        GrassClumps[index].field_2c = GrassClumps[edgra_copy_source].field_2c;
        GrassClumps[index].unknown_26 = GrassClumps[edgra_copy_source].unknown_26;
        GrassClumps[index].field_30 = GrassClumps[edgra_copy_source].field_30;
        GrassClumps[index].kind = GrassClumps[edgra_copy_source].kind;
        GrassClumps[index].near_distance = GrassClumps[edgra_copy_source].near_distance;
        GrassClumps[index].field_42 = GrassClumps[edgra_copy_source].field_42;
        GrassClumps[index].far_distance = GrassClumps[edgra_copy_source].far_distance;
        GrassClumps[index].field_44 = GrassClumps[edgra_copy_source].field_44;
        GrassClumps[index].field_43 = GrassClumps[edgra_copy_source].field_43;
    } else {
        clump->special_index = edgra_instance_type;
        clump->element_count = edgra_mode == 3 ? 1 : edgra_clump_size;
        clump->field_18 = 0.2f;
        clump->field_20 = 1.0f;
        clump->flags = 1;
        clump->page = edgra_pageid;
        clump->seed = NuRand(NULL);
        GrassClumps[index].unknown_25 = 1;
        GrassClumps[index].unknown_26 = 1;
        GrassClumps[index].field_2c = 0.0f;
        GrassClumps[index].field_30 = 1.0f;
        GrassClumps[index].kind = edgra_mode;
        GrassClumps[index].near_distance = edgra_global_fadein;
        GrassClumps[index].far_distance = edgra_global_fadeout;
        GrassClumps[index].field_42 = 1;
        GrassClumps[index].field_44 = 0.0f;
        GrassClumps[index].field_43 = 1;
    }
    if (GrassClumps[index].kind == 3) {
        i32 individual = 0;
        while (IndGrassClumpsUsed[individual])
            ++individual;
        GrassClumps[index].individual_index = individual;
        IndGrassClumpsUsed[individual] = 1;
        GetIndGrassClump(individual, 0)->position.x = 0.0f;
        GetIndGrassClump(individual, 0)->position.y = 0.0f;
        GetIndGrassClump(individual, 0)->position.z = 0.0f;
        GetIndGrassClump(individual, 0)->field_0c = 1.0f;
        GetIndGrassClump(individual, 0)->field_10 = edgra_rotz;
        GetIndGrassClump(individual, 0)->field_12 = edgra_roty;
        ++edgra_ind_clumps_used;
    } else
        GrassClumps[index].individual_index = -1;
    ++edgra_clumps_used;
    if (!edgra_page_used[edgra_pageid]) {
        edgra_page_used[edgra_pageid] = 1;
        edgra_page_scene[edgra_pageid] = edbits_base_scene;
        edgra_page_terrain[edgra_pageid] = edbits_base_terrain;
        edgra_page_matrix_stack[edgra_pageid] = edgra_mtxbuffer;
    }
    GrassClumps[index].vector_buffer = edgra_free_vecbuffer;
    edgra_last_clump_in_buffer = index;
    edgraClumpPlace(index, position);
    return index;
}

void edpartDrawCursor() {
    STUBBED();
}

void EdDrawLineCircleX(VuVec const &, float, i32, i32) {
    STUBBED();
}

void EdDrawLineCircleY(VuVec const &, float, i32, i32) {
    STUBBED();
}

void EdDrawLineCircleZ(VuVec const &, float, i32, i32) {
    STUBBED();
}

void EdDrawLineSegment(VuVec const &, VuVec const &, i32) {
    STUBBED();
}

void edanimParamCreate(i32) {
    STUBBED();
}

void edpartSaveEffects(char *, char) {
    STUBBED();
}

void edppPtlChangeType(i32 index, i32 effect_index) {
    edpp_particle_s *particle = &edpp_ptls[index];
    if (particle->effect_index != effect_index) {
        edppPtlDestroy(index);
        AddDebrisEffect(&edpp_ptls[index].instance_id, effect_index,
                        particle->position.x, particle->position.y, particle->position.z);
        if (particle->instance_id != -1)
            debkeydata[particle->instance_id].field_2f9 = 0;
        edpp_ptls[index].effect_index = effect_index;
    }
}

void edppPtlCreateCopy(nuvec_s *, i32) {
    STUBBED();
}

void EdDrawPolyCylinder(VuMtx const &, float, float, float, i32, i32, i32, i32) {
    STUBBED();
}

void EdDrawPolyCylinder(VuVec const &, VuVec const &, i32, i32, i32, float, float, float) {
    STUBBED();
}

void edanimParamDestroy(i32 index) {
    if (AnimParams[index].instance_id != -1)
        AnimParams[index].instance_id = -1;
    --edanim_params_used;
}

void edbitsDoSingleDump(i32 face) {
    char filename[32];
    i32 index;
    for (index = 0; index < 1000; ++index) {
        sprintf(filename, "pictures\\cub%03d_%d.bmp", index, face);
        if (NuFileSize(filename) <= 0)
            break;
        if (index == 999)
            break;
    }
    sprintf(filename, "pictures\\cub%03d_", index);
    NuPs2VideoScreenDump(filename, 1, 1.0f, 1.0f, face, 0, 0);
}

void edgraCalculatePage(char, i32) {
    STUBBED();
}

void edgraInstancePlace(i32 index, NUVEC *position) {
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->position.x =
        position->x - GrassClumps[edgra_nearest].position.x;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->position.y =
        position->y - GrassClumps[edgra_nearest].position.y;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->position.z =
        position->z - GrassClumps[edgra_nearest].position.z;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->field_0c = 1.0f;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->field_10 = edgra_rotz;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->field_12 = edgra_roty;
    edgraInitAllClumps();
}

i32 edpartLookupObject(char *name) {
    if (edbits_base_scene != NULL)
        return edpartLookupObjectInScene(name, edbits_base_scene);
    return -1;
}

i32 edSpline_FindAllBeg(NUGSCN *scene, char *prefix, NUGSPLINE **results, i32 capacity) {
    i32 count = 0;
    if (capacity <= 0)
        return 0;
    if (splineStore != NULL) {
        for (i32 i = 0; i < numSplinesLoaded; ++i) {
            if (NuStrNICmp(prefix, splineStore[i].name, -1) == 0) {
                results[count++] = &splineStore[i];
                if (count >= capacity)
                    return count;
            }
        }
    }
    if (scene != NULL) {
        for (i32 i = 0; i < scene->numsplines; ++i) {
            if (NuStrNICmp(prefix, scene->splines[i].name, -1) == 0) {
                results[count++] = &scene->splines[i];
                if (count >= capacity)
                    return count;
            }
        }
    }
    return count;
}

i32 edSpline_FindAllSub(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity) {
    i32 count = 0;
    if (capacity <= 0)
        return 0;
    if (splineStore != NULL) {
        for (i32 i = 0; i < numSplinesLoaded; ++i) {
            if (NuStrIStr(splineStore[i].name, name) != NULL) {
                results[count++] = &splineStore[i];
                if (count >= capacity)
                    return count;
            }
        }
    }
    if (scene != NULL) {
        for (i32 i = 0; i < scene->numsplines; ++i) {
            if (NuStrIStr(scene->splines[i].name, name) != NULL) {
                results[count++] = &scene->splines[i];
                if (count >= capacity)
                    return count;
            }
        }
    }
    return count;
}

i32 LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end) {
    splineStore = reinterpret_cast<NUGSPLINE *>(buf->void_ptr);
    buf->addr = (buf->addr + 3) & ~static_cast<usize>(3);

    EdFileSetMedia(1);
    if (EdFileOpen(path, NUFILE_READ) == 0) {
        splineStore = NULL;
        return 0;
    }

    const i32 file_check_length = NuStrLen(EDSPLINE_FILECHECK);
    for (i32 i = 0; i < file_check_length; ++i) {
        if (EdFileReadChar() != EDSPLINE_FILECHECK[i]) {
            EdFileClose();
            splineStore = NULL;
            return 0;
        }
    }

    EdFileReadInt();
    i32 spline_count = EdFileReadInt();
    i32 point_count = EdFileReadInt();
    i32 string_bytes = EdFileReadInt();

    char *name_cursor = reinterpret_cast<char *>(splineStore + spline_count);
    char *name_end = name_cursor + string_bytes;
    NUVEC *point_cursor = reinterpret_cast<NUVEC *>((reinterpret_cast<usize>(name_end) + 3) & ~static_cast<usize>(3));
    NUVEC *data_end = point_cursor + point_count;
    if (reinterpret_cast<usize>(data_end) > buf_end->addr) {
        EdFileClose();
        splineStore = NULL;
        return 0;
    }

    for (i32 i = 0; i < spline_count; ++i) {
        EdFileReadChar();
        i32 name_length = EdFileReadInt();
        NUGSPLINE *spline = &splineStore[i];
        spline->name = name_cursor;
        spline->pt_size = sizeof(NUVEC);
        spline->length = static_cast<i16>(EdFileReadInt());
        spline->pts = point_cursor;

        for (i32 j = 0; j < name_length; ++j) {
            spline->name[j] = EdFileReadChar();
        }
        name_cursor += name_length;
        if (name_cursor > name_end) {
            EdFileClose();
            splineStore = NULL;
            EdFileClose();
            numSplinesLoaded = spline_count;
            buf->void_ptr = data_end;
            return 0;
        }

        for (i32 j = 0; j < spline->length; ++j) {
            point_cursor[j].x = EdFileReadFloat();
            point_cursor[j].y = EdFileReadFloat();
            point_cursor[j].z = EdFileReadFloat();
        }
        point_cursor += spline->length;
        if (point_cursor > data_end) {
            EdFileClose();
            splineStore = NULL;
            EdFileClose();
            numSplinesLoaded = spline_count;
            buf->void_ptr = data_end;
            return 0;
        }
    }

    EdFileClose();
    numSplinesLoaded = spline_count;
    buf->void_ptr = data_end;
    return spline_count;
}

NUGSPLINE *edSpline_SplineFind(NUGSCN *scene, char *name) {
    if (splineStore != NULL) {
        for (i32 i = 0; i < numSplinesLoaded; ++i) {
            if (NuStrICmp(splineStore[i].name, name) == 0) {
                return &splineStore[i];
            }
        }
    }
    return NuSplineFind(scene, name);
}

void edSpline_SplineList(nugscn_s *) {
    STUBBED();
}

void edanimParticlePlace(i32 index, NUVEC *position) {
    nuhspecial_s special;
    NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
    NuVecSub(reinterpret_cast<NUVEC *>(AnimParams[edanim_nearest_param_id].effect_positions[index]),
             position, NuSpecialGetPos(&special));
    AnimParams[edanim_nearest_param_id].effect_angles[index] = edanim_emitrotz;
    AnimParams[edanim_nearest_param_id].effect_angle_ranges[index] = edanim_emitroty;
}

void edanimStartAllPages() {
    edanimStartPage(0);
    edanimStartPage(1);
    edanimStartPage(2);
    edanimStartPage(3);
    edanimStartPage(4);
    edanimStartPage(5);
    edanimStartPage(6);
    edanimStartPage(7);
}

void edgraInstanceCreate(NUVEC *position) {
    if (edgra_nearest != -1 && GrassClumps[edgra_nearest].element_count != EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP) {
        i32 index = GrassClumps[edgra_nearest].element_count++;
        edgraInstancePlace(index, position);
    }
}

void edpartPtlChangeType(i32, i32) {
    STUBBED();
}

void edppDestroyAllPages() {
    if (edpp_page_used[0]) edppClearPage(0);
    if (edpp_page_used[1]) edppClearPage(1);
    if (edpp_page_used[2]) edppClearPage(2);
    if (edpp_page_used[3]) edppClearPage(3);
    if (edpp_page_used[4]) edppClearPage(4);
    if (edpp_page_used[5]) edppClearPage(5);
    if (edpp_page_used[6]) edppClearPage(6);
    if (edpp_page_used[7]) edppClearPage(7);
}

void edanimParticleCreate(NUVEC *position) {
    i32 index = AnimParams[edanim_nearest_param_id].effect_count;
    if (index != 8 && edanim_particle_type != -1) {
        edanimParticlePlace(index, position);
        AnimParams[edanim_nearest_param_id].effect_ids[index] = edanim_particle_type;
        strcpy(AnimParams[edanim_nearest_param_id].effect_names[index], debtab[edanim_particle_type]->name);
        AnimParams[edanim_nearest_param_id].effect_intervals[index] = 60;
        AnimParams[edanim_nearest_param_id].effect_flags[index] = 0;
        ++AnimParams[edanim_nearest_param_id].effect_count;
    }
}

void edgraInstanceDestroy(i32 index) {
    if (edgra_nearest != -1 && GrassClumps[edgra_nearest].element_count != 1) {
        if (index != GrassClumps[edgra_nearest].element_count - 1) {
            for (i32 i = index; i < GrassClumps[edgra_nearest].element_count - 1; ++i) {
                GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i)->position =
                    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i + 1)->position;
                GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i)->field_0c =
                    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i + 1)->field_0c;
                GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i)->field_10 =
                    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i + 1)->field_10;
                GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i)->field_12 =
                    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i + 1)->field_12;
            }
        }
        --GrassClumps[edgra_nearest].element_count;
        edgraInitAllClumps();
    }
}

void edppDetermineNearest(float max_distance_squared) {
    NUVEC delta;
    if (edpp_nearest != -1) {
        edpp_particle_s *particle = &edpp_ptls[edpp_nearest];
        if (particle->instance_id != 99999 && particle->instance_id != -1) {
            NuVecSub(&delta, &edpp_cam_pos, &particle->position);
            if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f) {
                return;
            }
        }
    }
    edpp_nearest = -1;
    for (i32 i = 0; i < 512; ++i) {
        if (edpp_ptls[i].instance_id == -1 || edpp_ptls[i].instance_id == 99999) {
            continue;
        }
        NuVecSub(&delta, &edpp_cam_pos, &edpp_ptls[i].position);
        float distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (max_distance_squared < 0.0f || max_distance_squared > distance_squared) {
            max_distance_squared = distance_squared;
            edpp_nearest = i;
        }
    }
}

void edppHighlightNearest() {
    STUBBED();
}

void edppMultipleCopyCopy() {
    STUBBED();
}

void edbriDetermineNearest(float) {
    STUBBED();
}

void edgraSortVectorBuffer(i32 index) {
    NUVEC temporary[256];
    if (index != -1 && index != edgra_last_clump_in_buffer && GrassClumps[index].vector_buffer) {
        u8 *buffer = static_cast<u8 *>(GrassClumps[index].vector_buffer);
        usize bytes = GrassClumps[index].element_count * sizeof(NUVEC);
        memcpy(temporary, buffer, bytes);
        memmove(buffer, buffer + bytes, static_cast<u8 *>(edgra_free_vecbuffer) - (buffer + bytes));
        memcpy(static_cast<u8 *>(edgra_free_vecbuffer) - bytes, temporary, bytes);
        edgra_last_clump_in_buffer = index;
        for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
            if (i != index && GrassClumps[i].element_count &&
                GrassClumps[i].vector_buffer > GrassClumps[index].vector_buffer) {
                GrassClumps[i].vector_buffer = static_cast<u8 *>(GrassClumps[i].vector_buffer) - bytes;
            }
        }
        GrassClumps[index].vector_buffer = static_cast<u8 *>(edgra_free_vecbuffer) - bytes;
        edgraInitAllClumps();
    }
}

void edpartDestroyAllPages() {
    if (part_page_used[0]) edpartClearPage(0);
    if (part_page_used[1]) edpartClearPage(1);
    if (part_page_used[2]) edpartClearPage(2);
    if (part_page_used[3]) edpartClearPage(3);
    if (part_page_used[4]) edpartClearPage(4);
    if (part_page_used[5]) edpartClearPage(5);
    if (part_page_used[6]) edpartClearPage(6);
    if (part_page_used[7]) edpartClearPage(7);
}

void edppMultipleCopyClear() {
    STUBBED();
}

void edppMultipleCopyPaste() {
    STUBBED();
}

void edppStartSingleEffect(i32) {
    STUBBED();
}

void edpartHighlightNearest() {
    STUBBED();
}

void edpartMultipleCopyCopy() {
    STUBBED();
}

void edpartMultipleCopyClear() {
    STUBBED();
}

float edanimPlayerAnimDistance(i32 parameter_index) {
    if (edmainQueryLocVec() != NULL) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edanim_page_scene[AnimParams[parameter_index].page],
                         AnimParams[parameter_index].instance_id);
        NUVEC *position = edmainQueryLocVec();
        return NuVecDist(NuSpecialGetPos(&special), position, NULL);
    }
    return 0.0f;
}

void edanimRenderSoundEmitters(i32) {
    STUBBED();
}

void edbobs_DrawCoordinateInfo(nuvec_s *, i32, i32) {
    STUBBED();
}

void edanimDetermineNearestAnim(float) {
    STUBBED();
}

void edgraDetermineNearestClump(f32 distance) {
    NUVEC delta;
    if (edgra_nearest != -1) {
        NuVecSub(&delta, &edgra_cam_pos, &GrassClumps[edgra_nearest].position);
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f)
            return;
    }
    edgra_nearest = -1;
    for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
        if (GrassClumps[i].element_count) {
            NuVecSub(&delta, &edgra_cam_pos, &GrassClumps[i].position);
            f32 candidate = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (distance < 0.0f || candidate < distance) {
                distance = candidate;
                edgra_nearest = i;
            }
        }
    }
    if (edgra_nearest != -1)
        edgraSortVectorBuffer(edgra_nearest);
}

void eduiItemFileSelectorCreate(u32, eduiiattr_s *, void (*)(eduimenu_s *, eduiitem_s *, u32), char *) {
    STUBBED();
}

void edanimDetermineNearestSound(float) {
    STUBBED();
}

void edanimRenderParticleEmitters(i32) {
    STUBBED();
}

void edgraDetermineNearestInstance(f32 distance) {
    NUVEC delta;
    if (edgra_nearest == -1) {
        edgra_nearest_instance = -1;
        return;
    }
    i32 individual = GrassClumps[edgra_nearest].individual_index;
    if (edgra_nearest_instance != -1) {
        NuVecAdd(&delta, &GrassClumps[edgra_nearest].position,
                 &GetIndGrassClump(individual, edgra_nearest_instance)->position);
        NuVecSub(&delta, &edgra_cam_pos, &delta);
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f)
            return;
    }
    edgra_nearest_instance = -1;
    for (i32 i = 0; i < GrassClumps[edgra_nearest].element_count; ++i) {
        NuVecAdd(&delta, &GrassClumps[edgra_nearest].position, &GetIndGrassClump(individual, i)->position);
        NuVecSub(&delta, &edgra_cam_pos, &delta);
        f32 candidate = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distance < 0.0f || candidate < distance) {
            distance = candidate;
            edgra_nearest_instance = i;
        }
    }
}

void edanimDetermineNearestParticle(float) {
    STUBBED();
}

void EdDrawEnd() {
    STUBBED();
}

void EdDrawMtx(VuMtx const *) {
    STUBBED();
}

void EdTerrRay(VuVec &, VuVec &) {
    STUBBED();
}

EdManScale::EdManScale() {
    STUBBED();
}

void EdManScale::Process(EdInputContext &, ClassObjectList &) {
    STUBBED();
}

void EdManScale::Render(ClassObjectList &) {
    STUBBED();
}

void EdRegistry::AddMapping(char *, char *) {
    STUBBED();
}

void EdRegistry::AddObjectNotifier(EdObjectNotifier *) {
    STUBBED();
}

void EdRegistry::ClassIFaceProcess(EdClass *, void *, EdInputContext &) {
    STUBBED();
}

void EdRegistry::ClassIFaceProcess(i32, void *, EdInputContext &) {
    STUBBED();
}

void EdRegistry::ClassIFaceRender(EdClass *, void *, i32) {
    STUBBED();
}

void EdRegistry::ClassIFaceRender(i32, void *, i32) {
    STUBBED();
}

void EdRegistry::CreateObject(EdClassInterface *, void *, i32, i32, i32) {
    STUBBED();
}

void EdRegistry::DefunctObject(EdClassInterface *, void *, i32, i32) {
    STUBBED();
}

void EdRegistry::DestroyObject(EdClassInterface *, void *, i32, i32) {
    STUBBED();
}

void EdRegistry::Flush() {
    type_count = 0;
    class_count = 0;
    object_count = 0;
}

void EdRegistry::GetClass(char *) {
    STUBBED();
}

EdClass *EdRegistry::GetClass(i32 index) {
    if (index < 0 || index >= class_count) {
        return nullptr;
    }
    return &classes[index];
}

void EdRegistry::GetClassId(char *) {
    STUBBED();
}

i32 EdRegistry::GetClassId(EdClass *object_class) {
    return object_class - classes;
}

void EdRegistry::GetStreamClassMapping(EdStream &, i32 *, i32 &, i32) {
    STUBBED();
}

void EdRegistry::GetType(char *) {
    STUBBED();
}

EdType *EdRegistry::GetType(i32 index) {
    if (index < 0 || index >= type_count) {
        return nullptr;
    }
    return &types[index];
}

void EdRegistry::GetTypeId(char *) {
    STUBBED();
}

void EdRegistry::Initialise(variptr_u &, variptr_u &, i32, i32, i32, i32) {
    STUBBED();
}

void EdRegistry::MapName(char *) {
    STUBBED();
}

void EdRegistry::NotifyCreateObject(void *, EdClass *, void *, i32, i32, i32) {
    STUBBED();
}

void EdRegistry::NotifyDefunctObject(void *, EdClass *, i32) {
    STUBBED();
}

void EdRegistry::NotifyDestroyObject(void *, EdClass *, i32, i32) {
    STUBBED();
}

void EdRegistry::NotifyReviveObject(void *, EdClass *, i32) {
    STUBBED();
}

void EdRegistry::RegisterBaseTypes() {
    STUBBED();
}

void EdRegistry::RegisterClass(char *, EdClassInterface *, i32) {
    STUBBED();
}

void EdRegistry::RegisterType(char *, i32, void (*)(EdStream &, void *, i32)) {
    STUBBED();
}

void EdRegistry::Serialise(EdStream &) {
    STUBBED();
}

void EdRegistry::SerialiseObjects(EdStream &, EdRegistry *) {
    STUBBED();
}

EdManRotate::EdManRotate() {
    STUBBED();
}

void EdManRotate::Process(EdInputContext &, ClassObjectList &) {
    STUBBED();
}

void EdManRotate::Render(ClassObjectList &) {
    STUBBED();
}

void EdManRotate::RotateItem(EdInputContext &, ClassObjectList &, i32, i32) {
    STUBBED();
}

void EdRefSpline::GetMemberData(void *, i32, void *, i32) {
    STUBBED();
}

void EdRefSpline::SetMemberData(void *, i32, void *, i32, i16 *) {
    STUBBED();
}

void EdBitControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdBitControl::Refresh() {
    STUBBED();
}

void EdBitControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdBitControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdBitControl::cbSelectItem(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdDefunctList::ReviveAll(i32) {
    STUBBED();
}

void EdEnumControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdEnumControl::GetEnumString(i32) {
    STUBBED();
}

void EdEnumControl::GetEnumValue(char *) {
    STUBBED();
}

void EdEnumControl::Refresh() {
    STUBBED();
}

void EdEnumControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdEnumControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdEnumControl::cbSelectItem(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdInputStream::SerialiseString(char **) {
    STUBBED();
}

void EdInputStream::SerialiseString(char **, i32) {
    STUBBED();
}

void EdInputStream::SerialiseString(char *, i32) {
    STUBBED();
}

void EdManipulator::DrawAxis(VuVec &, VuMtx *) {
    STUBBED();
}

void EdManipulator::DrawRotator(VuVec &) {
    STUBBED();
}

void EdManipulator::GetAxisLocators(VuVec &, VuVec *, VuMtx *) {
    STUBBED();
}

void EdManipulator::Process(EdInputContext &, ClassObjectList &) {
    STUBBED();
}

void EdManipulator::Render(ClassObjectList &) {
    STUBBED();
}

void EdManipulator::SelectAxis(EdInputContext &, VuVec &, VuVec &, VuVec &, VuMtx *) {
    STUBBED();
}

void EdManipulator::SelectRotator(EdInputContext &, VuVec &, VuVec &) {
    STUBBED();
}

void EdInputContext::Clear(i32 input) {
    if (static_cast<u32>(input) < 40) {
        values[input] = 0.0f;
        cleared[input] = 1;
    }
}

EdInputContext::EdInputContext() {
    STUBBED();
}

f32 EdInputContext::Get(i32 input) {
    if (static_cast<u32>(input) < 40) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetHold(i32 input) {
    if (static_cast<u32>(input) < 40 && held[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetPress(i32 input) {
    if (static_cast<u32>(input) < 40 && pressed[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetRelease(i32 input) {
    if (static_cast<u32>(input) < 40 && released[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetRepeat(i32 input) {
    if (static_cast<u32>(input) < 40 && repeated[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

void EdInputContext::Set(i32 input, float value, float repeat_delay) {
    if (value != 0.0f) {
        float now = current_time;
        float repeat_threshold = repeat_window + now;
        values[input] = value;
        float next_repeat = repeat_times[input];
        pressed[input] = held[input] == 0;
        held[input] = 1;
        if (next_repeat >= repeat_threshold || next_repeat == 0.0f) {
            repeated[input] = 1;
        }
        repeat_times[input] = now + repeat_delay;
        return;
    }

    values[input] = value;
    released[input] = held[input] != 0;
    repeat_times[input] = 0.0f;
    held[input] = 0;
}

void EdInputContext::Update(nucamera_s *, nupad_s *, float, bool) {
    STUBBED();
}

void EdOutputStream::SerialiseString(char **) {
    STUBBED();
}

void EdOutputStream::SerialiseString(char **, i32) {
    STUBBED();
}

void EdOutputStream::SerialiseString(char *, i32) {
    STUBBED();
}

void EdRefPlaceable::GetMemberData(void *, i32, void *, i32) {
    STUBBED();
}

void EdRefPlaceable::SetMemberData(void *, i32, void *, i32, i16 *) {
    STUBBED();
}

void EdColourControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

EdColourControl::EdColourControl() {
    STUBBED();
}

void EdColourControl::Refresh() {
    STUBBED();
}

void EdColourControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdColourControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdColourControl::cbColourSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdMatrixControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdMatrixControl::Destroy() {
    STUBBED();
}

EdMatrixControl::EdMatrixControl() {
    STUBBED();
}

void EdMatrixControl::Refresh() {
    STUBBED();
}

void EdMatrixControl::SetMenuItemAttr(i32, eduiitem_s *, eduiiattr_s *, eduiiattr_s *) {
    STUBBED();
}

void EdMatrixControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdMatrixControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdMatrixControl::cbSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdStringControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

EdStringControl::EdStringControl() {
    STUBBED();
}

void EdStringControl::GetVal(char *, i32) {
    STUBBED();
}

void EdStringControl::Refresh() {
    STUBBED();
}

void EdStringControl::SetVal(char const *) {
    STUBBED();
}

void EdStringControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdStringControl::cbPress(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdVectorControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdVectorControl::Destroy() {
    STUBBED();
}

EdVectorControl::EdVectorControl() {
    STUBBED();
}

void EdVectorControl::Refresh() {
    STUBBED();
}

void EdVectorControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdVectorControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdVectorControl::cbSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClassInterface::DistanceToObject(VuVec &, VuVec &, void *, EdRef **) {
    STUBBED();
}

void EdClassInterface::DistanceToObject(VuVec &, void *, EdRef **) {
    STUBBED();
}

void EdClassInterface::GetNextObject(void *, i32 (*)(void *)) {
    STUBBED();
}

void EdSfxNameControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

EdSfxNameControl::EdSfxNameControl() {
    STUBBED();
}

void EdSfxNameControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSfxNameControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSfxNameControl::cbSelectSfx(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdFileInputStream::BeginBlock(char const *) {
    STUBBED();
}

void EdFileInputStream::Eat(i32, i32) {
    STUBBED();
}

void EdFileInputStream::EndBlock() {
    STUBBED();
}

void EdFileInputStream::Open(i32, i32) {
    STUBBED();
}

void EdFileInputStream::SerialiseBuffer(void *, i32, i32) {
    STUBBED();
}

void EdFileOutputStream::BeginBlock(char const *) {
    STUBBED();
}

void EdFileOutputStream::Eat(i32, i32) {
    STUBBED();
}

void EdFileOutputStream::EndBlock() {
    STUBBED();
}

void EdFileOutputStream::Open(i32, i32) {
    STUBBED();
}

void EdFileOutputStream::SerialiseBuffer(void *, i32, i32) {
    STUBBED();
}

void EdRefSpecialObject::GetMemberData(void *, i32, void *, i32) {
    STUBBED();
}

void EdRefSpecialObject::SetMemberData(void *, i32, void *, i32, i16 *) {
    STUBBED();
}

EdSpecialObjectControl::EdSpecialObjectControl() {
    STUBBED();
}

void EdSpecialObjectControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSpecialObjectControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSpecialObjectControl::cbSelectObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSpecialObjectControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdSpecialObjectControl::Process(EdInputContext &) {
    STUBBED();
}

void EdSpecialObjectControl::Render() {
    STUBBED();
}

void EdClassObjectNameControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

EdClassObjectNameControl::EdClassObjectNameControl() {
    STUBBED();
}

void EdClassObjectNameControl::Process(EdInputContext &) {
    STUBBED();
}

void EdClassObjectNameControl::Render() {
    STUBBED();
}

void EdClassObjectNameControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClassObjectNameControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClassObjectNameControl::cbSelectClass(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClassObjectNameControl::cbSelectObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdRef::CheckType(i32) {
    STUBBED();
}

EdRef::EdRef(char *, char *, i32, i32, i32, EdControl *, i32) {
    STUBBED();
}

void EdRef::GetAttributeData(void *, i32, i32, void *, i32) {
    STUBBED();
}

void EdRef::GetMemberData(void *, i32, void *, i32) {
    STUBBED();
}

void EdRef::GetMemberObject(void *) {
    STUBBED();
}

void EdRef::GetTypeSize(i32, i32) {
    STUBBED();
}

void EdRef::Serialise(EdStream &, i32 *) {
    STUBBED();
}

void EdRef::SetAttributeData(void *, i32, i32, void *, i32) {
    STUBBED();
}

void EdRef::SetMemberData(void *, i32, void *, i32, i16 *) {
    STUBBED();
}

void EdType::Serialise(EdStream &) {
    STUBBED();
}

EdStream::EdStream() {
    STUBBED();
}

EdStream::EdStream(MemoryBuffer *) {
    STUBBED();
}

EdStream::EdStream(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void EdString::Set(char const *) {
    STUBBED();
}

EdString::~EdString() {
}

void EdSystem::Initalise(variptr_u &buffer, variptr_u &buffer_end, i32 flags) {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubInitialise(buffer, buffer_end, flags);
}

void EdSystem::Process(float delta_time) {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubProcess(delta_time);
}

void EdSystem::RegisterSubSystem(EdSubSystem *subsystem) {
    subsystem->next = NULL;
    subsystem->previous = last_subsystem;
    if (last_subsystem != NULL)
        last_subsystem->next = subsystem;
    last_subsystem = subsystem;
    if (first_subsystem == NULL)
        first_subsystem = subsystem;
    ++subsystem_count;
}

void EdSystem::Render() {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubRender();
}

void EdSystem::Reset() {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next) {
        subsystem->SubReset();
    }
}

__attribute__((weak)) void EdSubSystem::SubInitialise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

__attribute__((weak)) void EdSubSystem::SubReset() {
    STUBBED();
}

__attribute__((weak)) void EdSubSystem::SubProcess(float) {
    STUBBED();
}

__attribute__((weak)) void EdSubSystem::SubRender() {
    STUBBED();
}

void EdControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdControl::Process(EdInputContext &) {
    STUBBED();
}

void EdControl::Render() {
    STUBBED();
}

void EdControl::SelectSubObject() {
    STUBBED();
}

void EdControl::Refresh() {
    STUBBED();
}

void EdControl::SetMenuItemAttr(i32, eduiitem_s *, eduiiattr_s *, eduiiattr_s *) {
    STUBBED();
}

void EdControl::cbSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

EdManMove::EdManMove() {
    STUBBED();
}

void EdManMove::Process(EdInputContext &, ClassObjectList &) {
    STUBBED();
}

void EdManMove::Render(ClassObjectList &) {
    STUBBED();
}

void EdRefKnot::GetMemberData(void *, i32, void *, i32) {
    STUBBED();
}

void EdRefKnot::SetMemberData(void *, i32, void *, i32, i16 *) {
    STUBBED();
}
