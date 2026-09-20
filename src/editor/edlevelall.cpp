#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include <string.h>
#include <new>

extern EdRegistry theRegistry;
extern i32 EdType_String;

DECOMP_ASSERT(offsetof(EdClass, flags) == 0x04, "EdClass flags offset");
DECOMP_ASSERT(offsetof(EdClass, members) == 0x08, "EdClass members offset");
DECOMP_ASSERT(offsetof(EdClass, last_member) == 0x0c, "EdClass last member offset");
DECOMP_ASSERT(offsetof(EdClass, member_count) == 0x10, "EdClass member count offset");
DECOMP_ASSERT(offsetof(EdClass, interface) == 0x14, "EdClass interface offset");
DECOMP_ASSERT(sizeof(EdMember) == 0x08, "EdMember size");
DECOMP_ASSERT(offsetof(EdMember, reference) == 0x04, "EdMember reference offset");

i32 BaseEditor::blockDepth;
i32 BaseEditor::blockStart[8];
eduimenu_s *edLevelActiveMenu;
extern eduimenu_s *edLevelPinnedMenu;
i32 edLevelDestroyActiveMenu;
LevelEditor theLevelEditor;

extern "C" void eduiSetCameraEnabled(i32);
eduimenu_s *GetMenuActiveChild(eduimenu_s *);
extern "C" void NuFntSet(i32);
extern "C" void NuFntScale(i32, i32);

PropertyTool thePropertyTool;
PropertyMenuMetrics menu_startmetrics = {20, 5, 200, 400};
eduiiattr_s EdLevelAttr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
i32 EdLevelFnt;
i32 EdLevelFntScale = 24;

void BaseEditor::ReadBuffer(void **destination, void *source, i32 size) {
    if (field_0x0c != 0) {
        memcpy(*destination, source, size);
    } else {
        *destination = source;
    }
}

void BaseEditor::WriteBeginBlock(i32 file, i32 type) {
    blockStart[blockDepth++] = NuFilePos(file);
    NuFileWriteInt(file, type);
    NuFileWriteInt(file, 0);
    NuFileWriteInt(file, 0);
    NuFileWriteInt(file, 0);
}

void BaseEditor::WriteEndBlock(i32 file) {
    --blockDepth;
    NuFileAlign(file, 15);
    i32 end = NuFilePos(file);
    NuFileSeek(file, blockStart[blockDepth] + 4, NUFILE_SEEK_START);
    NuFileWriteInt(file, end - blockStart[blockDepth]);
    NuFileSeek(file, end, NUFILE_SEEK_START);
}

void BaseEditor::WriteMetaData(i32 file, i32 type, i32 version, i32 count) {
    NuFileWriteInt(file, 1);
    NuFileWriteInt(file, type);
    NuFileWriteInt(file, version);
    NuFileWriteInt(file, count);
}

void CursorTool::Initialise(variptr_u &, variptr_u &, i32) {
}

void CursorTool::Process(EdInputContext &) {
    STUBBED();
}

void CursorTool::Render() {
    STUBBED();
}

ClassEditor::ClassEditor() {
    STUBBED();
}

void ClassEditor::AddMenuItems(eduimenu_s *) {
    STUBBED();
}

void ClassEditor::ClearLevel(i32) {
    STUBBED();
}

void ClassEditor::CreateObject() {
    STUBBED();
}

void ClassEditor::CreateObject(ClassObject &) {
    STUBBED();
}

void ClassEditor::CreateObject(EdClass *) {
    STUBBED();
}

void ClassEditor::CreateObject(i32) {
    STUBBED();
}

void ClassEditor::Enter() {
    STUBBED();
}

void ClassEditor::Exit() {
    STUBBED();
}

void ClassEditor::Flush() {
    STUBBED();
}

void ClassEditor::Initialise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

void ClassEditor::Process(EdInputContext &) {
    STUBBED();
}

void ClassEditor::ReadBlock(DATAPTR *) {
    STUBBED();
}

void ClassEditor::Render() {
    STUBBED();
}

void ClassEditor::Serialise(EdStream &) {
    STUBBED();
}

void ClassEditor::WriteBlock(i32) {
    STUBBED();
}

void ClassEditor::DestroySelectedObjects() {
    STUBBED();
}

void ClassEditor::DestroySelectedObjectsNow() {
    STUBBED();
}

