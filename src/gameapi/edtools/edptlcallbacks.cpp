#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include <string.h>

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern i32 edpp_readout;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_damageflag_menu;
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);

static i32 edptl_superscale = 1;

extern "C" {
    extern eduimenu_s *edpp_active_menu;
    extern eduimenu_s *edptl_testdetail_menu;
    extern eduimenu_s *edptl_detail_menu;
    extern eduimenu_s *edptl_drawflag_menu;
    extern eduimenu_s *edptl_group_menu;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_switch_menu;
    extern eduimenu_s *dpadmodemenu;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *edptl_scaleeffect_menu;
    extern eduimenu_s *edptl_instancesettings_menu;
    extern eduimenu_s *edptl_torus_menu;
    extern eduimenu_s *collmenu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_soundcontrol_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern eduimenu_s *edptl_soundx_menu;
    extern eduimenu_s *edptl_sounds_menu;
    extern eduimenu_s *edptl_bounce_menu;
    extern eduimenu_s *ptljibmenu;
    extern eduimenu_s *ptlrotmenu;
    extern eduimenu_s *ptlsizemenu;
    extern eduimenu_s *edptl_ghost_menu;
    extern eduimenu_s *ptlcutoffmenu;
    extern eduimenu_s *emittimemenu;
    extern eduimenu_s *etimemenu;
    extern eduimenu_s *changegenratemenu;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlstartvelmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlemitmenu;
    extern eduimenu_s *textureselectmenu;
    extern eduimenu_s *texturemenu;
    extern eduimenu_s *ptlcolmenu;
    extern eduimenu_s *ptlgsortmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_quickdel_menu;
    extern eduimenu_s *edptl_orphanlist_menu;
    extern eduimenu_s *ptlclipmenu;
    extern eduimenu_s *messagemenu;
    extern eduimenu_s *ptldatamenu;
    extern eduimenu_s *ptltypemenu;
    extern eduimenu_s *effectlistmenu;
}

static eduiitem_s *torus_env1_item;
static eduiitem_s *torus_env2_item;
static eduiitem_s *torus_env3_item;
static eduiitem_s *coll_env_item;
static edui_slider_s *grad_jib_x_freq_item;
static edui_slider_s *grad_jib_x_amp_item;
static edui_slider_s *grad_jib_y_freq_item;
static edui_slider_s *grad_jib_y_amp_item;
static eduiitem_s *grad_rot_item;
static edui_slider_s *grad_rot_min_item;
static edui_slider_s *grad_rot_max_item;
static eduiitem_s *grad_size_w_item;
static eduiitem_s *grad_size_h_item;
static edui_slider_s *grad_size_min_item;
static edui_slider_s *grad_size_max_item;
static eduiitem_s *grad_item;
static eduiitem_s *grad_alpha_item;

// Particle-list editor UI/menu callback stubs (static, internal linkage).

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_ghost_menu);
    edptl_ghost_menu = NULL;
}

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_group_menu);
    edptl_group_menu = NULL;
}

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32) {
    edptl_soundcontrol_menu = NULL;
    u32 data = static_cast<u32>(item->data);
    if (edpp_nearest != -1 && edpp_ptls[edpp_nearest].instance_id != -1) {
        debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
        u32 sound_index = data >> 16;
        effect->sound_data[sound_index * 3 + 1] = data - (sound_index << 16);
    }
    for (i32 i = 0; i < 512; ++i) {
        i32 instance_id = edpp_ptls[i].instance_id;
        if (instance_id == 99999 || instance_id == -1) {
            continue;
        }
        debkeydatatype_s *key = &debkeydata[instance_id];
        debinftype *effect = debtab[key->effect_index];
        key->collision_timers[0] = 9999;
        if (effect->sound_data[1] == 3 || effect->sound_data[1] == 4) {
            key->collision_timers[0] = 1;
        }
        key->collision_timers[1] = 9999;
        if (effect->sound_data[4] == 3 || effect->sound_data[4] == 4) {
            key->collision_timers[1] = 1;
        }
        key->collision_timers[2] = 9999;
        if (effect->sound_data[7] == 3 || effect->sound_data[7] == 4) {
            key->collision_timers[2] = 1;
        }
        key->collision_timers[3] = 9999;
        if (effect->sound_data[10] == 3 || effect->sound_data[10] == 4) {
            key->collision_timers[3] = 1;
        }
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}

static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_bounce_menu);
    edptl_bounce_menu = NULL;
}

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_detail_menu);
    edptl_detail_menu = NULL;
}

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_soundx_menu);
    edptl_soundx_menu = NULL;
}

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_sounds_menu);
    edptl_sounds_menu = NULL;
}

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_switch_menu);
    edptl_switch_menu = NULL;
}

