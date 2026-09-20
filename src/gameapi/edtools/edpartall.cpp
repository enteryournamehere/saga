#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"

#include <stdio.h>
#include <string.h>

void edpartDoInput(nupad_s *pad);
void edpartDetermineNearest(f32 distance);
void edpartDrawCursor();
void edpartHighlightNearest();
void edpartScaleType(i32 type, f32 scale);

extern "C" {
    i32 edpart_set_part = 5;
    i32 edpart_filter;
    char edpart_filter_string[16] = "PART";
    i32 edpart_first_time_this_level;
    i32 edpart_curr;
    i32 edpart_create_type = -1;
    i8 edpart_effect_list;
    i32 edpart_emitrotx;
    i32 edpart_emitroty;
    i32 edpart_emitrotz;
    f32 edpart_scale_factor = 1.0f;
    NUMTL *edpart_mtl;
    NUMTL *edpart_boxmtl;
    extern i32 edpart_nearest;
    extern part_emit_s *edpart_nearest_emit;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[512];
    extern i32 part_types_used;
    extern eduimenu_s *edpart_active_menu;
    extern eduimenu_s *edpart_opt_menu;
    extern eduimenu_s *edpart_type_menu;
    void eduiMenuRender(eduimenu_s *menu);
    eduimenu_s *edpart_active_menu;
    eduimenu_s *edpart_opt_menu;
    eduimenu_s *edpart_sscale_menu;
    eduimenu_s *edpart_message_menu;
    eduimenu_s *edpart_name_menu;
    eduimenu_s *edpart_data_menu;
    eduimenu_s *edpart_scaletype_menu;
    eduimenu_s *edpart_switchtype_menu;
    eduimenu_s *edpart_switch_menu;
    eduimenu_s *edpart_soundcontrol_menu;
    eduimenu_s *edpart_soundid_menu;
    eduimenu_s *edpart_soundx_menu;
    eduimenu_s *edpart_sounds_menu;
    eduimenu_s *edpart_debrisscale_menu;
    eduimenu_s *edpart_partindex_menu;
    eduimenu_s *edpart_impactpart_menu;
    eduimenu_s *edpart_debrisindex_menu;
    eduimenu_s *edpart_diedebris_menu;
    eduimenu_s *edpart_impactdebris_menu;
    eduimenu_s *edpart_emitterdebris_menu;
    eduimenu_s *edpart_trail2debris_menu;
    eduimenu_s *edpart_trail1debris_menu;
    eduimenu_s *edpart_debrissettings_menu;
    eduimenu_s *edpart_cutoff_menu;
    eduimenu_s *edpart_emittime_menu;
    eduimenu_s *edpart_changegenrate_menu;
    eduimenu_s *edpart_varemit_menu;
    eduimenu_s *edpart_varstart_menu;
    eduimenu_s *edpart_grav_menu;
    eduimenu_s *edpart_emitvel_menu;
    eduimenu_s *edpart_emit_menu;
    eduimenu_s *edpart_instancescale_menu;
    eduimenu_s *edpart_tint_menu;
    eduimenu_s *edpart_instanceflags_menu;
    eduimenu_s *edpart_instorient_menu;
    eduimenu_s *edpart_maxlife_menu;
    eduimenu_s *edpart_instancesettings_menu;
    eduimenu_s *edpart_instanceorphans_menu;
    eduimenu_s *edpart_thingsinstance_menu;
    eduimenu_s *edpart_worldinstance_menu;
    eduimenu_s *edpart_instance_menu;
    eduimenu_s *edpart_leveltype_menu;
    eduimenu_s *edpart_generaltype_menu;
    eduimenu_s *edpart_type_menu;
}

