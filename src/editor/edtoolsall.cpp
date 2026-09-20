#include "decomp.h"
#include "editor/edpath.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {
    void pathEditorDrawPaths();
    void creatureEditor_RenderAllCreatures();
    void areaEditorDrawAreas();
    void locatorEditorDrawLocators();
    void antinodeEditorDrawAntinodes();
}

void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void routeEditor_Render(i32 x, i32 y, float xscale, float yscale) {
    if (aieditor->current_path != NULL) {
        NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 - 40, 16, "Edit Routes (Path = \"%s\")",
                     aieditor->current_path->name);
        NuQFntSetColour(system_qfont, 0x80000000);
        NuQFntSetScale(system_qfont, xscale, yscale);
        if (aieditor->current_path->current_route != NULL) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 120, 16, "\"%s\"",
                         aieditor->current_path->current_route->name);
        } else {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 120, 16, "NO ROUTES AVAILABLE");
        }
        if (aieditor->current_path->current_node != NULL) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 240, 16, "SQR - Sub menu");
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 360, 16, "SELECT - Select nearest");
            if (aieditor->current_path->nearest_node != NULL &&
                aieditor->current_path->nearest_node != aieditor->current_path->current_node) {
                NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 480, 16, "X - Select");
            }
            if (!(aieditor->flags & 1) && aieditor->current_path->nearest_node != NULL) {
                NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 720, 16, "O - Add/remove cnx to route.");
            }
        }
    }
    pathEditorDrawPaths();
    if (aieditorsettings.show_creatures_display) {
        creatureEditor_RenderAllCreatures();
    }
    areaEditorDrawAreas();
    locatorEditorDrawLocators();
    antinodeEditorDrawAntinodes();
}

void routeEditor_Process(nupad_s *) {
    STUBBED();
}

i32 InModelListDataFlags(APICHARACTERMODELLIST_s *, u32, u32, i32, i32) {
    STUBBED();
    return 0;
}

void antinodeEditor_Enter() {
    STUBBED();
}

void creatureEditor_Enter() {
    STUBBED();
}

void antinodeEditor_Render(i32, i32, float, float) {
    STUBBED();
}

void creatureEditor_Render(i32, i32, float, float) {
    STUBBED();
}

void antinodeEditor_Process(nupad_s *) {
    STUBBED();
}

void creatureEditor_Process(nupad_s *) {
    STUBBED();
}

void creatureEditor_CalculatePos(EDCREATURE_s *, i32, nuvec_s *, i32) {
    STUBBED();
}

void creatureEditor_IsSelectable(EDCREATURE_s *) {
    STUBBED();
}
