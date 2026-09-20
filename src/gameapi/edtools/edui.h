#pragma once

#include "nu2api/nucore/common.h"

struct eduimenu_s;
struct eduiitem_s;
struct nupad_s;
struct edui_interact_s;
struct eduiiattr_s {
    u32 background;
    u32 text;
    u32 highlight;
    u32 disabled;
};
struct nugraph_s;
typedef void (*EdUiMenuCallback)(eduimenu_s *menu, eduimenu_s *parent);
typedef void (*EdUiItemCallback)(eduimenu_s *menu, eduiitem_s *item, u32 value);

enum EdUiItemFlags {
    EDUI_ITEM_HIGHLIGHTED = 0x01,
    EDUI_ITEM_DISABLED = 0x02,
};

enum EdUiItemType {
    EDUI_ITEM_TOGGLE = 3,
    EDUI_ITEM_EXPANDER = 19,
};

enum EdUiExpanderFlags {
    EDUI_EXPANDER_OPEN = 1,
};

enum EdUiAnalogPad {
    EDUI_ANALOG_PAD_NONE = 0,
    EDUI_ANALOG_PAD_LEFT = 1,
    EDUI_ANALOG_PAD_RIGHT = 2,
};

enum EdUiCursorButtons {
    EDUI_CURSOR_SECONDARY = 0x10,
    EDUI_CURSOR_PRIMARY = 0x40,
};

// Fields not yet traced retain their original byte ranges.
struct eduiitem_s {
    eduiitem_s *next;
    eduiitem_s *previous;
    i32 type;
    union {
        i32 data;
        void *data_ptr;
        u8 unknown_0c[4];
    };
    u8 unknown_10;
    union {
        u8 flags;
        struct {
            u8 highlighted : 1;
            u8 disabled : 1;
            u8 unknown_flags : 6;
        };
    };
    i8 text_alignment;
    u8 unknown_13[5];
    i32 selection_group;
    i32 x;
    i32 y;
    char *text;
    u32 colours[4];
    i32 (*input)(eduimenu_s *, eduiitem_s *, u32, u32);
    i32 (*process)(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    i32 (*render)(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    void (*destroy)(eduimenu_s *, eduiitem_s *);
};

struct eduimenu_s {
    eduiitem_s *first;
    eduiitem_s *last;
    eduiitem_s *selected;
    eduiitem_s *field_0c;
    eduiitem_s *field_10;
    i32 x;
    i32 y;
    i32 width;
    i32 height;
    i32 field_24;
    i32 field_28;
    void *font;
    EdUiMenuCallback callback;
    char *title;
    u32 flags : 3;
    u32 unknown_flags : 29;
    eduimenu_s *child;
    eduimenu_s *parent;
};

struct ed_module_s {
    ed_module_s *next;
    ed_module_s *previous;
    const char *name;
    void (*init)();
    void (*close)();
    void (*activate)();
    void (*deactivate)();
    void (*apply)();
    void (*write)(i32 file);
    void (*read)(i32 file);
    u32 block_id;
    i32 (*process)(f32 delta_time, nupad_s *pad);
    void (*render)();
    void *reserved;
};

struct edui_slider_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    void (*changed)(eduimenu_s *, eduiitem_s *, u32);
    f32 normalized_value;
    f32 value;
    f32 minimum;
    f32 range;
    i32 change_timer;
    char *format;
    f32 granularity;
};

struct edui_sel_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    EdUiItemCallback selected;
    EdUiItemCallback held;
};

struct edui_text_selector_s : edui_slider_s {
    char **options;
};

struct edui_colour_slider_s : edui_slider_s {
    u8 red;
    u8 green;
    u8 blue;
    u8 unknown_6f;
};

struct edui_colour_pick_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    f32 cursor_x;
    f32 cursor_y;
    f32 hue;
    f32 saturation;
    f32 value;
    u8 unknown_60[0x10];
    EdUiItemCallback changed;
};

struct edui_texture_pick_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    f32 uv_x[2];
    f32 uv_y[2];
    EdUiItemCallback changed;
    u8 unknown_60[4];
    i32 selected_corner;
    f32 zoom;
};

struct edui_expander_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    eduiitem_s *first_child;
    eduiitem_s *last_child;
    u32 open : 1;
    u32 unknown_flags : 31;
    u8 unknown_58[0xc];
    i32 depth;
    EdUiItemCallback changed;
};

struct edui_graph_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    nugraph_s *graph;
    nugraph_s *onion_skins[8];
    EdUiItemCallback changed;
    f32 cursor_x;
    f32 cursor_y;
    i32 width;
    i32 height;
    f32 x_scale;
    f32 y_scale;
    i32 selected_point;
    char x_label[16];
    char y_label[16];
    char title[16];
};

struct edui_file_pick_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    EdUiItemCallback changed;
    char *format;
    u8 unknown_54[4];
    char name[0x40];
    char directory[0x100];
    char filename[0x108];
    void *directory_list;
};