static eduiitem_s *edpart_nullobject_highlight;
static i32 edpart_count;
static i32 edpart_superscale = 1;
static NUVEC edpart_entry_position;
extern "C" {
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edpart_sscale_menu;
    extern eduimenu_s *edpart_cutoff_menu;
    extern eduimenu_s *edpart_emittime_menu;
    extern eduimenu_s *edpart_scaletype_menu;
    extern eduimenu_s *edpart_debrisscale_menu;
    extern eduimenu_s *edpart_instancescale_menu;
    extern eduimenu_s *edpart_changegenrate_menu;
    extern eduimenu_s *edpart_name_menu;
    extern i32 edpart_nearest;
    extern i32 edpart_create_type;
    extern f32 edpart_scale_factor;
    extern part_typedesc_s part_types[128];
}

static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *);

extern "C" {
    i32 edpart_which_scene = 1;
    i32 edpart_set_debris;
}

static inline void edpartRefreshEmitterSounds() {
    i32 effect = edpart_nearest_emit->effect_id;
    part_typedesc_s *type = &part_types[effect];
    for (i32 emitter = 0; emitter < 40; ++emitter) {
        if (part_emits[emitter].effect_id == effect) {
            part_emits[emitter].sounds_active = 0;
            // Both original setters test the emitter index here, not the sound index.
            for (i32 sound = 0; emitter < 4; ++sound) {
                if (type->sounds[sound] != -1 && type->sound_modes[sound] != 0) {
                    part_emits[emitter].sounds_active = 1;
                    break;
                }
            }
        }
    }
}