void ClassEditor::DrawObjectSphere(ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::Editable(void *, EdClass *, i32) {
    STUBBED();
}

void ClassEditor::FindNearestObject(VuVec &, ClassObject &, ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::FindNearestObject(VuVec &, ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::FindNearestObject(VuVec &, VuVec &, ClassObject &, ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::FindNearestObject(VuVec &, VuVec &, ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::FocusSelected() {
    STUBBED();
}

void ClassEditor::InitialiseObject(ClassObject &) {
    STUBBED();
}

void ClassEditor::IsSelectedClass(EdClass *) {
    STUBBED();
}

void ClassEditor::IsSelectedObject(ClassObject &) {
    STUBBED();
}

void ClassEditor::IsSelectedObject(void *, EdRef *) {
    STUBBED();
}

void ClassEditor::IsUniqueName(char *) {
    STUBBED();
}

void ClassEditor::MakeUniqueName(char const *, char *, i32) {
    STUBBED();
}

void ClassEditor::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void ClassEditor::PostSaveInitialisation() {
    STUBBED();
}

void ClassEditor::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void ClassEditor::PreSaveInitialisation() {
    STUBBED();
}

void ClassEditor::SelectLED(i32) {
    STUBBED();
}

void ClassEditor::SelectObject(ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::SetMode(i32) {
    STUBBED();
}

void ClassEditor::SetViewMenuHilight(eduimenu_s *) {
    STUBBED();
}

void ClassEditor::SnapPoint(VuVec &) {
    STUBBED();
}

void ClassEditor::UpdateClassFilter(EdInputContext &) {
    STUBBED();
}

void ClassEditor::UpdateLists(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void ClassEditor::UpdateSelectedObjects(EdInputContext &) {
    STUBBED();
}

void ClassEditor::ViewSelected() {
    STUBBED();
}

void ClassEditor::cbDestroyMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

void ClassEditor::cbDestroyObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassDeleteObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassExportMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassFileMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassImportMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassModeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassNewMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassNewObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassRemoveDuplicates(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSelectClassMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSelectObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSelectObjectMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSetMode(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSetPinned(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSetSnap(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSetView(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSnapMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassToolsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassViewMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdCopySelectedObject(EdInputContext &) {
    STUBBED();
}

void ClassEditor::cbEdCreateClassNewObject(i32) {
    STUBBED();
}

void ClassEditor::cbEdFilterLED(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdLevelDeselectAll(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdLevelSelectAll(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdPadSetManipulatorMode(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbFileSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassObject::GetName(char *, i32) {
    STUBBED();
}

void ClassObject::Set(char *) {
    STUBBED();
}

void LevelEditor::AddInfoText(char *text) {
    for (i32 i = 0; i < 32; ++i) {
        if (info_text[i] == NULL) {
            info_text[i] = AddText(text);
            return;
        }
    }
}

i32 LevelEditor::AddScene(char *name, nugscn_s *scene, i32 active) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (NuStrICmp(scenes[i].name, name) == 0) {
            scenes[i].scene = scene;
            scenes[i].active = active;
            return i;
        }
    }
    if (reset_pending < 10) {
        i32 index = reset_pending++;
        NuStrNCpy(scenes[index].name, name, 32);
        scenes[index].scene = scene;
        scenes[index].active = active;
        return index;
    }
    return 0;
}

char *LevelEditor::AddText(char *text) {
    i32 size = NuStrLen(text) + 1;
    char *result = text_buffer + text_length;
    NuStrCpy(result, text);
    text_length += size;
    return result;
}

void LevelEditor::BeginMultiLoad(variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::ClearLevel(i32 index) {
    scenes[index].active = 0;
    scenes[index].scene = NULL;
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->ClearLevel(index);
    }
}

void LevelEditor::CloseMenu() {
    eduiSetCameraEnabled(1);
    if (edLevelActiveMenu != NULL) {
        if (edLevelActiveMenu->parent != NULL) {
            edLevelActiveMenu->parent->child = NULL;
        }
        if (edLevelActiveMenu->child != NULL) {
            edLevelActiveMenu->child->parent = NULL;
        }
    }
    edLevelDestroyActiveMenu = 1;
}

void LevelEditor::CreateEditorList(eduimenu_s *, eduiitem_s *) {
    STUBBED();
}

void LevelEditor::CreateMenu() {
    STUBBED();
}

void LevelEditor::Display(ThingRenderData *) {
    STUBBED();
}

void LevelEditor::DrawInfoText(char **, i32, i32, i32, i32, i32, i32, i32) {
    STUBBED();
}

void LevelEditor::EndMultiLoad(variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::Enter() {
    edLevelActiveMenu = NULL;
    eduiSetUsingMenuFocus(1);
    editors_entered = 1;
    active = 1;
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->Enter();
    }
}

void LevelEditor::Exit() {
    active = 0;
    eduiSetUsingMenuFocus(0);
    if (editors_entered != 0) {
        for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
            editor->Exit();
        }
        editors_entered = 0;
    }
}

i32 LevelEditor::FindSceneId(char *name) {
    for (i32 i = 0; i < 10; ++i) {
        if (NuStrICmp(scenes[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void LevelEditor::Flush() {
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->Flush();
    }
    reset_pending = 0;
    memset(scenes, 0, sizeof(scenes));
}

LevelEditorScene *LevelEditor::GetEdScene(i32 index) {
    if (static_cast<u32>(index) >= 10) {
        return NULL;
    }
    return &scenes[index];
}

nugscn_s *LevelEditor::GetScene(char *name) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (NuStrICmp(name, scenes[i].name) == 0) {
            return scenes[i].scene;
        }
    }
    return NULL;
}

void LevelEditor::Initalise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

i32 LevelEditor::IsActiveScene(nugscn_s *scene) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (scenes[i].scene == scene) {
            return scenes[i].active;
        }
    }
    return 0;
}

i32 LevelEditor::IsEditable(i32 index) {
    if (index == -1) {
        return 1;
    }
    if (static_cast<u32>(index) >= 10) {
        return 0;
    }
    return scenes[index].editable != 0 || scenes[index].active == 0;
}

LevelEditor::LevelEditor() {
    first_editor = NULL;
    last_editor = NULL;
    editor_count = 0;
    background_colour[3] = 1.0f;
    overlay_alpha = 0.6f;
    active = 0;
    background_colour[0] = 0.0f;
    reset_pending = 0;
    background_colour[1] = 0.0f;
    field_0x20 = 0;
    background_colour[2] = 0.0f;
    field_0x24 = 0;
    field_0x28 = 1;
    field_0x30 = 1;
    field_0x38 = 1;
    info_x = 0;
    info_y = 0;
    info_width = 640;
    info_height = 448;
    info_colour = 0x80808080;
    info_background = 0x40000000;
    pad_x = 0;
    pad_y = 0;
    pad_width = 640;
    pad_height = 448;
    pad_background = 0x80000000;
    pad_colour = 0x80808080;
    SetPadText(0x1000, const_cast<char *>("D-Up"));
    SetPadText(0x4000, const_cast<char *>("D-Down"));
    SetPadText(0x8000, const_cast<char *>("D-Left"));
    SetPadText(0x2000, const_cast<char *>("D-Right"));
    SetPadText(0x10, const_cast<char *>("Triangle"));
    SetPadText(0x40, const_cast<char *>("Cross"));
    SetPadText(0x80, const_cast<char *>("Square"));
    SetPadText(0x20, const_cast<char *>("Circle"));
    SetPadText(4, const_cast<char *>("L1"));
    SetPadText(1, const_cast<char *>("L2"));
    SetPadText(8, const_cast<char *>("R1"));
    SetPadText(2, const_cast<char *>("R2"));
    SetPadText(0x200, const_cast<char *>("L3"));
    SetPadText(0x400, const_cast<char *>("R1"));
    SetPadText(0x800, const_cast<char *>("START"));
    SetPadText(0x100, const_cast<char *>("SELECT"));
}

void LevelEditor::Load(char *, variptr_u *, variptr_u *, i32) {
    STUBBED();
}

void LevelEditor::LoadState(variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::ProcessEvenWhenPaused(ThingProcessData *) {
    STUBBED();
}

void LevelEditor::ReadStream(EdFileInputStream &) {
    STUBBED();
}

void LevelEditor::Reset() {
    reset_pending = 0;
}

void LevelEditor::Save() {
    STUBBED();
}

void LevelEditor::SaveState(i32, variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::SaveState(variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::SetPadText(i32 buttons, char *text) {
    for (i32 i = 0; i < 32; ++i) {
        if ((static_cast<u32>(buttons) & (1u << i)) != 0) {
            pad_text[i] = text;
        }
    }
}

void LevelEditor::SetSaveFilename(char *name) {
    if (name != NULL) {
        NuStrCpy(save_filename, name);
        NuStrCpy(editor_filename, save_filename);
        char *extension = NuStrRChr(editor_filename, '.');
        if (extension != NULL) {
            *extension = '\0';
        }
        NuStrCat(editor_filename, ".led");
    } else {
        save_filename[0] = '\0';
        editor_filename[0] = '\0';
    }
}

void LevelEditor::WriteStream(EdFileOutputStream &) {
    STUBBED();
}

void PropertyMenu::AddObject(ClassObject &object) {
    if (object_count < 8) {
        objects[object_count++] = object;
    }
}

void PropertyMenu::ClearObjecs() {
    object_count = 0;
}

bool PropertyMenu::ContainsObject(ClassObject &object) {
    for (i32 i = 0; i < object_count; ++i) {
        if (objects[i].object == object.object && objects[i].reference == object.reference) {
            return true;
        }
    }
    return false;
}

void PropertyMenu::Destroy() {
    eduicbMenuCloseAllexpanders(menu);
    if (menu != NULL) {
        for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
            delete static_cast<EdControl *>(item->data_ptr);
        }
    }
    eduiMenuDestroy(menu);
}

void PropertyMenu::SelectAttr(i32 selected) {
    for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
        EdControl *control = static_cast<EdControl *>(item->data_ptr);
        if (control != NULL) {
            control->SetMenuItemAttr(selected, item, &thePropertyTool.selected_attr,
                                     &thePropertyTool.unselected_attr);
        }
    }
}

void PropertyTool::AddPropertyMenuItems(eduimenu_s *, EdClass *, void *, eduiitem_s *) {
    STUBBED();
}

void PropertyTool::AutoLocateMenu(PropertyMenu *property_menu) {
    eduimenu_s *menu = property_menu->menu;
    menu->x = 70;
    menu->y = 30;
    bool use_stored_metrics = true;
    PropertyMenu *other = active_menu;
    while (other != NULL) {
        if (other->menu != menu) {
            if (menu->x + menu->width >= other->menu->x &&
                other->menu->x + other->menu->width >= menu->x) {
                menu->x += 10;
                if (static_cast<float>(menu->x + menu->width) > 590.0f) {
                    menu->x = 20;
                    menu->y = 5;
                    return;
                }
                use_stored_metrics = false;
                other = active_menu;
                continue;
            }
            use_stored_metrics = false;
        }
        other = other->next;
    }
    if (use_stored_metrics) {
        ediMenuRetrieveMetrics(menu);
    }
    if (static_cast<float>(menu->x + menu->width) > 590.0f) {
        menu->x = 20;
        menu->y = 5;
    }
}

void PropertyTool::BringToFront(PropertyMenu *menu) {
    if (menu->next != NULL) {
        menu->next->previous = menu->previous;
    } else {
        last_menu = menu->previous;
    }
    if (menu->previous != NULL) {
        menu->previous->next = menu->next;
    } else {
        active_menu = menu->next;
    }
    menu->next = NULL;
    menu->previous = NULL;
    --menu_count;
    menu->order = -2;
    PropertyMenu *position = active_menu;
    while (position != NULL && position->order < -1) {
        position = position->next;
    }
    if (position != NULL) {
        menu->next = position;
        menu->previous = position->previous;
        if (position->previous != NULL) {
            position->previous->next = menu;
        } else {
            active_menu = menu;
        }
        position->previous = menu;
    } else {
        menu->previous = last_menu;
        if (last_menu != NULL) {
            last_menu->next = menu;
        }
        last_menu = menu;
        if (active_menu == NULL) {
            active_menu = menu;
        }
    }
    ++menu_count;
}

void PropertyTool::CreatePropertyMenu(ClassObject &) {
    STUBBED();
}

PropertyMenu *PropertyTool::FindItemMenu(PropertyMenu *menu, ClassItem *item) {
    for (; menu != NULL; menu = menu->next) {
        if (menu->ContainsObject(item->object)) {
            return menu;
        }
    }
    return NULL;
}

PropertyMenu *PropertyTool::GetActiveMenu(PropertyMenu *menu) {
    for (; menu != NULL; menu = menu->next) {
        if (menu->menu == eduiGetActiveMenuParent()) {
            return menu;
        }
    }
    return NULL;
}

void PropertyTool::GetClassName(EdRef *, char *) {
    STUBBED();
}

PropertyMenu *PropertyTool::GetNextActiveMenu() {
    PropertyMenu *menu = GetActiveMenu(active_menu);
    return menu != NULL ? menu->next : NULL;
}

eduimenu_s *PropertyTool::GetNextDefaultActiveMenu(eduimenu_s *menu) {
    if (menu == NULL) {
        return edLevelActiveMenu;
    }
    return menu == edLevelActiveMenu ? edLevelPinnedMenu : NULL;
}

void PropertyTool::GetTypeName(EdRef *, char *) {
    STUBBED();
}

void PropertyTool::Initialise(variptr_u &, variptr_u &, i32) {
}

i32 PropertyTool::Process(EdInputContext &input) {
    i32 result = ProcessMenu(input);
    ProcessControls(input);
    return result;
}

i32 PropertyTool::ProcessControls(EdInputContext &input) {
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        if (menu->control != NULL) {
            menu->control->Process(input);
        }
    }
    return 0;
}

i32 PropertyTool::ProcessMenu(EdInputContext &) {
    STUBBED();
    return 0;
}

PropertyTool::PropertyTool() {
    next = NULL;
    previous = NULL;
    active_menu = NULL;
    last_menu = NULL;
    menu_count = 0;
    show_type_names = 1;
    menu_attr.background = 0x80000000;
    menu_attr.text = 0x80ff0000;
    menu_attr.highlight = 0x80804040;
    menu_attr.disabled = 0x80404040;
    selected_attr.background = 0x80000000;
    selected_attr.text = 0x80ff0000;
    selected_attr.highlight = 0x80406080;
    selected_attr.disabled = 0x80404040;
    unselected_attr.background = 0x80000000;
    unselected_attr.text = 0x80ff0000;
    unselected_attr.highlight = 0x80707080;
    unselected_attr.disabled = 0x80808080;
}

void PropertyTool::RefreshMenuControls(PropertyMenu *property_menu) {
    eduiitem_s *item = property_menu->menu->field_0c;
    NuFntSet(EdLevelFnt);
    NuFntScale(EdLevelFntScale, EdLevelFntScale);
    for (; item != NULL; item = item->next) {
        EdControl *control = static_cast<EdControl *>(item->data_ptr);
        if (control != NULL) {
            control->Refresh();
        }
        if (item == property_menu->menu->field_10) {
            break;
        }
    }
}

void PropertyTool::Render() {
    PropertyMenu *selected = NULL;
    for (PropertyMenu *menu = last_menu; menu != NULL; menu = menu->previous) {
        if (menu == GetActiveMenu(active_menu)) {
            selected = menu;
        } else {
            RenderMenu(menu);
        }
    }
    if (selected != NULL) {
        RenderMenu(selected);
    }
}

void PropertyTool::RenderMenu(PropertyMenu *) {
    STUBBED();
}

void PropertyTool::RetrievePropertyMenu(ClassObject *, PropertyMenuList *) {
    STUBBED();
}

void PropertyTool::SelectAttr(i32 selected) {
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        menu->SelectAttr(selected);
    }
}

void PropertyTool::SetDefaultActiveMenu(PropertyMenu *menu) {
    if (menu != NULL && menu->menu != NULL) {
        eduiSetActiveMenu(GetMenuActiveChild(menu->menu));
    } else {
        eduiSetActiveMenu(edLevelActiveMenu != NULL ? edLevelActiveMenu : edLevelPinnedMenu);
    }
}

void PropertyTool::ToggleActiveMenu() {
    PropertyMenu *menu = GetActiveMenu(active_menu);
    if (menu != NULL) {
        if (menu == last_menu) {
            eduiSetActiveMenu(NULL);
        } else {
            SetDefaultActiveMenu(GetNextActiveMenu());
        }
    } else if (GetNextDefaultActiveMenu(eduiGetActiveMenu()) == NULL) {
        SetDefaultActiveMenu(active_menu);
    }
}

PropertyMenuMetrics PropertyTool::ediGetMenuStartMetrics() {
    return menu_startmetrics;
}

void PropertyTool::ediMenuRetrieveMetrics(eduimenu_s *menu) {
    menu->x = menu_startmetrics.x;
    menu->y = menu_startmetrics.y;
    menu->width = menu_startmetrics.width;
    menu->height = menu_startmetrics.height;
}

void PropertyTool::ediMenuStoreMetrics(eduimenu_s *menu) {
    menu_startmetrics.x = menu->x > 0 ? menu->x : 20;
    menu_startmetrics.y = menu->y > 0 ? menu->y : 5;
    menu_startmetrics.width = menu->width;
    menu_startmetrics.height = menu->height;
}

void ClassObjectList::GetAveragePosition(VuVec &) {
    STUBBED();
}

void ClassObjectList::GetAveragePosition(VuVec &, float &) {
    STUBBED();
}

bool ClassObjectList::IsInList(void *object, EdRef *reference) {
    ClassObjectListEntry *entry = first;
    if (entry == NULL) {
        return false;
    }

    if (reference != NULL) {
        do {
            if (entry->object == object && entry->reference == reference) {
                return true;
            }
            entry = entry->next;
        } while (entry != NULL);
    } else {
        do {
            if (entry->object == object) {
                return true;
            }
            entry = entry->next;
        } while (entry != NULL);
    }
    return false;
}

void DumpAreaData(i32, i32) {
    STUBBED();
}

void cbEdLevelSave(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void areaEditor_Enter() {
    STUBBED();
}

void cbEdLevelDestroy(eduimenu_s *menu, eduimenu_s *) {
    if ((menu->flags & 4) != 0) {
        return;
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    eduiSetCameraEnabled(1);
    if (menu == edLevelActiveMenu) {
        edLevelActiveMenu = NULL;
    }
    if (menu == edLevelPinnedMenu) {
        edLevelPinnedMenu = NULL;
    } else if (edLevelPinnedMenu != NULL && edLevelActiveMenu == NULL) {
        edLevelActiveMenu = edLevelPinnedMenu;
    }
}

void cbEdLevelSetText(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void areaEditor_Render(i32, i32, float, float) {
    STUBBED();
}

void areaEditor_Process(nupad_s *) {
    STUBBED();
}

void cbEdLevelToggleInt(eduimenu_s *, eduiitem_s *item, u32) {
    i32 *value = static_cast<i32 *>(item->data_ptr);
    *value ^= 1;
    item->highlighted = *value;
}

void cbCEDeleteConfirmed(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void LightEverythingInEditor(void *) {
    STUBBED();
}

void cbEdLevelDestroyOnSelect(eduimenu_s *menu, eduiitem_s *, u32) {
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    if (menu == edLevelActiveMenu) {
        edLevelActiveMenu = NULL;
    }
    if (menu == edLevelPinnedMenu) {
        edLevelPinnedMenu = NULL;
    }
}

void EdClass::SerialiseObject(EdStream &stream, void *object) {
    if (stream.BeginBlock("Object") == NULL) {
        return;
    }
    if (object != NULL) {
        u8 data[256];
        for (EdRef *member = members; member != NULL; member = member->next) {
            if ((stream.flags & 0x400000) != 0 ? (member->attributes & 0x400000) != 0
                                              : (member->attributes & 0x10000000) != 0) {
                continue;
            }
            if (member->attributes < 0) {
                EdClass *member_class = theRegistry.GetClass(member->type_id);
                member_class->SerialiseObject(stream, member->GetMemberObject(object));
            } else {
                EdType *type = theRegistry.GetType(member->type_id);
                i32 size = member->size > 0 ? member->size : type->size;
                if (stream.mode == 2) {
                    member->GetMemberData(object, member->type_id, data, sizeof(data));
                }
                type->serialise(stream, data, size);
                if (stream.mode == 1) {
                    member->SetMemberData(object, member->type_id, data, sizeof(data), NULL);
                }
            }
        }
        if (interface != NULL) {
            interface->vtable->serialise_object(interface, stream, object);
        }
    }
    stream.EndBlock();
}

u8 EdClass::SerialiseObjectHeader(EdStream &stream, void *object) {
    u8 present = 0;
    if (stream.mode == 2 && object != NULL) {
        present = 1;
    }
    stream.SerialiseBuffer(&present, 1, 1);
    return present;
}

void EdClass::CopyObject(void *destination, void *source) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if ((member->attributes & 0x1800000) != 0) {
            continue;
        }
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *source_member = member->GetMemberObject(source);
            void *destination_member = member->GetMemberObject(destination);
            member_class->CopyObject(destination_member, source_member);
        } else {
            theRegistry.GetType(member->type_id);
            u8 data[256];
            member->GetMemberData(source, member->type_id, data, sizeof(data));
            member->SetMemberData(destination, member->type_id, data, sizeof(data), NULL);
        }
    }
}

void EdClass::AddType(EdRef *member) {
    member->next = NULL;
    member->previous = last_member;
    if (last_member != NULL) {
        last_member->next = member;
    }
    EdRef *first = members;
    last_member = member;
    if (first == NULL) {
        members = member;
    }
    ++member_count;
    flags |= member->attributes & 0x17a;
    if (member->attributes < 0) {
        flags |= theRegistry.GetClass(member->type_id)->flags & 0x17a;
    }
}

void EdClass::Serialise(EdStream &stream, i32 *class_mapping) {
    if (stream.BeginBlock("Class") == NULL) {
        return;
    }
    stream.SerialiseString(&name);
    i32 count;
    if (stream.version == 0) {
        stream.SerialiseBuffer(&count, sizeof(count), 1);
    }
    if (stream.mode == 2) {
        count = 0;
        for (EdRef *member = members; member != NULL; member = member->next) {
            if ((stream.flags & 0x400000) != 0) {
                if ((member->attributes & 0x400000) != 0) {
                    continue;
                }
            } else {
                if ((member->attributes & 0x10000000) != 0 ||
                    (class_mapping != NULL && member->attributes < 0 &&
                     class_mapping[member->type_id] == -1)) {
                    continue;
                }
            }
            ++count;
        }
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (EdRef *member = members; member != NULL; member = member->next) {
            if ((stream.flags & 0x400000) != 0) {
                if ((member->attributes & 0x400000) != 0) {
                    continue;
                }
            } else {
                if ((member->attributes & 0x10000000) != 0 ||
                    (class_mapping != NULL && member->attributes < 0 &&
                     class_mapping[member->type_id] == -1)) {
                    continue;
                }
            }
            member->Serialise(stream, class_mapping);
        }
    }
    if (stream.mode == 1) {
        if (stream.version <= 2) {
            stream.SerialiseBuffer(&count, sizeof(count), 1);
            for (i32 i = 0; i < count; ++i) {
                EdRef *member = new (stream.secondary_buffer->Allocate(sizeof(EdRef))) EdRef;
                member->Serialise(stream, NULL);
                AddType(member);
            }
        }
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 i = 0; i < count; ++i) {
            EdRef *member = new (stream.secondary_buffer->Allocate(sizeof(EdRef))) EdRef;
            member->Serialise(stream, NULL);
            AddType(member);
        }
    }
    stream.EndBlock();
}

i32 EdClass::GetStreamClasses(EdStream &stream, i32 *classes, i32 &count, i32 capacity) {
    i32 result = 0;
    if (count < capacity) {
        classes[count] = theRegistry.GetClassId(this);
        ++count;
    }
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (member->attributes >= 0) {
            continue;
        }
        if ((stream.flags & 0x400000) != 0 ? (member->attributes & 0x400000) != 0
                                          : (member->attributes & 0x10000000) != 0) {
            continue;
        }
        theRegistry.GetClass(member->type_id)->GetStreamClasses(stream, classes, count, capacity);
        result = 1;
    }
    return result;
}

EdRef *EdClass::FindTypeRef(char *member_name, i32 recursive) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (recursive != 0 && member->attributes < 0) {
            EdRef *reference = theRegistry.GetClass(member->type_id)->FindTypeRef(member_name, 1);
            if (reference != NULL) {
                return reference;
            }
        } else if (NuStrICmp(member->name, member_name) == 0) {
            return member;
        }
    }
    return NULL;
}

void EdClass::SerialiseObject(EdStream &stream, void *object, EdClass *schema, EdRegistry *registry) {
    if (stream.BeginBlock("Object") == NULL) {
        return;
    }
    if (object != NULL) {
        u8 data[256];
        for (EdRef *source = schema->members; source != NULL; source = source->next) {
            EdRef *member = FindTypeRef(source->name, 0);
            if (member != NULL) {
                if (stream.mode != 1) {
                    if (stream.mode != 2) {
                        continue;
                    }
                    if ((stream.flags & 0x400000) != 0 ? (member->attributes & 0x400000) != 0
                                                      : (member->attributes & 0x10000000) != 0) {
                        continue;
                    }
                }
                if (member->attributes < 0) {
                    EdClass *source_class = registry->GetClass(source->type_id);
                    EdClass *member_class = theRegistry.GetClass(member->type_id);
                    member_class->SerialiseObject(stream, member->GetMemberObject(object), source_class,
                                                 registry);
                } else {
                    registry->GetType(source->type_id);
                    EdType *type = theRegistry.GetType(member->type_id);
                    i32 size = member->size > 0 ? member->size : type->size;
                    if (stream.mode == 2) {
                        member->GetMemberData(object, member->type_id, data, sizeof(data));
                    }
                    type->serialise(stream, data, size);
                    if (stream.mode == 1) {
                        member->SetMemberData(object, member->type_id, data, sizeof(data), NULL);
                    }
                }
            } else {
                EdType *type = registry->GetType(source->type_id);
                if (source->size <= 0) {
                    stream.Eat(type->size, 1);
                } else if (NuStrICmp(type->name, "String") != 0) {
                    stream.Eat(source->size, 1);
                } else {
                    i32 length;
                    stream.SerialiseBuffer(&length, sizeof(length), 1);
                    stream.Eat(length, 1);
                }
            }
        }
        if (interface != NULL) {
            interface->vtable->serialise_object(interface, stream, object);
        }
    }
    stream.EndBlock();
}

EdRef *EdClass::FindTypeRef(i32 attributes, i32 recursive) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            if (recursive != 0) {
                EdRef *reference = theRegistry.GetClass(member->type_id)->FindTypeRef(attributes, 1);
                if (reference != NULL) {
                    return reference;
                }
            }
        } else if ((member->attributes & attributes) != 0) {
            return member;
        }
    }
    return NULL;
}

i32 EdClass::FindMember(EdMember *result, void *object, i32 attributes, i32 recursive) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            if (recursive != 0) {
                EdClass *member_class = theRegistry.GetClass(member->type_id);
                void *member_object = member->GetMemberObject(object);
                if (member_class->FindMember(result, member_object, attributes, 1) != 0) {
                    return 1;
                }
            }
        } else if ((member->attributes & attributes) != 0) {
            result->object = object;
            result->reference = member;
            return 1;
        }
    }
    return 0;
}

void *EdClass::FindObject(char *object_name) {
    void *object = interface->vtable->get_next_object(interface, NULL);
    while (object != NULL) {
        EdMember member;
        i32 string_type = EdType_String;
        if (FindMember(&member, object, 2, 1) != 0) {
            char name_buffer[256];
            if (member.reference->GetAttributeData(member.object, 2, string_type, name_buffer,
                                                   sizeof(name_buffer)) != 0 &&
                NuStrICmp(object_name, name_buffer) == 0) {
                return object;
            }
        }
        object = interface->vtable->get_next_object(interface, object);
    }
    return NULL;
}

EditorSettings::EditorSettings() {
    cursor_radius = 1.0f;
    snap_terrain = 1;
}

void EditorSettings::AddMenuItems(eduimenu_s *) {
    STUBBED();
}

void EditorSettings::Serialise(EdStream &) {
    STUBBED();
}

eduimenu_s *edLevelNextMenu;

void BaseEditor::Initialise(variptr_u &, variptr_u &, i32 value) {
    field_0x0c = value;
}

void cbEdLevelSetSliderFloat(eduimenu_s *, eduiitem_s *item, u32) {
    *static_cast<f32 *>(item->data_ptr) = static_cast<edui_slider_s *>(item)->value;
}

void ClassEditor::RegisterTool(EdTool &tool) {
    tool.next = NULL;
    tool.previous = last_tool;
    if (last_tool != NULL) {
        last_tool->next = &tool;
    }
    EdTool *old_first = first_tool;
    last_tool = &tool;
    if (old_first == NULL) {
        first_tool = &tool;
    }
    ++tool_count;
}

void ClassEditor::UpdateSnapRay(VuVec &position) {
    if (snap_mode == 1) {
        snap_ray.x = 0.0f;
        snap_ray.y = -1000.0f;
        snap_ray.z = 0.0f;
    } else if (snap_mode == 2) {
        snap_ray = position;
    }
}

bool ClassObjectList::IsInList(ClassObject object) {
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        if (entry->object == object.object && entry->reference == object.reference) {
            return true;
        }
    }
    return false;
}

bool ClassObjectList::IsInList(EdClass *ed_class) {
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        if (entry->ed_class == ed_class) {
            return true;
        }
    }
    return false;
}

void LevelEditor::RegisterEditor(BaseEditor &editor) {
    editor.next = NULL;
    editor.previous = last_editor;
    if (last_editor != NULL) {
        last_editor->next = &editor;
    }
    BaseEditor *old_first = first_editor;
    last_editor = &editor;
    if (old_first == NULL) {
        first_editor = &editor;
    }
    ++editor_count;
}

nugscn_s *LevelEditor::GetScene(i32 index) {
    if (static_cast<u32>(index) < 10) {
        return scenes[index].scene;
    }
    return NULL;
}

void cbEdLevelSetSliderInt(eduimenu_s *, eduiitem_s *item, u32) {
    *static_cast<i32 *>(item->data_ptr) = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

void LevelEditor::SetNextMenu(eduimenu_s *menu) {
    edLevelNextMenu = menu;
}

bool PropertyMenu::ContainsObject(void *object) {
    for (i32 i = 0; i < object_count; ++i) {
        if (objects[i].object == object) {
            return true;
        }
    }
    return false;
}

void PropertyTool::SetMenuControl(eduimenu_s *menu, EdControl *control) {
    for (PropertyMenu *property_menu = active_menu; property_menu != NULL; property_menu = property_menu->next) {
        if (property_menu->menu == menu) {
            property_menu->control = control;
            return;
        }
    }
}

bool PropertyTool::HasActiveMenu() {
    return active_menu != NULL;
}
