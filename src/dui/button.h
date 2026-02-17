//
// Created by thomas on 04.01.2026.
//

#ifndef DUI_BUTTON_H
#define DUI_BUTTON_H

#include <dui/animation.h>
#include <dui/colors.h>
#include <dui/context.h>
#include <dui/layout.h>
#include <dui/types.h>

// button theming
#define DUI_BUTTON_BG(kind) DUI_SHADE_DISPATCH(kind, 800, 800, 600)
#define DUI_BUTTON_BG_HOVER(kind) DUI_SHADE_DISPATCH(kind, 900,  900, 700)
#define DUI_BUTTON_BG_DOWN(kind) DUI_SHADE_DISPATCH(kind, 950, 950, 800)
#define DUI_BUTTON_BG_DISABLED(kind) DUI_DEFAULT_COLOR(600)

#define DUI_BUTTON_FG(kind) DUI_SHADE_DISPATCH(kind, 50, 50, 50)
#define DUI_BUTTON_FG_DISABLED(kind) DUI_DEFAULT_COLOR(400)

typedef DUI_PaletteKind DUI_ButtonKind;

typedef struct
{
    DUI_LAYOUT_DATA_FIELDS;

    const char* caption;
    DUI_ButtonKind kind;
    bool disabled;
} DUI_ButtonData;

typedef struct
{
    DUI_Element element;

    DUI_ColorAnimation background;
    DUI_ColorAnimation foreground;
    DUI_State state;
} DUI_ButtonElement;

#define dui_button(...) dui_button_impl(__COUNTER__, (DUI_ButtonData) { __VA_ARGS__ })
bool dui_button_impl(int id, DUI_ButtonData data);

#endif //DUI_BUTTON_H