static void edpartInit() {
    edpart_first_time_this_level = 1;
    edpart_mtl = NuMtlCreate3D(1);
    edpart_mtl->opacity = 1.0f;
    edpart_mtl->diffuse_color.r = 0.5f;
    edpart_mtl->diffuse_color.g = 0.5f;
    edpart_mtl->diffuse_color.b = 0.5f;
    edpart_mtl->attribs.alpha_mode = 0;
    edpart_mtl->attribs.cull_mode = 2;
    edpart_mtl->attribs.z_mode = 0;
    NuMtlUpdate(edpart_mtl);
    edpart_boxmtl = NuMtlCreate(1);
    edpart_boxmtl->opacity = 1.0f;
    edpart_boxmtl->diffuse_color.r = 0.5f;
    edpart_boxmtl->diffuse_color.g = 0.5f;
    edpart_boxmtl->diffuse_color.b = 0.5f;
    edpart_boxmtl->attribs.alpha_mode = 0;
    edpart_boxmtl->attribs.cull_mode = 2;
    edpart_boxmtl->attribs.z_mode = 3;
    NuMtlUpdate(edpart_boxmtl);
    edpart_nearest = -1;
    edpart_curr = -1;
    edpart_create_type = -1;
    edpart_effect_list = 0;
    edpart_emitrotz = 0;
    edpart_emitroty = 0;
    edpart_emitrotx = 0;
}
static i32 edpartProc(float delta_time, nupad_s *pad) {
    edpart_count += 5;
    if (edpart_active_menu) {
        eduiMenuProcess(edpart_active_menu, delta_time, pad);
        return 0;
    }
    edpartDoInput(pad);
    edpartDetermineNearest(1.0f);
    return (pad->digital_buttons_pressed >> 11) & 1;
}
static void edpartApply() {
}
static void edpartClose() {
    eduiMenuDestroy(edpart_opt_menu);
}
static void edpartEnter() {
    NUVEC origin = {0.0f, 0.0f, 0.0f};
    if (edmainQueryLocVec())
        edpart_entry_position = *edmainQueryLocVec();
    else {
        edpart_entry_position.x = global_camera.mtx.m30;
        edpart_entry_position.y = global_camera.mtx.m31;
        edpart_entry_position.z = global_camera.mtx.m32;
    }
    if (edpart_first_time_this_level) {
        if (edmainQueryLocVec())
            edcamSetPosAng(edmainQueryLocVec(), 0, 0);
        else
            edcamSetPosAng(&origin, 0, 0);
        edpart_first_time_this_level = 0;
    }
    edpart_nearest = -1;
}
static void edpartRender() {
    edcamSet();
    edpartDrawCursor();
    edpartHighlightNearest();
    if (edpart_active_menu)
        eduiMenuRender(edpart_active_menu);
}
static void edpartSelType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_type_menu = NULL;
    edpart_active_menu = NULL;
    edpart_create_type = item->data;
    edpart_effect_list = part_types[edpart_create_type].field_b3;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edpartCopyType(eduimenu_s *menu, eduiitem_s *, u32) {
    if (part_types_used < 128 && edpart_create_type != -1) {
        for (i32 index = 0; index < 128; ++index) {
            if (part_types[index].name[0] == '\0') {
                part_type_s *source = &part_types[edpart_create_type];
                part_type_s *copy = &part_types[index];
                *copy = *source;
                if (strlen(source->name) > 12) {
                    char name[16];
                    strcpy(name, source->name);
                    name[12] = '\0';
                    sprintf(copy->name, "%s%03d", name, index);
                } else
                    sprintf(copy->name, "%s%03d", source->name, index);
                ++part_types_used;
                edpart_create_type = index;
                break;
            }
        }
    }
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edpartDataMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartGravMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_grav_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelGravMenu, "Gravity");
        if (edpart_grav_menu != NULL) {
            eduiMenuAddItem(edpart_grav_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeGrav, edpart_superscale * -10.0f,
                                                 edpart_superscale * 20.0f, edpart_nearest_type->gravity * 0.5f,
                                                 "Gravity"));
            eduiMenuAddItem(edpart_grav_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeBounce, 0.0f, edpart_superscale,
                                                 edpart_nearest_type->bounce, "Bounce"));
            eduiMenuAttach(menu, edpart_grav_menu);
            edpart_grav_menu->x = menu->x + 10;
            edpart_grav_menu->y = menu->y + 40;
        }
    }
}
static void edpartMoveList(eduimenu_s *menu, eduiitem_s *, u32) {
    part_type_s *type = &part_types[edpart_create_type];
    type->page = !type->field_b3;
    type->field_b3 = type->page;
    edpart_effect_list = type->page;
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edpartTintMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_tint_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelTintMenu, "Instance Tint");
        if (edpart_tint_menu != NULL) {
            eduiMenuAddItem(edpart_tint_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeTintR, 0.0f, 2.0f,
                                                 edpart_nearest_type->field_160, "Red Tint"));
            eduiMenuAddItem(edpart_tint_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeTintG, 0.0f, 2.0f,
                                                 edpart_nearest_type->field_164, "Green Tint"));
            eduiMenuAddItem(edpart_tint_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeTintB, 0.0f, 2.0f,
                                                 edpart_nearest_type->field_168, "Blue Tint"));
            eduiMenuAttach(menu, edpart_tint_menu);
            edpart_tint_menu->x = menu->x + 10;
            edpart_tint_menu->y = menu->y + 40;
        }
    }
}
static void edpartTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeGrav(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->gravity = static_cast<edui_slider_s *>(item)->value * 2.0f;
}
static void edpartChangeName(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_create_type != -1) {
        char *name = static_cast<edui_textpicker_s *>(item)->value;
        NuStrNCpy(part_types[edpart_create_type].name, name, 16);
        for (i32 index = 0; index < 40; ++index) {
            if (part_emits[index].effect_id == edpart_create_type)
                NuStrNCpy(part_emits[index].name, name, 16);
        }
    }
}
static void edpartCutOffMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_cutoff_menu = eduiMenuCreate(70, 70, 250, 200, ed_fnt, edpartCancelCutOffMenu, "Radii");
        if (edpart_cutoff_menu != NULL) {
            eduiMenuAddItem(edpart_cutoff_menu,
                           eduiItemSliderCreate(0, edblack, 0, edpartChangeCutOff, 0.0f,
                                                edpart_superscale * 25.0f, edpart_nearest_type->maximum_distance,
                                                "CutOff Rad"));
            eduiMenuAttach(menu, edpart_cutoff_menu);
            edpart_cutoff_menu->x = menu->x + 10;
            edpart_cutoff_menu->y = menu->y + 40;
        }
    }
}
static void edpartDeleteType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_sscale_menu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, edpartCancelSScaleMenu, "Super Scale");
    if (edpart_sscale_menu != NULL) {
        eduiMenuAddItem(edpart_sscale_menu,
                       eduiItemSliderCreateInt(0, edblack, 0, edpartChangeSScale, 1, 99, edpart_superscale,
                                              "Super Scale"));
        eduiMenuAttach(menu, edpart_sscale_menu);
        edpart_sscale_menu->x = menu->x + 10;
        edpart_sscale_menu->y = menu->y + 40;
    }
}
static void edpartSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_soundid_menu = NULL;
    u32 slot = static_cast<u32>(item->data) >> 16;
    i32 value = item->data & 0xffff;
    if (value == 9999)
        value = -1;
    if (edpart_nearest_type != NULL)
        edpart_nearest_type->sounds[slot] = value;
    edpartRefreshEmitterSounds();
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edpartSoundXMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSoundsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSwitchMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_168 = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_164 = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_160 = static_cast<edui_slider_s *>(item)->value;
}
static void edpartEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_emitvel_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelEmitVelMenu, "Emitter Vel");
        if (edpart_emitvel_menu != NULL) {
            eduiMenuAddItem(edpart_emitvel_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeEmitVel, -(edpart_superscale * 10.0f),
                                                 edpart_superscale * 20.0f, edpart_nearest_type->speed, "Emitter Vel"));
            eduiMenuAttach(menu, edpart_emitvel_menu);
            edpart_emitvel_menu->x = menu->x + 10;
            edpart_emitvel_menu->y = menu->y + 40;
        }
    }
}
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_emit)
        edpart_nearest_emit->field_46 = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edpartSoundIDMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartVarEmitMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_varemit_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelVarEmitMenu, "Emitter Variation");
        if (edpart_varemit_menu != NULL) {
            eduiMenuAddItem(edpart_varemit_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeVarEmit, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->velocity_random.x, "Rand Emit X"));
            eduiMenuAddItem(edpart_varemit_menu,
                            eduiItemSliderCreate(1, edblack, 0, edpartChangeVarEmit, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->velocity_random.y, "Rand Emit Y"));
            eduiMenuAddItem(edpart_varemit_menu,
                            eduiItemSliderCreate(2, edblack, 0, edpartChangeVarEmit, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->velocity_random.z, "Rand Emit Z"));
            eduiMenuAttach(menu, edpart_varemit_menu);
            edpart_varemit_menu->x = menu->x + 10;
            edpart_varemit_menu->y = menu->y + 40;
        }
    }
}
static void edpartAddLevelType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->bounce = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->maximum_distance = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_period = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_superscale = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edpartEmitTimeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_emittime_menu =
            eduiMenuCreate(70, 70, 260, 300, ed_fnt, edpartCancelEmitTimeMenu, "Emitter Timing");
        if (edpart_emittime_menu != NULL) {
            eduiMenuAddItem(edpart_emittime_menu,
                           eduiItemToggleCreate(0x400000, edblack, (edpart_nearest_type->flags >> 22) & 1, 1,
                                                edpartChangeInstanceFlag, "Trigger effect inside cutoff"));
            eduiMenuAddItem(edpart_emittime_menu,
                           eduiItemSliderCreate(0, edblack, 0, edpartChangeIvalOn, 0.01f,
                                                edpart_superscale * 5.0f, edpart_nearest_type->emission_period,
                                                "On Time"));
            eduiMenuAddItem(edpart_emittime_menu,
                           eduiItemSliderCreate(0, edblack, 0, edpartChangeIvalOnRan, 0.0f,
                                                edpart_superscale * 5.0f,
                                                edpart_nearest_type->emission_period_random, "Random On Time"));
            eduiMenuAddItem(edpart_emittime_menu,
                           eduiItemSliderCreate(0, edblack, 0, edpartChangeIvalOff, 0.0f,
                                                edpart_superscale * 5.0f, edpart_nearest_type->emission_pause,
                                                "Off Time"));
            eduiMenuAddItem(edpart_emittime_menu,
                           eduiItemSliderCreate(0, edblack, 0, edpartChangeIvalOffRan, 0.0f,
                                                edpart_superscale * 5.0f,
                                                edpart_nearest_type->emission_pause_random, "Random Off Time"));
            eduiMenuAttach(menu, edpart_emittime_menu);
            edpart_emittime_menu->x = menu->x + 10;
            edpart_emittime_menu->y = menu->y + 40;
        }
    }
}
static void edpartInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartToggleFilter(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_filter = item->highlighted;
}
static void edpartVarStartMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_varstart_menu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, edpartCancelVarStartMenu, "Random Start");
        if (edpart_varstart_menu != NULL) {
            eduiMenuAddItem(edpart_varstart_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeVarStart, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->position_random.x, "Rand Start X"));
            eduiMenuAddItem(edpart_varstart_menu,
                            eduiItemSliderCreate(1, edblack, 0, edpartChangeVarStart, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->position_random.y, "Rand Start Y"));
            eduiMenuAddItem(edpart_varstart_menu,
                            eduiItemSliderCreate(2, edblack, 0, edpartChangeVarStart, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->position_random.z, "Rand Start Z"));
            eduiMenuAttach(menu, edpart_varstart_menu);
            edpart_varstart_menu->x = menu->x + 10;
            edpart_varstart_menu->y = menu->y + 40;
        }
    }
}
static void edpartCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    edpart_active_menu = NULL;
    edpart_opt_menu = NULL;
}
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->speed = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeGenRate(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_rate = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_pause = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->lifetime = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        switch (item->data) {
            case 0:
                edpart_nearest_type->velocity_random.x = static_cast<edui_slider_s *>(item)->value;
                break;
            case 1:
                edpart_nearest_type->velocity_random.y = static_cast<edui_slider_s *>(item)->value;
                break;
            case 2:
                edpart_nearest_type->velocity_random.z = static_cast<edui_slider_s *>(item)->value;
                break;
        }
    }
}
static void edpartDieDebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartLevelTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartScaleTypeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest != -1) {
        edpart_scaletype_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelScaleTypeMenu, "Scale Type");
        if (edpart_scaletype_menu != NULL) {
            eduiMenuAddItem(edpart_scaletype_menu,
                           eduiItemSliderCreate(0, edblack, 0, edpartSetScaleFactor, 0.1f, 9.9f,
                                                edpart_scale_factor, "Scale Factor"));
            eduiMenuAddItem(edpart_scaletype_menu,
                           eduiItemSelCreate(1, edblack, 0, 0, edpartApplyScaleType, "Apply Scale to Type"));
            eduiMenuAttach(menu, edpart_scaletype_menu);
            edpart_scaletype_menu->x = menu->x + 10;
            edpart_scaletype_menu->y = menu->y + 40;
        }
    }
}
static void edpartSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_switchtype_menu = NULL;
    if (edpart_nearest_emit != NULL) {
        edpart_nearest_emit->switch_type = item->data;
        if (edpart_nearest_emit->switch_type == 0)
            edpart_nearest_emit->switch_state = 1;
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edpartAddGeneralType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartApplyScaleType(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type)
        edpartScaleType(edpart_nearest_emit->effect_id, edpart_scale_factor);
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_data_menu);
    edpart_data_menu = NULL;
}
static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emit_menu);
    edpart_emit_menu = NULL;
}
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_grav_menu);
    edpart_grav_menu = NULL;
}
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_tint_menu);
    edpart_tint_menu = NULL;
}
static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_type_menu);
    edpart_type_menu = NULL;
}
static void edpartChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_create_type != -1) {
        part_typedesc_s *type = &part_types[edpart_create_type];
        edpart_name_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelChangeNameMenu, "Type Name");
        if (edpart_name_menu != NULL) {
            eduiMenuAddItem(edpart_name_menu, eduiItemTextPickCreate(0, edblack, edpartChangeName, "Name: "));
            strcpy(static_cast<edui_textpicker_s *>(edui_last_item)->value, type->name);
            static_cast<edui_textpicker_s *>(edui_last_item)->max_length = 15;
            eduiMenuAttach(menu, edpart_name_menu);
            edpart_name_menu->x = menu->x + 10;
            edpart_name_menu->y = menu->y + 40;
        }
    }
}
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        switch (item->data) {
            case 0:
                edpart_nearest_type->position_random.x = static_cast<edui_slider_s *>(item)->value;
                break;
            case 1:
                edpart_nearest_type->position_random.y = static_cast<edui_slider_s *>(item)->value;
                break;
            case 2:
                edpart_nearest_type->position_random.z = static_cast<edui_slider_s *>(item)->value;
                break;
        }
    }
}
static void edpartImpactPartMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_scale_factor = static_cast<edui_slider_s *>(item)->value;
}
static void edpartSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_period_random = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangePartIndex(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL && edpart_set_part == 5) {
        edpart_nearest_type->impact_part = item->data;
    }
}
static void edpartDebrisScaleMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_debrisscale_menu =
        eduiMenuCreate(70, 70, 180, 300, ed_fnt, edpartCancelDebrisScaleMenu, "Debris Scale");
    if (edpart_debrisscale_menu != NULL && edpart_nearest_type != NULL) {
        eduiMenuAddItem(edpart_debrisscale_menu,
                       eduiItemSliderCreate(0, edblack, 0, edpartChangeDebrisScale, 0.0f,
                                            edpart_superscale * 10.0f, edpart_nearest_type->effect_scale,
                                            "Debris Scale"));
        eduiMenuAttach(menu, edpart_debrisscale_menu);
        edpart_debrisscale_menu->x = menu->x + 10;
        edpart_debrisscale_menu->y = menu->y + 40;
    }
}
static void edpartFileLoadEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartFileSaveEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartGeneralTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL)
        edpart_nearest_type->variant_mode = item->data;
}
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type == NULL)
        return;
    if (item->data == 9999) {
        for (i32 index = 0; index < 8; ++index) {
            edpart_nearest_type->effect_ids[index] = -1;
            edpart_nearest_type->effect_pages[index] = 1;
        }
        if (item->highlighted)
            edpart_nearest_type->effect_ids[0] = 9999;
        edpart_nearest_type->variant_count = 0;
        edpart_nearest_type->flags |= 0x10;
        return;
    }
    edpart_nullobject_highlight->highlighted = 0;
    if (item->highlighted) {
        if (edpart_nearest_type->variant_count < 8) {
            i32 index = edpart_nearest_type->variant_count;
            edpart_nearest_type->effect_ids[index] = item->data;
            edpart_nearest_type->effect_pages[index] = edpart_which_scene;
            ++edpart_nearest_type->variant_count;
        } else {
            item->highlighted = 0;
        }
    } else {
        for (i32 index = 0; index < 8; ++index) {
            if (edpart_nearest_type->effect_ids[index] == item->data) {
                for (i32 next = index; next < 7; ++next) {
                    edpart_nearest_type->effect_ids[next] = edpart_nearest_type->effect_ids[next + 1];
                    edpart_nearest_type->effect_pages[next] = edpart_nearest_type->effect_pages[next + 1];
                }
                edpart_nearest_type->effect_ids[7] = -1;
                edpart_nearest_type->effect_pages[7] = -1;
                --edpart_nearest_type->variant_count;
                break;
            }
        }
    }
}
static void edpartSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_soundcontrol_menu = NULL;
    u32 slot = static_cast<u32>(item->data) >> 16;
    i32 value = item->data & 0xffff;
    if (value == 9999)
        value = -1;
    if (edpart_nearest_type != NULL)
        edpart_nearest_type->sound_modes[slot] = value;
    edpartRefreshEmitterSounds();
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_cutoff_menu);
    edpart_cutoff_menu = NULL;
}
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_sscale_menu);
    edpart_sscale_menu = NULL;
}

