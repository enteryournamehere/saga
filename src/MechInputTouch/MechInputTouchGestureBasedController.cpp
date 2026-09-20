#include "decomp.h"
#include "MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/contexts.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nucore/numemory.h"
#include "legoapi/menus/core/gamehint.h"
#include "nu2api/numath/nutrig.h"

#include <new>

i32 GetMenuID();
CABLE_s *GameObjOwnsAnyCables(GameObject_s *);
void ReleaseCable(CABLE_s *, i32);
extern "C" i16 id_WATTO;
bool isBucking;

void MechInputTouchGestureBasedController::Activate() {
    if (active) {
        return;
    }
    if (field_90 != NULL && !field_90->is_down) {
        field_90 = NULL;
    }
    if (field_94 != NULL && !field_94->is_down) {
        field_94 = NULL;
    }
    active = 1;
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 150);
    field_90 = NULL;
    MechSystems::Get()->gesture_controller = this;
}

void MechInputTouchGestureBasedController::Deactivate() {
    if (!active) {
        return;
    }
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        Obj[i].KillTasks();
    }
    active = 0;
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
    if (player != NULL) {
        player->jump_input_flags &= ~0x20;
    }
    buttons_were_pressed = 0;
    buttons_pressed = 0;
    buttons_repeat = 0;
}

void MechInputTouchGestureBasedController::KillTasks(bool kill_active_task) {
    GameObject_s *object = Player[player_id];
    if (object == NULL) {
        return;
    }
    if (object->touch_task == NULL) {
        return;
    }
    if ((object->touch_task->flags & 2) != 0 && !kill_active_task) {
        return;
    }
    object->KillTasks();
}

MechInputTouchGestureBasedController::MechInputTouchGestureBasedController(
    i32 index, MechInputTouchGestureBasedController::StickMode mode)
    : MechInputTouchMainController(index), temporary_position(), stick_mode(mode), field_a5(0), target(NULL) {
    active = 0;
    field_a6 = 0;
    field_98 = 0.0f;
    current_task = NULL;
    field_90 = NULL;
    field_94 = NULL;
    field_9c = 0.0f;
    field_a0 = 0.0f;
}

bool MechInputTouchGestureBasedController::MenuDisable() {
    const i32 menu_id = GetMenuID();
    if (menu_id == 12 || menu_id == 16 || menu_id == 13 || menu_id == 17 || menu_id == 8 || menu_id == 18) {
        return true;
    }
    return menu_id == 14;
}

bool MechInputTouchGestureBasedController::OnClick(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchGestureBasedController::OnDoubleClick(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchGestureBasedController::OnDown(GameObject_s &object, TouchHolder &holder) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL) {
        return false;
    }
    VuVec position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    MechObjectInterface *force_target = MechInputTouchSystem::FindTargetObject(object, position, 0x2010, NULL, NULL);
    if (force_target != NULL) {
        StartNewTask(new MechTouchTaskUseForce(*this, force_target, position), holder, true, true);
        if (object.touch_task != NULL && object.touch_task->GetHashId().value == MechTouchTaskUseForce::HashId.value) {
            object.touch_task->Update();
        }
        return true;
    }
    if ((object.incoming_bolt != NULL || (object.field_0xe21 & 0x20) != 0) && holder.target_object != NULL &&
        holder.target_object->GetCharacterObject() == &object) {
        button_was_pressed[0] = 1;
        return true;
    }
    if (field_90 == NULL) {
        field_a0 = 0.0f;
        field_90 = &holder;
        field_a5 = 0;
    } else if (field_94 == NULL) {
        const f32 dx = holder.touch_position.x - field_90->touch_position.x;
        const f32 dy = holder.touch_position.y - field_90->touch_position.y;
        if (dx * dx + dy * dy < 1.0f) {
            field_94 = &holder;
        }
    }
    if (current_task == NULL && player != NULL && holder.target_object == player->GetMechObjectInterface() &&
        TouchHacks::CanUseVehicleSmartBomb(object)) {
        current_task = &holder;
    }
    return true;
}

