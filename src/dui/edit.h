#ifndef DUI_EDIT_H
#define DUI_EDIT_H

#include <nob.h>

#include <dui/animation.h>
#include <dui/context.h>
#include <dui/layout.h>
#include <dui/types.h>

#define DUI_EDIT_BG(kind) DUI_SHADE_DISPATCH(kind, 950, 950, 800)
#define DUI_EDIT_BG_HOVER(kind) DUI_SHADE_DISPATCH(kind, 900,  900, 700)
#define DUI_EDIT_BG_DISABLED(kind) DUI_DEFAULT_COLOR(800)
#define DUI_EDIT_BG_SELECTION(kind) DUI_SHADE_DISPATCH(kind, 800,  800, 600)

#define DUI_EDIT_BORDER(kind) DUI_SHADE_DISPATCH(kind, 600, 600, 400)
#define DUI_EDIT_BORDER_DISABLED(kind) DUI_DEFAULT_COLOR(600)

#define DUI_EDIT_TEXT(kind) DUI_SHADE_DISPATCH(kind, 50, 50, 50)
#define DUI_EDIT_TEXT_DISABLED(kind) DUI_DEFAULT_COLOR(500)

#define DUI_EDIT_KEY_DELAY 0.02f
#define DUI_EDIT_BACK_SCROLL_STEP 50

typedef struct {
    DUI_LAYOUT_DATA_FIELDS;

    DUI_Kind kind;
    bool disabled;
    Nob_String_Builder* text;
} DUI_EditData;

typedef struct {
    DUI_Element element;

    size_t selection_start;
    int selection_length;

    DUI_ColorAnimation background;
    DUI_ColorAnimation border;
    DUI_State state;

    int scroll_offset;

    float left_key_state;
    float right_key_state;
    float backspace_key_state;
    float delete_key_state;
} DUI_EditElement;

#define dui_edit(...) dui_edit_impl(__COUNTER__, (DUI_EditData) { __VA_ARGS__ })
bool dui_edit_impl(int id, DUI_EditData data);

#endif // DUI_EDIT_H