static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundx_menu);
    edpart_soundx_menu = NULL;
}

static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_sounds_menu);
    edpart_sounds_menu = NULL;
}

static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_switch_menu);
    edpart_switch_menu = NULL;
}

static void edpartChangeFilterName(eduimenu_s *, eduiitem_s *item, u32) {
    NuStrNCpy(edpart_filter_string, static_cast<edui_textpicker_s *>(item)->value, 16);
}

static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_pause_random = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->lifetime_random = static_cast<edui_slider_s *>(item)->value;
}

static void edpartImpactDebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartSoundControlMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartTrail1DebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartTrail2DebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emitvel_menu);
    edpart_emitvel_menu = NULL;
}

static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_message_menu);
    edpart_message_menu = NULL;
}

static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundid_menu);
    edpart_soundid_menu = NULL;
}

static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_varemit_menu);
    edpart_varemit_menu = NULL;
}

static void edpartChangeDebrisIndex(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        switch (edpart_set_debris) {
            case 0:
                edpart_nearest_type->trail_effects[0] = item->data;
                break;
            case 1:
                edpart_nearest_type->trail_effects[1] = item->data;
                break;
            case 2:
                edpart_nearest_type->attached_effect = item->data;
                break;
            case 3:
                edpart_nearest_type->impact_effect = item->data;
                break;
            case 4:
                edpart_nearest_type->kill_effect = item->data;
                break;
        }
    }
}