static void edptlcbSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edptl_soundcontrol_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt,
                                          edptlcbCancelSoundControlMenu, "Sound Control");
    if (edptl_soundcontrol_menu != NULL) {
        eduiMenuAddItem(edptl_soundcontrol_menu,
                       eduiItemCheckCreate(static_cast<u32>(item->data) << 16, colours,
                                           effect->sound_data[item->data * 3 + 1] == 0, 1,
                                           edptlcbSetSoundControl, "Off"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAddItem(edptl_soundcontrol_menu,
                       eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 1, colours,
                                           effect->sound_data[item->data * 3 + 1] == 1, 1,
                                           edptlcbSetSoundControl, "On Edge"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAddItem(edptl_soundcontrol_menu,
                       eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 2, colours,
                                           effect->sound_data[item->data * 3 + 1] == 2, 1,
                                           edptlcbSetSoundControl, "Off Edge"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAddItem(edptl_soundcontrol_menu,
                       eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 3, colours,
                                           effect->sound_data[item->data * 3 + 1] == 3, 1,
                                           edptlcbSetSoundControl, "Per Particle"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAddItem(edptl_soundcontrol_menu,
                       eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 4, colours,
                                           effect->sound_data[item->data * 3 + 1] == 4, 1,
                                           edptlcbSetSoundControl, "Continuous"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAttach(menu, edptl_soundcontrol_menu);
        edptl_soundcontrol_menu->x = menu->x + 10;
        edptl_soundcontrol_menu->y = menu->y + 40;
    }
}

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_soundid_menu);
    edptl_soundid_menu = NULL;
}

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(dpadmodemenu);
    dpadmodemenu = NULL;
}

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_drawflag_menu);
    edptl_drawflag_menu = NULL;
}

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlclipmenu);
    ptlclipmenu = NULL;
}

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_orphanlist_menu);
    edptl_orphanlist_menu = NULL;
}

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_switchtype_menu);
    edptl_switchtype_menu = NULL;
}

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_testdetail_menu);
    edptl_testdetail_menu = NULL;
}

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_scaleeffect_menu);
    edptl_scaleeffect_menu = NULL;
}

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_soundcontrol_menu);
    edptl_soundcontrol_menu = NULL;
}

// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeX(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeY(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeZ(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlColMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlJibMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlRotMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32) {
    if (edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    if (grad_jib_x_freq_item)
        effect->jib_x_frequency = grad_jib_x_freq_item->value;
    if (grad_jib_x_amp_item)
        effect->jib_x_amplitude = grad_jib_x_amp_item->value;
    if (grad_jib_y_freq_item)
        effect->jib_y_frequency = grad_jib_y_freq_item->value;
    if (grad_jib_y_amp_item)
        effect->jib_y_amplitude = grad_jib_y_amp_item->value;
    GenericDebinfoDmaTypeUpdate(effect);
}

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!grad_rot_min_item || !grad_rot_max_item || !grad_rot_item)
        return;
    i32 count = eduiGradPickRead(grad_rot_item, stages, 8);
    f32 minimum = grad_rot_min_item->value;
    f32 maximum = grad_rot_max_item->value;
    if (minimum == maximum)
        maximum += 0.1f;
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    for (i32 i = 0; i < count; ++i) {
        effect->rotation_keys[i].time = stages[i].time;
        effect->rotation_keys[i].value = static_cast<i32>((stages[i].red * (maximum - minimum) + minimum) *
                                                        (65536.0f / 360.0f));
    }
    effect->min_rotation = minimum;
    effect->max_rotation = maximum;
    GenericDebinfoDmaTypeUpdate(effect);
}