struct edui_prop_s : eduiitem_s {
    u8 unknown_48[0x1c];
    char *property_text;
    u8 unknown_68[0x18];
};

struct edui_filter_s : edui_prop_s {
    eduiitem_s *first_child;
    u8 unknown_84[8];
};

#ifndef HOST_BUILD
static_assert(sizeof(eduiitem_s) == 0x48, "eduiitem_s ABI");
static_assert(offsetof(eduiitem_s, colours) == 0x28, "eduiitem_s colours ABI");
static_assert(offsetof(eduiitem_s, input) == 0x38, "eduiitem_s input ABI");
static_assert(sizeof(edui_sel_s) == 0x54, "edui_sel_s ABI");
static_assert(sizeof(edui_slider_s) == 0x6c, "edui_slider_s ABI");
static_assert(sizeof(edui_colour_pick_s) == 0x74, "edui_colour_pick_s ABI");
static_assert(sizeof(edui_texture_pick_s) == 0x6c, "edui_texture_pick_s ABI");
static_assert(sizeof(edui_expander_s) == 0x6c, "edui_expander_s ABI");
static_assert(sizeof(edui_graph_s) == 0xc0, "edui_graph_s ABI");
static_assert(sizeof(edui_file_pick_s) == 0x2a4, "edui_file_pick_s ABI");
static_assert(sizeof(edui_filter_s) == 0x8c, "edui_filter_s ABI");
#endif

struct edui_interact_s {
    f32 x, y, width, height;
    eduimenu_s *menu;
    eduiitem_s *item;
    i32 (*callback)(edui_interact_s *);
    u32 buttons;
    i32 field_20;
};

extern "C" {
    extern ed_module_s edptldesc;
    extern ed_module_s edgradesc;
    extern ed_module_s edbridesc;
    extern ed_module_s edanimdesc;
    i32 edmainActivate(ed_module_s *module, i32 notify);
    ed_module_s *edmainCurrent(void);
    i32 edmainRegister(ed_module_s *module);
    void edmainClose(void);
    eduimenu_s *edGetMainMenu(void);
    void eduiGetCursorCoords(f32 *x, f32 *y);
    void eduiSetCursorCoords(f32 x, f32 y);
    void eduiGetCursorDelta(f32 *x, f32 *y);
    void eduiProcessCursor(f32 delta_time, nupad_s *pad);
    void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
    i32 eduiUsedAlgPad(nupad_s *pad);
    extern i32 bUsingMenuFocus;
    i32 eduiGetUsingMenuFocus(void);
    void eduiSetUsingMenuFocus(i32 enabled);
    eduimenu_s *eduiGetActiveMenu(void);
    eduimenu_s *eduiGetActiveMenuParent(void);
    eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
    void eduiSetActiveMenu(eduimenu_s *menu);
    void eduiSetDefaultActiveMenu(eduimenu_s *menu);
    i32 eduiClearActiveMenu(void);
    i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
    i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
    i32 eduiMenuDetach(eduimenu_s *menu);
    i32 eduiMenuIsActive(eduimenu_s *menu);
    void eduiMenuEnsureSelection(eduimenu_s *menu);
    void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
    void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
    eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
    eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
    eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
    eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 maximum, f32 value, char *text);
    eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 maximum, i32 value, char *text);
    eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
    i32 eduiItemSetText(eduiitem_s *item, char *text);
    i32 eduiItemPropSetText(edui_prop_s *item, char *text);
    void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
    void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
    void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
    void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
    void eduiMenuDestroy(eduimenu_s *menu);
    void eduiMenuDestroyItems(eduimenu_s *menu);
    void eduicbMenuCloseAllexpanders(eduimenu_s *menu);
    void eduicbMenuOpenAllexpanders(eduimenu_s *menu);
    void eduiMenuSelectFirstEntry(eduimenu_s *menu);
    i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
    i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
    i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
    void eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
    i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
    void eduiFlushInteracts(void);
    i32 eduiCursorOverMenu(eduimenu_s *menu);
    i32 cbInteractMenuTitle(edui_interact_s *interact);
    i32 cbInteractMenuScrollUp(edui_interact_s *interact);
    i32 cbInteractMenuScrollDown(edui_interact_s *interact);
    void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
    void cbInteractMenuKeySelect(eduimenu_s *menu);
    eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback, char *title);
    extern eduiitem_s *edui_last_item;
    eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
    void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
    void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
    void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
    void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
    void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
    i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
    i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
    void eduiMenuRender(eduimenu_s *menu);
    void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
    void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
    void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
    void eduiMenuSortItemsByTxt(eduimenu_s *menu);
    void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
    f32 eduiGetAnalougePadValue(nupad_s *pad);
    void eduiSetGlobalSliderAccel(f32 acceleration);
}