static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->effect_scale = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeGenRateMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_changegenrate_menu =
            eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelChangeGenRateMenu, "Emits per Second");
        if (edpart_changegenrate_menu != NULL) {
            eduiMenuAddItem(edpart_changegenrate_menu,
                           eduiItemSliderCreateInt(0, edblack, 0, edpartChangeGenRate, 0,
                                                   static_cast<i32>(edpart_superscale * 1200.0f),
                                                   static_cast<i32>(edpart_nearest_type->emission_rate),
                                                   "Emits per Second"));
            eduiMenuAttach(menu, edpart_changegenrate_menu);
            edpart_changegenrate_menu->x = menu->x + 10;
            edpart_changegenrate_menu->y = menu->y + 40;
        }
    }
}

static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        switch (item->data) {
            case 0:
                edpart_nearest_type->rotation[0] =
                    static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
                break;
            case 1:
                edpart_nearest_type->rotation[1] =
                    static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
                break;
            case 2:
                edpart_nearest_type->rotation[2] =
                    static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
                break;
        }
    }
}

static void edpartChangeMaxLifeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_maxlife_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelChangeMaxLifeMenu, "Max Instance Life");
        if (edpart_maxlife_menu != NULL) {
            eduiMenuAddItem(edpart_maxlife_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeMaxLife, 0.0f, edpart_superscale * 10.0f,
                                                 edpart_nearest_type->lifetime, "Base Max Life"));
            eduiMenuAddItem(edpart_maxlife_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeRanMaxLife, 0.0f, edpart_superscale * 10.0f,
                                                 edpart_nearest_type->lifetime_random, "Random Max Life"));
            eduiMenuAttach(menu, edpart_maxlife_menu);
            edpart_maxlife_menu->x = menu->x + 10;
            edpart_maxlife_menu->y = menu->y + 40;
        }
    }
}