static void cbPtlCollMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    edui_gradient_pick_s *gradient;
    if (item->data == 1) {
        memcpy(effect->height_keys, effect->width_keys, sizeof(effect->height_keys));
        gradient = static_cast<edui_gradient_pick_s *>(grad_size_h_item);
    } else {
        memcpy(effect->width_keys, effect->height_keys, sizeof(effect->width_keys));
        gradient = static_cast<edui_gradient_pick_s *>(grad_size_w_item);
    }
    while (gradient->first_stage)
        eduiGradStageDelete(gradient, gradient->first_stage);
    for (i32 i = 0; i < 8; ++i) {
        if (item->data == 1) {
            f32 grey = (effect->height_keys[i].value - effect->min_size) / (effect->max_size - effect->min_size);
            eduiGradStageAddRGB(grad_size_h_item, effect->height_keys[i].time, grey, grey, grey);
            if (effect->height_keys[i].time == 1.0f)
                break;
        } else {
            f32 grey = (effect->width_keys[i].value - effect->min_size) / (effect->max_size - effect->min_size);
            eduiGradStageAddRGB(grad_size_w_item, effect->width_keys[i].time, grey, grey, grey);
            if (effect->width_keys[i].time == 1.0f)
                break;
        }
    }
    GenericDebinfoDmaTypeUpdate(effect);
}

static void cbPtlDataMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    ptlgravmenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbPtlCancelGravMenu, "Gravity");
    if (ptlgravmenu != NULL) {
        eduiMenuAddItem(ptlgravmenu, eduiItemSliderCreate(0, colours, 0, cbPtlChangeGrav,
                            -10.0f * edptl_superscale, 20.0f * edptl_superscale, effect->field_0a0, "Gravity"));
        eduiMenuAttach(menu, ptlgravmenu);
        ptlgravmenu->x = menu->x + 10;
        ptlgravmenu->y = menu->y + 40;
    }
}

static void cbPtlSelGCode(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSelGSort(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSizeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (grad_item) {
        i32 count = eduiGradPickRead(grad_item, stages, 8);
        if (count >= 2 && count <= 8 && edpp_nearest != -1) {
            i32 instance = edpp_ptls[edpp_nearest].instance_id;
            if (instance != -1) {
                debinftype *effect = debtab[debkeydata[instance].effect_index];
                for (i32 i = 0; i < count; ++i) {
                    effect->colour_keys[i].time = stages[i].time;
                    effect->colour_keys[i].red = static_cast<i32>(stages[i].red * 255.0f);
                    effect->colour_keys[i].green = static_cast<i32>(stages[i].green * 255.0f);
                    effect->colour_keys[i].blue = static_cast<i32>(stages[i].blue * 255.0f);
                }
                GenericDebinfoDmaTypeUpdate(effect);
            }
        }
    }
    if (grad_alpha_item) {
        i32 count = eduiGradPickRead(grad_alpha_item, stages, 8);
        if (count >= 2 && count <= 8 && edpp_nearest != -1) {
            i32 instance = edpp_ptls[edpp_nearest].instance_id;
            if (instance != -1) {
                debinftype *effect = debtab[debkeydata[instance].effect_index];
                for (i32 i = 0; i < count; ++i) {
                    effect->alpha_keys[i].time = stages[i].time;
                    effect->alpha_keys[i].value = stages[i].red * 255.0f;
                }
                GenericDebinfoDmaTypeUpdate(effect);
            }
        }
    }
}

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!grad_size_min_item || !grad_size_max_item || !grad_size_h_item || !grad_size_w_item)
        return;
    i32 count = eduiGradPickRead(grad_size_w_item, stages, 8);
    f32 minimum = grad_size_min_item->value;
    f32 maximum = grad_size_max_item->value;
    if (count >= 2 && count <= 8 && edpp_nearest != -1) {
        i32 instance = edpp_ptls[edpp_nearest].instance_id;
        if (instance != -1) {
            debinftype *effect = debtab[debkeydata[instance].effect_index];
            for (i32 i = 0; i < count; ++i) {
                effect->width_keys[i].time = stages[i].time;
                effect->width_keys[i].value = stages[i].red * (maximum - minimum) + minimum;
            }
            effect->min_size = minimum;
            effect->max_size = maximum;
            GenericDebinfoDmaTypeUpdate(effect);
        }
    }
    count = eduiGradPickRead(grad_size_h_item, stages, 8);
    if (count >= 2 && count <= 8 && edpp_nearest != -1) {
        i32 instance = edpp_ptls[edpp_nearest].instance_id;
        if (instance != -1) {
            debinftype *effect = debtab[debkeydata[instance].effect_index];
            for (i32 i = 0; i < count; ++i) {
                effect->height_keys[i].time = stages[i].time;
                effect->height_keys[i].value = stages[i].red * (maximum - minimum) + minimum;
            }
            effect->min_size = minimum;
            effect->max_size = maximum;
            GenericDebinfoDmaTypeUpdate(effect);
        }
    }
}

