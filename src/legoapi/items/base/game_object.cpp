#include "legoapi/world/world_shared.h"

#include "decomp.h"
#include "globals.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>
#include <new>

// Forward declarations for local (static) game-object helper stubs.
struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;
struct BOLT_s;
struct debinftype;

extern void SetGameObjectCharacterData(GameObject_s *obj);
extern void GetTopBot(GameObject_s *obj);
extern void GameObjectDimensions(GameObject_s *obj);
extern void GameObjectOrigin(GameObject_s *obj);
extern void ResetCharacterIdle(GameObject_s *obj, i32 mode, i32 idle);
extern void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *data);
extern void Hub_ResetPanel();

extern "C" {
    extern i16 id_MOSEISLEYCITIZEN;
    extern i16 id_CANTINAALIEN;
    extern i16 id_CLOUDCITYCITIZEN;
    extern i16 id_GEONOSIAN;
    extern i16 id_BOB;
    extern i16 id_SPEEDERBIKE;
    extern i16 id_SPEEDERBIKESNOW;
    extern i16 id_STAP;
    extern i16 id_STAP2;
    extern i16 id_TROOPERCANNON;
    extern i16 id_CANNON;
    extern i16 id_MOSCANNON;
    extern i16 id_ATST;
    extern i16 id_BASKETCANNON;
    extern i16 id_BIGGUN;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_GAMORREANGUARD;
    extern i16 id_ZAMSSPEEDER;
}

i32 addcreature_override_id_check;
f32 default_mover_extra = 0.05f;

void ClearGameObjects(APIOBJECTSYS_s *api_object_sys) {
    for (i32 i = 0; i < 64; i++) {
        Obj[i].KillTasks();
        Obj[i].ClearAddons();
        Obj[i].ClearMechObjectInterface();
    }
    memset(Obj, 0, sizeof(GameObject_s) * 64);
    APIObjectDestroyAll(api_object_sys);
    HIGHGAMEOBJECT = 0;
}

GameObject_s *AddGameObject(i32 id) {
    GameObject_s *object = reinterpret_cast<GameObject_s *>(APIObjectCreate(WORLD->api_object_sys));
    if (object == NULL) {
        return NULL;
    }

    const u8 object_index = object->apiobj.field_0x289;
    object->field_0x661 = 0xff;
    object->apiobj.field_0x27f = 0xff;
    object->apiobj.field_0x280 = 0xff;
    object->field_0x1086 = 2;
    object->apiobj.flags_low |= APIOBJECT_FLAG_IN_USE;
    object->apiobj.character = 1;
    object->field_0x1054 = 1;
    object->apiobj.collision_identity_mask = u64(1) << object_index;
    object->apiobj.field_0xa8 = 1.0f;
    object->field_0x1004 = 1.0f;
    object->field_0x1020 = 2000000.0f;
    object->apiobj.field_0x281 = 0xff;
    object->apiobj.field_0x218 = object->apiobj.water_height = object->apiobj.field_0x220 = 2000000.0f;

    HIGHGAMEOBJECT = 0;
    for (i32 i = 0; i < 64; i++) {
        if ((Obj[i].apiobj.flags_low & APIOBJECT_FLAG_IN_USE) != 0) {
            HIGHGAMEOBJECT = i + 1;
        }
    }

    object->ai.owner = object;
    object->apiobj.field_0x2a8 = 0;
    object->apiobj.field_0x2ac = 0;
    object->apiobj.objptr = object;
    object->apiobj.ai = &object->ai;

    MechAddonCollection *addons = object->GetAddons(true);
    if (addons != NULL) {
        MechEdgeStopAddon *edge_stop = new MechEdgeStopAddon(*object->GetMechObjectInterface());
        addons->Add(*edge_stop);
        if (VehicleArea != 0 || id == id_SPEEDERBIKE || id == id_SPEEDERBIKESNOW || id == id_STAP ||
            id == id_STAP2 || id == id_TROOPERCANNON || id == id_CANNON || id == id_MOSCANNON ||
            id == id_ATST || id == id_BASKETCANNON || id == id_BIGGUN) {
            MechObjectInterface *target = object->GetMechObjectInterface();
            MechAutofireAddon *addon = NU_ALLOC_T(MechAutofireAddon, 1, "", 0);
            if (addon != NULL)
                new (addon) MechAutofireAddon(*target);
            addons->Add(*addon);
        }
    }
    return object;
}
// Local (static) game-object behaviour codes and per-object helpers. Stubbed
// as local `t` symbols matching res/libTTapp.so.