static void edpartEmitterDebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceFlagsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceScaleMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_instancescale_menu =
        eduiMenuCreate(70, 70, 180, 300, ed_fnt, edpartCancelInstanceScaleMenu, "Instance Scale");
    if (edpart_instancescale_menu != NULL && edpart_nearest_type != NULL) {
        eduiMenuAddItem(edpart_instancescale_menu,
                       eduiItemSliderCreate(0, edblack, 0, edpartChangeInstanceScale, 0.0f,
                                            edpart_superscale * 10.0f, edpart_nearest_type->particle_scale,
                                            "Instance Scale"));
        eduiMenuAttach(menu, edpart_instancescale_menu);
        edpart_instancescale_menu->x = menu->x + 10;
        edpart_instancescale_menu->y = menu->y + 40;
    }
}

static void edpartWorldInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emittime_menu);
    edpart_emittime_menu = NULL;
}

static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instance_menu);
    edpart_instance_menu = NULL;
    edpart_nullobject_highlight = 0;
}

static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_varstart_menu);
    edpart_varstart_menu = NULL;
}

static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        switch (edpart_set_debris) {
            case 0:
                edpart_nearest_type->trail_rates[0] = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
                break;
            case 1:
                edpart_nearest_type->trail_rates[1] = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
                break;
        }
    }
}

