#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/NuDynamicLight.h"

i16 Placeable::CurrentLedFile;

void PlaceableHelper::Find(char *) {
    STUBBED();
}

void PlaceableHelper::Find(char *, Placeable **, i32) {
    STUBBED();
}

void PlaceableHelper::FindObject(char *) {
    STUBBED();
}

void PlaceableHelper::GetNextObject(void *) {
    STUBBED();
}

void PlaceableHelper::GetNextObject(void *, i32 (*)(void *)) {
    STUBBED();
}

void PlaceableHelper::Initialise() {
    STUBBED();
}

void PlaceableHelper::IsEditorObject(ClassObject &) {
    STUBBED();
}

PlaceableHelper::PlaceableHelper() {
    object_type_count = 0;
}

void PlaceableHelper::RegisterObjectType(char *, PlaceableInterface *) {
    STUBBED();
}

void PlaceableInterface::DebugOutputObjects() {
    STUBBED();
}

void PlaceableInterface::Find(char *) {
    STUBBED();
}

void PlaceableInterface::Find(char *, Placeable **, i32) {
    STUBBED();
}

void PlaceableNameControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

PlaceableNameControl::PlaceableNameControl() {
    STUBBED();
}

void PlaceableNameControl::Process(EdInputContext &) {
    STUBBED();
}

void PlaceableNameControl::Render() {
    STUBBED();
}

void PlaceableNameControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void PlaceableNameControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void PlaceableNameControl::cbSelectObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

VuVec const *Placeable::GetCurrentPosition() const {
    VuMtx const *matrix = GetCurrentTransform();
    return matrix ? reinterpret_cast<VuVec const *>(&matrix->matrix.m30) : NULL;
}

VuVec const *Placeable::GetInitialPosition() const {
    VuMtx const *matrix = GetInitialTransform();
    return matrix ? reinterpret_cast<VuVec const *>(&matrix->matrix.m30) : NULL;
}

void Placeable::SetCurrentPosition(VuVec const *position) {
    NUMTX matrix;
    VuMtx const *current = GetCurrentTransform();
    // The original updates only a temporary matrix.
    if (current) {
        matrix = current->matrix;
        NuMtxTranslate(&matrix, const_cast<NUVEC *>(&position->xyz));
    } else {
        NuMtxSetTranslation(&matrix, const_cast<NUVEC *>(&position->xyz));
    }
}

void Placeable::SetInitialPosition(VuVec const *position) {
    NUMTX matrix;
    VuMtx const *initial = GetInitialTransform();
    if (initial) {
        matrix = initial->matrix;
        NuMtxTranslate(&matrix, const_cast<NUVEC *>(&position->xyz));
    } else {
        NuMtxSetTranslation(&matrix, const_cast<NUVEC *>(&position->xyz));
    }
}