static __used__ void ShieldCode(GameObject_s *object) {
    f32 target = 0.0f;
    if (object->field_0xe37 != 0) {
        if (object->id == id_ZAMSSPEEDER && WORLD->area != NULL && WORLD->area == BOUNTYHUNTERPURSUIT_ADATA) {
            target = 5.0f;
        } else {
            const i32 animation = CurrentAnim(&object->apiobj.anim_packet);
            if ((object->apiobj.field_0x27d != 0 || object->ground_contact_grace_timer > 0.0f) &&
                animation != 3 && animation != -1 && animation != 5 && animation != 0x23 && animation != 0x6a &&
                object->character_context != 0x17 && object->character_context != 0x3d) {
                target = 1.0f;
            }
        }
    }
    const f32 previous = object->field_0xd24;
    object->field_0xd24 = SeekLinearF(previous, target, 5.0f * FRAMETIME);
    if ((previous == 0.0f && object->field_0xd24 > 0.0f) ||
        (previous == 1.0f && object->field_0xd24 < 1.0f)) {
        PlaySfx("DDekaShOn", &object->apiobj.collision_position);
    }
}

static __used__ void TrenchMove(GameObject_s *) {
    STUBBED();
}

static __used__ void Punch_HitHold(GameObject_s *attacker, GameObject_s *target) {
    if (target != NULL && attacker != NULL &&
        (target->id == id_IMPERIALGUARD || target->id == id_GAMORREANGUARD)) {
        GameAudio_PlaySfx(0x4a, &target->apiobj.collision_position, 0, 0);
    }
}

static __used__ i32 Punch_GetDamage_LSW(GameObject_s *attacker, GameObject_s *target) {
    if (attacker->apiobj.character_data->game_character->field275_0x116 == 7 && Cheat_IsOn(0x0c)) {
        GameAudio_PlaySfx(0x4a, target != NULL ? &target->apiobj.collision_position :
                                             &attacker->apiobj.collision_position, 0, 0);
        GameCam_NewShake(NULL, 0.75f, 0.75f, 1.0f);
        return -1;
    }
    if (target != NULL && (target->id == id_IMPERIALGUARD || target->id == id_GAMORREANGUARD) &&
        (target->character_context == 0x18 || target->character_context == 0x0c)) {
        GameAudio_PlaySfx(0x4a, &target->apiobj.collision_position, 0, 0);
    }
    return 1;
}

static __used__ void Punch_HitExtraCode_LSW(GameObject_s *object, nuvec_s *position) {
    if (object->id == id_GAMORREANGUARD &&
        ((object->character_context == 0x26 && object->context_animation == 0x56) ||
         object->character_context == 0x0d)) {
        NewRumbleAllPlayers(0.4f, 0.0f, 1, 0);
        NewRumble(object->pad_gamepad->pad, 0.7f, 0);
        GameCam_Judder(GameCam, 0.25f, 0, &object->apiobj.collision_position);
        PlaySfx("fs_gamorr_land", &object->apiobj.lower_position);
    } else if ((AnimMiscFlags(object->apiobj.character_model, object->context_animation) & 4) != 0) {
        if (position != NULL)
            PlaySfx("WhipHit", position);
        else
            PlaySfx("WhipNowt", &object->apiobj.collision_position);
    }
}

static __used__ void TrenchKilledCallback(GameObject_s *) {
    STUBBED();
}

static __used__ void SurfaceInfo_ExtraReflect(GameObject_s *object) {
    if (WORLD->current_level == CRUISERE_LDATA && object->field_0x1020 == 2000000.0f &&
        object->apiobj.position.x < 11.0f) {
        object->field_0x1087 = 3;
        object->field_0x1020 = -39.2f;
    }
    if (WORLD->current_level == DEATHSTARRESCUED_LDATA) {
        if (object->apiobj.position.z > 20.75f) {
            object->field_0x1020 = 22.4f;
            object->field_0x1087 = 3;
        }
        if (object->apiobj.position.x < -20.75f) {
            object->field_0x1087 = 1;
            object->field_0x1020 = -22.4f;
        }
    }
}

static __used__ void PauseGame_ExtraCode() {
    Hub_ResetPanel();
}

static __used__ i32 SpecialObjectFilter(void *) {
    STUBBED();
    return 0;
}

static __used__ void KilledTrooperCannon(GameObject_s *) {
    STUBBED();
}
