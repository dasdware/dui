#ifndef DUI_CHECKBOX_H
#define DUI_CHECKBOX_H

#include <dui/animation.h>
#include <dui/colors.h>
#include <dui/context.h>
#include <dui/layout.h>
#include <dui/types.h>

#define DUI_CHECKBOX_TEXT(kind) DUI_SHADE_DISPATCH(kind, 500, 500, 400)
#define DUI_CHECKBOX_TEXT_HOVER(kind) DUI_SHADE_DISPATCH(kind, 600, 600, 500)
#define DUI_CHECKBOX_TEXT_DOWN(kind) DUI_SHADE_DISPATCH(kind, 700, 700, 600)
#define DUI_CHECKBOX_TEXT_DISABLED(kind) DUI_DEFAULT_COLOR(600)

#define DUI_CHECKBOX_BOX(kind) DUI_SHADE_DISPATCH(kind, 600, 600, 500)
#define DUI_CHECKBOX_BOX_HOVER(kind) DUI_SHADE_DISPATCH(kind, 700, 700, 600)
#define DUI_CHECKBOX_BOX_DOWN(kind) DUI_SHADE_DISPATCH(kind, 800, 800, 700)
#define DUI_CHECKBOX_BOX_DISABLED(kind) DUI_DEFAULT_COLOR(700)

#define DUI_CHECKBOX_CHECK(kind) DUI_SHADE_DISPATCH(kind, 100, 100, 100)

typedef DUI_PaletteKind DUI_CheckboxKind;

typedef enum
{
    CHECK_UNCHECKED,
    CHECK_CHECKED,
    CHECK_GRAYED,
} DUI_CheckState;

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;

    const char* caption;
    DUI_CheckboxKind kind;
    bool* checked;
    bool disabled;
} DUI_CheckboxData;

typedef struct
{
    DUI_Element element;

    DUI_ColorAnimation text_color;
    DUI_ColorAnimation box_color;
    DUI_State state;
    DUI_CheckState check_state;
} DUI_CheckboxElement;

#define dui_checkbox(...) dui_checkbox_impl(__COUNTER__, (DUI_CheckboxData) { __VA_ARGS__ })
bool dui_checkbox_impl(int id, DUI_CheckboxData data);

#endif // DUI_CHECKBOX_H