static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (item->highlighted)
            edpart_nearest_type->flags |= item->data;
        else
            edpart_nearest_type->flags &= ~item->data;
    }
}

static void edpartDebrisSettingsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceOrientMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartLevelPartIndexMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartThingsInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_diedebris_menu);
    edpart_diedebris_menu = NULL;
}

static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_leveltype_menu);
    edpart_leveltype_menu = NULL;
}

static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_partindex_menu);
    edpart_partindex_menu = NULL;
}

static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_scaletype_menu);
    edpart_scaletype_menu = NULL;
}

static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->particle_scale = static_cast<edui_slider_s *>(item)->value;
}

static void edpartInstanceOrphansMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_name_menu);
    edpart_name_menu = NULL;
}

static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_impactpart_menu);
    edpart_impactpart_menu = NULL;
}

static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_switchtype_menu);
    edpart_switchtype_menu = NULL;
}

static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        switch (item->data) {
            case 0:
                edpart_nearest_type->rotation_random[0] =
                    static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
                break;
            case 1:
                edpart_nearest_type->rotation_random[1] =
                    static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
                break;
            case 2:
                edpart_nearest_type->rotation_random[2] =
                    static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
                break;
        }
    }
}

static void edpartDeleteInstanceOrphan(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartFileSaveEffectsLevel(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartGeneralPartIndexMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceSettingsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartLevelDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrisindex_menu);
    edpart_debrisindex_menu = NULL;
}