bool MechInputTouchGestureBasedController::OnHold(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchGestureBasedController::OnRelease(GameObject_s &object, TouchHolder &holder) {
    if (current_task == &holder) {
        current_task = NULL;
    }
    if (field_94 == &holder) {
        field_94 = NULL;
    }
    if (field_90 == &holder) {
        if (((object.apiobj.character_data != NULL && (object.apiobj.character_data->model_flags & 0x40) != 0) ||
             object.id == id_WATTO) && object.character_context == LEGOCONTEXT_JUMP) {
            KillTasks(true);
        }
        CABLE_s *cable = GameObjOwnsAnyCables(&object);
        if (cable != NULL) {
            if (cable->target != NULL && TouchHacks::FindBombTarget(object) != NULL) {
                MechObjectInterface *target_object = cable->target->GetMechObjectInterface();
                CantPickupBombTimerAddon *addon = NU_ALLOC_T(CantPickupBombTimerAddon, 1, "", 0);
                if (addon != NULL) {
                    new (addon) CantPickupBombTimerAddon(*target_object, 6.0f);
                }
                cable->target->GetAddons(true)->Add(*addon);
            }
            ReleaseCable(cable, 0);
        }
        field_90 = NULL;
        field_a5 = 0;
    }
    if (object.touch_task != NULL && (object.touch_task->flags & 1) != 0 &&
        object.touch_task->touch_holder == &holder) {
        KillTasks(true);
        StartNewTask(NULL, holder, false, true);
    }
    return false;
}

bool MechInputTouchGestureBasedController::OnSwipe(GameObject_s &, TouchHolder &, i32) {
    STUBBED();
    return false;
}

void MechInputTouchGestureBasedController::PerformCloseMechanic(GameObject_s &, TouchHolder &) {
    STUBBED();
}

void MechInputTouchGestureBasedController::ProcessAutoJumpOverGap(GameObject_s *) {
    STUBBED();
}

void MechInputTouchGestureBasedController::ProcessAutoJumpWhenStuck(GameObject_s &) {
    STUBBED();
}

void MechInputTouchGestureBasedController::ProcessDragMovement(GameObject_s &object) {
    if (field_90 == NULL) {
        return;
    }
    f32 origin_x = field_90->down_position.x;
    f32 origin_y = field_90->down_position.y;
    if (object.id != id_GRABCONTROL && stick_mode.value == 0) {
        origin_x = MAX(-0.6f, (MIN(object.camera_screen_position.x, 0.6f)));
        origin_y = MAX(-0.6f, (MIN(object.camera_screen_position.y, 0.6f)));
    }
    NuVec2 drag = {origin_x - field_90->touch_position.x, origin_y - field_90->touch_position.y};
    if (field_94 != NULL) {
        const NuVec2 other_drag = {origin_x - field_94->touch_position.x, origin_y - field_94->touch_position.y};
        if (drag.x * drag.x + drag.y * drag.y < other_drag.x * other_drag.x + other_drag.y * other_drag.y) {
            drag = other_drag;
        }
    }
    const f32 distance = NuFsqrt(drag.y * drag.y + drag.x * drag.x);
    bool moving = distance > 0.05f;
    const NuVec2 down_drag = {field_90->down_position.x - field_90->touch_position.x,
                             field_90->down_position.y - field_90->touch_position.y};
    if (field_90->target_object == NULL && isBucking) {
        moving = true;
    }
    const bool dragged = down_drag.x * down_drag.x + down_drag.y * down_drag.y > 0.0025f;
    if (!dragged) {
        if (field_90->held_time > 0.2f || field_90->target_object == NULL) {
            if (object.character_context != -1 &&
                (object.character_context != LEGOCONTEXT_JUMP || !isBucking)) {
                moving = false;
            }
        } else {
            moving = false;
        }
    }
    field_a0 -= FRAMETIME;
    if (field_a0 > 0.0f) {
        moving = false;
    }
    const i32 context = object.character_context;
    if (context == 0x2b || context == 0x0f || context == 0x36 || context == 0x2a || context == 0x4a ||
        context == 0x47 || context == 0x22 || context == 0x61 || context == 0x3c || target.Get() != NULL || !moving) {
        field_9c = 0.0f;
        return;
    }
    if (!(object.touch_task != NULL && (object.touch_task->flags & 4) != 0) && context != 0x27 &&
        context != 0x33 && context != 0x18 && context != 0x0c && context != LEGOCONTEXT_TUBE &&
        context != LEGOCONTEXT_PUSHSPINNER && context != LEGOCONTEXT_JUMP && context != LEGOCONTEXT_BIGJUMP) {
        object.character_context = -1;
    }
    field_a5 = 1;
    KillTasks(false);
    field_90->consumed = 1;
    if (field_94 != NULL) {
        field_94->consumed = 1;
    }
    if (object.touch_task != NULL && (object.touch_task->flags & 8) == 0) {
        return;
    }
    const i32 angle = NuAtan2D(drag.x, drag.y);
    const f32 strength = (MAX(0.0f, (MIN((distance - 0.05f) * 4.0f, 1.0f)))) * 1.4f;
    const f32 stick_x = -(strength * NU_SIN_LUT(angle));
    const f32 stick_y = strength * NU_COS_LUT(angle);
    stick_values[0] = MAX(-1.0f, (MIN(stick_x, 1.0f)));
    stick_values[1] = MAX(-1.0f, (MIN(stick_y, 1.0f)));
    field_9c += FRAMETIME;
    if (field_9c > 1.0f) {
        Hint_SetComplete(0x5f3);
    }
    if (field_94 != NULL) {
        stick_values[2] = stick_values[0];
        stick_values[0] = 0.0f;
        stick_values[3] = stick_values[1];
        stick_values[1] = 0.0f;
    }
}

void MechInputTouchGestureBasedController::Render() {
}

void MechInputTouchGestureBasedController::StartJumpUsingAIPath(JumpTriggerPacket const &, i32) {
    STUBBED();
}

void MechInputTouchGestureBasedController::StartNewTask(MechTouchTask *task, TouchHolder &holder, bool clear_touches,
                                                      bool kill_tasks) {
    GameObject_s *object = Player[player_id];
    if (object == NULL) {
        return;
    }
    if (object->touch_task != NULL && (object->touch_task->flags & 2) != 0) {
        delete task;
        return;
    }
    if (kill_tasks) {
        KillTasks(false);
    }
    if (object->touch_task != NULL && task != NULL) {
        object->touch_task->OnSuspend();
        task->next = object->touch_task;
    }
    object->touch_task = task;
    if (clear_touches) {
        field_90 = NULL;
        field_94 = NULL;
    }
    if (task != NULL) {
        task->character = Player[player_id];
        task->touch_holder = &holder;
        object->touch_task->OnStart();
    }
}

void MechInputTouchGestureBasedController::TriggerJumpTask(JumpTriggerPacket const &, bool, bool, bool) {
    STUBBED();
}

void MechInputTouchGestureBasedController::Update(NuInputTouchData const *) {
    STUBBED();
}

MechInputTouchGestureBasedController::~MechInputTouchGestureBasedController() {
}
