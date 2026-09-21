#include "decomp.h"
#include "MechInputTouch_types.h"
#include "legoapi/items/base/apiobject.h"

HashedKey MechJumpAutoPilotAddon::s_hashId("MechJumpAutopilotAddon");

void MechJumpAutoPilotAddon::AnalyseJumpTrajectory() {
    STUBBED();
}

void MechJumpAutoPilotAddon::CalculateModifiedJumpTrajectory() {
    STUBBED();
}

void MechJumpAutoPilotAddon::LookForBottomInt(VuVec const &) {
    STUBBED();
}

void MechJumpAutoPilotAddon::LookForLandingPoint() {
    STUBBED();
}

void MechJumpAutoPilotAddon::LookForLandingSpotAroundPoint(VuVec const &) {
    STUBBED();
}

void MechJumpAutoPilotAddon::LookForTerrInt(VuVec const &) {
    STUBBED();
}

MechJumpAutoPilotAddon::MechJumpAutoPilotAddon(MechObjectInterface &object)
    : MechAddon(object, s_hashId.value), character(object.GetCharacterObject()), state(0), elapsed_time(0.0f),
      speed_scale(1.0f), started(false) {
    character->jump_input_flags &= ~0x10;
}

void MechJumpAutoPilotAddon::ModifyJump() {
}

bool MechJumpAutoPilotAddon::OnProcess(MechAddon::ProcessStage, float) {
    STUBBED();
    return false;
}

void MechJumpAutoPilotAddon::ProcJumpingToCertainDoom() {
}

void MechJumpAutoPilotAddon::Recalculate() {
    speed_scale = 1.0f;
    state = 0;
    started = false;
    field_9c = false;
    field_9d = false;
}

MechJumpAutoPilotAddon::~MechJumpAutoPilotAddon() {
    character->jump_input_flags &= ~0x10;
}