static void cbPtlGSortMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDamageMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    edptl_damage_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, cbPtlCancelDamageMenu, "Particle Damage");
    if (edptl_damage_menu != NULL) {
        eduiMenuAddItem(edptl_damage_menu, eduiItemSelCreate(1, edblack, 0, 0, cbPtlDamageFlagMenu, "Damage Flags..."));
        eduiMenuAddItem(edptl_damage_menu, eduiItemSelCreate(1, edblack, 0, 0, cbPtlCollMenu, "Collision Spheres..."));
        eduiMenuAddItem(edptl_damage_menu, eduiItemSelCreate(1, edblack, 0, 0, cbPtlTorusMenu, "Collision Torus..."));
    }
    eduiMenuAttach(menu, edptl_damage_menu);
    edptl_damage_menu->x = menu->x + 10;
    edptl_damage_menu->y = menu->y + 40;
}

static void cbPtlSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    sscalemenu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, cbPtlCancelSScaleMenu, "Super Scale");
    if (sscalemenu != NULL) {
        eduiMenuAddItem(sscalemenu, eduiItemSliderCreateInt(0, colours, 0, cbPtlChangeSScale,
                                                        1, 99, edptl_superscale, "Super Scale"));
        eduiMenuAttach(menu, sscalemenu);
        sscalemenu->x = menu->x + 10;
        sscalemenu->y = menu->y + 40;
    }
}

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbSelEffectList(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbEffectListMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    ptlemitvelmenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbPtlCancelEmitVelMenu, "Emitter Vel");
    if (ptlemitvelmenu != NULL) {
        eduiMenuAddItem(ptlemitvelmenu, eduiItemSliderCreate(0, colours, 0, cbPtlChangeEmitVel,
                            -(10.0f * edptl_superscale), 20.0f * edptl_superscale, effect->field_048, "Emitter Vel"));
        eduiMenuAttach(menu, ptlemitvelmenu);
        ptlemitvelmenu->x = menu->x + 10;
        ptlemitvelmenu->y = menu->y + 40;
    }
}

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    ptlreadoutmenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbPtlCancelReadoutMenu, "Info Box Style");
    if (ptlreadoutmenu != NULL) {
        eduiMenuAddItem(ptlreadoutmenu, eduiItemCheckCreate(0, colours, edpp_readout == 0, 1,
                                                         cbPtlSelReadout, "Normal"));
        eduiMenuAddItem(ptlreadoutmenu, eduiItemCheckCreate(1, colours, edpp_readout == 1, 1,
                                                         cbPtlSelReadout, "Co-ordinates"));
    }
    eduiMenuAttach(menu, ptlreadoutmenu);
    ptlreadoutmenu->x = menu->x + 10;
    ptlreadoutmenu->y = menu->y + 40;
}

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlTextureMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!coll_env_item)
        return;
    i32 count = eduiGradPickRead(coll_env_item, stages, 8);
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    f32 minimum = effect->min_size / 10000.0f;
    f32 maximum = effect->max_size / 10000.0f;
    for (i32 i = 0; i < count; ++i) {
        effect->collision_keys[i].time = stages[i].time;
        effect->collision_keys[i].value = stages[i].red * (maximum - minimum) + minimum;
    }
}

static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!torus_env1_item)
        return;
    i32 count = eduiGradPickRead(torus_env1_item, stages, 8);
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    for (i32 i = 0; i < count; ++i) {
        effect->torus_keys1[i].time = stages[i].time;
        effect->torus_keys1[i].value = stages[i].red;
    }
}

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!torus_env2_item)
        return;
    i32 count = eduiGradPickRead(torus_env2_item, stages, 8);
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    for (i32 i = 0; i < count; ++i) {
        effect->torus_keys2[i].time = stages[i].time;
        effect->torus_keys2[i].value = stages[i].red;
    }
}

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!torus_env3_item)
        return;
    i32 count = eduiGradPickRead(torus_env3_item, stages, 8);
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    for (i32 i = 0; i < count; ++i) {
        effect->torus_keys3[i].time = stages[i].time;
        effect->torus_keys3[i].value = stages[i].red;
    }
}

