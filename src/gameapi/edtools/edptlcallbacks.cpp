#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"

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

static i32 edptl_superscale = 1;

// Particle-list editor UI/menu callback stubs (static, internal linkage).

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
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

static void edptlcbSetSoundControl(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbSoundControlMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
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
    STUBBED();
}

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCollMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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
    STUBBED();
}

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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
    STUBBED();
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
    STUBBED();
}

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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

static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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
    STUBBED();
}

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