static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrisscale_menu);
    edpart_debrisscale_menu = NULL;
}

static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_generaltype_menu);
    edpart_generaltype_menu = NULL;
}

static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_impactdebris_menu);
    edpart_impactdebris_menu = NULL;
}

static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundcontrol_menu);
    edpart_soundcontrol_menu = NULL;
}

static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_trail1debris_menu);
    edpart_trail1debris_menu = NULL;
}

static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_trail2debris_menu);
    edpart_trail2debris_menu = NULL;
}

static void edpartFileSaveEffectsGeneral(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartGeneralDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_changegenrate_menu);
    edpart_changegenrate_menu = NULL;
}

static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_maxlife_menu);
    edpart_maxlife_menu = NULL;
}

static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emitterdebris_menu);
    edpart_emitterdebris_menu = NULL;
}

static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instanceflags_menu);
    edpart_instanceflags_menu = NULL;
}

static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instancescale_menu);
    edpart_instancescale_menu = NULL;
}

static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_worldinstance_menu);
    edpart_worldinstance_menu = NULL;
}

static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrissettings_menu);
    edpart_debrissettings_menu = NULL;
}

static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instorient_menu);
    edpart_instorient_menu = NULL;
}

static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_thingsinstance_menu);
    edpart_thingsinstance_menu = NULL;
}

static void edpartDeleteAllInstanceOrphans(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelInstanceOrphansMenu(eduimenu_s *, eduimenu_s *) {
    if (edpart_nearest_type->variant_count == 0) {
        for (i32 i = 0; i < 8; i++) {
            edpart_nearest_type->effect_ids[i] = -1;
            edpart_nearest_type->effect_pages[i] = 1;
        }
        edpart_nearest_type->effect_ids[0] = 9999;
        edpart_nearest_type->flags |= 0x10;
    }
    eduiMenuDestroy(edpart_instanceorphans_menu);
    edpart_instanceorphans_menu = NULL;
}

static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instancesettings_menu);
    edpart_instancesettings_menu = NULL;
}

static void edpartDeleteAllInstanceDuplicates(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

extern "C" {
    ed_module_s edpartdesc = {NULL, NULL, "PART Editor", edpartInit, edpartClose, edpartEnter, NULL,
                             edpartApply, NULL, NULL, 0x74726170, edpartProc, edpartRender, NULL};
}