static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edptl_damageflag_menu = eduiMenuCreate(70, 70, 200, 250, ed_fnt, cbPtlCancelDamageFlagMenu, "Damage Flags");
    if (edptl_damageflag_menu != NULL) {
        eduiMenuAddItem(edptl_damageflag_menu, eduiItemToggleCreate(1, edblack, effect->field_2f2 & 1, 1,
                                                                 cbPtlChangeDamageFlags, "Good"));
        eduiMenuAddItem(edptl_damageflag_menu, eduiItemToggleCreate(2, edblack, (effect->field_2f2 >> 1) & 1, 2,
                                                                 cbPtlChangeDamageFlags, "Evil"));
    }
    eduiMenuAttach(menu, edptl_damageflag_menu);
    edptl_damageflag_menu->x = menu->x + 10;
    edptl_damageflag_menu->y = menu->y + 40;
}

static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpp_nearest != -1) {
        i32 instance = edpp_ptls[edpp_nearest].instance_id;
        if (instance != -1) {
            debinftype *effect = debtab[debkeydata[instance].effect_index];
            for (i32 i = 0; i < 8; ++i) {
                effect->collision_keys[i].time = effect->width_keys[i].time;
                effect->collision_keys[i].value = effect->width_keys[i].value / 10000.0f;
            }
        }
    }
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}

static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlInstanceSettingsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCancel(eduimenu_s *, eduimenu_s *) {
    edpp_active_menu = NULL;
}

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlcolmenu);
    ptlcolmenu = NULL;
    grad_item = NULL;
    grad_alpha_item = NULL;
}

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptljibmenu);
    ptljibmenu = NULL;
    grad_jib_x_freq_item = NULL;
    grad_jib_x_amp_item = NULL;
    grad_jib_y_freq_item = NULL;
    grad_jib_y_amp_item = NULL;
}

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlrotmenu);
    ptlrotmenu = NULL;
    grad_rot_item = NULL;
    grad_rot_min_item = NULL;
    grad_rot_max_item = NULL;
}

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(messagemenu);
    messagemenu = NULL;
}

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(collmenu);
    collmenu = NULL;
    coll_env_item = NULL;
}

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptldatamenu);
    ptldatamenu = NULL;
}

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlemitmenu);
    ptlemitmenu = NULL;
}

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlgravmenu);
    ptlgravmenu = NULL;
}

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlsizemenu);
    ptlsizemenu = NULL;
    grad_size_w_item = NULL;
    grad_size_h_item = NULL;
    grad_size_min_item = NULL;
    grad_size_max_item = NULL;
}

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptltypemenu);
    ptltypemenu = NULL;
}

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlgsortmenu);
    ptlgsortmenu = NULL;
}

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_torus_menu);
    edptl_torus_menu = NULL;
    torus_env1_item = NULL;
    torus_env2_item = NULL;
    torus_env3_item = NULL;
}

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlcutoffmenu);
    ptlcutoffmenu = NULL;
}

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_damage_menu);
    edptl_damage_menu = NULL;
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(sscalemenu);
    sscalemenu = NULL;
}

static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(effectlistmenu);
    effectlistmenu = NULL;
}

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlemitvelmenu);
    ptlemitvelmenu = NULL;
}

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlreadoutmenu);
    ptlreadoutmenu = NULL;
}

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *) {
    if (textureselectmenu) {
        eduiMenuDestroy(textureselectmenu);
        textureselectmenu = NULL;
    }
    eduiMenuDestroy(texturemenu);
    texturemenu = NULL;
}

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlvaremitmenu);
    ptlvaremitmenu = NULL;
}

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(etimemenu);
    etimemenu = NULL;
}

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(emittimemenu);
    emittimemenu = NULL;
}

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlstartvelmenu);
    ptlstartvelmenu = NULL;
}

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlvarstartmenu);
    ptlvarstartmenu = NULL;
}

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(changegenratemenu);
    changegenratemenu = NULL;
}

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_damageflag_menu);
    edptl_damageflag_menu = NULL;
}

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_quickdel_menu);
    edptl_quickdel_menu = NULL;
}

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_instancesettings_menu);
    edptl_instancesettings_menu = NULL;
}
